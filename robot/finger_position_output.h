/*
 * finger_position_output.h
 *
 *  Created on: Nov 28, 2013
 *      Author: jolande
 */

#ifndef FINGER_POSITION_OUTPUT_H_
#define FINGER_POSITION_OUTPUT_H_

#include <barrett/detail/ca_macro.h>
#include <barrett/products/product_manager.h>
#include <barrett/systems.h>
#include <barrett/systems/abstract/system.h>
#include <barrett/systems/abstract/single_io.h>

using namespace barrett;
using namespace barrett::systems;

class FingerPositionOutput : public System, public SingleOutput<Hand::jp_type> {
public:
	explicit FingerPositionOutput(Hand* hand, const std::string& sysName = "TactileOutput") :
		System(sysName), SingleOutput<Hand::jp_type>(this), hand(hand) { }
	virtual ~FingerPositionOutput() { mandatoryCleanUp(); }

protected:
	virtual void operate() {
		hand->update(Hand::S_POSITION, true);
		outputValue->setData(&hand->getOuterLinkPosition());
	}
	virtual void invalidateOutputs() { /* do nothing */ }

	Hand* hand;

private:
	DISALLOW_COPY_AND_ASSIGN(FingerPositionOutput);
};

#endif /* FINGER_POSITION_OUTPUT_H_ */
