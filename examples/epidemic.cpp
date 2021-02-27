#include <vector>
#include <string>
#include<fstream>
#include <sys/stat.h>
#include <cstdlib>
#include <iomanip>

#include "../include/parser.hpp"
#include "../include/pagerank.hpp"


	// Util functiobns

float genProba() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}



unsigned genInteger(unsigned max) {
	return (rand() % static_cast<unsigned>(max + 1));
}



bool isVaccinated(std::vector<unsigned> &vaccinated, unsigned &node) {
	for(unsigned i=0; i<vaccinated.size(); i++) {
		if(vaccinated[i] == node) {
			return 1;
		}
	}
	return 0;
}



	// Store simulation data

void exportResults(std::vector<unsigned> &results, std::string path) {

	std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();

	// Save cases
	freopen(path.c_str(), "w", stdout);

	for(unsigned i=0; i<results.size(); i++) {
		std::cout << (float) results[i] + 0.001f << std::endl;
	}

	std::cout.rdbuf( oldCoutStreamBuf );
}



	// Modify matrix to make sure that all edges go both way

void removeOrientation(Eigen::MatrixXf &A) {
	for(unsigned j=0; j<A.cols(); j++) {
		for(unsigned i=0; i<A.cols(); i++) {

			// If edge exists, create the opposite one
			if( A(i,j) ) {
				A(j,i) = 1.0f;
			}

		}
	}
}



	// Implementation of simulations with both vaccination methods


// Infection functions

void infectNeigbours(
	Eigen::MatrixXf &A, Eigen::ArrayXf &infections,
	std::vector<unsigned> &vaccinated,
	unsigned &j, float nu
) {

	// Check all nodes to find neighbours
	for(unsigned i=0; i<A.rows(); i++) {

		// If we find one..
		if(A(i, j)) {
			// ... try to infect it with a probability nu
			if(genProba() < nu) {
				if(!isVaccinated(vaccinated, i)) {
					// It will be active from this step if i>j !
					// (this should not affect the simulation results too much)
					infections(i) = 1;
				}
			}
		}
	}
}



void infectByJumping(
	Eigen::MatrixXf &A, Eigen::ArrayXf &infections,
	std::vector<unsigned> &vaccinated,
	unsigned &j, float alpha
) {
	// First, make sure that this node is not neighbour with everyone to
	// avoid infinite loop later
	if(A.block(j, 0, 1, A.rows()).sum() == A.rows()) {
		return;
	}

	// Pick a random node until we're not on ourselves or a neighobur
	bool complete = false;

	while(!complete) {
		unsigned node = genInteger(A.rows() - 1);

		// We found a potential target
		if((unsigned) node != j && !A(j, node)) {
			if(genProba() < (1 - alpha)) {
				if(!isVaccinated(vaccinated, node)) {
					// Same remark
					infections(node) = 1;
				}
			}

			complete = true;	// Mark tentative as complete
		}
	}
}



// This is the simulation main loop, vaccinations / infections should be done
// before calling this
std::vector<unsigned> loopSimulation(
	Eigen::MatrixXf &A,
	Eigen::ArrayXf &infections,
	std::vector<unsigned> vaccinated,
	unsigned &kMax,
	float &nu,
	float &alpha,
	float &delta,
	unsigned nInfInit
) {

		// Initialize results array

	std::vector<unsigned> results = {
		nInfInit
	};


		// Start iterations

	for(unsigned k=0; k<kMax; k++) {

		// Iterate over all nodes ...
		for(unsigned j=0; j<infections.rows(); j++) {

			// ... and if infected, try to infect others
			if(infections(j)) {

				// Iterate over all nodes again, check if they are neighbours
				infectNeigbours(A, infections, vaccinated, j, nu);

				// Pick a non neighbour node and try to infect it
				infectByJumping(A, infections, vaccinated, j, alpha);

				// Try to cure that node
				if(genProba() < delta) {
					// We made the choice to make recovered nodes
					// infectable again
					infections(j) = 0;
				}
			}

		}

		results.push_back(infections.sum());
	}

	return results;
}


// This version implements random vaccination (no PageRank)
std::vector<unsigned> simulateRandomVaccination(
	std::string path,
	unsigned kMax,
	float nu,
	float alpha,
	float delta,
	unsigned nInfInit,
	unsigned nVaccInit
) {

	Eigen::MatrixXf A = parseNeighboursList(path);
	removeOrientation(A);


		// Initialize vector of infections

	Eigen::ArrayXf infections;
	infections.setZero(A.rows(), 1);


		// Pick random infected nodes

	std::vector<unsigned> pickedNodes = {};
	while(pickedNodes.size() < nInfInit) {
		unsigned node = genInteger(A.rows());

		// Make sure it has not been picked before
		bool picked = false;
		for(unsigned i=0; i<pickedNodes.size(); i++) {
			if(picked == pickedNodes[i]) {
				picked = true;
				break;
			}
		}

		if(picked) {
			continue;
		}

		// At this point, node has not been added and can be infected
		infections(node) = 1;
		pickedNodes.push_back(node);
	}

	pickedNodes = {};


		// Vaccinate randomly

	std::vector<unsigned> vaccinated = {};
	while(vaccinated.size() < nVaccInit) {
		unsigned node = genInteger(A.rows());

		// Make sure it has not been picked before
		bool picked = false;
		for(unsigned i=0; i<vaccinated.size(); i++) {
			if(picked == vaccinated[i]) {
				picked = true;
				break;
			}
		}

		if(picked) {
			continue;
		}

		// At this point, node has not been added and can be infected
		infections(node) = 0;
		vaccinated.push_back(node);
	}



		// Compute actual core of simulation (independant of vaccination methods)

	return loopSimulation(
		A,
		infections,
		vaccinated,
		kMax,
		nu,
		alpha,
		delta,
		nInfInit
	);
}



// This version implements smart vaccination (w/ PageRank)
std::vector<unsigned> simulateSmartVaccination(
	std::string path,
	unsigned kMax,
	float nu,
	float alpha,
	float delta,
	unsigned nInfInit,
	unsigned nVaccInit
) {

	Eigen::MatrixXf A = parseNeighboursList(path);
	removeOrientation(A);


		// Initialize vector of infections

	Eigen::ArrayXf infections;
	infections.setZero(A.rows(), 1);


		// Pick random infected nodes

	std::vector<unsigned> pickedNodes = {};
	while(pickedNodes.size() < nInfInit) {
		unsigned node = genInteger(A.rows());

		// Make sure it has not been picked before
		bool picked = false;
		for(unsigned i=0; i<pickedNodes.size(); i++) {
			if(picked == pickedNodes[i]) {
				picked = true;
				break;
			}
		}

		if(picked) {
			continue;
		}

		// At this point, node has not been added and can be infected
		infections(node) = 1;
		pickedNodes.push_back(node);
	}

	pickedNodes = {};


		// Vaccinate according to PageRank ranking

	Eigen::MatrixXf x = improvedPageRank(A, alpha, 0.01f);

	// Get the nVaccInit biggest elements of the PageRank vector,
	// and delete (vaccinate) them
	std::vector<unsigned> vaccinated = {};
	int maxIndex;
	for(unsigned i=0; i<nVaccInit; i++) {
		x.col(0).maxCoeff(&maxIndex);

		infections(maxIndex) = 0;
		x(maxIndex) = 0;
		vaccinated.push_back(maxIndex);
	}


		// Compute actual core of simulation (independant of vaccination methods)

	return loopSimulation(
		A,
		infections,
		vaccinated,
		kMax,
		nu,
		alpha,
		delta,
		nInfInit
	);
}


int main(void) {
	srand (time(NULL));
	std::cout << std::setprecision(10);

	unsigned kMax = 100;
	float nu = 0.2;
	float alpha = 0.85;
	float delta = 0.24;
	unsigned nInfInit = 5;
	unsigned nVaccInit = 800;

	std::vector<unsigned> results = simulateRandomVaccination(
		"facebook_combined.txt",
		kMax,
		nu,
		alpha,
		delta,
		nInfInit,
		nVaccInit
	);

	exportResults(results, "epidemic_random.txt");


	// Alternative : smart vaccination (PageRank based)

	// std::vector<unsigned> results = simulateSmartVaccination(
	// 	"facebook_combined.txt",
	// 	kMax,
	// 	nu,
	// 	alpha,
	// 	delta,
	// 	nInfInit,
	// 	nVaccInit
	// );

	// exportResults(results, "epidemic_smart.txt");



	return 0;
}
