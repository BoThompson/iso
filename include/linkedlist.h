#pragma once

#ifndef LIST_H
#define LIST_H

class List
{
	typedef struct EntityData
	{
		int data;
		EntityData * next; // points to next EntityData in list
	}*EntityDataPtr;

	EntityDataPtr head; //beginning of list
	EntityDataPtr current;
	EntityDataPtr temp;

public:
	List();
	void AddEntityData(int addData);
	void DeleteEntityData(int delData); // looks through list to find EntityData with data matching parameter
	void PrintList();
};

#endif