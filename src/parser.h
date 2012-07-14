/**
 * parser.h - this file defines a simple lisp-style language parser where
 *            the values, functions and expressions are defined in the
 *            language/classes of LKit. This will take a std::string
 *            as the "source" code and "compile" it into an execution
 *            tree that can be calculated as many times as needed. The
 *            goal is to use smart (reduced) updates in the tree to make
 *            the updating as simple and efficient as possible.
 */
#ifndef __LKIT_PARSER_H
#define __LKIT_PARSER_H

//	System Headers
#include <stdint.h>
#include <ostream>
#include <string>

//	Third-Party Headers
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>

//	Other Headers
#include "variable.h"

//	Forward Declarations
/**
 * We are going to build up expressions from functions and arguments (values)
 * so we need to have a forward reference on the function so we can use it
 * in the signatures.
 */
namespace lkit {
class function;
class expression;
}	// end of namespace lkit

//	Public Constants

//	Public Datatypes
/**
 * We are going to have all our constants as values in a list so that we
 * can manage the lifecycle external to the expressions (which is necessary)
 * and at the same time, make it easy and efficient to manage the values.
 * So we'll have a simple list of pointers, and as needed, we'll just add
 * to this list, destroying everything when we're all done.
 */
typedef std::vector<lkit::value *> var_list_t;
/**
 * We are going to have all our sub-expressions in a list so that we
 * can manage the lifecycle external to the expressions (which is necessary)
 * and at the same time, make it easy and efficient to manage the expressions.
 * So we'll have a simple list of pointers, and as needed, we'll just add
 * to this list, destroying everything when we're all done.
 */
typedef std::vector<lkit::expression *> expr_list_t;
/**
 * We are going to have a series of variables in the parsed source and
 * we need to maintain them in a reasonable container, so we're going to
 * pick a simple boost map where the key is the variable's name and the
 * value is the pointer to the actual value.
 */
typedef boost::unordered_map<std::string, lkit::variable *> var_map_t;
/**
 * We are going to have a map of the function names to the actual pointers
 * to the lkit::function instances, and rather than clutter up the code
 * with all the syntax, it makes sense to create a nice, simple typedef
 * here so that we can use it in the code and keep things cleaner.
 */
typedef boost::unordered_map<std::string, lkit::function *> fcn_map_t;

//	Public Data Constants


/**
 * Main class definition
 */
namespace lkit {
class parser
{
	public:
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
		parser();
		/**
		 * This is a general constructor of a parser - it takes a source
		 * code string and parses it into a language tree for evaluation.
		 */
		parser( const std::string & aSource );
		/**
		 * This is the standard copy constructor that needs to be in every
		 * class to make sure that we control how many copies we have
		 * floating around in the system.
		 */
		parser( const parser & anOther );

		/**
		 * This is the standard destructor and needs to be virtual to make
		 * sure that if we subclass off this, the right destructor will be
		 * called.
		 */
		virtual ~parser();

		/**
		 * When we process the result of an equality we need to make sure
		 * that we do this right by always having an equals operator on
		 * all classes.
		 */
		parser & operator=( const parser & anOther );

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
		virtual void setSource( const std::string & aSource );
		/**
		 * This method returns the currently active source code that is
		 * compiled into the language tree for current evaluation.
		 */
		virtual std::string getSource() const;

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
		virtual const var_map_t & getVariables() const;
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
		virtual bool addVariable( const variable & aVariable );
		virtual bool addVariable( variable * & aVariable );
		virtual bool addVariable( const std::string & aName, const value & aValue );
		virtual bool addVariable( const std::string & aName, value * & aValue );
		/**
		 * This method returns the pointer to the value for the
		 * provided variable name. The returned pointer is the
		 * ACTUAL value pointer in the table for this parser, so
		 * please exercise care when modifying it.
		 */
		virtual const value *getVariable( const std::string & aName );
		/**
		 * These methods allow the user to remove a variable from
		 * the parser which can be a bit dicey proposition, if you're
		 * not careful. If it's a variable that's been added by the
		 * caller, and NOT required, then removing it is OK. But if
		 * it's required, then no matter how it got there, you're
		 * going to be sad if you remove it. So use with care.
		 */
		virtual bool removeVariable( std::string & aName );
		/**
		 * This method simple clears out all the variables in this
		 * parser instance. It's a clean-slate starting point.
		 */
		virtual void clearVariables();
		/**
		 * This method provides the parser with a series of default
		 * variables for the expressions. These are values most likely
		 * to be used by standard mathematical expressions.
		 */
		virtual void useDefaultVariables();

		/**
		 * This method returns the actual reference to the map of known
		 * functions for this parser. Great care should be exercised, as
		 * this is the ACTUAL map, and can be modified by other threads
		 * so please be careful. The key is the name of the function as
		 * it exists in the source, and the value is the pointer to the
		 * lkit::function that is used.
		 */
		virtual const fcn_map_t & getFunctions() const;
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
		virtual bool addFunction( const std::string & aName, function *aFunction );
		/**
		 * This method will look to see if the provided function name
		 * is in the list for this parser, and will remove it if so.
		 * If one is removed, this method will return 'true', otherwise,
		 * it will return 'false'.
		 */
		virtual bool removeFunction( const std::string & aName );
		/**
		 * This method will remove ALL the known functions from the
		 * table for this parser.
		 */
		virtual void clearFunctions();
		/**
		 * This method will set up this parser with the DEFAULT functions
		 * for the language. This does not include any user-supplied
		 * functions, but does include the basic functionality of the
		 * language.
		 */
		virtual void useDefaultFunctions();

		/**
		 * This method attempts to compile the source, if there is any, and
		 * it's not already compiled into a language tree, and then evaluates
		 * the final expression, and all sub-expressions and functions to
		 * come up with a final value of the result. Everything is left in
		 * place so that additional evaluations are done as quickly as
		 * possible.
		 */
		virtual value eval();

		/**
		 * This method will clear out EVERYTHING for the parser and have
		 * it start as a "blank slate". This is not necessarily the state
		 * you want to use the parser in, as there's nothing there, but it
		 * is an important reference point that we need to be able to
		 * return to from time to time.
		 */
		virtual void clear();
		/**
		 * This method returns the parser to a "new start" state. It clears
		 * out everything and then adds back in the default variables and
		 * functions so that it's all ready to take your source and compile
		 * it into an executable language tree.
		 */
		virtual bool reset();

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
		virtual std::string toString() const;
		/**
		 * When we have a custom '==' operator, it's wise to have a hash
		 * method as well. This makes it much easier to used hash-based
		 * containers like boost, etc. It's a requirement that if two
		 * instances are '==', then their hash() methods must be '==' as
		 * well.
		 */
		virtual size_t hash() const;

		/**
		 * This method checks to see if two parsers are equal in their
		 * contents and not their pointer values. This is how you'd likely
		 * expect equality to work.
		 */
		bool operator==( const parser & anOther ) const;
		/**
		 * This method checks to see if two parsers are NOT equal in their
		 * contents and not their pointer values. This is how you'd likely
		 * expect equality to work.
		 */
		bool operator!=( const parser & anOther ) const;

	protected:
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
		value *lookUpVariable( const std::string & aName );

		/**
		 * This method looks to the map of all functions we know about in
		 * this parser instance and will attempt to find the one registered
		 * under the name provided. If one does exist under that name, then
		 * we'll return it for use in an expression. If not, we'll return
		 * NULL and that should be the sign of an error. Functions have to
		 * be defined prior to parsing of the code.
		 */
		function *lookUpFunction( const std::string & aName );

		/**
		 * This method returns the actual reference to the map of known
		 * constants for this parser. Great care should be exercised, as
		 * this is the ACTUAL list, and can be modified by other threads
		 * so please be careful.
		 */
		virtual const var_list_t & getConsts() const;
		/**
		 * This method adds the provided value to the list of constants
		 * for this parser. No checks will be made to see if this value
		 * already exists as it's a constant. Period.
		 *
		 * The memory management of the values will become the
		 * responsible of the parser, so the caller has to be willing
		 * to relinquish control.
		 */
		virtual bool addConst( value *aConstant );
		/**
		 * This method will remove ALL the known constants from the
		 * list for this parser.
		 */
		virtual void clearConsts();

		/**
		 * This method returns the actual reference to the list of parsed
		 * expressions for this parser. Great care should be exercised,
		 * as this is the ACTUAL list, and can be modified by other threads
		 * so please be careful.
		 */
		virtual const expr_list_t & getExpr() const;
		/**
		 * This method returns the actual reference to the map of known
		 * sub-expressions for this parser. Great care should be exercised,
		 * as this is the ACTUAL list, and can be modified by other threads
		 * so please be careful.
		 */
		virtual const expr_list_t & getSubExpr() const;
		/**
		 * This method adds the provided expression to the list of
		 * expressions for this parser. No checks will be made to see
		 * if this expression already exists. Period.
		 *
		 * The memory management of the expressions will become the
		 * responsible of the parser, so the caller has to be willing
		 * to relinquish control.
		 */
		virtual bool addExpr( expression *anExpression );
		/**
		 * This method adds the provided expression to the list of
		 * sub-expressions for this parser. No checks will be made to see
		 * if this expression already exists. Period.
		 *
		 * The memory management of the expressions will become the
		 * responsible of the parser, so the caller has to be willing
		 * to relinquish control.
		 */
		virtual bool addSubExpr( expression *anExpression );
		/**
		 * This method will remove ALL the known expressions from the
		 * list for this parser.
		 */
		virtual void clearExpr( bool includeSubExpr = true );
		/**
		 * This method looks at the listof expressions in a thread-safe
		 * way, and if there are expressions coompiled from source, then
		 * it returns 'true'. This is used to see if we need to compile
		 * the source into a language tree.
		 */
		virtual bool isCompiled();

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
		virtual bool compile();
		/**
		 * This method assumes that the string starting at position
		 * 'aPos' is the start of an expression, and will build up
		 * the necessary expression and move the position of the
		 * 'cursor' as the parsing continues. This will be called
		 * recursively to generate the complete, final expression for
		 * the code starting at this point.
		 */
		virtual value *parseExpr( const std::string & aSrc, uint32_t & aPos );
		/**
		 * This method assumes that it's pointing into the source
		 * string just AFTER the '(set' part of a variable definition.
		 * We need to finish parsing the variable name and value -
		 * no matter how complex that value might be, and return it
		 * as a new variable. The caller will be responsible for
		 * deleting the variable when he's done with it or we will
		 * leak.
		 */
		virtual variable *parseVariable( const std::string & aSrc, uint32_t & aPos );
		/**
		 * This method takes a parsed string (token) from the source
		 * and depending on the state of the current expression we're
		 * working on, assumes this to be the function name (if it's
		 * not defined already) or the name of a variable or the
		 * definition of a constant. Based on this context, it does
		 * what's necessary to place the value/variable/etc. into
		 * the expression properly;
		 */
		virtual bool handleToken( expression *anExpr, const std::string & aToken );
		/**
		 * This method looks at the provided string and attempts to
		 * parse out a constant from it and return it as a new value
		 * pointer. If this is NOT successful, then this method
		 * will return NULL, but if not-NULL, it is the responsibility
		 * of the caller to delete the returned value or we will leak.
		 */
		virtual value *parseConst( const std::string & aToken );

	private:
		/**
		 * We are going to need to have the raw "source" code for the
		 * parser - this is the initial textual representation of the
		 * structure that we're going to compile into a language tree
		 * and then execute for the different values.
		 */
		std::string						_src;
		// ...and a simple spinlock to control access to it
		mutable boost::detail::spinlock	_src_mutex;
		/**
		 * These are all the functions that this parser knows about - keyed
		 * by their name. The value is a pointer to the function, but since
		 * we are NOT doing memory management of these functions, we can
		 * easily replace them without worrying about leaking.
		 */
		fcn_map_t						_fcns;
		// ...and a simple spinlock to control access to it
		mutable boost::detail::spinlock	_fcns_mutex;
		/**
		 * These are all the variables that this parser knows about - keyed
		 * on the name of the variable, which might also be in the variable,
		 * but it's possible to have a value or even an expression as a
		 * variable in the parser, and dealing with the pointers makes all
		 * the magic possible.
		 */
		var_map_t						_vars;
		// ...and a simple spinlock to control access to it
		mutable boost::detail::spinlock	_vars_mutex;
		/**
		 * These are all the constants that we'll parse out of the source
		 * code and need a place to hold them as they are referenced in
		 * the expressions. Because the expressions don't manage the memory
		 * of the functions and values it uses, we need to do that externally.
		 */
		var_list_t						_const;
		// ...and a simple spinlock to control access to it
		mutable boost::detail::spinlock	_const_mutex;
		/**
		 * These are all the expressions and sub-expressions that this
		 * parser knows about - in two simple lists. These are created
		 * in the parsing of the source, but really don't hold much as
		 * all the values, constants, and functions are owned by the
		 * parser.
		 */
		expr_list_t						_expr;
		expr_list_t						_subs;
		// ...and a simple spinlock to control access to it
		mutable boost::detail::spinlock	_expr_mutex;
};
}		// end of namespace lkit

/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this value. It basically is just the toString() method of the
 * parser streamed out.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::parser & aValue );

#endif		// __LKIT_PARSER_H
