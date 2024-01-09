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
	for (int index = 0; index < size; ++index)
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
	C_Sort<Obj*>(b2, b2_s, m, comporator);
	C_Sort<Obj*>(b3, b3_s, m,comporator);
	C_Sort<Obj*>(b4, b4_s, m, comporator);
	C_Sort<Obj*>(b5, b5_s, m, comporator);
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
	Comporator<int> comporator_int;
	int n = pow(10, 3);
	int m = pow(10, 3);
	int* vec1 = new int[n];
	for (int index2 = 0; index2 < n; ++index2)
	{
		int RV = rand() % n;
		vec1[index2] = RV;
	}
	std::cout << "Array 1 : ";
	for (int index2 = 0; index2 < n; ++index2)
	{
		std::cout << vec1[index2] << "\n";
	}
	std::cout << std::endl;
	int* vec2 = new int[n];
	int* vec3 = new int[n];
	memcpy(vec2, vec1, n * sizeof(int));
	memcpy(vec3, vec1, n * sizeof(int));
	clock_t timer1 = clock();
	C_Sort(vec1, n, m);
	clock_t timer2 = clock();
	std::cout << "| Full time : " << (timer2 - timer1) / (double)CLOCKS_PER_SEC << " s |\n";
	std::cout << "Sorted array 1 : ";
	for (int index2 = 0; index2 < n; ++index2)
	{
		std::cout << vec1[index2] << "\n";
	}
	//
	timer1 = clock();
	HP<int>* hp = new HP<int>(vec2, n, comporator_int, true);
	hp->Sort(comporator_int);
	timer2 = clock();
	std::cout << "| Full time : " << (timer2 - timer1) / (double)CLOCKS_PER_SEC << " s |\n";
	hp->Print();
	// 
	timer1 = clock();
	B_Sort(vec3, n, m);
	timer2 = clock();
	std::cout << "| Full time : " << (timer2 - timer1) / (double)CLOCKS_PER_SEC << " s |\n";
	std::cout << "Sorted array 3 : ";
	for (int index2 = 0; index2 < n; ++index2)
	{
		std::cout << vec3[index2] << "\n";
	}
	//
	std::cout << "Comparing\n";
	for (int index2 = 0; index2 < n; ++index2)
	{
		if (vec1[index2] == hp->vec[index2] && vec3[index2] == hp->vec[index2] && vec1[index2] == vec3[index2])
			std::cout << vec1[index2] << "\n";
		else
		{
			std::cout << "Wrong comparison" << "\n";
			break;
		}
	}
	delete[] vec1;
	delete[] vec2;
	delete[] vec3;
	return 0;
}

int main_objs()
{
	Comporator<Obj*> comporator_obj;
	Comporator<double> comporator_double;
	Comporator<char> comporator_char;
	int n = pow(10, 3);
	int m = pow(10, 3);
	Obj** vec1 = new Obj*[n];
	for (int index = 0; index < n; ++index)
	{
		Obj* obj = new Obj();
		obj->c = 'a' + rand() % 26;
		obj->d = ((rand() << 15) + rand()) / ((double)pow(2, 30));
		vec1[index] = obj;
	}
	std::cout << "Array 1 : ";
	for (int index = 0; index < n; ++index)
	{
		std::cout << "| ID : " << index+1 << "| D : " << vec1[index]->d << "| C : " << vec1[index]->c <<"\n";
	}
	std::cout << std::endl;
	Obj** vec2 = new Obj * [n];
	memcpy(vec2, vec1, n * sizeof(Obj*));
	clock_t timer1 = clock();
	HP<Obj*>* hp = new HP<Obj*>(vec1, n, comporator_obj, true);
	hp->Sort(comporator_obj);
	clock_t timer2 = clock();
	std::cout << "| Full time : " << (timer2 - timer1) / (double)CLOCKS_PER_SEC << " s |\n";
	std::cout << "Sorted array 1 : ";
	for (int index = 0; index < n; ++index)
	{
		std::cout << "| ID : " << index + 1 << "| D : " << hp->vec[index]->d << "| C : " << hp->vec[index]->c << "\n";
	}
	std::cout << std::endl;
	//
	timer1 = clock();
	B_Sort(vec2, n, 1, &Key_D, comporator_double);
	timer2 = clock();
	std::cout << "| Full time : " << (timer2 - timer1) / (double)CLOCKS_PER_SEC << " s |\n";
	std::cout << "Sorted array 2 : ";
	for (int index = 0; index < n; ++index)
	{
		std::cout << "| ID : " << index + 1 << "| D : " << vec2[index]->d << "| C : " << vec2[index]->c << "\n";
	}
	std::cout << std::endl;
	//
	std::cout << "Comparing\n";
	for (int index = 0; index < n; ++index)
	{
		if (hp->vec[index]->d == vec2[index]->d)
			std::cout << vec2[index]->d << "\n";
		else
		{
			std::cout << "Wrong comparison" << "\n";
			break;
		}
	}
	delete[] vec1;
	delete[] vec2;
	return 0;
}

int main()
{
	//main_ints();
	main_objs();
    return 0;
}
