/*
 * incrementalDeterministicGraphGenerator.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include <vector>
#include "incrementalDeterministicGraphGenerator.h"
#include "graphNode.h"

namespace std {

incrementalDeterministicGraphGenerator::incrementalDeterministicGraphGenerator() {
	randomGenerator.seed(chrono::system_clock::now().time_since_epoch().count());
}

incrementalDeterministicGraphGenerator::~incrementalDeterministicGraphGenerator() {
	// TODO Auto-generated destructor stub
}

// ####### Generate edges #######
//int incrementalDeterministicGraphGenerator::findSourceNode(config::edge & edgeType, vector<graphNode*> &nodesWithOpenICs) {
//	double randomValue = uniformDistr(randomGenerator);
//	int subjectNodeId = cumDistrUtils.calculateCDF(nodesWithOpenICs, tempNode, randomValue);
//
//	return subjectNodeId;
//}
//
//int incrementalDeterministicGraphGenerator::findTargetNode(config::edge & edgeType, int sourceNodeLocalId, vector<graphNode*> &nodesWithOpenICs) {
//	double randomValue = uniformDistr(randomGenerator);
//	int objectNodeId = cumDistrUtils.calculateCDF(nodesWithOpenICs, nodes.first[sourceNodeLocalId], randomValue);
//
//	return objectNodeId;
//}


void incrementalDeterministicGraphGenerator::addEdge(graphNode &sourceNode, graphNode &targetNode, int predicate) {
//	cout << "SourceNode" << sourceNode.iterationId << endl;
//	cout << "Before: subject openICs = " << sourceNode.getNumberOfOpenInterfaceConnections() << endl;
	sourceNode.decrementOpenInterfaceConnections();
//	cout << "After: subject openICs = " << sourceNode.getNumberOfOpenInterfaceConnections() << endl;
	targetNode.decrementOpenInterfaceConnections();

//	if (outputBufferLines < 10 && predicate == 1) {
//		cout << to_string(sourceNode.type) + "-" + to_string(sourceNode.iterationId) + " " + to_string(predicate) + " " + to_string(targetNode.type) + "-" + to_string(targetNode.iterationId) + "\n";
//	}

	edge2 newEdge;
	newEdge.subjectId = sourceNode.id;
	newEdge.predicate = to_string(predicate);
	newEdge.objectId = targetNode.id;

	edges.push_back(newEdge);

//	string outputBuffer = to_string(sourceNode.type) + "-" + to_string(sourceNode.iterationId) + " " + to_string(predicate) + " " + to_string(targetNode.type) + "-" + to_string(targetNode.iterationId);

//	outputBufferLines++;
//	if (outputBufferLines % 50 == 0 || lastEdge) {
		// Use endl to flush
//		*outputFile << outputBuffer << endl;
//	} else {
//		outputBuffer += outputBuffer + "\n";
//	}
//	*outputFile << sourceNode.type << "-" << sourceNode.iterationId << " " << predicate << " " << targetNode.type << "-" << targetNode.iterationId << endl;
}
// ####### Generate edges #######


// ####### Update interface-connections #######
void incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> *nodesVec, distribution distr) {
//	cout << "New Zipfian case" << endl;
	int nmNodes = nodesVec->size();

	vector<double> zipfianCdf = cumDistrUtils.zipfCdf(distr.arg2, nmNodes);
	int newInterfaceConnections = 0;
	int difference = 0;
	for (graphNode & node: *nodesVec) {
		newInterfaceConnections = cumDistrUtils.findPositionInCdf(zipfianCdf, node.getPosition()) + distr.arg1;

//		cout << "newInterfaceConnections: " << newInterfaceConnections << endl;
		difference = newInterfaceConnections - node.getNumberOfInterfaceConnections();
		node.incrementOpenInterfaceConnectionsByN(difference);
		node.setNumberOfInterfaceConnections(newInterfaceConnections);
//		cout << "after openICs: " << node.getNumberOfOpenInterfaceConnections() << endl;
	}
}


void incrementalDeterministicGraphGenerator::updateICsForNonScalableType(vector<graphNode> & nodes, int nmNodesOther, double meanUpdateDistr, double meanNonUpdateDistr, distribution & distr) {
//	cout << "Updating ICs for non scalable type" << endl;
	if (nodes.size() == 0) {
		cout << "A fixed amount of nodes of one of the types is equal to 0" << endl;
		return;
	}

//	cout << "iterationNumber: " << iterationNumber << endl;
	int increment = floor((((nmNodesOther + 1) * meanNonUpdateDistr) / nodes.size()) - meanUpdateDistr);
	if (increment > 0) {
		if (distr.type == DISTRIBUTION::NORMAL || distr.type == DISTRIBUTION::ZIPFIAN) {
//			cout << "Before distr.arg1: " << distr.arg1 << endl;
			distr.arg1 += increment;
//			cout << "After distr.arg1: " << distr.arg1 << endl;
		} else if (distr.type == DISTRIBUTION::UNIFORM) {
//			cout << "Before distr.arg1: " << distr.arg1 << endl;
//			cout << "Before distr.arg2: " << distr.arg2 << endl;
			double diff = distr.arg2 - distr.arg1;
			double newMean = ((distr.arg2 + distr.arg1) / 2) + (double) increment;
			if (diff > 0.0) {
				distr.arg1 = max(round(newMean - (diff/2)), 0.0);
				distr.arg2 = round(newMean + (diff/2));
			} else {
				distr.arg1 = newMean;
				distr.arg2 = newMean;
			}
//				cout << "After distr.arg1: " << distr.arg1 << endl;
//				cout << "After distr.arg2: " << distr.arg2 << endl;
		}

//		cout << "Update nodes with " << increment << endl;
		for (graphNode & node: nodes) {
			node.incrementOpenInterfaceConnectionsByN(increment);
			node.incrementInterfaceConnectionsByN(increment);
		}
	}
}

void incrementalDeterministicGraphGenerator::updateICsForNonScalableType(config::edge & edgeType) {
	if (!conf.types.at(edgeType.subject_type).scalable) {
		// Subject is not scalable so update the ICs of all the subject nodes
		if (conf.types.at(edgeType.object_type).size >= conf.types.at(edgeType.subject_type).size) {
			double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, nodes.first.size());
			double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, nodes.second.size());
			updateICsForNonScalableType(nodes.first, conf.types.at(edgeType.object_type).size, meanOutDistr, meanInDistr, edgeType.outgoing_distrib);
		}
	} else {
		// object is not scalable so update the ICs of all the object nodes
		if (conf.types.at(edgeType.subject_type).size >= conf.types.at(edgeType.object_type).size) {
			double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, nodes.first.size());
			double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, nodes.second.size());
			updateICsForNonScalableType(nodes.second, conf.types.at(edgeType.subject_type).size, meanInDistr, meanOutDistr, edgeType.incoming_distrib);
		}
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
		if (distr.arg2 > 3.5) {
			// This mean hold for alpha > 2, but I will use it only when alpha > 3.5 to keep the error between the theoretical mean and the practical mean as low as possible
			// TODO: maybe instead of the magic number 3.5: use a value dependent on alpha and the total number of nodes in the graph, n. The higher the n, the lower the alpha can be. But always alpha > 2.
			meanEdgesPerNode = (1.0/(distr.arg2-2.0)) + distr.arg1;
		} else {
			double temp = 0.0;
			for (int i=1; i<=zipfMax; i++) {
				temp += i*pow((i), -1*distr.arg2);
			}
			meanEdgesPerNode = temp - 1 + distr.arg1;
		}
//		cout << "Zipfian mean=" << temp << endl;
	} else {
		meanEdgesPerNode = 1;
	}
	return meanEdgesPerNode;
}

void incrementalDeterministicGraphGenerator::changeDistributionParams(distribution & distr, double meanICsPerNodeForOtherDistr, double probOrSizeOther, double probOrSize) {
	if (distr.type == DISTRIBUTION::NORMAL) {
		distr.arg1 = (probOrSizeOther * meanICsPerNodeForOtherDistr) / probOrSize;
//			cout << "new normal mean: " << distr.arg1 << endl;
	} else if (distr.type == DISTRIBUTION::UNIFORM) {
		double diff = distr.arg2 - distr.arg1;
		double newMean = (probOrSizeOther * meanICsPerNodeForOtherDistr) / probOrSize;
		if (diff > 0.0) {
			distr.arg1 = max(round(newMean - (diff/2)), 0.0);
			distr.arg2 = round(newMean + (diff/2));
		} else {
			distr.arg1 = round(newMean);
			distr.arg2 = round(newMean);
		}
//		cout << "New uniform min: " << distr.arg1 << endl;
//		cout << "New uniform max: " << distr.arg2 << endl;
	}
}

void incrementalDeterministicGraphGenerator::changeDistributionParams(config::edge & edgeType) {
	if (conf.types.at(edgeType.subject_type).scalable && conf.types.at(edgeType.subject_type).proportion == 0) {
		conf.types.at(edgeType.subject_type).proportion = 1.0 / (double)conf.types.size();
		conf.types.at(edgeType.subject_type).size = (1.0 / (double)conf.types.size()) * conf.nb_nodes;
	}
	if (conf.types.at(edgeType.object_type).scalable && conf.types.at(edgeType.object_type).proportion == 0) {
		conf.types.at(edgeType.object_type).scalable = true;
		conf.types.at(edgeType.object_type).proportion = 1.0 / (double)conf.types.size();
		conf.types.at(edgeType.object_type).size = (1.0 / (double)conf.types.size()) * conf.nb_nodes;
	}

	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
		if (conf.types.at(edgeType.subject_type).scalable) {
			// In-distr is not scalable, so change the in-distribution
			double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000);
			double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, conf.types.at(edgeType.object_type).size);
			if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
				edgeType.incoming_distrib.arg1 = max(1.0, round(meanICsPerNodeForOutDistr - meanICsPerNodeForInDistr));
			} else {
				changeDistributionParams(edgeType.incoming_distrib, meanICsPerNodeForOutDistr, conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.object_type).size);
			}
		} else {
			// Out-distr is not scalable, so change the out-distribution
			double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000);
			double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, conf.types.at(edgeType.subject_type).size);
			if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
				edgeType.outgoing_distrib.arg1 = max(1.0, round(meanICsPerNodeForInDistr - meanICsPerNodeForOutDistr));
			} else {
				changeDistributionParams(edgeType.outgoing_distrib, meanICsPerNodeForInDistr, conf.types.at(edgeType.subject_type).size, conf.types.at(edgeType.subject_type).size);
			}
		}


		if (edgeType.incoming_distrib.type == DISTRIBUTION::UNDEFINED) {
			edgeType.incoming_distrib = distribution(DISTRIBUTION::UNIFORM, 1, 1);
		}
		return;
	}

	double subjectProbOrSize;
	double objectProbOrSize;
	if (conf.types.at(edgeType.subject_type).scalable) {
		subjectProbOrSize = conf.types.at(edgeType.subject_type).proportion;
		objectProbOrSize = conf.types.at(edgeType.object_type).proportion;
	} else {
		subjectProbOrSize = conf.types.at(edgeType.subject_type).size;
		objectProbOrSize = conf.types.at(edgeType.object_type).size;
	}

	// Out-distr is Zipfian, so change the params of the in-distr (only if )
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000);
		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 0);

		if ((objectProbOrSize * meanICsPerNodeForInDistr) > (subjectProbOrSize * meanICsPerNodeForOutDistr)) {
			// Mean of ICsPerIteration for non-Zipfian distr is higher that the mean of the Zipfian distr
//			changeDistributionParams(edgeType, meanICsPerNodeForOutDistr, false, subjectProbOrSize, objectProbOrSize);
			double newMean = (objectProbOrSize * meanICsPerNodeForInDistr) / subjectProbOrSize;
			edgeType.outgoing_distrib.arg1 = ceil(newMean - meanICsPerNodeForOutDistr);
//			cout << "Change the Zipfian start value to: " << zipfianStartValueOut << endl;
		}
		return;
	}
	// In-distr is Zipfian, so change the params of the out-distr
	if (edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000);
		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 0);

		if ((subjectProbOrSize * meanICsPerNodeForOutDistr) > (objectProbOrSize * meanICsPerNodeForInDistr)) {
			// Mean of ICsPerIteration for non-Zipfian distr is higher that the mean of the Zipfian distr
//			changeDistributionParams(edgeType, meanICsPerNodeForInDistr, true, subjectProbOrSize, objectProbOrSize);
			double newMean = (subjectProbOrSize * meanICsPerNodeForOutDistr) / objectProbOrSize;
			edgeType.incoming_distrib.arg1 = ceil(newMean - meanICsPerNodeForInDistr);
//			cout << "Change the Zipfian start value to: " << zipfianStartValueIn << endl;
		}
		return;
	}

	// Both are non-Zipfian
	if (edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 0);
		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 0);

		if (meanICsPerNodeForOutDistr * subjectProbOrSize >
			meanICsPerNodeForInDistr * objectProbOrSize) {
			// Change in-distr
			changeDistributionParams(edgeType.incoming_distrib, meanICsPerNodeForOutDistr, subjectProbOrSize, objectProbOrSize);
			return;
		} else {
			// Change out-distr
			changeDistributionParams(edgeType.outgoing_distrib, meanICsPerNodeForInDistr, objectProbOrSize, subjectProbOrSize);
			return;
		}
	}

	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000);
		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000);
		if (meanICsPerNodeForOutDistr * subjectProbOrSize >
			meanICsPerNodeForInDistr * objectProbOrSize) {
			// Change in-distr
			double newMean = (subjectProbOrSize * meanICsPerNodeForOutDistr) / objectProbOrSize;
			edgeType.incoming_distrib.arg1 = ceil(newMean - meanICsPerNodeForInDistr);
//			cout << "Change the ZipfianIn start value to: " << zipfianStartValueIn << endl;
			return;
		} else {
			// Change out-distr
			double newMean = (objectProbOrSize * meanICsPerNodeForInDistr) / subjectProbOrSize;
			edgeType.outgoing_distrib.arg1 = ceil(newMean - meanICsPerNodeForOutDistr);
//			cout << "Change the ZipfianOut start value to: " << zipfianStartValueOut << endl;
			return;
		}
	}
}

//int incrementalDeterministicGraphGenerator::getNumberOfOpenICs(vector<graphNode*> nodes) {
//	int openICs = 0;
//	for(int i=0; i<nodes.size(); i++) {
//		openICs += (*nodes.at(i)).getNumberOfOpenInterfaceConnections();
//	}
//	return openICs;
//}

//int incrementalDeterministicGraphGenerator::getNumberOfEdgesPerIteration(config::edge edgeType, vector<graphNode*> subjectNodesWithOpenICs, vector<graphNode*> objectNodesWithOpenICs, int iterationNumber) {
//	int scale = 5;
//
//	int subjects;
//	if (conf.types.at(edgeType.subject_type).scalable) {
//		subjects = round(max(1.0, conf.types.at(edgeType.subject_type).proportion / conf.types.at(edgeType.object_type).proportion)) * iterationNumber;
//	} else {
//		subjects = conf.types.at(edgeType.subject_type).size;
//	}
//
//	int objects;
//	if (conf.types.at(edgeType.object_type).scalable) {
//		objects = round(max(1.0, conf.types.at(edgeType.object_type).proportion / conf.types.at(edgeType.subject_type).proportion)) * iterationNumber;
//	} else {
//		objects = conf.types.at(edgeType.object_type).size;
//	}
//
//	int c = round(max(getMeanICsPerNode(edgeType.outgoing_distrib, subjects), getMeanICsPerNode(edgeType.incoming_distrib, objects)));
//
//	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
//		c = 1;
//	}
//	int openICs = min(getNumberOfOpenICs(subjectNodesWithOpenICs), getNumberOfOpenICs(objectNodesWithOpenICs));
////	cout << "OpenConnections: "  << openICs << endl;
////	cout << "c: "  << c << endl;
//	return openICs - (c*scale);
//}
//
//vector<graphNode*> getNodesWithAtLeastOneOpenIC(vector<graphNode> & nodes) {
//	vector<graphNode*> nodesWithOpenICs;
//	for (graphNode & node: nodes) {
//		if (node.getNumberOfOpenInterfaceConnections() > 0) {
//			nodesWithOpenICs.push_back(&node);
//		}
//	}
//	return nodesWithOpenICs;
//}

int incrementalDeterministicGraphGenerator::getDistributionRandomnessTradeoff(config::edge edgeType, int iterationNumber) {
	int scale = 5;
	int c = 1;

	if ((conf.types.at(edgeType.subject_type).scalable && conf.types.at(edgeType.object_type).scalable)
			|| (!conf.types.at(edgeType.subject_type).scalable && !conf.types.at(edgeType.object_type).scalable)) {
		int subjects;
		if (conf.types.at(edgeType.subject_type).scalable) {
			subjects = round(max(1.0, conf.types.at(edgeType.subject_type).proportion / conf.types.at(edgeType.object_type).proportion)) * iterationNumber;
		} else {
			subjects = conf.types.at(edgeType.subject_type).size;
		}

		int objects;
		if (conf.types.at(edgeType.object_type).scalable) {
			objects = round(max(1.0, conf.types.at(edgeType.object_type).proportion / conf.types.at(edgeType.subject_type).proportion)) * iterationNumber;
		} else {
			objects = conf.types.at(edgeType.object_type).size;
		}

		c = round(max(getMeanICsPerNode(edgeType.outgoing_distrib, subjects), getMeanICsPerNode(edgeType.incoming_distrib, objects)));
	}

	return max(scale, c*scale);
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

//vector<graphNode*> incrementalDeterministicGraphGenerator::constructNodesVectorAndRemoveNodeWithZeroICs(vector<graphNode> &nodes_) {
//	vector<graphNode*> nodesVector;
//	int size = nodes_.size();
////	cout << "Vector: [";
//	for (int i=0; i<size; i++) {
//		int ics = nodes_[i].getNumberOfOpenInterfaceConnections();
////		cout << ics << endl;
//		if (ics == 0 && nodes_.size() > 0) {
//			graphNode last = nodes_.back();
////			cout << "i=" << i << endl;
////			cout << "size: " << nodes_.size() << endl;
////			cout << "Before: " << nodes_.at(i).iterationId << " with "<< nodes_.at(i).getNumberOfOpenInterfaceConnections() << endl;
//			nodes_.at(i) = last;
////			cout << "After: " << nodes_.at(i).iterationId << " with "<< nodes_.at(i).getNumberOfOpenInterfaceConnections() << endl;
//			nodes_.pop_back();
//			size--;
//		} else {
//			for (int i=0; i<ics; i++) {
//				nodesVector.push_back(&nodes_[i]);
////				cout << n.iterationId << " ";
//			}
//		}
//	}
////	cout << "]" << endl;
//	return nodesVector;
//}


void incrementalDeterministicGraphGenerator::processIteration(config::edge & edgeType) {
//	if (iterationNumber % 1000 == 0) {
//		cout <<÷ endl<< "---Process interationNumber " << to_string(iterationNumber) << " of edgeType " << to_string(edgeType.edge_type_id) << "---" << endl;
//	}
	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	nodeGen.addSubjectNodes(edgeType);
	nodeGen.addObjectNodes(edgeType);
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>( end - start ).count();
	timeForNodeGen += duration;

//	cout << "Subjects: " << nodes.first.size() << endl;
//	cout << "Objects: " << nodes.second.size() << endl;
//	for (graphNode n: nodes.first) {
//		cout << n.iterationId << " openICs: " << n.getNumberOfOpenInterfaceConnections() << endl;
//		cout << n.iterationId << " ICs: " << n.getNumberOfInterfaceConnections() << endl;
//	}

	start = chrono::high_resolution_clock::now();
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
		updateInterfaceConnectionsForZipfianDistributions(&nodes.first, edgeType.outgoing_distrib);
	}
	if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		updateInterfaceConnectionsForZipfianDistributions(&nodes.second, edgeType.incoming_distrib);
	}
	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
		updateICsForNonScalableType(edgeType);
	}

	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::milliseconds>( end - start ).count();
	timeForUpdating += duration;


	start = chrono::high_resolution_clock::now();
	vector<int> subjectNodeIdVector = constructNodesVector(nodes.first);
	vector<int> objectNodeIdVector = constructNodesVector(nodes.second);
	shuffle(objectNodeIdVector.begin(), objectNodeIdVector.end(), randomGenerator);
	shuffle(subjectNodeIdVector.begin(), subjectNodeIdVector.end(), randomGenerator);
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::milliseconds>( end - start ).count();
	timeForShuffling += duration;


	start = chrono::high_resolution_clock::now();
	for (int i=0; i<min(subjectNodeIdVector.size(), objectNodeIdVector.size()); i++) {
		double randomValue = uniformDistr(randomGenerator);
		if (randomValue > 0.03 || (conf.types[edgeType.subject_type].scalable ^ conf.types[edgeType.object_type].scalable)) {
//			cout << "Add edge: " << subjectNodeIdVector[i]->iterationId << " - " << objectNodeIdVector[i]->iterationId << endl;
			addEdge(nodes.first[subjectNodeIdVector[i]], nodes.second[objectNodeIdVector[i]], edgeType.predicate);
		}
	}
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::milliseconds>( end - start ).count();
	timeForAddingEdges += duration;
//	timeForIteration += duration;

}

int incrementalDeterministicGraphGenerator::processEdgeType(config::config configuration, config::edge & edgeType, ofstream & outputFile, bool printZipfPos) {
//	cout << endl << endl << "-----Processing edge-type " << to_string(edgeType.edge_type_id) << "-----" << endl;

	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	this->conf = configuration;
//	cout << "Number of nodes: " << conf.nb_nodes << endl;

	changeDistributionParams(edgeType);
	nodeGen = nodeGenerator(edgeType, nodes.first.size(), nodes.second.size(), &randomGenerator, &nodes, &conf);



	processIteration(edgeType);

	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
	auto durationWitoutMaterialize = chrono::duration_cast<chrono::milliseconds>( end - start ).count();

	// Materialize the edge
//	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
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
//	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
//	auto durationForMaterialize = chrono::duration_cast<chrono::milliseconds>( end - start ).count();

//	cout << "Time for node gen: " << timeForNodeGen << endl;
//	cout << "Time for updating: " << timeForUpdating << endl;
//	cout << "Time for shuffling: " << timeForShuffling << endl;
//	cout << "Time for adding edges: " << timeForAddingEdges << endl;
//	cout << "Time for materialization: " << durationForMaterialize << endl;
//	cout << "Time for iteration: " << timeForIteration << endl;

	if (printZipfPos && edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
//		cout << "Print out-distr pos" << endl;
		ofstream zipfPosOutDistrFile;
		zipfPosOutDistrFile.open("zipfPosSmallestGraphOutDistr" + to_string(edgeType.edge_type_id) + ".txt", ios::trunc);
		for (graphNode node: nodes.first) {
			zipfPosOutDistrFile << node.id << " = " << node.getPosition() << endl;
		}
		zipfPosOutDistrFile.close();
	}
	if (printZipfPos && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
//		cout << "Print in-distr pos" << endl;
		ofstream zipfPosInDistrFile;
		zipfPosInDistrFile.open("zipfPosSmallestGraphInDistr" + to_string(edgeType.edge_type_id) + ".txt", ios::trunc);
		for (graphNode node: nodes.second) {
			zipfPosInDistrFile << node.id << " = " << node.getPosition() << endl;
		}
		zipfPosInDistrFile.close();
	}


	return durationWitoutMaterialize;
}


} /* namespace std */
