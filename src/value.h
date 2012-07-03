/**
 * value.h - this file defines a simple, untyped, value. This will be the
 *           base of the variable, constant, and expression storage for
 *           the parser. This will be the interface for setting and getting
 *           values for the parsing as well as making the storage and
 *           retrieval as fast and efficient as possible.
 */
#ifndef __LKIT_VALUE_H
#define __LKIT_VALUE_H

//	System Headers
#include <stdint.h>
#include <ostream>

//	Third-Party Headers
#include <boost/smart_ptr/detail/spinlock.hpp>

//	Other Headers

//	Forward Declarations

//	Public Constants

//	Public Datatypes

//	Public Data Constants


/**
 * Main class definition
 */
namespace lkit {
class value
{
	public:
		/*******************************************************************
		 *
		 *                     Constructors/Destructor
		 *
		 *******************************************************************/
		/**
		 * This is the default constructor that assumes NOTHING - it just
		 * makes a value that can be assigned something and that's it.
		 */
		value();
		/**
		 * These constructors take the different allowed types and create
		 * values based on that argument. This sets the type as well as the
		 * actual value for this new instance.
		 */
		value( bool aValue );
		value( int aValue );
		value( double aValue );
		value( uint64_t aValue );
		/**
		 * This is the standard copy constructor that needs to be in every
		 * class to make sure that we control how many copies we have
		 * floating around in the system.
		 */
		value( const value & anOther );
		/**
		 * This is the standard destructor and needs to be virtual to make
		 * sure that if we subclass off this, the right destructor will be
		 * called.
		 */
		virtual ~value();

		/**
		 * When we process the result of an equality we need to make sure
		 * that we do this right by always having an equals operator on
		 * all classes.
		 */
		value & operator=( const value & anOther );
		/**
		 * These versions of the assignment operator will take the value
		 * and the implied type and save this information into this
		 * instance.
		 */
		value & operator=( bool aValue );
		value & operator=( int aValue );
		value & operator=( double aValue );
		value & operator=( uint64_t aValue );

		/*******************************************************************
		 *
		 *                        Accessor Methods
		 *
		 *******************************************************************/
		/**
		 * These are the different types of values that can be stored in
		 * this instance. They are pretty simple to understand, and there
		 * is pretty good casting possibilities as well.
		 */
		enum value_type {
			eUnknown = -1,
			eBool = 0,
			eInt = 1,
			eDouble = 2,
			eTime = 3,
		};

		/**
		 * This method takes a value for this instance and places it into
		 * this guy if it's possible. If all goes well, then a 'true' will
		 * be returned, otherwise, a 'false' will be returned. This will be
		 * the easiest way to set values for constants, etc.
		 */
		virtual bool set( bool aValue );
		virtual bool set( int aValue );
		virtual bool set( double aValue );
		virtual bool set( uint64_t aValue );

		/**
		 * This method gets the value for this instance, and it may be quite
		 * involved in getting the value. This will be the way to get
		 * constants as well as evaluate functions and expressions.
		 */
		virtual bool evalAsBool();
		virtual int evalAsInt();
		virtual double evalAsDouble();
		virtual uint64_t evalAsTime();

		/**
		 * These operators allow me to use the values as regular types
		 * when casting to the right type. The trick is that if the value
		 * is not the casted type, an exception will be thrown due to an
		 * illegal cast.
		 */
		operator bool();
		operator int();
		operator double();
		operator uint64_t();

		/**
		 * These methods can be used to interrogate the instance to see
		 * what it's currently holding. This can change at any time, so
		 * you have to realize that, but it's a good way to know if the
		 * value is holding what you think it's holding.
		 */
		bool isUndefined() const;
		bool isInteger() const;
		bool isDouble() const;
		bool isTime() const;

		/**
		 * This method simply resets the value to it's "undefined"
		 * state - clearing out any value that might be currently
		 * stored in the value.
		 */
		void clear();

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
		bool operator==( const value & anOther ) const;
		/**
		 * These versions of the '==' operator will handle the fixed
		 * value equality checks where we have a known data type.
		 */
		bool operator==( bool aValue );
		bool operator==( int aValue );
		bool operator==( double aValue );
		bool operator==( uint64_t aValue );
		/**
		 * This method checks to see if two values are NOT equal in their
		 * contents and not their pointer values. This is how you'd likely
		 * expect equality to work.
		 */
		bool operator!=( const value & anOther ) const;
		/**
		 * These versions of the '!=' operator will handle the fixed
		 * value equality checks where we have a known data type.
		 */
		bool operator!=( bool aValue );
		bool operator!=( int aValue );
		bool operator!=( double aValue );
		bool operator!=( uint64_t aValue );
		/**
		 * These methods complete the inequality tests for the value and
		 * we need these as we might be doing a lot of testing and this
		 * makes it a lot easier than converting to the right type and
		 * then doing a low-level test.
		 */
		bool operator<( const value & anOther ) const;
		bool operator<=( const value & anOther ) const;
		bool operator>( const value & anOther ) const;
		bool operator>=( const value & anOther ) const;
		/**
		 * These versions of the operators will handle the fixed
		 * value inequality checks where we have a known data type.
		 */
		bool operator<( bool aValue ) const;
		bool operator<( int aValue ) const;
		bool operator<( double aValue ) const;
		bool operator<( uint64_t aValue ) const;

		bool operator<=( bool aValue ) const;
		bool operator<=( int aValue ) const;
		bool operator<=( double aValue ) const;
		bool operator<=( uint64_t aValue ) const;

		bool operator>( bool aValue ) const;
		bool operator>( int aValue ) const;
		bool operator>( double aValue ) const;
		bool operator>( uint64_t aValue ) const;

		bool operator>=( bool aValue ) const;
		bool operator>=( int aValue ) const;
		bool operator>=( double aValue ) const;
		bool operator>=( uint64_t aValue ) const;

		/*
		 * These operators are the convenience assignment operators for
		 * the value and are meant to make it easy to use these guys in
		 * code. If the operation doesn't make sense for the data an
		 * exception will be thrown.
		 */
		value & operator+=( const value & anOther );
		value & operator+=( bool aValue );
		value & operator+=( int aValue );
		value & operator+=( double aValue );
		value & operator+=( uint64_t aValue );

		value & operator-=( const value & anOther );
		value & operator-=( bool aValue );
		value & operator-=( int aValue );
		value & operator-=( double aValue );
		value & operator-=( uint64_t aValue );

		value & operator*=( const value & anOther );
		value & operator*=( bool aValue );
		value & operator*=( int aValue );
		value & operator*=( double aValue );
		value & operator*=( uint64_t aValue );

		value & operator/=( const value & anOther );
		value & operator/=( bool aValue );
		value & operator/=( int aValue );
		value & operator/=( double aValue );
		value & operator/=( uint64_t aValue );

		/*
		 * There are times that values will be used in mathematical
		 * expressions by themselves, these operator functions will make
		 * it very easy for the user to do simple 'a + b' coding on
		 * even the most complex structures.
		 */
		friend value operator+( const value & aVar, const value & anOtherVar );
		friend value operator-( const value & aVar, const value & anOtherVar );
		friend value operator*( const value & aVar, const value & anOtherVar );
		friend value operator/( const value & aVar, const value & anOtherVar );

		/*
		 * These operator functions will allow the mixed-mode math with
		 * value casting the result up to a value in each case. First,
		 * start with simple addition, then subtraction, then multiplication
		 * and finally division.
		 */
		friend value operator+( const value & aVar, bool aValue );
		friend value operator+( const value & aVar, int aValue );
		friend value operator+( const value & aVar, double aValue );
		friend value operator+( const value & aVar, uint64_t aValue );
		friend value operator+( bool aValue, const value & aVar );
		friend value operator+( int aValue, const value & aVar );
		friend value operator+( double aValue, const value & aVar );
		friend value operator+( uint64_t aValue, const value & aVar );

		friend value operator-( const value & aVar, bool aValue );
		friend value operator-( const value & aVar, int aValue );
		friend value operator-( const value & aVar, double aValue );
		friend value operator-( const value & aVar, uint64_t aValue );
		friend value operator-( bool aValue, const value & aVar );
		friend value operator-( int aValue, const value & aVar );
		friend value operator-( double aValue, const value & aVar );
		friend value operator-( uint64_t aValue, const value & aVar );

		friend value operator*( const value & aVar, bool aValue );
		friend value operator*( const value & aVar, int aValue );
		friend value operator*( const value & aVar, double aValue );
		friend value operator*( const value & aVar, uint64_t aValue );
		friend value operator*( bool aValue, const value & aVar );
		friend value operator*( int aValue, const value & aVar );
		friend value operator*( double aValue, const value & aVar );
		friend value operator*( uint64_t aValue, const value & aVar );

		friend value operator/( const value & aVar, bool aValue );
		friend value operator/( const value & aVar, int aValue );
		friend value operator/( const value & aVar, double aValue );
		friend value operator/( const value & aVar, uint64_t aValue );
		friend value operator/( bool aValue, const value & aVar );
		friend value operator/( int aValue, const value & aVar );
		friend value operator/( double aValue, const value & aVar );
		friend value operator/( uint64_t aValue, const value & aVar );

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
		void clear_nl();

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

		/**
		 * This method returns the pointer to the mutex for this instance,
		 * and is quite useful for those times the subclasses need to be able
		 * to lock something up while they do their thing.
		 */
		boost::detail::spinlock & mutex() const;

	private:
		/**
		 * Every value can be a bool, int, double or time, and we need to
		 * be able to store the individual values as well as know what it
		 * is that we are really storing.
		 */
		value_type		_type;
		union {
			bool		_boolValue;
			int			_intValue;
			double		_doubleValue;
			uint64_t	_timeValue;
		};
		// ...and a spinlock to protect the access to the value
		mutable boost::detail::spinlock		_mutex;
};
}		// end of namespace lkit

/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this value. It basically is just the value of the base data type
 * and puts it into the stream.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::value & aValue );

#endif		// __LKIT_VALUE_H
