#pragma once

#include <iostream>
  
#include <stdexcept>  

using namespace std;

template<typename T>

class TVector
{
protected:
    size_t sz;

    size_t vector_capacity;

    T* pMem;

public:
    TVector(size_t size = 0) : sz(size), vector_capacity(sz * 2 + 2)
    {
        pMem = new T[vector_capacity];

        if (pMem == nullptr)
        {
            throw runtime_error("Failed to allocate memory for vector");
        }
    }

    TVector(T* tmp, size_t s) : sz(s), vector_capacity(sz * 2 + 2)
    {
        if (tmp == nullptr)
        {
            throw logic_error("TVector constructor requires a non-nullptr argument for the array");
        }
        pMem = new T[vector_capacity];

        if (pMem == nullptr)
        {
            throw runtime_error("Memory allocation failed while trying to create a vector");
        }
        copy(tmp, tmp + sz, pMem);
    }

    TVector(const TVector& k) : sz(k.sz), vector_capacity(k.vector_capacity)
    {
        pMem = new T[vector_capacity];

        if (pMem == nullptr)

        {
            throw runtime_error("Failed to allocate memory for vector during copy construction");
        }

        copy(k.pMem, k.pMem + sz, pMem);
    }

    TVector(TVector&& v) noexcept
    {
        sz = 0;

        vector_capacity = 0;

        pMem = nullptr;

        swap(*this, v);
    }

    ~TVector()
    {
        delete[] pMem;

        pMem = nullptr;
    }

    TVector& operator=(const TVector& v)
    {
        if (this != &v)
        {
            if (sz != v.sz)
            {
                sz = v.sz;

                delete[] pMem;

                T* p = new T[sz];

                if (p == nullptr)

                {
                    throw runtime_error("Failed to allocate memory for vector");
                }
                pMem = p;

                p = nullptr;
            }
            copy(v.pMem, v.pMem + sz, pMem);
        }
        return *this;
    }

    TVector& operator=(TVector&& v) noexcept
    {
        sz = 0;

        vector_capacity = 0;

        delete[] pMem;

        pMem = nullptr;

        swap(*this, v);

        return *this;
    }

    size_t size() const noexcept
    {
        return sz;
    }

    size_t getVector_capacity() const noexcept
    {
        return vector_capacity;
    }

    bool full() const noexcept
    {
        return (sz == vector_capacity);
    }

    bool empty() const noexcept
    {
        return (sz == 0);
    }

    T& operator[] (size_t i)
    {
        return pMem[i];
    }

    const T& operator[] (size_t i) const
    {
        return pMem[i];
    }

    bool operator==(const TVector& v) const noexcept
    {
        if (sz == v.sz)
        {
            for (size_t i = 0; i < sz; i++)
            {
                if (pMem[i] != v.pMem[i])
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    bool operator!=(const TVector& v) const noexcept
    {
        return !(*this == v);
    }

    friend void swap(TVector& k1, TVector& k2) noexcept
    {
        swap(k1.sz, k2.sz);

        swap(k1.vector_capacity, k2.vector_capacity);

        swap(k1.pMem, k2.pMem);
    }

    void resize(size_t Snew)
    {
        T* p = new T[Snew * 2 + 2];

        if (p == nullptr)
        {
            throw runtime_error("Failed to allocate memory for vector");
        }
        copy(pMem, pMem + sz, p);

        delete[] pMem;

        pMem = p;

        vector_capacity = Snew * 2 + 2;
    }

    void push_back(T Elem)
    {
        if (full())
        {
            resize(sz + 1);
        }
        pMem[sz++] = Elem;
    }

    void pop_back()
    {
        if (!empty())
        {
            sz--;
        }
        else
        {
            throw logic_error("Cannot pop back: vector is empty");
        }
    }

    void push_front(T Elem)
    {
        insert(Elem, 0);
    }

    void pop_front()
    {
        erase(0);
    }

    void insert(T Elem, size_t index)
    {
        if (index < 0 || index >= vector_capacity)
        {
            throw out_of_range("Index must be between 0 and the current size of the vector");
        }

        if (sz == vector_capacity)
        {
            resize(sz + 1);
        }

        for (size_t i = sz; i > index; i--)
        {
            pMem[i] = pMem[i - 1];
        }

        pMem[index] = Elem;

        sz++;
    }

    void erase(size_t index)
    {
        if (index < 0 || index >= vector_capacity)
        {
            throw out_of_range("Index must be between 0 and the current size of the vector");
        }

        if (sz == 0)
        {
            throw logic_error("Cannot erase from an empty vector");
        }

        for (size_t i = index; i < sz; i++)
        {
            pMem[i] = pMem[i + 1];
        }

        sz--;
    }
};

template<typename T>

class Stack : private TVector<T>
{
	using TVector<T>::sz;

	using TVector<T>::vector_capacity;

	using TVector<T>::pMem;

	using TVector<T>::push_back;

	using TVector<T>::pop_back;

	using TVector<T>::resize;

public:

	using TVector<T>::full;

	using TVector<T>::empty;

	using TVector<T>::size;

	using TVector<T>::getVector_capacity;

	Stack() : TVector<T>(0) { }; // создаётся пустой стек

	void push(T Elem)
	{
		push_back(Elem);
	}

	void pop()
	{
		pop_back();
	}

	T top()
	{
		if (empty())
		{
			throw logic_error("Stack underflow: no elements in the stack");
		}
		return pMem[sz - 1]; // возращаем последний элемент
	}
};

template<typename T>

class Queue : private TVector<T>
{
    using TVector<T>::sz;

    using TVector<T>::vector_capacity;

    using TVector<T>::pMem;

    using TVector<T>::push_back;

    using TVector<T>::resize;

    size_t front;

    size_t back;

public:

    using TVector<T>::full;

    using TVector<T>::empty;

    using TVector<T>::size;

    using TVector<T>::getVector_capacity;

    Queue() : TVector<T>(0), front(0), back(0) { }

    void push(T elem)
    {
        push_back(elem);

        if (back == vector_capacity)
        {
            back = 0;
        }
        else
        {
            back++;
        }
    }

    void pop()

    {
        if (empty())
        {
            throw logic_error("error");
        }
        if (front == vector_capacity)
        {

            front = 0;
        }
        else
        {
            front++;
        }
        sz--;
    }
    T getFront() const noexcept 
    { 
        return pMem[front];
    }

    T getBack() const noexcept
    { 
        if (back == 0)
        {
            return pMem[vector_capacity - 1];
        }
        return pMem[back - 1]; 
    }
    size_t getBackIndex() const noexcept 
    {
        return back;
    }
};


template<typename T>

class Queue_Stack : private TVector<Stack<T>>
{
	using TVector<Stack<T>>::pMem;

	size_t sz;

	size_t vector_capacity;

public:
	Queue_Stack() : TVector<Stack<T>>(0), sz(pMem[0].size() + pMem[1].size()), vector_capacity(pMem[0].getVector_capacity() + pMem[1].getVector_capacity()) { }

	size_t size() const noexcept 
	{ 
		return sz; 
	}

	size_t getVector_capacity() const noexcept 
	{ 
		return vector_capacity;
	}

	bool full() const noexcept 
	{ 
		return (pMem[0].isFull() && pMem[1].isFull());
	}

	bool empty() const noexcept 
	{ 
		return (pMem[0].isEmpty() && pMem[1].isEmpty());
	}

	void push(T Elem)
	{
		pMem[1].push(Elem);

		sz++;
	}

	void pop()
	{
		if (pMem[0].isEmpty())
		{
			while (!pMem[1].isEmpty())
			{
				pMem[0].push(pMem[1].top());

				pMem[1].pop();
			}

			pMem[0].pop();
		}

		if (!pMem[0].isEmpty())
		{
			pMem[0].pop();
			--sz;
		}
		else
		{
			throw logic_error("Queue is empty");
		}

	}

	T front()

	{
		if (pMem[0].isEmpty())
		{
			while (!pMem[1].isEmpty())
			{
				pMem[0].push(pMem[1].top());

				pMem[1].pop();
			}
		}

		if (!pMem[0].isEmpty())
		{
			return pMem[0].top();  
		}

		throw logic_error("Queue is empty");
	}

    T back()
    {
        if (!pMem[1].isEmpty())

        {
            return pMem[1].top();
        } 

        if (!pMem[0].isEmpty())
        {

            while (!pMem[0].isEmpty())
            {
                pMem[1].push(pMem[0].top());

               pMem[0].pop();

            }
        }

        throw logic_error("Queue is empty");
    }

};