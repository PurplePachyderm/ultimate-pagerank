#include "include/parser.hpp"
#include "include/pagerank.hpp"

int main(void) {

	float alpha = 0.85f;
	float epsilon = 0.01f;

	Eigen::MatrixXf R = parseFile("data.txt");
	Eigen::MatrixXf x = improvedPageRank(R, alpha, epsilon);

	std::cout << x << std::endl;

	return 0;
}