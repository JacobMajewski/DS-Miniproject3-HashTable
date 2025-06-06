#pragma once
class HashTable
{
	virtual void insert(int key, int elem) = 0;
	virtual int remove(int key) = 0;
};

