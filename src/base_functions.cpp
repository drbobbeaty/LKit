/**
 * base_functions.cpp - this file implements the simple base functions for the
 *                      function table. These functions need to be added to
 *                      the parser with the proper names and arguments, but
 *                      then they are ready to use.
 */

//	System Headers

//	Third-Party Headers

//	Other Headers
#include "base_functions.h"

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
value max::eval( std::vector<value *> & anArg )
{
	// get the number of arguments to check
	size_t		sz = anArg.size();
	/**
	 * Get the initial value as the first, defined, arg - if we
	 * have ANY - we just have to find it.
	 */
	value		ans;
	size_t		pos = 0;
	while ((pos < sz) && (anArg[pos] == NULL)) {
		++pos;
	}
	// if we have something, then get it, and process the rest
	if (pos < sz) {
		ans = *(anArg[pos++]);
		// run through all the rest, comparing as we go
		value	*v = NULL;
		while (pos < sz) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply look for the largest one that's not undefined
			if ((v != NULL) && !v->isUndefined()) {
				ans = (*v > ans ? *v : ans);
			}
			++pos;
		}
	}
	return ans;
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
std::string max::toString() const
{
	return "<max>";
}






/**
 * min() - this function simply takes the smallest value in the argument
 *         list and returns it. The comparisons are all done on the
 *         values themselves.
 */
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
value min::eval( std::vector<value *> & anArg )
{
	// get the number of arguments to check
	size_t		sz = anArg.size();
	/**
	 * Get the initial value as the first, defined, arg - if we
	 * have ANY - we just have to find it.
	 */
	value		ans;
	size_t		pos = 0;
	while ((pos < sz) && (anArg[pos] == NULL)) {
		++pos;
	}
	// if we have something, then get it, and process the rest
	if (pos < sz) {
		ans = *(anArg[pos++]);
		// run through all the rest, comparing as we go
		value	*v = NULL;
		while (pos < sz) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply look for the smallest one that's not undefined
			if ((v != NULL) && !v->isUndefined()) {
				ans = (*v < ans ? *v : ans);
			}
			++pos;
		}
	}
	return ans;
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
std::string min::toString() const
{
	return "<min>";
}






/**
 * sum() - this function simply sums the values of the arguments
 *         and returns it. The operations are all done on the
 *         values themselves.
 */
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
value sum::eval( std::vector<value *> & anArg )
{
	// get the number of arguments to check
	size_t		sz = anArg.size();
	/**
	 * Get the initial value as the first, defined, arg - if we
	 * have ANY - we just have to find it.
	 */
	value		ans;
	size_t		pos = 0;
	while ((pos < sz) && (anArg[pos] == NULL)) {
		++pos;
	}
	// if we have something, then get it, and process the rest
	if (pos < sz) {
		ans = *(anArg[pos++]);
		// run through all the rest, comparing as we go
		value	*v = NULL;
		while (pos < sz) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply sum up all the valid values as best we can...
			if ((v != NULL) && !v->isUndefined()) {
				ans += *v;
			}
			++pos;
		}
	}
	return ans;
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
std::string sum::toString() const
{
	return "<sum>";
}
}		// end of namespace func
}		// end of namespace lkit
