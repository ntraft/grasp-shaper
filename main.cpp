
#include <iostream>
#include <string>

#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>

#include <barrett/standard_main_function.h>


using namespace barrett;
using detail::waitForEnter;
using systems::connect;
using systems::disconnect;
using systems::reconnect;

#define FINGER_JOINT_LIMIT 2.4435 // = 140 degrees
#define PI 3.1415

void Pause(int ms = 1000) {
	boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

template<size_t DOF>
void prepareHand(systems::Wam<DOF>& wam, Hand* hand, char graspType) {
	Hand::jp_type prism;
	Hand::jp_type tripod;
	tripod[3] = 0.52;
	Hand::jp_type wrap;
	wrap[3] = PI;

	switch (graspType) {
	case '\0':
	case 'g':
	case 'p':
	case 'm':
		hand->trapezoidalMove(prism);
		break;
	case 't':
		hand->trapezoidalMove(tripod);
		break;
	case 'w':
		hand->trapezoidalMove(wrap);
		break;
	}
}

void grasp(Hand* hand, char graspType) {
	Hand::jp_type currPos = hand->getInnerLinkPosition();
	std::cout << "Inner link position: " << currPos << std::endl;
	currPos = hand->getOuterLinkPosition();
	std::cout << "Outer link position: " << currPos << std::endl;
	currPos[0] = FINGER_JOINT_LIMIT;
	currPos[1] = FINGER_JOINT_LIMIT;
	currPos[2] = FINGER_JOINT_LIMIT;
	hand->trapezoidalMove(currPos);
}

void ungrasp(Hand* hand) {
	hand->open();
}

template<size_t DOF>
void liftAndReturn(systems::Wam<DOF>& wam) {
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	jp_type targetPos = wam.getJointPositions();
	jp_type liftPos = jp_type(targetPos);
	liftPos[1] -= 0.3;
	wam.moveTo(liftPos);
	Pause(2000);
	wam.moveTo(targetPos);
}

template<size_t DOF>
void graspAndLift(systems::Wam<DOF>& wam, Hand* hand, char graspType) {
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	double sp[] = {0, 0, 0, 1.57, 0, 0, 0};
	jp_type safePos = jp_type(sp);
	double p1[] = {0, 0.713, 0, 2.211, 0, -1.458, 0};
	jp_type powerPos = jp_type(p1);
	double p2[] = {0, 0.651, 0, 2.404, -0.113, -1.458, 0};
	jp_type precisionPos = jp_type(p2);
	double p3[] = {0, 0.705, 0, 1.174, 0, 1.205, 0};
	jp_type topDownPos = jp_type(p3);

	jp_type targetPos;
	switch (graspType) {
	case '\0':
	case 'g':
	case 'w':
		targetPos = powerPos;
		break;
	case 'p':
		targetPos = precisionPos;
		break;
	case 'm':
	case 't':
		targetPos = topDownPos;
		break;
	}

	wam.moveTo(safePos);
	prepareHand(wam, hand, graspType);
	wam.moveTo(targetPos);
	Pause();
	liftAndReturn(wam);
	Pause();
	ungrasp(hand);
}

void printMenu() {
	printf("Commands:\n");
	printf("  g\tInitiate a grasp\n");
	printf("   \tg\tPower grip [default]\n");
	printf("   \tp\tPrismatic precision\n");
	printf("   \tm\tTop-down prismatic\n");
	printf("   \tt\tTop-down tripod\n");
	printf("   \tw\tHeavy wrap\n");
	printf("  h\tMove to the home position\n");
	printf("  i\tIdle (release position/orientation constraints)\n");
	printf("  q\tQuit\n");
}

template<size_t DOF>
int wam_main(int argc, char** argv, ProductManager& pm, systems::Wam<DOF>& wam) {
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	Hand* hand;
	if (pm.foundHand()) {
		hand = pm.getHand();
	} else {
		printf("No hand found! Exiting.");
		return 0;
	}

	wam.gravityCompensate();
	jp_type initPos = wam.getHomePosition();
	std::cout << "Home position: " << initPos << std::endl;
	Pause();
	printf("Initalizing hand\n");
	initPos[3] = 2.211;
	wam.moveTo(initPos);
	hand->initialize();
	printMenu();

	std::string line;
	bool going = true;
	while (going) {
		printf(">>> ");
		std::getline(std::cin, line);

		switch (line[0]) {
		case 'g':
			graspAndLift(wam, hand, line[1]);
			break;

		case 'h':
			wam.moveHome();
			break;

		case 'i':
			printf("WAM idled.\n");
			wam.idle();
			break;

		case 'q':
		case 'x':
			printf("Quitting.\n");
			going = false;
			break;
		}
	}


	wam.idle();
	pm.getSafetyModule()->waitForMode(SafetyModule::IDLE);
	return 0;
}
