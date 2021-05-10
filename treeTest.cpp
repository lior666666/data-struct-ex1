
#include "avl_tree.h"

/*
 * testAVLTree.cpp
 *
 *  Created on: 19 áàôø 2013
 *      Author: shirel
 */

#include <vector>
#include <ostream>
#include <random>
#include <chrono>
#include <algorithm>

int main(){
	AvlTree<int> tree;

	//Specific Cases

	/*correct output for the four next cases
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0 */

	//basic LL root change
	tree.insertElement(3);
	tree.insertElement(2);
	tree.insertElement(1);
	tree.printTree();
	tree.clearTree();
    
	//basic LR root change
	tree.insertElement(3);
	tree.insertElement(1);
	tree.insertElement(2);
	tree.printTree();
	tree.clearTree();

	//basic RR root change
	tree.insertElement(1);
	tree.insertElement(2);
	tree.insertElement(3);
	tree.printTree();
	tree.clearTree();

	//basic RL root change
	tree.insertElement(1);
	tree.insertElement(3);
	tree.insertElement(2);
	tree.printTree();
	tree.clearTree();

	//basic LR not root change
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	4 BF: 1 Height: 2
	5 BF: 0 Height: 0
	6 BF: 1 Height: 3
	7 BF: 0 Height: 0
	8 BF: 1 Height: 1 */
	tree.insertElement(6);
	tree.insertElement(4);
	tree.insertElement(8);
	tree.insertElement(3);
	tree.insertElement(5);
	tree.insertElement(7);
	tree.insertElement(1);
	tree.insertElement(2);
	tree.printTree();
	tree.clearTree();

	//basic LL not root change
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	4 BF: 1 Height: 2
	5 BF: 0 Height: 0
	6 BF: 1 Height: 3
	7 BF: 0 Height: 0
	8 BF: 1 Height: 1 */
	tree.insertElement(6);
	tree.insertElement(4);
	tree.insertElement(8);
	tree.insertElement(3);
	tree.insertElement(5);
	tree.insertElement(7);
	tree.insertElement(2);
	tree.insertElement(1);
	tree.printTree();
	tree.clearTree();

	//basic RR not root change
	/*correct output
	 * 1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	5 BF: 1 Height: 2
	7 BF: 0 Height: 0*/
	tree.insertElement(5);
	tree.insertElement(7);
	tree.insertElement(1);
	tree.insertElement(2);
	tree.insertElement(3);
	tree.printTree();
	tree.clearTree();

	//basic RL not root change
	/*correct output
	 * 1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	5 BF: 1 Height: 2
	7 BF: 0 Height: 0*/
	tree.insertElement(5);
	tree.insertElement(7);
	tree.insertElement(1);
	tree.insertElement(3);
	tree.insertElement(2);
	tree.printTree();
	tree.clearTree();

	//root deletion no roll successor is a neighbour
	/*correct output
	2 BF: 0 Height: 0
	3 BF: 1 Height: 1
	5 BF: 1 Height: 2
	6 BF: 0 Height: 0*/
	tree.insertElement(4);
	tree.insertElement(5);
	tree.insertElement(3);
	tree.insertElement(2);
	tree.insertElement(6);
	tree.removeElement(4);
	tree.printTree();
	tree.clearTree();

	//root deletion no roll successor is not a neighbour
	/*correct output
	2 BF: 0 Height: 0
	3 BF: 1 Height: 1
	5 BF: 0 Height: 2
	6 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0 */
	tree.insertElement(4);
	tree.insertElement(3);
	tree.insertElement(7);
	tree.insertElement(2);
	tree.insertElement(5);
	tree.insertElement(8);
	tree.insertElement(6);
	tree.removeElement(4);
	tree.printTree();
	tree.clearTree();

	//node deletion no roll successor is a neighbour case7
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 1 Height: 1
	4 BF: 1 Height: 2
	5 BF: 0 Height: 0
	7 BF: 1 Height: 3
	8 BF: -1 Height: 1
	9 BF: 0 Height: 0*/
	tree.insertElement(7);
	tree.insertElement(3);
	tree.insertElement(8);
	tree.insertElement(2);
	tree.insertElement(4);
	tree.insertElement(9);
	tree.insertElement(5);
	tree.insertElement(1);
	tree.removeElement(3);
	tree.printTree();
	tree.clearTree();

	//node deletion no roll successor is not a neighbour case8
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 1 Height: 1
	5 BF: 0 Height: 2
	6 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0
	9 BF: 0 Height: 3
	10 BF: 0 Height: 0
	12 BF: -1 Height: 2
	13 BF: -1 Height: 1
	14 BF: 0 Height: 0*/
	tree.insertElement(9);
	tree.insertElement(3);
	tree.insertElement(12);
	tree.insertElement(2);
	tree.insertElement(7);
	tree.insertElement(10);
	tree.insertElement(13);
	tree.insertElement(1);
	tree.insertElement(5);
	tree.insertElement(8);
	tree.insertElement(14);
	tree.insertElement(6);
	tree.removeElement(3);
	tree.printTree();
	tree.clearTree();

	//node deletion causing LR case9
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	4 BF: 0 Height: 2
	7 BF: -1 Height: 1
	8 BF: 0 Height: 0*/
	tree.insertElement(7);
	tree.insertElement(2);
	tree.insertElement(8);
	tree.insertElement(1);
	tree.insertElement(4);
	tree.insertElement(9);
	tree.insertElement(3);
	tree.removeElement(9);
	tree.printTree();
	tree.clearTree();

	//node deletion causing LL case10
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 1 Height: 1
	3 BF: 0 Height: 2
	4 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0*/
	tree.insertElement(7);
	tree.insertElement(3);
	tree.insertElement(8);
	tree.insertElement(2);
	tree.insertElement(4);
	tree.insertElement(9);
	tree.insertElement(1);
	tree.removeElement(9);
	tree.printTree();
	tree.clearTree();

	//node deletion causing RR case11
	/*correct output
	2 BF: 0 Height: 0
	3 BF: 0 Height: 1
	7 BF: 0 Height: 0
	8 BF: 0 Height: 2
	9 BF: -1 Height: 1
	10 BF: 0 Height: 0*/
	tree.insertElement(3);
	tree.insertElement(2);
	tree.insertElement(8);
	tree.insertElement(7);
	tree.insertElement(1);
	tree.insertElement(9);
	tree.insertElement(10);
	tree.removeElement(1);
	tree.printTree();
	tree.clearTree();

	//node deletion causing RL case12
	/*correct output
	* 2 BF: 0 Height: 0
	3 BF: 1 Height: 1
	7 BF: 0 Height: 2
	10 BF: 0 Height: 0
	13 BF: 0 Height: 1
	14 BF: 0 Height: 0*/
	tree.insertElement(3);
	tree.insertElement(2);
	tree.insertElement(13);
	tree.insertElement(7);
	tree.insertElement(1);
	tree.insertElement(14);
	tree.insertElement(10);
	tree.removeElement(1);
	tree.printTree();
	tree.clearTree();

	//double rotations RL and RR case 13
	/*correct output
	3 BF: 0 Height: 0
	5 BF: 0 Height: 1
	7 BF: 0 Height: 0
	9 BF: 0 Height: 2
	11 BF: 0 Height: 0
	13 BF: 1 Height: 1
	15 BF: 0 Height: 3
	17 BF: 0 Height: 0
	19 BF: -1 Height: 2
	21 BF: -1 Height: 1
	23 BF: 0 Height: 0*/
	tree.insertElement(9);
	tree.insertElement(3);
	tree.insertElement(15);
	tree.insertElement(1);
	tree.insertElement(7);
	tree.insertElement(13);
	tree.insertElement(19);
	tree.insertElement(5);
	tree.insertElement(11);
	tree.insertElement(17);
	tree.insertElement(21);
	tree.insertElement(23);
	tree.removeElement(1);
	tree.printTree();
	tree.clearTree();

	//double rotations RR and RR case 14
	/*correct output
	3 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0
	9 BF: 0 Height: 2
	11 BF: 0 Height: 0
	13 BF: 1 Height: 1
	15 BF: 0 Height: 3
	17 BF: 0 Height: 0
	19 BF: -1 Height: 2
	21 BF: -1 Height: 1
	23 BF: 0 Height: 0*/
	tree.insertElement(9);
	tree.insertElement(3);
	tree.insertElement(15);
	tree.insertElement(1);
	tree.insertElement(7);
	tree.insertElement(13);
	tree.insertElement(19);
	tree.insertElement(8);
	tree.insertElement(11);
	tree.insertElement(17);
	tree.insertElement(21);
	tree.insertElement(23);
	tree.removeElement(1);
	tree.printTree();
	tree.clearTree();

	//double rotations RL and LL case 15
	/*correct output
	6 BF: 0 Height: 0
	7 BF: 1 Height: 1
	8 BF: 1 Height: 2
	9 BF: 0 Height: 0
	10 BF: 0 Height: 3
	11 BF: -1 Height: 1
	12 BF: 0 Height: 0
	15 BF: 0 Height: 2
	17 BF: 0 Height: 0
	18 BF: 0 Height: 1
	20 BF: 0 Height: 0
	 */
	tree.insertElement(15);
	tree.insertElement(10);
	tree.insertElement(20);
	tree.insertElement(8);
	tree.insertElement(11);
	tree.insertElement(17);
	tree.insertElement(21);
	tree.insertElement(7);
	tree.insertElement(9);
	tree.insertElement(12);
	tree.insertElement(18);
	tree.insertElement(6);
	tree.removeElement(21);
	tree.printTree();
	tree.clearTree();

	//double rotations LR and LL case 16
	/*correct output
	6 BF: 0 Height: 0
	7 BF: 1 Height: 1
	8 BF: 1 Height: 2
	9 BF: 0 Height: 0
	10 BF: 0 Height: 3
	11 BF: -1 Height: 1
	12 BF: 0 Height: 0
	15 BF: 0 Height: 2
	20 BF: 0 Height: 0
	21 BF: 0 Height: 1
	22 BF: 0 Height: 0 */
	tree.insertElement(15);
	tree.insertElement(10);
	tree.insertElement(20);
	tree.insertElement(8);
	tree.insertElement(11);
	tree.insertElement(17);
	tree.insertElement(22);
	tree.insertElement(7);
	tree.insertElement(9);
	tree.insertElement(12);
	tree.insertElement(21);
	tree.insertElement(6);
	tree.removeElement(17);
	tree.printTree();
	tree.clearTree();

	//delete node cause LR
	/*correct output
	 * 2 BF: 0 Height: 0
	3 BF: -1 Height: 2
	4 BF: 0 Height: 0
	6 BF: 1 Height: 1*/

	tree.insertElement(5);
	tree.insertElement(3);
	tree.insertElement(6);
	tree.insertElement(2);
	tree.insertElement(4);
	tree.removeElement(5);
	tree.printTree();
	tree.clearTree();

	//delete node cause LR
	/*correct output
	 * 2 BF: 0 Height: 0
	3 BF: 0 Height: 1
	6 BF: 0 Height: 0*/
	tree.insertElement(5);
	tree.insertElement(3);
	tree.insertElement(6);
	tree.insertElement(2);
	tree.removeElement(5);
	tree.printTree();
	tree.clearTree();

	// std::vector<int> vector;
	//  for (int i=1; i<=100; i++) vector.push_back(i);

	//  //Randomly insert and removes nodes
	//  for (int k = 0; k < 20; ++k) {
	// 	 unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	// 	 shuffle (vector.begin(), vector.end(), std::default_random_engine(seed));
	// 	 for (std::vector<int>::iterator it = vector.begin() ; it != vector.end(); ++it){
	// 			tree.insertElement(*it,*it);

	// 	 }
	// 	 tree.printTree();
	// 	 shuffle (vector.begin(), vector.end(), std::default_random_engine(seed));
	// 	 for (std::vector<int>::iterator it = vector.begin() ; it != vector.end(); ++it){
	// 			tree.removeElement(*it);
	// 			tree.printTree();

	// 	 }
	// 	 tree.clearTree();
	// 	 tree.printTree();
	// 	 std::cout << '\n';
	// }

	return 0;
}

