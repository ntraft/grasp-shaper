/*
 * object_recognizer.h
 *
 *  Created on: Dec 15, 2013
 *      Author: jolande
 */

#ifndef OBJECT_RECOGNIZER_H_
#define OBJECT_RECOGNIZER_H_

#include "utils.h"
#include "finger_position_output.h"
#include "fingertip_torque_output.h"
#include "force_torque_output.h"
#include "tactile_output.h"

#include <barrett/math/matrix.h>

using namespace barrett::math;

#define INPUT_LAYER_SIZE 101
#define HIDDEN_LAYER_SIZE 25
#define LABEL_SIZE 17

std::string labelmap[LABEL_SIZE+1] = {
		std::string("Nothing"),
		std::string("BeanBag"),
		std::string("Bottle"),
		std::string("Cone"),
		std::string("Cookie"),
		std::string("Cube"),
		std::string("Egg"),
		std::string("FoamSquare"),
		std::string("FoamStar"),
		std::string("FoamUgly"),
		std::string("Football"),
		std::string("Octopus"),
		std::string("SoftFoam"),
		std::string("Tape"),
		std::string("WhiteBall"),
		std::string("WickerBall"),
		std::string("WoodBlock"),
		std::string("Fail"),
};

class ObjectRecognizer {
public:
	ObjectRecognizer(FingerPositionOutput* fingerPosOut, ForceTorqueOutput* forceTorqueOut, TactileOutput* tactOut);
	virtual ~ObjectRecognizer() {}

	void predictObject(char graspType);

protected:

	template<int R, int C>
	void readmat(Matrix<R,C>& mat, const char* filename);

	void predict(
			Matrix<INPUT_LAYER_SIZE+1, HIDDEN_LAYER_SIZE> layer1,
			Matrix<HIDDEN_LAYER_SIZE+1, LABEL_SIZE> layer2);

	template<int NumSamples, int FromSize, int ToSize>
	Matrix<NumSamples, ToSize> passThroughLayer(
			Matrix<NumSamples, FromSize> data,
			Matrix<FromSize, ToSize> layer);

	template<int R, int C>
	Matrix<R,C> sigmoid(Matrix<R,C> mat);

	// Sensor Data (NN features)
	FingerPositionOutput* fingerPosOut;
    ForceTorqueOutput* forceTorqueOut;
    TactileOutput* tactOut;

    // NN Parameters
    Matrix<INPUT_LAYER_SIZE+1, HIDDEN_LAYER_SIZE> glayer1;
    Matrix<HIDDEN_LAYER_SIZE+1, LABEL_SIZE> glayer2;
    Matrix<INPUT_LAYER_SIZE+1, HIDDEN_LAYER_SIZE> mlayer1;
    Matrix<HIDDEN_LAYER_SIZE+1, LABEL_SIZE> mlayer2;
    Matrix<INPUT_LAYER_SIZE+1, HIDDEN_LAYER_SIZE> wlayer1;
    Matrix<HIDDEN_LAYER_SIZE+1, LABEL_SIZE> wlayer2;
};

ObjectRecognizer::ObjectRecognizer(FingerPositionOutput* fingerPosOut, ForceTorqueOutput* forceTorqueOut, TactileOutput* tactOut) :
	fingerPosOut(fingerPosOut), forceTorqueOut(forceTorqueOut), tactOut(tactOut)
{
	readmat(glayer1, "glayer1.ssv");
	readmat(glayer2, "glayer2.ssv");
	readmat(mlayer1, "mlayer1.ssv");
	readmat(mlayer2, "mlayer2.ssv");
	readmat(wlayer1, "wlayer1.ssv");
	readmat(wlayer2, "wlayer2.ssv");
}

template<int R, int C>
void ObjectRecognizer::readmat(Matrix<R,C>& mat, const char* filename) {
	std::ifstream ifs(filename);
	std::string line;
	int i = 0;
	while (getline(ifs, line)) {
		if (!parseDoubles(mat, i++, line)) {
			printf("WARNING: SSV file parsing failed: %s\n", filename);
		}
	}
	ifs.close();
}

void ObjectRecognizer::predictObject(char graspType) {
	switch (graspType) {
	case 'g':
		predict(glayer1, glayer2);
		break;
	case 'm':
		predict(mlayer1, mlayer2);
		break;
	case 'w':
		predict(wlayer1, wlayer2);
		break;
	}
}

void ObjectRecognizer::predict(
		Matrix<INPUT_LAYER_SIZE+1, HIDDEN_LAYER_SIZE> layer1,
		Matrix<HIDDEN_LAYER_SIZE+1, LABEL_SIZE> layer2)
{
	// Collect Samples
	Matrix<1, INPUT_LAYER_SIZE+1> samples;
	const tactile_data* tactData = tactOut->getValue();
	samples << 1,
		(*fingerPosOut->getValue()).transpose(),
		(*forceTorqueOut->getValue()).transpose(),
		tactData->block(0,1,1,2),
		tactData->block(0,1,4,92),
		tactData->row(1),
		tactData->row(2),
		tactData->row(3);

	// Feed forward through neural network
	Matrix<1, HIDDEN_LAYER_SIZE> h1 = passThroughLayer(samples, layer1);
	Matrix<1, HIDDEN_LAYER_SIZE+1> h1a;
	h1a << 1, h1;
	Matrix<1, LABEL_SIZE> h2 = passThroughLayer(h1a, layer2);

	// Get maximum probable label.
	int prediction = 0;
	double max = 0;
	for (int i = 0; i < LABEL_SIZE; i++) {
		if (h2(0, i) > max) {
			max = h2(0, i);
			prediction = i;
		}
	}
	const char* label = labelmap[prediction+1].c_str();
	if (label[0] == 'a' || label[0] == 'e' || label[0] == 'i' || label[0] == 'o' || label[0] == 'u') {
		printw("This object is an %s (#%d).\n", label, prediction);
	} else {
		printw("This object is a %s (#%d).\n", label, prediction);
	}
	refresh();
}

template<int NumSamples, int FromSize, int ToSize>
Matrix<NumSamples, ToSize> ObjectRecognizer::passThroughLayer(
		Matrix<NumSamples, FromSize> data,
		Matrix<FromSize, ToSize> layer)
{
	return sigmoid<NumSamples, ToSize>(data * layer);
}

template<int R, int C>
Matrix<R,C> ObjectRecognizer::sigmoid(Matrix<R,C> mat) {
	return ((-mat).cwise().exp().cwise() + 1.0).cwise().inverse();
}

#endif /* OBJECT_RECOGNIZER_H_ */
