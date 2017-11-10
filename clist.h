

#pragma once

#include <chrono>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

using namespace std;

/// TODO: complete this implementation of a thread-safe (concurrent) sorted
/// linked list of integers
class clist
{
	/// a Node consists of a value and a pointer to another node
	struct Node
	{
		int value;
		Node* next;
	};

	/// The head of the list is referenced by this pointer
	Node* head;
	size_t size = 0;
	mutable std::mutex list_mutex;

public:
	clist(int)
	: head(NULL)
	{}

	/// insert *key* into the linked list if it doesn't already exist; return
	/// true if the key was added successfully.
	bool insert(int key)
	{
		bool ret = false;
		Node* toinsert = (Node*) malloc(sizeof(struct Node));
		list_mutex.lock();		
			toinsert -> value = key;
			toinsert -> next = NULL;
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
			// //print list after insert
			// cout << "insert " << key << " result:" << endl;
			// Node* temp = head;
			// while(temp){
			// 	cout << temp->value << "->";
			// 	temp = temp -> next;
			// }
			// cout << endl;
		list_mutex.unlock();
		return ret;
	}
	/// remove *key* from the list if it was present; return true if the key
	/// was removed successfully.
	bool remove(int key)
	{
		bool ret = false;
		list_mutex.lock();
			Node* temp = head;
			if(head -> value == key){
				head = head -> next;
				free(temp);
				ret = true;
				size--;
			}
			else{
				while(temp -> next){
					if(temp -> next -> value == key){
						Node* toremove = temp -> next;
						if(temp -> next -> next){
							temp -> next = temp -> next -> next;
							free(toremove);
						}
						else{
							temp -> next = NULL;
							free(temp -> next);
						}
						ret = true;
						size--;
						break;
					}
					else if(temp -> next -> value > key){
						break;
					}
					temp = temp -> next;
				}
			}
			// //print list after remove
			// cout << "remove " << key << " result:" << endl;
		 	// 	temp = head;
			// while(temp){
			// 	cout << temp->value << "->";
			// 	temp = temp -> next;
			// }
			// cout << endl;
		list_mutex.unlock();
		return ret;
	}
	/// return true if *key* is present in the list, false otherwise
	bool lookup(int key) const
	{
		bool ret = false;
		list_mutex.lock();
		Node* temp = head;
		while(temp){
			if(temp -> value == key){
				ret = true;
				break;
			}
			else if(temp -> value > key){
				break;
			}
			temp = temp -> next;
		}
		list_mutex.unlock();
		return ret;
	}


	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.
	size_t getSize() const
	{
		return size;
	}
	int getElement(size_t idx) const
	{
		int ret = 0;
		if(idx >= getSize()){
			return ret;
		}
		int count = idx;
		Node* temp = head;
		while(count > 0){
			temp = temp -> next;
			count --;
		}
		return temp -> value;
	}


	//These functions just need to exist, they do not need to do anything
	size_t getBucketSize(size_t bucket) const
	{
		return 0;
	}
	int getElement(size_t bucket, size_t idx) const
	{
		return 0;
	}
};
