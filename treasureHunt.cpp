#include "treasureHunt.h"

#include <iostream>
#include <algorithm>
#include <cctype>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::getline;
using std::stoi;

void TreasureHunt::read_data() {
	string junk;
	Cell start(-1, -1);
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
				map[static_cast<size_t>(i)].resize(static_cast<size_t> \
						(map_size), '.');
			}
		} else { // reading map content
			if (is_map) {
				for (int i = 0; i < map_size; ++i) {
					map[static_cast<size_t>(row)][static_cast<size_t>(col)] \
						= junk[static_cast<size_t>(i)];
					if (map[static_cast<size_t>(row)][static_cast<size_t> \
							(col)] == '@') {
						start.row = row;
						start.col = col;
						sea.push_back(start);
					}
					++col;
				}
				++row;
				col = 0;
			} else {
				map[static_cast<size_t>(junk[0]-'0')][static_cast<size_t> \
					(junk[2]-'0')] = junk[4];
				if (junk[4] == '@') {
					start.row = junk[0]-'0';
					start.col = junk[2]-'0';
					sea.push_back(start);
				}
			}
		}
	}
};

void TreasureHunt::get_options(int, char**) {

};

void TreasureHunt::hunt() {
	captain_do();
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
	cout << "start location (" << sea.front().row << " , " <<
		sea.front().col << ")\n";
}

bool TreasureHunt::is_valid_cell(Cell c, bool on_land) {
	// check if out of bounds
	if (!is_in_bound(c)) {
		return false;
	}
	// check if terrain unpassable
	if (map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] == '#') {
		return false;
	}
	// check if cell is already explored
	if (map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] == 'n' ||
		map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] == 'e' ||
		map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] == 's' ||
		map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] == 'w') {
		return false;
	}
	// if on-land, check if ocean cell
	if (on_land && map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] \
			== '.') {
		return false;
	}
	return true;
}

inline bool TreasureHunt::is_land(Cell c) {
	return map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] == 'o';
}

inline bool TreasureHunt::is_in_bound(Cell c) {
	if (c.row < 0 || c.row >= map_size) {
		return false;
	}
	if (c.col < 0 || c.col >= map_size) {
		return false;
	}
	return true;
}

inline void TreasureHunt::set_cell(Cell c, const char ch) {
	if (!is_in_bound(c)) {
		cerr << "Cell out of bound in " << __func__ << endl;
	}
	map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] = ch;
}

inline bool TreasureHunt::is_ashore(Cell c, char dir) {
	Cell next(-1, -1);
	switch (dir) {
		case 'N':
			next = c.north();
			break;
		case 'E':
			next = c.east();
			break;
		case 'S':
			next = c.south();
			break;
		case 'W':
			next = c.west();
			break;
		default:
			cerr << "Unknown direction '" << dir <<
				"' in " << __func__ << endl;
	}
	if (is_valid_cell(next, false) && is_land(next)) {
		land.push_back(next);
		set_cell(c, dir);
		return true;
	}
	return false;
}

// Make sure is_valid_cell returns true before you call this function
inline bool TreasureHunt::is_treasure(Cell c, char dir) {
	Cell next(-1, -1);
	switch (dir) {
		case 'N':
			next = c.north();
			break;
		case 'E':
			next = c.east();
			break;
		case 'S':
			next = c.south();
			break;
		case 'W':
			next = c.west();
			break;
		default:
			cerr << "Unknown direction '" << dir <<
				"' in " << __func__ << endl;
	}
	if (map[static_cast<size_t>(next.row)][static_cast<size_t>(next.col)] == '$') {
		treasure.row = next.row;
		treasure.col = next.col;
		return true;
	}
	return false;
}

void TreasureHunt::captain_do() {
	if (sea.empty()) {
		return;
	} else {
		Cell curr = Cell(-1, -1);

		// get next cell and pop it from sail container
		if (capt_mode == 's') {
			curr = sea.back();
			sea.pop_back();
		} else {
			curr = sea.front();
			sea.pop_front();
		}

		// explore surrounding cells
		for (int i = 0; i < 4; ++i) {
			switch (order[i]) {
				case 'N':
					if (is_ashore(curr, 'N')) {
						first_mate_do();
					} else {
						add_cell(curr.north(), false);
					}
					break;
				case 'E':
					if (is_ashore(curr, 'E')) {
						first_mate_do();
					} else {
						add_cell(curr.east(), false);
					}
					break;
				case 'S':
					if (is_ashore(curr, 'S')) {
						first_mate_do();
					} else {
						add_cell(curr.south(), false);
					}
					break;
				case 'W':
					if (is_ashore(curr, 'W')) {
						first_mate_do();
					} else {
						add_cell(curr.west(), false);
					}
					break;
			} // switch
		} // for
		captain_do();
	} // else
}

// The ashore (sea->land) case should already be handled before
// attempt to call this function. Therefore, this function only
// handles seas->sea and land->land cell exploration
void TreasureHunt::add_cell(Cell c, bool on_land) {
	// if on-land and is valid cell
	if (on_land && is_valid_cell(c, on_land)) {
		land.push_back(c);
	}
	// if on-sea and cell is valid sea
	if (!on_land && is_valid_cell(c, on_land) && !is_land(c)) {
		sea.push_back(c);
	}
}

void TreasureHunt::first_mate_do() {
	if (land.empty()) {
		return;
	} else {
		Cell curr = Cell(-1, -1);
		if (mate_mode == 's') {
			curr = land.back();
			land.pop_back();
		} else {
			curr = land.front();
			land.pop_front();
		}
		for (int i = 0; i < 4; ++i) {
			switch (order[i]) {
				case 'N':
					add_cell(curr.north(), true);
					break;
				case 'E':
					add_cell(curr.east(), true);
					break;
				case 'S':
					add_cell(curr.south(), true);
					break;
				case 'W':
					add_cell(curr.west(), true);
					break;
			} // switch
		} // for
		first_mate_do();
	} // else
}
