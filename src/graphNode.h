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
//	bool is_virtual;

	pair<float, float>* position;
	pair<int,int>* numberOfInterfaceConnections;
	pair<int,int>* numberOfOpenInterfaceConnections;

	int* connections;

	graphNode();
	graphNode(int id, int iterationId, int type, int numberOfEdgeTypes, int maxNumberOfConnections);
	virtual ~graphNode();

	int getNumberOfOpenInterfaceConnections(int edgeTypeNumber, bool first);
	void setNumberOfOpenInterfaceConnections(int index, int number, bool isSource);

	int getNumberOfInterfaceConnections(int edgeTypeNumber, bool isSource);
	void setNumberOfInterfaceConnections(int index, int number, bool isSource);

	void decrementOpenInterfaceConnections(int index, bool isSource);
	void incrementOpenInterfaceConnectionsByN(int index, int number, bool isSource);

	void setPosition(int index, double number, bool isSource);
	float getPosition(int index, bool isSource);

	void setConnection(int connectionIterationId, int value);
	int getConnection(int connectionIterationId);
};

} /* namespace std */

#endif /* GRAPHNODE_H_ */
