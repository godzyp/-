#include <iostream>
#include <string>
#include <fstream>
#define operators_1_number 7
#define operators_2_number 7
#define separator_number 11
#define reserved_word_number 8
using namespace std;

static char number[10] = { '0','1','2','3','4','5','6','7','8','9' };
static char operators_1[7] = { '!','+','-','*','/','<','>' };
static string operators_2[7] = { "==","<=",">=","!=","++","--","+="};
static char separator[11] = { '(',')','{','}',',',';','=','[',']',' ','\t' };
static string reserved_word[8] = { "const","main","if","else","while","int","for","return" };
enum {
	Reserved_Word = 1, Identifier = 2, Unsigned_Int = 3, Operators = 4, Separator = 5
};
enum {
	a = 0,	//字母
	b,		//数字
	c,		//运算符
	d,		//分隔符
};
static int state_array[5][4] =//状态矩阵
{	/*a b c d*/
	{ 1,2,4,3 },  //0
	{ 1,1,4,3 },  //1
	{ 0,2,4,3 },  //2
	{ 0,0,0,0 },  //3
	{ 1,2,4,3 },};//4

void read_file(string &text)
{	ifstream file("text.txt");
	string temp;
	if (file.is_open())
		while (getline(file,temp))
			text+=temp;
	else cout << "Read_Error" << endl;
}
bool is_char(char temp)
{	if (('A' <= temp && temp <= 'Z') || ('a' <= temp && temp <= 'z')|| (temp == '_'))
		return true;
	return false;
}
bool is_number(char temp)
{	for (int i = 0; i < 10; i++)
		if (number[i] == temp)
			return true;
	return false;
}
bool is_operators_1(char temp)
{	for (int i = 0; i < operators_1_number; i++)
		if (operators_1[i] == temp)
			return true;
	return false;
}
bool is_operators_2(string temp)
{	for (int i = 0; i < operators_2_number; i++)
		if (operators_2[i].compare(temp) == 0)
			return true;
	return false;
}
bool is_separator(char temp)
{	for (int i = 0; i < separator_number; i++)
		if (separator[i] == temp)
			return true;
	return false;
}
bool is_reserved_word(string temp)
{	for (int i = 0; i < reserved_word_number; i++)
		if (reserved_word[i].compare(temp) == 0)
			return true;
	return false;
}

int main()
{	string text("");
	read_file(text);
	int n = text.length();

	int i = 0;
	string temp;
	int state = 0;
	for (int i = 0; i < n;)
	{	switch (state)	//text[i]=遇到的字符
		{	case 0:
				if (is_char(text[i]))
				{	state = state_array[0][a];
					temp += text[i++];
				}
				else if (is_number(text[i]))
				{	state = state_array[0][b];
					temp += text[i++];
				}
				else if (is_operators_1(text[i]))
				{	state = state_array[0][c];
					temp += text[i++];
				}
				else if (is_separator(text[i]))
				{	state = state_array[0][d];
					temp += text[i++];
				}
				break;
			case 1:
				if (is_char(text[i]))
				{	state = state_array[1][a];
					temp += text[i++];
				}
				else if (is_number(text[i]))
				{	state = state_array[1][b];
					temp += text[i++];
				}	
				else if (is_operators_1(text[i]))
				{	if (is_reserved_word(temp))		//1 保留字
						cout << "( " << Reserved_Word << " , " << temp << " )" << endl;
					else							//2 标识符
						cout << "( " << Identifier << " , " << temp << " )" << endl;
					temp = "";
					state = state_array[1][c];
					temp += text[i++];
				}
				else if (is_separator(text[i]))
				{	if (is_reserved_word(temp))		//1 保留字
						cout << "( " << Reserved_Word << " , " << temp << " )" << endl;
					else							//2 标识符
						cout << "( " << Identifier << " , " << temp << " )" << endl;
					temp = "";
					state = state_array[1][d];
					temp += text[i++];
				}
				break;   
			case 2:
				if (is_char(text[i]))
				{	state = state_array[2][a];
					temp += text[i++];
				}
				else if (is_number(text[i]))
				{	state = state_array[2][b];
					temp += text[i++];
				}
				else if (is_operators_1(text[i]))		//3 无符号整数
				{	cout << "( " << Unsigned_Int << " , " << temp << " )" << endl;	temp = "";
					state = state_array[2][c];
					temp += text[i++];
				}
				else if (is_separator(text[i]))			//3 无符号整数
				{	cout << "( " << Unsigned_Int << " , " << temp << " )" << endl;	temp = "";
					state = state_array[2][d];
					temp += text[i++];
				}
				break;
			case 3:	
				if ((text[i - 1] != ' ') && text[i - 1] != '\t')		//5 分隔符
					cout << "( " << Separator << " , " << temp << " )" << endl;	temp = "";
				state = 0;
				break;
			case 4:
				if (is_operators_1(text[i - 1]))		//4 运算符
				{	if (is_operators_2(temp + text[i]))
						temp += text[i++];
					cout << "( " << Operators << " , " << temp << " )" << endl;	temp = "";
					temp += text[i++];
				}
				else if (is_char(text[i]))
				{	state = state_array[4][a];
					temp += text[i++];
				}
				else if (is_number(text[i]))
				{	state = state_array[4][b];
					temp += text[i++];
				}
				else if (is_separator(text[i]))
				{	state = state_array[4][d];
					temp += text[i++];
				}
				break;
		}
	}
	cout << endl;
	return 0;
}

