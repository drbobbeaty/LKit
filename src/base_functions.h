/**
 * base_functions.h - this file defines the simple base functions for the
 *                    function table. These functions need to be added to
 *                    the parser with the proper names and arguments, but
 *                    then they are ready to use.
 */
#ifndef __LKIT_BASE_FUNCTIONS_H
#define __LKIT_BASE_FUNCTIONS_H

//	System Headers

//	Third-Party Headers

//	Other Headers
#include "function.h"

//	Forward Declarations

//	Public Constants

//	Public Datatypes

//	Public Data Constants


namespace lkit {
namespace func {

/**
 * max() - this function simply takes the largest value in the argument
 *         list and returns it. The comparisons are all done on the
 *         values themselves.
 */
class max :
	public function
{
	public:
		/*******************************************************************
		 *
		 *                     Constructors/Destructor
		 *
		 *******************************************************************/
		/**
		 * This function has no state, so these are all very simple to
		 * stub out here in the header.
		 */
		max() { };
		max( const max & anOther ) { };
		virtual ~max() { };
		max & operator=( const max & anOther ) { return *this; };

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
		virtual value eval( std::vector<value *> & anArg );

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
};



/**
 * min() - this function simply takes the smallest value in the argument
 *         list and returns it. The comparisons are all done on the
 *         values themselves.
 */
class min :
	public function
{
	public:
		/*******************************************************************
		 *
		 *                     Constructors/Destructor
		 *
		 *******************************************************************/
		/**
		 * This function has no state, so these are all very simple to
		 * stub out here in the header.
		 */
		min() { };
		min( const min & anOther ) { };
		virtual ~min() { };
		min & operator=( const min & anOther ) { return *this; };

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
		virtual value eval( std::vector<value *> & anArg );

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
};



/**
 * sum() - this function simply sums the values of the arguments
 *         and returns it. The operations are all done on the
 *         values themselves.
 */
class sum :
	public function
{
	public:
		/*******************************************************************
		 *
		 *                     Constructors/Destructor
		 *
		 *******************************************************************/
		/**
		 * This function has no state, so these are all very simple to
		 * stub out here in the header.
		 */
		sum() { };
		sum( const sum & anOther ) { };
		virtual ~sum() { };
		sum & operator=( const sum & anOther ) { return *this; };

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
		virtual value eval( std::vector<value *> & anArg );

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
};
}		// end of namespace func
}		// end of namespace lkit

#endif		// __LKIT_BASE_FUNCTIONS_H
