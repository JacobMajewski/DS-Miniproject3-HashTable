	#pragma once
	class HashTable
	{
	public:
		virtual ~HashTable() = default;
		virtual void insert(int key, int elem) = 0;
		virtual int remove(int key) = 0;
	};

