#pragma once
#include "trit.h"
#include <map>

class TritSet
{
	uint* storage_; 
	size_t size_; 
	
	void reallocMemory(size_t idx);
public:

	/*Member types*/
	class reference
	{
		friend class TritSet;

		TritSet& tritset_;
		size_t index_;

		reference();

	public:
		reference(TritSet& tritset, size_t idx);
		~reference() {};

		operator Trit() const;
		
		reference& operator=(Trit value);
		reference& operator=(const reference& other);
	};

	friend class reference;

	/*Member functions*/
	TritSet(size_t size = 0, Trit value = Trit::Unknown); 
	TritSet(const TritSet& other); 
	TritSet(TritSet&& other); 
	~TritSet(); 

	TritSet& operator=(const TritSet& other); 
	TritSet& operator=(TritSet&& other); 

	//Element access
	reference operator[](size_t idx);
	Trit operator[](size_t idx) const;

	//Capacity
	void shrink();
	size_t length() const;
	void trim(size_t idx);
	size_t get_size() const { return size_; }

	//Values cardinality
	size_t cardinality(Trit value) const;
	std::map< Trit, size_t > cardinality() const;

	//Modifiers
	void setTrit(Trit value, size_t idx);
	Trit getTrit(size_t idx) const;

	/*Non-member functions*/
	friend TritSet operator&(const TritSet& left, const TritSet& right);
	friend TritSet operator|(const TritSet& left, const TritSet& right);
	friend TritSet operator~(const TritSet& obj);

	friend bool operator==(const TritSet& left, const TritSet& right); 
	friend bool operator!=(const TritSet& left, const TritSet& right);
};

TritSet operator&(const TritSet& left, const TritSet& right);
TritSet operator|(const TritSet& left, const TritSet& right);
TritSet operator~(const TritSet& obj);

bool operator==(const TritSet& left, const TritSet& right);
bool operator!=(const TritSet& left, const TritSet& right);
