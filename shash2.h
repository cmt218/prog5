#pragma once
using namespace std;
#include <mutex>

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
///       contains the lock, so we can't just reuse the clist implementation.
///       In addition, the API now allows for multiple keys on each
///       operation.
class shash2
{

	const std::vector<sentinel> buckets;
	int numBuckets = 0;

public:
	shash2(unsigned _buckets) : buckets((int)_buckets)
	{
		numBuckets = (int)_buckets;
	}

	int hash(int key) const{
		return key % numBuckets;
	}

	/// insert /num/ values from /keys/ array into the hash, and return the
	/// success/failure of each insert in /results/ array.
	void insert(int* keys, bool* results, int num)
	{
		
		for(int x=0;x<num;x++){
			int bucket = hash(keys[x]);

			buckets[bucket].sentinel_mutex.lock();
			int key = keys[x];
			bool ret = true;
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
				else if(buckets[bucket].head -> value > key){
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
							//cout << "CONTINUE CASE " << endl;
							continue;
						}
						// else if(temp -> next && temp -> next -> value == key){
						// 	free(toinsert);
						// 	ret = false;
						// 	cout << "FALSE CASE " << endl;
						// 	break;
						// }
						else if(temp -> next && temp -> next -> value > key){
							toinsert -> next = temp -> next;
							temp -> next = toinsert;
							ret = true;
							//cout << "TRUE CASE " << endl;
							//size++;
							break;
						}
						else{
							temp -> next = toinsert;
							ret = true;
							//cout << "TRUE CASE " << endl;
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

			results[x] = ret;

			buckets[bucket].sentinel_mutex.unlock();
		}
		

	}
	/// remove *key* from the list if it was present; return true if the key
	/// was removed successfully.
	void remove(int* keys, bool* results, int num)
	{

		for(int x=0;x<num;x++){
			int bucket = hash(keys[x]);
			buckets[bucket].sentinel_mutex.lock();
			int key = keys[x];

			bool ret = false;
			
			Node* temp = buckets[bucket].head;
			Node* prev = NULL;
			while(temp != NULL){
				if(temp->value == key && prev ==NULL){
					buckets[bucket].head = temp->next;
					//size--;
					ret = true;
					break;
				}
				else if (temp->value == key && prev != NULL){
					Node* next = temp->next;
					prev->next = next;
					//size--;
					//cout << "CASE 1 HAPPENED" << endl;

					ret = true;
					break;
				}
				prev = temp;
				temp = temp->next;
			}
			//print remove key result
			// cout << "remove " << key << " result:" << endl;
			// temp = buckets[bucket].head;
			// while(temp){
			// 	cout << temp->value << "->";
			// 	temp = temp -> next;
			// }
			// cout << endl;

			results[x] = ret;

			buckets[bucket].sentinel_mutex.unlock();
		}

		
	}
	/// return true if *key* is present in the list, false otherwise
	void lookup(int* keys, bool* results, int num) const
	{

		for(int x=0;x<num;x++){
			int bucket = hash(keys[x]);
			buckets[bucket].sentinel_mutex.lock();
			int key = keys[x];

			bool ret = false;
		
			Node* temp = buckets[bucket].head;
			Node* prev = NULL;
			while(temp){
				if(temp->value == key && prev == NULL){
					ret = true;
					break;
				}
				else if(temp->value == key && prev != NULL){
					ret = true;
					break;
				}
				prev = temp;
				temp = temp -> next;

				
			}

			results[x] = ret;
			buckets[bucket].sentinel_mutex.unlock();
		}


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
		
		size_t toReturn = 0;
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
