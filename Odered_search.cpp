#include <iostream>
#include<ctime>
#include<cstdlib>
#include<limits>
#include<cmath>
#include<cstddef>
#include<fstream>
#include<cctype>

using namespace std;

enum Error_code
{
	success, fail, range_out, underflow, overflow, not_present, fatal, duplicate_error, emtry_inserted, entry_found, internal_error
};

int const max_list = 1000;


//顺序表
template <class List_entry>
class List
{
public:
	List();
	int size() const;
	bool full() const;
	bool empty() const;
	void clear();
	void traverse(void(*vist)(List_entry&));
	Error_code retrieve(int position, List_entry &x) const;
	Error_code replace(int position, const List_entry& x);
	Error_code remove(int position, List_entry &x);
	Error_code insert(int position, const List_entry& x);
protected:
	int count;
	List_entry entry[max_list];
};
template <class List_entry>
List<List_entry>::List()
{
	count = 0;
}

template <class List_entry>
int List<List_entry>::size() const
{
	return count;
}

template <class List_entry>
bool List<List_entry>::full() const
{
	return count >= max_list;
}

template <class List_entry>
bool List<List_entry>::empty() const
{
	return count <= 0;
}

template <class List_entry>
void List<List_entry>::clear()
{
	count = 0;
}


template <class List_entry>
void List<List_entry>::traverse(void(*visit)(List_entry&))
{
	for (int i = 0; i < count; i++)
		(*visit)(entry[i]);
}


template <class List_entry>
Error_code List<List_entry>::retrieve(int position, List_entry& x)const
{
	if (position < 0 || position >= count)
		return range_out;
	else
		x = entry[position];
	return success;
}

template <class List_entry>
Error_code List<List_entry>::replace(int position, const List_entry& x)
{
	if (position < 0 || position >= count)
		return range_out;
	else
		entry[position] = x;
	return success;
}

template <class List_entry>
Error_code List<List_entry>::remove(int position, List_entry &x)
{
	if (position == 0)
		return underflow;
	if (position < 0 || position >= count)
		return range_out;
	x = entry[position];
	count--;
	while (position < count)
	{
		entry[position] = entry[position + 1];
		position++;
	}
	return success;
}

template <class List_entry>
Error_code List<List_entry>::insert(int position, const List_entry &x)
{
	if (full())
		return overflow;
	if (position < 0 || position > count)
		return range_out;
	for (int i = count - 1; i >= position; i--)
		entry[i + 1] = entry[i];
	entry[position] = x;
	count++;
	return success;
}

typedef int Key;
typedef int Record;



//顺序查找算法
Error_code sequential_search(const List<Record> &the_list, const Key &target, int &position)
{
	int s = the_list.size();
	for (position = 0; position < s; position++)
	{
		Record data = 0;
		the_list.retrieve(position, data);
		if (data == target)
			return success;
	}
	return not_present;
}

//有序的顺序表
class Ordered_list :public List<Record>
{
public:
	Error_code insert(const Record &data);
	Error_code insert(int position, const Record &data);
};

Error_code Ordered_list::insert(const Record &data)
{
	int s = size();
	int position;
	for (position = 0; position < s; position++)
	{
		Record list_data = 0;
		retrieve(position, list_data);
		if (data >= list_data)
			break;
	}
	return List<Record>::insert(position, data);
}

Error_code Ordered_list::insert(int position, const Record &data)
{
	Record list_data = 0;
	if (position > 0)
	{
		retrieve(position - 1, list_data);
		if (data < list_data)
			return fail;
	}
	if (position < size())
	{
		retrieve(position, list_data);
		if (data > list_data)
			return fail;
	}
	return List<Record>::insert(position, data);
}


//递归二分查找算法
/*
Error_code recursive_binary(const Ordered_list &the_list, const Key target, int bottom, int top, int position)
{
Record data;
if (bottom < top)
{
int mid = (bottom + top) / 2;
the_list.retrieve(mid, data);
if (data < target)
return recursive_binary(the_list, target, mid + 1, top, position);
else
return recursive_binary(the_list, target, mid, top, position);
}
else if (top < bottom)
return not_present;
else
{
position = bottom;
the_list.retrieve(bottom, data);
if (data == target)
return success;
else
return not_present;
}
}

Error_code run_recursive_binary(const Ordered_list &the_list, const Key target, int position)
{
return recursive_binary(the_list, target, 0, the_list.size() - 1, position);
}*/

//普通二分查找算法
Error_code binary_search_1(const Ordered_list &the_list, const Key &target, int &position)
{
	Record data = 0;
	int bottom = 0, top = the_list.size() - 1;
	while (bottom < top)
	{
		int mid = (bottom + top) / 2;
		the_list.retrieve(mid, data);
		if (data < target)
			bottom = mid + 1;
		else
			top = mid;
	}
	if (top < bottom)
		return not_present;
	else
	{
		position = bottom;
		the_list.retrieve(bottom, data);
		if (data == target)
			return success;
		else
			return not_present;
	}
}

Error_code binary_search_2(const Ordered_list &the_list, const Key &target, int &position)
{
	Record data;
	int bottom = 1, top = the_list.size() - 1;
	while (bottom <= top)
	{
		position = (bottom + top) / 2;
		the_list.retrieve(position, data);
		if (data == target)
			return success;
		if (data <= target)
			bottom = position - 1;
		else
			top = position - 1;
	}
	return not_present;
}




int main()
{
	int num, target = 0, position = 0;
	List<int> lis;
	Ordered_list O_lis;
	for (int i = 0; i < max_list; i++)
	{
		num = rand() * 2 + 1;
		//lis.insert(0, num);
		O_lis.insert(num);
	}

	int O_lis_size = O_lis.size();
	clock_t TimeBeign = clock();
	for (int i = 0; i < 1000000; i++)
	{
		lis.retrieve(rand() % O_lis_size, target);
		//sequential_search(lis, target, position);
		//binary_search_1(O_lis, target, position);
		binary_search_2(O_lis, target, position);
	}
	clock_t TimeEnd = clock();

	cout << "Binary_search_1查找成功100万次耗时：" << double(TimeEnd - TimeBeign) / CLOCKS_PER_SEC << endl;
	clock_t TimeB = clock();
	for (int i = 0; i < 1000000; i++)
	{
		//sequential_search(lis, 2, position);
		//binary_search_1(O_lis, 2, position);
		binary_search_2(O_lis, target, position);
	}
	clock_t TimeE = clock();
	cout << "Binary_search_1查找失败100万次耗时：" << double(TimeE - TimeB) / CLOCKS_PER_SEC << endl;
}