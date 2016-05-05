
#ifndef MY_BINARY_HEAP_H
#define MY_BINARY_HEAP_H

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
	void DecreaseKey(const key& nk,const value& val);
	bool Empty() { return size == 0; }
	//the following methods are specific for search algorithms
	void IsExist(const value& val);
	key GetKey(const value& val);
	//void Merge()
private:
	//TODO implement heapify()
	void heapify(int index);
	std::vector<key> keys;
	std::vector<value> values;
	std::unordered_map<value, int> indices;
	int size;
}

template<typename key, typename value, typename keyLess>
void MyBinaryHeap<key, value, keyLess>::DeleteMin()
{
	//proper resize might needed
	indices.erase(indices.find(values[0]));
	keys[0] = keys[size - 1];
	values[0] = value[size - 1];
	size--;
	heapify(size-1);
}

template<typename key, typename value, typename keyLess>
void MyBinaryHeap<key, value, keyLess>::Insert(const key& k,const value& val)
{
	//proper resize might needed
	if (keys.size() < size)
	{
		keys.push_back(k);
		values.push_back(k);
	}
	else
	{
		keys[size] = k;
		values[size] = val;
	}
	indices[val] = size;
	heapify(size);
	size++;
}

template<typename key, typename value, typename keyLess>
void MyBinaryHeap<key, value, keyLess>::IsExist(const value& val)
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
	int index = indices[val];
	keys[index] = nk;
	heapify(index);
}

template<typename key, typename value, typename keyLess>
void MyBinaryHeap<key, value, keyLess>::heapify(int index)
{
	if (index == 0)
		return;
	int left = index * 2 + 1;
	int right = index * 2 + 2;

	int min;
	if (left < size && keyLess(keys[left],keys[index]))
		min = left;
	else
		min = index;
	if (right < size && keyLess(keys[right], keys[index]))
		min = right;
	
	if (min != index)
	{
		//swap keys, swap values and update indices
		indices[value[min]] = index;
		indices[value[index]] = min;
		std::swap(keys[min], keys[index]);
		std::swap(values[min], values[index]);
	}

	heapify(min);
}

#endif // !MY_BINARY_HEAP_H
