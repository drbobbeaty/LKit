/**
 * variable.h - this file defines a simple, typed, variable. This will be
 *              a simple named value that can have many different values
 *              over time. It's evaluation is simple - it's just the value
 *              of the variable, but it can also be used in all expression
 *              processing.
 */
#ifndef __LKIT_VARIABLE_H
#define __LKIT_VARIABLE_H

//	System Headers
#include <stdint.h>
#include <ostream>
#include <string>

//	Third-Party Headers

//	Other Headers
#include "value.h"

//	Forward Declarations

//	Public Constants

//	Public Datatypes

//	Public Data Constants


/**
 * Main class definition
 */
namespace lkit {
class variable :
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
		 * makes a simple variable ready to hold some kind of value.
		 */
		variable();
		/**
		 * This version of the constructor takes the name of the variable
		 * and sets that but does not attempt to set the VALUE of the
		 * variable at all. This will be commonly used in the parsing of
		 * expressions when a variable is defined in the code, and not
		 * yet provided by the caller.
		 */
		variable( const std::string & aName );
		/**
		 * This version of the constructor takes a name and a value and
		 * makes a simple variable. This is very convenient as the
		 * variable is a value, but sometimes we need to make a new
		 * instance based on an old one.
		 */
		variable( const std::string & aName, const value & anOther );
		/**
		 * These constructors take the different allowed types and create
		 * variables based on that argument. This sets the name, type as
		 * well as the actual value for this new instance.
		 */
		variable( const std::string & aName, bool aValue );
		variable( const std::string & aName, int aValue );
		variable( const std::string & aName, double aValue );
		variable( const std::string & aName, uint64_t aValue );
		variable( const std::string & aName, value *aValue );
		/**
		 * This is the standard copy constructor that needs to be in every
		 * class to make sure that we control how many copies we have
		 * floating around in the system.
		 */
		variable( const variable & anOther );
		/**
		 * This is the standard clone method that we'll have for all
		 * classes so that it's possible to clone the value without
		 * having to worry about memory management, etc.
		 */
		virtual value *clone() const;
		/**
		 * This is the standard destructor and needs to be virtual to make
		 * sure that if we subclass off this, the right destructor will be
		 * called.
		 */
		virtual ~variable();

		/**
		 * When we process the result of an equality we need to make sure
		 * that we do this right by always having an equals operator on
		 * all classes.
		 */
		variable & operator=( const variable & anOther );
		/**
		 * When we want to assign a variable a value, this operator
		 * will come in VERY handy. It takes any value of the right
		 * type and evaluates it for the value to place into this
		 * variable.
		 */
		variable & operator=( const value & anOther );

		/*******************************************************************
		 *
		 *                        Accessor Methods
		 *
		 *******************************************************************/
		/**
		 * This method expands the support for the set() methods in
		 * the value class by allowing the caller to set the value with
		 * an expression, or some other value subclass. This is not in
		 * the base value class, so we had to add it here.
		 */
		virtual bool set( bool aValue );
		virtual bool set( int aValue );
		virtual bool set( double aValue );
		virtual bool set( uint64_t aValue );
		virtual bool set( value *aValue );
		/**
		 * This method takes a value for this instance and places it into
		 * this guy if it's possible. If all goes well, then a 'true' will
		 * be returned, otherwise, a 'false' will be returned. This will be
		 * the easiest way to set values for constants, etc.
		 */
		virtual bool set( const std::string & aName, bool aValue );
		virtual bool set( const std::string & aName, int aValue );
		virtual bool set( const std::string & aName, double aValue );
		virtual bool set( const std::string & aName, uint64_t aValue );
		virtual bool set( const std::string & aName, value *aValue );
		/**
		 * This method returns the name of this variable, as defined by
		 * the caller. This should be set in one of the setters or the
		 * constructor, but it's possible that it hasn't been set, and
		 * care should be taken in processing it's value.
		 */
		virtual std::string getName() const;

		/*******************************************************************
		 *
		 *                         Utility Methods
		 *
		 *******************************************************************/
		/**
		 * Because C++ doesn't have a nice 'instanceof' operator, we
		 * need to have an efficient way to know what this particular
		 * instance is REALLY. Since we can have the base class and a
		 * few subclasses, it is necessary to put the tests in this,
		 * the base class, and then just overwrite them in the subclasses.
		 */
		virtual bool isVariable() const;

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
		 * This method checks to see if two values are equal in their
		 * contents and not their pointer values. This is how you'd likely
		 * expect equality to work.
		 */
		bool operator==( const variable & anOther ) const;
		/**
		 * This method checks to see if two values are NOT equal in their
		 * contents and not their pointer values. This is how you'd likely
		 * expect equality to work.
		 */
		bool operator!=( const variable & anOther ) const;

		/**
		 * This method looks at the value of this variable and the evaluated
		 * value of the arguments and sees if they are equivalent. If so,
		 * this operator returns 'true'. This is a very powerful operator
		 * for the variable as it allows values to be compared to variables
		 * without worrying about the different classes.
		 */
		bool operator==( const value & anOther ) const;
		/**
		 * This method looks at the value of this variable and the evaluated
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
		 * This method takes a value for this instance and places it into
		 * this guy if it's possible. The key with the "_nl" is that it
		 * means "no lock". No lock will be placed on the instance in the
		 * process of setting these values. That means the caller has to
		 * do it.
		 */
		virtual bool set_nl( value *aValue );
		virtual bool set_nl( const value & aValue );
		virtual bool set_nl( bool aValue );
		virtual bool set_nl( int aValue );
		virtual bool set_nl( double aValue );
		virtual bool set_nl( uint64_t aValue );

		/**
		 * This method simply resets the value to it's "undefined"
		 * state - clearing out any value that might be currently
		 * stored in the value. The key with the "_nl" is that it
		 * means "no lock". No lock will be placed on the instance
		 * in the process of setting these values. That means the
		 * caller has to do it.
		 */
		virtual void clear_nl();

		/**
		 * This method gets the value for this instance, and it may be quite
		 * involved in getting the value. This will be the way to get
		 * constants as well as evaluate functions and expressions.
		 *
		 * The key with the "_nl" is that it means "no lock". No lock
		 * will be placed on the instance in the process of setting
		 * these values. That means the caller has to do it.
		 */
		virtual value eval_nl();
		virtual bool evalAsBool_nl();
		virtual int evalAsInt_nl();
		virtual double evalAsDouble_nl();
		virtual uint64_t evalAsTime_nl();

		/*******************************************************************
		 *
		 *                     Subcalss Utility Methods
		 *
		 *******************************************************************/
		/**
		 * There are a lot of times that a human-readable version of
		 * this instance will come in handy. This is that method. It's
		 * not necessarily meant to be something to process, but most
		 * likely what a debugging system would want to write out for
		 * this guy.
		 *
		 * The "_nl" is for no-lock, and this is used by subclasses to
		 * get the representation of this instance variable without the
		 * locking that we'll get with the public interface.
		 */
		virtual std::string toString_nl() const;

	private:
		/**
		 * Every variable has a name, and this is the one for this guy.
		 * The name is some string, and there are very few rules about
		 * what it can be, but common sense should rule the day.
		 */
		std::string				_name;
		/**
		 * There will be times when the variable isn't a simple value,
		 * but an expression. In order to have this work properly, we
		 * need to have a pointer to the expression, and then we'll
		 * evaluate it, if necessary, in the protected evaluation
		 * methods.
		 */
		value					*_expr;
};
}		// end of namespace lkit

/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this value. It basically is just the value of the base data type
 * and puts it into the stream.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::variable & aValue );

#endif		// __LKIT_VARIABLE_H
