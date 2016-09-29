/*
 * analysisIncrDetGraph.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: wilcovanleeuwen
 */

#include <fstream>
#include "analysisIncrDetGraph.h"


namespace std {

analysisIncrDetGraph::analysisIncrDetGraph(incrementalDeterministicGraph incDetGraph, config::config configuration) {
	this->graph =  incDetGraph;
	this->conf = configuration;
}

analysisIncrDetGraph::~analysisIncrDetGraph() {
	// TODO Auto-generated destructor stub
}

void analysisIncrDetGraph::numberOfNodesAnalysis() {
	cout << "\n\n-----Node-size analysis-----\n";

	vector<vector<graphNode>> nodes = graph.nodes;
	float accuracy, relError;
	float totalAccuracy = 0;
	int shouldBe, real, absError;
	for(int i=0; i<conf.types.size(); i++) {
		shouldBe = conf.types.at(i).size;

		cout << "NodeType:" << conf.types.at(i).alias << " should have size=" << to_string(shouldBe) << endl;
		int tempSize = 0;
		for (graphNode n: nodes.at(i)) {
//			if(!n.is_virtual) {
				tempSize++;
//			}
		}
		real = tempSize;
		if(nodes.at(i).size() > 0) {
			cout << "NodeType:" << conf.types.at(nodes.at(i).at(0).type).alias << " has size=" << to_string(real) << endl;
		} else {
			cout << "NodeType:" << conf.types.at(i).alias << " has size=" << to_string(real) << endl;
		}
		absError = abs(shouldBe-real);
		relError = (float)absError / (float)shouldBe;
		accuracy = 1.0 - relError;
		totalAccuracy += accuracy;
		cout << "Accuracy=" << to_string(accuracy) << endl;
	}

	totalAccuracy = totalAccuracy / (float)conf.types.size();
	cout << "Total accuracy: " << to_string(totalAccuracy) << endl;
}


void analysisIncrDetGraph::distributionAnalysis(int edgeType, ofstream & rFile) {
//	cout << "\n\n-----Distribution analysis-----\n";
//	cout << "EdgeType: " << to_string(edgeType) << endl;

	vector<vector<graphNode>> nodes = graph.nodes;
	vector<vector<graphEdge>> edges = graph.edges;
	vector<graphEdge> edgesOfEdgeType = edges.at(edgeType);
	vector<int> outDistr;
	vector<int> inDistr;

	// Initialize outDistr-vector
	int subjectType = conf.schema.edges.at(edgeType).subject_type;
	int numberOfNodesWithSubjectType = 0;
	for (graphNode n: nodes.at(subjectType)) {
//		if(!n.is_virtual) {
			numberOfNodesWithSubjectType++;
//		}
	}
	for (int i=0; i<numberOfNodesWithSubjectType; i++) {
		outDistr.push_back(0);
	}

	// Initialize inDistr-vector
	int objectType = conf.schema.edges.at(edgeType).object_type;
	int numberOfNodesWithObjectType = 0;
	for (graphNode n: nodes.at(objectType)) {
//		if(!n.is_virtual) {
			numberOfNodesWithObjectType++;
//		}
	}
	for (int i=0; i<numberOfNodesWithObjectType; i++) {
		inDistr.push_back(0);
	}

	// Analyze all edges
	for(graphEdge e: edgesOfEdgeType) {
		outDistr.at(e.source.iterationId)++;
		inDistr.at(e.target.iterationId)++;
	}

	int i = 0;
	rFile << "OutDistribution <- c(";
	for (int nm: outDistr) {
		rFile << to_string(nm);
		if(i != outDistr.size()-1) {
			rFile << ", ";
		}
		i++;
		if(i!=0 && i % 100 == 0) {
			rFile << endl;
		}
	}
	rFile << ")" << endl;
	printToRfile(rFile, true, conf.schema.edges.at(edgeType));

	i=0;
	rFile << "InDistribution <- c(";
	for (int nm: inDistr) {
		rFile << to_string(nm);
		if(i != inDistr.size()-1) {
			rFile << ", ";
		}
		i++;
		if(i!=0 && i % 100 == 0) {
			rFile << endl;
		}
	}
	rFile << ")" << endl;

	printToRfile(rFile, false, conf.schema.edges.at(edgeType));
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
	} else if(distr->type == DISTRIBUTION::NORMAL) {
		rFile << "xValues = seq(min(" << distributionVar << ")-0.5, max(" << distributionVar << ")+0.5, 0.01)" << endl;
		rFile << "norm = dnorm(xValues, mean=" << distr->arg1 << ", sd=" << distr->arg2 << ")" << endl;
		rFile << "par(pch=22, col=\"blue\")" << endl;
		rFile << "lines(xValues, norm, type=\"l\")" << endl;
	}
	rFile << endl;
}

} /* namespace std */
