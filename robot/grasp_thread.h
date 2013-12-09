/*
 * grasp_thread.h
 *
 *  Created on: Nov 25, 2013
 *      Author: jolande
 */

#ifndef GRASP_THREAD_H_
#define GRASP_THREAD_H_

#include "utils.h"
#include "finger_position_output.h"
#include "fingertip_torque_output.h"
#include "force_torque_output.h"
#include "tactile_output.h"

#include <cstdio>

#include <boost/thread.hpp>
#include <boost/tuple/tuple.hpp>

#include <barrett/log.h>
#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>

using namespace barrett;

#define FINGER_JOINT_LIMIT 2.4435 // = 140 degrees
#define PI 3.1415

template<size_t DOF> class GraspThread;
template<size_t DOF> void graspEntryPoint(GraspThread<DOF>* gt);

template<size_t DOF>
class GraspThread {

	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	#define LOG_DATA_TYPES double, jp_type, Hand::jp_type, finger_torques, cf_type, tactile_data
	typedef boost::tuple<LOG_DATA_TYPES> sample;

private:
	// Robotic systems
	systems::RealTimeExecutionManager* em;
	systems::Wam<DOF>* wam;
	Hand* hand;
	ForceTorqueSensor* ftSensor;

    // Data logging
	unsigned int* logCount;
	char tmpFile[14];
    const double T_s;
	systems::Ramp time;
    systems::TupleGrouper<LOG_DATA_TYPES> dataOutput;
    FingerPositionOutput fingerPosOut;
    FingertipTorqueOutput fingerTorqueOut;
    ForceTorqueOutput forceTorqueOut;
    TactileOutput tactOut;
	systems::PeriodicDataLogger<sample>* logger;

public:
	// Grasp description
	char graspType;
	jp_type prepPos;
	jp_type targetPos;
	Hand::jp_type handPrepPos;
	Hand::jp_type graspShape;

	GraspThread(systems::RealTimeExecutionManager* em, systems::Wam<DOF>* wam, Hand* hand, ForceTorqueSensor* ftSensor,
			unsigned int* logCount, char graspType, jp_type prepPos, jp_type targetPos, Hand::jp_type handPrepPos);
	virtual ~GraspThread();

	void failedGrasp();

	template<size_t S>
	friend void graspEntryPoint(GraspThread<S>* gt);

private:
	void startLogging();
	void stopLogging();
	void setPositions();
	void prepareHand();
	void moveToPrep();
	void moveToTarget();
	void move(jp_type& dest);
	void grasp();
	void ungrasp();
	void moveFingersTo(double newPos);
	void moveHand(Hand::jp_type& dest);
	void liftAndReturn();
	void pauseUntilMoveIsDone();
	void recordGraspShape();
};

template<size_t DOF>
void graspEntryPoint(GraspThread<DOF>* gt) {
	try {
		gt->moveToPrep();
		gt->prepareHand();
//		gt->startLogging();
		gt->moveToTarget();
		Pause();
		gt->grasp();
		gt->liftAndReturn();
		Pause();
		gt->ungrasp();
		gt->moveToPrep();
	} catch (boost::thread_interrupted const&) {
		// We don't need to check for interruption ourselves; if interrupted,
		// an exception will be thrown whenever the thread sleeps.
	} catch (...) {
		printf("Unexpected exception!");
	}
//	gt->stopLogging();
}

template<size_t DOF>
GraspThread<DOF>::GraspThread(systems::RealTimeExecutionManager* em, systems::Wam<DOF>* wam, Hand* hand, ForceTorqueSensor* ftSensor,
		unsigned int* logCount, char graspType, jp_type prepPos, jp_type targetPos, Hand::jp_type handPrepPos) :
	em(em), wam(wam), hand(hand), ftSensor(ftSensor), logCount(logCount), T_s(em->getPeriod()), time(em),
	fingerPosOut(hand), fingerTorqueOut(hand), forceTorqueOut(ftSensor), tactOut(hand->getTactilePucks()), logger(NULL),
	graspType(graspType), prepPos(prepPos), targetPos(targetPos), handPrepPos(handPrepPos)
{
	systems::connect(time.output, dataOutput.template getInput<0>());
	systems::connect(wam->jpOutput, dataOutput.template getInput<1>());
	systems::connect(fingerPosOut.output, dataOutput.template getInput<2>());
	systems::connect(fingerTorqueOut.output, dataOutput.template getInput<3>());
	systems::connect(forceTorqueOut.output, dataOutput.template getInput<4>());
	systems::connect(tactOut.output, dataOutput.template getInput<5>());
}

template<size_t DOF>
GraspThread<DOF>::~GraspThread() {
	delete logger;
}

template<size_t DOF>
void GraspThread<DOF>::startLogging() {
	if (logger != NULL) {
		printf("ERROR: Already logging!"); // TODO change printf's to printw's
		return;
	}

	strcpy(tmpFile, "/tmp/btXXXXXX");
	if (mkstemp(tmpFile) == -1) {
		printf("ERROR: Couldn't create temporary file! Nothing will be logged.\n");
		return;
	}

	// Can't reuse loggers or writers. Have to create new ones for each log file.
	const size_t RATE = 25; // Take samples every 50 ms (20 Hz).
	logger = new systems::PeriodicDataLogger<sample>(em, new log::RealTimeWriter<sample>(tmpFile, RATE*T_s), RATE);
	systems::connect(dataOutput.output, logger->input);
	time.start();
}

template<size_t DOF>
void GraspThread<DOF>::stopLogging() {
	if (logger == NULL) {
		return;
	}

	logger->closeLog();
	systems::disconnect(logger->input);
	time.stop();
	time.reset();
	delete logger;
	logger = NULL;

	// The counter isn't perfectly thread safe but it's close enough.
	char *logFile;
	asprintf(&logFile, "logs/dataLog%d.log", (*logCount)++);
	log::Reader<sample> reader(tmpFile);
	reader.exportCSV(logFile);
	std::remove(tmpFile);
}

template<size_t DOF>
void GraspThread<DOF>::failedGrasp() {
	// TODO record failure
	ungrasp();
}

template<size_t DOF>
void GraspThread<DOF>::recordGraspShape() {
	hand->update();
	graspShape = hand->getInnerLinkPosition();
}

template<size_t DOF>
void GraspThread<DOF>::prepareHand() {
	moveHand(handPrepPos);
}

template<size_t DOF>
void GraspThread<DOF>::moveToPrep() {
	move(prepPos);
}

template<size_t DOF>
void GraspThread<DOF>::moveToTarget() {
	move(targetPos);
}

template<size_t DOF>
void GraspThread<DOF>::move(jp_type& dest) {
	wam->moveTo(dest, false, 1.0, 1.0);
	pauseUntilMoveIsDone();
}

template<size_t DOF>
void GraspThread<DOF>::grasp() {
	moveFingersTo(FINGER_JOINT_LIMIT);
}

template<size_t DOF>
void GraspThread<DOF>::ungrasp() {
	moveHand(handPrepPos);
}

template<size_t DOF>
void GraspThread<DOF>::moveFingersTo(double newPos) {
	hand->update();
	Hand::jp_type currPos = hand->getInnerLinkPosition();
	currPos[0] = currPos[1] = currPos[2] = newPos;
	moveHand(currPos);
}

template<size_t DOF>
void GraspThread<DOF>::moveHand(Hand::jp_type& dest) {
	hand->trapezoidalMove(dest, false);
	pauseUntilMoveIsDone();
}

template<size_t DOF>
void GraspThread<DOF>::liftAndReturn() {
	jp_type targetPos = wam->getJointPositions();
	jp_type liftPos(targetPos);
	liftPos[1] -= 0.3;
	wam->moveTo(liftPos, false);
	pauseUntilMoveIsDone();
	Pause(2000);
	recordGraspShape();
	wam->moveTo(targetPos, false);
	pauseUntilMoveIsDone();
}

template<size_t DOF>
void GraspThread<DOF>::pauseUntilMoveIsDone() {
	while (!wam->moveIsDone() || !hand->doneMoving()) Pause(20);
}

#endif /* GRASP_THREAD_H_ */
