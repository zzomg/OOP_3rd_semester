#include <iostream>
#include <cstdlib>
#include <cassert>
#include <unordered_map>
#include <map>

using uint = unsigned int;

enum Trit : uint { False = 2 , Unknown = 0, True = 1 };
//logic values: False = 10, Unknown = 00, True = 01

class TritSet
{
	uint* storage_; //внутреннее хранилище
	size_t size_; //количество ТРИТОВ в массиве
	size_t capacity_; //количество UINT'ОВ в массиве

public:

	class reference
	{
		friend class TritSet;

		uint* word_;
		size_t bit_pos_;

		reference();

	public:
		reference(TritSet& tritset, size_t idx);
		~reference() {};

		reference & operator=(int value);
		reference& operator=(reference & other);
	};

	friend class reference;

	//constructors & destructor
	TritSet(size_t size = 0);
	TritSet(const TritSet& other);
	TritSet(TritSet&& other);
	~TritSet();

	//operators overloading
	TritSet& operator=(TritSet & other);
	reference operator[](size_t idx);
	uint operator[](size_t idx) const;

	friend TritSet operator&(const TritSet& left, const TritSet& right)
	{
		size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
		TritSet res(res_size);
		for (size_t i = 0; i < res_size; ++i) {
			//False & (anything)
			if (left[i] == False || right[i] == False) {
				res[i] = False;
			}
			//Unknown & True
			else if ((left[i] == Unknown && right[i] == True) ||
				(right[i] == Unknown && left[i] == True)) {
				res[i] = Unknown;
			}
			//Unknown & Unknown
			else if (left[i] == Unknown && right[i] == Unknown) {
				res[i] = Unknown;
			}
			//True & True
			else if (left[i] == True && right[i] == True) {
				res[i] = True;
			}
			//default
			else res[i] = Unknown;
		}

		return res;
	}

	friend TritSet operator|(const TritSet& left, const TritSet& right)
	{
		size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
		TritSet res(res_size);
		for (size_t i = 0; i < res_size; ++i) {
			//True | (anything)
			if (left[i] == True || right[i] == True) {
				res[i] = True;
			}
			//Unknown | False
			else if ((left[i] == Unknown && right[i] == False) || 
				(right[i] == Unknown && left[i] == False)) {
				res[i] = Unknown;
			}
			//Unknown | Unknown
			else if (left[i] == Unknown && right[i] == Unknown) {
				res[i] = Unknown;
			}
			//False | False
			else if (left[i] == False && right[i] == False) {
				res[i] = False;
			}
			//default
			else res[i] = Unknown;
		}

		return res;
	}

	friend TritSet operator~(const TritSet& obj)
	{
		size_t res_size = obj.size_;
		TritSet res(res_size);
		for (size_t i = 0; i < res_size; ++i) {
			//~True
			if(obj[i] == True) {
				res[i] = False;
			}
			//~False
			else if (obj[i] == False) {
				res[i] = True;
			}
			//~Unknown
			else if (obj[i] == Unknown) {
				res[i] = Unknown;
			}
			//default
			else res[i] = Unknown;
		}

		return res;
	}

	//member functions
	void shrink();
	size_t length();
	void trim(size_t idx);
	size_t cardinality(int value);
	size_t get_capacity() { return capacity_; }
	//std::unordered_map< Trit, int, std::hash<int> > cardinality();
	std::map< int, size_t > cardinality();

	//utilities
	void reallocMemory(size_t idx);
	static size_t tritsToBytes(size_t trit_n) {
		return (trit_n * 2 / 8 / sizeof(uint));
	}
	static size_t tritsInByte() {
		return (sizeof(uint) * 8 / 2); 
	}
	
}; 

std::map< int, size_t > TritSet::cardinality() 
{
	std::map< int, size_t > values;
	int value = Unknown;
	for (size_t i = 0; i < tritsInByte() * length(); ++i) {
		value = (const_cast<const TritSet&>(*this))[i];
		++values[value];
	} 

	/*for (auto& t : values)
		std::cout << t.first << " "
		<< t.second.first << " "
		<< t.second.second << "\n";*/

	return values;
}

void TritSet::trim(size_t idx) 
{
	for (size_t i = idx; i < tritsInByte() * length(); ++i) {
		(*this)[i] = Unknown;
	}
}

size_t TritSet::length() {
	size_t last_set_idx = 0;
	int met_set = 0;
	for (size_t i = 0; i < size_; ++i) {
		if (storage_[i] != 0) {
			if (!met_set) {
				met_set = 1;
				last_set_idx = i;
			}
			continue;
		}
		else {
			if (met_set) {
				met_set = 0;
			}
			continue;
		}
	}
	return last_set_idx + 1;
}

size_t TritSet::cardinality(int value) //кол-во тритов типа value 
{
	size_t count = 0;
	for (size_t i = 0; i < tritsInByte() * length(); ++i) {
		if((const_cast<const TritSet&>(*this))[i] == value) {
			++count;
		}
	}
	return count;
}

//std::unordered_map< Trit, int, std::hash<int> > TritSet::cardinality() {}

void TritSet::shrink(void)
{
	size_t last_set = length();
	uint* shrunk_storage = (uint*)malloc(last_set * sizeof(uint));
	for (size_t i = 0; i < last_set; ++i) {
		shrunk_storage[i] = storage_[i];
	}
	free(storage_);
	storage_ = shrunk_storage;
	capacity_ = last_set;
	size_ = tritsInByte(); //по идее нужно рассчитать кол-во до последнего установленного трита
}

TritSet::reference& TritSet::reference::operator=(reference & other) 
{
	uint mask = 0x03; //0000...00011
	uint* other_copy = other.word_;
	(*word_) &= ~(mask << (sizeof(uint) * 8 - 2 - bit_pos_));
	//инвертировали маску, обнулили конкретные биты
	(*other_copy) &= (mask << (sizeof(uint) * 8 - 2 - other.bit_pos_));
	//обнулили все биты кроме тех, которые хотим передать
	(*other_copy) <<= (other.bit_pos_ - bit_pos_);
	(*word_) |= (*other_copy);
	//совместили маску и то, что хотели изменить
	return *this;
}

TritSet::reference & TritSet::reference::operator=(int value) 
{
	uint mask = 0x03; //0000...00011
	(*word_) &= ~(mask << (sizeof(uint) * 8 - 2 - bit_pos_));
	//инвертировали маску, обнулили конкретные биты
	(*word_) |= (value << (sizeof(uint) * 8 - 2 - bit_pos_));

	return *this;
}

TritSet::reference::reference(TritSet& tritset, size_t idx) 
{
	word_ = &tritset.storage_[tritsToBytes(idx)];
	bit_pos_ = 2 * (idx % (sizeof(uint) * 8 / 2)); //первый бит трита
	//делим на 2 чтобы получить кол-во тритов в uint'e
}

TritSet::reference TritSet::operator[](size_t idx) 
{
	if (idx >= size_) {
		reallocMemory(idx);
	}
	return reference(*this, idx);
}

uint TritSet::operator[](size_t idx) const 
{
	uint return_value = Unknown;
	if (idx < size_) {
		uint mask = 0x03;
		uint word = storage_[tritsToBytes(idx)];
		size_t bit_pos = 2 * (idx % (sizeof(uint) * 8 / 2));
		word >>= (sizeof(uint) * 8 - 2 - bit_pos);
		return_value = mask & word;
	}
	return return_value;
}

TritSet::TritSet(const TritSet& other) //Конструктор копирования
{
	size_ = other.size_;
	capacity_ = other.capacity_;
	storage_ = (uint*)malloc(capacity_ * sizeof(uint));
	for (size_t i = 0; i < capacity_; ++i) {
		storage_[i] = other.storage_[i];
	}
}

TritSet::TritSet(size_t size): size_(size) //Конструктор по умолчанию
{
	if (size == 0) {
		capacity_ = 0;
		storage_ = nullptr;
	}
	else {
		capacity_ = tritsToBytes(size) + 1;
		storage_ = (uint*)malloc(capacity_ * sizeof(uint));
	}
	for (size_t i = 0; i < capacity_; ++i) { storage_[i] = 0; }
}

void TritSet::reallocMemory(size_t idx) 
{
	size_t new_capacity_ = tritsToBytes(idx) + 1;
	uint* tmp = (uint*)malloc(new_capacity_ * sizeof(uint));
	for (size_t i = 0; i < capacity_; ++i) { 
		tmp[i] = storage_[i]; 
	}
	for (size_t i = capacity_; i < new_capacity_; ++i) { 
		tmp[i] = 0; 
	}
	capacity_ = new_capacity_;
	size_ = idx + 1;
	free(storage_);
	storage_ = tmp;
}

TritSet & TritSet::operator=(TritSet & other) //оператор присваивания состояния одного объекта другому
{
	if (this != &other) {
		free(storage_);
		size_ = other.size_;
		capacity_ = other.capacity_;
		storage_ = (uint*)malloc(capacity_ * sizeof(uint));
		for (size_t i = 0; i < capacity_; ++i) { 
			storage_[i] = other.storage_[i]; 
		}
		//Тут по идее нужно применить метод trim
	}
	return *this;
}

TritSet::TritSet(TritSet&& other) //Move-конструктор
{
	size_ = other.size_;
	capacity_ = other.capacity_;
	free(storage_);
	storage_ = other.storage_;
	other.storage_ = nullptr;
	other.capacity_ = 0;
	other.size_ = 0;
}

TritSet::~TritSet() //деструктор
{
	free(storage_);
}

int main()
{
	TritSet setA(3);
	setA[0] = True;
	setA[1] = Unknown;
	setA[2] = False;

	TritSet setB(2);
	setB[0] = False;
	setB[1] = True;

	TritSet setC = (setA & setB); //{False, Unknown, False, Unknown, Unknown, ..., Unknown}

	setC.cardinality();

	/*int FalseCnt = setC.cardinality(False);
	int UnkCnt = setC.cardinality(Unknown);

	std::cout << "FalseCnt = " << FalseCnt << std::endl;
	std::cout << "UnkCnt = " <<UnkCnt << std::endl;

	setC.trim(2);

	FalseCnt = setC.cardinality(False);
	std::cout << "FalseCnt = " << FalseCnt << std::endl;*/

	return 0;
}
