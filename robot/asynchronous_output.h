/*
 * asynchronous_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef ASYNCHRONOUS_OUTPUT_H_
#define ASYNCHRONOUS_OUTPUT_H_

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
	explicit AsynchronousOutput(const std::string& sysName = "AsynchronousOutput") :
		System(sysName), SingleOutput<OutputType>(this), updating(false) { }
	virtual ~AsynchronousOutput() { mandatoryCleanUp(); }

	virtual void updateSensor() =0;

protected:
	virtual void invalidateOutputs() { /* do nothing */ }
	void operate();
	void doneUpdating();

	boost::mutex updateLock;
	bool updating;

private:
	DISALLOW_COPY_AND_ASSIGN(AsynchronousOutput);
};

template<typename OutputType>
void updateEntryPoint(AsynchronousOutput<OutputType>* out) {
	out->updateSensor();
}

template<typename OutputType>
void AsynchronousOutput<OutputType>::doneUpdating() {
	updateLock.lock();
	updating = false;
	updateLock.unlock();
}

template<typename OutputType>
void AsynchronousOutput<OutputType>::operate() {
	updateLock.lock();
	if (!updating) {
		updating = true;
		boost::thread thr(updateEntryPoint<OutputType>, this);
	}
	updateLock.unlock();
}

#endif /* ASYNCHRONOUS_OUTPUT_H_ */
