
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

template<size_t DOF>
void sleep(unsigned int ms) {
	boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

template<size_t DOF, int R, int C, typename Units>
void blockingMove(systems::Wam<DOF>& wam, math::Matrix<R,C, Units> pos) {
	std::cout << "Move to: " << pos << std::endl;
	wam.moveTo(pos);
	printf("Waiting for move to complete...\n");
	while (!wam.moveIsDone()) {
		sleep(10);
	}
	printf("Done.\n");
}

template<size_t DOF>
void graspAndLift(systems::Wam<DOF>& wam) {
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	double gp[] = {0, 0.713, 0, 2.211, 0, -1.458, 0};
	double lp[] = {0, 0.437, 0, 2.1, 0, -1.028, 0};
	jp_type graspPos = jp_type(gp);
	jp_type liftPos = jp_type(lp);

	blockingMove(wam, graspPos);
//	sleep(1000);
	blockingMove(wam, liftPos);
//	sleep(1000);
	blockingMove(wam, graspPos);
}

template<size_t DOF>
int wam_main(int argc, char** argv, ProductManager& pm, systems::Wam<DOF>& wam) {

	wam.gravityCompensate();

	printMenu();

	std::string line;
	bool going = true;
	while (going) {
		char c = std::getchar();

		switch (c) {
		case '\n':
			graspAndLift(wam);
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
