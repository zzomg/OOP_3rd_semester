#pragma once
#include <map>

using uint = unsigned int;

enum class Trit : uint { False = 2, Unknown = 0, True = 1 };
//logic values: False = 10, Unknown = 00, True = 01

Trit operator | (Trit left, Trit right);
Trit operator & (Trit left, Trit right);
Trit operator ~ (Trit value);

class TritSet
{
	uint* storage_; //внутреннее хранилище
	size_t size_; //количество ТРИТОВ в массиве
	size_t capacity_; //количество UINT'ОВ в массиве

public:

	class reference
	{
		friend class TritSet;

		TritSet& tritset_;
		size_t index_;

		reference();
	public:
		operator Trit() const;
		reference(TritSet& tritset, size_t idx);
		~reference() {};
		reference& operator=(Trit value);
		reference& operator=(const reference& other);
	};

	friend class reference;

	//constructors & a destructor
	TritSet(size_t size = 0);
	TritSet(const TritSet& other);
	TritSet(TritSet&& other);
	~TritSet();

	//operators overloading
	TritSet& operator=(TritSet& other);
	reference operator[](size_t idx);
	Trit operator[](size_t idx) const;
	friend TritSet operator&(const TritSet& left, const TritSet& right);
	friend TritSet operator|(const TritSet& left, const TritSet& right);
	friend TritSet operator~(const TritSet& obj);

	//member functions
	void shrink();
	size_t length();
	void trim(size_t idx);
	size_t cardinality(int value);
	size_t get_capacity() { return capacity_; }
	std::map< int, size_t > cardinality();

	//utilities
	static void reallocMemory(TritSet& tritset, size_t idx);
	static size_t tritsToBytes(size_t trit_n) {
		return (trit_n * 2 / 8 / sizeof(uint));
	}
	static size_t tritsInByte() {
		return (sizeof(uint) * 8 / 2);
	}
};

/*
//MY OLD VERSION
#pragma once
#include <map>

using uint = unsigned int;

enum Trit : uint { False = 2, Unknown = 0, True = 1 };
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

		reference& operator=(int value);
		reference& operator=(reference& other);
	};

	friend class reference;

	//constructors & a destructor
	TritSet(size_t size = 0);
	TritSet(const TritSet& other);
	TritSet(TritSet&& other);
	~TritSet();

	//operators overloading
	TritSet& operator=(TritSet& other);
	reference operator[](size_t idx);
	uint operator[](size_t idx) const;

	friend TritSet operator&(const TritSet& left, const TritSet& right);
	friend TritSet operator|(const TritSet& left, const TritSet& right);
	friend TritSet operator~(const TritSet& obj);

	//member functions
	void shrink();
	size_t length();
	void trim(size_t idx);
	size_t cardinality(int value);
	size_t get_capacity() { return capacity_; }
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
*/
/*
//SEMINAR VERSION 
#pragma once
#include "stdafx.h"
#include "targetver.h"
#include <map>

using uint = unsigned int;

enum Trit : uint { False = 2, Unknown = 0, True = 1 };
//logic values: False = 10, Unknown = 00, True = 01

Trit operator | (Trit left, Trit right);
Trit operator & (Trit left, Trit right);
Trit operator ~ (Trit value);

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
		size_t index;
		TritSet& tritset;

		reference();

	public:
		operator Trit() const;
		reference(TritSet& tritset, size_t idx);
		~reference() {};

		reference& operator=(int value);
		reference& operator=(reference& other);
	};

	friend class reference;

	//constructors & a destructor
	TritSet(size_t size = 0);
	TritSet(const TritSet& other);
	TritSet(TritSet&& other);
	~TritSet();

	//operators overloading
	TritSet& operator=(TritSet& other);
	reference operator[](size_t idx);
	Trit operator[](size_t idx) const;

	friend TritSet operator&(const TritSet& left, const TritSet& right);
	friend TritSet operator|(const TritSet& left, const TritSet& right);
	friend TritSet operator~(const TritSet& obj);

	//member functions
	void shrink();
	size_t length();
	void trim(size_t idx);
	size_t cardinality(int value);
	size_t get_capacity() { return capacity_; }
	std::map< int, size_t > cardinality();

	//utilities
	static void reallocMemory(TritSet& tritset, size_t idx);
	static size_t tritsToBytes(size_t trit_n) {
		return (trit_n * 2 / 8 / sizeof(uint));
	}
	static size_t tritsInByte() {
		return (sizeof(uint)* 8 / 2);
	}
};
*/
