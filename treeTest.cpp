#include <iostream>
#include "avl_tree.h"

int main()
{
    AvlTree<int> tr = AvlTree<int>();
    tr.insertElement(5);
    tr.insertElement(7);
    tr.insertElement(2);
    tr.insertElement(1);
    tr.insertElement(4);
    tr.insertElement(6);
    tr.insertElement(9);
    tr.insertElement(3);
    tr.insertElement(16);
    tr.insertElement(15);
    int test = tr.removeElement(9);
    std::cout << test << std::endl; 
    tr.printTree();
    std::cout << tr.getHeight();


}