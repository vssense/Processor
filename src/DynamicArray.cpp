#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct DynamicArray
{
	elem_t* buffer = nullptr;
	size_t size = 0;
	size_t capacity = 0;
};

DynamicArray* ConstructArray(size_t capacity)
{
	DynamicArray* arr = (DynamicArray*)calloc(1, sizeof(DynamicArray));
	
	arr->buffer = (elem_t*)calloc(capacity, sizeof(elem_t));
	assert(arr->buffer);

	arr->size = 0;
	arr->capacity = capacity;

	return arr;	
}

void CheckBuffer(DynamicArray* arr)
{
	assert(arr);

	if (arr->size >= arr->capacity - 1)
	{
		elem_t* tmp = (elem_t*)realloc(arr->buffer, arr->capacity * 2 * sizeof(elem_t));
		assert(tmp);
		arr->capacity *= 2;
		arr->buffer = tmp;
	}
	else if (arr->size < arr->capacity / 4 and arr->size > 100)
	{
		arr->buffer = (elem_t*)realloc(arr->buffer, (arr->capacity * sizeof(elem_t)) / 2) ;
		arr->capacity /= 2;
	}
}

void PushArr(DynamicArray* arr, elem_t elem)
{
	assert(arr);

	CheckBuffer(arr);

	arr->buffer[arr->size] = elem;
	arr->size++;
}

size_t GetSize(DynamicArray* arr) { return arr->size; }

void PopArr(DynamicArray* arr)
{
	assert(arr);

	if (GetSize(arr) == 0)
	{
		printf("You try to pop from empty array\n");
	}
	else
	{
		arr->size--;
		CheckBuffer(arr);
	}
}

elem_t TopArr(DynamicArray* arr)
{
	assert(arr);

	if (GetSize(arr) == 0)
	{
		printf("You try to top from empty array\n");
		return arr->buffer[arr->size - 1];
	}
	else
	{
		CheckBuffer(arr);
		return arr->buffer[arr->size - 1];
	}
}

void DestructArray(DynamicArray* arr)
{
	assert(arr);
	assert(arr->buffer);

	free(arr->buffer);
	arr->size = 0;
	arr->capacity = 0;
}