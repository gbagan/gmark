/*
 * incrementalDeterministicGraphGenerator.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include "incrementalDeterministicGraphGenerator.h"

namespace std {

incrementalDeterministicGraphGenerator::incrementalDeterministicGraphGenerator(config::config configuration) {
	this->conf = configuration;

}

incrementalDeterministicGraphGenerator::~incrementalDeterministicGraphGenerator() {
	// TODO Auto-generated destructor stub
}

void incrementalDeterministicGraphGenerator::generateIncDetGraph() {
	cout << "Generate a incremental deterministic graph (given a seed)";
}

} /* namespace std */
