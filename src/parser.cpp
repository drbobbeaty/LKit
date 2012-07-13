/**
 * parser.cpp - this file implements a simple lisp-style language parser where
 *              the values, functions and expressions are defined in the
 *              language/classes of LKit. This will take a std::string
 *              as the "source" code and "compile" it into an execution
 *              tree that can be calculated as many times as needed. The
 *              goal is to use smart (reduced) updates in the tree to make
 *              the updating as simple and efficient as possible.
 */

//	System Headers
#include <ctype.h>
#include <stdlib.h>

//	Third-Party Headers
#include <boost/foreach.hpp>

//	Other Headers
#include "parser.h"
#include "base_functions.h"
#include "expression.h"
#include "util/timer.h"

//	Forward Declarations

//	Private Constants

//	Private Datatypes

//	Private Data Constants



/**
 * Make it easy to reference the spinlock and it's scoped lock. They
 * are both in the boost::detail namespace, and it's just going to make
 * the code a little cleaner.
 */
using namespace boost::detail;

namespace lkit {
/*******************************************************************
 *
 *                     Constructors/Destructor
 *
 *******************************************************************/
/**
 * This is the default constructor that assumes NOTHING - it just
 * makes a simple parser ready to be loaded with some kind of
 * source code in the setter and then executed to come up with the
 * different possible values of that code.
 */
parser::parser() :
	_src(),
	_src_mutex(),
	_fcns(),
	_fcns_mutex(),
	_vars(),
	_vars_mutex(),
	_const(),
	_const_mutex(),
	_subs(),
	_subs_mutex(),
	_expr(NULL)
{
	// reset the parser for the star of something neat
	reset();
}


/**
 * This is a general constructor of a parser - it takes a source
 * code string and parses it into a language tree for evaluation.
 */
parser::parser( const std::string & aSource ) :
	_src(aSource),
	_src_mutex(),
	_fcns(),
	_fcns_mutex(),
	_vars(),
	_vars_mutex(),
	_const(),
	_const_mutex(),
	_subs(),
	_subs_mutex(),
	_expr(NULL)
{
	// reset the parser for the star of something neat
	if (reset()) {
		// try to compile the source
		compile();
	}
}


/**
 * This is the standard copy constructor that needs to be in every
 * class to make sure that we control how many copies we have
 * floating around in the system.
 */
parser::parser( const parser & anOther ) :
	_src(),
	_src_mutex(),
	_fcns(),
	_fcns_mutex(),
	_vars(),
	_vars_mutex(),
	_const(),
	_const_mutex(),
	_subs(),
	_subs_mutex(),
	_expr(NULL)
{
	// let the '=' operator do the heavy lifting...
	*this = anOther;
}


/**
 * This is the standard destructor and needs to be virtual to make
 * sure that if we subclass off this, the right destructor will be
 * called.
 */
parser::~parser()
{
	// clear out everything we're responsible for
	clear();
}


/**
 * When we process the result of an equality we need to make sure
 * that we do this right by always having an equals operator on
 * all classes.
 */
parser & parser::operator=( const parser & anOther )
{
	if (this != & anOther) {
		// first, reset this instance to the ground state
		reset();
		// start with the source for the parser - if there is anything
		_src = anOther._src;
	}
	return *this;
}


/*******************************************************************
 *
 *                        Accessor Methods
 *
 *******************************************************************/
/**
 * This method wipes out the old source code of this parser and
 * replaces it with the supplied code. It then compiles this code
 * into a language tree and prepares it for evaluation.
 */
void parser::setSource( const std::string & aSource )
{
	spinlock::scoped_lock		lock(_src_mutex);
	_src = aSource;
	// if we have an expression, then drop it
	if (_expr != NULL) {
		delete _expr;
		_expr = NULL;
	}
}


/**
 * This method returns the currently active source code that is
 * compiled into the language tree for current evaluation.
 */
std::string parser::getSource() const
{
	spinlock::scoped_lock		lock(_src_mutex);
	std::string	retval = _src;
	return retval;
}


/**
 * This method returns the actual map of variables that are
 * defined for this parser. These can be default variables like
 * pi, 'e', and such, but they can also be extracted from the
 * source code and require values from the caller in order to
 * parse properly. There can even be variables given to this
 * parser by the caller that MIGHT be used, but may not be.
 *
 * The memory management of the values is done by the parser,
 * so do NOT attempt to clear out any of the values in the map.
 */
const var_map_t & parser::getVariables() const
{
	return _vars;
}


/**
 * These methods allow the caller to add a variable to the
 * parser as either a complete variable, or a name and a value.
 * This can also be used to update the value of a variable in
 * the parser - as might be the case of variables extracted
 * from the parsing of the source code.
 *
 * The variable and value pointers will be memory managed by
 * the parser from this point on, so the caller must give
 * over control to the parser for these arguments.
 */
bool parser::addVariable( const variable & aVariable )
{
	bool		error = false;

	spinlock::scoped_lock		lock(_vars_mutex);
	// see if we already have a variable with this name
	std::string		name = aVariable.getName();
	variable	*old = _vars[name];
	if (old != NULL) {
		// assign the new value to the existing variable
		*old = aVariable;
	} else {
		// place a clone of the variable in the map
		_vars[name] = (variable *)aVariable.clone();
	}

	return !error;
}


bool parser::addVariable( variable * & aVariable )
{
	bool		error = false;
	if (aVariable == NULL) {
		error = true;
	} else {
		spinlock::scoped_lock		lock(_vars_mutex);
		// see if we already have a variable with this name
		std::string		name = aVariable->getName();
		variable	*old = _vars[name];
		if (old != NULL) {
			// assign the new value to the existing variable
			*old = *aVariable;
			delete aVariable;
			aVariable = old;
		} else {
			// place the new variable into the map
			_vars[name] = aVariable;
		}
	}
	return !error;
}


bool parser::addVariable( const std::string & aName, value * & aValue )
{
	bool		error = false;
	if (aValue == NULL) {
		error = true;
	} else {
		spinlock::scoped_lock		lock(_vars_mutex);
		// see if we already have a variable with this name
		variable	*old = _vars[aName];
		if (old != NULL) {
			// assign the new value to the existing variable
			*old = *aValue;
			delete aValue;
			aValue = old;
		} else {
			// place the new value into the map
			_vars[aName] = new variable(aName, aValue);
		}
	}
	return !error;
}


bool parser::addVariable( const std::string & aName, const value & aValue )
{
	bool		error = false;

	spinlock::scoped_lock		lock(_vars_mutex);
	// see if we already have a variable with this name
	variable	*old = _vars[aName];
	if (old != NULL) {
		// assign the new value to the existing variable
		*old = aValue;
	} else {
		// place a new variable into the map
		_vars[aName] = new variable(aName, aValue.clone());
	}

	return !error;
}


/**
 * This method returns the pointer to the value for the
 * provided variable name. The returned pointer is the
 * ACTUAL value pointer in the table for this parser, so
 * please exercise care when modifying it.
 */
const value *parser::getVariable( const std::string & aName )
{
	value		*retval = NULL;
	spinlock::scoped_lock		lock(_vars_mutex);
	// try to find the variable for the provided name
	var_map_t::iterator	it = _vars.find(aName);
	if (it != _vars.end()) {
		retval = it->second;
	}
	// return what we might have found
	return retval;
}


/**
 * These methods allow the user to remove a variable from
 * the parser which can be a bit dicey proposition, if you're
 * not careful. If it's a variable that's been added by the
 * caller, and NOT required, then removing it is OK. But if
 * it's required, then no matter how it got there, you're
 * going to be sad if you remove it. So use with care.
 */
bool parser::removeVariable( std::string & aName )
{
	bool		removed = false;
	spinlock::scoped_lock		lock(_vars_mutex);
	// try to find the variable for the provided name
	var_map_t::iterator	it = _vars.find(aName);
	if (it != _vars.end()) {
		if (it->second != NULL) {
			delete it->second;
		}
		// NULL or not, we need to remove it from the map
		_vars.erase(it);
		removed = true;
	}
	return removed;
}


/**
 * This method simple clears out all the variables in this
 * parser instance. It's a clean-slate starting point.
 */
void parser::clearVariables()
{
	spinlock::scoped_lock		lock(_vars_mutex);
	// scan the map and delete everything that's there
	value		*v = NULL;
	for (var_map_t::iterator it = _vars.begin(); it != _vars.end(); ++it) {
		if ((v = it->second) != NULL) {
			delete v;
		}
	}
	// now we can clear out the map as everything is deleted
	_vars.clear();
}


/**
 * This method provides the parser with a series of default
 * variables for the expressions. These are values most likely
 * to be used by standard mathematical expressions.
 */
void parser::useDefaultVariables()
{
	// these are the default variables
	addVariable("e", 2.71828183);
	addVariable("pi", 3.14159265);
}


/**
 * This method returns the actual reference to the map of known
 * functions for this parser. Great care should be exercised, as
 * this is the ACTUAL map, and can be modified by other threads
 * so please be careful. The key is the name of the function as
 * it exists in the source, and the value is the pointer to the
 * lkit::function that is used.
 */
const fcn_map_t & parser::getFunctions() const
{
	return _fcns;
}


/**
 * This method adds the provided name and function to the list
 * of functions for this parser. Checks will be made to see if
 * this name has already been used, and if so, the argument
 * will replace the existing function in the table. This allows
 * for custom functionality under the same name in the scripts.
 *
 * The memory management of the functions will become the
 * responsible of the parser, so the caller has to be willing
 * to relinquish control.
 */
bool parser::addFunction( const std::string & aName, function *aFunction )
{
	bool		error = false;
	if (aFunction == NULL) {
		error = true;
	} else {
		spinlock::scoped_lock		lock(_fcns_mutex);
		// see if we already have a function with this name
		function	*old = _fcns[aName];
		if (old != NULL) {
			delete old;
			old = NULL;
		}
		// place the new function into the map
		_fcns[aName] = aFunction;
	}
	return !error;
}


/**
 * This method will look to see if the provided function name
 * is in the list for this parser, and will remove it if so.
 * If one is removed, this method will return 'true', otherwise,
 * it will return 'false'.
 */
bool parser::removeFunction( const std::string & aName )
{
	spinlock::scoped_lock		lock(_fcns_mutex);
	// based on how many are erased, return the right flag
	return (_fcns.erase(aName) > 0);
}


/**
 * This method will remove ALL the known functions from the
 * table for this parser.
 */
void parser::clearFunctions()
{
	spinlock::scoped_lock		lock(_fcns_mutex);
	// scan the map and delete everything that's there
	function	*f = NULL;
	for (fcn_map_t::iterator it = _fcns.begin(); it != _fcns.end(); ++it) {
		if ((f = it->second) != NULL) {
			delete f;
		}
	}
	// now we can clear out the map as everything is deleted
	_fcns.clear();
}


/**
 * This method will set up this parser with the DEFAULT functions
 * for the language. This does not include any user-supplied
 * functions, but does include the basic functionality of the
 * language.
 */
void parser::useDefaultFunctions()
{
	// these are the default functions
	addFunction("max", new func::max());
	addFunction("min", new func::min());
	addFunction("+", new func::sum());
	addFunction("-", new func::diff());
	addFunction("*", new func::prod());
	addFunction("/", new func::quot());
	addFunction("==", new func::comp(func::comp::eEquals));
	addFunction("!=", new func::comp(func::comp::eNotEquals));
	addFunction("<", new func::comp(func::comp::eLessThan));
	addFunction(">", new func::comp(func::comp::eGreaterThan));
	addFunction("<=", new func::comp(func::comp::eLessOrEqual));
	addFunction(">=", new func::comp(func::comp::eGreaterOrEqual));
	addFunction("and", new func::bin(func::bin::eAnd));
	addFunction("or", new func::bin(func::bin::eOr));
	addFunction("not", new func::bin(func::bin::eNot));
}


/**
 * This method attempts to compile the source, if there is any, and
 * it's not already compiled into a language tree, and then evaluates
 * the final expression, and all sub-expressions and functions to
 * come up with a final value of the result. Everything is left in
 * place so that additional evaluations are done as quickly as
 * possible.
 */
value parser::eval()
{
	value		v;
	// make sure it's compiled and ready to go
	if (compile()) {
		spinlock::scoped_lock		lock(_src_mutex);
		// if we have an expression, then eval it for returning
		if (_expr != NULL) {
			v = _expr->eval();
		}
	}
	return v;
}


/**
 * This method will clear out EVERYTHING for the parser and have
 * it start as a "blank slate". This is not necessarily the state
 * you want to use the parser in, as there's nothing there, but it
 * is an important reference point that we need to be able to
 * return to from time to time.
 */
void parser::clear()
{
	// ...just assign the empty string to the source
	setSource(std::string());
	// clear out everything we have - functions, variables and such
	clearFunctions();
	clearVariables();
	clearConsts();
	clearSubExpr();
}


/**
 * This method returns the parser to a "new start" state. It clears
 * out everything and then adds back in the default variables and
 * functions so that it's all ready to take your source and compile
 * it into an executable language tree.
 */
bool parser::reset()
{
	// first clear everything out...
	clear();
	// ...and then load back int he default functions and variables
	useDefaultFunctions();
	useDefaultVariables();
	return true;
}


/*******************************************************************
 *
 *                         Utility Methods
 *
 *******************************************************************/
/**
 * There are a lot of times that a human-readable version of
 * this instance will come in handy. This is that method. It's
 * not necessarily meant to be something to process, but most
 * likely what a debugging system would want to write out for
 * this guy.
 */
std::string parser::toString() const
{
	return "<parser>";
}


/**
 * When we have a custom '==' operator, it's wise to have a hash
 * method as well. This makes it much easier to used hash-based
 * containers like boost, etc. It's a requirement that if two
 * instances are '==', then their hash() methods must be '==' as
 * well.
 */
size_t parser::hash() const
{
	size_t	ans = 0;
	{
		spinlock::scoped_lock	lock(_src_mutex);
		ans = boost::hash_value(_src);
	}
	{
		spinlock::scoped_lock	lock(_const_mutex);
		BOOST_FOREACH( value *v, _const ) {
			if (v != NULL) {
				boost::hash_combine(ans, v->hash());
			}
		}
	}
	{
		spinlock::scoped_lock	lock(_vars_mutex);
		BOOST_FOREACH( var_map_t::value_type i, _vars ) {
			if (i.second != NULL) {
				boost::hash_combine(ans, i.first);
				boost::hash_combine(ans, i.second->hash());
			}
		}
	}
	{
		spinlock::scoped_lock	lock(_fcns_mutex);
		BOOST_FOREACH( fcn_map_t::value_type i, _fcns ) {
			if (i.second != NULL) {
				boost::hash_combine(ans, i.first);
				boost::hash_combine(ans, i.second->hash());
			}
		}
	}
	return ans;
}


/**
 * This method checks to see if two parsers are equal in their
 * contents and not their pointer values. This is how you'd likely
 * expect equality to work.
 */
bool parser::operator==( const parser & anOther ) const
{
	bool		equals = true;
	// start by seeing if we're looking at ourselves...
	bool		keepChecking = !(this == & anOther);

	// if it's not us, then check the source code...
	if (keepChecking) {
		spinlock::scoped_lock	lock(_src_mutex);
		if (_src != anOther._src) {
			equals = false;
			keepChecking = false;
		}
	}

	// check the constants
	if (keepChecking) {
		spinlock::scoped_lock	lock(_const_mutex);
		if (_const.size() != anOther._const.size()) {
			equals = false;
			keepChecking = false;
		} else {
			for (size_t i = 0; i < _const.size(); ++i) {
				if (_const[i] == NULL) {
					// I'm NULL, he's not - that's not equal
					if (anOther._const[i] != NULL) {
						equals = false;
						keepChecking = false;
						break;
					}
				} else {
					// he's NULL, I'm not - that's not equal
					if (anOther._const[i] == NULL) {
						equals = false;
						keepChecking = false;
						break;
					}
					// we are not NULLs, so check equality
					if (_const[i]->operator!=((const value &)*(anOther._const[i]))) {
						equals = false;
						keepChecking = false;
						break;
					}
				}
			}
		}
	}

	// next, let's check the variables...
	if (keepChecking) {
		var_map_t::const_iterator	it;
		spinlock::scoped_lock	lock(_vars_mutex);
		// check to make sure the number of variables match...
		if (_vars.size() != anOther._vars.size()) {
			equals = false;
			keepChecking = false;
		} else {
			// then look up each to make sure it matches
			BOOST_FOREACH( var_map_t::value_type i, _vars ) {
				// try to find the variable in the 'other' guy...
				if ((it = anOther._vars.find(i.first)) == anOther._vars.end()) {
					equals = false;
					keepChecking = false;
					break;
				}
				// ...and then check the values
				if (i.second == NULL) {
					// I'm NULL, he's not - that's not equal
					if (it->second != NULL) {
						equals = false;
						keepChecking = false;
						break;
					}
				} else {
					// he's NULL, I'm not - that's not equal
					if (it->second == NULL) {
						equals = false;
						keepChecking = false;
						break;
					}
					// we are not NULLs, so check equality
					if (i.second->operator!=((const value &)*(it->second))) {
						equals = false;
						keepChecking = false;
						break;
					}
				}
			}
		}
	}

	// next, let's check the functions...
	if (keepChecking) {
		fcn_map_t::const_iterator	it;
		spinlock::scoped_lock	lock(_fcns_mutex);
		// check to make sure the number of functions match...
		if (_fcns.size() != anOther._fcns.size()) {
			equals = false;
			keepChecking = false;
		} else {
			// then look up each to make sure it matches
			BOOST_FOREACH( fcn_map_t::value_type i, _fcns ) {
				// try to find the function in the 'other' guy...
				if ((it = anOther._fcns.find(i.first)) == anOther._fcns.end()) {
					equals = false;
					keepChecking = false;
					break;
				}
				// ...and then check the values
				if (i.second == NULL) {
					// I'm NULL, he's not - that's not equal
					if (it->second != NULL) {
						equals = false;
						keepChecking = false;
						break;
					}
				} else {
					// he's NULL, I'm not - that's not equal
					if (it->second == NULL) {
						equals = false;
						keepChecking = false;
						break;
					}
					// we are not NULLs, so check equality
					if (i.second->operator!=(*(it->second))) {
						equals = false;
						keepChecking = false;
						break;
					}
				}
			}
		}
	}

	return equals;
}


/**
 * This method checks to see if two parsers are NOT equal in their
 * contents and not their pointer values. This is how you'd likely
 * expect equality to work.
 */
bool parser::operator!=( const parser & anOther ) const
{
	return !operator==(anOther);
}


/*******************************************************************
 *
 *                     Protected Accessor Methods
 *
 *******************************************************************/
/**
 * This method looks to the map of all variables we know about in
 * this parser instance and will attempt to find the one registered
 * under the name provided. If one does exist under that name, then
 * we'll return it for use in an expression. If not, we'll create
 * a location for one in the map, and it'll be up to the caller to
 * assign a value to that variable before evaluation starts.
 */
value *parser::lookUpVariable( const std::string & aName )
{
	// lock this map up so we can see what's there safely
	spinlock::scoped_lock		lock(_vars_mutex);
	// see if we already have a variable with this name
	variable	*v = _vars[aName];
	if (v == NULL) {
		// create it with the right variable name
		if ((v = new variable(aName)) == NULL) {
			throw std::runtime_error("[parser::lookUpVariable] unable to create new placeholder variable for provided name");
		}
		// place the new variable into the map
		_vars[aName] = v;
	}
	// return what we have now
	return v;
}


/**
 * This method looks to the map of all functions we know about in
 * this parser instance and will attempt to find the one registered
 * under the name provided. If one does exist under that name, then
 * we'll return it for use in an expression. If not, we'll return
 * NULL and that should be the sign of an error. Functions have to
 * be defined prior to parsing of the code.
 */
function *parser::lookUpFunction( const std::string & aName )
{
	// lock this map up so we can see what's there safely
	spinlock::scoped_lock		lock(_fcns_mutex);
	// see if we have a function with this name
	function	*f = NULL;
	fcn_map_t::const_iterator	it = _fcns.find(aName);
	if (it != _fcns.end()) {
		f = it->second;
	}
	// return what we have now
	return f;
}


/**
 * This method returns the actual reference to the map of known
 * constants for this parser. Great care should be exercised, as
 * this is the ACTUAL list, and can be modified by other threads
 * so please be careful.
 */
const var_list_t & parser::getConsts() const
{
	return _const;
}


/**
 * This method adds the provided value to the list of constants
 * for this parser. No checks will be made to see if this value
 * already exists as it's a constant. Period.
 *
 * The memory management of the functions will become the
 * responsible of the parser, so the caller has to be willing
 * to relinquish control.
 */
bool parser::addConst( value *aConstant )
{
	spinlock::scoped_lock		lock(_const_mutex);
	_const.push_back(aConstant);
	return true;
}


/**
 * This method will remove ALL the known constants from the
 * list for this parser.
 */
void parser::clearConsts()
{
	spinlock::scoped_lock		lock(_const_mutex);
	// delete everything from the list that's not NULL...
	for (var_list_t::iterator it = _const.begin(); it != _const.end(); ++it) {
		if (*it != NULL) {
			delete (*it);
		}
	}
	// now we can clear out the list as everything is deleted
	_const.clear();
}


/**
 * This method returns the actual reference to the map of known
 * sub-expressions for this parser. Great care should be exercised,
 * as this is the ACTUAL list, and can be modified by other threads
 * so please be careful.
 */
const expr_list_t & parser::getSubExpr() const
{
	return _subs;
}


/**
 * This method adds the provided expression to the list of
 * sub-expressions for this parser. No checks will be made to see
 * if this expression already exists. Period.
 *
 * The memory management of the expressions will become the
 * responsible of the parser, so the caller has to be willing
 * to relinquish control.
 */
bool parser::addSubExpr( expression *anExpression )
{
	spinlock::scoped_lock		lock(_subs_mutex);
	_subs.push_back(anExpression);
	return true;
}


/**
 * This method will remove ALL the known subexpressions from the
 * list for this parser.
 */
void parser::clearSubExpr()
{
	spinlock::scoped_lock		lock(_subs_mutex);
	// delete everything from the list that's not NULL...
	for (expr_list_t::iterator it = _subs.begin(); it != _subs.end(); ++it) {
		if (*it != NULL) {
			delete (*it);
		}
	}
	// now we can clear out the list as everything is deleted
	_subs.clear();
}


/*******************************************************************
 *
 *                   Compiling/Evaluation Methods
 *
 *******************************************************************/
/**
 * This method compiles the source code in this parser into a
 * language tree for evaluation. This will replace the existing
 * language tree with this new version, so it's a destructive
 * operation in that regard.
 */
bool parser::compile()
{
	bool		error = false;
	// make sure they don't change the source while we work...
	spinlock::scoped_lock		lock(_src_mutex);
	// see if we need to compile anything at all
	if (_expr == NULL) {
		uint32_t	pos = _src.find('(');
		if (pos < _src.length()) {
			if ((_expr = parseExpr(_src, pos)) == NULL) {
				error = true;
			}
		} else {
			// no start of an expression - bad news
			error = true;
		}
	}
	return !error;
}


/**
 * This method assumes that the string starting at position
 * 'aPos' is the start of an expression, and will build up
 * the necessary expression and move the position of the
 * 'cursor' as the parsing continues. This will be called
 * recursively to generate the complete, final expression for
 * the code starting at this point.
 */
value *parser::parseExpr( const std::string & aSrc, uint32_t & aPos )
{
	/**
	 * See if we start this off right, and of not, then toss
	 * an error that can be caught. If so, then create an
	 * expression to build upon.
	 */
	value		*expr = NULL;
	if (aSrc[aPos] == '(') {
		if ((expr = new expression()) == NULL) {
			throw std::runtime_error("[parser::parseExpr] unable to create expression to place parsed data into!");
		}
		// move past the start of the expression: '('
		++aPos;
	} else {
		throw std::runtime_error("[parser::parseExpr] syntax error - no '(' to start expression!");
	}

	/**
	 * Now we need to get set up to scan the incoming source
	 * at the current cursor position. We'll need to have
	 * some sense of a 'token', and if we're in the middle
	 * of building it. Then it's just a matter of being careful
	 * to cover all the bases as we go.
	 */
	char			c = '\0';
	std::string		token;
	bool			in_token = false;
	while ((aPos < aSrc.length()) && (aSrc[aPos] != ')')) {
		if ((c = aSrc[aPos]) == '(') {
			// make sure we have a function already
			if (((expression *)expr)->getFunction() == NULL) {
				// no can do - drop what we've created and bail
				if (expr != NULL) {
					delete expr;
					expr = NULL;
				}
				throw std::runtime_error("[parser:parseExpr] an expression can't be the first element in an expression - it must be a function!");
			}
			// starting a new expression to parse
			value	*sub = parseExpr(aSrc, aPos);
			if (sub != NULL) {
				if (((expression *)expr)->addToArgs(sub)) {
					// add as sub-expr ONLY if it's an expression
					if (sub->isExpression()) {
						addSubExpr((expression *)sub);
					}
				} else {
					// couldn't add it - delete it so we don't leak
					delete sub;
					sub = NULL;
				}
			}
			// the expression parser left us in the right spot
			continue;
		} else if (isspace(c)) {
			if (in_token) {
				/**
				 * Handle the token, and then clear it out for the
				 * next one. There is a special "function" for the
				 * setting of a variable - 'set'. We need to look
				 * for that, and if we have it, we need to drop the
				 * expression we're building and realize that it's
				 * a variable, and build that instead.
				 */
				if (token == "set") {
					// delete the existing expression as it's just wrong
					if (expr != NULL) {
						delete expr;
						expr = NULL;
					}
					// now try to parse out the variable definition
					if ((expr = parseVariable(aSrc, aPos)) == NULL) {
						throw std::runtime_error("[parser::parseExpr] could not parse the variable definition");
					} else {
						// add this to the parser's list of vars
						addVariable((variable * &)expr);
					}
					/**
					 * We thought this was going to be an expression, but
					 * it turned out to be a variable definition, we have
					 * parsed that bad boy, and now it's time to leave.
					 * Make sure we aren't in a token, and bail out.
					 */
					in_token = false;
					break;
				} else {
					// simple token processing, with a clear to reset
					handleToken((expression *)expr, token);
					token.clear();
					in_token = false;
				}
			}
		} else {
			// part of the token we're building
			token.push_back(c);
			in_token = true;
		}
		// move to the next character and keep processing
		++aPos;
	}

	/**
	 * If we were in the middle of a token when we hit the end
	 * of the expression, then handle it, but there's no need to
	 * clear it out, etc. as we're all done.
	 */
	if (in_token) {
		handleToken((expression *)expr, token);
	}

	// make sure to move past the closing ')'
	if ((aSrc[aPos] == ')') && (aPos < aSrc.length())) {
		++aPos;
	}

	// return whatever expression we have built up to now
	return expr;
}


/**
 * This method assumes that it's pointing into the source
 * string just AFTER the '(set' part of a variable definition.
 * We need to finish parsing the variable name and value -
 * no matter how complex that value might be, and return it
 * as a new variable. The caller will be responsible for
 * deleting the variable when he's done with it or we will
 * leak.
 */
variable *parser::parseVariable( const std::string & aSrc, uint32_t & aPos )
{
	variable	*retval = NULL;

	/**
	 * The general form of a setter is:
	 *   (set x 14)
	 * but it can be a complex expression as well:
	 *   (set y (+ 1 2 3))
	 * so we need to parse the name, and then parse the
	 * potential expression that is the value, and make a
	 * variable out of this and return it to the caller.
	 */
	char			c = '\0';
	std::string		token;
	bool			in_token = false;
	bool			complete = false;
	while ((aPos < aSrc.length()) && (aSrc[aPos] != ')')) {
		if ((c = aSrc[aPos]) == '(') {
			// make sure we have a variable already
			if (retval == NULL) {
				throw std::runtime_error("[parser:parseVariable] an expression can't be the first element after 'set' in a variable definition - it must be a name!");
			}
			// starting a new expression to parse as the value
			complete = retval->set(parseExpr(aSrc, aPos));
			// the expression parser left us in the right spot
			continue;
		} else if (isspace(c)) {
			if (in_token) {
				/**
				 * Handle the token and then clear it out for
				 * the rest of the parsing. If we have no name
				 * for the variable, that's first, and we keep
				 * it - otherwise, we parse out the value and
				 * we should be done.
				 */
				if (retval == NULL) {
					if ((retval = new variable(token)) == NULL) {
						// create the error message for the bad creation
						std::string		msg = "[parser::parseVariable] unable to create variable for the name: ";
						msg.append(token);
						// ...and now throw it so it can be delt with
						throw std::runtime_error(msg);
					}
				} else if (!complete) {
					// flag that we're done - if it succeeds
					complete = retval->set(parseConst(token));
				} else {
					// drop the variable that we created
					if (retval != NULL) {
						delete retval;
						retval = NULL;
					}
					// ...and then throw the exception
					throw std::runtime_error("[parser:parseVariable] a 'set' requires only two things, and you have provided three!");
				}
				// now we can clear out the token for next time
				token.clear();
				in_token = false;
			}
		} else {
			// part of the token we're building
			token.push_back(c);
			in_token = true;
		}
		// move to the next character and keep processing
		++aPos;
	}

	/**
	 * If we were in the middle of a token when we hit the end
	 * of the variable, then handle it, but there's no need to
	 * clear it out, etc. as we're all done.
	 */
	if (in_token) {
		if (!complete) {
			retval->set(parseConst(token));
		} else {
			// drop the variable that we created
			if (retval != NULL) {
				delete retval;
				retval = NULL;
			}
			// ...and then throw the exception
			throw std::runtime_error("[parser:parseVariable] a 'set' requires only two things, and you have provided three!");
		}
	}

	// make sure to move past the closing ')'
	if ((aSrc[aPos] == ')') && (aPos < aSrc.length())) {
		++aPos;
	}

	// return whatever variable we have built up to now
	return retval;
}


/**
 * This method takes a parsed string (token) from the source
 * and depending on the state of the current expression we're
 * working on, assumes this to be the function name (if it's
 * not defined already) or the name of a variable or the
 * definition of a constant. Based on this context, it does
 * what's necessary to place the value/variable/etc. into
 * the expression properly;
 */
bool parser::handleToken( expression *anExpr, const std::string & aToken )
{
	bool		error = false;
	bool		handled = false;

	// make sure there's something we can do
	if (!error) {
		if (anExpr == NULL) {
			error = true;
			throw std::runtime_error("[parser::handleToken] the provided expression was NULL, and that's very bad news.");
		}
	}

	/**
	 * See if the expression has a function, if not, then this token needs
	 * to be a function, and we need to be able to find that function in
	 * the table we have. If not, it's a syntax error and we can't continue.
	 */
	if (!error && !handled) {
		if (anExpr->getFunction() == NULL) {
			function	*f = lookUpFunction(aToken);
			if (f == NULL) {
				error = true;
				// create the error message for the missing function
				std::string		msg = "[parser::handleToken] there is no function for the name: ";
				msg.append(aToken);
				// ...and now throw it so it can be delt with
				throw std::runtime_error(msg);
			} else {
				// save the function with the expression
				anExpr->setFunction(f);
				// ...and indicate that this token has been handled
				handled = true;
			}
		}
	}

	/**
	 * At this point, if we haven't handled the token, then it's an
	 * argument. Now it could be a variable, or it could be a simple
	 * constant. We need to identify the constants as they are simple:
	 * integer, double, or date/time. If it's one of these, then make
	 * the value based on the constant and add it to the expression.
	 */
	if (!error && !handled) {
		value	*v = parseConst(aToken);
		if (v != NULL) {
			// save this new guy as a constant in the parser
			addConst(v);
			// ...and add as argument to the expression
			anExpr->addToArgs(v);
			// ...and indicate that this token has been handled
			handled = true;
		}
	}

	/**
	 * Not a function, not a constant - got to be a variable... lookup
	 * or create a new variable with this name and place it in the table
	 * of all variables in this parser.
	 */
	if (!error && !handled) {
		value	*v = lookUpVariable(aToken);
		if (v == NULL) {
			error = true;
			// create the error message for the missing variable
			std::string		msg = "[parser::handleToken] unable to find/create variable: ";
			msg.append(aToken);
			// ...and now throw it so it can be delt with
			throw std::runtime_error(msg);
		} else {
			// save the variable argument with the expression
			anExpr->addToArgs(v);
			// ...and indicate that this token has been handled
			handled = true;
		}
	}

	return !error;
}


/**
 * This method looks at the provided string and attempts to
 * parse out a constant from it and return it as a new value
 * pointer. If this is NOT successful, then this method
 * will return NULL, but if not-NULL, it is the responsibility
 * of the caller to delete the returned value or we will leak.
 */
value *parser::parseConst( const std::string & aToken )
{
	value		*retval = NULL;
	bool		handled = false;

	/**
	 * Our constants look like one of the following patterns:
	 *
	 * +/- 1 2 3 - simple integers all matching isdigit()
	 * ... plus '.' and 'e'/'E' - doubles
	 * 'YYYY-MM-DD' or 'YYYY-MM-DD HH:MM:SS.ssss' - timestamps
	 */
	size_t		len = aToken.length();
	if ((aToken[0] == '\'') && (aToken[len - 1] == '\'')) {
		// likely a data by our format
		retval = new value(util::timer::parseTimestamp(aToken.substr(1, len - 2)));
		if (retval == NULL) {
			// create the error message for the malformed timestamp
			std::string		msg = "[parser::parseConst] unable to parse timestamp value: ";
			msg.append(aToken);
			// ...and now throw it so it can be delt with
			throw std::runtime_error(msg);
		} else {
			// ...and indicate that this token has been handled
			handled = true;
		}
	} else if (aToken.find_first_not_of("+-0123456789.eE") == std::string::npos) {
		// it's a number of some kind - find out which kind
		if (aToken.find_first_of(".eE") == std::string::npos) {
			// it's an integer
			retval = new value(atoi(aToken.c_str()));
			if (retval == NULL) {
				// create the error message for the malformed integer
				std::string		msg = "[parser::parseConst] unable to parse int value: ";
				msg.append(aToken);
				// ...and now throw it so it can be delt with
				throw std::runtime_error(msg);
			} else {
				// ...and indicate that this token has been handled
				handled = true;
			}
		} else {
			// it's a double
			retval = new value(atof(aToken.c_str()));
			if (retval == NULL) {
				// create the error message for the malformed double
				std::string		msg = "[parser::parseConst] unable to parse double value: ";
				msg.append(aToken);
				// ...and now throw it so it can be delt with
				throw std::runtime_error(msg);
			} else {
				// ...and indicate that this token has been handled
				handled = true;
			}
		}
	} else if ((aToken == "true") || (aToken == "false")) {
		// it's a boolean
		retval = new value(aToken[0] == 't');
		if (retval == NULL) {
			// create the error message for the malformed boolean
			std::string		msg = "[parser::parseConst] unable to parse bool value: ";
			msg.append(aToken);
			// ...and now throw it so it can be delt with
			throw std::runtime_error(msg);
		} else {
			// ...and indicate that this token has been handled
			handled = true;
		}
	}

	return retval;
}
}		// end of namespace lkit


/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this value. It basically is just the toString() method of the
 * parser streamed out.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::parser & aValue )
{
	aStream << aValue.toString();
	return aStream;
}
