/*
 * utils.h
 *
 *  Created on: Nov 25, 2013
 *      Author: jolande
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <curses.h>

#include <barrett/math/matrix.h>

#include <boost/thread/thread.hpp>

void Pause(int ms = 1000) {
	boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

void printlog(const char* str, ...) {
	va_list args;
	va_start(args, str);
	printw(str, args);
	va_end(args);
	refresh();
}

template<int R, typename Units>
void printData(const char* msg, const barrett::math::Matrix<R,1, Units>& from) {
	printlog(msg);
	printw("[%6.3f", from[0]);
	for (size_t i = 1; i < R; ++i) {
		printw(", %6.3f", from[i]);
	}
	printw("]\n");
	refresh();
}

// parses a string of space-separated doubles into a vector of doubles
template<int R, int C>
bool parseDoubles(barrett::math::Matrix<R,C>& dest, int row, const std::string& str) {
	const char* cur = str.c_str();
	const char* next = cur;

	for (int i = 0; i < C; ++i) {
		dest(row, i) = strtod(cur, (char**) &next);
		if (cur == next) {
			return false;
		} else {
			cur = next;
		}
	}

	// Make sure there are no extra numbers in the string.
	double ignore = strtod(cur, (char**) &next);
	(void)ignore;  // Prevent unused variable warnings

	if (cur != next) {
		return false;
	}

	return true;
}

#endif /* UTILS_H_ */
