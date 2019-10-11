#include "tritset.h"
#include <iostream>
#include <map>

Trit operator | (Trit left, Trit right)
{
	if (left == Trit::True || right == Trit::True) {
		return Trit::True;
	}
	else if (left == Trit::False && right == Trit::False) {
		return Trit::False;
	}
	else return Trit::Unknown;
}

Trit operator & (Trit left, Trit right)
{
	if (left == Trit::False || right == Trit::False) {
		return Trit::False;
	}
	else if (left == Trit::True && right == Trit::True) {
		return Trit::True;
	}
	else return Trit::Unknown;
}

Trit operator ~ (Trit value)
{
	if (value == Trit::True) {
		return Trit::False;
	}
	else if (value == Trit::False) {
		return Trit::True;
	}
	else return Trit::Unknown;
}

TritSet operator~(const TritSet& obj)
{
	size_t res_size = obj.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i) {
		res[i] = ~((Trit)obj[i]);
	}

	return res;
}

bool operator==(const TritSet& left, const TritSet& right) 
{
	if (left.size_ != right.size_) 
		return false;

	for (size_t i = 0; i < left.size_; i++)
		if ((Trit)left[i] != (Trit)right[i]) 
			return false;

	return true;
}

bool operator!=(const TritSet& left, const TritSet& right) {
	return !(left == right);
}

TritSet operator&(const TritSet& left, const TritSet& right)
{
	size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
	TritSet res(res_size);
	
	for (size_t i = 0; i < res_size; ++i)
		res[i] = (Trit)left[i] & (Trit)right[i];

	return res;
}

TritSet operator|(const TritSet& left, const TritSet& right)
{
	size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
	TritSet res(res_size);
	
	for (size_t i = 0; i < res_size; ++i)
		res[i] = (Trit)left[i] | (Trit)right[i];

	return res;
}

std::map< Trit, size_t > TritSet::cardinality() //кол-во тритов всех видов
{
	std::map< Trit, size_t > values;
	
	for (size_t i = 0; i < size_; ++i) {
		++values[Trit((*this)[i])];
	}
	return values;
}

size_t TritSet::cardinality(Trit value) //кол-во тритов типа value 
{
	size_t count = 0;
	
	for (size_t i = 0; i < size_; ++i) {
		if ((Trit)((*this)[i]) == value) {
			++count;
		}
	}
	return count;
}

void TritSet::trim(size_t idx) //"забыть" триты после idx
{
	size_t len = length();
	for (size_t i = idx; i < len; ++i) {
		(*this)[i] = Trit::Unknown;
	}
	size_ = idx;
}

size_t TritSet::length() //размер в тритах до послед. устан. трита
{
	size_t last_set_idx = -1;
	int met_set = 0;
	
	for (size_t i = 0; i < size_; ++i) {
		if ((Trit)((*this)[i]) != Trit::Unknown) {
			last_set_idx = i;
		}
	}
	return last_set_idx + 1;
}

void TritSet::shrink() //сжать тритсет до размера, достат. для хран. послед. устан. трита
{
	size_t len;
	if (length() == 0) len = 0;
	else len = tritsToUints(length()) + 1;

	uint* shrunk_storage = new uint[len];

	for (size_t i = 0; i < len; ++i) {
		shrunk_storage[i] = storage_[i];
	}

	delete[] storage_;
	storage_ = shrunk_storage;
	if (len == 0) size_ = 0;
	else size_ = length();
}

TritSet::reference& TritSet::reference::operator=(const reference& other)
{
	tritset_.setTrit((Trit)other, index_);
	return *this;
}

TritSet::reference& TritSet::reference::operator=(Trit value)
{
	tritset_.setTrit(value, index_);
	return *this;
}

TritSet::reference TritSet::operator[](size_t idx)
{
	return reference(*this, idx);
}

Trit TritSet::operator[](size_t idx) const
{
	return TritSet::getTrit(idx);
}

void TritSet::setTrit(Trit value, size_t idx)
{
	if (idx >= size_) {
		if (value == Trit::Unknown) return;
		reallocMemory(idx);
	}

	size_t bit_pos = 2 * (idx % tritsInByte());
	size_t shift = sizeof(uint) * 8 - 2 - bit_pos;
	uint mask = 0x03;

	uint* word = &storage_[tritsToUints(idx)];
	*word &= ~(mask << shift);
	*word |= (((uint)value) << shift);

	size_ = length();
}

Trit TritSet::getTrit(size_t idx) const
{
	if (idx < size_) {
		size_t bit_pos = 2 * (idx % tritsInByte());
		uint word = storage_[tritsToUints(idx)];
		size_t shift = sizeof(uint) * 8 - 2 - bit_pos;
		uint mask = 0x03;
		return (Trit)((word & (mask << shift)) >> shift);
	}
	return Trit::Unknown;
}

void TritSet::reallocMemory(size_t idx)
{
	size_t capacity = tritsToUints(size_) + 1;
	size_t new_capacity = tritsToUints(idx) + 1;

	uint* tmp = new uint[new_capacity];

	for (size_t i = 0; i < capacity; ++i) {
		tmp[i] = storage_[i];
	}

	for (size_t i = capacity; i < new_capacity; ++i) {
		tmp[i] = 0;
	}

	size_ = idx + 1;
	delete[] storage_;
	storage_ = tmp;
}

TritSet::reference::operator Trit() const
{
	return tritset_.getTrit(index_);
}

TritSet::reference::reference(TritSet& tritset, size_t idx): 
	tritset_(tritset), index_(idx)
{}

TritSet::TritSet(const TritSet& other) //Конструктор копирования
{
	size_ = other.size_;
	storage_ = new uint[tritsToUints(size_) + 1];
	for (size_t i = 0; i < tritsToUints(size_) + 1; ++i) {
		storage_[i] = other.storage_[i];
	}
}

TritSet::TritSet(size_t size, Trit value) : size_(size) //Конструктор по умолчанию
{
	if (size == 0) {
		storage_ = nullptr; 
	}
	else {
		storage_ = new uint[tritsToUints(size) + 1];
		for (size_t i = 0; i < (tritsToUints(size) + 1); ++i)
			storage_[i] = (uint)value;
	}
}

TritSet& TritSet::operator=(TritSet const& other) //оператор присваивания состояния одного объекта другому
{
	if (this == &other) {
		return *this;
	}
	delete[] storage_;
	size_ = other.size_;
	storage_ = new uint[tritsToUints(size_) + 1];

	for (size_t i = 0; i < tritsToUints(size_) + 1; ++i) {
		storage_[i] = other.storage_[i];
	}
	return *this;
}

TritSet::TritSet(TritSet&& other) //Move-конструктор
{
	size_ = other.size_;
	delete[] storage_;
	storage_ = other.storage_;

	other.storage_ = nullptr;
	other.size_ = 0;
}

TritSet::~TritSet() //деструктор
{
	delete[] storage_;
}
