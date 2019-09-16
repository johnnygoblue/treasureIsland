#include <vector>
#include <deque>
#include <getopt.h>
#include <string>

using std::vector;
using std::deque;
using std::string;

class Cell {
	public:
		Cell(const int n, const int m) : row(n), col(m) {}
		inline Cell north() {
			return Cell(row-1, col);
		};
		inline Cell east() {
			return Cell(row, col+1);
		};
		inline Cell south() {
			return Cell(row+1, col);
		};
		inline Cell west() {
			return Cell(row, col-1);
		};
		int row;
		int col;
};

class TreasureHunt {

	public:

		void read_data();

		void print_map();

		void print_sea();

		void print_land();

		void print_start();

		void print_treasure();

		void get_options(int argc, char** argv);

		void hunt();

	private:

		bool is_number(const string &);

		inline bool is_land(Cell c);

		inline bool is_in_bound(Cell c);

		bool is_valid_cell(Cell c, bool on_land);

		inline bool is_ashore(Cell c, char dir);

		inline bool is_treasure(Cell c, char dir);

		bool add_cell(Cell c, bool on_land, char dir);

		inline void set_cell(Cell c, const char ch);

		void captain_do();

		void first_mate_do();

		void print_cell(Cell c);

		void clean_up();

		vector<vector<char>> map;

		deque<Cell> sea;

		deque<Cell> land;

		Cell treasure = Cell(-1, -1);

		int map_size = 0;

		//int iter = 0;

		char order[4] = {'N', 'E', 'S', 'W'};

		unsigned short water_length = 0;

		unsigned short land_length = 0;

		char capt_mode = 's';

		char mate_mode = 'q';

		char path_mode = 'M';

		bool print_verbose = false;

		bool print_stats = false;

		bool show_path = false;

};
