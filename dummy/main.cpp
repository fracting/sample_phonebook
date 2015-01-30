// Phone Book Project
// Programmer : Ahmad Ahmadi
// Email : ahmadi.dt@gmail.com
// Release Date : Tue Jan 29 2013 17:45:13 GMT+0330 (Iran Standard Time)






#include <conio.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <vector>
#include <fstream>
#include <direct.h>
//#include <string>
//#include <math.h>
//#include <WinBase.h>
//#include <stdio.h>
#define GetCurrentDir _getcwd

using namespace std;





//Enums

enum Color
{
	black = 0,
	dark_blue = 1,
	dark_green = 2,
	dark_aqua,
	dark_cyan = 3,
	dark_red = 4,
	dark_purple = 5,
	dark_pink = 5,
	dark_magenta = 5,
	dark_yellow = 6,
	dark_white = 7,
	gray = 8,
	blue = 9,
	green = 10,
	aqua = 11,
	cyan = 11,
	red = 12,
	purple = 13,
	pink = 13,
	magenta = 13,
	yellow = 14,
	white = 15
};

enum Key
{
	_0 = 48,
	_9 = 57,
	A = 65,
	Z = 90,
	a =  97,
	z = 122,
	Up = 72,
	Left = 75,
	Right = 77,
	Down = 80,
	Enter = 13,
	Esc = 27,
	Del = 83,
	BackSpace = 8,
	Tab = 9,
	Ctrl_q = 17,
	Ctrl_s = 19,
	Ctrl_e = 5,
	Ctrl_f = 6,
	Ctrl_n = 14,
	Ctrl_w = 23,
	Ctrl_z = 26,
	Ctrl_a = 1,
	Ctrl_d = 4

};





//Structs

struct Contact
{
	int id;
	string FirstName;
	string LastName;
	string Mobile;
};

struct ConsoleColor
{
	Color TextColor;
	Color BackColor;
};

struct Position
{
	int x;
	int y;
};

struct ShortCut
{
	string title;
	Key key;
};

struct MenuItem
{
	string title;
	bool isSpecial;
	Key shortcutKey;
};





// Consts

const ShortCut shortCuts[] =
{
	{ "Esc" , Esc },
	{ "Enter" , Enter },
	{ "Del" , Del },
	{ "Ctrl+Q" , Ctrl_q },
	{ "Ctrl+S" , Ctrl_s },
	{ "Ctrl+E" , Ctrl_e },
	{ "Ctrl+F" , Ctrl_f },
	{ "Ctrl+N" , Ctrl_n }
};

const MenuItem contactsPageMenu[] =
{
	{ "Add" , false , Ctrl_n },
	{ "Edit" , true , Ctrl_e },
	{ "Delete" , true , Del },
	{ "Exit" , false , Ctrl_q },
};

const int pageSize = 10;

const string contactFieldSplitter = "|";

const ConsoleColor
	highlightColor = { Color::black, Color::white },
	normalColor = { Color::white, Color::black };

const Position
	contactNewPosition = { 0, 0 },
	contactsTableHeaderPosition = { 0, 0 },
	contactsTableRowsPosition = { 0, 2 },
	contactsTablePagerPosition = { 0, pageSize + 3},
	confirmPosition = { 15, 10 },
	contactFormPosition = { 5, 2 },
	contactSearchPosition = { 0, contactsTablePagerPosition.y + 2 },
	contactsPageMenuPosition = { 0, contactSearchPosition.y + 2 },
	aboutPosition = { 0, contactsPageMenuPosition.y + 10 };

const HANDLE stdOutHandle = GetStdHandle( STD_OUTPUT_HANDLE );





// Global Variables

int
	currentContactIndex = 0,
	currentPageIndex = 0;

vector<Contact>
	originalContacts,
	foundContacts;

string searchKey;





// Functions Prototype

WORD getWordAttribute();
void hideCursor();
void showCursor();
string getShortCutKeyTitle(Key key);
int getTextColor();
int getBackColor();
string getCurrentPath();
bool fileExists(string path);
void createFile(string path);
int getCols();
int getPageSize();
int getPageCount();
int getContactStartIndex();
int getContactEndIndex();
void fillContacts(const int contactToMake);
void splitToString(const string& str, const string& delimiters , vector<string>& tokens);
Contact createContactFromString(string data);
void loadContacts();
string createMenuItem(const string itemText, const Key shortCutKey , const int maxMenuItemTextLength , const int maxShortCutTitleLength);
void printLineSplitter();
void clearLine(int y, int lineLength);
int getMaxMenuItemLength();
int getMaxShortCutTitleLength(const ShortCut shortCuts[], const int size);
bool isSelectedContactInCurrentPage();
void printContactsPageMenu(bool contactExists);
void printContatsHeader();
void printContactsRows(int activeIndex = -1);
void printContactsPager();
void printContactsNotExists();
void setColor(ConsoleColor consoleColor);
void setColor(Color textColor, Color backColor);
void setColor(int textColor, int backColor);
void gotoPosition(Position position);
void gotoPosition(int x, int y);
void syncContacts();
void addNewContact(Contact contact);
void printNewContactPage();
void printContactsPage();
void printConfirmExitPage();
void printConfirmDeletePage(Contact contact);
void deleteContact(int id);
int findContactById(int id);
void searchContacts(string key);
void printSearchBox();
void printAbout();
void sortContacts();
void printEditContactPage(Contact contact);
void updateContact(Contact contact);
void clearScreen();
bool stringHastSubstring(string main, string subString , bool ignoreCase = true);
string tolower(string text);





// Main Function

int main()
{
	printAbout();

	srand(std::time(0));

	// Test Data
	//fillContacts(12);

	loadContacts();
	searchContacts("");
	printContactsPage();

	return 0;
}





// Functions Definition

template <typename T,unsigned S>
unsigned arraysize(const T (&v)[S]) { return S; }

WORD getWordAttribute()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(stdOutHandle, &csbi);
	return csbi.wAttributes;
}

void hideCursor()
{
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo( stdOutHandle, &structCursorInfo ); // Get current cursor size
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo( stdOutHandle, &structCursorInfo );
}

void showCursor()
{
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo( stdOutHandle, &structCursorInfo ); // Get current cursor size
	structCursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo( stdOutHandle, &structCursorInfo );
}

string getShortCutKeyTitle(Key key)
{
	int shortCutsCount = arraysize(shortCuts);

	for (int i = 0; i < shortCutsCount; i++)
		if(shortCuts[i].key == key)
			return shortCuts[i].title;
	return "";
}

int getTextColor()
{
	return getWordAttribute() % 16;
}

int getBackColor()
{
	return ( getWordAttribute() / 16 ) % 16;
}

string getCurrentPath()
{
	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		cerr << errno;
	}

	const int size = sizeof(cCurrentPath) - 1;

	cCurrentPath[size] = '\0';

	stringstream tmp;
	string path;

	tmp << cCurrentPath;
	getline( tmp, path );

	return path;
}

bool fileExists(string path)
{
	ifstream fin( path.c_str() );
	return !fin.fail();
}

void createFile(string path)
{
	ofstream file(path, ios::out);
	file << "";
	file.close();
}

int getCols()
{	
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return -1;
	else
		return csbi.srWindow.Right - csbi.srWindow.Left + 1; // Window width
}

int getPageSize()
{
	if(pageSize > foundContacts.size())
		return foundContacts.size();
	return pageSize;
}

int getPageCount()
{
	int count = ceil( (float)foundContacts.size() / (float)pageSize );
	
	if(count == 0)
		count++;

	return count;
}

int getContactStartIndex()
{
	return currentPageIndex * getPageSize();
}

int getContactEndIndex()
{
	int endIndex = currentPageIndex * getPageSize() + ( getPageSize() - 1 );

	if(endIndex >= foundContacts.size())
		endIndex = foundContacts.size() - 1;

	return endIndex;
}

void fillContacts(const int contactToMake)
{
	for (int i = 0; i < contactToMake; i++)
	{
		Contact c;
		originalContacts.push_back(c);

		originalContacts[i].id = rand();

		stringstream tmp;
		tmp << "First Name " << ( i + 1 );
		originalContacts[i].FirstName = tmp.str();
		tmp.str(std::string());

		tmp << "Last Name " << ( i + 1 );
		originalContacts[i].LastName = tmp.str();
		tmp.str(std::string());

		tmp << "093550936" << ( i + 1 );
		originalContacts[i].Mobile = tmp.str();
	}
}

void splitToString(const string& str, const string& delimiters , vector<string>& tokens)
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

Contact createContactFromString(string data)
{
	Contact tmp;
	vector<string> items;
	splitToString(data, contactFieldSplitter,items);

	stringstream stmp;
	stmp << items[0];
	stmp >>	tmp.id;

	tmp.FirstName = items[1];
	tmp.LastName = items[2];
	tmp.Mobile = items[3];

	return tmp;
}

void loadContacts()
{
	const string fileName = "Contacts.txt";

	string p = getCurrentPath() + "\\" + fileName;

	if(!fileExists(p))
		createFile(p);

	ifstream contactsFile( p, ios::in );

	if ( !contactsFile )
	{
		cerr << "File could not be opened" << endl;
		exit( 1 );
	}

	while (!contactsFile.eof())
	{
		string tmp;
		getline( contactsFile, tmp );

		if (std::string::npos != tmp.find(contactFieldSplitter))
			originalContacts.push_back(createContactFromString(tmp));
	}
}

string createMenuItem(const string itemText, const Key shortCutKey , const int maxMenuItemTextLength , const int maxShortCutTitleLength)
{
	string shortCutTitle = getShortCutKeyTitle(shortCutKey);
	stringstream tmp;
	tmp << left << setw(2) << "-" << setw(maxMenuItemTextLength + 1) << itemText;

	if(shortCutTitle != "")
		tmp << setw(maxShortCutTitleLength + 1) << shortCutTitle;

	tmp << endl;

	return tmp.str();
}

void printLineSplitter()
{
	for (int i = 0; i < 72; i++)
		cout << "-";
	cout << endl;
}

void clearLine(int y, int lineLength)
{
	for (int i = 0; i < lineLength; i++)
	{
		gotoPosition( i, y );
		cout << " ";
	}
}

int getMaxMenuItemLength()
{
	int max = 0;
	int size = arraysize(contactsPageMenu);

	for (int i = 0; i < size; i++)
	{
		if(max < contactsPageMenu[i].title.length())
			max = contactsPageMenu[i].title.length();
	}
	return max;
}

int getMaxShortCutTitleLength(const ShortCut shortCuts[], const int size)
{
	int max = 0;
	for (int i = 0; i < size; i++)
	{
		if(max < shortCuts[i].title.length())
			max = shortCuts[i].title.length();
	}
	return max;
}

bool isSelectedContactInCurrentPage()
{
	return ( getContactStartIndex() <= currentContactIndex ) && ( getContactEndIndex() >= currentContactIndex );
}

void printContactsPageMenu(bool contactExists)
{
	int menuItemsCount = arraysize(contactsPageMenu);

	for (int i = 1; i <= menuItemsCount; i++)
		clearLine(contactsPageMenuPosition.y + i, getCols()/2);

	gotoPosition(contactsPageMenuPosition);
	printLineSplitter();

	int maxMenuItemTextLength = getMaxMenuItemLength();
	int maxShortCutTitleLength = getMaxShortCutTitleLength(shortCuts, arraysize(contactsPageMenu));

	for (int i = 0; i < menuItemsCount; i++)
	{
		MenuItem item = contactsPageMenu[i];
		if( (!item.isSpecial) || (item.isSpecial && contactExists) )
			cout << createMenuItem(item.title , item.shortcutKey , maxMenuItemTextLength , maxShortCutTitleLength );
	}
}

void printContatsHeader()
{
	gotoPosition(contactsTableHeaderPosition);
	cout << left << setw(20) << "First Name" << setw(20) << "Last Name" << setw(20) << "Mobile" << endl;
	printLineSplitter();
}

void printContactsRows(int activeIndex)
{	
	gotoPosition(contactsTableRowsPosition);

	for (int i = getContactStartIndex(); i <= getContactEndIndex(); i++)
	{
		if( ( activeIndex == -1 && i == currentContactIndex ) || ( activeIndex == i ) )
			setColor(highlightColor);
		else
			setColor(normalColor);

		Contact contact = foundContacts[i];
		cout << left << setw(20) << contact.FirstName << setw(20) << contact.LastName << setw(20) << contact.Mobile << endl;

		setColor(normalColor);
	}

	int contactInThisPage = ( getContactEndIndex() - getContactStartIndex() ) + 1;
	if( contactInThisPage < pageSize )
		for (int i = 0; i < ( pageSize - contactInThisPage ); i++)
		{
			gotoPosition(contactsTableRowsPosition.x, contactsTableRowsPosition.y + contactInThisPage + i);
			cout << setw(60) << " ";
		}
}

void printContactsPager()
{
	clearLine(contactsTablePagerPosition.y, getCols());
	gotoPosition(contactsTablePagerPosition);
	int pageCount = getPageCount();

	if(pageCount > 0)
	{
		cout << "|";
		for (int i = 0; i < pageCount; i++)
		{
			if(i == currentPageIndex)
				setColor(highlightColor);
			else
				setColor(normalColor);

			cout << " " << ( i + 1 ) << " ";

			setColor(normalColor);
			cout << "|";
		}
	}
}

void printContactsNotExists()
{
	for (int i = 0; i < contactSearchPosition.y; i++)
		clearLine(i, getCols());

	gotoPosition(contactsTableHeaderPosition);
	cout << "No Contact Exists!" << endl;
}

void setColor(ConsoleColor consoleColor)
{
	setColor(consoleColor.TextColor, consoleColor.BackColor);
}

void setColor(Color textColor, Color backColor)
{
	setColor((int)textColor, (int)backColor);
}

void setColor(int textColor, int backColor)
{
	if( ( textColor % 16 ) == ( backColor % 16 ) )
		textColor++;

	textColor %= 16;
	backColor %= 16;

	unsigned short wAttributes = ( (unsigned)backColor << 4 ) | (unsigned)textColor;
	SetConsoleTextAttribute(stdOutHandle, wAttributes);
}

void gotoPosition(Position position)
{
	gotoPosition( (int)position.x, (int)position.y );
}

void gotoPosition(int x, int y)
{
	COORD pos;
	if (INVALID_HANDLE_VALUE != stdOutHandle)
	{
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition( stdOutHandle, pos );
	}
}

void syncContacts()
{
	sortContacts();

	ofstream contactsFile( "Contacts.txt", ios::out );

	if ( !contactsFile )
	{
		cerr << "File could not be opened" << endl;
		exit( 1 );
	}

	for (int i = 0; i < originalContacts.size(); i++)
	{
		Contact currentContact = originalContacts[i];
		contactsFile << currentContact.id << "|" << currentContact.FirstName << "|" << currentContact.LastName << "|" << currentContact.Mobile << endl;
	}

	contactsFile.close();
}

void addNewContact(Contact contact)
{
	originalContacts.push_back(contact);
	syncContacts();
	currentContactIndex = findContactById( contact.id );
	currentPageIndex = floor( currentContactIndex / pageSize );
}

void printNewContactPage()
{
	clearScreen();
	showCursor();
	Contact newContact;

	gotoPosition(contactNewPosition);

	newContact.id = rand() * rand();

	cout << "First Name : ";
	cin >> newContact.FirstName;
	cout << endl;

	cout << "Last Name : ";
	cin >> newContact.LastName;
	cout << endl;

	cout << "Mobile : ";
	cin >> newContact.Mobile;
	cout << endl;

	addNewContact(newContact);
	printContactsPage();
}

void printContactsPage()
{
	clearScreen();
	hideCursor();

	searchContacts(searchKey);

	bool mustExit = false;

	while (!mustExit)
	{
		searchContacts(searchKey);

		bool contactExists = foundContacts.size() > 0;
		printContactsPageMenu(contactExists);
		printSearchBox();

		if(getPageCount() <= currentPageIndex)
			currentPageIndex = getPageCount() - 1;

		if(!contactExists)
			printContactsNotExists();
		else
		{
			printContatsHeader();
			printContactsPager();

			if(isSelectedContactInCurrentPage())
				printContactsRows();
			else
				printContactsRows(getContactStartIndex());
		}

		int pressedChar = _getch();

		if(pressedChar == Key::Ctrl_n)
		{
			printNewContactPage();
			return;
		}
		else if(pressedChar == Key::Ctrl_q)
		{
			printConfirmExitPage();
			return;
		}
		else if(
			(pressedChar >= a && pressedChar <= z) ||
			//(pressedChar >= A && pressedChar <= Z) ||
			(pressedChar >= _0 && pressedChar <= _9))
		{
			searchKey += pressedChar;
		}
		else if(pressedChar == BackSpace)
		{
			if(searchKey.length() > 0)
			{
				searchKey = string(searchKey.begin(), searchKey.begin() + searchKey.length() - 1);
			}
		}

		if(contactExists)
		{
			if(pressedChar == Key::Up || pressedChar == Key::Down)
			{
				if(isSelectedContactInCurrentPage())
					if(pressedChar == Key::Up)
					{
						currentContactIndex--;
						if(currentContactIndex < getContactStartIndex())
							currentContactIndex = getContactEndIndex();
					}
					else
					{
						currentContactIndex++;
						if( currentContactIndex > getContactEndIndex() )
							currentContactIndex = getContactStartIndex();
					}
				else
					if(pressedChar == Key::Up)
						currentContactIndex = getContactEndIndex();
					else
						currentContactIndex = getContactStartIndex() + 1;
			}
			else if(pressedChar == Key::Ctrl_e || pressedChar == Key::Del)
			{
				Contact contact;

				if(isSelectedContactInCurrentPage())
					contact = foundContacts[currentContactIndex];
				else
					contact = foundContacts[getContactStartIndex()];

				if(pressedChar == Key::Ctrl_e)
					printEditContactPage(contact);
				else
				{
					printConfirmDeletePage(contact);
					return;
				}
			}
			if(getPageCount() > 1)
			{
				if(pressedChar == Key::Left)
				{
					currentPageIndex--;
					if(currentPageIndex < 0)
						currentPageIndex = getPageCount() - 1;
				}
				else if(pressedChar == Key::Right)
				{
					currentPageIndex++;
					if( currentPageIndex > ( getPageCount() - 1 ) )
						currentPageIndex = 0;
				}
			}
		}//End If Contacts Exists
	}//End While
}

void printConfirmExitPage()
{
	clearScreen();

	gotoPosition(confirmPosition);
	cout << "are you sure want to exit?" << endl;

	gotoPosition(confirmPosition.x, confirmPosition.y + 2);
	cout << "(" << getShortCutKeyTitle(Enter) << ":Yes,i want to leave - " << getShortCutKeyTitle(Esc) << ":No,i want stay)" << endl;

	while(true)
	{
		int pressedChar = _getch();

		if(pressedChar == Key::Enter)
			return;
		else if(pressedChar == Key::Esc)
			break;
	}

	printContactsPage();
}

void printConfirmDeletePage(Contact contact)
{
	clearScreen();

	gotoPosition(confirmPosition);
	cout << "Deleting " << contact.FirstName << " " << contact.LastName << endl;

	gotoPosition(confirmPosition.x, confirmPosition.y + 2);
	cout << "are you sure want to Delete?" << endl;

	gotoPosition(confirmPosition.x, confirmPosition.y + 4);
	cout << "(" << getShortCutKeyTitle(Enter) << ":Yes - " << getShortCutKeyTitle(Esc) << ":No)" << endl;

	while(true)
	{
		int pressedChar = _getch();

		if(pressedChar == Key::Enter)
		{
			deleteContact(contact.id);
			break;
		}
		else if(pressedChar == Key::Esc)
			break;
	}

	printContactsPage();
}

void deleteContact(int id)
{
	int index = findContactById(id);

	if(index != -1)
	{
		originalContacts.erase(originalContacts.begin() + index);
		syncContacts();
	}

	if(currentContactIndex > getContactEndIndex())
		currentContactIndex = getContactEndIndex();

	currentPageIndex = floor( currentContactIndex / pageSize );
}

int findContactById(int id)
{
	for (int i = 0; i < originalContacts.size(); i++)
		if(originalContacts[i].id == id)
			return i;
	return -1;
}

void searchContacts(string key)
{
	foundContacts.clear();

	if(key == "")
		foundContacts = originalContacts;
	else
		for (int i = 0; i < originalContacts.size(); i++)
		{
			Contact currentContact = originalContacts[i];
			if(stringHastSubstring(currentContact.FirstName, key) || stringHastSubstring(currentContact.LastName, key) || stringHastSubstring(currentContact.Mobile, key))
				foundContacts.push_back(currentContact);
		}
}

void printSearchBox()
{
	clearLine(contactSearchPosition.y + 1 , getCols());

	gotoPosition(contactSearchPosition);
	printLineSplitter();

	cout << "Search For : " << searchKey;
}

void printAbout()
{
	gotoPosition(aboutPosition);
	
	printLineSplitter();
	printLineSplitter();

	cout
		<< "Phone Book Project" << endl
		<< "Programmer : Ahmad Ahmadi" << endl
		<< "Email : ahmadi.dt@gmail.com" << endl
		<< "Release Date : Tue Jan 29 2013 17:45:13 GMT+0330 (Iran Standard Time)" << endl;
	
	printLineSplitter();
	printLineSplitter();
}

void sortContacts()
{
	if(originalContacts.size() < 1)
		return;

	for (int i = 0; i < originalContacts.size() - 1; i++)
		for (int i = 0; i < originalContacts.size() - 1; i++)
			if(originalContacts[i].FirstName > originalContacts[i + 1].FirstName)
			{
				Contact tmp = originalContacts[i];
				originalContacts[i] = originalContacts[i + 1];
				originalContacts[i + 1] = tmp;
			}
}

void printEditContactPage(Contact contact)
{	
	clearScreen();
	showCursor();
	string tmp;

	gotoPosition(contactNewPosition);

	cout << "editing '" << contact.FirstName << " " << contact.LastName << "'" << endl;
	cout << "(to prevent change field value tpye dot (.) instead of value.)" << endl << endl;

	cout << "First Name : ";
	cin >> tmp;
	if(tmp != ".")
		contact.FirstName = tmp;
	cout << endl;

	cout << "Last Name : ";
	cin >> tmp;
	if(tmp != ".")
		contact.LastName = tmp;
	cout << endl;

	cout << "Mobile : ";
	cin >> tmp;
	if(tmp != ".")
		contact.Mobile = tmp;
	cout << endl;

	updateContact(contact);
	printContactsPage();
}

void updateContact(Contact contact)
{
	int index = findContactById(contact.id);

	if(index != -1)
	{
		originalContacts[index] = contact;
		syncContacts();

		currentContactIndex = findContactById( contact.id );
		currentPageIndex = floor( currentContactIndex / pageSize );
	}
}

void clearScreen()
{
	system("cls");
	printAbout();
}

bool stringHastSubstring(string main, string subString , bool ignoreCase)
{
	if(ignoreCase)
	{
		main = tolower(main);
		subString = tolower(subString);
	}

	if ( std::string::npos != main.find(subString) )
		return true;
	return false;
}

string tolower(string text)
{
	string tmp  = "";
	for (int i = 0; i < text.length(); i++)
		tmp += tolower(text[i]);
	return tmp;
}