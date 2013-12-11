/*
 * fingertip_torque_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef FINGERTIP_TORQUE_OUTPUT_H_
#define FINGERTIP_TORQUE_OUTPUT_H_

#include <boost/asio/io_service.hpp>

#include <barrett/detail/ca_macro.h>
#include <barrett/products/product_manager.h>
#include <barrett/math/matrix.h>
#include <barrett/systems.h>
#include <barrett/systems/abstract/system.h>
#include <barrett/systems/abstract/single_io.h>

using namespace barrett;
using namespace barrett::systems;

typedef math::Vector<Hand::DOF, int>::type finger_torques;

class FingertipTorqueOutput : public System, public SingleOutput<finger_torques> {
public:
	explicit FingertipTorqueOutput(Hand* hand, boost::asio::io_service* sensorUpdater, const std::string& sysName = "FingertipTorqueOutput") :
		System(sysName), SingleOutput<finger_torques>(this), hand(hand), sensorUpdater(sensorUpdater)
	{
		this->outputValue->setData(&data);
	}
	virtual ~FingertipTorqueOutput() { mandatoryCleanUp(); }

protected:
	virtual void operate() {
		sensorUpdater->post(boost::bind(&FingertipTorqueOutput::updateSensor, this));
		const std::vector<int> torques = hand->getFingertipTorque();
		for (unsigned int i = 0; i < torques.size(); ++i) {
			data[i] = torques[i];
		}
	}
	void updateSensor() { hand->update(Hand::S_FINGERTIP_TORQUE); }
	virtual void invalidateOutputs() { /* do nothing */ }

	Hand* hand;
	finger_torques data;
	boost::asio::io_service* sensorUpdater;

private:
	DISALLOW_COPY_AND_ASSIGN(FingertipTorqueOutput);
};

#endif /* FINGERTIP_TORQUE_OUTPUT_H_ */
