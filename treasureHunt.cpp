// IDENTIFIER = 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include "treasureHunt.h"

#include <iostream>
#include <getopt.h>
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
	int row = 0, col = 0;
	bool comment = true;
    bool is_map = false;

	while (getline(cin, junk, '\n')) {
		if (junk[0] == '#' && comment) { // continue if is comment line
			continue;
		} else if (junk[0] == 'M' || junk[0] == 'L') { // determine input mode
			if (junk[0] == 'M') {
				is_map = true;
			}
			comment = false;
		} else if (is_number(junk)) { // if line is map size
			map_size = stoi(junk);
			map.resize(static_cast<size_t>(map_size));
			for (int i = 0; i < map_size; ++i) {
				map[static_cast<size_t>(i)].resize(static_cast<size_t> \
						(map_size), '.');
			}
		} else if (!comment && junk.empty()) { // if empty line
			continue;
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
}

void TreasureHunt::get_options(int argc, char **argv) {
	string stack = "STACK";
	string st = "S";
	string queue = "QUEUE";
	string q = "Q";
	int option_index = 0, option = 0;
	opterr = false;
	int dir_count[4] = {-1, -1, -1, -1}; // count N, E, S, W
	int cnt_show_path = 0;

	struct option longOpts[] = {{ "captain", required_argument, nullptr, 'c' },
								{ "first-mate", required_argument, nullptr, 'f'},
								{ "hunter-order", required_argument, nullptr, 'o'},
								{ "show-path", required_argument, nullptr, 'p'},
								{ "verbose", no_argument, nullptr, 'v'},
								{ "stats", no_argument, nullptr, 's'},
								{ "help", no_argument, nullptr, 'h'},
								{ nullptr, 0, nullptr, '\0'}};
    while ((option = getopt_long(argc, argv, "c:f:p:o:vsh", longOpts, &option_index)) != -1) {
		switch (option) {
			case 'c':
				if (strcmp(optarg, st.c_str()) == 0 ||
					strcmp(optarg, stack.c_str()) == 0) {
					capt_mode = 's';
				} else if (strcmp(optarg, q.c_str()) == 0 ||
					strcmp(optarg, queue.c_str()) == 0) {
					capt_mode = 'q';
				} else {
					cerr << "Invalid input argument for captain mode" << endl;
					exit(1);
				}
				break;
			case 'f':
				if (strcmp(optarg, st.c_str()) == 0 ||
					strcmp(optarg, stack.c_str()) == 0) {
					mate_mode = 's';
				} else if (strcmp(optarg, q.c_str()) == 0 ||
						strcmp(optarg, queue.c_str()) == 0) {
					mate_mode = 'q';
				} else {
					cerr << "Invalid input argument for first-mate" << endl;
					exit(1);
				}
				break;
			case 'o':
				if (optarg[4] != '\0') {
					cerr << "Wrong number of directions in hunt-order" << endl;
					exit(1);
				} //if
				for (int i = 0; i < 4; ++i) {
					switch (optarg[i]) {
						case 'n':
							dir_count[0]++;
							break;
						case 'e':
							dir_count[1]++;
							break;
						case 's':
							dir_count[2]++;
							break;
						case 'w':
							dir_count[3]++;
							break;
						default:
							cerr << "Uncognized direction in hunt order" << endl;
							exit(1);
					} // switch
				} // for
				for (int i = 0; i < 4; ++i) {
					if (dir_count[i]) {
						cerr << "Direction mismatch in hunt order" << endl;
						exit(1);
					} // if
					order[i] = (char) (optarg[i]-32); // lower to upper
				} // for
				break;
			case 'p':
				if (cnt_show_path >= 1) {
					cerr << "Show path can only be set once" << endl;
					exit(1);
				}
				if (optarg[0] == 'M') {
					cnt_show_path++;
					show_path = 'M';
				} else if (optarg[0] == 'L') {
					cnt_show_path++;
					show_path = 'L';
				} else {
					cerr << "Invalid show path argument" << endl;
					exit(1);
				}
				break;
			case 'v':
				print_verbose = true;
				break;
			case 's':
				print_stats = true;
				break;
			case 'h':
				cout << "EECS 281 Project 1 Treasure Hunt\n";
				cout << "Please refer to specs to simulate the game!\n";
				exit(0);
			default:
				cerr << "Unrecognized option '" << option << "'" << endl;
				exit(1);
		} // switch
	} // while
}

void TreasureHunt::hunt() {
	if (print_verbose) {
		cout << "Treasure hunt started at: " << start.row << "," <<
			start.col << "\n";
	}
	captain_do();
}

// Cell is valid cell before you call this function
inline char TreasureHunt::get_cell(Cell c) {
	return map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)];
}

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

void TreasureHunt::print_cell(Cell c) {
	cout << "" << c.row << "," << c.col << " ";
}

void TreasureHunt::print_current_cell(Cell c, bool on_land) {
	if (on_land) {
		cout << "[first-mate]";
	} else {
		cout << "[captain]";
	}
	cout << " current cell " << c.row << "," << c.col << "\n";
}

void TreasureHunt::print_start() {
	cout << "Start location: ";
	print_cell(sea.back());
}

void TreasureHunt::print_sea() {
	cout << "sea: ";
	deque<Cell> sea_copy = sea;
	while (!sea_copy.empty()) {
		print_cell(sea_copy.back());
		sea_copy.pop_back();
	}
	cout << "\n";
}

void TreasureHunt::print_land() {
	cout << "land: ";
	deque<Cell> land_copy = land;
	while (!land_copy.empty()) {
		print_cell(land_copy.front());
		land_copy.pop_front();
	}
	cout << "\n";
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
	if (tolower(map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)]) == 'n' ||
		tolower(map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)]) == 'e' ||
		tolower(map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)]) == 's' ||
		tolower(map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)]) == 'w') {
		return false;
	}
	// if on-land, check if ocean cell
	if (on_land && map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] \
			== '.') {
		return false;
	}
	return true;
}

inline bool TreasureHunt::cell_equal(Cell c1, Cell c2) {
	return (c1.row == c2.row) && (c1.col == c2.col);
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

// This function should only be called when the Cell c is a sea cell
// Given Cell c and diretion dir to c, return whether the target cell
// is a land cell
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
	} //switch
	if (is_valid_cell(next, false)) {
		if (is_treasure(next, dir)) { // if cell ashore is treasure
			if (print_verbose) {
				cout << "Went ashore at: " << next.row << "," << next.col << "\n";
				cout << "Searching island... ";
			}
			ashore++;
			return true;
		} else if (is_land(next)) { // if cell ashore is good land
			if (print_verbose) {
				cout << "Went ashore at: " << next.row << "," << next.col << "\n";
				cout << "Searching island... ";
			}
			ashore++;
			land.push_back(next);
			set_cell(next, dir);
			return true;
		} else {
			return false;
		}
	} // if is_valid_cell
	return false;
}

// Make sure is_valid_cell returns true before you call this function
// Check whether current cell is indeed treasure
inline bool TreasureHunt::is_treasure(Cell c, char dir) {
	if (map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] == '$') {
		treasure.row = c.row;
		treasure.col = c.col;
		set_cell(treasure, dir);
		land_loc++;
		treasure_found = true;
		if (print_verbose) {
			cout << "party found treasure at " << treasure.row << "," <<
				treasure.col << ".\n";
		}
		calculate_path_length();
		clean_up();
		return true;
	}
	return false;
}

void TreasureHunt::captain_do() {
	if (sea.empty()) {
		if (!treasure_found) {
			if (print_verbose) {
				cout << "Treasure hunt failed\n";
			}
			cout << "No treasure found after investigating " << water_loc +
				land_loc << " locations.\n";
		}
		if (print_stats) {
			print_hunt_stats();
		}
		print_path();
		return;
	} else {
		water_loc++;
		Cell curr = Cell(-1, -1);

		// get next cell and pop it from sail container
		if (capt_mode == 's') {
			curr = sea.back();
			sea.pop_back();
		} else {
			curr = sea.front();
			sea.pop_front();
		}

		//print_current_cell(curr, false);
		// explore surrounding cells
		for (int i = 0; i < 4; ++i) {
			switch (order[i]) {
				case 'N':
					if (is_ashore(curr, 'N')) {
						first_mate_do();
					} else {
						if (add_cell(curr.north(), false, 'N')) {
							cout << "return in dir " << order[i] << endl;
							return;
						}
					}
					break;
				case 'E':
					if (is_ashore(curr, 'E')) {
						first_mate_do();
					} else {
						if (add_cell(curr.east(), false, 'E')) {
							cout << "return in dir " << order[i] << endl;
							return;
						}
					}
					break;
				case 'S':
					if (is_ashore(curr, 'S')) {
						first_mate_do();
					} else {
						if (add_cell(curr.south(), false, 'S')) {
							cout << "return in dir " << order[i] << endl;
							return;
						}
					}
					break;
				case 'W':
					if (is_ashore(curr, 'W')) {
						first_mate_do();
					} else {
						if (add_cell(curr.west(), false, 'W')) {
							cout << "return in dir " << order[i] << endl;
							return;
						}
					}
					break;
				default:
					cerr << "Bad direction in " << __func__ << endl;
			} // switch
			if (treasure_found) {
				break;
			}
		} // for
		captain_do();
	} // else
}

// The ashore (sea->land) case should already be handled before
// attempt to call this function. Therefore, this function only
// handles seas->sea and land->land cell exploration.
// Returns true if is_treasure() is true, false othewise.
bool TreasureHunt::add_cell(Cell c, bool on_land, char dir) {
	if (is_valid_cell(c, on_land)) {
		// check if is treasure cell
		if (is_treasure(c, dir)) {
			return true;
		}
		// if on-land and is valid land cell
		else if (on_land) {
			set_cell(c, dir);
			land.push_back(c);
		}
		// if on sea and is valid sea cell
		else if (!on_land) {
			set_cell(c, static_cast<char>(tolower(dir)));
			sea.push_back(c);
		}
		// should never be here
		else {
			cerr << "Bad state in " << __func__ << endl;
		}
	} // outer if
	return false;
}

void TreasureHunt::first_mate_do() {
	if (land.empty()) {
		if (print_verbose) {
			cout << "party returned with no treasure.\n";
		}
		return;
	} else {
		land_loc++;
		Cell curr = Cell(-1, -1);
		if (mate_mode == 's') {
			curr = land.back();
			land.pop_back();
		} else {
			curr = land.front();
			land.pop_front();
		}
		//print_current_cell(curr, true);
		for (int i = 0; i < 4; ++i) {
			switch (order[i]) {
				case 'N':
					if (add_cell(curr.north(), true, 'N')) {
						return;
					}
					break;
				case 'E':
					if (add_cell(curr.east(), true, 'E')) {
						return;
					}
					break;
				case 'S':
					if (add_cell(curr.south(), true, 'S')) {
						return;
					}
					break;
				case 'W':
					if (add_cell(curr.west(), true, 'W')) {
						return;
					}
					break;
			} // switch
		} // for
		first_mate_do();
	} // else
}

void TreasureHunt::calculate_path_length() {
	Cell temp = treasure;
	while (!cell_equal(temp, start)) {
		switch (get_cell(temp)) {
			case 'N': case 'n':
				temp = temp.south();
				break;
			case 'E': case 'e':
				temp = temp.west();
				break;
			case 'S': case 's':
				temp = temp.north();
				break;
			case 'W': case 'w':
				temp = temp.east();
				break;
			default:
				cerr << "Unrecognized cell '" << get_cell(temp) << "' in " <<
					__func__ << endl;
		} // switch
		path_length++;
	} // while
}

// clean up sea and land container after treasure is found
void TreasureHunt::clean_up() {
	while (!land.empty()) {
		land.pop_back();
	}
	while (!sea.empty()) {
		sea.pop_back();
	}
}

void TreasureHunt::print_hunt_stats() {
	cout << "--- STATS ---\n";
	cout << "Starting location: " << start.row << "," << start.col << "\n";
	cout << "Water locations investigated: " << water_loc << "\n";
	cout << "Land locations investigated: " << land_loc << "\n";
	cout << "Went ashore: " << ashore << "\n";
	if (treasure_found) {
		cout << "Path length: " << path_length << "\n";
		cout << "Treasure location: " << treasure.row << "," <<
			treasure.col << "\n";
	}
	cout << "--- STATS ---\n";
}

// Reuse the sea container to hold path
void TreasureHunt::print_path() {
	if (treasure_found) {
		Cell temp = treasure;
		bool flag = true;
		while (!cell_equal(temp, start)) {
			sea.push_back(temp);
			temp = temp.prev(get_cell(temp));
		} // while
		sea.push_back(temp);

		if (show_path == 'M') {

		} else if (show_path == 'L') {
			cout << "Sail:\n";
			while (!sea.empty()) {
				temp = sea.back();
				if (flag && isupper(get_cell(temp))) {
					cout << "Search:\n";
					flag = false;
				}
				temp.output();
				sea.pop_back();
			}
		} else {

		}
		cout << "Treasure found at " << treasure.row << "," << treasure.col <<
			" with path length " << path_length << ".";
	} // treasure_found
}
