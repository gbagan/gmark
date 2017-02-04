/*
 * analyseCorrelation.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: wilcovanleeuwen
 */

#include "analyseCorrelation.h"
#include <algorithm>
#include <vector>

namespace std {

analyseCorrelation::analyseCorrelation(config::config & conf_) {
	conf = conf_;
}

analyseCorrelation::~analyseCorrelation() {
	// TODO Auto-generated destructor stub
}

void analyseCorrelation::analyzeLocationUniCorrelation(string fileNameLoc, string fileNameUni) {
	vector<long> persons;
	vector<vector<long>> basisEdges;
	vector<vector<long>> correlatedEdges;
	vector<long> locations;

	vector<long> dummy(2);
	// Find all person ids
	string line;
	ifstream myfile(fileNameLoc);
	myfile.clear();
	myfile.seekg(0, ios::beg);
	int i = 0;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			if (i!=0) {
				string pid = line.substr(0, line.find("|"));
				cout << "Pid: " << pid << endl;
				long persId = stol(pid);

				string lid = line.substr(line.find("|")+1, line.size());
				long locationId = stol(lid);
				cout << "locationId: " << locationId << endl;
				locations.push_back(locationId);

				vector<long>::iterator index = find(persons.begin(), persons.end(), persId);
				if (index == persons.end()) {
					persons.push_back(persId);
				}
				dummy[0] = persId;
				dummy[1] = locationId;
				basisEdges.push_back(dummy);


			}
			i++;
		}
		myfile.close();
	} else {
		cout << "Unable to open file: " << fileNameLoc << endl;
	}

	sort(locations.begin(), locations.end());
	cout << "Sorted locations: ";
	for (long loc: locations) {
		cout << loc << ",";
	}
	cout << endl;


	i = 0;
	ifstream uniFile("ignore/person_hasInterest_tag_0_0.csv");
	uniFile.clear();
	uniFile.seekg(0, ios::beg);
	if (uniFile.is_open()) {
		while (getline(uniFile, line)) {
			if (i != 0) {
				string pid = line.substr(0, line.find("|"));
//				cout << "Pid: " << pid << endl;
				long persId = stol(pid);

				string temp = line.erase(0, line.find("|")+1);
				string uid = temp.substr(0, line.size());
//				cout << "Uid: " << uid << endl;
				long uniId = stol(uid);

				vector<long>::iterator index = find(persons.begin(), persons.end(), persId);
				if (index == persons.end()) {
					persons.push_back(persId);
				}
				dummy[0] = persId;
				dummy[1] = uniId;
				correlatedEdges.push_back(dummy);
			}
			i++;
		}
		uniFile.close();
	} else {
		cout << "Unable to open file: " << fileNameUni << endl;
	}

	cout << "NbPersons: " << persons.size() << endl;
	cout << "Basis.size: " << basisEdges.size() << endl;
	cout << "Correlated edges.size: " << correlatedEdges.size() << endl;
//	cout << "persLocUni.size(): " << persLocUni.size() << endl;
//	vector<vector<long>> persLocUniNew;
//	for (vector<long> persLocUniInst : persLocUni) {
//		if (persLocUniInst[1] > 0 && persLocUniInst[2] > 0) {
//			persLocUniNew.push_back(persLocUniInst);
//			cout << "Test: " << persLocUniInst[0] << " " << persLocUniInst[1] << " " << persLocUniInst[2] << endl;
//		}
//	}







	ofstream corrPersonLocation;
	corrPersonLocation.open ("basisLocationInterest.txt");
	corrPersonLocation << "basis = c(";

	ofstream simCorrelatedET;
	simCorrelatedET.open ("corrLocationInterest.txt");
	simCorrelatedET << "corrET = c(";


	// Similarity w.r.t basis
	int countForLineBreak = 0;
	for (int i=0; i<persons.size(); i++) {
		for (int j=i+1; j<persons.size(); j++) {
			vector<long> s1, s2;
			for (vector<long> edge: basisEdges) {
				if (edge[0] == persons[i]) {
					if (std::find(s1.begin(), s1.end(), edge[1]) == s1.end()) {
						s1.push_back(edge[1]);
					}
				}
				if (edge[0] == persons[j]) {
					if (std::find(s2.begin(), s2.end(), edge[1]) == s2.end()) {
						s2.push_back(edge[1]);
					}
				}
			}


			// For the basis
			vector<int> intersectionVec(max(s1.size(), s2.size()));
			vector<int>::iterator itIntersection;

			sort(s1.begin(), s1.end());
			sort(s2.begin(), s2.end());

			itIntersection = set_intersection(s1.begin(), s1.end(),
					s2.begin(), s2.end(), intersectionVec.begin());

			intersectionVec.resize(itIntersection-intersectionVec.begin());

			int unionCount = s1.size() + s2.size() - intersectionVec.size();


			if (unionCount == 0) {
				corrPersonLocation << "0.0";
			} else {
				corrPersonLocation << (float)intersectionVec.size() / (float)unionCount;
//				if ((float)intersectionVec.size() / (float)unionCount > 0) {
//					cout << "Found: " << (float)intersectionVec.size() / (float)unionCount << endl;
//				}
			}
			if (!(i >= persons.size()-2 && j >= persons.size()-1)) {
				corrPersonLocation << ",";
			}
			countForLineBreak++;
			if (countForLineBreak % 300 == 0) {
//				cout << "Line: " << countForLineBreak << endl;
				corrPersonLocation << endl;
				cout << i << "." << j << endl;
			}

		}
	}
	corrPersonLocation << ")" << endl;


	// Similarity w.r.t. correlated edge-type
	countForLineBreak = 0;
	for (int i=0; i<persons.size(); i++) {
		for (int j=i+1; j<persons.size(); j++) {
//			cout << i << "." << j << " - ";
			vector<long> s1, s2;
			for (vector<long> edge: correlatedEdges) {
				if (edge[0] == persons[i]) {
					if (std::find(s1.begin(), s1.end(), edge[1]) == s1.end()) {
						s1.push_back(edge[1]);
					}
				}
				if (edge[0] == persons[j]) {
					if (std::find(s2.begin(), s2.end(), edge[1]) == s2.end()) {
						s2.push_back(edge[1]);
					}
				}
			}
//			cout << "In s1: ";
//			for (int i=0; i<s1.size(); i++) {
//				cout << s1[i];
//				if (i != s1.size()-1) {
//					cout << ",";
//				}
//			}
//			cout << "In s2: ";
//			for (int i=0; i<s2.size(); i++) {
//				cout << s2[i];
//				if (i != s2.size()-1) {
//					cout << ",";
//				}
//			}


			// For the basis
			vector<int> intersectionVec(max(s1.size(), s2.size()));
			vector<int>::iterator itIntersection;

			sort(s1.begin(), s1.end());
			sort(s2.begin(), s2.end());

			itIntersection = set_intersection(s1.begin(), s1.end(),
					s2.begin(), s2.end(), intersectionVec.begin());

			intersectionVec.resize(itIntersection-intersectionVec.begin());

			int unionCount = s1.size() + s2.size() - intersectionVec.size();


			if (unionCount == 0) {
				simCorrelatedET << "0";
			} else {
				simCorrelatedET << (float)intersectionVec.size() / (float)unionCount;
//				if ((float)intersectionVec.size() / (float)unionCount > 0) {
//					cout << "Found: " << (float)intersectionVec.size() / (float)unionCount << endl;
//				}
			}
			if (!(i >= persons.size()-2 && j >= persons.size()-1)) {
				simCorrelatedET << ",";
			}
			countForLineBreak++;
			if (countForLineBreak % 300 == 0) {
//				cout << "Line: " << countForLineBreak << endl;
				simCorrelatedET << endl;

				cout << i << "." << j << endl;
			}

		}
	}
	simCorrelatedET << ")" << endl;
}

void analyseCorrelation::printSimilarities(int nodes, vector<incrementalDeterministicGraphGenerator::edge2> edges, int nbEdgeTypesInCorrelation) {
//	cout << "Node similarities" << endl;
	int countForLineBreak = 0;
	for (int i=0; i<nodes; i++) {
		for (int j=i+1; j<nodes; j++) {
//			cout << "i:" << i << " and j:" << j << " gives: ";
			cout << calculateSimilarity(i, j, edges, nbEdgeTypesInCorrelation);
			if (!(i >= nodes-2 && j >= nodes-1)) {
				cout << ",";
			}
			countForLineBreak++;
			if (countForLineBreak % 300 == 0) {
				cout << endl;
			}
		}
	}
//	cout << endl;
}

double analyseCorrelation::calculateSimilarity(int n1, int n2, vector<incrementalDeterministicGraphGenerator::edge2> edges, int nbEdgeTypesInCorrelation) {
//	cout << "Sim between " << n1 << " and " << n2 << endl;
//	cout << "Simialrity between node " << n1.iterationId << " and node " << n2.iterationId << endl;
	vector<int> s1, s2;
//	cout << "Number of edges: " << edges.size() << endl;
	for (incrementalDeterministicGraphGenerator::edge2 e: edges) {
//		cout << "Edge from " << e.subjectIterationId << " to " << e.objectIterationId << endl;
//		cout << "Edge: " << e.subjectIterationId << " " << e.objectIterationId << endl;
		if (e.subjectIterationId == n1) {
			int objectId = e.objectIterationId;
			if (std::find(s1.begin(), s1.end(), objectId) == s1.end()) {
				// Set s1 does not contain the new objectId
				s1.push_back(objectId);
//				if (n1 == 0 && n2 == 1) {
//					cout << "In s1: " << e.objectIterationId << endl;
//				}
			}
		}
		if (e.subjectIterationId == n2) {
			int objectId = e.objectIterationId;
			if (std::find(s2.begin(), s2.end(), objectId) == s2.end()) {
				// Set s2 does not contain the new objectId
				s2.push_back(objectId);
//				if (n1 == 0 && n2 == 1) {
//					cout << "In s2: " << e.objectIterationId << endl;
//				}
			}
		}
	}


	vector<int> intersectionVec(max(s1.size(), s2.size()));
	vector<int>::iterator itIntersection;

	sort(s1.begin(), s1.end());
	sort(s2.begin(), s2.end());

//		cout << "In s1: ";
//		for (int i:s1) {
//			cout << i << ",";
//		}
//		cout << endl;
//
//		cout << "In s2: ";
//		for (int i:s2) {
//			cout << i << ",";
//		}
//		cout << endl;

	itIntersection = set_intersection(s1.begin(), s1.end(),
			s2.begin(), s2.end(), intersectionVec.begin());

	intersectionVec.resize(itIntersection-intersectionVec.begin());

	int unionCount = s1.size() + s2.size() - intersectionVec.size();

//	cout << "Intersection: " << intersectionVec.size() << endl;
//	cout << "Union: " << unionCount << endl;
	if (unionCount == 0) {
		return 0.0;
	} else {
		return (float)intersectionVec.size() / (float)unionCount;
	}
}


int analyseCorrelation::findEdgeTypeId(int subjectId, int predicate, int objectId) {
	int edgeTypeId = -1;
	for (config::edge edgeType: conf.schema.edges) {
		if (edgeType.subject_type == subjectId &&
				edgeType.predicate == predicate &&
				edgeType.object_type == objectId) {
			edgeTypeId =  edgeType.edge_type_id;
		}
	}
	return edgeTypeId;
}


vector<incrementalDeterministicGraphGenerator::edge2> analyseCorrelation::addEdges(string file, vector<int> basis) {
	vector<incrementalDeterministicGraphGenerator::edge2> basisEdges;
	string line;
	ifstream myfile(file);
	myfile.clear();
	myfile.seekg(0, ios::beg);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
//			cout << "Line: " << line << endl;
			string temp = line;
			string getPred = temp.erase(0, temp.find(" ")+1);
			string predicate = getPred.substr(0, getPred.find(" "));
			int pred = stoi(predicate);

			temp = line;
			string subjectStr = temp.substr(0, temp.find(" "));
			int subject = stoi(subjectStr);
			int subjectType = subject % conf.types.size();
			int subjectId = subject / conf.types.size();

			getPred = getPred.erase(0, getPred.find(" ")+1);

			string objectStr = getPred.substr(0, getPred.length());
			int object = stoi(objectStr);
			int objectType = object % conf.types.size();
			int objectId = object / conf.types.size();

//			cout << subjectId << " " << pred << " " << objectId << endl;

			int edgeTypeId = findEdgeTypeId(subjectType, pred, objectType);
//			cout << "Edge above belongs to edge-type: " << edgeTypeId << endl;
			if (std::find(basis.begin(), basis.end(), edgeTypeId) != basis.end()) {
//				cout << line << endl;

				incrementalDeterministicGraphGenerator::edge2 basisEdge;
				basisEdge.objectId = object;
				basisEdge.objectIterationId = objectId;
				basisEdge.subjectId = subject;
				basisEdge.subjectIterationId = subjectId;
				basisEdge.predicate = pred;
				basisEdges.push_back(basisEdge);

//				cout << "SubjectId: " << basisEdge.subjectId << ". SubjectNm: " << basisEdge.subjectIterationId << endl;
//				cout << "ObjectId: " << basisEdge.objectId << ". ObjectNm: " << basisEdge.objectIterationId << endl;
//				cout << "Predicate: " << basisEdge.predicate << endl;
			}
		}
		myfile.close();
	} else {
		cout << "Unable to open file: " << file << endl;
	}

	return basisEdges;
}





void analyseCorrelation::analyze(config::edge & edgeType, int graphNumber) {
//	for (config::edge & edgeType: conf.schema.edges) {
//		if (edgeType.correlated_with.size() > 0) {
//			for (int i=0; i<conf.nb_graphs; i++) {
				string basisFile = "ignore/outputGraph" + to_string(graphNumber) + ".txt";

				// Similarity of basis
				vector<incrementalDeterministicGraphGenerator::edge2> basisEdges = addEdges(basisFile, edgeType.correlated_with);
				cout << "# Basis for correlation, based on edgeTypes: ";
				for (int id: edgeType.correlated_with) {
					cout << id << ", ";
				}
				cout << endl;

//				cout << "\n\n BasisEdges: " << endl;
//				for (incrementalDeterministicGraphGenerator::edge2 e: basisEdges) {
//					cout << e.subjectId << "-" << e.subjectIterationId << " " << e.predicate << " " << e.objectId << "-" << e.objectIterationId << endl;
//				}
//				cout << endl << endl;

				cout << "simP = c(";
				printSimilarities(conf.types[edgeType.subject_type].size[graphNumber] , basisEdges, edgeType.correlated_with.size());
				cout << ")" << endl;

				// Similarity of generated edge-type
				vector<int> correlatedEdgeType;
				correlatedEdgeType.push_back(edgeType.edge_type_id);
				vector<incrementalDeterministicGraphGenerator::edge2> correlatedEdges = addEdges(basisFile, correlatedEdgeType);

//				cout << "\n\n CorrelatedEdges: " << endl;
//				for (incrementalDeterministicGraphGenerator::edge2 e: correlatedEdges) {
//					cout << e.subjectId << "-" << e.subjectIterationId << " " << e.predicate << " " << e.objectId << "-" << e.objectIterationId << endl;
//				}
//				cout << endl << endl;

				cout << "# Generated correlated edge-type: " << endl;
				cout << "simQ = c(";
				printSimilarities(conf.types[edgeType.subject_type].size[graphNumber], correlatedEdges, 1);
				cout << ")" << endl;

				cout << "par(cex.main=1.8, cex.lab=1.5, cex.axis=1.5, col=\"black\")" << endl;
				cout << "plot(simP, simQ, main=\"nbObjects(p) = 2*nbObjects(q)\", xlab=\"Similarity in edge-type p\", ylab=\"Similarity in edge-type q\", col=rgb(0,0,0,50,maxColorValue=255), pch=16)" << endl;
				cout << "abline(lm(simQ~simP), col=\"blue\") # regression line (y~x)"  << endl;
				cout << "cov(simP, simQ) / (sd(simP)*sd(simQ))" << endl << endl;
//			}
//		}
//	}
}




} /* namespace std */
