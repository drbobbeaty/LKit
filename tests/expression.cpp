/**
 * This is the test of the expression class
 */
//	System Headers
#include <iostream>
#include <string>

//	Third-Party Headers

//	Other Headers
#include "value.h"
#include "base_functions.h"
#include "expression.h"
#include "util/timer.h"

int main(int argc, char *argv[]) {
	bool	error = false;

	/**
	 * Based on the order of the arguments, we can get very different
	 * values. This is because the type of the first value sets the
	 * type of the answer - BY DESIGN. So, if we sum() a bunch of
	 * values and the first is an integer, then the sum() will be the
	 * integer component of EACH argument. If it's a double, then all
	 * the values will be cast as doubles and then summed.
	 *
	 * So we can check these as two distinct conditions for the expression.
	 */
	if (!error) {
		lkit::value	a, b, c, d;
		a = 10;
		b = 5.5;
		c = 3.14;
		d = 6.2;
		lkit::func::sum		f;
		// sum with the integer first
		lkit::expression	y(&f, &a, &b, &c, &d);
		if (y.evalAsDouble() == 24.0) {
			std::cout << "Success - " << y << " evaluates properly!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - " << y << " evaluates WRONG: " << y.evalAsDouble() << std::endl;
		}
		// sum with a double first
		lkit::expression	z(&f, &b, &a, &c, &d);
		if (z.evalAsDouble() == 24.84) {
			std::cout << "Success - " << z << " evaluates properly!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - " << z << " evaluates WRONG: " << z.evalAsDouble() << std::endl;
		}
	}

	/**
	 * Now let's check the max() and min() to make sure that they are
	 * working properly.
	 */
	if (!error) {
		lkit::value	a, b, c, d;
		a = 10.1;
		b = 5.5;
		c = 3.14;
		d = 6.2;
		// try the max() first
		lkit::func::max		f;
		lkit::expression	y(&f, &a, &b, &c, &d);
		if (y.evalAsDouble() == 10.1) {
			std::cout << "Success - " << y << " evaluates properly!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - " << y << " evaluates WRONG: " << y.evalAsDouble() << std::endl;
		}
		// now the min()
		lkit::func::min		g;
		lkit::expression	z(&g, &a, &b, &c, &d);
		if (z.evalAsDouble() == 3.14) {
			std::cout << "Success - " << z << " evaluates properly!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR - " << z << " evaluates WRONG: " << z.evalAsDouble() << std::endl;
		}
	}

	std::cout << (error ? "FAILED!" : "SUCCESS") << std::endl;
	return (error ? 1 : 0);
}
