// ALGO2 IS1 224B LAB06
// ILLIA KARMAZIN
// ki53819@zut.edu.pl
#include <iostream>
#include <string>
#include <vector>

class Obj
{
public:
	Obj()=default;
	~Obj();
	double d;
	char c;
};

Obj::~Obj(){}

template <class T>
class Comporator
{
public:
	Comporator() = default;
	~Comporator();
	bool operator()(Obj x , Obj y)
	{
		return (x.d > y.d) ? true : (x.d < y.d) ? false : (x.c > y.c) ? true : false;
	}
	bool operator()(int x, int y)
	{
		return (x > y) ? true : false;
	}
};

template<class T>
Comporator<T>::~Comporator() {}

template <class T>
class HP
{
public:
	HP() = default;
	HP(T* vec2,int size, Comporator<T> comporator, bool status);
	~HP();
	int Size{ 0 }, MS{ 1 };
	T* vec = new T[MS];
	void Add(T value, Comporator<T> comporator);
	T Delete_Max(Comporator<T> comporator);
	void Clear(Comporator<T> comporator);
	void Print();
	void HP_Up(int index, Comporator<T> comporator);
	void HP_Down(int index, Comporator<T> comporator);
	void Sort(Comporator<T> comporator);
};

template<class T>
HP<T>::~HP()
{
	this->vec = nullptr;
	delete[] this->vec;
}

template<class T>
HP<T>::HP(T* vec2, int size, Comporator<T> comporator, bool status)
{
	for (int index = 0; index < size; ++index)
	{
		if (this->Size != this->MS)
		{
			this->vec[this->Size] = vec2[index];
			++this->Size;
		}
		else
		{
			this->MS *= 2;
			T* copy_val = this->vec;
			this->vec = nullptr;
			this->vec = new T[this->MS];
			for (int index2 = 0; index2 < this->Size; ++index2)
			{
				this->vec[index2] = copy_val[index2];
			}
			this->vec[this->Size] = vec2[index];
			++this->Size;
			copy_val = nullptr;
			delete[] copy_val;
		}
		if (this->Size != 1)
		{
			int index = this->Size - 1;
			if (status)
			{
				this->HP_Up(index, comporator);	
			}
			else
			{
				this->HP_Down(index, comporator);
			}
		}
	}
}

template<class T>
void HP<T>::Add(T value, Comporator<T> comporator)
{
	if (this->Size != this->MS)
	{
		this->vec[this->Size] = value;
		++this->Size;
	}
	else
	{
		this->MS *= 2;
		T* copy_val = this->vec;
		this->vec = nullptr;
		this->vec = new T[this->MS];
		for (int index = 0; index < this->Size; ++index)
		{
			this->vec[index] = copy_val[index];
		}
		this->vec[this->Size] = value;
		++this->Size;
		copy_val = nullptr;
		delete[] copy_val;
	}
	if (this->Size != 1)
	{
		int index = this->Size - 1;
		this->HP_Up(index, comporator);
	}
}

template<class T>
T HP<T>::Delete_Max(Comporator<T> comporator)
{
	if (this->Size == 0)
		return NULL;
	else if (this->Size == 1)
	{
		int MAX = this->vec[0];
		this->vec[0] = NULL;
		--this->Size;
		return MAX;
	}
	else
	{
		int MAX = this->vec[0];
		this->vec[0] = this->vec[this->Size - 1];
		this->vec[this->Size - 1] = NULL;
		this->HP_Down(0, comporator);
		--this->Size;
		return MAX;
	}
}

template<class T>
void HP<T>::Clear(Comporator<T> comporator)
{
	while (this->Size)
		this->Delete_Max(comporator);
}

template<class T>
void HP<T>::Print()
{
	std::cout << "\t| Binary Heap (Max) |\n\t| Size : " << this->Size << " |\n";
	std::string parent, left, right;
	for (int index = 0; index < this->Size; ++index)
	{
		if (index == 0)
			parent = "NULL";
		else
			parent = std::to_string(int(floor((index - 1) / 2)));
		if (((2 * index) + 1) < this->Size)
			left = std::to_string((2 * index) + 1);
		else
			left = "NULL";
		if (((2 * index) + 2) < this->Size)
			right = std::to_string((2 * index) + 2);
		else
			right = "NULL";
		std::cout << "| Index " << index << " | Parent : " << parent << " | Left : " << left << " | Right : " << right << " | Value : " << this->vec[index] << " |\n";
	}
}

template<class T>
void HP<T>::HP_Up(int index, Comporator<T> comporator)
{
	int index2;
	if (index <= 0)
		return;
	index2 = floor((index - 1) / 2);
	if (comporator(this->vec[index], this->vec[index2]))
	{
		T val = this->vec[index];
		this->vec[index] = this->vec[index2];
		this->vec[index2] = val;
		index = index2;
	}
	else
		return;
	HP_Up(index, comporator);
	return;
}

template<class T>
void HP<T>::HP_Down(int index, Comporator<T> comporator)
{
	if ((((2 * index) + 1) < this->Size) || (((2 * index) + 2) < this->Size))
	{
		if (((((2 * index) + 1) < this->Size) && (((2 * index) + 2) >= this->Size)) && (this->vec[index] < this->vec[(2 * index) + 1]))
		{
			T val = this->vec[index];
			this->vec[index] = this->vec[(2 * index) + 1];
			this->vec[(2 * index) + 1] = val;
			index = (2 * index) + 1;
			HP_Down(index, comporator);
		}
		else if (((((2 * index) + 1) >= this->Size) && (((2 * index) + 2) < this->Size)) && (this->vec[index] < this->vec[(2 * index) + 2]))
		{
			T val = this->vec[index];
			this->vec[index] = this->vec[(2 * index) + 2];
			this->vec[(2 * index) + 2] = val;
			index = (2 * index) + 2;
			HP_Down(index, comporator);
		}
		else if ((comporator(this->vec[(2 * index) + 1], this->vec[(2 * index) + 2])) && (this->vec[index] < this->vec[(2 * index) + 1]))
		{
			T val = this->vec[index];
			this->vec[index] = this->vec[(2 * index) + 1];
			this->vec[(2 * index) + 1] = val;
			index = (2 * index) + 1;
			HP_Down(index, comporator);
		}
		else if ((comporator(this->vec[(2 * index) + 2], this->vec[(2 * index) + 1])) && (this->vec[index] < this->vec[(2 * index) + 2]))
		{
			T val = this->vec[index];
			this->vec[index] = this->vec[(2 * index) + 2];
			this->vec[(2 * index) + 2] = val;
			index = (2 * index) + 2;
			HP_Down(index, comporator);
		}
		else
			return;
	}
	else
		return;
	return;
}

template<class T>
void HP<T>::Sort(Comporator<T> comporator)
{
	int size2 = this->Size;
	int max = this->vec[0];
	int max_index = 0;
	for (int index = 0; index < size2;--size2)
	{
		for (int index2 = 0; index2 < size2; ++index2)
		{
			if (this->vec[index2] > max)
			{
				max = this->vec[index2];
				max_index = index2;
			}
		}
		HP_Up(max_index, comporator);
		T value = this->vec[0];
		this->vec[0] = this->vec[size2 - 1];
		this->vec[size2 - 1] = value;
		max = this->vec[0];
		max_index = 0;
	}
}

void C_Sort(int* vec2, int size, int m)
{
	std::vector<int> vec3;
	std::vector<int> res;
	for (int index = 0; index < m; ++index)
	{
		vec3.push_back(0);
		res.push_back(vec2[index]);
	}
	for (int index = 0; index < size; ++index)
	{
		int index2 = vec2[index];
		vec3[index2] += 1;
	}
	for (int index = 1; index < m; ++index)
	{
		vec3[index] += vec3[index - 1];
	}
	for (int index = 0; index < size; ++index)
	{
		int index2 = vec2[index];
		int index3 = vec3[index2] - 1;
		res[index3] = vec2[index];
		if (vec3[index2] != 0)
			vec3[index2] -= 1;
	}
	for (int index = 0; index < size; ++index)
	{
		vec2[index] = res[index];
	}
}

int main_ints()
{
	int* array1 = new int[7];
	Comporator<int> comporator;
	array1[0] = 1;
	array1[1] = 4;
	array1[2] = 1;
	array1[3] = 2;
	array1[4] = 7;
	array1[5] = 5;
	array1[6] = 2;
	for (int index = 0; index < 7; ++index)
	{
		std::cout << array1[index] << " ";
	}
	std::cout << "\n";
	C_Sort(array1, 7, 9);
	for (int index = 0; index < 7; ++index)
	{
		std::cout << array1[index] << " ";
	}
	return 0;
}

int main_objs()
{
	//
	return 0;
}

int main()
{
	main_ints();
	main_objs();
    return 0;
}
