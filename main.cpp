
#include "Grasper.h"
#include "utils.h"

#include <curses.h>

#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>

#include <barrett/standard_main_function.h>

using namespace barrett;


void printMenu(int &line) {
	mvprintw(line++, 0, "Commands:\n");
	mvprintw(line++, 0, "  g\tInitiate a grasp\n");
	mvprintw(line++, 0, "   g\t Power grip [default]\n");
	mvprintw(line++, 0, "   p\t Prismatic precision\n");
	mvprintw(line++, 0, "   m\t Top-down prismatic\n");
	mvprintw(line++, 0, "   t\t Top-down tripod\n");
	mvprintw(line++, 0, "   w\t Heavy wrap\n");
	mvprintw(line++, 0, "  h\tMove to the home position\n");
	mvprintw(line++, 0, "  i\tIdle (release position/orientation constraints)\n");
	mvprintw(line++, 0, "  r\tReset hand (make sure it has room!)\n");
	mvprintw(line++, 0, "  o\tOpen hand\n");
	mvprintw(line++, 0, "  c\tClose hand\n");
	mvprintw(line++, 0, "  j\tPrint current joint position\n");
	mvprintw(line++, 0, "  q\tQuit\n");
}

void printlog(const char* str, ...) {
	va_list args;
	va_start(args, str);
	printw(str, args);
	va_end(args);
}

#define BARRETT_SMF_CONFIGURE_PM
bool configure_pm(int argc, char** argv, ::barrett::ProductManager& pm) {
	printf("The WAM is not configured correctly!\n");
	printf("This program requires a 7-degree WAM, a BarrettHand, and a 6-DOF force/torque sensor.\n");
	return pm.foundWam7() && pm.foundHand() && pm.foundForceTorqueSensor();
}

template<int R, typename Units>
void printData(const char* msg, const math::Matrix<R,1, Units>& from) {
	printlog(msg);
	printw("[%6.3f", from[0]);
	for (size_t i = 1; i < R; ++i) {
		printw(", %6.3f", from[i]);
	}
	printw("]\n");
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
	refresh();
	int consoleTop = 0;
	printMenu(consoleTop);
	move(++consoleTop, 0);
	printData("Home position: ", wam.getHomePosition());
	refresh();

	int c;
	bool going = true;
	while (going) {
		c = getch();

		switch (c) {
		case 'g': case 'p': case 'm': case 't': case 'w':
			grasper.doGrasp(c);
			break;

		case 'h':
			wam.moveHome(false);
			Pause(500);
			hand->open(false);
			while (!wam.moveIsDone() || !hand->doneMoving()) Pause(100);
			break;

		case 'i':
			printlog("WAM idled.\n");
			wam.idle();
            hand->idle();
			break;

		case 'r':
			printlog("Initializing hand.\n");
			hand->initialize();
			break;

		case 'o':
			printlog("Opening hand.\n");
			hand->open();
			break;

		case 'c':
			printlog("Closing hand.\n");
			hand->close();
			break;

		case 'j':
			hand->update();
			printData("WAM position: ", wam.getJointPositions());
			printData("Hand position: ", hand->getOuterLinkPosition());
			break;

		case 'q':
		case 'x':
		case 27: // Esc
			printlog("Quitting.\n");
			going = false;
			break;
		}
	}

	pm.getSafetyModule()->waitForMode(SafetyModule::IDLE);
	endwin();
	return 0;
}
