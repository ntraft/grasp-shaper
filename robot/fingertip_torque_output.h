/*
 * fingertip_torque_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef FINGERTIP_TORQUE_OUTPUT_H_
#define FINGERTIP_TORQUE_OUTPUT_H_

#include "asynchronous_output.h"

#include <barrett/detail/ca_macro.h>
#include <barrett/products/product_manager.h>
#include <barrett/math/matrix.h>
#include <barrett/systems.h>
#include <barrett/systems/abstract/system.h>
#include <barrett/systems/abstract/single_io.h>

using namespace barrett;
using namespace barrett::systems;

typedef math::Vector<Hand::DOF, int>::type finger_torques;

class FingertipTorqueOutput : public AsynchronousOutput<finger_torques> {
public:
	explicit FingertipTorqueOutput(Hand* hand, const std::string& sysName = "FingertipTorqueOutput") :
		AsynchronousOutput<finger_torques>(sysName), hand(hand)
	{
		this->outputValue->setData(&data);
	}
	virtual ~FingertipTorqueOutput() { mandatoryCleanUp(); }

protected:
	void updateSensor() {
//		hand->update(Hand::S_FINGERTIP_TORQUE);
		const std::vector<int> torques = hand->getFingertipTorque();
		for (unsigned int i = 0; i < torques.size(); ++i) {
			data[i] = torques[i];
		}
	}

	Hand* hand;
	finger_torques data;

private:
	DISALLOW_COPY_AND_ASSIGN(FingertipTorqueOutput);
};

#endif /* FINGERTIP_TORQUE_OUTPUT_H_ */
