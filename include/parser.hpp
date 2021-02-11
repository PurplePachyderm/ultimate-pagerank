#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include<cstdlib>
#include <time.h>

#include <Eigen/Dense>


// Splits a string into a vector, with a delimiter char
std::vector<std::string> split(std::string str, char delimeter);



// Reads a file and generates the adjacency matrix
Eigen::MatrixXf parseFile(std::string path);



// Generates a graph of size n with a probability p of edge spawn,
// and saves it in path
void genGraph(std::string path, unsigned n, float p);
