/**
 * expression.cpp - this file implements an expression which is a single
 *                  function and a series of values as arguments. This
 *                  will be subclassed off value so that it can "fit in"
 *                  as a value (argument) in another expression, and will
 *                  use the value's contents as the calculated value for
 *                  cached access.
 */

//	System Headers
#include <math.h>
#include <sstream>

//	Third-Party Headers
#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>

//	Other Headers
#include "expression.h"
#include "function.h"

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
 * makes a simple expression ready to hold some kind of function
 * and a possible series of values.
 */
expression::expression() :
	value(),
	_name(),
	_fcn(NULL),
	_args()
{
}


/**
 * This is the most general constructor of an expression - it
 * takes a function and a vector of arguments that will be passed
 * into that function for processing when the time comes to
 * evaluate this expression.
 */
expression::expression( function *aFcn, std::vector<value *> & anArgs ) :
	value(),
	_name(),
	_fcn(aFcn),
	_args()
{
	// add each argument to the list
	BOOST_FOREACH( value *v, anArgs ) {
		if (v != NULL) {
			_args.push_back(v);
		}
	}
}


/**
 * This is a convenience constructor that takes the function with
 * up to six arguments. The caller can supply more arguments, as
 * needed, but this is ideally suited for the case where the
 * function only needs the arguments provided. The memory
 * management of the values is the responsible of the caller, and
 * they are used here only as references and not managed.
 */
expression::expression( function *aFcn, value *anArg1, value *anArg2,
						value *anArg3, value *anArg4,
						value *anArg5, value *anArg6 ) :
	value(),
	_name(),
	_fcn(aFcn),
	_args()
{
	// add all the args that we have...
	if (anArg1 != NULL) {
		_args.push_back(anArg1);
	}
	if (anArg2 != NULL) {
		_args.push_back(anArg2);
	}
	if (anArg3 != NULL) {
		_args.push_back(anArg3);
	}
	if (anArg4 != NULL) {
		_args.push_back(anArg4);
	}
	if (anArg5 != NULL) {
		_args.push_back(anArg5);
	}
	if (anArg6 != NULL) {
		_args.push_back(anArg6);
	}
}


/**
 * This is the standard copy constructor that needs to be in every
 * class to make sure that we control how many copies we have
 * floating around in the system.
 */
expression::expression( const expression & anOther ) :
	value(anOther),
	_name(),
	_fcn(NULL),
	_args()
{
	// let the '=' operator do the heavy lifting...
	*this = anOther;
}


/**
 * This is the standard clone method that we'll have for all
 * classes so that it's possible to clone the value without
 * having to worry about memory management, etc.
 */
value *expression::clone() const
{
	return new expression(*this);
}


/**
 * This is the standard destructor and needs to be virtual to make
 * sure that if we subclass off this, the right destructor will be
 * called.
 */
expression::~expression()
{
	// just clear out the arguments...
	clearArgs();
}


/**
 * When we process the result of an equality we need to make sure
 * that we do this right by always having an equals operator on
 * all classes.
 */
expression & expression::operator=( const expression & anOther )
{
	if (this != & anOther) {
		// first, let the super do it's thing...
		value::operator=(anOther);
		// ...now I can do my stuff
		_name = anOther._name;
		_fcn = anOther._fcn;
		_args = anOther._args;
	}
	return *this;
}


/*******************************************************************
 *
 *                        Accessor Methods
 *
 *******************************************************************/
/**
 * This method sets the name, or identifier, of this expression
 * so that it can be used to later identify the particular
 * expression instance in the system. It's just used to make things
 * easier to understand.
 */
void expression::setName( const std::string & aName )
{
	spinlock::scoped_lock	lock(mutex());
	_name = aName;
}


/**
 * This method returns the name of this expression, as defined by
 * the creator. This should be set in one of the setters or the
 * constructor, but it's possible that it hasn't been set, and
 * care should be taken in processing it's value.
 */
std::string expression::getName() const
{
	spinlock::scoped_lock	lock((spinlock &)mutex());
	std::string		n = _name;
	return n;
}


/**
 * This method can be used to set the function pointer (reference)
 * that this expression will be using to evaluate the arguments
 * it's holding.
 */
void expression::setFunction( function *aFunction )
{
	spinlock::scoped_lock	lock(mutex());
	_fcn = aFunction;
}


/**
 * This method returns the pointer to the function that this
 * expression is using to evaluate the arguments. If this hasn't
 * been set, then this method will return NULL - you will probably
 * want to check on that.
 */
function *expression::getFunction()
{
	return _fcn;
}


/**
 * This method sets the vector of ALL arguments for this expression
 * to the provided set. This will REPLACE the existing argument list
 * and put in it's place the provided list. The memory management
 * of the values is the responsible of the caller, and they are
 * used here only as references and not managed.
 */
void expression::setArgs( const std::vector<value *> & anArgs )
{
	spinlock::scoped_lock	lock(mutex());
	_args = anArgs;
}


/**
 * This method returns the actual reference to the list of args
 * for this expression. Great care should be exercised, as this
 * is the ACTUAL vector, and can be modified by other threads
 * so please be careful.
 */
const std::vector<value *> & expression::getArgs() const
{
	return _args;
}


/**
 * This method adds the provided argument to the list of args
 * for this expression. No check will be made to see if this
 * argument already exists as it's quite possible that the
 * caller wants the same value in the argument list multiple
 * times.
 *
 * The memory management of the values is the responsible of
 * the caller, and they are used here only as references and
 * not managed.
 */
bool expression::addToArgs( value *anArg )
{
	bool		error = false;
	if (anArg == NULL) {
		error = true;
	} else {
		spinlock::scoped_lock	lock(mutex());
		_args.push_back(anArg);
	}
	return !error;
}


/**
 * This method adds each of the the provided arguments in the
 * list to the list of args for this expression. No checks will
 * be made to see if any argument already exists as it's quite
 * possible that the caller wants the same value in the argument
 * list multiple times.
 *
 * The memory management of the values is the responsible of
 * the caller, and they are used here only as references and
 * not managed.
 */
bool expression::addToArgs( const std::vector<value *> & anArgs )
{
	bool		error = false;
	spinlock::scoped_lock	lock(mutex());
	// add each argument to the list
	BOOST_FOREACH( value *v, anArgs ) {
		if (v == NULL) {
			error = true;
		} else {
			_args.push_back(v);
		}
	}
	return !error;
}


/**
 * This method will look to see if the provided argument is in
 * the list for this expression, and will remove the FIRST one
 * it sees. If one is removed, this method will return 'true',
 * otherwise, it will return 'false'.
 */
bool expression::removeFromArgs( const value *anArg )
{
	// assume we won't find what we're looking for
	bool		removed = false;
	if (anArg != NULL) {
		spinlock::scoped_lock	lock(mutex());
		// check each argument in the list
		for (std::vector<value *>::iterator it = _args.begin(); it != _args.end(); ++it) {
			if (*it == anArg) {
				_args.erase(it);
				removed = true;
				break;
			}
		}
	}
	return removed;
}


/**
 * This method will remove ALL the arguments from the list for
 * this expression. This is the classic "clear out" method for
 * the arguments so that the next time the expression is evaluated,
 * it will need new arguments provided by the setters.
 *
 * The memory management of the values is the responsible of
 * the caller, and they are used here only as references and
 * not managed.
 */
void expression::clearArgs()
{
	spinlock::scoped_lock	lock(mutex());
	_args.clear();
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
std::string expression::toString() const
{
	spinlock::scoped_lock	lock((spinlock &)mutex());
	std::ostringstream	msg;
	msg << "[";
	if (!_name.empty()) {
		msg << "'" << _name << "' ";
	}
	msg << "fn=" << (_fcn == NULL ? "<null>" : _fcn->toString().c_str())
		<< " args=(";
	for (size_t i = 0; i < _args.size(); ++i) {
		if (i > 0) {
			msg << ", ";
		}
		msg << (_args[i] == NULL ? "<null>" : _args[i]->toString().c_str());
	}
	msg << ")]";
	return msg.str();
}


/**
 * When we have a custom '==' operator, it's wise to have a hash
 * method as well. This makes it much easier to used hash-based
 * containers like boost, etc. It's a requirement that if two
 * instances are '==', then their hash() methods must be '==' as
 * well.
 */
size_t expression::hash() const
{
	size_t	ans = value::hash();
	spinlock::scoped_lock	lock((spinlock &)mutex());
	boost::hash_combine(ans, _name);
	boost::hash_combine(ans, _fcn);
	BOOST_FOREACH( value *v, _args ) {
		if (v != NULL) {
			boost::hash_combine(ans, v);
		}
	}
	return ans;
}


/**
 * This method checks to see if two expressions are equal in their
 * contents and not their pointer values. This is how you'd likely
 * expect equality to work.
 */
bool expression::operator==( const expression & anOther ) const
{
	bool		equals = false;
	if ((this == & anOther) ||
		(value::operator==(anOther) &&
		 (_name == anOther._name) &&
		 (_fcn == anOther._fcn) &&
		 (_args == anOther._args))) {
		equals = true;
	}
	return equals;
}


/**
 * This method checks to see if two expressions are NOT equal in their
 * contents and not their pointer values. This is how you'd likely
 * expect equality to work.
 */
bool expression::operator!=( const expression & anOther ) const
{
	return !operator==(anOther);
}


/**
 * This method looks at the value of this expression and the evaluated
 * value of the arguments and sees if they are equivalent. If so,
 * this operator returns 'true'. This is a very powerful operator
 * for the variable as it allows values to be compared to variables
 * without worrying about the different classes.
 */
bool expression::operator==( const value & anOther ) const
{
	bool		equals = false;
	if ((this == & anOther) || value::operator==(anOther)) {
		equals = true;
	}
	return equals;
}


/**
 * This method looks at the value of this expression and the evaluated
 * value of the arguments and sees if they are NOT equivalent. If so,
 * this operator returns 'true'. This is a very powerful operator
 * for the variable as it allows values to be compared to variables
 * without worrying about the different classes.
 */
bool expression::operator!=( const value & anOther ) const
{
	return !operator==(anOther);
}


/*******************************************************************
 *
 *                      Subclass Accessor Methods
 *
 *******************************************************************/
/**
 * This method gets the value for this instance, and it may be quite
 * involved in getting the value. This will be the way to get
 * constants as well as evaluate functions and expressions.
 *
 * The key with the "_nl" is that it means "no lock". No lock
 * will be placed on the instance in the process of setting
 * these values. That means the caller has to do it.
 */
bool expression::evalAsBool_nl()
{
	if (_fcn != NULL) {
		set_nl(_fcn->eval(_args));
	}
	return value::evalAsBool_nl();
}


int expression::evalAsInt_nl()
{
	if (_fcn != NULL) {
		set_nl(_fcn->eval(_args));
	}
	return value::evalAsInt_nl();
}


double expression::evalAsDouble_nl()
{
	if (_fcn != NULL) {
		set_nl(_fcn->eval(_args));
	}
	return value::evalAsDouble_nl();
}


uint64_t expression::evalAsTime_nl()
{
	if (_fcn != NULL) {
		set_nl(_fcn->eval(_args));
	}
	return value::evalAsTime_nl();
}
}		// end of namespace lkit


/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this value. It basically is just the toString() method of the
 * expression streamed out.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::expression & aValue )
{
	aStream << aValue.toString();
	return aStream;
}
