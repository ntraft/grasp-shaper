/*
 * force_torque_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef FORCE_TORQUE_OUTPUT_H_
#define FORCE_TORQUE_OUTPUT_H_

#include <barrett/detail/ca_macro.h>
#include <barrett/products/product_manager.h>
#include <barrett/systems.h>
#include <barrett/systems/abstract/system.h>
#include <barrett/systems/abstract/single_io.h>

using namespace barrett;
using namespace barrett::systems;

class ForceTorqueOutput : public System, public SingleOutput<ForceTorqueSensor::cf_type> {
public:
	explicit ForceTorqueOutput(ForceTorqueSensor* sensor, const std::string& sysName = "ForceTorqueOutput") :
		System(sysName), SingleOutput<ForceTorqueSensor::cf_type>(this), sensor(sensor) { }
	virtual ~ForceTorqueOutput() { mandatoryCleanUp(); }

protected:
	virtual void operate() {
		sensor->update(true);
		outputValue->setData(&sensor->getForce());
	}
	virtual void invalidateOutputs() { /* do nothing */ }

	ForceTorqueSensor* sensor;

private:
	DISALLOW_COPY_AND_ASSIGN(ForceTorqueOutput);
};

#endif /* FORCE_TORQUE_OUTPUT_H_ */
