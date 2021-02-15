#include "../include/parser.hpp"


// Splits a string into a vector, with a delimiter char
std::vector<std::string> split(std::string str, char delimeter){

	std::stringstream ss(str);
	std::string item;
	std::vector<std::string> splitted;

	while (std::getline(ss, item, delimeter)) {
		splitted.push_back(item);
	}

	return splitted;
}



// Reads a neighbours list and generates the adjacency matrix
Eigen::MatrixXf parseNeighboursList(std::string path) {

	std::ifstream in(path);
	if(!in.is_open()) {
		std::cout << "ERROR: file not found" << std::endl;
		return Eigen::MatrixXf();
	}


	std::string line;

	// Get rows
	std::getline(in, line);
	unsigned dim = std::stoi(line);


	// Initialize Eigen::Array
	Eigen::MatrixXf A;
	A.resize(dim, dim);

	for(unsigned i=0; i<dim; i++) {

		// Get elements vector
		std::getline(in, line);
		std::vector<std::string> stringElements = split(line, ' ');

		unsigned  startId = std::stof(stringElements[0]);

		for(unsigned j=1; j< stringElements.size(); j++) {
			float destId = std::stof(stringElements[j]);
			A(startId, destId) = 1.0f;
		}
	}

	return A;
}



// Reads an edges list and generates the adjacency matrix
Eigen::MatrixXf parseEdgesList(std::string path) {

	std::ifstream in(path);
	if(!in.is_open()) {
		std::cout << "ERROR: file not found" << std::endl;
		return Eigen::MatrixXf();
	}


	std::string line;

	// Get rows
	std::getline(in, line);
	unsigned dim = std::stoi(line);


	// Initialize Eigen::Array
	Eigen::MatrixXf A;
	A.resize(dim, dim);

	while(std::getline(in, line)) {
		std::vector<std::string> stringElements = split(line, ' ');

		float start = std::stof(stringElements[0]);
		float end = std::stof(stringElements[1]);

		A(start, end) = 1.0f;
	}

	return A;
}



// Generates a graph of size n with a probability p of edge spawn,
// and saves it in path (as a neighbours list)
void genNeighboursList(std::string path, unsigned n, float p) {
	srand(time(NULL));

	std::ofstream file;
	file.open (path);

	file << n << std::endl;

	for(unsigned i=0; i<n; i++) {
		// Prevent non-connective graph
		bool isConnected = false;

		for(unsigned j=0; j<n; j++) {
			float rand = (float) std::rand() / RAND_MAX;
			if(rand < p && i != j) {
				file << j << " ";
				isConnected = true;
			}
		}

		// Adding additional edge if node is not connected
		if(!isConnected) {
			// Pick a random edge != i
			unsigned edge = i;
			while(edge != i) {
				edge = std::rand() % static_cast<unsigned>(n + 1);
			}
			file << edge << " ";
		}

		file << std::endl;
	}
}
