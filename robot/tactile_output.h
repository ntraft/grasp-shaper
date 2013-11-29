/*
 * tactile_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef TACTILE_OUTPUT_H_
#define TACTILE_OUTPUT_H_

#include <barrett/detail/ca_macro.h>
#include <barrett/systems/abstract/system.h>
#include <barrett/systems/abstract/single_io.h>

using namespace barrett::systems;

class TactileOutput : public System, public SingleOutput<double> {
public:
	explicit TactileOutput(double value, const std::string& sysName = "TactileOutput") :
		System(sysName), SingleOutput<double>(this), data(value), operated(0)
	{
		this->outputValue->setData(&data);
	}
	virtual ~TactileOutput() { mandatoryCleanUp(); }

protected:
	virtual void operate() {
		data += 1;
		outputValue->setData(&data);
	}
	virtual void invalidateOutputs() { /* do nothing */ }

	double data;
	int operated;

private:
	DISALLOW_COPY_AND_ASSIGN(TactileOutput);
};

#endif /* TACTILE_OUTPUT_H_ */
