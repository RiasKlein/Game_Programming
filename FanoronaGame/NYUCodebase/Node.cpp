#include "Node.h"

Node::Node() {}
Node::Node(std::vector<Chip*> boardConfiguration, Node* parent, bool AIisWhite) : boardConfiguration(boardConfiguration), parent(parent), AIisWhite(AIisWhite) {}

/*	This function tests this Node's board configuration with another Node's board configuration.
	It will return true if the two board configurations are the same.
*/
bool Node::sameConfiguration(Node* nodeToCompare) {
	//	First we check whether the two boardConfigurations are of the same size -- if not then return false
	if (boardConfiguration.size() != (nodeToCompare->boardConfiguration.size())) {
		return false;
	}

	bool searchresult = false;
	//	Now that we know that the two boards are of the same size, let's compare their board configurations
	for (unsigned int i = 0; i < boardConfiguration.size(); i++){
		for (unsigned int j = 0; j < (nodeToCompare->boardConfiguration.size()); j++) {
			if ( (boardConfiguration[i]->boardCol == nodeToCompare->boardConfiguration[j]->boardCol) && 
				 (boardConfiguration[i]->boardRow == nodeToCompare->boardConfiguration[j]->boardRow) &&
				 (boardConfiguration[i]->isWhite) == nodeToCompare->boardConfiguration[j]->isWhite) {
				//	Check if there's a chip that has the same color, column, and row on both boards - if so we found the chip

				searchresult = true;
				break;
			}
		}

		if (!searchresult) { // searchresult is false if there was a chip on one board that was NOT found on the other
			return false;
		}

		//	Make searchresult false to check the next chip
		searchresult = false;
	}

	// If we didn't return false up until this point that means the boards have the same configuration
	return true;
}



int Node::UTILITY(int turnCounter) {
	//	Returns the heuristic value of THIS node
	int utilityValue = 0;

	//	The heuristic utility score will look at:
	//	1. The number of friendly pieces left (+ score)
	//	2. The number of enemy pieces left (- score)
	//	3. The number of pieces you captured (+ score) from parent to this node
	//	4. The number of pieces the opponent captured (- score) from parent to this node
	//	5. If this is a victory state for YOU -> +100 points
	//	6. If this is a victory state for the OPPONENT -> -100 points

	int friendlyParent = 0;
	int enemyParent = 0;
	int friendlyCurrent = 0;
	int enemyCurrent = 0;

	//	Consider the friendly / enemy situation on the current board
	for (unsigned int i = 0; i < boardConfiguration.size(); i++) {
		if (boardConfiguration[i]->isWhite == AIisWhite) { // This is a friendly piece
			friendlyCurrent++;
		}
		else {
			enemyCurrent++;
		}
	}

	//	Consider the friendly / enemy situation on the parent board
	for (unsigned int i = 0; i < parent->boardConfiguration.size(); i++) {
		if (parent->boardConfiguration[i]->isWhite == AIisWhite) {
			friendlyParent++;
		}
		else {
			enemyParent++;
		}
	}

	//	Check if it's a victory state for the AI
	int victoryScore = 0;
	if (friendlyCurrent == 0) { // The AI has LOST!
		victoryScore = -100;
	}
	else if (enemyCurrent == 0) { // The enemy has LOST!
		victoryScore = 100;
	}

	utilityValue = friendlyCurrent - enemyCurrent + (enemyParent - enemyCurrent) - (friendlyParent - friendlyCurrent) + victoryScore;

	return utilityValue;
}