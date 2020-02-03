# Pathfinding

A pathfinding project for my Game AI class at Northeastern University.

## Instructions for how to run:
In command line, navigate to the executable. Run it as follows:

`Pathfinding.exe inputfile.json outputfile.json [-s]`

where `inputfile.json` is the file with the map, `outputfile.json` is the file where you wish the output to be sent (it will be created if it does not exist), and `-s` is included if you wish the path to be smoothed.

There are some sample input files in the `tests` directory.

## Explanation
This code uses the A\* search algorithm; it will try to find the shortest path through the map while prioritizing paths that are closer to the goal. It is important to note that it is assumed that the traverser cannot move diagonally, only horizontally or vertically.

The heuristic I used is the distance obtained by the pythagorean theorem from a given tile to the destination (see heuristic method of map.cpp).

This program also implements path smoothing, if the user indicates they want the path to be smoothed. The algorithm will first compute the fastest "regular" path, then smooth it.

For path smoothing, the coordinates of a tile correspond to the bottom left of its tile (i.e. the bottom left of the bottom left tile is 0,0 while its top right is 1,1). Assumptions made about the start and end tile are as follows:
* It is assumed that the traverser starts in the bottom left corner of whichever tile they start in.
* It is assumed that, as long as the traverser reaches any point on the destination tile, they have completed their journey. This means that if the destination is listed as tile [0,0] and the traverser reaches its upper right point [1.0, 1.0], the journey is still considered completed. The alternative, having the bottom left corner always be the destination, would sometimes make the traverser cross its goal, which, in a real game, would look strange in many scenarios.


