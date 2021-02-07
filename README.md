# ultimate-pagerank

![License: Unlicense](https://img.shields.io/badge/license-Unlicense-green)

A simple [Eigen](http://eigen.tuxfamily.org) based implementation of the
improved [PageRank](https://www.wikiwand.com/en/PageRank) algorithm.


## Usage

Build the code using the `make` command. The `main.cpp` file is an example use
that reads a file to generate the adjacency matrix of the graph
and runs PageRank on it.

In the input file, the first line is the size of the graph, and
each other line contains the indices (starting from 0) of a node's neighbours,
separated with a space. See the `data.txt` file for a simple example.
You can also play with this file or create another one.
