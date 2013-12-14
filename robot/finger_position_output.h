/*
 * finger_position_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef FINGER_POSITION_OUTPUT_H_
#define FINGER_POSITION_OUTPUT_H_

#include "asynchronous_output.h"

#include <barrett/detail/ca_macro.h>
#include <barrett/products/product_manager.h>
#include <barrett/systems.h>
#include <barrett/systems/abstract/system.h>
#include <barrett/systems/abstract/single_io.h>

using namespace barrett;
using namespace barrett::systems;

class FingerPositionOutput : public AsynchronousOutput<Hand::jp_type> {
public:
	explicit FingerPositionOutput(Hand* hand, const std::string& sysName = "FingerPositionOutput") :
		AsynchronousOutput<Hand::jp_type>(sysName), hand(hand)
	{
		this->outputValue->setData(&hand->getOuterLinkPosition());
	}
	virtual ~FingerPositionOutput() { mandatoryCleanUp(); }

protected:
	void updateSensor() {
		hand->update(Hand::S_POSITION);
		outputValue->setData(&hand->getOuterLinkPosition());
		doneUpdating();
	}

	Hand* hand;

private:
	DISALLOW_COPY_AND_ASSIGN(FingerPositionOutput);
};

#endif /* FINGER_POSITION_OUTPUT_H_ */
