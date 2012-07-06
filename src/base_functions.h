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
		virtual function *clone() const { return new max(*this); }
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
		virtual function *clone() const { return new min(*this); }
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
		virtual function *clone() const { return new sum(*this); }
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



/**
 * diff() - this function simply differences the values of the arguments
 *          and returns it. The operations are all done on the
 *          values themselves.
 */
class diff :
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
		diff() { };
		diff( const diff & anOther ) { };
		virtual function *clone() const { return new diff(*this); }
		virtual ~diff() { };
		diff & operator=( const diff & anOther ) { return *this; };

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
 * prod() - this function simply multiplies the values of the arguments
 *          and returns it. The operations are all done on the
 *          values themselves.
 */
class prod :
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
		prod() { };
		prod( const prod & anOther ) { };
		virtual function *clone() const { return new prod(*this); }
		virtual ~prod() { };
		prod & operator=( const prod & anOther ) { return *this; };

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
 * quot() - this function simply divides the values of the arguments
 *          and returns it. The operations are all done on the
 *          values themselves.
 */
class quot :
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
		quot() { };
		quot( const quot & anOther ) { };
		virtual function *clone() const { return new quot(*this); }
		virtual ~quot() { };
		quot & operator=( const quot & anOther ) { return *this; };

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
 * comp() - this function does all the equality/inequality functions for
 *          the values of the arguments based on the constructor argument.
 *          The default is '=='. The comparisons are all done on the
 *          values themselves.
 */
class comp :
	public function
{
	public:
		/**
		 * These are the different types of comparisons that this function
		 * can perform. The default, no-arg constructor is eEquals, but the
		 * caller can create any one they desire.
		 */
		enum comp_type {
			eEquals = 0,
			eNotEquals,
			eLessThan,
			eGreaterThan,
			eLessOrEqual,
			eGreaterOrEqual
		};
		/*******************************************************************
		 *
		 *                     Constructors/Destructor
		 *
		 *******************************************************************/
		/**
		 * This function has no state, so these are all very simple to
		 * stub out here in the header.
		 */
		comp() : _type(eEquals) { };
		comp( comp_type aType ) : _type(aType) { };
		comp( const comp & anOther ) : _type(anOther._type) { };
		virtual function *clone() const { return new comp(*this); }
		virtual ~comp() { };
		comp & operator=( const comp & anOther )
		{
			_type = anOther._type;
			return *this;
		};

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
	private:
		// this is the type of comparison this instance is doing
		comp_type		_type;
};



/**
 * bin() - this function does all the binary functions for the values
 *         of the arguments based on the constructor argument.
 *         The default is 'and'. The comparisons are all done on the
 *         values themselves.
 */
class bin :
	public function
{
	public:
		/**
		 * These are the different types of operations that this function
		 * can perform. The default, no-arg constructor is eAnd, but the
		 * caller can create any one they desire.
		 */
		enum bin_type {
			eAnd = 0,
			eOr,
			eNot
		};
		/*******************************************************************
		 *
		 *                     Constructors/Destructor
		 *
		 *******************************************************************/
		/**
		 * This function has no state, so these are all very simple to
		 * stub out here in the header.
		 */
		bin() : _type(eAnd) { };
		bin( bin_type aType ) : _type(aType) { };
		bin( const bin & anOther ) : _type(anOther._type) { };
		virtual function *clone() const { return new bin(*this); }
		virtual ~bin() { };
		bin & operator=( const bin & anOther )
		{
			_type = anOther._type;
			return *this;
		};

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
	private:
		// this is the type of operation this instance is doing
		bin_type			_type;
};
}		// end of namespace func
}		// end of namespace lkit

#endif		// __LKIT_BASE_FUNCTIONS_H
