#pragma once

/// TODO: complete this implementation of a thread-safe (concurrent) AVL
///       tree of integers, implemented as a set of Node objects.
///       In addition, the API now allows for multiple insertions on each
///       operation.
///       Less than goes to the left. Greater than goes to the right.
class tree
{
//This is for testing purposes. Do not access your Node directly.
public:
	struct Node
	{
	public:
		int value;
		Node* left;
		Node* right;
	};
	Node* head;


	mutable std::mutex tree_mutex;

public:
	tree(int)
	: head(NULL)
	{}

	/// insert /num/ values from /data/ array into the tree, and return the
	/// success/failure of each insert in /results/ array.
	void insert(int* data, bool* results, int num)
	{
		tree_mutex.lock();
		for(int x=0; x<num; x++){
			bool ret = false;
			int key = data[x];

			struct Node *toinsert;
			toinsert = (struct Node*) malloc(sizeof(struct Node));
			toinsert -> value = key;
			toinsert -> left = NULL;
			toinsert -> right = NULL;
		
			if(!head){
				head = toinsert;
				ret = true;
			}
			else{
				Node* temp = head;
				while(temp){
					//value already exists
					if(temp->value == key){
						ret = false;
						break;
					}
					//key is less than, go left
					else if(key < temp->value){
						if(temp->left){
							temp = temp->left;
						}
						else{
							temp->left = toinsert;
							ret = true;
							break;
						}
					}
					//key is greter than, go right
					else{
						if(temp->right){
							temp = temp->right;
						}
						else{
							temp->right = toinsert;
							ret = true;
							break;
						}
					}
				}
				results[x] = ret;
			}
		}
		tree_mutex.unlock();
	}

	/// remove *data* from the list if it was present; return true if the data
	/// was removed successfully.
	void remove(int* data, bool* results, int num)
	{
		tree_mutex.lock();
		for(int x=0; x<num; x++){
			bool ret = false;
			int key = data[x];

			//case where we are removing the head
			if(head->value == key){
				//find parent of min value in right subtree to replace head
				Node* newheadpar = head -> right;
				while(newheadpar -> left -> left){
					newheadpar = newheadpar -> left;
				}
				//case where new head has no right child
				if(!newheadpar -> left -> right){
					head -> value = newheadpar -> left -> value;
					newheadpar -> left = NULL;
					ret = true;
				}
				//case where new head has a right child
				else{
					head -> value = newheadpar -> left -> value;
					newheadpar -> left = newheadpar -> left -> right;
					ret = true;
				}
			}
			
			//case where node to remove is internal
			else{
				Node* temp = head;
				//make temp the parent of the node to remove
				while(temp){
					
					//temp has left and right children
					if(temp -> left && temp -> right){
						if(key < temp->value){
							if(temp -> left -> value == key){
								//REMOVE LOGIC
								ret = true;
								break;
							}
							else{
								temp = temp -> left;
							}
						}
						else{
							if(temp -> right -> value == key){
								//REMOVE LOGIC
								ret = true;
								break;
							}
							else{
								temp = temp -> right;
							}
						}
					}
					
					//temp only has a left child
					else if(temp -> left){
						if(key < temp->value){
							if(temp -> left -> value == key){
								//REMOVE LOGIC
								ret = true;
								break;
							}
							else{
								temp = temp -> left;
							}
						}
						//key to be removed does not exist
						else{
							ret = false;
							break;
						}
					}
					
					//temp only has a right child
					else{
						if(key > temp->value){
							if(temp -> right -> value == key){
								//REMOVE LOGIC
								ret = true;
								break;
							}
							else{
								temp = temp -> right;
							}
						}
						//key to be removed does not exist
						else{
							ret = false;
							break;
						}
					}
				}
			}

			
			

			results[x] = ret;
		}

		tree_mutex.unlock();

	}
	/// return true if *data* is present in the list, false otherwise
	void lookup(int* data, bool* results, int num) const
	{}

	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.

	//Total number of elements in the tree
	size_t getSize() const
	{
		return 0;
	}

	int getElement(size_t idx) const
	{
		return getElement(0, head);
	}


	//These functions need to exist, they do not need to do anyting
	size_t getBucketSize() const
	{
		return 0;
	}
	int getElement(size_t bucket, size_t idx) const
	{
		return 0;
	}



private:
	//Returns the idx'th element from the tree assuming an inorder traversal.
	int getElement(size_t idx, Node* at) const
	{
		return 0;
	}
};
