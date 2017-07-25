#pragma once
#include <stddef.h>
using namespace std;
template <class T>
class ArrayList
{
private:
	int capacity;
	T *obj;

public:
	int size;
public:
	ArrayList()
	{
		size = capacity = 0;
		obj = NULL;
	}
	~ArrayList()
	{
		delete[] obj;
	}
	ArrayList(int a)
	{
		capacity = a;
		size = 0;
		obj = new T[capacity];
		for (int i = 0; i<capacity; i++)
		{
			obj[i] = NULL;
		}
	}
	int size_of_list();
	bool empty();
	void reserve(int);
	T& operator[](int);
	T at(int);
	T front();
	T back();
	void push_back(T element);
	void pop_back();
	void insert(T);
	void erase(int);

};
template<class T>
int ArrayList<T>::size_of_list()
{
	return size;
}
template<class T>
bool ArrayList<T>::empty()
{
	if (size == 0)
		return true;
	else
		return false;
}
template<class T>
void ArrayList<T>::reserve(int Num)
{
	if (capacity >Num)
		return;
	T *temp = new T[Num];
	for (int i = 0; i<size; i++)
	{
		temp[i] = obj[i];
	}
	if (obj != 0) delete[] obj;
	obj = temp;
	capacity = Num;
}
template<class T>
T& ArrayList<T>::operator[](int index)
{
	return obj[index];
}
template<class T>
T ArrayList<T>::at(int i)
{
	return obj[i];
}
template<class T>
T ArrayList<T>::front()
{
	return obj[0];
}
template<class T>
T ArrayList<T>::back()
{
	return obj[size - 1];
}

template<class T>
void ArrayList<T>::insert(T e)
{
	if (size >= capacity) {
		reserve(capacity + 10);
	}
	obj[size] = e;
	size++;
}

template<class T>
void ArrayList<T>::erase(int i)
{
	if (i >= 0 && i<size)
	{
		for (int j = 0; j<size; j++)
		{
			obj[j] = obj[j + 1];
		}
		obj[size] = NULL;
		size--;
	}
}
