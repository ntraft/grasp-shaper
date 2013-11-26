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
	systems::Wam<DOF> wam;
	Hand hand;

	jp_type inFront;
	jp_type above;
	jp_type powerPos;
	jp_type precisionPos;
	jp_type topDownPos;
	Hand::jp_type prism;
	Hand::jp_type tripod;
	Hand::jp_type wrap;

public:
	Grasper(systems::Wam<DOF>& wam, Hand& hand);
	virtual ~Grasper();

	void doGrasp(char graspType);

private:
	void prepareHand(char graspType);
	void grasp();
	void ungrasp();
	void moveFingersTo(double newPos);
	void liftAndReturn();
};

#endif /* GRASPER_H_ */
