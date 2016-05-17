
#ifndef MY_BINARY_HEAP_H
#define MY_BINARY_HEAP_H

#include <iostream>
#include <vector>
#include <unordered_map>

template<typename key, typename value, typename keyLess>
class MyBinaryHeap
{
public:
	MyBinaryHeap() :size(0) { keys.resize(100); values.resize(100); }
	~MyBinaryHeap() {}
	key MinKey() { return keys[0]; }
	value ExtractMin() { return values[0]; }
	void DeleteMin();
	void Insert(const key& k, const value& val);
	void DecreaseKey(const key& nk, const value& val);
	void IncreaseKey(const key& nk, const value& val);
	bool Empty() { return size == 0; }
	//the following methods are specific for search algorithms
	bool IsExist(const value& val);
	key GetKey(const value& val);
	//void Merge()
private:
	//TODO implement heapify()
	void heapify(std::size_t index);
	std::vector<key> keys;
	std::vector<value> values;
	std::unordered_map<value, std::size_t> indices;
	std::size_t size;
};

template<typename key, typename value, typename keyLess>
void MyBinaryHeap<key, value, keyLess>::DeleteMin()
{
	//std::cout << "calling deletemin \n";
	//proper resize might needed
	
	indices[values[size - 1]] = 0;
	indices[values[0]] = size - 1;
	std::swap(keys[0], keys[size - 1]);
	std::swap(values[0], values[size - 1]);

	indices.erase(indices.find(values[size-1]));
	size--;
	if (size > 0)
		heapify(0);
	if (indices.size() != size)
	{
		std::cout << "error in delete\n";
		std::cout << "indices.size"<< indices.size() <<" size "<<size<<"\n";
	}
}

template<typename key, typename value, typename keyLess>
void MyBinaryHeap<key, value, keyLess>::IncreaseKey(const key& nk, const value& val)
{
	std::size_t index = indices[val];
	keys[index] = nk;
	heapify(index);
}

template<typename key, typename value, typename keyLess>
void MyBinaryHeap<key, value, keyLess>::Insert(const key& k,const value& val)
{
	//std::cout << "calling insert \n";
	//proper resize might needed
	if (keys.size() <= size)
	{
		keys.resize(2 * size);
		values.resize(2 * size);
	}
	
	values[size] = val;
	indices[val] = size;
	size++;
	DecreaseKey(k, val);
	if (indices.size() != size)
	{
		std::cout << "error in insert\n";
		std::cout << "indices.size" << indices.size() << " size " << size << "\n";
	}
	//std::cout << "Minkey after insert: " << MinKey() << "\n";
}

template<typename key, typename value, typename keyLess>
bool MyBinaryHeap<key, value, keyLess>::IsExist(const value& val)
{
	if (indices.find(val) == indices.end())
		return false;
	return true;
}

template<typename key, typename value, typename keyLess>
key MyBinaryHeap<key, value, keyLess>::GetKey(const value& val)
{
	return keys[indices[val]];
}

template<typename key, typename value, typename keyLess>
void MyBinaryHeap<key, value, keyLess>::DecreaseKey(const key& nk, const value& val)
{
	//std::cout << "calling decrease key \n";
	std::size_t index = indices[val];
	keys[index] = nk;
	if (values[index] != val)
	{
		std::cout << "error in decrease key\n";
		std::cout << "index " << index << " size "<<size<<"\n";
		std::cout << "values[index] "<<values[index]<<"\n";
		std::cout << "val " << val << "\n";
		return;
	}
	std::size_t parent;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (keyLess()(keys[index], keys[parent]))
		{
			indices[values[parent]] = index;
			indices[values[index]] = parent;
			std::swap(keys[parent], keys[index]);
			std::swap(values[parent], values[index]);
			index = parent;
		}
		else
			break;
	}
}

template<typename key, typename value, typename keyLess>
void MyBinaryHeap<key, value, keyLess>::heapify(std::size_t index)
{
	//std::cout << "calling heapify(" << index << " )\n";
	std::size_t left = index * 2 + 1;
	std::size_t right = index * 2 + 2;

	std::size_t min;
	if (left < size && keyLess()(keys[left],keys[index]))
		min = left;
	else
		min = index;
	if (right < size && keyLess()(keys[right], keys[min]))
		min = right;
	
	if (min != index)
	{
		//swap keys, swap values and update indices
		indices[values[min]] = index;
		indices[values[index]] = min;
		std::swap(keys[min], keys[index]);
		std::swap(values[min], values[index]);
		heapify(min);
	}

	
}

#endif // !MY_BINARY_HEAP_H
