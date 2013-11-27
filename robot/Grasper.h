/*
 * Grasper.h
 *
 *  Created on: Nov 25, 2013
 *      Author: jolande
 */

#ifndef GRASPER_H_
#define GRASPER_H_

#include "utils.h"

#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>

#include <barrett/standard_main_function.h>

using namespace barrett;

#define FINGER_JOINT_LIMIT 2.4435 // = 140 degrees
#define PI 3.1415

template<size_t DOF>
class Grasper {

	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);

private:
	systems::Wam<DOF>* wam;
	Hand* hand;

	jp_type inFront;
	jp_type above;
	jp_type powerPos;
	jp_type precisionPos;
	jp_type topDownPos;
	Hand::jp_type prism;
	Hand::jp_type tripod;
	Hand::jp_type wrap;

public:
	Grasper(systems::Wam<DOF>* wam, Hand* hand);
	virtual ~Grasper();

	void doGrasp(char graspType);

private:
	void prepareHand(char graspType);
	void grasp();
	void ungrasp();
	void moveFingersTo(double newPos);
	void liftAndReturn();
};

template<size_t DOF>
Grasper<DOF>::Grasper(systems::Wam<DOF>* wam, Hand* hand) {
	this->wam = wam;
	this->hand = hand;

	double p1[] = {0, 0.39, 0, 2.67, 0, -1.6, 0};
	inFront(*p1);
	double p2[] = {0, 0.675, 0, .997, 0, 1.392, 0};
	above(*p2);
	double p3[] = {0, 0.713, 0, 2.211, 0, -1.458, 0};
	powerPos(*p3);
	double p4[] = {0, 0.664, 0, 2.358, 0, -1.515, 0};
	precisionPos(*p4);
	double p5[] = {0, 0.705, 0, 1.174, 0, 1.205, 0};
	topDownPos(*p5);
	tripod[3] = 0.52;
	wrap[3] = PI;
}

template<size_t DOF>
Grasper<DOF>::~Grasper() {
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
		targetPos = powerPos;
		break;
	case 'p':
		prepPos = inFront;
		targetPos = precisionPos;
		break;
	case 'm':
	case 't':
		prepPos = above;
		targetPos = topDownPos;
		break;
	}

	wam->moveTo(prepPos, 5.0, 5.0);
	prepareHand(graspType);
	wam->moveTo(targetPos, 5.0, 5.0);
	Pause();
	grasp();
	liftAndReturn();
	Pause();
	ungrasp();
	wam->moveTo(prepPos, 5.0, 5.0);
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