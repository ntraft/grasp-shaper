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


#endif /* UTILS_H_ */
