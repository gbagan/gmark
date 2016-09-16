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
	bool isVirtual;

	vector<pair<int,int>> positions;
	vector<pair<int,int>> numberOfOpenInterfaceConnections;

	graphNode();
	virtual ~graphNode();
};

} /* namespace std */

#endif /* GRAPHNODE_H_ */
