// ALGO2 IS1 224B LAB06
// ILLIA KARMAZIN
// ki53819@zut.edu.pl
#include <iostream>
#include <string>
#include <vector>
#include <functional>

class Obj
{
public:
	Obj()=default;
	Obj(double D, char C);
	~Obj();
	double d;
	char c;
};

Obj::Obj(double D, char C) :d(D), c(C){}

Obj::~Obj(){}

template <class T>
class Comporator
{
public:
	Comporator() = default;
	~Comporator();
	bool operator()(int x , int y)
	{
		return (x > y) ? true : false;;
	}
	bool operator()(double x, double y)
	{
		return (x > y) ? true : false;;
	}
	bool operator()(Obj* x,Obj* y)
	{
		if (x->d > y->d)
			return true;
		else if (x->d < y->d)
			return false;
		else
			return (x->c > y->c) ? true : false;
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
	T max = this->vec[0];
	int max_index = 0;
	for (int index = 0; index < size2;--size2)
	{
		for (int index2 = 0; index2 < size2; ++index2)
		{
			if (comporator(this->vec[index2],max))
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

void C_Sort(Obj* vec2, int size, int m,Comporator<char> comparator)
{
	std::vector<int> vec3;
	std::vector<Obj> res;
	for (int index = 0; index < m; ++index)
	{
		vec3.push_back(0);
		res.push_back(vec2[index]);
	}
	for (int index = 0; index < size; ++index)
	{
		int index2 = vec2[index].c;
		vec3[index2] += 1;
	}
	for (int index = 1; index < m; ++index)
	{
		vec3[index] += vec3[index - 1];
	}
	for (int index = 0; index < size; ++index)
	{
		int index2 = vec2[index].c;
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

template <class T>
void C_Sort(Obj* vec2, int size, int m, Comporator<double> comparator)
{
	std::vector<double> vec3;
	std::vector<Obj> res;
	for (int index = 0; index < m; ++index)
	{
		vec3.push_back(0);
	}
	for (int index = 0; index < size; ++index)
	{
		res.push_back(vec2[index]);
	}
	int i, j, min_idx;
	for (i = 0; i < size - 1; i++) 
	{
		min_idx = i;
		for (j = i + 1; j < size; j++) {
			if (res[j].d < res[min_idx].d)
				min_idx = j;
		}
		if (min_idx != i)
			std::swap(res[min_idx], res[i]);
	}
	for (int index = 0; index < size; ++index)
	{
		vec2[index] = res[index];
	}
}

void B_Sort(int* vec2, int size, int m)
{
	int Max = vec2[0], Min = vec2[0] , b1_s = 0 , b2_s = 0 , b3_s = 0 , b4_s = 0 , b5_s = 0;
	for (int index = 0; index < size; ++index)
	{
		Max = (vec2[index] > Max)?vec2[index]:Max;
		Min = (vec2[index] < Min) ? vec2[index] : Min;
	}
	int b_size = (Max - Min) / 5;
	int* b1 = new int[size];
	int* b2 = new int[size];
	int* b3 = new int[size];
	int* b4 = new int[size];
	int* b5 = new int[size];
	for (int index = 0; index < size; ++index)
	{
		int index2 = int(vec2[index] / b_size);
		switch (index2)
		{
		case 0:
			b1[b1_s++] = vec2[index];
			break;
		case 1:
			b2[b2_s++] = vec2[index];
			break;
		case 2:
			b3[b3_s++] = vec2[index];
			break;
		case 3:
			b4[b4_s++] = vec2[index];
			break;
		case 4:
			b5[b5_s++] = vec2[index];
			break;
		default:
			b5[b5_s++] = vec2[index];
			break;
		}
	}
	C_Sort(b1, b1_s, m);
	C_Sort(b2, b2_s, m);
	C_Sort(b3, b3_s, m);
	C_Sort(b4, b4_s, m);
	C_Sort(b5, b5_s, m);
	int index2 = 0;
	for (int index = 0; index < b1_s; ++index)
	{
		vec2[index2++] = b1[index];
	}
	for (int index = 0; index < b2_s; ++index)
	{
		vec2[index2++] = b2[index];
	}
	for (int index = 0; index < b3_s; ++index)
	{
		vec2[index2++] = b3[index];
	}
	for (int index = 0; index < b4_s; ++index)
	{
		vec2[index2++] = b4[index];
	}
	for (int index = 0; index < b5_s; ++index)
	{
		vec2[index2++] = b5[index];
	}
}

char Key_C(Obj* obj) 
{ 
	return obj->c; 
}

double Key_D(Obj* obj) 
{ 
	return obj->d; 
}


void B_Sort(Obj** vec2, int size, int m, char(*Key_C)(Obj*), Comporator<char> comporator)
{
	Obj* Max = vec2[0];
	Obj* Min = vec2[0];
	int b1_s = 0, b2_s = 0, b3_s = 0, b4_s = 0, b5_s = 0;
	for (int index = 0; index < size; ++index)
	{
		Max = (comporator(Key_C(vec2[index]),Key_C(Max))) ? vec2[index] : Max;
		Min = (comporator(Key_C(Min),Key_C(vec2[index]))) ? vec2[index] : Min;
	}
	int b_size = (Key_C(Max) - Key_C(Min)) / 5;
	Obj* b1 = new Obj[size];
	Obj* b2 = new Obj[size];
	Obj* b3 = new Obj[size];
	Obj* b4 = new Obj[size];
	Obj* b5 = new Obj[size];
	for (int index = 0; index < size; ++index)
	{
		if (b_size == 0)
		{
			b_size = 1;
		}
		int index2 = int(Key_C(vec2[index]) / b_size);
		switch (index2)
		{
		case 0:
			b1[b1_s++] = *vec2[index];
			break;
		case 1:
			b2[b2_s++] = *vec2[index];
			break;
		case 2:
			b3[b3_s++] = *vec2[index];
			break;
		case 3:
			b4[b4_s++] = *vec2[index];
			break;
		case 4:
			b5[b5_s++] = *vec2[index];
			break;
		default:
			b5[b5_s++] = *vec2[index];
			break;
		}
	}
	C_Sort(b1, b1_s, m,comporator);
	C_Sort(b2, b2_s, m, comporator);
	C_Sort(b3, b3_s, m, comporator);
	C_Sort(b4, b4_s, m, comporator);
	C_Sort(b5, b5_s, m, comporator);
	int index2 = 0;
	for (int index = 0; index < b1_s; ++index)
	{
		vec2[index2++] = &b1[index];
	}
	for (int index = 0; index < b2_s; ++index)
	{
		vec2[index2++] = &b2[index];
	}
	for (int index = 0; index < b3_s; ++index)
	{
		vec2[index2++] = &b3[index];
	}
	for (int index = 0; index < b4_s; ++index)
	{
		vec2[index2++] = &b4[index];
	}
	for (int index = 0; index < b5_s; ++index)
	{
		vec2[index2++] = &b5[index];
	}
}

void B_Sort(Obj** vec2, int size, int m, double(*Key_D)(Obj*), Comporator<double> comporator)
{
	Obj* Max = vec2[0];
	Obj* Min = vec2[0];
	int b1_s = 0, b2_s = 0, b3_s = 0, b4_s = 0, b5_s = 0;
	for (int index = 0; index < size; ++index)
	{
		Max = (comporator(Key_D(vec2[index]), Key_D(Max))) ? vec2[index] : Max;
		Min = (comporator(Key_D(Min), Key_D(vec2[index]))) ? vec2[index] : Min;
	}
	int b_size = (Key_D(Max) - Key_D(Min)) / 5;
	Obj* b1 = new Obj[size];
	Obj* b2 = new Obj[size];
	Obj* b3 = new Obj[size];
	Obj* b4 = new Obj[size];
	Obj* b5 = new Obj[size];
	for (int index = 0; index < size; ++index)
	{
		int index2 = int(Key_D(vec2[index]) / b_size);
		switch (index2)
		{
		case 0:
			b1[b1_s++] = *vec2[index];
			break;
		case 1:
			b2[b2_s++] = *vec2[index];
			break;
		case 2:
			b3[b3_s++] = *vec2[index];
			break;
		case 3:
			b4[b4_s++] = *vec2[index];
			break;
		case 4:
			b5[b5_s++] = *vec2[index];
			break;
		default:
			b5[b5_s++] = *vec2[index];
			break;
		}
	}
	C_Sort<Obj*>(b1, b1_s, m, comporator);
	for (int q = 0; q < b1_s; ++q)
	{
		std::cout << b1[q].d << " ";
	}
	std::cout << std::endl;
	C_Sort<Obj*>(b2, b2_s, m, comporator);
	for (int q = 0; q < b2_s; ++q)
	{
		std::cout << b2[q].d << " ";
	}
	std::cout << std::endl;
	C_Sort<Obj*>(b3, b3_s, m,comporator);
	for (int q = 0; q < b3_s; ++q)
	{
		std::cout << b3[q].d << " ";
	}
	std::cout << std::endl;
	C_Sort<Obj*>(b4, b4_s, m, comporator);
	for (int q = 0; q < b4_s; ++q)
	{
		std::cout << b4[q].d << " ";
	}
	std::cout << std::endl;
	C_Sort<Obj*>(b5, b5_s, m, comporator);
	for (int q = 0; q < b5_s; ++q)
	{
		std::cout << b5[q].d << " ";
	}
	std::cout << std::endl;
	int index2 = 0;
	for (int index = 0; index < b1_s; ++index)
	{
		vec2[index2++] = &b1[index];
	}
	for (int index = 0; index < b2_s; ++index)
	{
		vec2[index2++] = &b2[index];
	}
	for (int index = 0; index < b3_s; ++index)
	{
		vec2[index2++] = &b3[index];
	}
	for (int index = 0; index < b4_s; ++index)
	{
		vec2[index2++] = &b4[index];
	}
	for (int index = 0; index < b5_s; ++index)
	{
		vec2[index2++] = &b5[index];
	}
}

int main_ints()
{
	//
	return 0;
}

int main_objs()
{
	//
	return 0;
}

int main()
{
	Obj** array = new Obj*[10];
	Comporator<char> cmpchar;
	Comporator<double> cmpdouble;
	Comporator<Obj*> cmpobj;
	Obj* obj1 = new Obj(1.4,'c');
	array[0] = obj1;
	Obj* obj2 = new Obj(1.6, 'a');
	array[1] = obj2;
	Obj* obj3 = new Obj(2.4, 'f');
	array[2] = obj3;
	Obj* obj4 = new Obj(5.4, 'g');
	array[3] = obj4;
	Obj* obj5 = new Obj(1.4, 'n');
	array[4] = obj5;
	Obj* obj6 = new Obj(0.04, 'c');
	array[5] = obj6;
	Obj* obj7 = new Obj(5.78, 't');
	array[6] = obj7;
	Obj* obj8 = new Obj(0.12, 'y');
	array[7] = obj8;
	Obj* obj9 = new Obj(1.5, 'q');
	array[8] = obj9;
	Obj* obj10 = new Obj(3.9, 'w');
	array[9] = obj10;
	for (int i = 0; i < 10; ++i)
	{
		std::cout << array[i]->c << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		std::cout << array[i]->d << " ";
	}
	std::cout << std::endl;
	B_Sort(array,10,300,&Key_C,cmpchar);
	for (int i = 0; i < 10; ++i)
	{
		std::cout << array[i]->c << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		std::cout << array[i]->d << " ";
	}
	main_ints();
	main_objs();
    return 0;
}
