/*
 * graphEdge.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef GRAPHEDGE_H_
#define GRAPHEDGE_H_

#include "graphNode.h"

namespace std {

class graphEdge {
public:
	graphNode source;
	int predicate;
	graphNode target;

	graphEdge();
	virtual ~graphEdge();
};

} /* namespace std */

#endif /* GRAPHEDGE_H_ */
