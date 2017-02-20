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
	// TODO Auto-generated destructor stub
}









//void incrementalDeterministicGraphGenerator::printRankZipf(vector<graphNode> nodes, int edgeTypeId, int nbNodes) {
//	int maxDegree = 0;
//	for (graphNode node: nodes) {
//		int degree = node.numberOfInterfaceConnections - node.numberOfOpenInterfaceConnections;
//		if (degree > maxDegree) {
//			maxDegree = degree;
//		}
//	}
//
//	cout << "Maxdegree=" << maxDegree << endl;
//	ofstream rankFile, degreeFile;
//	rankFile.open("rankFileET" + to_string(edgeTypeId) + "n" + to_string(nbNodes) + ".txt", ios::trunc);
//	degreeFile.open("rankFileET" + to_string(edgeTypeId) + "n" + to_string(nbNodes) + "degree.txt", ios::trunc);
//	for (graphNode node: nodes) {
//		double rank = ((double)node.numberOfInterfaceConnections - (double)node.numberOfOpenInterfaceConnections) / (double) maxDegree;
//		rankFile << to_string(rank) << endl;
//		degreeFile << to_string(node.numberOfInterfaceConnections - node.numberOfOpenInterfaceConnections) << "-" << to_string(maxDegree) << endl;
//	}
//	rankFile.close();
//}
//
//void incrementalDeterministicGraphGenerator::printRankNonZipf(vector<graphNode> nodes, int edgeTypeId, int nbNodes) {
//	ofstream rankFile;
//	rankFile.open("rankFileET" + to_string(edgeTypeId) + "n" + to_string(nbNodes) + ".txt", ios::trunc);
//	for (graphNode currentNode: nodes) {
//		int nodesWithLowerDegree = 0;
//		int degree = currentNode.numberOfInterfaceConnections - currentNode.numberOfOpenInterfaceConnections;
//		for (graphNode compareNode: nodes) {
//			if (compareNode.numberOfInterfaceConnections - compareNode.numberOfOpenInterfaceConnections <= degree) {
//				nodesWithLowerDegree++;
//			}
//		}
//		double rank = (double)nodesWithLowerDegree / (double)nodes.size();
//		rankFile << to_string(rank) << endl;
//	}
//
//
//	rankFile.close();
//}







// ####### Generate edges #######
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
// ####### Generate edges #######


// ####### Update interface-connections #######
void incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(distribution distr, bool outDistr) {
//	cout << "New Zipfian case" << endl;
	vector<graphNode> *nodesVec;
	if (outDistr) {
		nodesVec = &nodes.first;
	} else {
		nodesVec = &nodes.second;
	}
	int nmNodes = nodesVec->size();

	vector<double> zipfianCdf = cumDistrUtils.zipfCdf(distr.arg2, nmNodes);

	int shift = 0;
	if (outDistr) {
		shift = outDistrShift;
	} else {
		shift = inDistrShift;
	}

	int newInterfaceConnections = 0;
	int difference = 0;
	for (graphNode & node: *nodesVec) {
		newInterfaceConnections = cumDistrUtils.findPositionInCdf(zipfianCdf, node.getPosition()) + shift;

//		cout << "newInterfaceConnections: " << newInterfaceConnections << endl;
		difference = newInterfaceConnections - node.getNumberOfInterfaceConnections();
		if (difference > 0) {
			node.incrementOpenInterfaceConnectionsByN(difference);
			node.setNumberOfInterfaceConnections(newInterfaceConnections);
		}
//		cout << "after openICs: " << node.getNumberOfOpenInterfaceConnections() << endl;
	}
}

void incrementalDeterministicGraphGenerator::performOutDistributionShift(config::edge & edgeType) {
	double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, -1) + outDistrShift;
	double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, -1) + inDistrShift;
	int newMeanOutDistr = ((double) conf.types.at(edgeType.object_type).size[graphNumber] * meanInDistr) / (double) conf.types.at(edgeType.subject_type).size[graphNumber];
	int incr = floor(newMeanOutDistr - meanOutDistr);

	if (incr > 0) {
		cout << "Conflicting schema requirements identified. The mean of the out-degree distribution will be set to " << newMeanOutDistr << endl;
		cout << "This is done by a shift of " << incr << " value to the right in the out-degree distribution\n" << endl;
		outDistrShift += incr;
		for (graphNode & n: nodes.first) {
			n.incrementInterfaceConnectionsByN(incr);
			n.incrementOpenInterfaceConnectionsByN(incr);
		}
	}
}

void incrementalDeterministicGraphGenerator::performInDistributionShift(config::edge & edgeType) {
	double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, -1) + outDistrShift;
	double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, -1) + inDistrShift;
	double newMeanInDistr = ((double) conf.types.at(edgeType.subject_type).size[graphNumber] * meanOutDistr) / (double) conf.types.at(edgeType.object_type).size[graphNumber];
	int incr = floor(newMeanInDistr - meanInDistr);

	if (incr > 0) {
		cout << "Conflicting schema requirements identified. The mean of the in-degree distribution will be set to " << newMeanInDistr << endl;
		cout << "This is done by a shift of " << incr << " value to the right in the in-degree distribution\n" << endl;
		inDistrShift += incr;
		for (graphNode & n: nodes.second) {
			n.incrementInterfaceConnectionsByN(incr);
			n.incrementOpenInterfaceConnectionsByN(incr);
		}
	}
}

void incrementalDeterministicGraphGenerator::performShiftForNonScalableNodes(config::edge & edgeType) {
	// Both distribution are non-Zipfian

	if (!conf.types.at(edgeType.subject_type).scalable) {
		performOutDistributionShift(edgeType);
	} else {
		performInDistributionShift(edgeType);
	}
}
// ####### Update interface-connections #######

double incrementalDeterministicGraphGenerator::getMeanICsPerNode(distribution & distr, int zipfMax) {
	double meanEdgesPerNode;
	if (distr.type == DISTRIBUTION::NORMAL) {
		meanEdgesPerNode = distr.arg1;
	} else if (distr.type == DISTRIBUTION::UNIFORM) {
		meanEdgesPerNode = (distr.arg2 + distr.arg1) / 2.0;
	} else if (distr.type == DISTRIBUTION::ZIPFIAN) {
		double temp = 0.0;
		for (int i=1; i<=zipfMax; i++) {
			temp += (i-1)*pow((i), -1*distr.arg2);
		}
		meanEdgesPerNode = temp;
		cout << "Zipfian mean=" << meanEdgesPerNode << endl;
		cout << "####----###### SHOULD NEVER HAPPEN!!!! ####----######" << endl;
	} else {
		meanEdgesPerNode = 1;
	}
	return meanEdgesPerNode;
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

void incrementalDeterministicGraphGenerator::performSchemaIndicatedShift(config::edge & edgeType) {
	int sf = edgeType.scale_factor;
	if (sf <= 0) {
		return;
	}

	if (conf.types[edgeType.subject_type].size > conf.types[edgeType.object_type].size) {
		// Increment all ICs and openICs with 1
		outDistrShift += sf;
		cout << "Perform schema-indicated shift: out-degree distribution " << sf << endl;
		for (graphNode & node: nodes.first) {
			node.incrementOpenInterfaceConnectionsByN(sf);
			node.incrementInterfaceConnectionsByN(sf);
		}

		// Increment all ICs and openICs with floor(num) or floor(num)+1 with probability num-floor(num)
		double num = ((double) sf * (double)conf.types[edgeType.subject_type].size[graphNumber]) / (double)conf.types[edgeType.object_type].size[graphNumber];
		int numIncrement = floor(num);
		double randomValue = uniformDistr(randomGenerator);
		if (randomValue < num - (double)floor(num)) {
			numIncrement++;
		}
		inDistrShift += numIncrement;
		cout << "Perform schema-indicated shift: in-degree distribution " << sf << "\n" << endl;
		for (graphNode & node: nodes.second) {
			node.incrementOpenInterfaceConnectionsByN(numIncrement);
			node.incrementInterfaceConnectionsByN(numIncrement);
		}
	} else {
		// Increment all ICs and openICs with 1
		inDistrShift += sf;
		for (graphNode & node: nodes.second) {
			cout << "Perform schema-indicated shift: in-degree distribution " << sf << endl;
			node.incrementOpenInterfaceConnectionsByN(sf);
			node.incrementInterfaceConnectionsByN(sf);
		}

		// Increment all ICs and openICs with floor(num) or floor(num)+1 with probability num-floor(num)
		double num = ((double) sf * (double)conf.types[edgeType.object_type].size[graphNumber]) / (double)conf.types[edgeType.subject_type].size[graphNumber];
		int numIncrement = floor(num);
		double randomValue = uniformDistr(randomGenerator);
		if (randomValue < num - (double)floor(num)) {
			numIncrement++;
		}
		outDistrShift += numIncrement;
		for (graphNode & node: nodes.first) {
			cout << "Perform schema-indicated shift: out-degree distribution " << sf << "\n" <<  endl;
			node.incrementOpenInterfaceConnectionsByN(numIncrement);
			node.incrementInterfaceConnectionsByN(numIncrement);
		}
	}
}

void incrementalDeterministicGraphGenerator::performFixingShiftForZipfian(config::edge & edgeType, vector<int> & subjectNodeIdVector, vector<int> & objectNodeIdVector) {
	// Perform a shift in one of the distributions, when there is a large difference in the lengths of the vectors
	if (subjectNodeIdVector.size() < objectNodeIdVector.size()) {
		int diff = objectNodeIdVector.size() - subjectNodeIdVector.size();
		int incr = diff / conf.types[edgeType.subject_type].size[graphNumber];
		if (incr > 0) {
			cout << "Update schema requirements in the presence of a Zipfian distribution in graphNumber " << graphNumber << endl;
			cout << "Shift out-distr with " << incr << " values to the right" << endl;
			outDistrShift += incr;
			cout << "Total outDistrShift: " << outDistrShift << endl << endl;
			for (graphNode & n: nodes.first) {
				n.incrementInterfaceConnectionsByN(incr);
				n.incrementOpenInterfaceConnectionsByN(incr);
				for (int i=0; i<incr; i++) {
					subjectNodeIdVector.push_back(n.iterationId);
				}
			}
		}
	} else {
		int diff = subjectNodeIdVector.size() - objectNodeIdVector.size();
		int incr = diff / conf.types[edgeType.object_type].size[graphNumber];
		if (incr > 0) {
			cout << "Update schema requirements in the presence of a Zipfian distribution in graphNumber " << graphNumber << endl;
			cout << "Shift in-distr with " << incr << " values to the right" << endl;
			inDistrShift += incr;
			cout << "Total inDistrShift: " << inDistrShift << endl << endl;
			for (graphNode & n: nodes.second) {
				n.incrementInterfaceConnectionsByN(incr);
				n.incrementOpenInterfaceConnectionsByN(incr);
				for (int i=0; i<incr; i++) {
					objectNodeIdVector.push_back(n.iterationId);
				}
			}
		}
	}
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











int incrementalDeterministicGraphGenerator::processEdgeTypeSingleGraph(config::config configuration, config::edge & edgeType, ofstream & outputFile, int graphNumber_, bool printNodeProperties) {
	cout << "Processing graph " << graphNumber_ << " edge type " << edgeType.edge_type_id << endl;

	//	cout << endl << endl;
	this->conf = configuration;
	this->graphNumber = graphNumber_;
//	cout << "\n\n-----------GraphNumber: " << graphNumber << ". Edge-type: " << edgeType.edge_type_id << "--------------" << endl;
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

//	cout << "OutShif: " << outDistrShift << endl;
//	cout << "InShif: " << inDistrShift << endl;
//	int nbEdgesBeforeIncrementing = edges.size();


	if (edgeType.correlated_with.size() > 0) {
		cout << "Assume the following edge-type have been generated: " << endl;
		for (int etId: edgeType.correlated_with) {
			cout << " - " << etId << endl;
		}
	}

	incrementGraph(edgeType);

//	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
//	auto durationWitoutMaterialize = chrono::duration_cast<chrono::milliseconds>( end - start ).count();



//	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	// Materialize the edge
//	string outputBuffer = "";
//	cout << "Number of edges: " << edges.size() << endl;
	for (size_t i=0; i<edges.size(); i++) {
		edge2 e = edges[i];
		outputFile << e.subjectId << " " << e.predicate << " " << e.objectId << " | " << e.createdInGraph <<"\n";
	}
	outputFile.flush();

	if (printNodeProperties) {
		string subjectsNodesFileName = "ignore/subjects_edgeType" + to_string(edgeType.edge_type_id) + "graphNumber" + to_string(graphNumber_) +".txt";
		ofstream subjectNodes;
		subjectNodes.open(subjectsNodesFileName);
		subjectNodes << "Global node id, local node id, degree in the graph\n";
		for (graphNode subject: nodes.first) {
			subjectNodes << subject.id << ", " << subject.iterationId << ", " << subject.numberOfInterfaceConnections-subject.numberOfOpenInterfaceConnections << "\n";
		}
		subjectNodes.flush();


		string objectsNodesFileName = "ignore/objects_edgeType" + to_string(edgeType.edge_type_id) + "graphNumber" + to_string(graphNumber_) +".txt";
		ofstream objectNodes;
		objectNodes.open(objectsNodesFileName);
		objectNodes << "Global node id, local node id, degree in the graph\n";
		for (graphNode object: nodes.second) {
			objectNodes << object.id << ", " << object.iterationId << ", " << object.numberOfInterfaceConnections-object.numberOfOpenInterfaceConnections << "\n";
		}
		objectNodes.flush();
	}

//	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
//	auto materializeation = chrono::duration_cast<chrono::milliseconds>( end - start ).count();
//	cout << "Materialization: " << materializeation << endl;

//	// Number of nodes analysis
//	int subjectNodes = 0;
//	for (graphNode n: nodes.first) {
//		if (n.numberOfInterfaceConnections != n.numberOfOpenInterfaceConnections) {
//			subjectNodes++;
//		}
//	}
//	int objectNodes = 0;
//	for (graphNode n: nodes.second) {
//		if (n.numberOfInterfaceConnections != n.numberOfOpenInterfaceConnections) {
//			objectNodes++;
//		}
//	}
//
//	cout << "Subject nodes: " << subjectNodes << endl;
//	cout << "Object nodes: " << objectNodes << endl;

//	printRankZipf(nodes.first, edgeType.edge_type_id, conf.nb_nodes[graphNumber]);
//	printRankNonZipf(nodes.first, edgeType.edge_type_id, conf.nb_nodes[graphNumber]);
//	printRank(nodes.second, edgeType.edge_type_id, conf.nb_nodes[graphNumber]);


	// Connectiveness Analysis
//	int newnew = 0;
//	int newOld = 0;
//	int oldold = 0;
//	for (int i=nbEdgesBeforeIncrementing-1; i<edges.size(); i++) {
//		string subjectId = edges[i].subjectId;
//		string subjectIdLocalId = subjectId.substr(subjectId.find("-")+1, subjectId.length());
//		int subjectIdInt = stoi(subjectIdLocalId);
////		cout << "subjectIdInt: " << subjectIdInt << endl;
//
//		string objectId = edges[i].objectId;
//		string objectIdLocalId = objectId.substr(objectId.find("-")+1, objectId.length());
//		int objectIdInt = stoi(objectIdLocalId);
////		cout << "objectIdInt: " << objectIdInt << endl;
//
//		if (subjectIdInt >= 5000 && objectIdInt >= 5000) {
//			newnew++;
//		} else if (subjectIdInt < 5000 && objectIdInt < 5000) {
//			oldold++;
////			cout << "subjectIdInt: " << subjectIdInt << endl;
////			cout << "objectIdInt: " << objectIdInt << endl;
//		} else {
//			newOld++;
//		}
//	}
//	cout << "NewNew: " << newnew << endl;
//	cout << "OldOld: " << oldold << endl;
//	cout << "NewOld: " << newOld << endl;
//	ofstream newnewF, oldoldF, newOldF;
//	newnewF.open("newnew.txt", ios::app);
//	oldoldF.open("oldold.txt", ios::app);
//	newOldF.open("newold.txt", ios::app);
//	if (graphNumber > 0) {
//		newnewF << newnew << ",";
//		oldoldF << oldold << ",";
//		newOldF << newOld << ",";
//	}

//	vector<int> connectedToOldGraph;
//	for (int i=nbEdgesBeforeIncrementing-1; i<edges.size(); i++) {
//		connectedToOldGraph.push_back(0);
//	}
//	for (int i=nbEdgesBeforeIncrementing-1; i<edges.size(); i++) {
//		string subjectId = edges[i].subjectId;
//		string subjectIdLocalId = subjectId.substr(subjectId.find("-")+1, subjectId.length());
//		int subjectIdInt = stoi(subjectIdLocalId);
////		cout << "subjectIdInt: " << subjectIdInt << endl;
//
//		string objectId = edges[i].objectId;
//		string objectIdLocalId = objectId.substr(objectId.find("-")+1, objectId.length());
//		int objectIdInt = stoi(objectIdLocalId);
////		cout << "objectIdInt: " << objectIdInt << endl;
//
//		if (subjectIdInt >= 500 && objectIdInt < 500) {
//			connectedToOldGraph[subjectIdInt-500]++;
//		} else if (subjectIdInt < 500 && objectIdInt >= 500) {
//			connectedToOldGraph[objectIdInt-500]++;
//		}
//	}
//
//	int nodesConnected = 0;
//	for(int c: connectedToOldGraph) {
//		if (c>0) {
//			nodesConnected++;
//		}
//	}
//	ofstream nConnected;
//	nConnected.open("nConnected.txt", ios::app);
//	if (graphNumber > 0) {
//		nConnected << nodesConnected << ",";
//	}

	cout << endl;
	return 0;
}



} /* namespace std */
