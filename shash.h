//#include "sentinel.h"

#pragma once

struct Node
	{
		int value;
		Node* next;
	};


class sentinel
{
	
public:

	mutable std::mutex sentinel_mutex;
	mutable Node* head;

	sentinel() : head(NULL)
	{}
	
};



/// TODO: complete this implementation of a thread-safe (concurrent) hash
///       table of integers, implemented as an array of linked lists.  In
///       this implementation, each list should have a "sentinel" node that
///       contains the lock, so we can't just reuse the clist implementation
class shash
{
	const std::vector<sentinel> buckets;
	int numBuckets = 0;

public:
	shash(unsigned _buckets) : buckets((int)_buckets)
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
		buckets[bucket].sentinel_mutex.lock();

		bool ret = false;
		struct Node *toinsert;
		toinsert = (struct Node*) malloc(sizeof(struct Node));

		toinsert -> value = key;
		toinsert -> next = NULL;
		if(buckets[bucket].head){
			if(buckets[bucket].head -> value == key){
				free(toinsert);
				ret = false;
			}
				//toinsert becomes head
			else if(buckets[bucket].head-> value > key){
				toinsert -> next = buckets[bucket].head;
				buckets[bucket].head = toinsert;
				ret = true;
				//size++;
			}
			else{
				Node* temp = buckets[bucket].head;
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
						//size++;
						break;
					}
					else{
						temp -> next = toinsert;
						ret = true;
						//size++;
						break;
					}
					temp = temp -> next;
				}
			}
		}
		else{
			buckets[bucket].head = toinsert;
			ret = true;
			//size++;
		}
			// //print list after insert
			// cout << "insert " << key << " result:" << endl;
			// Node* temp = buckets[bucket].head;
			// while(temp){
			// 	cout << temp->value << "->";
			// 	temp = temp -> next;
			// }
			// cout << endl;
		buckets[bucket].sentinel_mutex.unlock();


		return ret;
	}
	/// remove *key* from the appropriate list if it was present; return true
	/// if the key was removed successfully.
	bool remove(int key)
	{
		int bucket = hash(key);
		buckets[bucket].sentinel_mutex.lock();
		Node* temp = buckets[bucket].head;
		Node* prev = NULL;
		while(temp != NULL){
			if(temp->value == key && prev ==NULL){
				buckets[bucket].head = temp->next;
				//size--;
				buckets[bucket].sentinel_mutex.unlock();
				return true;
			}
			else if (temp->value == key && prev != NULL){
				Node* next = temp->next;
				prev->next = next;
				//size--;
				buckets[bucket].sentinel_mutex.unlock();
				return true;
			}
			prev = temp;
			temp = temp->next;
		}
		//print remove key result
		// cout << "remove " << key << " result:" << endl;
		// 	temp = buckets[bucket].head;
		// 	while(temp){
		// 		cout << temp->value << "->";
		// 		temp = temp -> next;
		// 	}
		// 	cout << endl;
		buckets[bucket].sentinel_mutex.unlock();
		return false;
	}
	/// return true if *key* is present in the appropriate list, false
	/// otherwise
	bool lookup(int key) const
	{
		int bucket = hash(key);
		buckets[bucket].sentinel_mutex.lock();
		Node* temp = buckets[bucket].head;
		Node* prev = NULL;
		while(temp){
			if(temp->value == key && prev == NULL){
				buckets[bucket].sentinel_mutex.unlock();
				return true;
			}
			else if(temp->value == key && prev != NULL){
				buckets[bucket].sentinel_mutex.unlock();
				return true;
			}
			prev = temp;
			temp = temp -> next;
		}
		buckets[bucket].sentinel_mutex.unlock();
		return false;
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
		int toReturn = 0;
		Node* temp = buckets[bucket].head;
		while(temp){
			toReturn ++;
			temp = temp->next;
		}

		return toReturn;
	}
	int getElement(size_t bucket, size_t idx) const
	{
		int x = idx;
		Node* temp = buckets[bucket].head;
		while(x>0){
			temp = temp->next;
			x--;
		}
		return temp->value;
	}


	//These functions just need to exist, they do not need to do anything
	int getElement(size_t idx) const
	{
		return 0;
	}
};


