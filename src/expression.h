/**
 * expression.h - this file defines an expression which is a single
 *                function and a series of values as arguments. This
 *                will be subclassed off value so that it can "fit in"
 *                as a value (argument) in another expression, and will
 *                use the value's contents as the calculated value for
 *                cached access.
 */
#ifndef __LKIT_EXPRESSION_H
#define __LKIT_EXPRESSION_H

//	System Headers
#include <stdint.h>
#include <ostream>
#include <string>
#include <vector>

//	Third-Party Headers

//	Other Headers
#include "value.h"

//	Forward Declarations
/**
 * We are going to build up expressions from functions and arguments (values)
 * so we need to have a forward reference on the function so we can use it
 * in the signatures.
 */
namespace lkit {
class function;
}	// end of namespace lkit

//	Public Constants

//	Public Datatypes

//	Public Data Constants


/**
 * Main class definition
 */
namespace lkit {
class expression :
	public value
{
	public:
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
		expression();
		/**
		 * This is the most general constructor of an expression - it
		 * takes a function and a vector of arguments that will be passed
		 * into that function for processing when the time comes to
		 * evaluate this expression.
		 */
		expression( function *aFcn, std::vector<value *> & anArgs );
		/**
		 * This is a convenience constructor that takes the function with
		 * up to six arguments. The caller can supply more arguments, as
		 * needed, but this is ideally suited for the case where the
		 * function only needs the arguments provided. The memory
		 * management of the values is the responsible of the caller, and
		 * they are used here only as references and not managed.
		 */
		expression( function *aFcn, value *anArg1 = NULL, value *anArg2 = NULL,
					value *anArg3 = NULL, value *anArg4 = NULL,
					value *anArg5 = NULL, value *anArg6 = NULL );
		/**
		 * This is the standard copy constructor that needs to be in every
		 * class to make sure that we control how many copies we have
		 * floating around in the system.
		 */
		expression( const expression & anOther );

		/**
		 * This is the standard destructor and needs to be virtual to make
		 * sure that if we subclass off this, the right destructor will be
		 * called.
		 */
		virtual ~expression();

		/**
		 * When we process the result of an equality we need to make sure
		 * that we do this right by always having an equals operator on
		 * all classes.
		 */
		expression & operator=( const expression & anOther );

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
		virtual void setName( const std::string & aName );
		/**
		 * This method returns the name of this expression, as defined by
		 * the creator. This should be set in one of the setters or the
		 * constructor, but it's possible that it hasn't been set, and
		 * care should be taken in processing it's value.
		 */
		virtual std::string getName() const;

		/**
		 * This method can be used to set the function pointer (reference)
		 * that this expression will be using to evaluate the arguments
		 * it's holding.
		 */
		virtual void setFunction( function *aFunction );
		/**
		 * This method returns the pointer to the function that this
		 * expression is using to evaluate the arguments. If this hasn't
		 * been set, then this method will return NULL - you will probably
		 * want to check on that.
		 */
		virtual function *getFunction();

		/**
		 * This method sets the vector of ALL arguments for this expression
		 * to the provided set. This will REPLACE the existing argument list
		 * and put in it's place the provided list. The memory management
		 * of the values is the responsible of the caller, and they are
		 * used here only as references and not managed.
		 */
		virtual void setArgs( const std::vector<value *> & anArgs );
		/**
		 * This method returns the actual reference to the list of args
		 * for this expression. Great care should be exercised, as this
		 * is the ACTUAL vector, and can be modified by other threads
		 * so please be careful.
		 */
		virtual const std::vector<value *> & getArgs() const;
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
		virtual bool addToArgs( value *anArg );
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
		virtual bool addToArgs( const std::vector<value *> & anArgs );
		/**
		 * This method will look to see if the provided argument is in
		 * the list for this expression, and will remove the FIRST one
		 * it sees. If one is removed, this method will return 'true',
		 * otherwise, it will return 'false'.
		 */
		virtual bool removeFromArgs( const value *anArg );
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
		virtual void clearArgs();

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
		 * This method checks to see if two expressions are equal in their
		 * contents and not their pointer values. This is how you'd likely
		 * expect equality to work.
		 */
		bool operator==( const expression & anOther ) const;
		/**
		 * This method checks to see if two expressions are NOT equal in their
		 * contents and not their pointer values. This is how you'd likely
		 * expect equality to work.
		 */
		bool operator!=( const expression & anOther ) const;

		/**
		 * This method looks at the value of this expression and the evaluated
		 * value of the arguments and sees if they are equivalent. If so,
		 * this operator returns 'true'. This is a very powerful operator
		 * for the variable as it allows values to be compared to variables
		 * without worrying about the different classes.
		 */
		bool operator==( const value & anOther ) const;
		/**
		 * This method looks at the value of this expression and the evaluated
		 * value of the arguments and sees if they are NOT equivalent. If so,
		 * this operator returns 'true'. This is a very powerful operator
		 * for the variable as it allows values to be compared to variables
		 * without worrying about the different classes.
		 */
		bool operator!=( const value & anOther ) const;

	protected:
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
		virtual bool evalAsBool_nl();
		virtual int evalAsInt_nl();
		virtual double evalAsDouble_nl();
		virtual uint64_t evalAsTime_nl();

	private:
		/**
		 * While it's not necessary, it might come in very handy at times
		 * to be able it identify the individual expression instance, and
		 * for this purpose, we have given it a 'name'. Now the name is
		 * really just an identifier, but it's easily human-readable, and
		 * it might actually be nice to make it mean something.
		 */
		std::string				_name;
		/**
		 * This is the pointer to the function that this argument will be
		 * using to evaluate the arguments into a singluar value. This will
		 * be passed in on the constructor or the setter, and is just a
		 * reference to the function - we're not doing any memory management
		 * on this guy.
		 */
		function				*_fcn;
		/**
		 * These are all the arguments that the user has supplied for this
		 * expression. Each evaluation will see the function given this list
		 * and then asked to evaluate to return a value.
		 *
		 * The memory management of the values is the responsible of
		 * the caller, and they are used here only as references and
		 * not managed.
		 */
		std::vector<value *>	_args;
};
}		// end of namespace lkit

/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this value. It basically is just the toString() method of the
 * expression streamed out.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::expression & aValue );

#endif		// __LKIT_EXPRESSION_H
