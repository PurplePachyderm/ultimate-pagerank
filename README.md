# ultimate-pagerank

![License: Unlicense](https://img.shields.io/badge/license-Unlicense-green)

A simple [Eigen](http://eigen.tuxfamily.org) based implementation of the
improved [PageRank](https://www.wikiwand.com/en/PageRank) algorithm.


## Usage

Build the code using the `make` command. The `main.cpp` file is an example
that can read a few different files containing a graph, and generate the
adjacency matrix to run PageRank on it.

## Readable file formats

In all file formats, nodes indices must be separated by spaces

### Neighbours list

In this format, the first line is the size of the graph.
Other lines are composed of several indices : the first one is the base
node, and the other ones are the destination of edges departing from that
base node.

See the `data.txt` for an example. Alternatively, random files can be generated
in this format.

### Edges list
In this format, the first line is the size of the graph.
All other lines are composed of 2 indices, corresponding to the start and
destination of each edge.

The `get-edges-list.sh` script downloads and modifies a file from
[this](http://snap.stanford.edu/data/ego-Facebook.html) address, which is
a sample of Facebook social circles.
