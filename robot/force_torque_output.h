/*
 * force_torque_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef FORCE_TORQUE_OUTPUT_H_
#define FORCE_TORQUE_OUTPUT_H_

#include <boost/asio/io_service.hpp>

#include <barrett/detail/ca_macro.h>
#include <barrett/products/product_manager.h>
#include <barrett/systems.h>
#include <barrett/systems/abstract/system.h>
#include <barrett/systems/abstract/single_io.h>

using namespace barrett;
using namespace barrett::systems;

class ForceTorqueOutput : public System, public SingleOutput<ForceTorqueSensor::cf_type> {
public:
	explicit ForceTorqueOutput(ForceTorqueSensor* sensor, boost::asio::io_service* sensorUpdater, const std::string& sysName = "ForceTorqueOutput") :
		System(sysName), SingleOutput<ForceTorqueSensor::cf_type>(this), sensor(sensor), sensorUpdater(sensorUpdater)
        {
            this->outputValue->setData(&sensor->getForce());
        }
	virtual ~ForceTorqueOutput() { mandatoryCleanUp(); }

protected:
	virtual void operate() {
		sensorUpdater->post(boost::bind(&ForceTorqueOutput::updateSensor, this));
		outputValue->setData(&sensor->getForce());
	}
	void updateSensor() { sensor->update(); }
	virtual void invalidateOutputs() { /* do nothing */ }

	ForceTorqueSensor* sensor;
	boost::asio::io_service* sensorUpdater;

private:
	DISALLOW_COPY_AND_ASSIGN(ForceTorqueOutput);
};

#endif /* FORCE_TORQUE_OUTPUT_H_ */
