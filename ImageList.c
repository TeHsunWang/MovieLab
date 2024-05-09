/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* ImageList.c: source file for image list manipulations             */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "ImageList.h"

/* Create a new image list */
ILIST *CreateImageList(void)
{
	ILIST *l = NULL;
	l =(ILIST*) malloc(sizeof(ILIST));
	if (l == NULL)
	{
		printf("Fail to create Image List\n");
		return 0;
	}
	l->First = NULL;
	l->Last = NULL;
	l->Length = 0;	
	return l;
}

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list)
{
	assert(list);
	IENTRY* cur = NULL;
	cur = list->First;
	while(cur->Next != NULL)
	{
		IENTRY* nxt = NULL;
		nxt = cur->Next;
		free(nxt);
	}
	free(list);

}

/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, Image *RGBimage)
{
	assert(list);
	assert(RGBimage);
	IENTRY* e = NULL;
	e = (IENTRY*)malloc(sizeof(IENTRY));
	if (e == NULL)
	{
		printf("Fail create entry\n");
	}
	e->List = list;
	e->Next = NULL;
	if (list->Last != NULL)
	{	// everytime when we create the list, *F, *L have to point to entry
		e->Prev = list->Last;
		list->Last->Next = e;
		list->Last = e;
	}
	else
	{
		e->Prev = NULL;
		list->First = e;
		list->Last = e;
	}
	e->image = RGBimage;
	list->Length++;
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVImage *YUVimage) {
    assert(list);
    assert(YUVimage);
    IENTRY* new_entry = (IENTRY*) malloc(sizeof(IENTRY));
    assert(new_entry);
    
    new_entry->image = YUVimage;
    new_entry->List = list;
    new_entry->Next = NULL;  // next will always be NULL for the new last entry

    if(list->Last == NULL) {
        // If the list is empty
        new_entry->Prev = NULL;  // there's no previous entry
        list->First = new_entry; // this new entry is now the first
        list->Last = new_entry;  // and also the last
    } else {
        // If the list is not empty
        new_entry->Prev = list->Last;  // the previous last entry is the previous for new entry
        list->Last->Next = new_entry;  // the new entry is the next for the previous last entry
        list->Last = new_entry;        // update the list's last to be the new entry
    }
    list->Length += 1; // increment the length of the list
}


/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end)
{
    assert(list);
    IENTRY* curr;
    IENTRY* next;
    int count = 0;
    curr = list->First;

    if (end >= list->Length) {
        end = list->Length - 1;
    }

    // Remove frames before 'start'
    while (count < start && curr != NULL) {
        next = curr->Next;
        DeleteImage(curr->image);
        free(curr);
        curr = next;
        count++;
        list->Length--; // Decrement list length
    }

    list->First = curr;
    if (curr != NULL) {
        curr->Prev = NULL;
    }

    // Skip frames between 'start' and 'end'
    while (count < end && curr != NULL) {
        curr = curr->Next;
        count++;
    }

    // If 'end' is not the end of the list, fix the remaining part
    if (curr != NULL && curr->Next != NULL) {
        IENTRY* temp = curr->Next;
        curr->Next = NULL;  // Break the link
        list->Last = curr;  // Update 'Last' pointer

        // Delete frames after 'end'
        curr = temp;
        while (curr != NULL) {
            next = curr->Next;
            DeleteImage(curr->image);
            free(curr);
            curr = next;
            list->Length--; // Decrement list length
        }
    } else {
        // If 'end' is the end of the list
        list->Last = curr;
    }
}

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor)
{
	assert(list);
	int count = 0;
	IENTRY* current = NULL;
	IENTRY* next = NULL;
	current = list->First;
	current->Prev = NULL;
	while(current != NULL)
	{
		next = current->Next;
		if(count % factor != 0)
		{
			if(current->image) DeleteYUVImage(current->image);
			if(current->Prev) current->Prev->Next = current->Next;
			else list->First = current->Next;
			if(current->Next) current->Next->Prev = current->Prev;
			else list->Last = current->Prev;
			current->List->Length--;
			free(current);
		}
		count++;
		current = next;
	}	
}


/* Reverse an image list */
void ReverseImageList(ILIST *list)
{
	assert(list);
    if (list->First == NULL || list->First->Next == NULL) {
        return;
    }

    IENTRY *current = list->First;
    IENTRY *temp = NULL;

    while (current != NULL) {
        temp = current->Prev;
        current->Prev = current->Next;
        current->Next = temp;
        if (current->Prev == NULL) {
            // If there's no previous node, we've reached the new head of the list.
            list->First = current;
        }
        current = current->Prev;
    }
}

/* Insert a RGB image to the image list in the front */
void PrependRGBImage(ILIST *list, Image *RGBimage)
{
	assert(list);
	assert(RGBimage);
	IENTRY* current = (IENTRY*)malloc(sizeof(IENTRY));
	IENTRY* head = list->First;
	current->List = list;
	if(!head)
	{
		list->Last = current;
	}
	current->Next = head;
	if(head) head->Prev = current;
	list->First = current;
	current->Prev = NULL;
	current->image = RGBimage;
	current->List->Length++;
}


/* EOF */
