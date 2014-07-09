#include "ObjectFormatter.h"
#include "FileManager.h"
#include "TestClass.h"
#include "ObjectSerializer.h"
#include <Windows.h>

DECL_SERIALIZABLE(SmallTestClass);
DECL_SERIALIZABLE(TestClass);

int _tmain(int argc, LPTSTR argv[])
{
	const TCHAR* sourceCodeDir;
	const TCHAR* workingDir;
	if (argc == 1)
	{
		sourceCodeDir = g_FileManager.GetCurrentDir();
	}
	else if (argc == 2)
	{
		sourceCodeDir = argv[1];
	}
	else if (argc == 3)
	{
		sourceCodeDir = argv[1];
		workingDir = argv[2];
		g_FileManager.PushDir(workingDir);
	}

	printf("TypeInfoCollector\n=================\n");
	printf("- Working Directory:\n\t%s\n", g_FileManager.GetCurrentDir());
	printf("- Source Code Directory:\n\t%s\n", sourceCodeDir);
	g_ObjectFormatter.WriteTypeTable(sourceCodeDir);
	printf("\n\nTypeInfo Collection Ended ...\n");

	if (argc == 3)
		g_FileManager.PopDir();

	g_ObjectFormatter.FinalizeTypeTable(g_ObjectSerializer.TypeTable(), g_ObjectFactory.GetObjectTable());

	TestClass* pTestObj1 = new TestClass(true);
	auto* pVec = &pTestObj1->myVector;
	auto* pMap = &pTestObj1->myMap;
	auto* pSet = &pTestObj1->mySet;
	auto* pPair = &pTestObj1->myPair;

	ISerializable* pAsSerializable = dynamic_cast<ISerializable*>(pVec);
	IContainer* pAsContainer = dynamic_cast<IContainer*>(pVec);
	g_ObjectSerializer.Serialize(pTestObj1, "TestObj.bin");

	TestClass* pTestObj2 = new TestClass();
	g_ObjectSerializer.Deserialize(pTestObj2, "TestObj.bin");

	return 0;
}
