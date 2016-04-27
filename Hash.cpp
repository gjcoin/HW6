#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void printHashtable(vector<int> &my_vector, char * my_file_name)
{
	ofstream out;
	out.open(my_file_name);
	unsigned int i;
	out << my_vector[0]; //output first value
	for (i = 1; i<my_vector.size(); i++)
		out << ',' << my_vector[i]; //no commas at the end
	out << endl;
	out.clear();
	out.close();
}

bool quadraticProbing(vector<int> &hash_table, unsigned int key, unsigned int magic, int val)
{
	//Use long do to accommodate larger values
	unsigned long long max = hash_table.size();
	int i;
	for (i = 1; i < (max - 1) / 2; i++)
	{
		unsigned long long index = (key + magic*i*i) % max;
		if (hash_table[index] == 0) //entry available
		{
			hash_table[index] = val;
			return true;
		}
	}
	return false; //no available slots in hash table
}

int getDigit(int digit, int input)
{
	// if digit == 1, need input / 1;
	// if digit == 2, need input / 10;
	// if digit == 3, need input / 100;
	const static int divisor[] = { //Doesn't change, no need to allocate more memory each time
		1,
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000,
		100000000,
		1000000000
	};
	return (input/divisor[9-digit]) % 10; //Move desired digit into 1s place and discard the rest
}

//Extract 3rd, 5th, 7th, and 8th digits from input
int extractDigits(int input)
{
	return 1000 * getDigit(3, input) + 100 * getDigit(5, input) + 10 * getDigit(7, input) + getDigit(8, input);
}

bool hashFile(char* filepath, vector<int> &hash_table,int magic)  //gets file from filepath
{
	char lineData[11]; //Each social is 9 characters long (+comma and null terminator = 11)
	int number = 0; //ciel(ln(999,999,999)/ln(2)) --> requires 29 bits, integer is 32
	int key = 0;
	//int value_number = 0;
	ifstream in;
	in.open(filepath);
	if (!in.is_open())
	{
		cout << "I cannot find the file.\n";
		return false;
	}
	while (in.peek() != EOF)
	{
		/*value_number++; //this is a test that displays progress through the file
		if (value_number % 100 == 0) { cout << value_number << endl; cout.flush(); }*/
		in.getline(lineData, 10, ','); //Read 10 characters or until comma is reached
		number = atoi(lineData);
		if ((number < 100000000) || (number > 9999999999))
		{
			cout << "Error in input file" << endl;
			cout << number << " is out of range." << endl;
			system("PAUSE");
			exit(-3);
		}
		key = extractDigits(number);
		if (quadraticProbing(hash_table, key, magic, number) == false)
		{
			return false;
		}
	}
	in.clear();
	in.close();
	return true;
}

int main()
{
	vector<int> hash_table;
	int hash_size = 0;
	int magic;

	cout << "Please enter a number between 1 and 450 million: ";
	cin >> magic;
	hash_size = (magic / 2) - 1;
	/*test:
	if (extractDigits(123456789) != 3578) { cout << "I think I better think it out again" << endl; system("PAUSE"); return -2; }
	*/
	if (magic > 450000000 || magic < 1000000)
	{
		cout << "Invalid input." << endl; 
		system("PAUSE");
		return 1;
	}

	for (int i = 0; i < hash_size; i++)
	{
		hash_table.push_back(0); //Fills hash table with illegal values to help determine if slot is empty
	}
	if (hashFile("everybodys_socials.txt", hash_table,magic)==false)
	{
		cout << "No space available in hash table, use larger hash table max." << endl;
		system("PAUSE");
		return -1;
	}
	printHashtable(hash_table, "hashed_socials.txt");
	cout << "Success! Hash table outputted to hashed_socials.txt" << endl;
	system("PAUSE");
	return 0;

}