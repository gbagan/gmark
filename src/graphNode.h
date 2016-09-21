/*
 * graphNode.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>

namespace std {

class graphNode {
public:
	int id;
	int iterationId;
	int type;
	bool is_virtual;

	pair<float, float>* position;
	pair<int,int>* numberOfOpenInterfaceConnections;

	graphNode();
	graphNode(int id, int iterationId, int type, bool isVirtual, int numberOfEdgeTypes);
	virtual ~graphNode();

	int getNumberOfOpenInterfaceConnections(int edgeTypeNumber, bool first);
	void setNumberOfOpenInterfaceConnections(int index, int number, bool isSource);

	void decrementOpenInterfaceConnections(int index, bool isSource);

	void setPosition(int number, int maxNumber, int index, bool isSource);
};

} /* namespace std */

#endif /* GRAPHNODE_H_ */
