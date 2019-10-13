#include "tritset.h"
#include <iostream>
#include <map>

// Proxy-class "reference" constructor
TritSet::reference::reference(TritSet& tritset, size_t idx) :
	tritset_(tritset), index_(idx)
{}

// Type conversion to Trit type operator
TritSet::reference::operator Trit() const
{
	return tritset_.getTrit(index_);
}

// Operator assignment overloading: set[index_] = value
TritSet::reference& TritSet::reference::operator=(Trit value)
{
	tritset_.setTrit(value, index_);
	return *this;
}

// Operator assignment overloading: set[index_] = other_set[other.index_]
TritSet::reference& TritSet::reference::operator=(const reference& other)
{
	tritset_.setTrit((Trit)other, index_);
	return *this;
}

// Default constructor 
TritSet::TritSet(size_t size, Trit value) 
	: size_(size)
	, storage_(new uint[tritsToUints(size) + 1])
{
	uint mask = 0;

	//Fill uint with value using mask
	for (size_t i = 0; i < 4 * sizeof(uint); ++i) {
		mask |= (uint)value << i * 2;
	}

	//Fill elements of uint storage with mask
	for (uint* i = storage_; i != storage_ + tritsToUints(size) + 1; ++i) {
		*i = mask;
	}
}

// Copy constructor 
TritSet::TritSet(const TritSet& other) 
	: size_(other.size_)
	, storage_(new uint[tritsToUints(other.size_) + 1])
{
	for (size_t i = 0; i < tritsToUints(size_) + 1; ++i) {
		storage_[i] = other.storage_[i];
	}
}

// Move constructor
TritSet::TritSet(TritSet&& other) 
	: storage_(nullptr)
	, size_(0)
{
	size_ = other.size_;
	storage_ = other.storage_;

	other.size_ = 0;
	other.storage_ = nullptr;
}

// Destructor
TritSet::~TritSet() 
{
	delete[] storage_;
}

// Copy assignment operator
TritSet& TritSet::operator=(const TritSet& other) 
{
	if (this != &other) {
		delete[] storage_;

		size_ = other.size_;
		storage_ = new uint[tritsToUints(size_) + 1];

		for (size_t i = 0; i < tritsToUints(size_) + 1; ++i) {
			storage_[i] = other.storage_[i];
		}
	}
	return *this;
}

// Move assignment operator
TritSet& TritSet::operator=(TritSet&& other) 
{
	if (this != &other) {
		delete[] storage_;

		size_ = other.size_;
		storage_ = other.storage_;

		other.storage_ = nullptr;
		other.size_ = 0;
	}
	return *this;
}

// Element access operator (returns reference)
TritSet::reference TritSet::operator[](size_t idx)
{
	return reference(*this, idx);
}

// Element access operator (returns value)
Trit TritSet::operator[](size_t idx) const
{
	return TritSet::getTrit(idx);
}

// Shrinks storage to the size enough to store the last set trit
void TritSet::shrink() 
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

// Returns size in trits up to the last set bit
size_t TritSet::length() 
{
	size_t last_set_idx = 0;
	int met_set = 0;

	for (size_t i = 0; i < size_; ++i) {
		if ((Trit)((*this)[i]) != Trit::Unknown) {
			last_set_idx = i + 1;
		}
	}
	return last_set_idx;
}

// Resets trits starting from idx (incl. idx)
void TritSet::trim(size_t idx) 
{
	size_t len = length();
	for (size_t i = idx; i < len; ++i) {
		(*this)[i] = Trit::Unknown;
	}
	size_ = idx;
}

// Count number of trits of value
size_t TritSet::cardinality(Trit value) 
{
	size_t count = 0;

	for (size_t i = 0; i < size_; ++i) {
		if ((Trit)((*this)[i]) == value) {
			++count;
		}
	}
	return count;
}

// Count number of trits of all values
std::map< Trit, size_t > TritSet::cardinality() 
{
	std::map< Trit, size_t > values;

	values[Trit::False] = cardinality(Trit::False);
	values[Trit::True] = cardinality(Trit::True);
	values[Trit::Unknown] = cardinality(Trit::Unknown);

	return values;
}

// Reallocates memory
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

// Sets trit on idx position to value
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

// Gets trit from idx position
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

// Performs logic operation AND on tritsets
TritSet operator&(const TritSet& left, const TritSet& right)
{
	size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i)
		res[i] = (Trit)left[i] & (Trit)right[i];

	return res;
}

// Performs logic operation OR on tritsets
TritSet operator|(const TritSet& left, const TritSet& right)
{
	size_t res_size = (left.size_ > right.size_) ? left.size_ : right.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i)
		res[i] = (Trit)left[i] | (Trit)right[i];

	return res;
}

// Performs logic operation NOT on tritset
TritSet operator~(const TritSet& obj)
{
	size_t res_size = obj.size_;
	TritSet res(res_size);

	for (size_t i = 0; i < res_size; ++i) {
		res[i] = ~obj[i];
	}

	return res;
}

// Compares trits for equality
bool operator==(const TritSet& left, const TritSet& right) 
{
	if (left.size_ != right.size_) 
		return false;

	for (size_t i = 0; i < left.size_; i++)
		if ((Trit)left[i] != (Trit)right[i]) 
			return false;

	return true;
}

// Compares trits for inequality
bool operator!=(const TritSet& left, const TritSet& right) {
	return !(left == right);
}

