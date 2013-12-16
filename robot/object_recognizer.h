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

#define INPUT_LAYER_SIZE 107
#define HIDDEN_LAYER_SIZE 25
#define LABEL_SIZE 50

std::string labelmap[51] = {
		std::string("bean bag,g"),
		std::string("bean bag,m"),
		std::string("bean bag,p"),
		std::string("bean bag,t"),
		std::string("bean bag,w"),
		std::string("bottle,g"),
		std::string("bottle,p"),
		std::string("bottle,w"),
		std::string("cardboard,p"),
		std::string("cardboard,g"),
		std::string("cardboard,w"),
		std::string("cube,g"),
		std::string("cube,w"),
		std::string("cube,m"),
		std::string("cube,t"),
		std::string("cube,p"),
		std::string("peanut can,g"),
		std::string("peanut can,p"),
		std::string("peanut can,w"),
		std::string("peanut can,m"),
		std::string("peanut can,t"),
		std::string("wooden egg,w"),
		std::string("wooden egg,g"),
		std::string("wooden egg,p"),
		std::string("wooden egg,m"),
		std::string("wooden egg,t"),
		std::string("(small) football,m"),
		std::string("(small) football,t"),
		std::string("(small) football,p"),
		std::string("(small) football,w"),
		std::string("(small) football,g"),
		std::string("octopus,g"),
		std::string("piece of foam,m"),
		std::string("piece of foam,t"),
		std::string("puck,m"),
		std::string("puck,t"),
		std::string("small piece of foam,g"),
		std::string("small piece of foam,w"),
		std::string("soccer ball,t"),
		std::string("styrofoam ball,m"),
		std::string("styrofoam ball,t"),
		std::string("styrofoam ball,g"),
		std::string("styrofoam ball,p"),
		std::string("wicker ball,t"),
		std::string("wicker ball,m"),
		std::string("wicker ball,g"),
		std::string("wicker ball,p"),
		std::string("wood,m"),
		std::string("wood,t"),
		std::string("wrist attachment,g"),
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
}

template<int R, int C>
void ObjectRecognizer::readmat(Matrix<R,C>& mat, const char* filename) {
	std::ifstream ifs(filename);
	std::string line;
	int i = 0;
	while (getline(ifs, line)) {
//      printf("Read line: %s\n", line.c_str());
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
        0, 0, 0, 0,
		(*forceTorqueOut->getValue()).transpose(),
		tactData->row(0),
		tactData->row(1),
		tactData->row(2),
		tactData->row(3);

	// Feed forward through neural network
	Matrix<1, HIDDEN_LAYER_SIZE> h1 = passThroughLayer(samples, layer1);
	Matrix<1, HIDDEN_LAYER_SIZE+1> h1a;
	h1a << 1, h1;
	Matrix<1, LABEL_SIZE> h2 = passThroughLayer(h1a, layer2);

	// Get maximum probable label.
	int dummy, prediction;
	h2.maxCoeff(&dummy, &prediction);
	std::cout << h2 << std::endl;
	const char* label = labelmap[prediction].c_str();
	if (label[0] == 'a' || label[0] == 'e' || label[0] == 'i' || label[0] == 'o' || label[0] == 'u') {
		printlog("This object is an %s (#%d).\n", label, prediction);
	} else {
		printlog("This object is a %s (#%d).\n", label, prediction);
	}
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
