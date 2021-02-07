#include <math.h>
#include <iostream>

#include <Eigen/Dense>


// From a graph's adjacency matrix, generate its probability matrix A
Eigen::MatrixXf probMatrix(Eigen::MatrixXf &R, float alpha);



// Main body of improved Pagerank
Eigen::MatrixXf improvedPageRank(Eigen::MatrixXf &R, float alpha, float epsilon);
