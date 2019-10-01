#include <iostream>
#include <map>
#include "task1.h"

TritSet operator~(const TritSet& obj)
{
	size_t res_size = obj.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i) {
		//~True
		if (obj[i] == True) {
			res[i] = False;
		}
		//~False
		else if (obj[i] == False) {
			res[i] = True;
		}
		//~Unknown or default
		else res[i] = Unknown;
	}

	return res;
}

TritSet operator&(const TritSet& left, const TritSet& right)
{
	size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i) {
		//False & (anything)
		if (left[i] == False || right[i] == False) {
			res[i] = False;
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

TritSet operator|(const TritSet& left, const TritSet& right)
{
	size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i) {
		//True | (anything)
		if (left[i] == True || right[i] == True) {
			res[i] = True;
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

std::map< int, size_t > TritSet::cardinality() //кол-во тритов всех видов
{
	std::map< int, size_t > values;
	size_t len = length();
	const TritSet& trit_const = const_cast<const TritSet&>(*this);

	for (size_t i = 0; i < len; ++i) {
		++values[trit_const[i]];
	} 

	return values;
}


size_t TritSet::cardinality(int value) //кол-во тритов типа value 
{
	size_t count = 0;
	size_t len = length();
	const TritSet& trit_const = const_cast<const TritSet&>(*this);

	for (size_t i = 0; i < len; ++i) {
		if (trit_const[i] == value) {
			++count;
		}
	}
	return count;
}

void TritSet::trim(size_t idx) //"забыть" триты после idx
{
	size_t len = length();

	for (size_t i = idx; i < len; ++i) {
		(*this)[i] = Unknown;
	}
}

size_t TritSet::length() //размер в тритах до послед. устан. трита
{ 
	size_t last_set_idx = 0;
	int met_set = 0;
	const TritSet& trit_const = const_cast<const TritSet&>(*this);

	for (size_t i = 0; i < size_; ++i) {
		if (trit_const[i] != Unknown) {
			last_set_idx = i;
		}
	}

	return last_set_idx + 1;
}

void TritSet::shrink(void) //сжать тритсет до размера послед. устан. трита
{
	size_t len = tritsToBytes(length());
	uint* shrunk_storage = new uint[len];

	for (size_t i = 0; i < len; ++i) {
		shrunk_storage[i] = storage_[i];
	}

	delete[] storage_;
	storage_ = shrunk_storage;
	capacity_ = len;
	size_ = tritsInByte();
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

void TritSet::reallocMemory(size_t idx)
{
	size_t new_capacity_ = tritsToBytes(idx) + 1;
	uint* tmp = new uint[new_capacity_];

	for (size_t i = 0; i < capacity_; ++i) {
		tmp[i] = storage_[i];
	}
	for (size_t i = capacity_; i < new_capacity_; ++i) {
		tmp[i] = 0;
	}

	capacity_ = new_capacity_;
	size_ = idx + 1;
	delete[] storage_;
	storage_ = tmp;
}

TritSet::reference::reference(TritSet& tritset, size_t idx)
{
	word_ = &tritset.storage_[tritsToBytes(idx)];
	bit_pos_ = 2 * (idx % (sizeof(uint) * 8 / 2)); //первый бит трита
	//делим на 2 чтобы получить кол-во тритов в uint'e
}

TritSet::TritSet(const TritSet& other) //Конструктор копирования
{
	size_ = other.size_;
	capacity_ = other.capacity_;
	storage_ = new uint[capacity_];

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
		storage_ = new uint[capacity_];
	}
	for (size_t i = 0; i < capacity_; ++i) { storage_[i] = 0; }
}

TritSet & TritSet::operator=(TritSet & other) //оператор присваивания состояния одного объекта другому
{
	if (this != &other) {
		delete[] storage_;
		size_ = other.size_;
		capacity_ = other.capacity_;
		storage_ = new uint[capacity_];
		for (size_t i = 0; i < capacity_; ++i) { 
			storage_[i] = other.storage_[i]; 
		}
	}
	return *this;
}

TritSet::TritSet(TritSet&& other) //Move-конструктор
{
	size_ = other.size_;
	capacity_ = other.capacity_;
	delete[] storage_;
	storage_ = other.storage_;
	other.storage_ = nullptr;
	other.capacity_ = 0;
	other.size_ = 0;
}

TritSet::~TritSet() //деструктор
{
	delete[] storage_;
}

/*
#include "stdafx.h"
#include "targetver.h"
#include "task1.h"
#include <iostream>
#include <map>


TritSet operator~(const TritSet& obj)
{
	size_t res_size = obj.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i) {
		//~True
		if (obj[i] == True) {
			res[i] = False;
		}
		//~False
		else if (obj[i] == False) {
			res[i] = True;
		}
		//~Unknown or default
		else res[i] = Unknown;
	}

	return res;
}

TritSet operator&(const TritSet& left, const TritSet& right)
{
	size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i) {
		res[i] = left[i] & right[i];
		//False & (anything)
		if (left[i] == False || right[i] == False) {
			res[i] = False;
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

TritSet operator|(const TritSet& left, const TritSet& right)
{
	size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i) {
		//True | (anything)
		if (left[i] == True || right[i] == True) {
			res[i] = True;
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

std::map< int, size_t > TritSet::cardinality() //кол-во тритов всех видов
{
	std::map< int, size_t > values;
	size_t len = length();
	const TritSet& trit_const = const_cast<const TritSet&>(*this);

	for (size_t i = 0; i < len; ++i) {
		++values[trit_const[i]];
	}

	return values;
}


size_t TritSet::cardinality(int value) //кол-во тритов типа value 
{
	size_t count = 0;
	size_t len = length();
	const TritSet& trit_const = const_cast<const TritSet&>(*this);

	for (size_t i = 0; i < len; ++i) {
		if (trit_const[i] == value) {
			++count;
		}
	}
	return count;
}

void TritSet::trim(size_t idx) //"забыть" триты после idx
{
	size_t len = length();

	for (size_t i = idx; i < len; ++i) {
		(*this)[i] = Unknown;
	}
}

size_t TritSet::length() //размер в тритах до послед. устан. трита
{
	size_t last_set_idx = 0;
	int met_set = 0;
	const TritSet& trit_const = const_cast<const TritSet&>(*this);

	for (size_t i = 0; i < size_; ++i) {
		if (trit_const[i] != Unknown) {
			last_set_idx = i;
		}
	}

	return last_set_idx + 1;
}

void TritSet::shrink(void) //сжать тритсет до размера послед. устан. трита
{
	size_t len = tritsToBytes(length());
	uint* shrunk_storage = new uint[len];

	for (size_t i = 0; i < len; ++i) {
		shrunk_storage[i] = storage_[i];
	}

	delete[] storage_;
	storage_ = shrunk_storage;
	capacity_ = len;
	size_ = tritsInByte();
}

TritSet::reference::operator Trit() const 
{
	uint value;

	return (Trit)value;
}

TritSet::reference& TritSet::reference::operator=(const reference & other)
{
	*this = (Trit)other;
	return *this;

	uint mask = 0x03; //0000...00011
	uint* other_copy = other.word_;

	(*word_) &= ~(mask << (sizeof(uint)* 8 - 2 - bit_pos_));
	//инвертировали маску, обнулили конкретные биты
	(*other_copy) &= (mask << (sizeof(uint)* 8 - 2 - other.bit_pos_));
	//обнулили все биты кроме тех, которые хотим передать
	(*other_copy) <<= (other.bit_pos_ - bit_pos_);
	(*word_) |= (*other_copy);
	//совместили маску и то, что хотели изменить
	return *this;
}

TritSet::reference & TritSet::reference::operator=(Trit value)
{
	if (index >= tritset.size_) {
		if (value != Unknown) {
			reallocMemory(tritset, index);
		}
	}
	uint mask = 0x03; //0000...00011
	(*word_) &= ~(mask << (sizeof(uint)* 8 - 2 - bit_pos_));
	//инвертировали маску, обнулили конкретные биты
	(*word_) |= (value << (sizeof(uint)* 8 - 2 - bit_pos_));

	return *this;
}

TritSet::reference TritSet::operator[](size_t idx)
{
	return reference(*this, idx);
}

Trit TritSet::operator[](size_t idx) const
{
	uint return_value = Unknown;
	if (idx < size_) {
		uint mask = 0x03;
		uint word = storage_[tritsToBytes(idx)];
		size_t bit_pos = 2 * (idx % (sizeof(uint)* 8 / 2));
		word >>= (sizeof(uint)* 8 - 2 - bit_pos);
		return_value = mask & word;
	}
	return (Trit)return_value;
}

void TritSet::reallocMemory(TritSet& tritset, size_t idx)
{
	size_t new_capacity_ = tritsToBytes(idx) + 1;
	uint* tmp = new uint[new_capacity_];

	for (size_t i = 0; i < tritset.capacity_; ++i) {
		tmp[i] = tritset.storage_[i];
	}
	for (size_t i = tritset.capacity_; i < new_capacity_; ++i) {
		tmp[i] = 0;
	}

	tritset.capacity_ = new_capacity_;
	tritset.size_ = idx + 1;
	delete[] tritset.storage_;
	tritset.storage_ = tmp;
}

TritSet::reference::reference(TritSet& tritset, size_t idx) : index(idx), tritset(tritset)
{
	word_ = &tritset.storage_[tritsToBytes(idx)];
	bit_pos_ = 2 * (idx % (sizeof(uint)* 8 / 2)); //первый бит трита
	//делим на 2 чтобы получить кол-во тритов в uint'e
}

TritSet::TritSet(const TritSet& other) //Конструктор копирования
{
	size_ = other.size_;
	capacity_ = other.capacity_;
	storage_ = new uint[capacity_];

	for (size_t i = 0; i < capacity_; ++i) {
		storage_[i] = other.storage_[i];
	}
}

TritSet::TritSet(size_t size) : size_(size) //Конструктор по умолчанию
{
	if (size == 0) {
		capacity_ = 0;
		storage_ = nullptr;
	}
	else {
		capacity_ = tritsToBytes(size) + 1;
		storage_ = new uint[capacity_];
	}
	for (size_t i = 0; i < capacity_; ++i) { storage_[i] = 0; }
}

TritSet & TritSet::operator=(TritSet & other) //оператор присваивания состояния одного объекта другому
{
	if (this != &other) {
		delete[] storage_;
		size_ = other.size_;
		capacity_ = other.capacity_;
		storage_ = new uint[capacity_];
		for (size_t i = 0; i < capacity_; ++i) {
			storage_[i] = other.storage_[i];
		}
	}
	return *this;
}

TritSet::TritSet(TritSet&& other) //Move-конструктор
{
	size_ = other.size_;
	capacity_ = other.capacity_;
	delete[] storage_;
	storage_ = other.storage_;
	other.storage_ = nullptr;
	other.capacity_ = 0;
	other.size_ = 0;
}

TritSet::~TritSet() //деструктор
{
	delete[] storage_;
}

*/
