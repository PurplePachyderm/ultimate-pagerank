#include <vector>

#include "../include/parser.hpp"
#include "../include/pagerank.hpp"

class EpidemicState {
public:
	float infected;
	float cured;
	float vaccinated;


	EpidemicState(float nInfected, float nCured, float nVaccinated) {
		infected = nInfected;
		cured = nCured;
		vaccinated = nVaccinated;
	}


};



// Modify matrix to make sure that all edges go both way
void removeOrientation(Eigen::MatrixXf &R) {
	for(unsigned j=0; j<R.cols(); j++) {
		for(unsigned i=0; i<R.cols(); i++) {

			// If edge exists, create the opposite one
			if( R(i,j) ) {
				R(j,i) = 1.0f;
			}

		}
	}
}



// From a graph's adjacency matrix, generate its probability matrix A
Eigen::MatrixXf infectionMatrix(Eigen::MatrixXf &R, float alpha, float nu) {

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
					alpha * Rt(i, j) / Rsum(0, j) + nu / A.cols();
				}
			}

			// No edges
			else {
				A.block(0, j, A.rows(), 1).fill(nu / A.cols());
			}
		}

		return A;
}



// Re-implementation PageRank but adding steps to fully simulate the evolution
// of cases in the epidemic. Returns array containing infected, healed, and
// vaccinated after each timestep.


// This version implements random vaccination
std::vector<EpidemicState> simulateRandomVaccination(
	Eigen::MatrixXf &R,
	unsigned timesteps,
	float alpha,
	float nu,
	float delta,
	std::vector<float> v
) {

		// Initialization

	// Get population size and proportion of cured / vaccinated
	int n = R.rows();
	float uncured = 1;	// = cured / infected (1 is initial value only)
	float vaccinated = 0;	// Proportion of vaccinated

	// Get probability matrix
	Eigen::MatrixXf A = infectionMatrix(R, alpha, nu);

	// Initialize x
	Eigen::MatrixXf x;
	x.resize(A.rows(), 1);
	x.fill(1.0 / x.rows());

	// Create vector storing simulation results
	std::vector<EpidemicState> results;
	results.push_back(EpidemicState(1, 0, 0));

	// Store avg infection probabilities
	Eigen::MatrixXf avgPInf = x;

	std::cout << "About to start iterations" << std::endl;

		// Iteration
	for(unsigned k=0; k<timesteps; k++) {

		// Update simulation
		x = A * x;
		x = x / x.sum();

		uncured = uncured * (1.0f - delta);
		vaccinated = vaccinated + v[k];

		// Update avgPInf
		avgPInf = ((float)( k + 1) * avgPInf + x) / (float)(k + 2);

		// Add current epidemic state to results
		results.push_back(EpidemicState(
			((1.0f - (1.0f - avgPInf.array()).pow(k+1)) * uncured * (1 - vaccinated)).sum(),
			(float) n * uncured,
			(float) n * vaccinated
		));

	}

	return results;
}



// This version implements random vaccination
std::vector<EpidemicState> simulateSmartVaccination(
	Eigen::MatrixXf &R,
	unsigned timesteps,
	float alpha,
	float nu,
	float delta,
	std::vector<unsigned> v
) {

		// Initialization

	// Get population size and proportion of cured / vaccinated
	int n = R.rows();
	float uncured = 1;	// = cured / infected (1 is initial value only)
	float vaccinated = 0;	// Proportion of vaccinated

	// Get probability matrix
	Eigen::MatrixXf A = infectionMatrix(R, alpha, nu);

	// Initialize x
	Eigen::MatrixXf x;
	x.resize(A.rows(), 1);
	x.fill(1.0 / x.rows());

	// Create vector storing simulation results
	std::vector<EpidemicState> results;
	results.push_back(EpidemicState(1, 0, 0));

	// Store avg infection probabilities
	Eigen::MatrixXf avgPInf = x;


		// Iteration
	int maxX = 0, maxY = 0;	// Indices for vaccination
	for(unsigned k=0; k<timesteps; k++) {

		// Update simulation
		x = A * x;
		x = x / x.sum();

		uncured = uncured * (1.0f - delta);

		// Update avgPInf
		avgPInf = ((float)( k + 1) * avgPInf + x) / (float)(k + 2);

		// Apply smart vaccination
		for(unsigned i=0; i<v[k]; i++) {
			x.maxCoeff(&maxX, &maxY);
			avgPInf(maxX, maxY) = 0;

			// Also putting column to 0 so infections are impossible in the
			// future
			A.block(0, maxX, A.rows(), maxX);
		}


		// Add current epidemic state to results
		results.push_back(EpidemicState(
			((1.0f - (1.0f - avgPInf.array()).pow(k+1)) * uncured * (1 - vaccinated)).sum(),
			(float) n * uncured,
			(float) n * vaccinated
		));

	}

	return results;
}



int main(void) {

	unsigned timesteps = 100;
	float alpha = 0.65f;
	float nu = 0.85f;
	float delta = 0.02f;

	float vector for random vaccination
	std::vector<float> v = {};
	for(unsigned i=0; i<timesteps; i++) {
		v.push_back(0);
	}

	// Alternative : unsigned vector for smart vaccination
	// std::vector<unsigned> v = {};
	// for(unsigned i=0; i<timesteps; i++) {
	// 	v.push_back(4);
	// }

	// Read base data file
	Eigen::MatrixXf R = parseNeighboursList("facebook_combined.txt");
	removeOrientation(R);


	std::vector<EpidemicState> results = simulateRandomVaccination(
		R, timesteps,
		alpha, nu,
		delta, v
	);

	// Alternative : smart vaccination
	// std::vector<EpidemicState> results = simulateSmartVaccination(
	// 	R, timesteps,
	// 	alpha, nu,
	// 	delta, v
	// );

	for(unsigned i=0; i<timesteps; i++) {
		std::cout << "Results " << i << ":" << std::endl;
		std::cout << "	Cases : " << results[i].infected << std::endl;
	}
	return 0;
}
