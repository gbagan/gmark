/*
 * incrementalDeterministicGraph.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef INCREMENTALDETERMINISTICGRAPH_H_
#define INCREMENTALDETERMINISTICGRAPH_H_

#include <vector>
#include "graphNode.h"
#include "graphEdge.h"

namespace std {

class incrementalDeterministicGraph {
public:
	incrementalDeterministicGraph();
	virtual ~incrementalDeterministicGraph();

	vector<vector<graphNode>> nodes;
	vector<vector<graphEdge>> edges;
};

} /* namespace std */

#endif /* INCREMENTALDETERMINISTICGRAPH_H_ */
