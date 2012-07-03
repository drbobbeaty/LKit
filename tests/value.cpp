/**
 * This is the test of the value class
 */
//	System Headers
#include <iostream>
#include <string>

//	Third-Party Headers

//	Other Headers
#include "value.h"
#include "util/timer.h"

int main(int argc, char *argv[]) {
	bool	error = false;

	/**
	 * Start by making sure that we can hold all the basic data types,
	 * and additionally, that the '==' operator works.
	 */
	if (!error) {
		lkit::value		v(false);
		if (v == false) {
			std::cout << "Success - the value can represent a bool!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - the value cannot represent a bool: " << v  << " should be: (bool)false" << std::endl;
		}
	}
	if (!error) {
		lkit::value		v(10);
		if (v == 10) {
			std::cout << "Success - the value can represent an integer!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - the value cannot represent an integer: " << v  << " should be: (int)10" << std::endl;
		}
	}
	if (!error) {
		lkit::value		v(10.11);
		if (v == 10.11) {
			std::cout << "Success - the value can represent a double!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - the value cannot represent a double: " << v  << " should be: (double)10.11" << std::endl;
		}
	}
	if (!error) {
		uint64_t		t = 123123123;
		lkit::value		v(t);
		if (v == t) {
			std::cout << "Success - the value can represent a uint64_t!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - the value cannot represent a uint64_t: " << v  << " should be: (uint64_t)123123123" << std::endl;
		}
	}

	/**
	 * Now let's try some manipulations of the values to see that the
	 * operators we have defined are working.
	 */
	if (!error) {
		lkit::value		v(10);
		v += 5;
		if (v == 15) {
			std::cout << "Success - '+=' works on the value for integers!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - '+=' fails for value on integers: " << v  << " should be: (int)15" << std::endl;
		}

		v *= 3;
		if (v == 45) {
			std::cout << "Success - '*=' works on the value for integers!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - '*=' fails for value on integers: " << v  << " should be: (int)45" << std::endl;
		}

		v -= 5;
		if (v == 40) {
			std::cout << "Success - '-=' works on the value for integers!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - '-=' fails for value on integers: " << v  << " should be: (int)40" << std::endl;
		}

		v /= 10;
		if (v == 4) {
			std::cout << "Success - '/=' works on the value for integers!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - '/=' fails for value on integers: " << v  << " should be: (int)4" << std::endl;
		}
	}

	if (!error) {
		lkit::value		v(10.10);
		v += 5.10;
		if (fabs((double)v - 15.20) < 1.0e06) {
			std::cout << "Success - '+=' works on the value for doubles!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - '+=' fails for value on doubles: " << v  << " should be: (double)15.15" << std::endl;
		}

		v *= 3.0;
		if (fabs((double)v - 45.60) < 1.0e06) {
			std::cout << "Success - '*=' works on the value for doubles!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - '*=' fails for value on doubles: " << v  << " should be: (double)45.60" << std::endl;
		}

		v -= 5.20;
		if (fabs((double)v - 40.40) < 1.0e06) {
			std::cout << "Success - '-=' works on the value for doubles!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - '-=' fails for value on doubles: " << v  << " should be: (double)40.40" << std::endl;
		}

		v /= 10.0;
		if (fabs((double)v - 4.04) < 1.0e06) {
			std::cout << "Success - '/=' works on the value for doubles!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - '/=' fails for value on doubles: " << v  << " should be: (double)4.04" << std::endl;
		}
	}

	std::cout << (error ? "FAILED!" : "SUCCESS") << std::endl;
	return (error ? 1 : 0);
}
