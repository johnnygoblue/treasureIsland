#include "treasureHunt.h"
#include <iostream>

int main() {
	try {
		TreasureHunt hunt;
		hunt.read_data();
		hunt.print_map();
		hunt.print_start();
	}
	catch(std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
