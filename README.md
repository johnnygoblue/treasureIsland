# EECS 281: Project 1 - Treasure Hunt

## Introduction

It's a Pirate's Life for me, for me, it's a Pirate's Life for me!

It's time to set sail in search for buried treasure. The only problem? There's
no map! But a wily captain and hardy first mate have teamed up for a treasure
hunt. They've conscripted you to write a program to lead their search for riches
in and around a chain of islands. The Captain will pilot the ship over the
water, and the First Mate will lead the search party on land.

### A Chain of Islands

The chain of islands will be specified in a text file, and can be represented
as a 2D-grid size N x N, with N >= 2. The text file will either be a map or a
list of grid coordinates and terrains. The following ascii characters will be
used to describe terrain.

* .: Open water
* o: Land
* #: Impassable (land or water)
* @: Starting location (always water)
* $: Treasure location

The grid is a complete representation of all relevant features in the chain but
it does not include a path from the start to the treasure. Using the algorithm
described below and input from the command line and a file, you will try to
find and describe the path, if one exists. In either case, the details of the
hunt must be displayed, or you will be forced to walk the plank!

### Example Map

```
# Spec Map #1
# Two small islands with treasure in the NW corner and start in the SE corner.
M
5
$o...
oo...
#..o.
..oo.
....@
```

## Routing Scheme

The Captain and the First Mate don't always see eye-to-eye, and will sometimes
favor different methods of exploration. Most days, the Captain will navigate
"by the seat of their pants", while the First Mate, in charge of discipline on
the ship is usually more methodical.

The Captain will travel from island to island with their own log and future
plans, moving only over water locations ('o'). As soon as land is discovered
while sailing, a search party is put ashore before any other discovery is
attempted. Once put ashore to lead a search party, the First Mate will search
all available land with a separate log and plans before returning to the ship,
moving only over land locations ('.'). Any water (inland or open ocean)
encountered by the search party is completely ignored.  Neither the Captain nor
the First Mate will ever accidentally overlook treasure ('$'), and both are
unable to move over impassable terrain ('#').

All discovery happens in adjacent squares using cardinal directions only: North,
South, East, or West. New locations can be discovered by looking one square in
each of these directions, but never diagonally or at a greater distance. Each
square can only be discovered once; get these pirates stuck in an infinite loop,
and you will most surely be keelhauled.

To satisfy the moods of your "employers", you develop two routing schemes, one
that uses a stack and one that uses a queue to store newly discovered locations.
The Captain and First Mate can choose from either of these container types.
Stack-based routing tending to continue along a recent heading, eventually
covering the search area, while queue-based routing covers the search area in
ever-increasing radii.

### The Search Pattern

When discovering new locations use the Search Pattern to determine the order
of investigation. By default the search pattern is North, East, South, West. Any
expedition can choose another search pattern at the command line.

### The Hunt Algorithm

The treasure hunt will be one large hunt (over water) with zero or more smaller
hunts (over land). Use the following hunt algorithm with the container (stack or
queue) that suits the hunter in charge.

1. Add the starting location to the container. 
  - For the Captain, this is '@' on the original map
  - For the First Mate, this is wherever the search party was put ashore
2. If the container is empty, part of the hunt has ended. If not, set the
"current location" to the "next" available location in the container (front for
queue, back for stack) and remove it from the container. 
  - For the Captain, an empty container means the large hunt has ended in
    failure; jump to Step #6
  - For the First Mate, an empty container means a small hunt has ended and the
    search party has failed and must return to the ship; jump to Step #3 in the
    Captain's hunt
3. From the current location, add any adjacent locations that are not impassable
and have not already been discovered. Discover new locations in the order of the
Search Pattern.
  - The Captain will only add water locations. If the Captain discovers land,
    the First Mate will be immediately put ashore, before examining other
    adjacent locations, to start a search party at that location using a
    separate container; jump to Step #1. 
  - The First Mate will only add land locations. If the First Mate encounters
    water it will be ignored regardless of whether it has already been
    discovered or not.
4. If any location added to a container is the treasure ('$'), end the hunt
right away, because the existence of a path has been found; jump to Step #6
5. Repeat from Step #2
6. Report the outcome of the hunt (see Output Format)

## Command Line Interfaces (CLI)

Discussion

## Input File

The input file representing the grid to be searched will be provided in one of
two ways: a 2D ASCII map, or a list of coordinate/terrain triples. Both files
will have similar front matter:

1. Zero or more lines of comments, each of which...
  - have an octothorpe (#) in column zero
  - can contain any combination of zero or more printable ascii characters
  - end with a newline character
2. A filetype specifier, which is...
  - a single ASCII character
  - either M or L (for map or list files)
  - followed by a newline character
3. A map size value, which...
  - is a positive integer, >= 2
  - represents both the width and the height of the map
  - is followed by a newline character

Any starting location is to be considered a water square.

### Map Format

After the front matter, if the first non-comment byte is 'M', the file is in map
format. There will be N rows of N characters (plus a newline), where N is the
map size value that is on the line immediately following the filetype specifier.
Each of the N characters must be from the Terrain Legend above.

See the Example Map above for a sample of a map format file.

### List Format

After the front matter, if the first non-comment byte is 'L', the file is in
list format. There will be zero or more lines that are either comments, blank,
or a coordinate/terrain triple (CTT). When reading list format files, ignore any
blank lines or comments that occur after the front matter. A CTT is two non-
negative integers followed by a single character to represent the terrain at
that location (refer to the Terrain Legend above). These three values will be
separated by commas followed by a newline character, with no other whitespace
characters on the line.

```
<row>,<col>,<terrain>
```

List format files may specify a subset of all CTT in a map. If fewer than NxN
CTT are provided, missing locations are assumed to be water (.). No location
will be specified more once in a list format file, so no more than NxN locations
can be included. The order of CTT are unspecified.

An example of one list format file of the example map above follows.

### Example List

```
# Spec Map #1
# Two small islands with treasure in the NW corner and start in the SE corner.
L
5
0,0,$
4,4,@
2,0,#
0,1,o
1,0,o
1,1,o
2,2,.
2,3,o
3,2,o
3,3,o
```

## The Command Line Interface (CLI)

Discussion

### Supported Options

* --captain <"queue"|"stack">, -c <"queue"|"stack">: The route-finding container
used while searching in the water. Default: stack
* --first-mate <"queue"|"stack">, -f <"queue"|"stack">: The route-finding
container used while searching on land. Default: queue
* --help, -h: Print a useful help message and exit.
* --output <M|L>, -o <M|L>: Display a treasure map or list of locations along
the route.
* --search-pattern <order>, -p <order>: The order of exploration around the
current location. Default: nesw
* --stats, -s: Display statistics after search is complete.
* --verbose, -v: Print verbose output while searching.

## Output Format

Discussion

### Option: Verbose

### Option: Stats

### Option: Output Map or List

#### Treasure Map

#### Coordinate List

## Error Handling and Assumptions
, if any other
characters are read in, the file should be considered unreadable and a prompt
termination with a non-zero exit code should result.

Discussion

### Possible Errors

## Submission to the Autograder

## Grading

### Simulation

### Memory Leaks

### Testing

## Hints and Advice

## Libraries and Restrictions

--------

## Appendix A: An Example

## Appendix B: Tips

## Appendix C: Test Case Legend
