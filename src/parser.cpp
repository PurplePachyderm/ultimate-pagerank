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



// Reads a file and generates the adjacency matrix
Eigen::MatrixXf parseFile(std::string path) {

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

		for(unsigned j=0; j< stringElements.size(); j++) {
			float index = std::stof(stringElements[j]);
			A(i, index) = 1.0f;
		}
	}

	return A;
}
