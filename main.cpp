
#include "Grasper.h"
#include "utils.h"

#include <iostream>
#include <string>

#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>

#include <barrett/standard_main_function.h>

using namespace barrett;

void printMenu() {
	printf("Commands:\n");
	printf("  g\tInitiate a grasp\n");
	printf("   g\t Power grip [default]\n");
	printf("   p\t Prismatic precision\n");
	printf("   m\t Top-down prismatic\n");
	printf("   t\t Top-down tripod\n");
	printf("   w\t Heavy wrap\n");
	printf("  h\tMove to the home position\n");
	printf("  i\tIdle (release position/orientation constraints)\n");
	printf("  p\tPrint current joint position\n");
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
		return 1;
	}

	Grasper<DOF> grasper(&wam, hand);
	wam.gravityCompensate();
	std::cout << "Home position: " << wam.getHomePosition() << std::endl;
	printMenu();

	std::string line;
	bool going = true;
	while (going) {
		printf(">>> ");
		std::getline(std::cin, line);

		switch (line[0]) {
		case 'g':
			grasper.doGrasp(line[1]);
			break;

		case 'h':
			wam.moveHome(false);
			Pause(500);
			hand->open(false);
			while (!wam.moveIsDone() || !hand->doneMoving()) Pause(100);
			break;

		case 'i':
			printf("WAM idled.\n");
			wam.idle();
			break;

		case 'p':
			hand->update();
			std::cout << "WAM position: " << wam.getJointPositions() << std::endl;
			std::cout << "Hand position: " << hand->getOuterLinkPosition() << std::endl;
			break;

		case 'q':
		case 'x':
			printf("Quitting.\n");
			going = false;
			break;
		}
	}

	pm.getSafetyModule()->waitForMode(SafetyModule::IDLE);
	return 0;
}
