/**
 * variable.cpp - this file implements a simple, typed, variable. This will be
 *                a simple named value that can have many different values
 *                over time. It's evaluation is simple - it's just the value
 *                of the variable, but it can also be used in all expression
 *                processing.
 */

//	System Headers
#include <sstream>

//	Third-Party Headers
#include <boost/functional/hash.hpp>

//	Other Headers
#include "variable.h"

//	Forward Declarations

//	Private Constants

//	Private Datatypes

//	Private Data Constants



/**
 * Make it easy to reference the spinlock and it's scoped lock. They
 * are both in the boost::detail namespace, and it's just going to make
 * the code a little cleaner.
 */
using namespace boost::detail;

namespace lkit {
/*******************************************************************
 *
 *                     Constructors/Destructor
 *
 *******************************************************************/
/**
 * This is the default constructor that assumes NOTHING - it just
 * makes a simple variable ready to hold some kind of value.
 */
variable::variable() :
	value(),
	_name()
{
}


/**
 * This version of the constructor takes the name of the variable
 * and sets that but does not attempt to set the VALUE of the
 * variable at all. This will be commonly used in the parsing of
 * expressions when a variable is defined in the code, and not
 * yet provided by the caller.
 */
variable::variable( const std::string & aName ) :
	value(),
	_name(aName)
{
}


/**
 * These constructors take the different allowed types and create
 * variables based on that argument. This sets the name, type as
 * well as the actual value for this new instance.
 */
variable::variable( const std::string & aName, bool aValue ) :
	value(aValue),
	_name(aName)
{
}


variable::variable( const std::string & aName, int aValue ) :
	value(aValue),
	_name(aName)
{
}


variable::variable( const std::string & aName, double aValue ) :
	value(aValue),
	_name(aName)
{
}


variable::variable( const std::string & aName, uint64_t aValue ) :
	value(aValue),
	_name(aName)
{
}


/**
 * This is the standard copy constructor that needs to be in every
 * class to make sure that we control how many copies we have
 * floating around in the system.
 */
variable::variable( const variable & anOther ) :
	value(anOther),
	_name(anOther._name)
{
}


/**
 * This is the standard clone method that we'll have for all
 * classes so that it's possible to clone the value without
 * having to worry about memory management, etc.
 */
value *variable::clone() const
{
	return new variable(*this);
}


/**
 * This is the standard destructor and needs to be virtual to make
 * sure that if we subclass off this, the right destructor will be
 * called.
 */
variable::~variable()
{
}


/**
 * When we process the result of an equality we need to make sure
 * that we do this right by always having an equals operator on
 * all classes.
 */
variable & variable::operator=( const variable & anOther )
{
	if (this != & anOther) {
		// let the super class do all it's stuff...
		value::operator=(anOther);
		// ...then we'll do our ivars
		_name = anOther._name;
	}
	return *this;
}


/**
 * When we want to assign a variable a value, this operator
 * will come in VERY handy. It takes any value of the right
 * type and evaluates it for the value to place into this
 * variable.
 */
variable & variable::operator=( const value & anOther )
{
	if (this != & anOther) {
		// let the super class do all it's stuff...
		value::operator=(anOther);
	}
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
bool variable::set( const std::string & aName, bool aValue )
{
	bool	error = false;
	spinlock::scoped_lock	lock(mutex());
	if ((error = !value::set_nl(aValue)) == false) {
		_name = aName;
	}
	return !error;
}


bool variable::set( const std::string & aName, int aValue )
{
	bool	error = false;
	spinlock::scoped_lock	lock(mutex());
	if ((error = !value::set_nl(aValue)) == false) {
		_name = aName;
	}
	return !error;
}


bool variable::set( const std::string & aName, double aValue )
{
	bool	error = false;
	spinlock::scoped_lock	lock(mutex());
	if ((error = !value::set_nl(aValue)) == false) {
		_name = aName;
	}
	return !error;
}


bool variable::set( const std::string & aName, uint64_t aValue )
{
	bool	error = false;
	spinlock::scoped_lock	lock(mutex());
	if ((error = !value::set_nl(aValue)) == false) {
		_name = aName;
	}
	return !error;
}


/**
 * This method returns the name of this variable, as defined by
 * the caller. This should be set in one of the setters or the
 * constructor, but it's possible that it hasn't been set, and
 * care should be taken in processing it's value.
 */
std::string variable::getName() const
{
	spinlock::scoped_lock	lock((spinlock &)mutex());
	std::string		n = _name;
	return n;
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
std::string variable::toString() const
{
	spinlock::scoped_lock	lock((spinlock &)mutex());
	std::ostringstream	msg;
	msg << "[" << _name << " = " << value::toString() << "]";
	return msg.str();
}


/**
 * When we have a custom '==' operator, it's wise to have a hash
 * method as well. This makes it much easier to used hash-based
 * containers like boost, etc. It's a requirement that if two
 * instances are '==', then their hash() methods must be '==' as
 * well.
 */
size_t variable::hash() const
{
	size_t	ans = value::hash();
	boost::hash_combine(ans, _name);
	return ans;
}


/**
 * This method checks to see if two values are equal in their
 * contents and not their pointer values. This is how you'd likely
 * expect equality to work.
 */
bool variable::operator==( const variable & anOther ) const
{
	bool		equals = false;
	if ((this == & anOther) ||
		(value::operator==(anOther) &&
		 (_name == anOther._name))) {
		equals = true;
	}
	return equals;
}


/**
 * This method checks to see if two values are NOT equal in their
 * contents and not their pointer values. This is how you'd likely
 * expect equality to work.
 */
bool variable::operator!=( const variable & anOther ) const
{
	return !operator==(anOther);
}


/**
 * This method looks at the value of this variable and the evaluated
 * value of the arguments and sees if they are equivalent. If so,
 * this operator returns 'true'. This is a very powerful operator
 * for the variable as it allows values to be compared to variables
 * without worrying about the different classes.
 */
bool variable::operator==( const value & anOther ) const
{
	bool		equals = false;
	if ((this == & anOther) || value::operator==(anOther)) {
		equals = true;
	}
	return equals;
}


/**
 * This method looks at the value of this variable and the evaluated
 * value of the arguments and sees if they are NOT equivalent. If so,
 * this operator returns 'true'. This is a very powerful operator
 * for the variable as it allows values to be compared to variables
 * without worrying about the different classes.
 */
bool variable::operator!=( const value & anOther ) const
{
	return !operator==(anOther);
}
}		// end of namespace lkit


/**
 * For debugging purposes, let's make it easy for the user to stream
 * out this value. It basically is just the value of the base data type
 * and puts it into the stream.
 */
std::ostream & operator<<( std::ostream & aStream, const lkit::variable & aValue )
{
	aStream << aValue.toString();
	return aStream;
}
