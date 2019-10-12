#include "tritset.h"
#include <gtest/gtest.h>
#include <map>

TritSet* tritSetFromString(std::string str) 
{
	if (!str.length()) {
		return new TritSet();
	}

	TritSet* set = new TritSet(str.length());
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	size_t pos = 0;
	for (char c : str) {
		switch (c) {
		case 'f':
			(*set)[pos] = Trit(Trit::False);
			break;
		case 'u':
			(*set)[pos] = Trit(Trit::Unknown);
			break;
		case 't':
			(*set)[pos] = Trit(Trit::True);
			break;
		default:
			delete set;
			return new TritSet();
		}
		pos++;
	}

	return set;
}

TEST(ConstructorTest, EmptySet)
{
	TritSet set;

	ASSERT_EQ(set.get_size(), 0);
}

TEST(ConstructorTest, AllocatingMemory)
{
	TritSet set(100);

	for (size_t i = 0; i < 100; ++i) {
		ASSERT_EQ(uint((Trit)set[i]), 0);
	}
}

TEST(ConstructorTest, SettingDefaultValue)
{
	TritSet set(100);

	for (size_t i = 0; i < 100; ++i) {
		set[i] = Trit::True;
	}

	ASSERT_EQ(100, set.get_size());

	for (size_t i = 0; i < 100; ++i) {
		ASSERT_EQ((Trit)set[i], Trit::True);
	}
}

TEST(MemberFunctionsTest, TrimTest)
{
	TritSet set(100);
	for (size_t i = 0; i < 100; ++i) {
		set[i] = Trit::True;
	}

	ASSERT_EQ(100, set.get_size());

	set.trim(50);

	ASSERT_EQ(50, set.get_size());

	// Проверка оставшихся
	for (size_t i = 0; i < 50; i++)
		ASSERT_EQ((Trit)set[i], Trit::True);

	// Проверка удаленных
	for (size_t i = 51; i < 100; i++)
		ASSERT_EQ((Trit)set[i], Trit::Unknown);
}

TEST(MemberFunctionsTest, ShrinkSet) 
{
	TritSet set(1000);

	for (size_t i = 0; i < 1000; ++i) {
		ASSERT_EQ(uint((Trit)set[i]), 0);
	}

	set.shrink();

	ASSERT_EQ(0, set.get_size());

	set[99] = Trit::True;

	ASSERT_EQ(100, set.get_size());

	set[49] = Trit::False;

	ASSERT_EQ(100, set.get_size());

	set[99] = Trit::Unknown;

	ASSERT_EQ(50, set.get_size());

	set.shrink();

	ASSERT_EQ(50, set.get_size());
}

TEST(MemberFunctionsTest, CardinalityTest) 
{
	TritSet emptySet;
	TritSet setTrue(100);
	TritSet setFalse(100);
	TritSet setUnknown(100);

	for (size_t i = 0; i < 100; ++i) {
		setTrue[i] = Trit::True;
	}
	for (size_t i = 0; i < 100; ++i) {
		setFalse[i] = Trit::False;
	}
	for (size_t i = 0; i < 100; ++i) {
		setUnknown[i] = Trit::Unknown;
	}

	ASSERT_EQ(emptySet.cardinality(Trit::True), 0);
	ASSERT_EQ(emptySet.cardinality(Trit::False), 0);
	ASSERT_EQ(emptySet.cardinality(Trit::Unknown), 0);

	ASSERT_EQ(setTrue.cardinality(Trit::True), 100);
	ASSERT_EQ(setTrue.cardinality(Trit::False), 0);
	ASSERT_EQ(setTrue.cardinality(Trit::Unknown), 0);

	ASSERT_EQ(setFalse.cardinality(Trit::True), 0);
	ASSERT_EQ(setFalse.cardinality(Trit::False), 100);
	ASSERT_EQ(setFalse.cardinality(Trit::Unknown), 0);

	ASSERT_EQ(setUnknown.cardinality(Trit::True), 0);
	ASSERT_EQ(setUnknown.cardinality(Trit::False), 0);
	ASSERT_EQ(setUnknown.cardinality(Trit::Unknown), 0);

	TritSet* set = tritSetFromString("FFFUUUTTT");
	std::map< Trit, size_t > map = {
			{Trit::True,    3},
			{Trit::False,   3},
			{Trit::Unknown, 3},
	};

	ASSERT_EQ(set->cardinality(), map);
	delete set;
}

TEST(MemberFunctionsTest, OperatorNOTFalseTest)
{
	TritSet setFalse(10, Trit::False);

	ASSERT_EQ(setFalse.get_size(), 10);

	setFalse = ~setFalse; 

	ASSERT_EQ(setFalse.get_size(), 10);

	for (size_t i = 0; i < 10; i++)
		ASSERT_EQ((Trit)setFalse[i], Trit::True);
}

TEST(MemberFunctionsTest, OperatorNOTUnknownTest)
{
	TritSet setUnknown(10, Trit::Unknown);

	ASSERT_EQ(setUnknown.get_size(), 10);

	setUnknown = ~setUnknown;

	ASSERT_EQ(setUnknown.get_size(), 0);

	for (size_t i = 0; i < 10; i++)
		ASSERT_EQ((Trit)setUnknown[i], Trit::Unknown);
}

TEST(MemberFunctionsTest, OperatorNOTTrueTest)
{
	TritSet setTrue(10, Trit::True);

	ASSERT_EQ(setTrue.get_size(), 10);

	setTrue = ~setTrue;

	ASSERT_EQ(setTrue.get_size(), 10);

	for (size_t i = 0; i < 10; i++)
		ASSERT_EQ((Trit)setTrue[i], Trit::False);
}

TEST(MemberFunctionsTest, OperatorNOTAllTest)
{
	TritSet* setAll = tritSetFromString("FUTUTF");

	ASSERT_EQ(setAll->get_size(), 6);

	*setAll = ~(*setAll);

	TritSet* notSet = tritSetFromString("TUFUFT");

	ASSERT_EQ((*setAll), (*notSet));

	delete setAll;
	delete notSet;
}

TEST(MemberFunctionsTest, OperatorORContinuallyTest)
{
	TritSet setEmpty;
	TritSet setTrue(10, Trit::True), setFalse(10, Trit::False), setUnknown(10, Trit::Unknown);

	ASSERT_EQ(setEmpty.get_size(), 0);

	ASSERT_EQ(setTrue.get_size(), 10);

	ASSERT_EQ(setFalse.get_size(), 10);

	ASSERT_EQ(setUnknown.get_size(), 10);

	// Empty | Empty
	TritSet set = setEmpty | setEmpty;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);

	// True | Empty
	set = setTrue | setEmpty;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setTrue);

	// Unknown | Empty
	set = setUnknown | setEmpty;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);

	// False | Empty
	set = setFalse | setEmpty;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);

	// False | False
	set = setFalse | setFalse;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setFalse);

	// True | True
	set = setTrue | setTrue;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setTrue);

	// True | False
	set = setTrue | setFalse;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setTrue);

	// Unknown | Unknown
	set = setUnknown | setUnknown;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);

	// Unknown | True
	set = setUnknown | setTrue;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setTrue);

	// Unknown | False
	set = setUnknown | setFalse;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);
}

TEST(MemberFunctionsTest, OperatorORDifferentTest) 
{
	TritSet* setLeft = tritSetFromString("FUT");
	TritSet* setRight = tritSetFromString("TFU");
	TritSet* setLeftSmall = tritSetFromString("TU");
	TritSet* setRightSmall = tritSetFromString("UF");

	TritSet set = (*setLeft) | (*setRight);
	ASSERT_EQ(set.get_size(), 3);
	TritSet* temp = tritSetFromString("TUT");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setLeft) | (*setLeftSmall);
	ASSERT_EQ(set.get_size(), 3);
	temp = tritSetFromString("TUT");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setRight) | (*setRightSmall);
	ASSERT_EQ(set.get_size(), 2);
	temp = tritSetFromString("TF");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setRight) | (*setLeftSmall);
	ASSERT_EQ(set.get_size(), 1);
	temp = tritSetFromString("T");
	ASSERT_EQ(set, (*temp));
	delete temp;

	delete setLeft;
	delete setRight;
	delete setLeftSmall;
	delete setRightSmall;
}

TEST(MemberFunctionsTest, OperatorANDContinuallyTest)
{
	TritSet setEmpty;
	TritSet setTrue(10, Trit::True), setFalse(10, Trit::False), setUnknown(10, Trit::Unknown);

	ASSERT_EQ(setEmpty.get_size(), 0);

	ASSERT_EQ(setTrue.get_size(), 10);

	ASSERT_EQ(setFalse.get_size(), 10);

	ASSERT_EQ(setUnknown.get_size(), 10);

	// Empty & Empty
	TritSet set = setEmpty & setEmpty;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);

	// True & Empty
	set = setTrue & setEmpty;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);

	// Unknown & Empty
	set = setUnknown & setEmpty;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);

	// False & Empty
	set = setFalse & setEmpty;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setFalse);

	// False & False
	set = setFalse & setFalse;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setFalse);

	// True & True
	set = setTrue & setTrue;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setTrue);

	// True & False
	set = setTrue & setFalse;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setFalse);

	// Unknown & Unknown
	set = setUnknown & setUnknown;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);

	// Unknown & True
	set = setUnknown & setTrue;
	ASSERT_EQ(set.get_size(), 0);
	ASSERT_EQ(set, setEmpty);

	// Unknown & False
	set = setUnknown & setFalse;
	ASSERT_EQ(set.get_size(), 10);
	ASSERT_EQ(set, setFalse);
}

TEST(MemberFunctionsTest, OperatorANDDifferentTest) 
{
	TritSet* setLeft = tritSetFromString("FUT");
	TritSet* setRight = tritSetFromString("TFU");
	TritSet* setLeftSmall = tritSetFromString("TU");
	TritSet* setRightSmall = tritSetFromString("UF");

	TritSet set = (*setLeft) & (*setRight);
	ASSERT_EQ(set.get_size(), 2);
	TritSet* temp = tritSetFromString("FF");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setLeft) & (*setLeftSmall);
	ASSERT_EQ(set.get_size(), 1);
	temp = tritSetFromString("F");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setRight) & (*setRightSmall);
	ASSERT_EQ(set.get_size(), 2);
	temp = tritSetFromString("UF");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setRight) & (*setLeftSmall);
	ASSERT_EQ(set.get_size(), 2);
	temp = tritSetFromString("TF");
	ASSERT_EQ(set, (*temp));
	delete temp;

	delete setLeft;
	delete setRight;
	delete setLeftSmall;
	delete setRightSmall;
}

TEST(MemberFunctionsTest, OperatorEqualsTest)
{
	TritSet setTrue(10, Trit::True), setFalse(10, Trit::False), setUnknown(10, Trit::Unknown);
	TritSet* setLeft = tritSetFromString("FUT");
	TritSet* setRight = tritSetFromString("UTF");
	TritSet* setSmall = tritSetFromString("TF");

	ASSERT_EQ(setTrue, setTrue);
	ASSERT_EQ(setFalse, setFalse);
	ASSERT_EQ(setUnknown, setUnknown);
	ASSERT_EQ((*setLeft), (*setLeft));
	ASSERT_EQ((*setSmall), (*setSmall));

	ASSERT_NE((*setLeft), (*setRight));
	ASSERT_NE(setTrue, setFalse);
	ASSERT_NE(setTrue, setUnknown);
	ASSERT_NE(setFalse, setUnknown);

	delete setLeft;
	delete setRight;
	delete setSmall;
}

TEST(MemberFunctionsTest, OperatorGetTest)
{
	TritSet set;

	set[0] = Trit::True;
	set[10] = Trit::True;

	ASSERT_EQ((Trit)set[0], Trit::True);
	ASSERT_EQ((Trit)set[10], Trit::True);
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS(); 
}
