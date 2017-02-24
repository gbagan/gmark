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
	int id;
	int iterationId;
	int type;

	float position;
	int numberOfInterfaceConnections;
	int numberOfOpenInterfaceConnections;


	graphNode();
	graphNode(int id, int iterationId, int type, int numberOfOpenICs,int numberOfICs, float position);
	virtual ~graphNode();

	int getNumberOfOpenInterfaceConnections();
	void setNumberOfOpenInterfaceConnections(int number);

	int getNumberOfInterfaceConnections();
	void setNumberOfInterfaceConnections(int number);

	void decrementOpenInterfaceConnections();
	void incrementOpenInterfaceConnectionsByN(int number);
	void incrementInterfaceConnectionsByN(int number);

	void setPosition(double number);
	float getPosition();

//	void setConnection(int connectionIterationId, int value);
//	int getConnection(int connectionIterationId);
};

} /* namespace std */

#endif /* GRAPHNODE_H_ */
