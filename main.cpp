
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


void printMenu() {
	printf("Commands:\n");
	printf("  [Enter]\tInitiate a grasp\n");
	printf("  h\t\tMove to the home position\n");
	printf("  i\t\tIdle (release position/orientation constraints)\n");
	printf("  q\t\tQuit\n");
}

void msleep(int ms) {
	boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

void grasp(Hand* hand) {
	hand->close();
}

void ungrasp(Hand* hand) {
	hand->open();
}

template<size_t DOF>
void graspAndLift(systems::Wam<DOF>& wam, Hand* hand) {
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	double gp[] = {0, 0.713, 0, 2.211, 0, -1.458, 0};
	double lp[] = {0, 0.437, 0, 2.1, 0, -1.028, 0};
	jp_type graspPos = jp_type(gp);
	jp_type liftPos = jp_type(lp);

	wam.moveTo(graspPos);
	msleep(1000);
	grasp(hand);
	msleep(1000);
	wam.moveTo(liftPos);
	msleep(1000);
	wam.moveTo(graspPos);
	ungrasp(hand);
}

template<size_t DOF>
int wam_main(int argc, char** argv, ProductManager& pm, systems::Wam<DOF>& wam) {
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	double ip[] = {0, 0, 0, 2.211, 0, 0, 0};
	jp_type initPos = jp_type(ip);

	Hand* hand;
	if (pm.foundHand()) {
		hand = pm.getHand();
	} else {
		printf("No hand found! Exiting.");
		return 0;
	}

	wam.gravityCompensate();
	wam.moveTo(initPos);
	hand->initialize();
	printMenu();

	std::string line;
	bool going = true;
	while (going) {
		printf(">>> ");
		std::getline(std::cin, line);

		switch (line[0]) {
		case '\0':
			graspAndLift(wam, hand);
			break;

		case 'h':
			std::cout << "Moving to home position: " << wam.getHomePosition() << std::endl;
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
