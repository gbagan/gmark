/*
 * correlation.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: wilcovanleeuwen
 */

#include "incrementalDeterministicGraphGenerator.h"

namespace std {


void incrementalDeterministicGraphGenerator::generateCorrelatedEdges(config::edge & edgeType, double prob, vector<edge2> correlatedEdges) {
	shuffle(correlatedEdges.begin(), correlatedEdges.end(), randomGenerator);

	for (edge2 possibleEdge: correlatedEdges) {
		if (nodes.first[possibleEdge.subjectIterationId].numberOfOpenInterfaceConnections > 0 &&
				nodes.second[possibleEdge.objectIterationId].numberOfOpenInterfaceConnections > 0) {
//			cout << "Add edge: " << possibleEdge.subjectIterationId << " - " << edgeType.predicate << " - " << possibleEdge.objectIterationId << endl;
//			cout << "Correlated edge: " << possibleEdge.subjectIterationId << " to " << possibleEdge.objectIterationId << endl;
			addEdge(nodes.first[possibleEdge.subjectIterationId], nodes.second[possibleEdge.objectIterationId], edgeType.predicate);
		}
	}
	generateEdges(edgeType, prob);
}

void incrementalDeterministicGraphGenerator::addToMapping(int subject, int target) {
	mapping[subject].push_back(target);
}


bool pairCompare(const pair<int, int>& firstElem, const pair<int, int>& secondElem) {
  return firstElem.first > secondElem.first;
}

int incrementalDeterministicGraphGenerator::findEdgeTypeId(int subjectId, int predicate, int objectId) {
	int edgeTypeId = -1;
	for (config::edge edgeType: conf.schema.edges) {
		if ((int)edgeType.subject_type == subjectId &&
				(int)edgeType.predicate == predicate &&
				(int)edgeType.object_type == objectId) {
			edgeTypeId =  edgeType.edge_type_id;
		}
	}
	return edgeTypeId;
}


vector< vector<int> > incrementalDeterministicGraphGenerator::icPreservingMapping(vector<int> subjects, vector<int> objects, int correlatedETid) {
	mapping.resize(max((int)subjects.size(), 1+subjects[subjects.size()-1]));


	// Get the degree of all nodes
	vector<int> inDistr(subjects.size());
	// Initialize outDistr-vector
	for (size_t i=0; i<subjects.size(); i++) {
		inDistr.push_back(0);
	}

	// Analyze all edges
	string line;
	ifstream outputfile("ignore/outputgraph" + to_string(graphNumber) + ".txt");
	outputfile.clear();
	outputfile.seekg(0, ios::beg);
	if (outputfile.is_open()) {
		while (getline(outputfile,line)) {
//			cout << "Line: " << line << endl;
			int subjectId = stoi(line.substr(0, line.find(" ")));
			int subjectType = subjectId % conf.types.size();
//			cout << "SubjectType: " << subjectType << ". SubjectNumber: " << subject << endl;

			string temp = line.erase(0, line.find(" ")+1);
			int predicate = stoi(temp.substr(0, temp.find(" ")));
//			cout << "Predicate: " << predicate << endl;

			temp = temp.erase(0, line.find(" ")+1);
			int objectId  = stoi(temp.substr(0, temp.length()));
			int objectType = objectId % conf.types.size();
			int objectIterationId = objectId / conf.types.size();
//			cout << "ObjectType: " << objectType << ". ObjectNumber: " << object << endl;

			int edgeTypeId = findEdgeTypeId(subjectType, predicate, objectType);
			if (edgeTypeId == correlatedETid) {
//				cout << "Found object: " << objectIterationId << endl;
				inDistr[objectIterationId]++;
			}
		}
		outputfile.close();
	}


//	for (int i=0; i<subjects.size(); i++) {
//		cout << "Degree of node" << i << ": " <<  inDistr[i] << endl;
//	}

	vector< pair<int,int> > degreeNodeIdPairSubjects(subjects.size());
	for (size_t i=0; i<subjects.size(); i++) {
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


	vector< pair<int,int> > degreeNodeIdPairObjects(objects.size());
	for (size_t i=0; i<objects.size(); i++) {
		degreeNodeIdPairObjects[i] = make_pair(nodes.second[i].numberOfInterfaceConnections, i);
	}
	sort(degreeNodeIdPairObjects.begin(), degreeNodeIdPairObjects.end(), pairCompare);
//	for (int i=0; i<objects.size(); i++) {
//		cout << "Degree of object node" << degreeNodeIdPairObjects[i].second << " sorted: " <<  degreeNodeIdPairObjects[i].first << endl;
//	}

//	cout << "OKE:::OKE" << endl;
//	cout << "degreeNodeIdPairSubjects.size(): " << degreeNodeIdPairSubjects.size() << endl;
//	cout << "degreeNodeIdPairObjects.size(): " << degreeNodeIdPairObjects.size() << endl;
	if (degreeNodeIdPairSubjects.size() > degreeNodeIdPairObjects.size()) {
		for (size_t i=0; i<degreeNodeIdPairSubjects.size(); i++) {
//			cout << "i: " << i << endl;
//			cout << "i % degreeNodeIdPairObjects.size(): " << i % degreeNodeIdPairObjects.size() << endl;
//			cout << "degreeNodeIdPairSubjects[i].second: " << degreeNodeIdPairSubjects[i].second << endl;
//			cout << "degreeNodeIdPairObjects[i % degreeNodeIdPairObjects.size()].second: " << degreeNodeIdPairObjects[i % degreeNodeIdPairObjects.size()].second << endl;
			addToMapping(degreeNodeIdPairSubjects[i].second, degreeNodeIdPairObjects[i % degreeNodeIdPairObjects.size()].second);
		}
	} else {
		for (size_t i=0; i<degreeNodeIdPairObjects.size(); i++) {
			addToMapping(degreeNodeIdPairSubjects[i % degreeNodeIdPairSubjects.size()].second, degreeNodeIdPairObjects[i].second);
		}
	}

	return mapping;
}


void incrementalDeterministicGraphGenerator::randomMapping(vector<int> subjects, vector<int> objects) {
	mapping.resize(max((int)subjects.size(), 1+subjects[subjects.size()-1]));

	if (subjects.size() > objects.size()) {
		shuffle(subjects.begin(), subjects.end(), randomGenerator);
		for (size_t i=0; i<subjects.size(); i++) {
			addToMapping(subjects[i], objects[i % objects.size()]);
		}
	} else {
		// Bijective when sizes are equal
		shuffle(objects.begin(), objects.end(), randomGenerator);
		for (size_t i=0; i<objects.size(); i++) {
			addToMapping(subjects[i % subjects.size()], objects[i]);
		}
	}
}





vector<incrementalDeterministicGraphGenerator::edge2> incrementalDeterministicGraphGenerator::generateCorrelatedEdgeSet(config::edge & edgeType) {
	// Define mapping
	// Get edges that form the basis of the correlation
	// Transform the target nodes of the edges with the mapping-function

	vector<edge2> correlatedEdges;


	// Object nodes from the basis of the correlation
	vector<int> subjectsOfMapping;
	size_t minSubject;
	size_t maxSubject;

	// We only focus on the correlation where an edge-type can be correlated with only one other edge-type
	int correlatedBasisEdgeTypeId = edgeType.correlated_with[0];
	int objectType = conf.schema.edges[correlatedBasisEdgeTypeId].object_type;

	if (graphNumber == 0) {
		minSubject = 0;
		maxSubject = conf.types[objectType].size[0];
	} else {
		minSubject = conf.types[objectType].size[graphNumber-1];
		maxSubject = conf.types[objectType].size[graphNumber];
	}
//	cout << "Min subj: " << minSubject << endl;
//	cout << "Max subj: " << maxSubject << endl;
	for (size_t j=minSubject; j<maxSubject; j++) {
		subjectsOfMapping.push_back(j);
	}


	// Object nodes from the correlated edge-type
	int minObject;
	int maxObject;
	if (graphNumber == 0) {
		minObject = 0;
		maxObject = conf.types[edgeType.object_type].size[0];
	} else {
		minObject = conf.types[edgeType.object_type].size[graphNumber-1];
		maxObject = conf.types[edgeType.object_type].size[graphNumber];
	}
//	cout << "Min obj: " << minObject << endl;
//	cout << "Max obj: " << maxObject << endl;

	vector<int> objectsOfMapping;
	for (int i=minObject; i<maxObject; i++) {
		objectsOfMapping.push_back(i);
	}

//	cout << "Subjects of mapping: " << subjectsOfMapping.size() << endl;
//	cout << "Objects of mapping: " << objectsOfMapping.size() << endl;
//	randomMapping(subjectsOfMapping, objectsOfMapping);
	mapping = icPreservingMapping(subjectsOfMapping, objectsOfMapping, edgeType.correlated_with[0]);

	// Print mapping:
//	cout << "After" << endl;
//	for(size_t i=0; i<mapping.size(); i++) {
//		cout << "M(" << i % edgeType.correlated_with.size() << "-" << i / edgeType.correlated_with.size() << ", " << i << "): [";
//		for (size_t j=0; j<mapping[i].size(); j++) {
//			cout << mapping[i][j] << ",";
//		}
//		cout << "]" << endl;
//	}

	string line;
	ifstream outputfile("ignore/outputgraph" + to_string(graphNumber) + ".txt");
	outputfile.clear();
	outputfile.seekg(0, ios::beg);
	if (outputfile.is_open()) {
		while (getline(outputfile,line)) {
//			cout << "Line: " << line << endl;
			int subjectId = stoi(line.substr(0, line.find(" ")));
			int subjectType = subjectId % conf.types.size();
			int subjectIterationId = subjectId / conf.types.size();
//			cout << "SubjectType: " << subjectType << ". SubjectNumber: " << subject << endl;

			string temp = line.erase(0, line.find(" ")+1);
			int predicate = stoi(temp.substr(0, temp.find(" ")));
//			cout << "Predicate: " << predicate << endl;

			temp = temp.erase(0, line.find(" ")+1);
			int objectId  = stoi(temp.substr(0, temp.length()));
			int objectType = objectId % conf.types.size();
			int objectIterationId = objectId / conf.types.size();
//			cout << "ObjectType: " << objectType << ". ObjectNumber: " << object << endl;

			int edgeTypeId = findEdgeTypeId(subjectType, predicate, objectType);
			vector<int>::iterator index;
			index = find(edgeType.correlated_with.begin(), edgeType.correlated_with.end(), edgeTypeId);
			if (index != edgeType.correlated_with.end()) {
//					cout << "Index: " << *index << endl;
//				cout << "Correct edge" << endl;
//				cout << "mappong.size(): " << mapping.size() << endl;
//				cout << "(index-edgeType.correlated_with.begin()): " << (index-edgeType.correlated_with.begin()) << endl;

				vector<int> mappedObjects = mapping[objectIterationId];
//				cout << "Object found: " << objectId << endl;
				for (int mappedObject: mappedObjects) {
//					cout << "Mapped to: " << mappedObject << endl;
					edge2 possibleEdge;
					possibleEdge.subjectIterationId = subjectIterationId;
					possibleEdge.subjectId = subjectId;
					possibleEdge.predicate = to_string(edgeType.edge_type_id);
					possibleEdge.objectIterationId = mappedObject;
					possibleEdge.objectId = mappedObject * conf.types.size() + edgeType.object_type;

//					cout << "Adding edge: " << possibleEdge.subjectId << " " << possibleEdge.predicate << " " <<  possibleEdge.objectId << endl;
					correlatedEdges.push_back(possibleEdge);
				}
			}
		}
		outputfile.close();
	}


	return correlatedEdges;

}

} /* namespace std */
