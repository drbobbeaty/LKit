/**
 * function.h - this file defines a simple function for the language. This
 *              will be simple call to this class where a vector of values
 *              are passed in as arguments to the function, and a single
 *              value is expected in return. Very simple.
 */
#ifndef __LKIT_FUNCTION_H
#define __LKIT_FUNCTION_H

//	System Headers
#include <stdint.h>
#include <ostream>
#include <vector>

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
class function
{
	public:
		/*******************************************************************
		 *
		 *                     Constructors/Destructor
		 *
		 *******************************************************************/
		/**
		 * This is the default constructor that assumes NOTHING - it just
		 * makes a function that can be called. This is all that simple
		 * functions like max(), min(), etc. need.
		 */
		function()
		{
		}


		/**
		 * This is the standard copy constructor that needs to be in every
		 * class to make sure that we control how many copies we have
		 * floating around in the system.
		 */
		function( const function & anOther )
		{
			// let the '=' operator do the heavy lifting...
			*this = anOther;
		}


		/**
		 * This is the standard destructor and needs to be virtual to make
		 * sure that if we subclass off this, the right destructor will be
		 * called.
		 */
		virtual ~function()
		{
		}


		/**
		 * When we process the result of an equality we need to make sure
		 * that we do this right by always having an equals operator on
		 * all classes.
		 */
		function & operator=( const function & anOther )
		{
			if (this != & anOther) {
				// this is where we might clean things up
			}
			return *this;
		}


		/*******************************************************************
		 *
		 *                        Accessor Methods
		 *
		 *******************************************************************/

		/*******************************************************************
		 *
		 *                       Evaluation Methods
		 *
		 *******************************************************************/
		/**
		 * This is the main evaluation point for the function. It takes a
		 * vector of values and returns a value - simple. How it does this
		 * is entirely up to the developer of that function.
		 */
		virtual value eval( std::vector<value *> & anArg ) = 0;


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
		virtual std::string toString() const
		{
			return "<function>";
		}


		/**
		 * When we have a custom '==' operator, it's wise to have a hash
		 * method as well. This makes it much easier to used hash-based
		 * containers like boost, etc. It's a requirement that if two
		 * instances are '==', then their hash() methods must be '==' as
		 * well.
		 */
		virtual size_t hash() const
		{
			return 0;
		}


		/**
		 * This method checks to see if two functions are equal in their
		 * contents and not their pointer values. This is how you'd likely
		 * expect equality to work.
		 */
		bool operator==( const function & anOther ) const
		{
			// right now, identity is the only equality we know
			return (this == & anOther);
		}


		/**
		 * This method checks to see if two functions are NOT equal in their
		 * contents and not their pointer values. This is how you'd likely
		 * expect equality to work.
		 */
		bool operator!=( const function & anOther ) const
		{
			return !operator==(anOther);
		}
};
}		// end of namespace lkit

/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this guy. It basically is just the toString() method on the
 * function streamed out.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::function & aValue );

#endif		// __LKIT_FUNCTION_H
