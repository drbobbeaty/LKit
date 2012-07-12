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
		ans = anArg[pos++]->eval();
		// run through all the rest, comparing as we go
		value	*v = NULL;
		while (pos < sz) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply look for the largest one that's not undefined
			if ((v != NULL) && !v->eval().isUndefined()) {
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
		ans = anArg[pos++]->eval();
		// run through all the rest, comparing as we go
		value	*v = NULL;
		while (pos < sz) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply look for the smallest one that's not undefined
			if ((v != NULL) && !v->eval().isUndefined()) {
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
		ans = anArg[pos++]->eval();
		// run through all the rest, comparing as we go
		value	*v = NULL;
		while (pos < sz) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply sum up all the valid values as best we can...
			if ((v != NULL) && !v->eval().isUndefined()) {
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
	return "<+>";
}






/**
 * diff() - this function simply differences the values of the arguments
 *          and returns it. The operations are all done on the
 *          values themselves.
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
value diff::eval( std::vector<value *> & anArg )
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
		ans = anArg[pos++]->eval();
		// check for unary minus
		if (pos >= sz) {
			// unary minus - just negate what we have
			ans *= -1;
		} else {
			// run through all the rest, comparing as we go
			value	*v = NULL;
			while (pos < sz) {
				// get the pointer to the next argument in the list
				v = anArg[pos];
				// simply difference all the valid values as best we can...
				if ((v != NULL) && !v->eval().isUndefined()) {
					ans -= *v;
				}
				++pos;
			}
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
std::string diff::toString() const
{
	return "<->";
}






/**
 * prod() - this function simply multiplies the values of the arguments
 *          and returns it. The operations are all done on the
 *          values themselves.
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
value prod::eval( std::vector<value *> & anArg )
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
		ans = anArg[pos++]->eval();
		// run through all the rest, comparing as we go
		value	*v = NULL;
		while (pos < sz) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply multiply all the valid values as best we can...
			if ((v != NULL) && !v->eval().isUndefined()) {
				ans *= *v;
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
std::string prod::toString() const
{
	return "<*>";
}






/**
 * quot() - this function simply divides the values of the arguments
 *          and returns it. The operations are all done on the
 *          values themselves.
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
value quot::eval( std::vector<value *> & anArg )
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
		ans = anArg[pos++]->eval();
		// run through all the rest, comparing as we go
		value	*v = NULL;
		while (pos < sz) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply divide all the valid values as best we can...
			if ((v != NULL) && !v->eval().isUndefined()) {
				ans /= *v;
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
std::string quot::toString() const
{
	return "</>";
}






/**
 * comp() - this function does all the equality/inequality functions for
 *          the values of the arguments based on the constructor argument.
 *          The default is '=='. The comparisons are all done on the
 *          values themselves.
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
value comp::eval( std::vector<value *> & anArg )
{
	// get the number of arguments to check
	size_t		sz = anArg.size();
	/**
	 * Get the initial value as the first, defined, arg - if we
	 * have ANY - we just have to find it.
	 */
	size_t		pos = 0;
	while ((pos < sz) && (anArg[pos] == NULL)) {
		++pos;
	}
	// if we have something, then get it, and process the rest
	bool		comp = true;
	size_t		cnt = 0;
	if (pos < sz) {
		value	first = anArg[pos++]->eval();
		// run through all the rest, comparing as we go
		value	*v = NULL;
		bool	keepGoing = true;
		while (keepGoing && (pos < sz)) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply check all the valid values as best we can...
			if ((v != NULL) && !v->eval().isUndefined()) {
				// make sure we count the valid values
				++cnt;
				// based on what we are, check for show stopper
				switch (_type) {
					case eEquals :
						if (first != *v) {
							comp = false;
							keepGoing = false;
						}
						break;
					case eNotEquals :
						if (first == *v) {
							comp = false;
							keepGoing = false;
						}
						break;
					case eLessThan :
						if (first < *v) {
							// compare against this value now
							first = *v;
						} else {
							comp = false;
							keepGoing = false;
						}
						break;
					case eGreaterThan :
						if (first > *v) {
							// compare against this value now
							first = *v;
						} else {
							comp = false;
							keepGoing = false;
						}
						break;
					case eLessOrEqual :
						if (first <= *v) {
							// compare against this value now
							first = *v;
						} else {
							comp = false;
							keepGoing = false;
						}
						break;
					case eGreaterOrEqual :
						if (first >= *v) {
							// compare against this value now
							first = *v;
						} else {
							comp = false;
							keepGoing = false;
						}
						break;
				}
			}
			++pos;
		}
	}
	// now make the return value for the caller
	value	ans;
	if (cnt > 0) {
		ans = comp;
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
std::string comp::toString() const
{
	switch (_type) {
		case eEquals :			return "<.eq.>"; break;
		case eNotEquals :		return "<.ne.>"; break;
		case eLessThan :		return "<.lt.>"; break;
		case eGreaterThan :		return "<.gt.>"; break;
		case eLessOrEqual :		return "<.le.>"; break;
		case eGreaterOrEqual :	return "<.ge.>"; break;
	}
	return "<.eq.>";
}






/**
 * bin() - this function does all the binary functions for the values
 *         of the arguments based on the constructor argument.
 *         The default is 'and'. The comparisons are all done on the
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
value bin::eval( std::vector<value *> & anArg )
{
	// get the number of arguments to check
	size_t		sz = anArg.size();
	size_t		pos = 0;
	// assume the test is true, and check it
	bool		test = true;
	size_t		cnt = 0;
	if (pos < sz) {
		// run through all the values, checking as we go
		value	*v = NULL;
		value	val;
		bool	keepGoing = true;
		while (keepGoing && (pos < sz)) {
			// get the pointer to the next argument in the list
			v = anArg[pos];
			// simply check all the valid values as best we can...
			if ((v != NULL) && !(val = v->eval()).isUndefined()) {
				// make sure we count the valid values
				++cnt;
				// based on what we are, check for show stopper
				switch (_type) {
					case eAnd :
						if (!val.evalAsBool()) {
							test = false;
							keepGoing = false;
						}
						break;
					case eOr :
						if (val.evalAsBool()) {
							test = true;
							keepGoing = false;
						}
						break;
					case eNot :
						// this is a unary operator
						test = !val.evalAsBool();
						keepGoing = false;
						break;
				}
			}
			++pos;
		}
	}
	// now make the return value for the caller
	value	ans;
	if (cnt > 0) {
		ans = test;
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
std::string bin::toString() const
{
	switch (_type) {
		case eAnd :		return "<.and.>"; break;
		case eOr :		return "<.or.>"; break;
		case eNot :		return "<.not.>"; break;
	}
	return "<.and.>";
}
}		// end of namespace func
}		// end of namespace lkit
