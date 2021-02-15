#include "../include/pagerank.hpp"

// From a graph's adjacency matrix, generate its probability matrix A
Eigen::MatrixXf probMatrix(Eigen::MatrixXf &R, float alpha) {

		Eigen::MatrixXf Rt = R.transpose();
		Eigen::MatrixXf Rsum = Rt.colwise().sum();

		// Create probability matrix A
		Eigen::MatrixXf A = Eigen::MatrixXf();
		A.resize(R.rows(), R.cols());

		// Fill A
		for(unsigned j=0; j<Rt.cols(); j++) {

			// Has edges
			if(Rsum(0, j) != 0) {
				for(unsigned i=0; i<A.rows(); i++) {
					A(i, j) =
					alpha * Rt(i, j) / Rsum(0, j) + (1.0 - alpha) / A.cols();
				}
			}

			// No edges
			else {
				A.block(0, j, A.rows(), 1).fill(1.0 / A.cols());
			}
		}

		return A;
}



// Main body of improved Pagerank
Eigen::MatrixXf improvedPageRank(Eigen::MatrixXf &R, float alpha, float epsilon) {

		// Initialization

	// Get probability matrix
	Eigen::MatrixXf A = probMatrix(R, alpha);

	// Initialize x and oldX
	Eigen::MatrixXf x;
	x.resize(A.rows(), 1);
	x.fill(1.0 / x.rows());

	Eigen::MatrixXf oldX;
	oldX.setZero(A.rows(), 1);


		// Iteration
	while( sqrt((x - oldX).squaredNorm()) > epsilon ) {
		oldX = x;
		x = A * x;
	}

	return x;
}
