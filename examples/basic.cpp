#include "../include/parser.hpp"
#include "../include/pagerank.hpp"

int main(void) {

	float alpha = 0.85f;
	float epsilon = 0.01f;

	// Read base data file
	Eigen::MatrixXf R = parseNeighboursList("data.txt");
	Eigen::MatrixXf x = improvedPageRank(R, alpha, epsilon);

	// Read Facebook data file (must be downloaded)
	// Eigen::MatrixXf R = parseEdgesList("facebook_combined.txt");
	// Eigen::MatrixXf x = improvedPageRank(R, alpha, epsilon);

	// Generate and read a random graph (neighbours list)
	// genNeighboursList("random.txt", 10, 0.2);
	// Eigen::MatrixXf R = parseNeighboursList("random.txt");
	// Eigen::MatrixXf x = improvedPageRank(R, alpha, epsilon);

	std::cout << x << std::endl;

	return 0;
}
