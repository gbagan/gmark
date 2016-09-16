/*
 * incrementalDeterministicGraphGenerator.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef INCREMENTALDETERMINISTICGRAPHGENERATOR_H_
#define INCREMENTALDETERMINISTICGRAPHGENERATOR_H_

#include "config.h"
#include "incrementalDeterministicGraph.h"

namespace std {

class incrementalDeterministicGraphGenerator {
private:
	config::config conf;
	incrementalDeterministicGraph graph;
public:
	incrementalDeterministicGraphGenerator(config::config conf);
	virtual ~incrementalDeterministicGraphGenerator();

	void generateIncDetGraph();
};

} /* namespace std */

#endif /* INCREMENTALDETERMINISTICGRAPHGENERATOR_H_ */
