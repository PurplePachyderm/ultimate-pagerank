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



// Re-implementation PageRank but adding steps to fully simulate the evolution
// of cases in the epidemic. Returns array containing infected, healed, and
// vaccinated after each timestep
std::vector<EpidemicState> simulateEpidemic(
	Eigen::MatrixXf &R,
	unsigned timesteps,
	float alpha,
	float delta,
	std::vector<float> v
) {

		// Initialization

	// Get population size and proportion of cured / vaccinated
	int n = R.rows();
	float uncured = 1;	// = cured / infected (1 is initial value only)
	float vaccinated = 0;	// Proportion of vaccinated

	// Get probability matrix
	Eigen::MatrixXf A = probMatrix(R, alpha);

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



int main(void) {

	float alpha = 0.85f;
	int timesteps = 100;

	std::vector<float> v = {};
	for(int i=0; i<timesteps; i++) {
		v.push_back(0);
	}

	// Read base data file
	Eigen::MatrixXf R = parseNeighboursList("facebook_combined.txt");
	removeOrientation(R);


	std::vector<EpidemicState> results = simulateEpidemic(R, timesteps, alpha, 0.02, v);

	for(int i=0; i<timesteps; i++) {
		std::cout << "Results " << i << ":" << std::endl;
		std::cout << "	Cases : " << results[i].infected << std::endl;
	}
	return 0;
}
