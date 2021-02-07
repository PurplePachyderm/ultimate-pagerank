#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <Eigen/Dense>


// Splits a string into a vector, with a delimiter char
std::vector<std::string> split(std::string str, char delimeter);



// Reads a file and generates the adjacency matrix
Eigen::MatrixXf parseFile(std::string path);
