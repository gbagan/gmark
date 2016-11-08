/*
 * analysisIncrDetGraph.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: wilcovanleeuwen
 */

#include <fstream>
#include <cmath>
#include "analysisIncrDetGraph.h"


namespace std {

analysisIncrDetGraph::analysisIncrDetGraph(string outputFile_, config::config configuration) {
	this->outpuFile =  outputFile_;
	this->conf = configuration;
}

analysisIncrDetGraph::~analysisIncrDetGraph() {
	// TODO Auto-generated destructor stub
}

void analysisIncrDetGraph::numberOfEdgesVsNode() {
	int numberOfEdges = 0;
	string line;
	ifstream myfile(outpuFile);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			numberOfEdges++;
		}
	} else {
		cout << "Unable to open file";
	}
	myfile.close();

	cout << "Number of nodes in conf: " << conf.nb_nodes << endl;
	cout << "Number of edges generated: " << numberOfEdges << endl;
}

void analysisIncrDetGraph::numberOfNodesAnalysis() {
	cout << "\n\n-----Node-size analysis-----\n";

//	vector<vector<graphNode>> nodes = graph.nodes;
//	float accuracy, relError;
//	float totalAccuracy = 0;
//	int shouldBe, real, absError;
//	for(int i=0; i<conf.types.size(); i++) {
//		shouldBe = conf.types.at(i).size;
//
//		cout << "NodeType:" << conf.types.at(i).alias << " should have size=" << to_string(shouldBe) << endl;
//		int tempSize = 0;
//		for (graphNode n: nodes.at(i)) {
////			if(!n.is_virtual) {
//				tempSize++;
////			}
//		}
//		real = tempSize;
//		if(nodes.at(i).size() > 0) {
//			cout << "NodeType:" << conf.types.at(nodes.at(i).at(0).type).alias << " has size=" << to_string(real) << endl;
//		} else {
//			cout << "NodeType:" << conf.types.at(i).alias << " has size=" << to_string(real) << endl;
//		}
//		absError = abs(shouldBe-real);
//		relError = (float)absError / (float)shouldBe;
//		accuracy = 1.0 - relError;
//		totalAccuracy += accuracy;
//		cout << "Accuracy=" << to_string(accuracy) << endl;
//	}
//
//	totalAccuracy = totalAccuracy / (float)conf.types.size();
//	cout << "Total accuracy: " << to_string(totalAccuracy) << endl;
}


void analysisIncrDetGraph::distributionAnalysis(config::edge edgeType, ofstream & rFile) {
	cout << "\n\n-----Distribution analysis-----\n";
//	cout << "EdgeType: " << to_string(edgeType) << endl;


	vector<int> outDistr;
	vector<int> inDistr;

	// Get max values (localIds) of the subjects and object nodes
	int maxSubject = 0;
	int maxObject = 0;
	string line;
	ifstream myfile(outpuFile);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string temp = line;
			string getPred = temp.erase(0, temp.find(" ")+1);
			string predicate = getPred.substr(0, getPred.find(" "));
			if (stoi(predicate) == edgeType.predicate) {
				temp = line;
				string subjectType = temp.substr(0, temp.find("-"));
				string getSub = temp.erase(0, temp.find("-")+1);
				string subject = getSub.substr(0, getSub.find(" "));
				if (stoi(subjectType) == edgeType.subject_type && stoi(subject) > maxSubject) {
					maxSubject = stoi(subject);
				}

				getPred = getPred.erase(0, getPred.find(" ")+1);
				string objectType = getPred.substr(0, getPred.find("-"));
				string getObj = getPred.erase(0, getPred.find("-")+1);
				string object = getObj.substr(0, getObj.length());
				if (stoi(objectType) == edgeType.object_type && stoi(object) > maxObject) {
					maxObject = stoi(object);
				}
			}
		}
	} else {
		cout << "Unable to open file";
	}
	maxSubject = max(maxSubject, (int)conf.types[edgeType.subject_type].size-1);
	maxObject = max(maxObject, (int)conf.types[edgeType.object_type].size-1);

//	cout << "MaxSubject: " << maxSubject << endl;
//	cout << "MaxObject: " << maxObject << endl;

	// Initialize outDistr-vector
	for (int i=0; i<=maxSubject; i++) {
		outDistr.push_back(0);
	}

	// Initialize inDistr-vector
	for (int i=0; i<=maxObject; i++) {
		inDistr.push_back(0);
	}

	// Analyze all edges
	myfile.clear();
	myfile.seekg(0, ios::beg);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string temp = line;
			string getPred = temp.erase(0, temp.find(" ")+1);
			string predicate = getPred.substr(0, getPred.find(" "));
			if (stoi(predicate) == edgeType.predicate) {
				temp = line;
				string subjectType = temp.substr(0, temp.find("-"));
				string getSub = temp.erase(0, temp.find("-")+1);
				string subject = getSub.substr(0, getSub.find(" "));

				getPred = getPred.erase(0, getPred.find(" ")+1);

				string objectType = getPred.substr(0, getPred.find("-"));
				string getObj = getPred.erase(0, getPred.find("-")+1);
				string object = getObj.substr(0, getObj.length());

				if (stoi(objectType) == edgeType.object_type && stoi(subjectType) == edgeType.subject_type) {
					outDistr.at(stoi(subject))++;
					inDistr.at(stoi(object))++;
				}
			}
		}
		myfile.close();
	} else {
		cout << "Unable to open file";
	}

	int i = 0;
	rFile << "OutDistribution <- c(";
	for (int nm: outDistr) {
		rFile << nm;
		if(i != outDistr.size()-1) {
			rFile << ", ";
		}
		i++;
		if(i!=0 && i % 400 == 0) {
			rFile << endl;
		}
	}
	rFile << ")" << endl;
	printToRfile(rFile, true, edgeType);

	i=0;
	rFile << "InDistribution <- c(";
	for (int nm: inDistr) {
		rFile << nm;
		if(i != inDistr.size()-1) {
			rFile << ", ";
		}
		i++;
		if(i!=0 && i % 400 == 0) {
			rFile << endl;
		}
	}
	rFile << ")" << endl;

	printToRfile(rFile, false, edgeType);
}

void analysisIncrDetGraph::printToRfile(ofstream& rFile, bool outDistr, config::edge edge) {
	string distributionVar;
	distribution* distr;
	int nodeType;
	if(outDistr) {
		distributionVar = "OutDistribution";
		distr = &edge.outgoing_distrib;
		nodeType = edge.subject_type;
	} else {
		distributionVar = "InDistribution";
		distr = &edge.incoming_distrib;
		nodeType = edge.object_type;
	}

	rFile << "par(pch=22, col=\"black\")" << endl;
	rFile << "hist(" << distributionVar <<", xlab=\"Number of edges per " << conf.types.at(nodeType).alias << "\", breaks=c(seq(min("<< distributionVar <<")-0.5, max("<< distributionVar <<")+0.5,1)), main=\""<< distributionVar <<" of edge-type " << edge.edge_type_id << "\", prob=TRUE, ylim=c(0,1))" << endl;
	if(distr->type == DISTRIBUTION::UNIFORM) {

		rFile << "xRange = c(" << distr->arg1 - 0.5 << ", " << distr->arg2 + 0.5 << ")" << endl;
		rFile << "yMean = c(rep(1/" << (distr->arg2 - distr->arg1)+1 << ", 2))" << endl;
		rFile << "par(pch=22, col=\"blue\")" << endl;
		rFile << "lines(xRange, yMean, lty=2)" << endl;
	} else if(distr->type == DISTRIBUTION::ZIPFIAN) {
		rFile << "x = c(1:max("<< distributionVar <<"))" << endl;
		rFile << "y = x^-" << distr->arg2 << endl;
		rFile << "sum = 0" << endl;
		rFile << "for(i in 1:length(x)) {" << endl;
		rFile << "  sum = sum + y[i]" << endl;
		rFile << "}" << endl;
		rFile << "par(pch=22, col=\"blue\")" << endl;
		rFile << "lines(x,y/sum)" << endl;

		rFile << "\n# Log-log plot" << endl;
		rFile << "plot(sort("<< distributionVar <<", decreasing=TRUE), log=\"xy\", ylab=\"Number of edges\", xlab=\"" << conf.types.at(nodeType).alias << " sorted on number of edges in edge-type " << edge.edge_type_id << "\", main=\"Log-log plot of the Zipfian "<< distributionVar <<" of edge-type " << edge.edge_type_id << "\")" << endl;

		rFile << "\n# Zoomed view" << endl;
		rFile << "hist(" << distributionVar <<", xlab=\"Number of edges per " << conf.types.at(nodeType).alias << "\", breaks=c(seq(min("<< distributionVar <<")-0.5, max("<< distributionVar <<")+0.5,1)), main=\""<< distributionVar <<" of edge-type " << edge.edge_type_id << "\", prob=TRUE, xlim=c(0,10))" << endl;
		rFile << "lines(x,y/sum)" << endl;

		rFile << "\n# Remove the nodes with zero edges" << endl;
		rFile << "noZeros = c()" << endl;
		rFile << "for (i in 1:length(" << distributionVar <<")) {" << endl;
		rFile << "if (" << distributionVar <<"[i] != 0)" << endl;
		rFile << "noZeros = c(noZeros, " << distributionVar <<"[i])" << endl;
		rFile << "}" << endl;
		rFile << "hist(noZeros, xlab=\"Number of edges per person\", breaks=c(seq(min(" << distributionVar <<")-0.5, max(" << distributionVar <<")+0.5,1)), main=\"" << distributionVar <<" of edge-type  " << edge.edge_type_id << "\", prob=TRUE, xlim=c(0,10))" << endl;
		rFile << "lines(x,y/sum)" << endl;


//		rFile << "sortedDistr = sort("<< distributionVar <<", decreasing=TRUE)" << endl;
//		rFile << "sum = 0" << endl;
//		rFile << "for(i in 1:length(" << distributionVar << ")) {" << endl;
//		rFile << "  sum = sum+sortedDistr[i]" << endl;
//		rFile << "}" << endl;
//		rFile << "pdf = sortedDistr/sum" << endl;
//		rFile << "par(pch=22, col=\"black\")" << endl;
//		rFile << "plot(pdf, main=\""<< distributionVar <<" of edge-type " << edge.edge_type_id << "\", xlab=\"" << conf.types.at(nodeType).alias << " sorted on the number of out-going edges with edge-type " << edge.edge_type_id << "\", ylab=\"Density\")" << endl;
//		rFile << "xValues = c(1:sortedDistr[1])" << endl;
//		rFile << "zipf = xValues^-" << distr->arg2 << endl;
//		rFile << "zipfSum = 0" << endl;
//		rFile << "for(i in 1:length(xValues)) {" << endl;
//		rFile << "  zipfSum = zipfSum + zipf[i]" << endl;
//		rFile << "}" << endl;
//		rFile << "zipf = zipf / zipfSum" << endl;
//		rFile << "par(pch=22, col=\"blue\")" << endl;
//		rFile << "lines(xValues, zipf, type=\"l\")" << endl;
//
//		rFile << "\n# Zoomed view" << endl;
//		rFile << "par(pch=22, col=\"black\")" << endl;
//		rFile << "plot(pdf, main=\""<< distributionVar <<" of edge-type " << edge.edge_type_id << "\", xlab=\"" << conf.types.at(nodeType).alias << " sorted on the number of out-going edges with edge-type " << edge.edge_type_id << "\", ylab=\"Density\", xlim=c(0,50))" << endl;
//		rFile << "par(pch=22, col=\"blue\")" << endl;
//		rFile << "lines(xValues, zipf, type=\"l\")" << endl;
//
//		rFile << "\n# Log view" << endl;
//		rFile << "par(pch=22, col=\"black\")" << endl;
//		rFile << "plot(pdf, main=\""<< distributionVar <<" of edge-type " << edge.edge_type_id << "\", xlab=\"" << conf.types.at(nodeType).alias << " sorted on the number of out-going edges with edge-type " << edge.edge_type_id << "\", ylab=\"Density\", log=\"xy\")" << endl;
//		rFile << "par(pch=22, col=\"blue\")" << endl;
//		rFile << "lines(xValues, zipf, type=\"l\")" << endl;
	} else if(distr->type == DISTRIBUTION::NORMAL || distr->type == DISTRIBUTION::UNDEFINED) {
		rFile << "xValues = seq(min(" << distributionVar << ")-0.5, max(" << distributionVar << ")+0.5, 0.01)" << endl;
		rFile << "norm = dnorm(xValues, mean=" << distr->arg1 << ", sd=" << distr->arg2 << ")" << endl;
		rFile << "par(pch=22, col=\"blue\")" << endl;
		rFile << "lines(xValues, norm, type=\"l\")" << endl;
	}
	rFile << endl;
}


string getNodeWithHighestZipfianPos(int edgeTypeId, bool outDistr) {
	double maxPos = 0.0;
	string node;
	string line;

	string fileName;
	if (outDistr) {
		fileName = "zipfPosSmallestGraphOutDistr" + to_string(edgeTypeId) + ".txt";
	} else {
		fileName = "zipfPosSmallestGraphInDistr" + to_string(edgeTypeId) + ".txt";
	}
	ifstream myfile(fileName);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string temp = line;
			string getPos = temp.erase(0, temp.find(" = ")+3);
			string pos = getPos.substr(0, getPos.size());
			if (stod(pos) > maxPos) {
				maxPos = stod(pos);
				node = line.substr(0, line.find(" = "));
			}
		}
		cout << "Node with highest position in Zipfian degree distribution: " << node << endl;
	} else {
		cout << "Unable to open file";
	}

	return node;
}

void analysisIncrDetGraph::zipfianPosAnalysis() {
	for (int i=0; i<conf.schema.edges.size(); i++) {

		if (conf.schema.edges[i].outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
			string influencorNode = getNodeWithHighestZipfianPos(i, true);
			for (int j=0; j<conf.nb_graphs; j++) {

			}
		}

		if (conf.schema.edges[i].incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
			string influencorNode = getNodeWithHighestZipfianPos(i, false);
			for (int j=0; j<conf.nb_graphs; j++) {

			}
		}
	}
}

void analysisIncrDetGraph::zipfianPosAnalysis(int edgeTypeId, bool outDistr) {
	// Find node with highest position in Zipf degree distr
	string node = getNodeWithHighestZipfianPos(edgeTypeId, outDistr);



	// Get the number of connection to or from this node with the highest position
	string line;
	int connections = 0;
	ifstream graphFile("outputGraph" + to_string(edgeTypeId+1) + ".txt");
	if (graphFile.is_open()) {
		while (getline(graphFile, line)) {
			string temp = line;
			string subject = temp.substr(0, temp.find(" "));
			if (outDistr) {
				if (subject.compare(node) == 0) {
					connections++;
				}
			} else {
				string remSubject = temp.erase(0, temp.find(" ")+1);
				string remPred = remSubject.erase(0, remSubject.find(" ")+1);
//				cout << "Object = " << remPred << endl;
				if (remPred.compare(node) == 0) {
					connections++;
				}
			}
		}
		cout << "Number of connections: " << connections << endl;
	} else {
		cout << "Unable to open file";
	}

}

double meann(vector<double> doubles) {
	double sum = 0;
	for (double d: doubles) {
		sum += d;
	}
	return sum / (double)doubles.size();
}


double sd(vector<string> doubleStrings) {
	vector<double> doubles;
	for (string dStr: doubleStrings) {
		if (dStr.compare("")) {
			doubles.push_back(stod(dStr));
		}
	}

	double mean = meann(doubles);
	double sumOfSquaredDifferences = 0;
	for (double d: doubles) {
		sumOfSquaredDifferences += (d - mean) * (d - mean);
	}

	double sd = sqrt(1.0 / (double) doubleStrings.size() * sumOfSquaredDifferences);
	return sd;
}


void analysisIncrDetGraph::relativeDegreeChange(int etId) {
	string baseFileName = "rankFileET" + to_string(etId);
	ifstream rankFile1(baseFileName + "n1000.txt");
	ifstream rankFile2(baseFileName + "n2000.txt");
	ifstream rankFile3(baseFileName + "n3000.txt");
	ifstream rankFile4(baseFileName + "n4000.txt");
	ifstream rankFile5(baseFileName + "n5000.txt");
	ifstream rankFile6(baseFileName + "n6000.txt");
	ifstream rankFile7(baseFileName + "n7000.txt");
	ifstream rankFile8(baseFileName + "n8000.txt");
	ifstream rankFile9(baseFileName + "n9000.txt");
	ifstream rankFile10(baseFileName + "n10000.txt");
	ifstream* files[10] = {&rankFile1, &rankFile2, &rankFile3, &rankFile4, &rankFile5, &rankFile6, &rankFile7, &rankFile8, &rankFile9, &rankFile10};
	string line;
	vector<vector<string>> rankVector;
	if (rankFile1.is_open() && rankFile2.is_open() && rankFile3.is_open() && rankFile4.is_open() && rankFile5.is_open() &&
			rankFile6.is_open() && rankFile7.is_open() && rankFile8.is_open() && rankFile9.is_open() && rankFile10.is_open()) {
		for (int i=0; i<9; i++) {
			cout << i << endl;
			while (getline(*files[i], line)) {
				vector<string> ranksForOneNode;
//				cout << line;
				ranksForOneNode.push_back(line);
				for (int i=1; i<10; i++) {
					getline(*files[i], line);
//					cout << ", " << line;
					ranksForOneNode.push_back(line);
				}
				rankVector.push_back(ranksForOneNode);
//				cout << endl;
			}
		}
		cout << "NbLines=" << rankVector.size() << endl;
	} else {
		cout << "Unable to open file";
	}

	vector<double> sds;
	for (vector<string> nodeRanks: rankVector) {
		sds.push_back(sd(nodeRanks));
	}

	double sum = 0;
	for (double sd: sds) {
		sum += sd;
	}

	cout << "Mean sd = " << sum / (double)sds.size();

}

} /* namespace std */
