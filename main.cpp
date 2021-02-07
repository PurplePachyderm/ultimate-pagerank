#include "include/parser.hpp"

int main(void) {

	Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> A = parseFile("data.txt");

	return 0;
}
