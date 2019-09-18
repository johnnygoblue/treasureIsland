
// IDENTIFIER = 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include "treasureHunt.h"
#include <iostream>

int main(int argc, char **argv) {
	try {
		TreasureHunt hunt;
		hunt.get_options(argc, argv);
		hunt.read_data();
		hunt.hunt();
	}
	catch(std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
