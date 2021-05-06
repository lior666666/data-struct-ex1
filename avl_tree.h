#ifndef AVL_TREE_H_
#define AVL_TREE_H_
#include <iostream>
template<class T>
class AvlTree
    {
        T data;
        AvlTree<T>* right;
        AvlTree<T>* left;
        AvlTree<T>* next;
        AvlTree<T>* parent;
        int node_height; 

        void cleanTree(AvlTree<T>* node)
        {
            if(node == NULL)
                return; 
            this->cleanTree(node->left);
            this->cleanTree(node->right);   
            delete node; 
        }

        int getBalanceFactor(AvlTree<T>* tree_node)
        {
            int left_hight;
            int right_hight;
            if(tree_node->left!=NULL)
               left_hight = tree_node->left->node_height; 
            else
               left_hight = 0;    
            if(tree_node->right!=NULL)
               right_hight = tree_node->right->node_height; 
            else
                right_hight = 0; 

            return  left_hight - right_hight;
        }

        void updateHeight()
        {
            int left_hight;
            int right_hight;
            if(this->left!=NULL)
               left_hight = this->left->node_height; 
            else
               left_hight = 0;    
            if(this->right!=NULL)
               right_hight = this->right->node_height; 
            else
                right_hight = 0; 

            if(left_hight > right_hight)
                this->node_height = left_hight + 1;
            else
               this->node_height = right_hight + 1; 
        }

        AvlTree<T>* rotateRight()
        {
            AvlTree<T>* left_to_top_pointer = this->left; 
            AvlTree<T>* right_to_left_pointer = left_to_top_pointer->right;

            left_to_top_pointer->right = this;
            this->left = right_to_left_pointer;
            this-> updateHeight();
            left_to_top_pointer-> updateHeight();
            return left_to_top_pointer;
        }

        AvlTree<T>* rotateLeft()
        {
            AvlTree<T>* right_to_top_pointer = this->right; 
            AvlTree<T>* left_to_right_pointer = right_to_top_pointer->left;

            right_to_top_pointer->left = this;
            this->right = left_to_right_pointer;
            this-> updateHeight();
            right_to_top_pointer-> updateHeight();
            return right_to_top_pointer;
        }
        
        AvlTree<T>* findMostLeftNode(AvlTree<T>* tree_node)
        {
            if(tree_node->left == NULL)
                return tree_node;
            return findMostLeftNode(tree_node->left);
        }

        AvlTree<T>* insert(T data, AvlTree<T>* tree_node)
        {
          
            // if(data < this->data)
            //     {
            //         if(this->left!=NULL)
            //             this->left = this->left->insert(data);
            //         else // make another func, duplicate code. 
            //         {
            //             AvlTree<T>* newTreeNode = new AvlTree<T>;
            //             newTreeNode->data = data; 
            //             newTreeNode->left = NULL;
            //             newTreeNode->right = NULL;
            //             newTreeNode->next = NULL;
            //             newTreeNode->parent = this; 
            //             newTreeNode->node_height = 1; // get back to this line 
            //             return newTreeNode; 
            //         }    
            //     }
            // else if(data > this->data)
            //     {
            //         if(this->right!=NULL)
            //             this->right = this->right->insert(data);
            //        else // make another func, duplicate code. 
            //         {
            //             AvlTree<T>* newTreeNode = new AvlTree<T>;
            //             newTreeNode->data = data; 
            //             newTreeNode->left = NULL;
            //             newTreeNode->right = NULL;
            //             newTreeNode->next = NULL;
            //             newTreeNode->parent = this;
            //             newTreeNode->node_height = 1; // get back to this line 
            //             return newTreeNode; 
            //         }     
            //     }


            if(tree_node == NULL)
             {
                 AvlTree<T>* newTreeNode = new AvlTree<T>;
                        newTreeNode->data = data; 
                        newTreeNode->left = NULL;
                        newTreeNode->right = NULL;
                        newTreeNode->next = NULL;
                        newTreeNode->parent = this; 
                        newTreeNode->node_height = 1; // get back to this line 
                        return newTreeNode; 
             }
             
            if(data < tree_node->data)
                tree_node->left = this->insert(data, tree_node->left);

            else if(data > tree_node->data)
                tree_node->right = this->insert(data, tree_node->right);
                

            tree_node->updateHeight();
                
                // LL
                if(getBalanceFactor(tree_node)>1 && data < tree_node->left->data)
                {
                    return tree_node->rotateRight();
                }
                //LR
                if(getBalanceFactor(tree_node)>1 && data > tree_node->left->data)
                {
                    tree_node->left = tree_node->left->rotateLeft();
                    return tree_node->rotateRight();
                }
                //RR
                if(getBalanceFactor(tree_node)<-1 && data > tree_node->right->data)
                {
                    return tree_node->rotateLeft();
                }
                //RL
                if(getBalanceFactor(tree_node)<-1 && data < tree_node->right->data)
                {
                    tree_node->right = tree_node->right->rotateRight();
                    return tree_node->rotateLeft();
                }
                
                return tree_node; 
        }
        AvlTree<T>* removeNode(T data, AvlTree<T>* tree_node)
        {
            if(tree_node == NULL)
            {
                return NULL; 
            }

            if(data < tree_node->data)
                tree_node->left = this->removeNode(data, tree_node->left);

            else if(data > tree_node->data)
                tree_node->right = this->removeNode(data, tree_node->right);
                
            else
            {
                if(tree_node->left == NULL || tree_node->right == NULL)
                {
                    AvlTree<T>* temp_node; 
                    if(tree_node->left != NULL)
                        temp_node = tree_node->left; 
                    else
                        temp_node = tree_node->right;      
                    if(temp_node == NULL)
                    // no child
                    {
                        temp_node = tree_node;
                        tree_node = NULL; 
                    }     
                    else
                    {
                        // one child 
                        // make copy node function!!
                        tree_node->data = temp_node->data;
                        tree_node->left = NULL;
                        tree_node->right = NULL;
                        tree_node->node_height = temp_node->node_height;
                    } 
                    delete temp_node;     
                }
                else
                {
                    AvlTree<T>* temp_node = findMostLeftNode(tree_node->right);
                    // make copy node function!!
                   tree_node->data = temp_node->data;
                   tree_node->right = removeNode(tree_node->data, tree_node->right); // this will delete the node that we just switched with our's.   
                     
                }
            }
            if(tree_node == NULL)
                return NULL; 

            tree_node->updateHeight();

            // LL
                if(getBalanceFactor(tree_node)>1 && getBalanceFactor(tree_node->left)>=0)
                {
                    return tree_node->rotateRight();
                }
                //LR
                if(getBalanceFactor(tree_node)>1 && getBalanceFactor(tree_node->left)<0)
                {
                    tree_node->left = tree_node->left->rotateLeft();
                    return tree_node->rotateRight();
                }
                //RR
                if(getBalanceFactor(tree_node)<-1 && getBalanceFactor(tree_node->right)<=0)
                {
                    return tree_node->rotateLeft();
                }
                //RL
                if(getBalanceFactor(tree_node)<-1 && getBalanceFactor(tree_node->right)>0)
                {
                    tree_node->right = tree_node->right->rotateRight();
                    return tree_node->rotateLeft();
                }

                return tree_node;

        }
        AvlTree<T>* findNode(T data, AvlTree<T>* tree_node)
        {
            if(tree_node ==NULL)
                return 0; // change to NULL; 
            else if(data == tree_node->data)
                return tree_node;
            else if (tree_node->data < data)   
                return findNode(data, tree_node->right);
            else  
                return findNode(data, tree_node->left);            
        }

        void printTr()
        {
            if(this->left!=NULL)
                this->left->printTr();
            std::cout << this->data << std::endl; 
            if(this->right!=NULL)
                this->right->printTr();     
        }


    public:

        AvlTree<T>()
        {
            left = NULL;
            right = NULL;
            parent = NULL; 
            next = NULL;
            node_height = 1; 
        }

       // ~AvlTree<T>() = default; 

        ~AvlTree<T>()
        {  
            this->cleanTree(this->next);   
        }
      
        void insertElement(T data)
        {
            if(this->next == NULL) // MAKE INTERNAL FUNC
            {
                AvlTree<T>* newTreeNode = new AvlTree<T>;
                newTreeNode->data = data; 
                newTreeNode->left = NULL;
                newTreeNode->right = NULL;
                newTreeNode->next = NULL;
                newTreeNode->parent = this;
                newTreeNode->node_height = 1; // get back to this line 
                this->next = newTreeNode; 
            }
            else
            {
                this->next = this->insert(data, this->next); 
            }    
        }

        int removeElement(T data)
        {
           AvlTree<T>* node = findNode(data, this->next);
           if(node ==NULL)
                return 0; 
           removeNode(data, this->next); 
           return 1; 
        }

        void printTree()
            {
                if(this->next!=NULL)
                {
                    this->next->printTr();
                }    
            }
        int getHeight()
        {
            return this->next->node_height; 
        }  

        T getNodeData(T data)
        {
            AvlTree<T>* node = findNode(data, this->next);  
            if(node == NULL)
                return 0;
            return node->data;      
        }  
    };        
#endif