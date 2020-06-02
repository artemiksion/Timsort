#include <iostream>
using namespace std;

struct NodeOfStack
{
	NodeOfStack* NextElement;
	int IndexOfTheFirstElement;
	int SizeOfRun;
};

void DeleteStack(NodeOfStack*& TopOfStack)
{
	while (TopOfStack != NULL)
	{
		NodeOfStack* p = TopOfStack;
		TopOfStack = TopOfStack->NextElement;
		delete p;
	}
}

template <typename T>

void SwapValues(T& first, T& second)
{
	first = first ^ second;
	second = first ^ second;
	first = first ^ second;
}

void MergingElementsOfRuns(NodeOfStack* StackNode, NodeOfStack* NextStackNode, int* Array)
{
	if (StackNode->SizeOfRun >= NextStackNode->SizeOfRun)
	{
		int* TemporaryArray;
		int StackNodeIndex = 0;
		int NextStackNodeIndex = 0;
		TemporaryArray = new int[NextStackNode->SizeOfRun];
		for (int i = 0; i < NextStackNode->SizeOfRun; i++)
			TemporaryArray[i] = Array[NextStackNode->IndexOfTheFirstElement + i];
		for (int i = 0; i < (StackNode->SizeOfRun + NextStackNode->SizeOfRun); i++)
		{
			if (((TemporaryArray[NextStackNodeIndex] <= Array[StackNode->IndexOfTheFirstElement + StackNodeIndex]) || (StackNodeIndex >= StackNode->SizeOfRun)) && (NextStackNodeIndex < NextStackNode->SizeOfRun))
			{
				Array[NextStackNode->IndexOfTheFirstElement + i] = TemporaryArray[NextStackNodeIndex];
				NextStackNodeIndex++;
			}
			else
			{
				Array[NextStackNode->IndexOfTheFirstElement + i] = Array[StackNode->IndexOfTheFirstElement + StackNodeIndex];
				StackNodeIndex++;
			}
		}
		delete[] TemporaryArray;
	}
	else
	{
		int* TemporaryArray;
		int StackNodeIndex = (StackNode->SizeOfRun - 1);
		int NextStackNodeIndex = (NextStackNode->SizeOfRun - 1);
		TemporaryArray = new int[StackNode->SizeOfRun];
		for (int i = 0; i < StackNode->SizeOfRun; i++)
			TemporaryArray[i] = Array[StackNode->IndexOfTheFirstElement + i];
		for (int i = (StackNode->SizeOfRun + NextStackNode->SizeOfRun - 1); i >= 0; i--)
		{
			if (((TemporaryArray[StackNodeIndex] <= Array[NextStackNode->IndexOfTheFirstElement + NextStackNodeIndex]) || (StackNodeIndex < 0)) && (NextStackNodeIndex >= 0))
			{
				Array[NextStackNode->IndexOfTheFirstElement + i] = Array[NextStackNode->IndexOfTheFirstElement + NextStackNodeIndex];
				NextStackNodeIndex--;
			}
			else
			{
				Array[NextStackNode->IndexOfTheFirstElement + i] = TemporaryArray[StackNodeIndex];
				StackNodeIndex--;
			}
		}
		delete[] TemporaryArray;
	}
	StackNode->IndexOfTheFirstElement = NextStackNode->IndexOfTheFirstElement;
	StackNode->SizeOfRun += NextStackNode->SizeOfRun;
	StackNode->NextElement = NextStackNode->NextElement;
	delete NextStackNode;
}

int InsertionSort(int NumberOfElements, int* Array)
{
	for (int i = 1; i < NumberOfElements; i++)
	{
		for (int j = i; j > 0; j--)
		{
			if (Array[j] < Array[j - 1])
				SwapValues(Array[j], Array[j - 1]);
			else
				break;
		}
	}
	return 0;
}

int Get_minrun(int NumberOfElements)
{
	int r = 0;
	while (NumberOfElements >= 64)
	{
		r |= NumberOfElements & 1;
		NumberOfElements = NumberOfElements >> 1;
	}
	return r + NumberOfElements;
}

NodeOfStack* GetRuns(int NumberOfElements, int* DynamicArray)
{
	int minrun;
	int sizeoflocalrun = 1;
	NodeOfStack* TopOfStack = NULL, * p = NULL;
	minrun = Get_minrun(NumberOfElements);
	for (int i = 1; i < NumberOfElements; i++)
	{
		try
		{
			if (NumberOfElements - i == 1)
			{
				if (DynamicArray[i - 1] > DynamicArray[i])
					SwapValues(DynamicArray[i - 1], DynamicArray[i]);
				sizeoflocalrun += 1;
				InsertionSort(sizeoflocalrun, DynamicArray + i + 1 - sizeoflocalrun);
				p = new NodeOfStack;
				p->NextElement = TopOfStack;
				p->IndexOfTheFirstElement = (i + 1 - sizeoflocalrun);
				p->SizeOfRun = sizeoflocalrun;
				TopOfStack = p;
				break;
			}
			if ((sizeoflocalrun <= minrun) || (DynamicArray[i] >= DynamicArray[i - 1]))
			{
				sizeoflocalrun += 1;
				if (DynamicArray[i - 1] > DynamicArray[i])
					SwapValues(DynamicArray[i - 1], DynamicArray[i]);
			}
			else
			{
				InsertionSort(sizeoflocalrun, DynamicArray + i - sizeoflocalrun);
				p = new NodeOfStack;
				p->NextElement = TopOfStack;
				p->IndexOfTheFirstElement = (i - sizeoflocalrun);
				p->SizeOfRun = sizeoflocalrun;
				TopOfStack = p;
				sizeoflocalrun = 1;
			}
		}
		catch (bad_alloc)
		{
			DeleteStack(TopOfStack);
			throw bad_alloc();
		}
	}
	return p;
}

void MergingStacks(NodeOfStack*& TopOfStack, int* Array)
{
	try
	{
		NodeOfStack* p = TopOfStack;
		while (TopOfStack->NextElement != NULL)
		{
			if (p->NextElement != NULL)
			{
				if (p->NextElement->NextElement != NULL)
				{
					if ((p->SizeOfRun <= p->NextElement->SizeOfRun + p->NextElement->NextElement->SizeOfRun) || (p->NextElement->SizeOfRun <= p->NextElement->NextElement->SizeOfRun))
					{
						if (p->SizeOfRun >= p->NextElement->NextElement->SizeOfRun)
							MergingElementsOfRuns(p->NextElement, p->NextElement->NextElement, Array);
						else
							MergingElementsOfRuns(p, p->NextElement, Array);
					}
					else
						p = p->NextElement;
				}
				else
				{
					MergingElementsOfRuns(p, p->NextElement, Array);
				}
			}
			else
				p = TopOfStack;
		}
		delete TopOfStack;
	}
	catch (bad_alloc)
	{
		DeleteStack(TopOfStack);
		throw bad_alloc();
	}
}

void timsort(int NumberOfElements, int* DynamicArray)
{
	NodeOfStack* StackWithIntElements = NULL;
	StackWithIntElements = GetRuns(NumberOfElements, DynamicArray);
	MergingStacks(StackWithIntElements, DynamicArray);
}

int CorrectInput()
{
	int value;
	while (true)
	{
		cin >> value;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Repeat input" << endl;
		}
		else
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
	}
	return value;
}

int main()
{
	int NumberOfElements;
	int* DynamicArray = NULL;
	cout << "Input number of numbers" << endl;
	NumberOfElements = CorrectInput();
	try
	{
		DynamicArray = new int[NumberOfElements];
		for (int i = 0; i < NumberOfElements; i++)
		{
			DynamicArray[i] = rand();
			cout << i << " " << DynamicArray[i] << endl;
		}
		timsort(NumberOfElements, DynamicArray);
		cout << endl << endl;
		for (int i = 0; i < NumberOfElements; i++)
		{
			cout << i << " " << DynamicArray[i] << endl;
		}
		system("pause");
	}
	catch (bad_alloc)
	{
		cout << "Memory allocation error" << endl;
		delete[] DynamicArray;
		system("pause");
	}
	return 0;
}
