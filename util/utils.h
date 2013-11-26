/*
 * utils.h
 *
 *  Created on: Nov 25, 2013
 *      Author: jolande
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <boost/thread/thread.hpp>

void Pause(int ms = 1000) {
	boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}


#endif /* UTILS_H_ */
