/*
 * incrementalDeterministicGraphGenerator.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include <vector>
#include <algorithm>    // std::set_intersection, std::sort
#include "incrementalDeterministicGraphGenerator.h"
#include "graphNode.h"

namespace std {

incrementalDeterministicGraphGenerator::incrementalDeterministicGraphGenerator() {
	randomGenerator.seed(chrono::system_clock::now().time_since_epoch().count());
}

incrementalDeterministicGraphGenerator::~incrementalDeterministicGraphGenerator() {
	// TODO Auto-generated destructor stub
}



void incrementalDeterministicGraphGenerator::calculateSimilarity(graphNode n1, graphNode n2) {
//	cout << "Simialrity between node " << n1.iterationId << " and node " << n2.iterationId << endl;
	vector<int> s1, s2;
	for (edge2 e: edges) {
		if (e.subjectIterationId == n1.iterationId) {
			if (std::find(s1.begin(), s1.end(), e.objectIterationId) == s1.end()) {
				// Set s1 does not contain the new objectId
				s1.push_back(e.objectIterationId);
			}
		}
		if (e.subjectIterationId == n2.iterationId) {
			if (std::find(s2.begin(), s2.end(), e.objectIterationId) == s2.end()) {
				// Set s2 does not contain the new objectId
				s2.push_back(e.objectIterationId);
			}
		}
	}


	vector<int> intersectionVec(max(s1.size(), s2.size()));
	vector<int>::iterator itIntersection;

	sort(s1.begin(), s1.end());
	sort(s2.begin(), s2.end());

	//	cout << "In s1: ";
	//	for (int i:s1) {
	//		cout << i << ",";
	//	}
	//	cout << endl;
	//
	//	cout << "In s2: ";
	//	for (int i:s2) {
	//		cout << i << ",";
	//	}
	//	cout << endl;

	itIntersection = set_intersection(s1.begin(), s1.end(),
			s2.begin(), s2.end(), intersectionVec.begin());

	intersectionVec.resize(itIntersection-intersectionVec.begin());

	int unionCount = s1.size() + s2.size() - intersectionVec.size();

//	cout << "Intersection: " << intersectionVec.size() << endl;
//	cout << "Union: " << unionCount << endl;
	if (unionCount == 0) {
		cout << 0.0 << ",";
	} else {
		cout <<	(float)intersectionVec.size() / (float)unionCount << ",";
	}
}





void incrementalDeterministicGraphGenerator::printRankZipf(vector<graphNode> nodes, int edgeTypeId, int nbNodes) {
	int maxDegree = 0;
	for (graphNode node: nodes) {
		int degree = node.numberOfInterfaceConnections - node.numberOfOpenInterfaceConnections;
		if (degree > maxDegree) {
			maxDegree = degree;
		}
	}

	cout << "Maxdegree=" << maxDegree << endl;
	ofstream rankFile, degreeFile;
	rankFile.open("rankFileET" + to_string(edgeTypeId) + "n" + to_string(nbNodes) + ".txt", ios::trunc);
	degreeFile.open("rankFileET" + to_string(edgeTypeId) + "n" + to_string(nbNodes) + "degree.txt", ios::trunc);
	for (graphNode node: nodes) {
		double rank = ((double)node.numberOfInterfaceConnections - (double)node.numberOfOpenInterfaceConnections) / (double) maxDegree;
		rankFile << to_string(rank) << endl;
		degreeFile << to_string(node.numberOfInterfaceConnections - node.numberOfOpenInterfaceConnections) << "-" << to_string(maxDegree) << endl;
	}
	rankFile.close();
}

void incrementalDeterministicGraphGenerator::printRankNonZipf(vector<graphNode> nodes, int edgeTypeId, int nbNodes) {
	ofstream rankFile;
	rankFile.open("rankFileET" + to_string(edgeTypeId) + "n" + to_string(nbNodes) + ".txt", ios::trunc);
	for (graphNode currentNode: nodes) {
		int nodesWithLowerDegree = 0;
		int degree = currentNode.numberOfInterfaceConnections - currentNode.numberOfOpenInterfaceConnections;
		for (graphNode compareNode: nodes) {
			if (compareNode.numberOfInterfaceConnections - compareNode.numberOfOpenInterfaceConnections <= degree) {
				nodesWithLowerDegree++;
			}
		}
		double rank = (double)nodesWithLowerDegree / (double)nodes.size();
		rankFile << to_string(rank) << endl;
	}


	rankFile.close();
}







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

	edges.push_back(newEdge);
}
// ####### Generate edges #######


// ####### Update interface-connections #######
void incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> *nodesVec, distribution distr, bool outDistr) {
//	cout << "New Zipfian case" << endl;
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
		node.incrementOpenInterfaceConnectionsByN(difference);
		node.setNumberOfInterfaceConnections(newInterfaceConnections);
//		cout << "after openICs: " << node.getNumberOfOpenInterfaceConnections() << endl;
	}
}

void incrementalDeterministicGraphGenerator::performOutDistributionShift(config::edge & edgeType) {
	double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, -1) + outDistrShift;
	double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, -1) + inDistrShift;
	int newMeanOutDistr = ((double) conf.types.at(edgeType.object_type).size * meanInDistr) / (double) conf.types.at(edgeType.subject_type).size;
	int incr = floor(newMeanOutDistr - meanOutDistr);

	if (incr > 0) {
		cout << "Shift out-distrib with: " << incr << endl;
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
	double newMeanInDistr = ((double) conf.types.at(edgeType.subject_type).size * meanOutDistr) / (double) conf.types.at(edgeType.object_type).size;
	int incr = floor(newMeanInDistr - meanInDistr);

	if (incr > 0) {
		cout << "Shift in-distrib with: " << incr << endl;
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
		cout << "####----###### DIT HAD NOOIT MOGEN GEBEUREN!!!! ####----######" << endl;
	} else {
		meanEdgesPerNode = 1;
	}
	return meanEdgesPerNode;
}


void incrementalDeterministicGraphGenerator::fixSchemaInequality(config::edge & edgeType) {
	double evOutDistribution = getMeanICsPerNode(edgeType.outgoing_distrib, conf.types[edgeType.subject_type].size) + outDistrShift;

	double evInDistribution = getMeanICsPerNode(edgeType.incoming_distrib, conf.types[edgeType.object_type].size) + inDistrShift;

	if ((conf.types[edgeType.subject_type].size * evOutDistribution) > (conf.types[edgeType.object_type].size * evInDistribution)) {
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
		cout << "Update out-nodes in dbsh: " << sf << endl;
		for (graphNode & node: nodes.first) {
			node.incrementOpenInterfaceConnectionsByN(sf);
			node.incrementInterfaceConnectionsByN(sf);
		}

		// Increment all ICs and openICs with floor(num) or floor(num)+1 with probability num-floor(num)
		double num = ((double) sf * (double)conf.types[edgeType.subject_type].size) / (double)conf.types[edgeType.object_type].size;
		int numIncrement = floor(num);
		double randomValue = uniformDistr(randomGenerator);
		if (randomValue < num - (double)floor(num)) {
			numIncrement++;
		}
		inDistrShift += numIncrement;
		cout << "Update in-nodes in dbsh: " << sf << endl;
		for (graphNode & node: nodes.second) {
			node.incrementOpenInterfaceConnectionsByN(numIncrement);
			node.incrementInterfaceConnectionsByN(numIncrement);
		}
	} else {
		// Increment all ICs and openICs with 1
		inDistrShift += sf;
		for (graphNode & node: nodes.second) {
			cout << "Update out-nodes in dbsh: " << sf << endl;
			node.incrementOpenInterfaceConnectionsByN(sf);
			node.incrementInterfaceConnectionsByN(sf);
		}

		// Increment all ICs and openICs with floor(num) or floor(num)+1 with probability num-floor(num)
		double num = ((double) sf * (double)conf.types[edgeType.object_type].size) / (double)conf.types[edgeType.subject_type].size;
		int numIncrement = floor(num);
		double randomValue = uniformDistr(randomGenerator);
		if (randomValue < num - (double)floor(num)) {
			numIncrement++;
		}
		outDistrShift += numIncrement;
		for (graphNode & node: nodes.first) {
			cout << "Update in-nodes in dbsh: " << sf << endl;
			node.incrementOpenInterfaceConnectionsByN(numIncrement);
			node.incrementInterfaceConnectionsByN(numIncrement);
		}
	}
}

void incrementalDeterministicGraphGenerator::performFixingShiftForZipfian(config::edge & edgeType, vector<int> & subjectNodeIdVector, vector<int> & objectNodeIdVector) {
	// Perform a shift in one of the distributions, when there is a large difference in the lengths of the vectors
	if (subjectNodeIdVector.size() < objectNodeIdVector.size()) {
		int diff = objectNodeIdVector.size() - subjectNodeIdVector.size();
		int incr = diff / conf.types[edgeType.subject_type].size;
		if (incr > 0) {
			cout << "Shift out-distr after vector gen: " << incr << endl;
			outDistrShift += incr;
			cout << "outDistrShift: " << outDistrShift << endl;
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
		int incr = diff / conf.types[edgeType.object_type].size;
		if (incr > 0) {
			cout << "Shift in-distr after vector gen: " << incr << endl;
			inDistrShift += incr;
			cout << "inDistrShift: " << inDistrShift << endl;
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


		for (int i=0; i<min(subjectNodeIdVector.size(), objectNodeIdVector.size()); i++) {
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
			for (int i=objectNodeIdVectorLastOne.size(); i<subjectNodeIdVectorLastOne.size(); i++) {
				subjectNodeIdVectorMinusOne.push_back(subjectNodeIdVectorLastOne[i]);
			}
		} else {
			for (int i=subjectNodeIdVectorLastOne.size(); i<objectNodeIdVectorLastOne.size(); i++) {
				objectNodeIdVectorMinusOne.push_back(objectNodeIdVectorLastOne[i]);
			}
		}



		// Shuffle large vectors
		shuffle(subjectNodeIdVectorMinusOne.begin(), subjectNodeIdVectorMinusOne.end(), randomGenerator);
		shuffle(objectNodeIdVectorMinusOne.begin(), objectNodeIdVectorMinusOne.end(), randomGenerator);


		// Create edges
		for (int i=0; i<min(subjectNodeIdVectorMinusOne.size(), objectNodeIdVectorMinusOne.size()); i++) {
			addEdge(nodes.first[subjectNodeIdVectorMinusOne[i]], nodes.second[objectNodeIdVectorMinusOne[i]], edgeType.predicate);
		}

		for (int i=0; i<min(subjectNodeIdVectorLastOne.size(), objectNodeIdVectorLastOne.size()); i++) {
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
	nodeGen.addSubjectNodes(edgeType, outDistrShift);
	nodeGen.addObjectNodes(edgeType, inDistrShift);



	// Update the ICs for the Zipfian distribution to satisfy the property that influecer nodes will get more ICs when the graph grows
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
		updateInterfaceConnectionsForZipfianDistributions(&nodes.first, edgeType.outgoing_distrib, true);
	}
	if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		updateInterfaceConnectionsForZipfianDistributions(&nodes.second, edgeType.incoming_distrib, false);
	}

	double prob = 0.25;
	if (edgeType.correlated_with.size() == 0) {
		generateEdges(edgeType, prob);
	} else {
		vector<edge2> possibleEdges = generateCorrelatedEdgeSet(edgeType);
		generateCorrelatedEdges(edgeType, prob, possibleEdges);
	}
}


void incrementalDeterministicGraphGenerator::generateCorrelatedEdges(config::edge & edgeType, double prob, vector<edge2> correlatedEdges) {
	shuffle(correlatedEdges.begin(), correlatedEdges.end(), randomGenerator);

	for (edge2 possibleEdge: correlatedEdges) {
		if (nodes.first[possibleEdge.subjectIterationId].numberOfOpenInterfaceConnections > 0 &&
				nodes.second[possibleEdge.objectIterationId].numberOfOpenInterfaceConnections > 0) {
//			cout << "Add edge: " << possibleEdge.subjectIterationId << " - " << edgeType.predicate << " - " << possibleEdge.objectIterationId << endl;
//			for (int i=0; i<nodes.second[possibleEdge.objectIterationId].numberOfOpenInterfaceConnections; i++) {
			addEdge(nodes.first[possibleEdge.subjectIterationId], nodes.second[possibleEdge.objectIterationId], edgeType.predicate);
//			}
		}
	}
	generateEdges(edgeType, prob);
}

void incrementalDeterministicGraphGenerator::addToMapping(vector<vector<int>> & mapping, int subject, int target) {
	mapping[subject].push_back(target);
}


bool pairCompare(const pair<int, int>& firstElem, const pair<int, int>& secondElem) {
  return firstElem.first > secondElem.first;
}


vector<vector<int>> incrementalDeterministicGraphGenerator::icPreservingMapping(vector<int> subjects, vector<int> objects, int correlatedETid) {
	vector<vector<int>> mapping = vector<vector<int>>(subjects.size());


	// Get the degree of all nodes
	vector<int> inDistr(subjects.size());
	// Initialize outDistr-vector
	for (int i=0; i<subjects.size(); i++) {
		inDistr.push_back(0);
	}

	// Analyze all edges
	string line;
	ifstream myfile("outputgraph" + to_string(graphNumber) + ".txt");
	myfile.clear();
	myfile.seekg(0, ios::beg);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string temp = line;
			string getPred = temp.erase(0, temp.find(" ")+1);
			string predicate = getPred.substr(0, getPred.find(" "));
			if (stoi(predicate) == correlatedETid) {
				temp = line;
				string subjectType = temp.substr(0, temp.find("-"));
				string getSub = temp.erase(0, temp.find("-")+1);
				string subject = getSub.substr(0, getSub.find(" "));

				getPred = getPred.erase(0, getPred.find(" ")+1);

				string objectType = getPred.substr(0, getPred.find("-"));
				string getObj = getPred.erase(0, getPred.find("-")+1);
				string object = getObj.substr(0, getObj.length());

				if (stoi(objectType) == conf.schema.edges[correlatedETid].object_type && stoi(subjectType) == conf.schema.edges[correlatedETid].subject_type) {
					inDistr.at(stoi(object))++;
				}
			}
		}
		myfile.close();
	} else {
		cout << "Unable to open file";
	}


//	for (int i=0; i<subjects.size(); i++) {
//		cout << "Degree of node" << i << ": " <<  inDistr[i] << endl;
//	}

	vector<pair<int,int>> degreeNodeIdPairSubjects(subjects.size());
	for (int i=0; i<subjects.size(); i++) {
		degreeNodeIdPairSubjects[i] = make_pair(inDistr[i], i);
	}
	sort(degreeNodeIdPairSubjects.begin(), degreeNodeIdPairSubjects.end(), pairCompare);

//	for (int i=0; i<degreeNodeIdPairSubjects.size(); i++) {
//		cout << "Degree of node" << degreeNodeIdPairSubjects[i].second << " sorted: " <<  degreeNodeIdPairSubjects[i].first << endl;
//	}

//	cout << "Number of object nodes: " << objects.size() << endl;
//	for (int i=0; i<objects.size(); i++) {
//		cout << "Degree of object node" << i << ": " <<  nodes.second[i].numberOfInterfaceConnections << endl;
//	}
	vector<pair<int,int>> degreeNodeIdPairObjects(objects.size());
	for (int i=0; i<objects.size(); i++) {
		degreeNodeIdPairObjects[i] = make_pair(nodes.second[i].numberOfInterfaceConnections, i);
	}
	sort(degreeNodeIdPairObjects.begin(), degreeNodeIdPairObjects.end(), pairCompare);
//	for (int i=0; i<objects.size(); i++) {
//		cout << "Degree of object node" << degreeNodeIdPairObjects[i].second << " sorted: " <<  degreeNodeIdPairObjects[i].first << endl;
//	}



	for (int i=0; i<min(degreeNodeIdPairSubjects.size(), degreeNodeIdPairObjects.size()); i++) {
//		cout << "Add mapping: " << degreeNodeIdPairSubjects[i].second << " -> " << degreeNodeIdPairObjects[i].second << endl;
		addToMapping(mapping, degreeNodeIdPairSubjects[i].second, degreeNodeIdPairObjects[i].second);
	}

	if (degreeNodeIdPairSubjects.size() > degreeNodeIdPairObjects.size()) {
		for (int i=degreeNodeIdPairObjects.size(); i<degreeNodeIdPairSubjects.size(); i++) {
//			cout << "Add mapping: " << degreeNodeIdPairSubjects[i].second << " -> " << degreeNodeIdPairObjects[i-degreeNodeIdPairObjects.size()].second << endl;
			addToMapping(mapping, degreeNodeIdPairSubjects[i].second, uniformDistr(randomGenerator)*objects.size());
		}
	} else {
		for (int i=degreeNodeIdPairSubjects.size(); i<degreeNodeIdPairObjects.size(); i++) {
//			cout << "Add mapping: " << degreeNodeIdPairSubjects[i-degreeNodeIdPairSubjects.size()].second << " -> " << degreeNodeIdPairObjects[i].second << endl;
			addToMapping(mapping, uniformDistr(randomGenerator)*subjects.size(), degreeNodeIdPairObjects[i].second);
		}
	}

//	for (int i=0; i<mapping.size(); i++) {
//		cout << "Mapping for subject node" << i << ": ";
//		for (int j=0; j<mapping[i].size(); j++) {
//			cout << mapping[i][j] << ",";
//		}
//		cout << endl;
//	}
	return mapping;
}


vector<vector<int>> incrementalDeterministicGraphGenerator::randomMapping(vector<int> subjects, vector<int> objects) {
	vector<vector<int>> mapping = vector<vector<int>>(subjects.size());
	if (subjects.size() > objects.size()) {
		shuffle(subjects.begin(), subjects.end(), randomGenerator);
		for (int i=0; i<objects.size(); i++) {
			addToMapping(mapping, subjects[i], objects[i]);
		}
		for (int i=objects.size(); i<subjects.size(); i++) {
			int randomObject = uniformDistr(randomGenerator) * objects.size();
			addToMapping(mapping, subjects[i], objects[randomObject]);
		}
	} else {
		shuffle(objects.begin(), objects.end(), randomGenerator);
		for (int i=0; i<subjects.size(); i++) {
			addToMapping(mapping, subjects[i], objects[i]);
		}
		for (int i=subjects.size(); i<objects.size(); i++) {
			int randomSubject = uniformDistr(randomGenerator) * subjects.size();
			addToMapping(mapping, subjects[randomSubject], objects[i]);
		}
	}
	return mapping;
}

vector<incrementalDeterministicGraphGenerator::edge2> incrementalDeterministicGraphGenerator::generateCorrelatedEdgeSet(config::edge & edgeType) {
	// Define mapping
	// Get edges that form the basis of the correlation
	// Transform the target nodes of the edges with the mapping-function

	vector<edge2> correlatedEdges;

	for (int edgeTypeId: edgeType.correlated_with) {
		vector<int> subjectsOfMapping;
		int targetNodesBasis = conf.types[conf.schema.edges[edgeTypeId].object_type].size;
		for (int i=0; i<targetNodesBasis; i++) {
			subjectsOfMapping.push_back(i);
		}

		vector<int> objectsOfMapping;
		int nmOfObjects = conf.types[edgeType.object_type].size;
		for (int i=0; i<nmOfObjects; i++) {
			objectsOfMapping.push_back(i);
		}

//		vector<vector<int>> mapping = randomMapping(subjectsOfMapping, objectsOfMapping);
		vector<vector<int>> mapping = icPreservingMapping(subjectsOfMapping, objectsOfMapping, edgeTypeId);

		string line;
		ifstream outputfile("outputgraph" + to_string(graphNumber) + ".txt");
		if (outputfile.is_open()) {
			while (getline(outputfile,line)) {
				string subjectId = line.substr(0, line.find(" "));
				string subject = line.substr(line.find("-")+1, line.find(" "));
				string temp = line.erase(0, line.find(" ")+1);
				int predicate = stoi(temp.substr(0, temp.find(" ")));
//				cout << "Predicate: " << predicate << endl;

				if (predicate == edgeTypeId) {
					int object = stoi(temp.substr(temp.find("-")+1, temp.length()));
					vector<int> mappedObjects = mapping[object];
					for (int mappedObject: mappedObjects) {
//						cout << "target: " << object << endl;
						edge2 possibleEdge;
						possibleEdge.subjectIterationId = stoi(subject);
						possibleEdge.subjectId = subjectId;
						possibleEdge.predicate = to_string(edgeType.edge_type_id);
						possibleEdge.objectIterationId = mappedObject;
						possibleEdge.objectId = to_string(edgeType.object_type) + "-" + to_string(mappedObject);

						correlatedEdges.push_back(possibleEdge);
					}
				}
			}
			outputfile.close();
		}
	}

	return correlatedEdges;

}




int incrementalDeterministicGraphGenerator::processEdgeTypeSingleGraph(config::config configuration, config::edge & edgeType, ofstream & outputFile, int graphNumber_) {
	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

	this->conf = configuration;
	this->graphNumber = graphNumber_;
	cout << "\n\n-----------GraphNumber: " << graphNumber << "--------------" << endl;
//	cout << "Number of nodes: " << conf.nb_nodes << endl;

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


	cout << "OutShif: " << outDistrShift << endl;
	cout << "InShif: " << inDistrShift << endl;
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


	cout << "Node similarities" << endl;
	int countForLineBreak = 0;
	for (graphNode n1: nodes.first) {
		for (graphNode n2: nodes.first) {
			if (n1.iterationId != n2.iterationId) {
				calculateSimilarity(n1, n2);
				countForLineBreak++;
				if (countForLineBreak % 500 == 0) {
					cout << endl;
				}
			}
		}
	}
	cout << endl;

	// Materialize the edge
//	string outputBuffer = "";
//	cout << "Number of edges: " << edges.size() << endl;
	for (int i=0; i<edges.size(); i++) {
		edge2 e = edges[i];
		string edgeString = e.subjectId + " " + e.predicate + " " + e.objectId;

		if (i==edges.size()-1) {
			// Use endl to flush
//			outputBuffer += edgeString;
			outputFile << edgeString << endl;
//			outputBuffer = "";
		} else {
//			outputBuffer += edgeString + "\n";
			outputFile << edgeString << "\n";
		}
	}
	outputFile.flush();
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
	auto durationWithMaterialize = chrono::duration_cast<chrono::milliseconds>( end - start ).count();

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

//	printRankZipf(nodes.first, edgeType.edge_type_id, conf.nb_nodes);
//	printRankNonZipf(nodes.first, edgeType.edge_type_id, conf.nb_nodes);
//	printRank(nodes.second, edgeType.edge_type_id, conf.nb_nodes);


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

	return durationWithMaterialize;
}



} /* namespace std */
