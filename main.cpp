
#include "Grasper.h"
#include "utils.h"

#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>

#include <barrett/standard_main_function.h>

using namespace barrett;

#define NAME_LIMIT 64

void printMenu() {
	printw("Grasping commands:\n");
	printw("  Shapes:\n");
	printw("  g\t Power grip [default]\n");
	printw("  p\t Prismatic precision\n");
	printw("  m\t Top-down prismatic\n");
	printw("  t\t Top-down tripod\n");
	printw("  w\t Heavy wrap\n");
	printw("  Metadata:\n");
	printw("  n\t Set object name\n");
	printw("  f\t Record a failed grasp\n");
	printw("Basic commands:\n");
	printw("  h\tMove to the home position\n");
	printw("  i\tIdle (release position/orientation constraints)\n");
	printw("  r\tReset hand (make sure it has room!)\n");
	printw("  o\tOpen hand\n");
	printw("  c\tClose hand\n");
	printw("  j\tPrint current joint position\n");
	printw("  q\tQuit\n");
	refresh();
}

#define BARRETT_SMF_CONFIGURE_PM
bool configure_pm(int argc, char** argv, ::barrett::ProductManager& pm) {
	printf("The WAM is not configured correctly!\n");
	printf("This program requires a 7-degree WAM, a BarrettHand, and a 6-DOF force/torque sensor.\n");
	return pm.foundWam7() && pm.foundHand() && pm.foundForceTorqueSensor();
}

template<size_t DOF>
int wam_main(int argc, char** argv, ProductManager& pm, systems::Wam<DOF>& wam) {
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);

	Hand* hand = pm.getHand();
	Grasper<DOF> grasper(pm.getExecutionManager(), &wam, hand, pm.getForceTorqueSensor());
	wam.gravityCompensate();

	// Ready the screen for display and input.
	initscr();
	noecho();
	cbreak();
	printMenu();
	int consoleTop = getcury(stdscr);
	move(consoleTop+=2, 0);
	printw("Object Name: ");
	int y, x, namey, namex;
	getyx(stdscr, namey, namex);
	move(consoleTop+=2, 0);
	printData("Home position: ", wam.getHomePosition());

	int c;
	char* newName;
	bool going = true;
	while (going) {
		c = getch();

		switch (c) {
		case 'g': case 'p': case 'm': case 't': case 'w':
			try {
				grasper.doGrasp(c);
			} catch (int e) {
				printlog("Invalid grasp type.\n");
			}
			break;

		case 'f':
			grasper.failedGrasp();
			break;

		case 'n':
			echo();
			newName = new char[NAME_LIMIT];
            getyx(stdscr, y, x);
            move(namey, namex);
            clrtoeol();
			getnstr(newName, NAME_LIMIT);
            move(y, x);
			noecho();
			grasper.setObjectName(newName);
			break;

		case 'h':
			grasper.halt();
			wam.moveHome(false);
			Pause(500);
			hand->open(false);
			break;

		case 'i':
			grasper.halt();
			wam.idle();
            hand->idle();
			break;

		case 'r':
			grasper.halt();
			printlog("Initializing hand.\n");
			hand->initialize();
			break;

		case 'o':
			printlog("Opening hand.\n");
			hand->open(false);
			break;

		case 'c':
			printlog("Closing hand.\n");
			hand->close(false);
			break;

		case 'j':
			hand->update();
			printData("WAM position: ", wam.getJointPositions());
			printData("Hand outer position: ", hand->getOuterLinkPosition());
			printData("Hand inner position: ", hand->getInnerLinkPosition());
			break;

		case 'q':
		case 'x':
		case 27: // Esc
			grasper.halt();
			printlog("Quitting.\n");
			going = false;
			break;
		}
	}

	printlog("Please Shift-idle the WAM.\n");
	pm.getSafetyModule()->waitForMode(SafetyModule::IDLE);
	endwin();
	return 0;
}
