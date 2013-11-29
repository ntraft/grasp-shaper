/*
 * tactile_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef TACTILE_OUTPUT_H_
#define TACTILE_OUTPUT_H_

#include <barrett/detail/ca_macro.h>
#include <barrett/products/product_manager.h>
#include <barrett/math/matrix.h>
#include <barrett/systems.h>
#include <barrett/systems/abstract/system.h>
#include <barrett/systems/abstract/single_io.h>

using namespace barrett;
using namespace barrett::systems;

typedef math::Matrix<5, TactilePuck::NUM_SENSORS, double> tactile_data;

class TactileOutput : public System, public SingleOutput<tactile_data> {
public:
	explicit TactileOutput(const std::string& sysName = "TactileOutput") :
		System(sysName), SingleOutput<tactile_data>(this), data(19.0)
	{
		this->outputValue->setData(&data);
	}
	virtual ~TactileOutput() { mandatoryCleanUp(); }

protected:
	virtual void operate() {
		outputValue->setData(&data);
	}
	virtual void invalidateOutputs() { /* do nothing */ }

	tactile_data data;

private:
	DISALLOW_COPY_AND_ASSIGN(TactileOutput);
};

#endif /* TACTILE_OUTPUT_H_ */
