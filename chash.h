

#pragma once
#include<vector>
#include "clist.h"
/// TODO: complete this implementation of a thread-safe (concurrent) hash
///       table of integers, implemented as an array of linked lists.
class chash
{
	/// The bucket list
	const std::vector<clist> buckets;

public:
	chash(unsigned _buckets) : buckets()//buckets(init(_buckets)){}
	{}

	/// insert *key* into the appropriate linked list if it doesn't already
	/// exist; return true if the key was added successfully.
	bool insert(int key)
	{
		cout << buckets.size();
		return false;
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

	std::vector<clist> init(unsigned buckets){
		clist test = 0;
		std::vector<clist> blah;


		//blah.push_back(clist(0));

		//threads.push_back(std::thread(task, i));
		
		
		

		return blah;
	}

};
