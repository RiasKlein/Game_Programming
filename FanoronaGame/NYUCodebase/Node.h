#pragma once
#include <vector>
#include "Chip.h"
#include <limits.h>

class Node {
public:
	Node();
	Node(std::vector<Chip*> boardConfiguration, Node* parent, bool AIisWhite);

	bool sameConfiguration(Node* nodeToCompare);	// Equals function to test for Node Equality
	int UTILITY(int turnCounter);					// Calculates the score for THIS node

	//	Each node should contain a board state (or a board configuration)
	std::vector<Chip*> boardConfiguration;

	int alpha = std::numeric_limits<int>::min();	// Initial value for Alpha (by default) is -INFINITY
	int beta = std::numeric_limits<int>::max();		// Initial value for Beta  (by default) is +INFINITY

	int nodeUTILITY = 0;

	//	Parent node and children nodes
	std::vector<Node*> children;	//	Vector of Nodes that are the children of this node
	Node* parent;					//	Parent of this node

	bool AIisWhite;					//	Whether the AI team is white or not

private:

};
