/**
 * This is the test of the timer (date) parsing
 */
//	System Headers
#include <iostream>
#include <string>

//	Third-Party Headers

//	Other Headers
#include "util/timer.h"

int main(int argc, char *argv[]) {
	bool	error = false;

	using namespace lkit::util;
	if (!error) {
		std::string	ts("2012-03-21 11:45:16");
		uint64_t	ref = 1332351916LL * 1000000LL;
		uint64_t	stamp = timer::parseTimestamp(ts);
		if (stamp == ref) {
			std::cout << "Success, converted complete date+time to stamp!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, could not convert " << ts << " to stamp! Got: " << stamp << std::endl;
		}

		ts = "2012-03-21 00:00:02";
		ref = 1332309602LL * 1000000LL;
		stamp = timer::parseTimestamp(ts);
		if (stamp == ref) {
			std::cout << "Success, converted complete date+time to stamp!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, could not convert " << ts << " to stamp! Got: " << stamp << std::endl;
		}

		ts = "2012-03-21";
		ref = 1332309600LL * 1000000LL;
		stamp = timer::parseTimestamp(ts);
		if (stamp == ref) {
			std::cout << "Success, converted date to stamp!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, could not convert " << ts << " to stamp! Got: " << stamp << std::endl;
		}

		ts = "11:45:16";
		ref = 42316LL * 1000000LL;
		stamp = timer::parseTimestamp(ts);
		if (stamp == ref) {
			std::cout << "Success, converted time to stamp!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, could not convert " << ts << " to stamp! Got: " << stamp << std::endl;
		}

		ts = "11:45:16.12";
		ref = 42316LL * 1000000LL + 120000LL;
		stamp = timer::parseTimestamp(ts);
		if (stamp == ref) {
			std::cout << "Success, converted time+fraction to stamp!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, could not convert " << ts << " to stamp! Got: " << stamp << std::endl;
		}

		ts = "11:45:16.123456";
		ref = 42316LL * 1000000LL + 123456LL;
		stamp = timer::parseTimestamp(ts);
		if (stamp == ref) {
			std::cout << "Success, converted time+fraction to stamp!" << std::endl;
		} else {
			error = true;
			std::cout << "ERROR, could not convert " << ts << " to stamp! Got: " << stamp << std::endl;
		}
	}

	std::cout << (error ? "FAILED!" : "SUCCESS") << std::endl;
	return (error ? 1 : 0);
}
