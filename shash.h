//#include "sentinel.h"

#pragma once

struct Node
	{
		int value;
		Node* next;
	};


class sentinel
{
	mutable std::mutex sentinel_mutex;
	mutable Node* head;
	

public:
	int size = 0;

	sentinel() : head(NULL)
	{}
	
	bool insert(int key){
		sentinel_mutex.lock();
			//initialization stuff
			bool ret = false;
			struct Node *toinsert;
			toinsert = (struct Node*) malloc(sizeof(struct Node));
			toinsert->value = key;
			toinsert->next = NULL;

			if(head){
				if(head -> value == key){
					free(toinsert);
					ret = false;
				}
					//toinsert becomes head
				else if(head -> value > key){
					toinsert -> next = head;
					head = toinsert;
					ret = true;
					size++;
				}
				else{
					Node* temp = head;
					//find toinsert's place
					while(temp -> value < key){
						if(temp -> next && temp -> next -> value < key){
							temp = temp -> next;
							continue;
						}
						else if(temp -> next && temp -> next -> value == key){
							free(toinsert);
							ret = false;
							break;
						}
						else if(temp -> next && temp -> next -> value > key){
							toinsert -> next = temp -> next;
							temp -> next = toinsert;
							ret = true;
							size++;
							break;
						}
						else{
							temp -> next = toinsert;
							ret = true;
							size++;
							break;
						}
						temp = temp -> next;
					}
				}
			}
			else{
				head = toinsert;
				ret = true;
				size++;
			}
			//print list after insert
			// cout << "insert " << key << " result:" << endl;
			// Node* temp = head;
			// while(temp){
			// 	cout << temp->value << "->";
			// 	temp = temp -> next;
			// }
			// cout << endl;

		sentinel_mutex.unlock();
		return ret;
	}

	bool remove(int key){
		sentinel_mutex.lock();
			Node* temp = head;
			Node* prev = NULL;
			while(temp != NULL){
				if(temp->value == key && prev ==NULL){
					head = temp->next;
					size--;
					sentinel_mutex.unlock();
					return true;
				}
				else if (temp->value == key && prev != NULL){
					Node* next = temp->next;
					prev->next = next;
					size--;
					sentinel_mutex.unlock();
					return true;
				}
				prev = temp;
				temp = temp->next;
			}

			//print list after remove
			// cout << "remove " << key << " result:" << endl;
			// temp = head;
			// while(temp){
			// 	cout << temp->value << "->";
			// 	temp = temp -> next;
			// }
			// cout << endl;

		sentinel_mutex.unlock();
		return false;
	}

	bool lookup(int key) const{
		sentinel_mutex.lock();
			bool ret = false;
		
			Node* temp = head;
			Node* prev = NULL;
			while(temp){
				if(temp->value == key && prev == NULL){
					sentinel_mutex.unlock();
					return true;
				}
				else if(temp->value == key && prev != NULL){
					sentinel_mutex.unlock();
					return true;
				}
				prev = temp;
				temp = temp -> next;
			}
		sentinel_mutex.unlock();
		return false;
	}

	int getElement(size_t idx) const
	{
		struct Node* currHead = head;
		int i;
		for(i=0; i < idx; i++){
			currHead = currHead->next;
		}
		return currHead->value;
	}
	
	
};





/// TODO: complete this implementation of a thread-safe (concurrent) hash
///       table of integers, implemented as an array of linked lists.  In
///       this implementation, each list should have a "sentinel" node that
///       contains the lock, so we can't just reuse the clist implementation
class shash
{
	std::vector<sentinel> buckets;
	int numBuckets = 0;

public:
	shash(unsigned _buckets) : buckets(_buckets)
	{
		numBuckets = (int)_buckets;
	}

	int hash(int key) const{
		return key % numBuckets;
	}


	/// insert *key* into the appropriate linked list if it doesn't already
	/// exist; return true if the key was added successfully.
	bool insert(int key)
	{
		int bucket = hash(key);
		return buckets[bucket].insert(key);
	}
	/// remove *key* from the appropriate list if it was present; return true
	/// if the key was removed successfully.
	bool remove(int key)
	{
		int bucket = hash(key);
		return buckets[bucket].remove(key);
	}
	/// return true if *key* is present in the appropriate list, false
	/// otherwise
	bool lookup(int key) const
	{
		int bucket = hash(key);
		return buckets[bucket].lookup(key);
	}

	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.

	//This refers to the number of buckets not the total number of elements.
	size_t getSize() const
	{
		return numBuckets;
	}

	//This refers to the number of elements in a bucket, not the sentinel node.
	size_t getBucketSize(size_t bucket) const
	{
		return buckets[bucket].size;
	}
	int getElement(size_t bucket, size_t idx) const
	{
		return buckets[bucket].getElement(idx);
	}


	//These functions just need to exist, they do not need to do anything
	int getElement(size_t idx) const
	{
		return 0;
	}
};


