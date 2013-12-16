/*
 * asynchronous_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef ASYNCHRONOUS_OUTPUT_H_
#define ASYNCHRONOUS_OUTPUT_H_

#include "utils.h"

#include <boost/thread.hpp>

#include <barrett/detail/ca_macro.h>
#include <barrett/products/product_manager.h>
#include <barrett/math/matrix.h>
#include <barrett/systems.h>
#include <barrett/systems/abstract/system.h>
#include <barrett/systems/abstract/single_io.h>

using namespace barrett;
using namespace barrett::systems;


template<typename OutputType>
class AsynchronousOutput : public System, public SingleOutput<OutputType> {
public:
	explicit AsynchronousOutput(const std::string& sysName = "AsynchronousOutput");

	virtual ~AsynchronousOutput() {
		threadRunner.interrupt();
		threadRunner.join();
		mandatoryCleanUp();
	}

	void checkUpdate();
	const OutputType* getValue() {
		return this->outputValue->getData();
	}

protected:
	virtual void invalidateOutputs() { /* do nothing */ }
	void operate();
	virtual void updateSensor() =0;

	boost::thread threadRunner;
	bool needsUpdate;

private:
	DISALLOW_COPY_AND_ASSIGN(AsynchronousOutput);
};

template<typename OutputType>
void updateEntryPoint(AsynchronousOutput<OutputType>* out) {
	while (true) {
		out->checkUpdate();
		Pause(2);
	}
}

template<typename OutputType>
AsynchronousOutput<OutputType>::AsynchronousOutput(const std::string& sysName) :
	System(sysName), SingleOutput<OutputType>(this),
	threadRunner(updateEntryPoint<OutputType>, this), needsUpdate(true) {
}

template<typename OutputType>
void AsynchronousOutput<OutputType>::checkUpdate() {
	if (needsUpdate) {
		updateSensor();
		// I know this doesn't guarantee that a new update wasn't requested
		// in the time since the update was performed. Who cares?
		needsUpdate = false;
	}
}

template<typename OutputType>
void AsynchronousOutput<OutputType>::operate() {
	needsUpdate = true;
}

#endif /* ASYNCHRONOUS_OUTPUT_H_ */
