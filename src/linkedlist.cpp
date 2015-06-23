#include <cstdlib>
#include <iostream>

#include "linkedlist.h"

List::List()
{
	head = NULL;
	current = NULL;
	temp = NULL;
}

void List::AddEntityData(int addData)
{
	EntityDataPtr n = new EntityData; // new EntityData ptr, new EntityData
	n->next = NULL; //attach to end of list
	n->data = addData;

	if (head != NULL) // if list already created
	{
		current = head; // make current ptr point to front of list

		while (current->next != NULL) // while we are not at theend of the list
		{
			current = current->next; // go to end of list
		}
		
		current->next = n; // will point to new EntityData that was made
	}
	else
	{
		head = n; // new EntityData will be front of list
	}
}

void List::DeleteEntityData(int delData)
{
	EntityDataPtr delPtr = NULL;
	temp = head; // will always be one step behind current to patch up list after delete
	current = head;

	while (current != NULL && current->data != delData) // will go search through list until the end looking for number matching delData
	{
		temp = current;
		current = current->next;
	}

	if (current == NULL) // if reached end of list, we did not find the value
	{
		printf("%i was not found in list\n", delData);
		delete delPtr; // well, won't be needing this
	}
	else // found it
	{
		delPtr = current;
		current = current->next; //now go to next EntityData...
		
		// temp is now pointing to the EntityData that we want to connect to the EntityData that curr is pointing to
		
		temp->next = current; //patches up list

		if (delPtr == head) // what if we're deleting the head?
		{
			head = head->next; // change the head
			temp = NULL;
		}

		delete delPtr; // delete the one in the middle or the EntityData captured by delPtr
	}
}

void List::PrintList()
{
	current = head; // go to front of list
	while (current != NULL) // go thru list
	{
		printf("%i\n", current->data);
		current = current->next; //NEXT
	}
}