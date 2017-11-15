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
	Node* head;

public:
	sentinel() : head(NULL)
	{}
	
	bool test(Node* n){
		sentinel_mutex.lock();
			cout << n->value;
		sentinel_mutex.unlock();
		return false;
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
		struct Node *toinsert;
		toinsert = (struct Node*) malloc(sizeof(struct Node));
		toinsert->value = key;

		int bucket = hash(key);
		return buckets[bucket].test(toinsert);
	}
	/// remove *key* from the appropriate list if it was present; return true
	/// if the key was removed successfully.
	bool remove(int key)
	{
		return false;
	}
	/// return true if *key* is present in the appropriate list, false
	/// otherwise
	bool lookup(int key) const
	{
		return false;
	}

	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.

	//This refers to the number of buckets not the total number of elements.
	size_t getSize() const
	{
		return 0;
	}

	//This refers to the number of elements in a bucket, not the sentinel node.
	size_t getBucketSize(size_t bucket) const
	{
		return 0;
	}
	int getElement(size_t bucket, size_t idx) const
	{
		return 0;
	}


	//These functions just need to exist, they do not need to do anything
	int getElement(size_t idx) const
	{
		return 0;
	}
};


