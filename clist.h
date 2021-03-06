

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
//std::mutex list_mutex;
class clist
{
	/// a Node consists of a value and a pointer to another node
	struct Node
	{
		int value;
		Node* next;
	};

	/// The head of the list is referenced by this pointer
	mutable Node* head;
	mutable std::mutex list_mutex;
	mutable int size = 0;
	//size_t size = 0;
	//mutable std::mutex list_mutex;

public:
	//Node* head;
	//size_t size = 0;
	
	clist(int)
	: head(NULL)
	{}
	clist()
	{
		clist(0);
	}

	/// insert *key* into the linked list if it doesn't already exist; return
	/// true if the key was added successfully.
	bool insert(int key) const
	{
		list_mutex.lock();
		bool ret = false;
		struct Node *toinsert;
		toinsert = (struct Node*) malloc(sizeof(struct Node));
		//lock is released automatically when out of scope
		//std::lock_guard<std::mutex> lock(list_mutex);
		//list_mutex.lock();
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
	bool remove(int key) const
	{
	
		list_mutex.lock();
		bool ret = false;
		//list_mutex.lock();
		//std::lock_guard<std::mutex> lock(list_mutex);
		Node* temp = head;
		Node* prev = NULL;
		while(temp != NULL){
			if(temp->value == key && prev ==NULL){
				head = temp->next;
				size--;
				list_mutex.unlock();
				return true;
			}
			else if (temp->value == key && prev != NULL){
				Node* next = temp->next;
				prev->next = next;
				size--;
				list_mutex.unlock();
				return true;
			}
			prev = temp;
			temp = temp->next;
		}
		//print remove key result
		// cout << "remove " << key << " result:" << endl;
		// 	temp = head;
		// 	while(temp){
		// 		cout << temp->value << "->";
		// 		temp = temp -> next;
		// 	}
		// 	cout << endl;
		list_mutex.unlock();
		return false;
	

	}



	/// return true if *key* is present in the list, false otherwise
	bool lookup(int key) const
	{
		list_mutex.lock();
		bool ret = false;
		//list_mutex.lock();
		//std::lock_guard<std::mutex> lock(list_mutex);
		Node* temp = head;
		Node* prev = NULL;
		while(temp){
			if(temp->value == key && prev == NULL){
				list_mutex.unlock();
				return true;
			}
			else if(temp->value == key && prev != NULL){
				list_mutex.unlock();
				return true;
			}
			prev = temp;
			temp = temp -> next;
		}
		list_mutex.unlock();
		return false;
	}


	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.
	size_t getSize() const
	{
		return size;
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