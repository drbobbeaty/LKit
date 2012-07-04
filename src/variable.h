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
		 * These constructors take the different allowed types and create
		 * variables based on that argument. This sets the name, type as
		 * well as the actual value for this new instance.
		 */
		variable( const std::string & aName, bool aValue );
		variable( const std::string & aName, int aValue );
		variable( const std::string & aName, double aValue );
		variable( const std::string & aName, uint64_t aValue );
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
		 * This method takes a value for this instance and places it into
		 * this guy if it's possible. If all goes well, then a 'true' will
		 * be returned, otherwise, a 'false' will be returned. This will be
		 * the easiest way to set values for constants, etc.
		 */
		virtual bool set( const std::string & aName, bool aValue );
		virtual bool set( const std::string & aName, int aValue );
		virtual bool set( const std::string & aName, double aValue );
		virtual bool set( const std::string & aName, uint64_t aValue );
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

	private:
		/**
		 * Every variable has a name, and this is the one for this guy.
		 * The name is some string, and there are very few rules about
		 * what it can be, but common sense should rule the day.
		 */
		std::string				_name;
};
}		// end of namespace lkit

/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this value. It basically is just the value of the base data type
 * and puts it into the stream.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::variable & aValue );

#endif		// __LKIT_VARIABLE_H
