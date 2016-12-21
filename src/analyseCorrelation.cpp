/*
 * analyseCorrelation.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: wilcovanleeuwen
 */

#include "analyseCorrelation.h"

namespace std {

analyseCorrelation::analyseCorrelation(config::config & conf_) {
	conf = conf_;
}

analyseCorrelation::~analyseCorrelation() {
	// TODO Auto-generated destructor stub
}

void analyseCorrelation::printSimilarities(int nodes, vector<incrementalDeterministicGraphGenerator::edge2> edges, int nbEdgeTypesInCorrelation) {
//	cout << "Node similarities" << endl;
	int countForLineBreak = 0;
	for (int i=0; i<nodes; i++) {
		for (int j=i+1; j<nodes; j++) {
//			cout << i << "." << j;
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
//	cout << "Simialrity between node " << n1.iterationId << " and node " << n2.iterationId << endl;
	vector<int> s1, s2;
	for (incrementalDeterministicGraphGenerator::edge2 e: edges) {
		if (e.subjectIterationId == n1) {
			int typeIdNumber = stoi(e.objectId.substr(0, e.objectId.find("-"))) + nbEdgeTypesInCorrelation * e.objectIterationId;
			if (std::find(s1.begin(), s1.end(), typeIdNumber) == s1.end()) {
				// Set s1 does not contain the new objectId
				s1.push_back(typeIdNumber);
//				if (n1 == 0 && n2 == 1) {
//					cout << "In s1: " << e.objectIterationId << endl;
//				}
			}
		}
		if (e.subjectIterationId == n2) {
			int typeIdNumber = stoi(e.objectId.substr(0, e.objectId.find("-"))) + nbEdgeTypesInCorrelation * e.objectIterationId;
			if (std::find(s2.begin(), s2.end(), typeIdNumber) == s2.end()) {
				// Set s2 does not contain the new objectId
				s2.push_back(typeIdNumber);
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
			string temp = line;
			string getPred = temp.erase(0, temp.find(" ")+1);
			string predicate = getPred.substr(0, getPred.find(" "));

			temp = line;
			string subjectType = temp.substr(0, temp.find("-"));
			string getSub = temp.erase(0, temp.find("-")+1);
			string subject = getSub.substr(0, getSub.find(" "));

			getPred = getPred.erase(0, getPred.find(" ")+1);

			string objectType = getPred.substr(0, getPred.find("-"));
			string getObj = getPred.erase(0, getPred.find("-")+1);
			string object = getObj.substr(0, getObj.length());

			int edgeTypeId = findEdgeTypeId(stoi(subjectType), stoi(predicate), stoi(objectType));
			if (std::find(basis.begin(), basis.end(), edgeTypeId) != basis.end()) {
//				cout << line << endl;

				incrementalDeterministicGraphGenerator::edge2 basisEdge;
				basisEdge.objectId = objectType;
				basisEdge.objectIterationId = stoi(object);
				basisEdge.subjectId = subjectType;
				basisEdge.subjectIterationId = stoi(subject);
				basisEdge.predicate = predicate;
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
				string basisFile = "outputGraph" + to_string(graphNumber) + ".txt";

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
				vector<int> correlatedEdgeType = {edgeType.edge_type_id};
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
