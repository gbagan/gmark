/*
 * graphNode.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include <iostream>
#include "graphNode.h"

namespace std {

graphNode::graphNode() {
	this->id = -1;
	this->iterationId = -1;
	this->type = -1;
	this->is_virtual = true;

	this->numberOfOpenInterfaceConnections = new pair<int,int>[1];
}
graphNode::graphNode(int globalId, int localId, int nodeType, bool isVirtual, int numberOfEdgeTypes) {
	this->id = globalId;
	this->iterationId = localId;
	this->type = nodeType;
	this->is_virtual = isVirtual;

	this->numberOfOpenInterfaceConnections = new pair<int,int>[numberOfEdgeTypes];
	this->position = new pair<float,float>[numberOfEdgeTypes];
}

graphNode::~graphNode() {
	// TODO Auto-generated destructor stub
}


int graphNode::getNumberOfOpenInterfaceConnections(int edgeTypeNumber, bool isSource) {
	int openConnections;
	if(isSource) {
		openConnections = numberOfOpenInterfaceConnections[edgeTypeNumber].first;
	} else {
		openConnections = numberOfOpenInterfaceConnections[edgeTypeNumber].second;
	}
	return openConnections;
}


void graphNode::setNumberOfOpenInterfaceConnections(int index, int number, bool isSource) {
	if(isSource) {
		this->numberOfOpenInterfaceConnections[index].first = number;
	} else {
		this->numberOfOpenInterfaceConnections[index].second = number;
	}
}

void graphNode::decrementOpenInterfaceConnections(int index, bool isSource) {
	if(isSource) {
		this->numberOfOpenInterfaceConnections[index].first--;
	} else {
		this->numberOfOpenInterfaceConnections[index].second--;
	}
}

void graphNode::setPosition(int number, int maxNumber, int index, bool isSource) {
//	cout << "MaxNUmber: " << maxNumber << endl;
	float pos = (float) number / (float) maxNumber;
//	cout << pos << endl;
	if(isSource) {
		this->position[index].first = pos;
	} else {
		this->position[index].second = pos;
	}
}

} /* namespace std */
