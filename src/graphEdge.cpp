/*
 * graphEdge.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include "graphEdge.h"

namespace std {

graphEdge::graphEdge(graphNode sourceNode, size_t pred, graphNode targetNode) {
	this->source = sourceNode;
	this->target = targetNode;
	this->predicate = pred;
}

graphEdge::~graphEdge() {
	this->source = graphNode();
	this->target = graphNode();
	this->predicate = -1;
}

string graphEdge::toString() {
	string res = "Edge[\n source: node" + to_string(source.iterationId) + "\n perdicate: " + to_string(predicate) + "\n target: node" + to_string(target.iterationId) + "\n]\n";
	return res;
}

} /* namespace std */
