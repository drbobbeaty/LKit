/**
 * This is the test of the parser class
 */
//	System Headers
#include <iostream>
#include <string>

//	Third-Party Headers

//	Other Headers
#include "parser.h"
#include "util/timer.h"

int main(int argc, char *argv[]) {
	bool	error = false;

	lkit::parser	p;
	if (!error) {
		std::string		src = "(+ 1 2 3)";
		lkit::value		ref = 6;
		p.setSource(src);
		if (p.eval() == ref) {
			std::cout << "Success, parsed " << src << " into: " << ref << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, unable to parse " << src << " into: " << ref << " ... got: " << p.eval() << std::endl;
		}
	}

	if (!error) {
		std::string		src = "(/ 10.0 2.0 5.0)";
		lkit::value		ref = 1.0;
		p.setSource(src);
		if (p.eval() == ref) {
			std::cout << "Success, parsed " << src << " into: " << ref << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, unable to parse " << src << " into: " << ref << " ... got: " << p.eval() << std::endl;
		}
	}

	if (!error) {
		std::string		src = "(and true false true)";
		lkit::value		ref = false;
		p.setSource(src);
		if (p.eval() == ref) {
			std::cout << "Success, parsed " << src << " into: " << ref << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, unable to parse " << src << " into: " << ref << " ... got: " << p.eval() << std::endl;
		}
	}

	if (!error) {
		std::string		src = "(or 1 0 1)";
		lkit::value		ref = true;
		p.setSource(src);
		if (p.eval() == ref) {
			std::cout << "Success, parsed " << src << " into: " << ref << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, unable to parse " << src << " into: " << ref << " ... got: " << p.eval() << std::endl;
		}
	}

	if (!error) {
		std::string		src = "(+ (+ 1 2) (+ 3 4 5) 6))";
		lkit::value		ref = 21;
		p.setSource(src);
		if (p.eval() == ref) {
			std::cout << "Success, parsed " << src << " into: " << ref << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, unable to parse " << src << " into: " << ref << " ... got: " << p.eval() << std::endl;
		}
	}

	if (!error) {
		std::string		src = "(+ (/ 10.0 2.5) (* (+ 1.5 2 6) 2.0))";
		lkit::value		ref = 23.0;
		p.setSource(src);
		if (p.eval() == ref) {
			std::cout << "Success, parsed " << src << " into: " << ref << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, unable to parse " << src << " into: " << ref << " ... got: " << p.eval() << std::endl;
		}
	}

	std::cout << (error ? "FAILED!" : "SUCCESS") << std::endl;
	return (error ? 1 : 0);
}
