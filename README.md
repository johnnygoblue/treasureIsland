# EECS 281: Project 1 - Treasure Hunt

## Introduction

It's a Pirate's Life for me, for me, it's a Pirate's Life for me!

It's time to set sail in search for buried treasure. The only problem? There's
no map! But a wily captain and hardy first mate have teamed up for a treasure
hunt.

They've conscripted you to write program to lead their search for riches in and
around a chain of islands. The Captain will pilot the ship over the water, and
the First Mate will lead the search party on land.

### A Chain of Islands

The chain of islands will be specified in a text file, and can be represented as
a 2D-grid size N x N, with N >= 2.

The text file will either be a map or a description of locations, using the
following ascii characters.

* .: Open water
* o: Land
* #: Impassable (land or water)
* @: Starting location (always water)
* $: Treasure location

The grid is a complete representation of all relevant features in the chain, but
there is no path from the start to the treasure given. Using the algorithm
described below and the inputs from the command line and file, you will try to
find and describe the path, if one exists. In either case, the details of the
hunt must be displayed, or you will be forced to walk the plank!

### Example Map

```
# Spec Map #1
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
favor different methods of exploration. For instance, most days the Captain will
navigate "by the seat of their pants", while the First Mate, in charge of
discipline on the ship is often more methodical.

The Captain will travel from island to island with their own log and future
plans, moving only over water locations ('o'). Once put ashore, the First Mate
will search all available land with a separate log and plans before returning to
the ship, moving only over land locations ('.'). Neither will ever accidentally
overlook treasure ('$'), and both are unable to move over impassable areas
('#').

All discovery happens in adjacent squares, in cardinal directions only, North,
South, East, or West. New locations can be discovered by looking one square in
each of these directions, but never diagonally or at a greater distance. Each
square can only be discovered once; get these pirates stuck in an infinite loop,
and you will most surely be keelhauled.

To satisfy the moods of your "employers", you develop two routing schemes, one
that uses a stack to store newly discovered locations, and one that uses a queue
to store newly discovered locations. The Captain and First Mate can choose from
either of these container types, with stack-based routing tending to continue
along a recent heading, eventually finding something, and queue-based routing
finding a direct path every time.

### Algorithm

Use the following algorithm with the container (stack or queue) that suits the
hunter in charge.

1. Add the starting location to the container. (This is '@' for the Captain, or
wherever the First Mate was put ashore.)
2. If the container is empty, part of the hunt has ended. Otherwise, set the
"current location" to the "next" location (front for queue, back for stack) in
the container, and remove it from the container. 
  - If the Captain's container is empty, the entire hunt has ended in failure.
  - If the First Mate's container is empty, the search party has failed and must
return to the ship to continue the hunt.
3. From the current location, any adjacent locations that are not impassable and
have not already been discovered.
  - The Captain will only add water locations. If the Captain discovers land,
the First Mate will be put ashore to start a search party at that location,
using a separate container, and beginning at Step #1. 
  - The First Mate will only add land locations. If the First Mate encounters
water it will be ignored.
4. If any location added to a container is the treasure ('$'), the hunt must end
right away, because a path has been found.
5. Repeat from Step #2.

## Input File

### Map Format

### List Format

## Solution Output

Discussion

### Option: Verbose

### Option: Stats

### Option: Output Map or List

#### Treasure Map

#### Coordinate List

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

## Error Handling and Assumptions

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