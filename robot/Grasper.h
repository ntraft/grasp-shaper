/*
 * Grasper.h
 *
 *  Created on: Nov 25, 2013
 *      Author: jolande
 */

#ifndef GRASPER_H_
#define GRASPER_H_

#include "grasp_thread.h"

#include <boost/thread.hpp>

#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>

using namespace barrett;

double inFrontPos[] = {0, 0.39, 0, 2.67, 0, -1.6, 0};
double abovePos[] = {0, 0.675, 0, .997, 0, 1.392, 0};
double powerPos[] = {0, 0.713, 0, 2.211, 0, -1.458, 0};
double precisionPos[] = {0, 0.664, 0, 2.358, 0, -1.515, 0};
double topDownPos[] = {0, 0.705, 0, 1.174, 0, 1.205, 0};

template<size_t DOF>
class Grasper {

	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	#define LOG_DATA_TYPES double, jp_type, Hand::jp_type, finger_torques, cf_type, tactile_data
	typedef boost::tuple<LOG_DATA_TYPES> sample;

private:
	// Robotic systems
	systems::RealTimeExecutionManager* em;
	systems::Wam<DOF>* wam;
	Hand* hand;
	ForceTorqueSensor* ftSensor;

	// Grasp tracking
	unsigned int logCount;
	char currGrasp;
	boost::thread* threadRunner;
	GraspThread<DOF>* graspThread;
	jp_type prepPos;
	jp_type targetPos;
	Hand::jp_type handPrepPos;

	// Joint positions
	jp_type inFront;
	jp_type above;
	jp_type power;
	jp_type precision;
	jp_type topDown;
	Hand::jp_type prism;
	Hand::jp_type tripod;
	Hand::jp_type wrap;

public:
	Grasper(systems::RealTimeExecutionManager* em, systems::Wam<DOF>* wam, Hand* hand, ForceTorqueSensor* ftSensor);
	virtual ~Grasper();

	void doGrasp(char graspType);
	void failedGrasp();
	void halt();

private:
	void startLogging();
	void stopLogging();
	void setPositions(char graspType);
	void prepareHand(char graspType);
	void move(jp_type& dest);
	void grasp();
	void ungrasp();
	void moveFingersTo(double newPos);
	void liftAndReturn();
};

template<size_t DOF>
Grasper<DOF>::Grasper(systems::RealTimeExecutionManager* em, systems::Wam<DOF>* wam, Hand* hand, ForceTorqueSensor* ftSensor) :
	em(em), wam(wam), hand(hand), ftSensor(ftSensor),
	logCount(0), currGrasp('\0'), threadRunner(NULL), graspThread(NULL),
	prepPos(inFrontPos), targetPos(powerPos), handPrepPos(prism),
	inFront(inFrontPos), above(abovePos), power(powerPos), precision(precisionPos), topDown(topDownPos)
{
	tripod[3] = 0.52;
	wrap[3] = PI;
}

template<size_t DOF>
Grasper<DOF>::~Grasper() {
	halt();
}

template<size_t DOF>
void Grasper<DOF>::doGrasp(char graspType) {
	currGrasp = graspType;
	setPositions(graspType);
	halt();
	graspThread = new GraspThread<DOF>(em, wam, hand, ftSensor, &logCount,
			currGrasp, prepPos, targetPos, handPrepPos);
	threadRunner = new boost::thread(graspEntryPoint<DOF>, graspThread);
}

template<size_t DOF>
void Grasper<DOF>::halt() {
	if (threadRunner != NULL) {
		threadRunner->interrupt();
		threadRunner->join();
		delete threadRunner;
		threadRunner = NULL;
		delete graspThread;
		graspThread = NULL;
	}
}

template<size_t DOF>
void Grasper<DOF>::failedGrasp() {
	currGrasp = '\0';
	if (graspThread != NULL) {
		graspThread->failedGrasp();
	}
	halt();
	wam->moveTo(prepPos, false);
}

template<size_t DOF>
void Grasper<DOF>::setPositions(char graspType) {
	switch (graspType) {
	case 'g':
	case 'w':
		prepPos = inFront;
		targetPos = power;
		break;
	case 'p':
		prepPos = inFront;
		targetPos = precision;
		break;
	case 'm':
	case 't':
		prepPos = above;
		targetPos = topDown;
		break;
	default:
		throw 1;
		return;
	}
	if (graspThread != NULL && graspThread->graspType == graspType && !graspThread->failed) {
		handPrepPos = graspThread->graspShape;
		// 70% of the original closure.
		handPrepPos[0] *= .7;
		handPrepPos[1] *= .7;
		handPrepPos[2] *= .7;
	} else {
		switch (graspType) {
		case 'g':
		case 'p':
		case 'm':
			handPrepPos = prism;
			break;
		case 't':
			handPrepPos = tripod;
			break;
		case 'w':
			handPrepPos = wrap;
			break;
		}
	}
}

#endif /* GRASPER_H_ */
