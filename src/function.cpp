/**
 * function.cpp - this file implements a simple function for the language. This
 *                will be simple call to this class where a vector of values
 *                are passed in as arguments to the function, and a single
 *                value is expected in return. Very simple.
 */

//	System Headers

//	Third-Party Headers

//	Other Headers
#include "function.h"

//	Forward Declarations

//	Private Constants

//	Private Datatypes

//	Private Data Constants



/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this guy. It basically is just the toString() method on the
 * function streamed out.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::function & aValue )
{
	aStream << aValue.toString();
	return aStream;
}
