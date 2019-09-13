#include "treasureHunt.h"

#include <iostream>
#include <algorithm>
#include <cctype>

using std::cin;
using std::cout;
using std::getline;
using std::stoi;

void TreasureHunt::read_data() {
	string junk;
	unsigned row = 0, col = 0;
	bool comment = true;
    bool is_map = false;

	while (getline(cin, junk, '\n')) {
		if (junk[0] == '#' && comment) {
			continue;
		} else if (junk[0] == 'M' || junk[0] == 'L') {
			if (junk[0] == 'M') {
				is_map = true;
			}
			comment = false;
		} else if (is_number(junk)) {
			map_size = static_cast<unsigned>(stoi(junk));
			map.resize(map_size);
			for (unsigned i = 0; i < map_size; ++i) {
				map[i].resize(map_size, '.');
			}
		} else { // reading map content
			if (is_map) {
				for (unsigned i = 0; i < map_size; ++i) {
					map[row][col++] = junk[i];
				}
				++row;
				col = 0;
			} else {
				cout << "Should not see this\n";
			}
		}
		cout << "line=" << junk << "\n";
	}
};

void TreasureHunt::get_options(int, char**) {

};

void TreasureHunt::hunt() {

};

bool TreasureHunt::is_number(const string &s) {
	string::const_iterator it = s.begin();
	while(it != s.end() && isdigit(*it)) {
		++it;
	}
	return !s.empty() && it == s.end();
}

void TreasureHunt::print_map() {
	for (unsigned i = 0; i < map_size; ++i) {
		for (unsigned j = 0; j < map_size; ++j) {
			cout << map[i][j];
		}
		cout << "\n";
	}
}
