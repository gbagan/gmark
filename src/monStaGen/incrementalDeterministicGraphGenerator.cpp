/*
 * incrementalDeterministicGraphGenerator.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include <vector>
#include <algorithm>    // std::set_intersection, std::sort
#include <chrono>
#include "incrementalDeterministicGraphGenerator.h"
#include "graphNode.h"

namespace std {

incrementalDeterministicGraphGenerator::incrementalDeterministicGraphGenerator() {
	randomGenerator.seed(chrono::system_clock::now().time_since_epoch().count());
//	randomGenerator.seed(22);
}

incrementalDeterministicGraphGenerator::~incrementalDeterministicGraphGenerator() {
}


void incrementalDeterministicGraphGenerator::addEdge(graphNode &sourceNode, graphNode &targetNode, int predicate) {
	sourceNode.decrementOpenInterfaceConnections();
	targetNode.decrementOpenInterfaceConnections();

	edge2 newEdge;
	newEdge.subjectIterationId = sourceNode.iterationId;
	newEdge.objectIterationId = targetNode.iterationId;
	newEdge.subjectId = sourceNode.id;
	newEdge.predicate = to_string(predicate);
	newEdge.objectId = targetNode.id;
	newEdge.createdInGraph = graphNumber;

	edges.push_back(newEdge);
}



void incrementalDeterministicGraphGenerator::fixSchemaInequality(config::edge & edgeType) {
	double evOutDistribution = getMeanICsPerNode(edgeType.outgoing_distrib, conf.types[edgeType.subject_type].size[graphNumber]) + outDistrShift;

	double evInDistribution = getMeanICsPerNode(edgeType.incoming_distrib, conf.types[edgeType.object_type].size[graphNumber]) + inDistrShift;

	if ((conf.types[edgeType.subject_type].size[graphNumber] * evOutDistribution) > (conf.types[edgeType.object_type].size[graphNumber] * evInDistribution)) {
		performInDistributionShift(edgeType);
	} else {
		performOutDistributionShift(edgeType);
	}
}


vector<int> constructNodesVector(vector<graphNode> & nodes) {
	vector<int> nodesVector;
	for (graphNode & n: nodes) {
//		cout << "node" << n.iterationId << " has " << n.getNumberOfOpenInterfaceConnections() << " openICs" << endl;
		for (int i=0; i<n.getNumberOfOpenInterfaceConnections(); i++) {
			nodesVector.push_back(n.iterationId);
		}
	}
	return nodesVector;
}

vector<int> constructNodesVectorMinusOne(vector<graphNode> & nodes) {
	vector<int> nodesVector;
	for (graphNode & n: nodes) {
//		cout << "node" << n.iterationId << " has " << n.getNumberOfOpenInterfaceConnections() << " openICs" << endl;
		for (int i=0; i<n.getNumberOfOpenInterfaceConnections()-1; i++) {
			nodesVector.push_back(n.iterationId);
		}
	}
	return nodesVector;
}

vector<int> constructNodesVectorLastOne(vector<graphNode> & nodes) {
	vector<int> nodesVector;
	for (graphNode & n: nodes) {
		if(n.getNumberOfOpenInterfaceConnections() > 0) {
			nodesVector.push_back(n.iterationId);
		}
	}
	return nodesVector;
}




void incrementalDeterministicGraphGenerator::generateEdges(config::edge & edgeType, double prob) {
	if (edgeType.scale_factor > 0 ||
			!conf.types[edgeType.subject_type].scalable || !conf.types[edgeType.object_type].scalable ||
			edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN || edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		vector<int> subjectNodeIdVector = constructNodesVector(nodes.first);
		vector<int> objectNodeIdVector = constructNodesVector(nodes.second);

		if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN || edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
			performFixingShiftForZipfian(edgeType, subjectNodeIdVector, objectNodeIdVector);
		}

		shuffle(subjectNodeIdVector.begin(), subjectNodeIdVector.end(), randomGenerator);
		shuffle(objectNodeIdVector.begin(), objectNodeIdVector.end(), randomGenerator);


		for (size_t i=0; i<min(subjectNodeIdVector.size(), objectNodeIdVector.size()); i++) {
			addEdge(nodes.first[subjectNodeIdVector[i]], nodes.second[objectNodeIdVector[i]], edgeType.predicate);
		}
	} else {
		// Create vectors
		vector<int> subjectNodeIdVectorMinusOne = constructNodesVectorMinusOne(nodes.first);
		vector<int> objectNodeIdVectorMinusOne = constructNodesVectorMinusOne(nodes.second);

		vector<int> subjectNodeIdVectorLastOne = constructNodesVectorLastOne(nodes.first);
		vector<int> objectNodeIdVectorLastOne = constructNodesVectorLastOne(nodes.second);

		// Shuffle small vectors
		shuffle(subjectNodeIdVectorLastOne.begin(), subjectNodeIdVectorLastOne.end(), randomGenerator);
		shuffle(objectNodeIdVectorLastOne.begin(), objectNodeIdVectorLastOne.end(), randomGenerator);

		// Add the difference in the lastOne-vector to the minusOne-vector
		if (subjectNodeIdVectorLastOne.size() > objectNodeIdVectorLastOne.size()) {
			for (size_t i=objectNodeIdVectorLastOne.size(); i<subjectNodeIdVectorLastOne.size(); i++) {
				subjectNodeIdVectorMinusOne.push_back(subjectNodeIdVectorLastOne[i]);
			}
		} else {
			for (size_t i=subjectNodeIdVectorLastOne.size(); i<objectNodeIdVectorLastOne.size(); i++) {
				objectNodeIdVectorMinusOne.push_back(objectNodeIdVectorLastOne[i]);
			}
		}



		// Shuffle large vectors
		shuffle(subjectNodeIdVectorMinusOne.begin(), subjectNodeIdVectorMinusOne.end(), randomGenerator);
		shuffle(objectNodeIdVectorMinusOne.begin(), objectNodeIdVectorMinusOne.end(), randomGenerator);


		// Create edges
		for (size_t i=0; i<min(subjectNodeIdVectorMinusOne.size(), objectNodeIdVectorMinusOne.size()); i++) {
			addEdge(nodes.first[subjectNodeIdVectorMinusOne[i]], nodes.second[objectNodeIdVectorMinusOne[i]], edgeType.predicate);
		}

		for (size_t i=0; i<min(subjectNodeIdVectorLastOne.size(), objectNodeIdVectorLastOne.size()); i++) {
			double randomValue = uniformDistr(randomGenerator);
			if (randomValue > prob) {
				addEdge(nodes.first[subjectNodeIdVectorLastOne[i]], nodes.second[objectNodeIdVectorLastOne[i]], edgeType.predicate);
			}
		}
	}
}







void incrementalDeterministicGraphGenerator::incrementGraph(config::edge & edgeType) {
	// Perform the shifting of the distribution as indecated by the user in the schema
	if (graphNumber > 0) {
		performSchemaIndicatedShift(edgeType);
		if ((conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) &&
					 edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN &&
					 edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
			performShiftForNonScalableNodes(edgeType);
		}
	}


	// Add subject and object nodes to the graph
	// Specify the current shift to get the wright amound of ICs
	nodeGen.addSubjectNodes(edgeType, outDistrShift, graphNumber);
	nodeGen.addObjectNodes(edgeType, inDistrShift, graphNumber);



	// Update the ICs for the Zipfian distribution to satisfy the property that influecer nodes will get more ICs when the graph grows
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
		updateInterfaceConnectionsForZipfianDistributions(edgeType.outgoing_distrib, true);
	}
	if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		updateInterfaceConnectionsForZipfianDistributions(edgeType.incoming_distrib, false);
	}


	double prob = 0.25;
	if (edgeType.correlated_with.size() == 0) {
		generateEdges(edgeType, prob);
	} else {
		vector<edge2> possibleEdges = generateCorrelatedEdgeSet(edgeType);
		generateCorrelatedEdges(edgeType, prob, possibleEdges);
	}

}











int incrementalDeterministicGraphGenerator::processEdgeTypeSingleGraph(config::config configuration, config::edge & edgeType,
		ofstream & outputFile, string outputFileName_,
		int graphNumber_, bool printNodeProperties) {
	cout << "Processing graph " << graphNumber_ << " edge type " << edgeType.edge_type_id << endl;

	//	cout << endl << endl;
	this->conf = configuration;
	this->graphNumber = graphNumber_;
	this->outputFileName = outputFileName_;
//	cout << "Number of nodes: " << conf.nb_nodes[graphNumber] << endl;

	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && outDistrShift == 0) {
		outDistrShift = 1;
	}
	if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN && inDistrShift == 0) {
		inDistrShift = 1;
	}

	if (graphNumber == 0 &&
			edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN &&
			edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
			fixSchemaInequality(edgeType);
	}

	nodeGen = nodeGenerator(edgeType, nodes.first.size(), nodes.second.size(), &randomGenerator, &nodes, &conf);


	if (edgeType.correlated_with.size() > 0) {
		cout << "Assume the following edge-type have been generated: " << endl;
		for (int etId: edgeType.correlated_with) {
			cout << " - " << etId << endl;
		}
	}

	incrementGraph(edgeType);


	// Materialize the edge
//	cout << "Number of edges: " << edges.size() << endl;
	for (size_t i=0; i<edges.size(); i++) {
		edge2 e = edges[i];
		outputFile << e.subjectId << " " << e.predicate << " " << e.objectId << " | " << e.createdInGraph <<"\n";
	}
	outputFile.flush();

	if (printNodeProperties) {
		string subjectsNodesFileName = outputFileName + "_subjects_edgeType" + to_string(edgeType.edge_type_id) + "graphNumber" + to_string(graphNumber_) +".txt";
		ofstream subjectNodes;
		subjectNodes.open(subjectsNodesFileName);
		subjectNodes << "Global node id, local node id, degree in the graph\n";
		for (graphNode subject: nodes.first) {
			subjectNodes << subject.id << ", " << subject.iterationId << ", " << subject.numberOfInterfaceConnections-subject.numberOfOpenInterfaceConnections << "\n";
		}
		subjectNodes.flush();


		string objectsNodesFileName = outputFileName + "_objects_edgeType" + to_string(edgeType.edge_type_id) + "graphNumber" + to_string(graphNumber_) +".txt";
		ofstream objectNodes;
		objectNodes.open(objectsNodesFileName);
		objectNodes << "Global node id, local node id, degree in the graph\n";
		for (graphNode object: nodes.second) {
			objectNodes << object.id << ", " << object.iterationId << ", " << object.numberOfInterfaceConnections-object.numberOfOpenInterfaceConnections << "\n";
		}
		objectNodes.flush();
	}
	cout << endl;
	return 0;
}



} /* namespace std */
