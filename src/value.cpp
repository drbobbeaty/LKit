/**
 * value.cpp - this file implements a simple, typed, value. This will be the
 *             base of the variable, constant, and expression storage for
 *             the parser. This will be the interface for setting and getting
 *             values for the parsing as well as making the storage and
 *             retrieval as fast and efficient as possible.
 */

//	System Headers
#include <sstream>

//	Third-Party Headers
#include <boost/functional/hash.hpp>

//	Other Headers
#include "value.h"

//	Forward Declarations

//	Private Constants

//	Private Datatypes

//	Private Data Constants



namespace lkit {
/*******************************************************************
 *
 *                     Constructors/Destructor
 *
 *******************************************************************/
/**
 * This is the default constructor that assumes NOTHING - it just
 * makes a value that can be assigned something and that's it.
 */
value::value() :
	_type(eUnknown),
	_intValue(0),
	_mutex()
{
}


/**
 * These constructors take the different allowed types and create
 * values based on that argument. This sets the type as well as the
 * actual value for this new instance.
 */
value::value( bool aValue ) :
	_type(eBool),
	_boolValue(aValue),
	_mutex()
{
}


value::value( int aValue ) :
	_type(eInt),
	_intValue(aValue),
	_mutex()
{
}


value::value( double aValue ) :
	_type(eDouble),
	_doubleValue(aValue),
	_mutex()
{
}


value::value( uint64_t aValue ) :
	_type(eTime),
	_timeValue(aValue),
	_mutex()
{
}


/**
 * This is the standard copy constructor that needs to be in every
 * class to make sure that we control how many copies we have
 * floating around in the system.
 */
value::value( const value & anOther ) :
	_type(eUnknown),
	_intValue(0),
	_mutex()
{
	// let the '=' operator do the heavy lifting...
	*this = anOther;
}


/**
 * This is the standard clone method that we'll have for all
 * classes so that it's possible to clone the value without
 * having to worry about memory management, etc.
 */
value *value::clone() const
{
	return new value(*this);
}


/**
 * This is the standard destructor and needs to be virtual to make
 * sure that if we subclass off this, the right destructor will be
 * called.
 */
value::~value()
{
}


/**
 * When we process the result of an equality we need to make sure
 * that we do this right by always having an equals operator on
 * all classes.
 */
value & value::operator=( const value & anOther )
{
	if (this != & anOther) {
		_type = anOther._type;
		switch (_type) {
			case eUnknown:	_intValue = 0;							break;
			case eBool:		_boolValue = anOther._boolValue;		break;
			case eInt:		_intValue = anOther._intValue;			break;
			case eDouble:	_doubleValue = anOther._doubleValue;	break;
			case eTime:		_timeValue = anOther._timeValue;		break;
		}
	}
	return *this;
}


/**
 * These versions of the assignment operator will take the value
 * and the implied type and save this information into this
 * instance.
 */
value & value::operator=( bool aValue )
{
	set(aValue);
	return *this;
}


value & value::operator=( int aValue )
{
	set(aValue);
	return *this;
}


value & value::operator=( double aValue )
{
	set(aValue);
	return *this;
}


value & value::operator=( uint64_t aValue )
{
	set(aValue);
	return *this;
}


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
bool value::set( bool aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return set_nl(aValue);
}


bool value::set( int aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return set_nl(aValue);
}


bool value::set( double aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return set_nl(aValue);
}


bool value::set( uint64_t aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return set_nl(aValue);
}


/**
 * This method gets the value for this instance, and it may be quite
 * involved in getting the value. This will be the way to get
 * constants as well as evaluate functions and expressions.
 */
bool value::evalAsBool()
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return evalAsBool_nl();
}


int value::evalAsInt()
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return evalAsInt_nl();
}


double value::evalAsDouble()
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return evalAsDouble_nl();
}


uint64_t value::evalAsTime()
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return evalAsTime_nl();
}


/**
 * These operators allow me to use the values as regular types
 * when casting to the right type. The trick is that if the value
 * is not the casted type, an exception will be thrown due to an
 * illegal cast.
 */
value::operator bool()
{
	return evalAsBool();
}


value::operator int()
{
	return evalAsInt();
}


value::operator double()
{
	return evalAsDouble();
}


value::operator uint64_t()
{
	return evalAsTime();
}


/**
 * These methods can be used to interrogate the instance to see
 * what it's currently holding. This can change at any time, so
 * you have to realize that, but it's a good way to know if the
 * value is holding what you think it's holding.
 */
bool value::isUndefined() const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return (_type == eUnknown);
}


bool value::isInteger() const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return (_type == eInt);
}


bool value::isDouble() const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return (_type == eDouble);
}


bool value::isTime() const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	return (_type == eTime);
}


/**
 * This method simply resets the value to it's "undefined"
 * state - clearing out any value that might be currently
 * stored in the value.
 */
void value::clear()
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	clear_nl();
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
std::string value::toString() const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	std::ostringstream	msg;
	switch (_type) {
		case eUnknown:
			msg << "(unknown)";
			break;
		case eBool:
			msg << "(bool) " << (_boolValue ? "true" : "false");
			break;
		case eInt:
			msg << "(int) " << _intValue;
			break;
		case eDouble:
			msg << "(double) " << _doubleValue;
			break;
		case eTime:
			msg << "(uint64_t) " << _timeValue;
			break;
	}
	return msg.str();
}


/**
 * When we have a custom '==' operator, it's wise to have a hash
 * method as well. This makes it much easier to used hash-based
 * containers like boost, etc. It's a requirement that if two
 * instances are '==', then their hash() methods must be '==' as
 * well.
 */
size_t value::hash() const
{
	size_t	ans = 0;
	switch (_type) {
		case eUnknown:											break;
		case eBool:		ans = boost::hash_value(_boolValue);	break;
		case eInt:		ans = boost::hash_value(_intValue);		break;
		case eDouble:	ans = boost::hash_value(_doubleValue);	break;
		case eTime:		ans = boost::hash_value(_timeValue);	break;
	}
	return ans;
}


/**
 * This method checks to see if two values are equal in their
 * contents and not their pointer values. This is how you'd likely
 * expect equality to work.
 */
bool value::operator==( const value & anOther ) const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	bool		equals = (this == & anOther);
	if (!equals && (_type == anOther._type)) {
		switch (_type) {
			case eUnknown:
				equals = true;
				break;
			case eBool:
				equals = (_boolValue == anOther._boolValue);
				break;
			case eInt:
				equals = (_intValue == anOther._intValue);
				break;
			case eDouble:
				equals = (_doubleValue == anOther._doubleValue);
				break;
			case eTime:
				equals = (_timeValue == anOther._timeValue);
				break;
		}
	}
	return equals;
}


/**
 * These versions of the '==' operator will handle the fixed
 * value equality checks where we have a known data type.
 */
bool value::operator==( bool aValue )
{
	return (evalAsBool() == aValue);
}


bool value::operator==( int aValue )
{
	return (evalAsInt() == aValue);
}


bool value::operator==( double aValue )
{
	return (evalAsDouble() == aValue);
}


bool value::operator==( uint64_t aValue )
{
	return (evalAsTime() == aValue);
}


/**
 * This method checks to see if two values are NOT equal in their
 * contents and not their pointer values. This is how you'd likely
 * expect equality to work.
 */
bool value::operator!=( const value & anOther ) const
{
	return !operator==(anOther);
}


/**
 * These versions of the '!=' operator will handle the fixed
 * value equality checks where we have a known data type.
 */
bool value::operator!=( bool aValue )
{
	return !operator==(aValue);
}


bool value::operator!=( int aValue )
{
	return !operator==(aValue);
}


bool value::operator!=( double aValue )
{
	return !operator==(aValue);
}


bool value::operator!=( uint64_t aValue )
{
	return !operator==(aValue);
}


/**
 * These methods complete the inequality tests for the value and
 * we need these as we might be doing a lot of testing and this
 * makes it a lot easier than converting to the right type and
 * then doing a low-level test.
 */
bool value::operator<( const value & anOther ) const
{
	bool	test = false;
	switch (anOther._type) {
		case eUnknown:
			break;
		case eBool:
			test = operator<(anOther._boolValue);
			break;
		case eInt:
			test = operator<(anOther._intValue);
			break;
		case eDouble:
			test = operator<(anOther._doubleValue);
			break;
		case eTime:
			test = operator<(anOther._timeValue);
			break;
	}
	return test;
}


bool value::operator<=( const value & anOther ) const
{
	return !operator>(anOther);
}


bool value::operator>( const value & anOther ) const
{
	bool	test = false;
	switch (anOther._type) {
		case eUnknown:
			break;
		case eBool:
			test = operator>(anOther._boolValue);
			break;
		case eInt:
			test = operator>(anOther._intValue);
			break;
		case eDouble:
			test = operator>(anOther._doubleValue);
			break;
		case eTime:
			test = operator>(anOther._timeValue);
			break;
	}
	return test;
}


bool value::operator>=( const value & anOther ) const
{
	return !operator<(anOther);
}


/**
 * These versions of the operators will handle the fixed
 * value inequality checks where we have a known data type.
 */
bool value::operator<( bool aValue ) const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	bool		test = false;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			test = (!_boolValue && aValue);
			break;
		case eInt:
			test = (_intValue < (aValue ? 1 : 0));
			break;
		case eDouble:
			test = (_doubleValue < (aValue ? 1.0 : 0.0));
			break;
		case eTime:
			test = (_timeValue < (aValue ? 1 : 0));
			break;
	}
	return test;
}


bool value::operator<( int aValue ) const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	bool		test = false;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			test = ((_boolValue ? 1 : 0) < aValue);
			break;
		case eInt:
			test = (_intValue < aValue);
			break;
		case eDouble:
			test = (_doubleValue < aValue);
			break;
		case eTime:
			test = (_timeValue < (uint64_t)aValue);
			break;
	}
	return test;
}


bool value::operator<( double aValue ) const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	bool		test = false;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			test = ((_boolValue ? 1.0 : 0.0) < aValue);
			break;
		case eInt:
			test = (_intValue < aValue);
			break;
		case eDouble:
			test = (_doubleValue < aValue);
			break;
		case eTime:
			test = (_timeValue < (uint64_t)aValue);
			break;
	}
	return test;
}


bool value::operator<( uint64_t aValue ) const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	bool		test = false;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			test = ((_boolValue ? 1 : 0) < aValue);
			break;
		case eInt:
			test = (_intValue < (int)aValue);
			break;
		case eDouble:
			test = (_doubleValue < aValue);
			break;
		case eTime:
			test = (_timeValue < aValue);
			break;
	}
	return test;
}


bool value::operator<=( bool aValue ) const
{
	return !operator>(aValue);
}


bool value::operator<=( int aValue ) const
{
	return !operator>(aValue);
}


bool value::operator<=( double aValue ) const
{
	return !operator>(aValue);
}


bool value::operator<=( uint64_t aValue ) const
{
	return !operator>(aValue);
}


bool value::operator>( bool aValue ) const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	bool		test = false;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			test = (_boolValue && !aValue);
			break;
		case eInt:
			test = (_intValue > (aValue ? 1 : 0));
			break;
		case eDouble:
			test = (_doubleValue > (aValue ? 1.0 : 0.0));
			break;
		case eTime:
			test = (_timeValue > (aValue ? 1 : 0));
			break;
	}
	return test;
}


bool value::operator>( int aValue ) const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	bool		test = false;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			test = ((_boolValue ? 1 : 0) > aValue);
			break;
		case eInt:
			test = (_intValue > aValue);
			break;
		case eDouble:
			test = (_doubleValue > aValue);
			break;
		case eTime:
			test = (_timeValue > (uint64_t)aValue);
			break;
	}
	return test;
}


bool value::operator>( double aValue ) const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	bool		test = false;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			test = ((_boolValue ? 1.0 : 0.0) > aValue);
			break;
		case eInt:
			test = (_intValue > aValue);
			break;
		case eDouble:
			test = (_doubleValue > aValue);
			break;
		case eTime:
			test = (_timeValue > (uint64_t)aValue);
			break;
	}
	return test;
}


bool value::operator>( uint64_t aValue ) const
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	bool		test = false;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			test = ((_boolValue ? 1 : 0) > aValue);
			break;
		case eInt:
			test = (_intValue > (int)aValue);
			break;
		case eDouble:
			test = (_doubleValue > aValue);
			break;
		case eTime:
			test = (_timeValue > aValue);
			break;
	}
	return test;
}


bool value::operator>=( bool aValue ) const
{
	return !operator<(aValue);
}


bool value::operator>=( int aValue ) const
{
	return !operator<(aValue);
}


bool value::operator>=( double aValue ) const
{
	return !operator<(aValue);
}


bool value::operator>=( uint64_t aValue ) const
{
	return !operator<(aValue);
}


/*
 * These operators are the convenience assignment operators for
 * the value and are meant to make it easy to use these guys in
 * code. If the operation doesn't make sense for the data an
 * exception will be thrown.
 */
value & value::operator+=( const value & anOther )
{
	switch (anOther._type) {
		case eUnknown:
			break;
		case eBool:
			operator+=(anOther._boolValue);
			break;
		case eInt:
			operator+=(anOther._intValue);
			break;
		case eDouble:
			operator+=(anOther._doubleValue);
			break;
		case eTime:
			operator+=(anOther._timeValue);
			break;
	}
	return *this;
}


value & value::operator+=( bool aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			set_nl(aValue);
			break;
		case eBool:
			_boolValue = (_boolValue ^ aValue);
			break;
		case eInt:
			_intValue += (aValue ? 1 : 0);
			break;
		case eDouble:
			_doubleValue += (aValue ? 1.0 : 0.0);
			break;
		case eTime:
			_timeValue += (aValue ? 1 : 0);
			break;
	}
	return *this;
}


value & value::operator+=( int aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			set_nl(aValue);
			break;
		case eBool:
			_boolValue = (((_boolValue ? 1 : 0) + aValue) != 0);
			break;
		case eInt:
			_intValue += aValue;
			break;
		case eDouble:
			_doubleValue += aValue;
			break;
		case eTime:
			_timeValue += aValue;
			break;
	}
	return *this;
}


value & value::operator+=( double aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			set_nl(aValue);
			break;
		case eBool:
			_boolValue = (((_boolValue ? 1.0 : 0.0) + aValue) != 0.0);
			break;
		case eInt:
			_intValue += (int)aValue;
			break;
		case eDouble:
			_doubleValue += aValue;
			break;
		case eTime:
			_timeValue += aValue;
			break;
	}
	return *this;
}


value & value::operator+=( uint64_t aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			set_nl(aValue);
			break;
		case eBool:
			_boolValue = (((_boolValue ? 1 : 0) + aValue) != 0);
			break;
		case eInt:
			_intValue += aValue;
			break;
		case eDouble:
			_doubleValue += aValue;
			break;
		case eTime:
			_timeValue += aValue;
			break;
	}
	return *this;
}


value & value::operator-=( const value & anOther )
{
	switch (anOther._type) {
		case eUnknown:
			break;
		case eBool:
			operator-=(anOther._boolValue);
			break;
		case eInt:
			operator-=(anOther._intValue);
			break;
		case eDouble:
			operator-=(anOther._doubleValue);
			break;
		case eTime:
			operator-=(anOther._timeValue);
			break;
	}
	return *this;
}


value & value::operator-=( bool aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			set_nl(!aValue);
			break;
		case eBool:
			_boolValue = (_boolValue ^ aValue);
			break;
		case eInt:
			_intValue -= (aValue ? 1 : 0);
			break;
		case eDouble:
			_doubleValue -= (aValue ? 1.0 : 0.0);
			break;
		case eTime:
			_timeValue -= (aValue ? 1 : 0);
			break;
	}
	return *this;
}


value & value::operator-=( int aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			set_nl(-1 * aValue);
			break;
		case eBool:
			_boolValue = (((_boolValue ? 1 : 0) - aValue) != 0);
			break;
		case eInt:
			_intValue -= aValue;
			break;
		case eDouble:
			_doubleValue -= aValue;
			break;
		case eTime:
			_timeValue -= aValue;
			break;
	}
	return *this;
}


value & value::operator-=( double aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			set_nl(-1.0 * aValue);
			break;
		case eBool:
			_boolValue = (((_boolValue ? 1.0 : 0.0) - aValue) != 0.0);
			break;
		case eInt:
			_intValue -= (int)aValue;
			break;
		case eDouble:
			_doubleValue -= aValue;
			break;
		case eTime:
			_timeValue -= aValue;
			break;
	}
	return *this;
}


value & value::operator-=( uint64_t aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			set_nl(-1 * aValue);
			break;
		case eBool:
			_boolValue = (((_boolValue ? 1 : 0) - aValue) != 0);
			break;
		case eInt:
			_intValue -= aValue;
			break;
		case eDouble:
			_doubleValue -= aValue;
			break;
		case eTime:
			_timeValue -= aValue;
			break;
	}
	return *this;
}


value & value::operator*=( const value & anOther )
{
	switch (anOther._type) {
		case eUnknown:
			break;
		case eBool:
			operator*=(anOther._boolValue);
			break;
		case eInt:
			operator*=(anOther._intValue);
			break;
		case eDouble:
			operator*=(anOther._doubleValue);
			break;
		case eTime:
			operator*=(anOther._timeValue);
			break;
	}
	return *this;
}


value & value::operator*=( bool aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			_boolValue = (_boolValue && aValue);
			break;
		case eInt:
			_intValue *= (aValue ? 1 : 0);
			break;
		case eDouble:
			_doubleValue *= (aValue ? 1.0 : 0.0);
			break;
		case eTime:
			_timeValue *= (aValue ? 1 : 0);
			break;
	}
	return *this;
}


value & value::operator*=( int aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			_boolValue = (_boolValue && (aValue != 0));
			break;
		case eInt:
			_intValue *= aValue;
			break;
		case eDouble:
			_doubleValue *= aValue;
			break;
		case eTime:
			_timeValue *= aValue;
			break;
	}
	return *this;
}


value & value::operator*=( double aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			_boolValue = (_boolValue && (aValue != 0.0));
			break;
		case eInt:
			_intValue *= (int)aValue;
			break;
		case eDouble:
			_doubleValue *= aValue;
			break;
		case eTime:
			_timeValue *= aValue;
			break;
	}
	return *this;
}


value & value::operator*=( uint64_t aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			_boolValue = (_boolValue && (aValue != 0));
			break;
		case eInt:
			_intValue *= aValue;
			break;
		case eDouble:
			_doubleValue *= aValue;
			break;
		case eTime:
			_timeValue *= aValue;
			break;
	}
	return *this;
}


value & value::operator/=( const value & anOther )
{
	switch (anOther._type) {
		case eUnknown:
			clear();
			break;
		case eBool:
			operator/=(anOther._boolValue);
			break;
		case eInt:
			operator/=(anOther._intValue);
			break;
		case eDouble:
			operator/=(anOther._doubleValue);
			break;
		case eTime:
			operator/=(anOther._timeValue);
			break;
	}
	return *this;
}


value & value::operator/=( bool aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			_boolValue = !(_boolValue ^ aValue);
			break;
		case eInt:
		case eDouble:
		case eTime:
			if (!aValue) {
				clear_nl();
			}
			break;
	}
	return *this;
}


value & value::operator/=( int aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			if (aValue == 0) {
				clear_nl();
			} else {
				_boolValue = (((_boolValue ? 1 : 0) / aValue) != 0);
			}
			break;
		case eInt:
			if (aValue == 0) {
				clear_nl();
			} else {
				_intValue /= aValue;
			}
			break;
		case eDouble:
			if (aValue == 0) {
				clear_nl();
			} else {
				_doubleValue /= aValue;
			}
			break;
		case eTime:
			if (aValue == 0) {
				clear_nl();
			} else {
				_timeValue /= aValue;
			}
			break;
	}
	return *this;
}


value & value::operator/=( double aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			if (aValue == 0.0) {
				clear_nl();
			} else {
				_boolValue = (((_boolValue ? 1.0 : 0.0) / aValue) != 0.0);
			}
			break;
		case eInt:
			if (aValue == 0.0) {
				clear_nl();
			} else {
				_intValue /= aValue;
			}
			break;
		case eDouble:
			if (aValue == 0.0) {
				clear_nl();
			} else {
				_doubleValue /= aValue;
			}
			break;
		case eTime:
			if (aValue == 0.0) {
				clear_nl();
			} else {
				_timeValue /= aValue;
			}
			break;
	}
	return *this;
}


value & value::operator/=( uint64_t aValue )
{
	boost::detail::spinlock::scoped_lock	lock(_mutex);
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			if (aValue == 0) {
				clear_nl();
			} else {
				_boolValue = (((_boolValue ? 1 : 0) / aValue) != 0);
			}
			break;
		case eInt:
			if (aValue == 0) {
				clear_nl();
			} else {
				_intValue /= aValue;
			}
			break;
		case eDouble:
			if (aValue == 0) {
				clear_nl();
			} else {
				_doubleValue /= aValue;
			}
			break;
		case eTime:
			if (aValue == 0) {
				clear_nl();
			} else {
				_timeValue /= aValue;
			}
			break;
	}
	return *this;
}


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
bool value::set_nl( const value & aValue )
{
	*this = aValue;
	return true;
}


bool value::set_nl( bool aValue )
{
	_type = eBool;
	_boolValue = aValue;
	return true;
}


bool value::set_nl( int aValue )
{
	_type = eInt;
	_intValue = aValue;
	return true;
}


bool value::set_nl( double aValue )
{
	_type = eDouble;
	_doubleValue = aValue;
	return true;
}


bool value::set_nl( uint64_t aValue )
{
	_type = eTime;
	_timeValue = aValue;
	return true;
}


/**
 * This method simply resets the value to it's "undefined"
 * state - clearing out any value that might be currently
 * stored in the value. The key with the "_nl" is that it
 * means "no lock". No lock will be placed on the instance
 * in the process of setting these values. That means the
 * caller has to do it.
 */
void value::clear_nl()
{
	_type = eUnknown;
	_intValue = 0;
}


/**
 * This method gets the value for this instance, and it may be quite
 * involved in getting the value. This will be the way to get
 * constants as well as evaluate functions and expressions.
 *
 * The key with the "_nl" is that it means "no lock". No lock
 * will be placed on the instance in the process of setting
 * these values. That means the caller has to do it.
 */
bool value::evalAsBool_nl()
{
	bool		retval = false;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			retval = _boolValue;
			break;
		case eInt:
			retval = (_intValue == 0 ? false : true);
			break;
		case eDouble:
			retval = (_doubleValue == 0.0 ? false : true);
			break;
		case eTime:
			retval = (_timeValue == 0 ? false : true);
			break;
	}
	return retval;
}


int value::evalAsInt_nl()
{
	int			retval = 0;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			retval = (_boolValue ? 1 : 0);
			break;
		case eInt:
			retval = _intValue;
			break;
		case eDouble:
			retval = (int)_doubleValue;
			break;
		case eTime:
			retval = (int)_timeValue;
			break;
	}
	return retval;
}


double value::evalAsDouble_nl()
{
	double		retval = NAN;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			retval = (_boolValue ? 1.0 : 0.0);
			break;
		case eInt:
			retval = (double)_intValue;
			break;
		case eDouble:
			retval = _doubleValue;
			break;
		case eTime:
			retval = (double)_timeValue;
			break;
	}
	return retval;
}


uint64_t value::evalAsTime_nl()
{
	uint64_t	retval = 0;
	switch (_type) {
		case eUnknown:
			break;
		case eBool:
			retval = (_boolValue ? 1 : 0);
			break;
		case eInt:
			retval = (uint64_t)_intValue;
			break;
		case eDouble:
			retval = (uint64_t)_doubleValue;
			break;
		case eTime:
			retval = _timeValue;
			break;
	}
	return retval;
}


/**
 * This method returns the pointer to the mutex for this instance,
 * and is quite useful for those times the subclasses need to be able
 * to lock something up while they do their thing.
 */
boost::detail::spinlock & value::mutex() const
{
	return _mutex;
}
}	// end of namespace lkit


/*
 * There are times that values will be used in mathematical
 * expressions by themselves, these operator functions will make
 * it very easy for the user to do simple 'a + b' coding on
 * even the most complex structures.
 */
using namespace lkit;
value operator+( const value & aVar, const value & anOtherVar )
{
	value	v(aVar);
	v += anOtherVar;
	return v;
}


value operator-( const value & aVar, const value & anOtherVar )
{
	value	v(aVar);
	v -= anOtherVar;
	return v;
}


value operator*( const value & aVar, const value & anOtherVar )
{
	value	v(aVar);
	v *= anOtherVar;
	return v;
}


value operator/( const value & aVar, const value & anOtherVar )
{
	value	v(aVar);
	v /= anOtherVar;
	return v;
}


/*
 * These operator functions will allow the mixed-mode math with
 * value casting the result up to a value in each case. First,
 * start with simple addition, then subtraction, then multiplication
 * and finally division.
 */
value operator+( const value & aVar, bool aValue )
{
	value	v(aVar);
	v += aValue;
	return v;
}


value operator+( const value & aVar, int aValue )
{
	value	v(aVar);
	v += aValue;
	return v;
}


value operator+( const value & aVar, double aValue )
{
	value	v(aVar);
	v += aValue;
	return v;
}


value operator+( const value & aVar, uint64_t aValue )
{
	value	v(aVar);
	v += aValue;
	return v;
}


value operator+( bool aValue, const value & aVar )
{
	value	v(aVar);
	v += aValue;
	return v;
}


value operator+( int aValue, const value & aVar )
{
	value	v(aVar);
	v += aValue;
	return v;
}


value operator+( double aValue, const value & aVar )
{
	value	v(aVar);
	v += aValue;
	return v;
}


value operator+( uint64_t aValue, const value & aVar )
{
	value	v(aVar);
	v += aValue;
	return v;
}


value operator-( const value & aVar, bool aValue )
{
	value	v(aVar);
	v -= aValue;
	return v;
}


value operator-( const value & aVar, int aValue )
{
	value	v(aVar);
	v -= aValue;
	return v;
}


value operator-( const value & aVar, double aValue )
{
	value	v(aVar);
	v -= aValue;
	return v;
}


value operator-( const value & aVar, uint64_t aValue )
{
	value	v(aVar);
	v -= aValue;
	return v;
}


value operator-( bool aValue, const value & aVar )
{
	value	v(aValue);
	v -= aVar;
	return v;
}


value operator-( int aValue, const value & aVar )
{
	value	v(aValue);
	v -= aVar;
	return v;
}


value operator-( double aValue, const value & aVar )
{
	value	v(aValue);
	v -= aVar;
	return v;
}


value operator-( uint64_t aValue, const value & aVar )
{
	value	v(aValue);
	v -= aVar;
	return v;
}


value operator*( const value & aVar, bool aValue )
{
	value	v(aVar);
	v *= aValue;
	return v;
}


value operator*( const value & aVar, int aValue )
{
	value	v(aVar);
	v *= aValue;
	return v;
}


value operator*( const value & aVar, double aValue )
{
	value	v(aVar);
	v *= aValue;
	return v;
}


value operator*( const value & aVar, uint64_t aValue )
{
	value	v(aVar);
	v *= aValue;
	return v;
}


value operator*( bool aValue, const value & aVar )
{
	value	v(aVar);
	v *= aValue;
	return v;
}


value operator*( int aValue, const value & aVar )
{
	value	v(aVar);
	v *= aValue;
	return v;
}


value operator*( double aValue, const value & aVar )
{
	value	v(aVar);
	v *= aValue;
	return v;
}


value operator*( uint64_t aValue, const value & aVar )
{
	value	v(aVar);
	v *= aValue;
	return v;
}


value operator/( const value & aVar, bool aValue )
{
	value	v(aVar);
	v /= aValue;
	return v;
}


value operator/( const value & aVar, int aValue )
{
	value	v(aVar);
	v /= aValue;
	return v;
}


value operator/( const value & aVar, double aValue )
{
	value	v(aVar);
	v /= aValue;
	return v;
}


value operator/( const value & aVar, uint64_t aValue )
{
	value	v(aVar);
	v /= aValue;
	return v;
}


value operator/( bool aValue, const value & aVar )
{
	value	v(aValue);
	v /= aVar;
	return v;
}


value operator/( int aValue, const value & aVar )
{
	value	v(aValue);
	v /= aVar;
	return v;
}


value operator/( double aValue, const value & aVar )
{
	value	v(aValue);
	v /= aVar;
	return v;
}


value operator/( uint64_t aValue, const value & aVar )
{
	value	v(aValue);
	v /= aVar;
	return v;
}


/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this value. It basically is just the value of the base data type
 * and puts it into the stream.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::value & aValue )
{
	aStream << aValue.toString();
	return aStream;
}
