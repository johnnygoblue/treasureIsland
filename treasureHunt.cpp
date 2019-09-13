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
	int row = 0, col = 0;
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
			map_size = stoi(junk);
			map.resize(static_cast<size_t>(map_size));
			for (int i = 0; i < map_size; ++i) {
				map[static_cast<size_t>(i)].resize(static_cast<size_t>(map_size), '.');
			}
		} else { // reading map content
			if (is_map) {
				for (int i = 0; i < map_size; ++i) {
					map[static_cast<size_t>(row)][static_cast<size_t>(col)] = junk[static_cast<size_t>(i)];
					if (map[static_cast<size_t>(row)][static_cast<size_t>(col)] == '@') {
						start.row = row;
						start.col = col;
					}
					++col;
				}
				++row;
				col = 0;
			} else {
				map[static_cast<size_t>(junk[0]-48)][static_cast<size_t>(junk[2]-48)] = junk[4];
				if (junk[4] == '@') {
					start.row = junk[0]-48;
					start.col = junk[2]-48;
				}
			}
		}
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
	for (int i = 0; i < map_size; ++i) {
		for (int j = 0; j < map_size; ++j) {
			cout << map[static_cast<size_t>(i)][static_cast<size_t>(j)];
		}
		cout << "\n";
	}
}

void TreasureHunt::print_start() {
	cout << "start location (" << start.row << " , " <<
		start.col << ")\n";
}

bool TreasureHunt::explore_cell(int row, int col, bool on_land) {
	// check if out of bounds
	if (row < 0 || row >= map_size || col < 0 || col >= map_size) {
		return false;
	}
	// check if terrain unpassable
	if (map[static_cast<size_t>(row)][static_cast<size_t>(col)] == '#') {
		return false;
	}
	// check if cell is already explored
	if (map[static_cast<size_t>(row)][static_cast<size_t>(col)] == 'n' || map[static_cast<size_t>(row)][static_cast<size_t>(col)] == 'e' || map[static_cast<size_t>(row)][static_cast<size_t>(col)] == 's' || map[static_cast<size_t>(row)][static_cast<size_t>(col)] == 'w') {
		return false;
	}
	// if on-land, check if ocean cell
	if (on_land && map[static_cast<size_t>(row)][static_cast<size_t>(col)] == '.') {
		return false;
	}
	return true;
}
