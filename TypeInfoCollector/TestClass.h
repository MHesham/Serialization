///> [Serializable]
#pragma once

#include "SVector.h"
#include "SMap.h"
#include "SSet.h"
#include "SPair.h"
#include <initializer_list>

namespace Serialization
{
	///> class=SmallTestClass
	class SmallTestClass : public ISerializable
	{
		OBJECT_SERIALIZABLE(SmallTestClass, &myInt, &myMap);
	public:
		SmallTestClass() {}
		SmallTestClass(bool init) :
			myInt(101010),
			myMap(std::map<char, short>{{ 'D', 5 }, { 'E', 6 }, { 'F', 7 }})
		{}

		///> type=int
		int myInt;
		///> type=map(pair(char,short))
		SMap<char, short> myMap;
	};

	class BaseClass
	{
	public:
		BaseClass() :
			myStr("myStr")
		{
			strcpy_s(myBuffer, "myBuffer");
		}
	protected:
		char myBuffer[16];
		std::string myStr;
	};

	///> class=TestClass
	class TestClass : public BaseClass, public ISerializable
	{
		OBJECT_SERIALIZABLE(TestClass, &myInt, &myVector, &myMap, &mySet, &myPair, &pMySmall);
	public:
		TestClass() :
			pMySmall(nullptr)
		{}
		TestClass(bool init) :
			myInt(7777),
			myVector(std::vector<int>{ 1, 2, 3, 4, 5 }),
			myMap(std::map<short, int>{{ 10, 100 }, { 20, 200 }, { 30, 300 }}),
			mySet(std::set<char>{'A', 'B', 'C'}),
			myPair(999, true),
			pMySmall(new SmallTestClass(true))
		{
		}

		///> type=int
		int myInt;
		///> type=vector(int)
		SVector<int> myVector;
		///> type=map(pair(short,int))
		SMap<short, int> myMap;
		///> type=set(char)
		SSet<char> mySet;
		///> type=pair(int,bool)
		SPair<int, bool> myPair;
		///> type=SmallTestClass*
		SmallTestClass* pMySmall;
	};
}

