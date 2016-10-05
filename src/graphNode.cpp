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
	this->id = "None";
	this->iterationId = -1;
	this->type = -1;

	pair<int,int> intPair;
	intPair.first = 0;
	intPair.second = 0;
	this->numberOfOpenInterfaceConnections = intPair;
	this->numberOfInterfaceConnections = intPair;
	pair<float,float> floatPair;
	floatPair.first = 0;
	floatPair.second = 0;
	this->position = floatPair;

	this->connections = new int[1];
}
graphNode::graphNode(string globalId, int localId, int nodeType, int numberOfEdgeTypes, int maxNumberOfConnections) {
	this->id = globalId;
	this->iterationId = localId;
	this->type = nodeType;

	pair<int,int> intPair;
	intPair.first = 0;
	intPair.second = 0;
	this->numberOfOpenInterfaceConnections = intPair;
	this->numberOfInterfaceConnections = intPair;
	pair<float,float> floatPair;
	floatPair.first = 0;
	floatPair.second = 0;
	this->position = floatPair;

	this->connections = new int[maxNumberOfConnections];
}

graphNode::~graphNode() {
	// TODO Auto-generated destructor stub
}


int graphNode::getNumberOfOpenInterfaceConnections(bool isSource) {
	int openConnections;
	if(isSource) {
		openConnections = numberOfOpenInterfaceConnections.first;
	} else {
		openConnections = numberOfOpenInterfaceConnections.second;
	}
	return openConnections;
}
void graphNode::setNumberOfOpenInterfaceConnections(int number, bool isSource) {
	if(isSource) {
		this->numberOfOpenInterfaceConnections.first = number;
	} else {
		this->numberOfOpenInterfaceConnections.second = number;
	}
}


int graphNode::getNumberOfInterfaceConnections(bool isSource) {
	int initConnections;
	if(isSource) {
		initConnections = this->numberOfInterfaceConnections.first;
	} else {
		initConnections = this->numberOfInterfaceConnections.second;
	}
	return initConnections;
}
void graphNode::setNumberOfInterfaceConnections(int number, bool isSource) {
	if(isSource) {
		this->numberOfInterfaceConnections.first = number;
	} else {
		this->numberOfInterfaceConnections.second = number;
	}
}

void graphNode::decrementOpenInterfaceConnections(bool isSource) {
	if(isSource) {
		this->numberOfOpenInterfaceConnections.first--;
	} else {
		this->numberOfOpenInterfaceConnections.second--;
	}
}
void graphNode::incrementOpenInterfaceConnectionsByN(int number, bool isSource) {
	if(isSource) {
		this->numberOfOpenInterfaceConnections.first += number;
	} else {
		this->numberOfOpenInterfaceConnections.second += number;
	}
}
void graphNode::incrementInterfaceConnectionsByN(int number, bool isSource) {
	if(isSource) {
		this->numberOfInterfaceConnections.first += number;
	} else {
		this->numberOfInterfaceConnections.second += number;
	}
}

void graphNode::setPosition(double number, bool isSource) {
	if(isSource) {
		this->position.first = number;
	} else {
		this->position.second = number;
	}
}
float graphNode::getPosition(bool isSource) {
	if(isSource) {
		return this->position.first;
	} else {
		return this->position.second;
	}
}

void graphNode::setConnection(int connectionIterationId, int value) {
	this->connections[connectionIterationId] = value;
}
int graphNode::getConnection(int connectionIterationId) {
	return this->connections[connectionIterationId];
}

} /* namespace std */
