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

	this->numberOfOpenInterfaceConnections = new pair<int,int>[1];
	this->numberOfInterfaceConnections = new pair<int,int>[1];
	this->position = new pair<float,float>[1];
}
graphNode::graphNode(int globalId, int localId, int nodeType, int numberOfEdgeTypes) {
	this->id = globalId;
	this->iterationId = localId;
	this->type = nodeType;

	this->numberOfOpenInterfaceConnections = new pair<int,int>[numberOfEdgeTypes];
	this->numberOfInterfaceConnections = new pair<int,int>[numberOfEdgeTypes];
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


int graphNode::getNumberOfInterfaceConnections(int edgeTypeNumber, bool isSource) {
	int initConnections;
	if(isSource) {
		initConnections = this->numberOfInterfaceConnections[edgeTypeNumber].first;
	} else {
		initConnections = this->numberOfInterfaceConnections[edgeTypeNumber].second;
	}
	return initConnections;
}
void graphNode::setNumberOfInterfaceConnections(int index, int number, bool isSource) {
	if(isSource) {
		this->numberOfInterfaceConnections[index].first = number;
	} else {
		this->numberOfInterfaceConnections[index].second = number;
	}
}

void graphNode::decrementOpenInterfaceConnections(int index, bool isSource) {
	if(isSource) {
		this->numberOfOpenInterfaceConnections[index].first--;
	} else {
		this->numberOfOpenInterfaceConnections[index].second--;
	}
}
void graphNode::incrementOpenInterfaceConnectionsByN(int index, int number, bool isSource) {
	if(isSource) {
		this->numberOfOpenInterfaceConnections[index].first += number;
	} else {
		this->numberOfOpenInterfaceConnections[index].second += number;
	}
}

void graphNode::setPosition(int index, double number, bool isSource) {
	if(isSource) {
		this->position[index].first = number;
	} else {
		this->position[index].second = number;
	}
}
float graphNode::getPosition(int index, bool isSource) {
	if(isSource) {
		return this->position[index].first;
	} else {
		return this->position[index].second;
	}
}

} /* namespace std */
