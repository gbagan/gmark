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
//	cout << "Add to mapping: " << subject << " -> " << target << endl;
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

vector<int> incrementalDeterministicGraphGenerator::sortOnDegree(vector<int> objectsIterationIds) {
	vector<pair<int,int> > degreeIteratioIdPairs;
	for (int i: objectsIterationIds) {
		int degree = nodes.second[i].numberOfInterfaceConnections;
		degreeIteratioIdPairs.push_back(make_pair(degree, i));
	}
	sort(degreeIteratioIdPairs.begin(), degreeIteratioIdPairs.end(), pairCompare);

//	cout << "Sorted on degree: ";
	vector<int> sortedObjectIds;
	for (pair<int,int> degreeIdPair: degreeIteratioIdPairs) {
//		cout << "(" << degreeIdPair.first << "," << degreeIdPair.second << "), ";
		sortedObjectIds.push_back(degreeIdPair.second);
	}
//	cout << endl;
	return sortedObjectIds;
}

vector<int> incrementalDeterministicGraphGenerator::sortOnDegree(vector<int> subjectsIterationIds, int correlatedETid) {
	if (subjectsIterationIds.size() == 0) {
		vector<int> dummy;
		return dummy;
	}

	vector<int> indegreeDistr;
	for (unsigned int i=0; i<subjectsIterationIds.size(); i++) {
		indegreeDistr.push_back(0);
	}
//	cout << "indegreeDistr size: " << indegreeDistr.size();

	string line;
	ifstream outputfile(outputFileName + to_string(graphNumber) + ".txt");
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
			if (edgeTypeId == correlatedETid && objectIterationId >= subjectsIterationIds[0]) {
//				cout << "Found object: " << objectIterationId << endl;
//				cout << "Add one to " << objectIterationId - subjectsIterationIds[0] << endl;
				indegreeDistr[objectIterationId - subjectsIterationIds[0]]++;
			}

		}
	}

	vector<pair<int, int> > degreeIteratioIdPairs;
	for (unsigned int i=0; i<subjectsIterationIds.size(); i++) {
		degreeIteratioIdPairs.push_back(make_pair(indegreeDistr[i], subjectsIterationIds[i]));
	}
	sort(degreeIteratioIdPairs.begin(), degreeIteratioIdPairs.end(), pairCompare);

//	cout << "Sorted on degree: ";
	vector<int> sortedSubjectIds;
	for (pair<int,int> degreeIdPair: degreeIteratioIdPairs) {
//		cout << "(" << degreeIdPair.first << "," << degreeIdPair.second << "), ";
		sortedSubjectIds.push_back(degreeIdPair.second);
	}
//	cout << endl;
	return sortedSubjectIds;
}

void incrementalDeterministicGraphGenerator::icPreservingMapping(vector<int> subjects, vector<int> objects, int correlatedETid) {
	mapping.resize(max((int)subjects.size(), 1+subjects[subjects.size()-1]));

	vector<int> objectSortedOnDegree = sortOnDegree(objects);
	vector<int> subjectSortedOnDegree = sortOnDegree(subjects, correlatedETid);

	if (subjects.size() > objects.size()) {
		for (unsigned int i=0; i<subjects.size(); i++) {
			addToMapping(subjectSortedOnDegree[i], objectSortedOnDegree[i % objects.size()]);
		}
	} else {
		for (unsigned int i=0; i<objects.size(); i++) {
			addToMapping(subjectSortedOnDegree[i % subjects.size()], objectSortedOnDegree[i]);
		}
	}
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

	icPreservingMapping(subjectsOfMapping, objectsOfMapping, edgeType.correlated_with[0]);

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
	ifstream outputfile(outputFileName + to_string(graphNumber) + ".txt");
	outputfile.clear();
	outputfile.seekg(0, ios::beg);
	if (outputfile.is_open()) {
		while (getline(outputfile,line)) {
			int edgeCreatedAtGraphNb = stoi(line.substr(line.find("| ")+1, line.size()) );
			if (edgeCreatedAtGraphNb == graphNumber) {
//				cout << "Line: " << line << endl;
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
					vector<int> mappedObjects = mapping[objectIterationId];
	//				cout << "Object found: " << objectId << endl;
					for (int mappedObject: mappedObjects) {
	//					cout << "Mapped to: " << mappedObject << endl;
						edge2 possibleEdge;
						possibleEdge.subjectIterationId = subjectIterationId;
						possibleEdge.subjectId = subjectId;
						possibleEdge.predicate = edgeType.edge_type_id;
						possibleEdge.objectIterationId = mappedObject;
						possibleEdge.objectId = mappedObject * conf.types.size() + edgeType.object_type;

	//					cout << "Adding edge: " << possibleEdge.subjectId << " " << possibleEdge.predicate << " " <<  possibleEdge.objectId << endl;
						correlatedEdges.push_back(possibleEdge);
					}
				}
			}
		}
		outputfile.close();
	}
	return correlatedEdges;
}

} /* namespace std */
