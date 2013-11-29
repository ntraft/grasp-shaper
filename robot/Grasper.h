/*
 * Grasper.h
 *
 *  Created on: Nov 25, 2013
 *      Author: jolande
 */

#ifndef GRASPER_H_
#define GRASPER_H_

#include "utils.h"
#include "tactile_output.h"

#include <cstdio>

#include <boost/tuple/tuple.hpp>

#include <barrett/log.h>
#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>

using namespace barrett;

#define FINGER_JOINT_LIMIT 2.4435 // = 140 degrees
#define PI 3.1415

double inFrontPos[] = {0, 0.39, 0, 2.67, 0, -1.6, 0};
double abovePos[] = {0, 0.675, 0, .997, 0, 1.392, 0};
double powerPos[] = {0, 0.713, 0, 2.211, 0, -1.458, 0};
double precisionPos[] = {0, 0.664, 0, 2.358, 0, -1.515, 0};
double topDownPos[] = {0, 0.705, 0, 1.174, 0, 1.205, 0};

template<size_t DOF>
class Grasper {

	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	#define LOG_DATA_TYPES double, jp_type, tactile_data
	typedef boost::tuple<LOG_DATA_TYPES> sample;

private:
	// Robotic systems
	systems::RealTimeExecutionManager* em;
	systems::Wam<DOF>* wam;
	Hand* hand;

    // Data logging
	unsigned int logCount;
	char tmpFile[14];
    const double T_s;
	systems::Ramp time;
    systems::TupleGrouper<LOG_DATA_TYPES> dataOutput;
    TactileOutput tactOut;
	systems::PeriodicDataLogger<sample>* logger;

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
	Grasper(systems::RealTimeExecutionManager* em, systems::Wam<DOF>* wam, Hand* hand);
	virtual ~Grasper();

	void doGrasp(char graspType);

private:
	void startLogging();
	void stopLogging();
	void prepareHand(char graspType);
	void grasp();
	void ungrasp();
	void moveFingersTo(double newPos);
	void liftAndReturn();
};

template<size_t DOF>
Grasper<DOF>::Grasper(systems::RealTimeExecutionManager* em, systems::Wam<DOF>* wam, Hand* hand) :
	em(em), wam(wam), hand(hand), T_s(em->getPeriod()), time(em), tactOut(), logger(NULL),
	inFront(inFrontPos), above(abovePos), power(powerPos), precision(precisionPos), topDown(topDownPos)
{
	logCount = 0;
	tripod[3] = 0.52;
	wrap[3] = PI;

	systems::connect(time.output, dataOutput.template getInput<0>());
	systems::connect(wam->jpOutput, dataOutput.template getInput<1>());
	if (hand->hasTactSensors()) {
//		std::vector<TactilePuck*> tps = hand->getTactilePucks();
//		systems::SingleOutput<tactile_data> tactOut;
		systems::connect(tactOut.output, dataOutput.template getInput<2>());
//		tps[0]->getFullData();
	}
}

template<size_t DOF>
Grasper<DOF>::~Grasper() {
	delete logger;
}

template<size_t DOF>
void Grasper<DOF>::startLogging() {
	if (logger != NULL) {
		printf("ERROR: Already logging!");
		return;
	}

	strcpy(tmpFile, "/tmp/btXXXXXX");
	if (mkstemp(tmpFile) == -1) {
		printf("ERROR: Couldn't create temporary file! Nothing will be logged.\n");
		return;
	}

	// Can't reuse loggers or writers. Have to create new ones for each log file.
	const size_t RATE = 1; // Samples are taken once every (rate*period) seconds.
	logger = new systems::PeriodicDataLogger<sample>(em, new log::RealTimeWriter<sample>(tmpFile, RATE*T_s), RATE);
	systems::connect(dataOutput.output, logger->input);
	time.start();
}

template<size_t DOF>
void Grasper<DOF>::stopLogging() {
	if (logger == NULL) {
		return;
	}

	logger->closeLog();
	systems::disconnect(logger->input);
	time.stop();
	time.reset();
	delete logger;
	logger = NULL;

	char *logFile;
	asprintf(&logFile, "logs/dataLog%d.log", logCount++);
	log::Reader<sample> reader(tmpFile);
	reader.exportCSV(logFile);
	std::remove(tmpFile);
}

template<size_t DOF>
void Grasper<DOF>::doGrasp(char graspType) {
	jp_type prepPos;
	jp_type targetPos;
	switch (graspType) {
	case '\0':
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
	}

	wam->moveTo(prepPos, 5.0, 5.0);
	prepareHand(graspType);
	startLogging();
	wam->moveTo(targetPos, 5.0, 5.0);
	Pause();
	grasp();
	liftAndReturn();
	Pause();
	ungrasp();
	wam->moveTo(prepPos, 5.0, 5.0);
	stopLogging();
}

template<size_t DOF>
void Grasper<DOF>::prepareHand(char graspType) {
	switch (graspType) {
	case '\0':
	case 'g':
	case 'p':
	case 'm':
		hand->trapezoidalMove(prism);
		break;
	case 't':
		hand->trapezoidalMove(tripod);
		break;
	case 'w':
		hand->trapezoidalMove(wrap);
		break;
	}
}

template<size_t DOF>
void Grasper<DOF>::grasp() {
	moveFingersTo(FINGER_JOINT_LIMIT);
}

template<size_t DOF>
void Grasper<DOF>::ungrasp() {
	moveFingersTo(0);
}

template<size_t DOF>
void Grasper<DOF>::moveFingersTo(double newPos) {
	hand->update();
	Hand::jp_type currPos = hand->getInnerLinkPosition();
	currPos[0] = currPos[1] = currPos[2] = newPos;
	hand->trapezoidalMove(currPos);
}

template<size_t DOF>
void Grasper<DOF>::liftAndReturn() {
	jp_type targetPos = wam->getJointPositions();
	jp_type liftPos(targetPos);
	liftPos[1] -= 0.3;
	wam->moveTo(liftPos);
	Pause(2000);
	wam->moveTo(targetPos);
}

#endif /* GRASPER_H_ */
