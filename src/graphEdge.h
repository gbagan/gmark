/*
 * graphEdge.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef GRAPHEDGE_H_
#define GRAPHEDGE_H_

#include <string>
#include "graphNode.h"

namespace std {

class graphEdge {
public:
	graphNode source;
	size_t predicate;
	graphNode target;

	graphEdge(graphNode sourceNode, size_t pred, graphNode targetNode);
	virtual ~graphEdge();

	string toString();
};

} /* namespace std */

#endif /* GRAPHEDGE_H_ */
