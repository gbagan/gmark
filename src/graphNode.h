/*
 * graphNode.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>

namespace std {

class graphNode {
public:
	string id;
	int iterationId;
	int type;
//	bool is_virtual;

	pair<float, float> position;
	pair<int,int> numberOfInterfaceConnections;
	pair<int,int> numberOfOpenInterfaceConnections;

	int* connections;

	graphNode();
	graphNode(string id, int iterationId, int type, int numberOfEdgeTypes, int maxNumberOfConnections);
	virtual ~graphNode();

	int getNumberOfOpenInterfaceConnections(bool first);
	void setNumberOfOpenInterfaceConnections(int number, bool isSource);

	int getNumberOfInterfaceConnections(bool isSource);
	void setNumberOfInterfaceConnections(int number, bool isSource);

	void decrementOpenInterfaceConnections(bool isSource);
	void incrementOpenInterfaceConnectionsByN(int number, bool isSource);
	void incrementInterfaceConnectionsByN(int number, bool isSource);

	void setPosition(double number, bool isSource);
	float getPosition(bool isSource);

	void setConnection(int connectionIterationId, int value);
	int getConnection(int connectionIterationId);
};

} /* namespace std */

#endif /* GRAPHNODE_H_ */
