// Libraries
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

// Variables
const string ignoreKeyword = "@SysIgnore";
const string fileName = "bankData.txt";
const string delimiter = "|.|";

enum enSecureTransaction { QWT = 1, WIT = 2, DEP = 3, CHK = 4, EXT = 5 };

vector <string> secureActions = {
	"Quick Withdraw",
	"Withdraw",
	"Deposit",
	"Check Blanace",
	"Log-Out",

};
vector <string> quickWithdrawAmounts = {
	"20",
	"50",
	"100",
	"200",
	"400",
	"600",
	"800",
	"1000",

};

struct stBankAccount {
	string accountNumber;
	string PINCode;
	string name;
	string phone;

	float balance = 0.00;

	bool deleteMark = false;

};

stBankAccount defaultUser;
stBankAccount* activeUser;

// Generic Functions
string tabs(short numberOfTabs)
{
	string tabs;

	for (short i = 0; i < numberOfTabs; i++)
	{
		tabs += "\t";

	}

	return tabs;

}
string spaces(short numberOfSpaces)
{
	string spaces;

	for (short i = 0; i < numberOfSpaces; i++)
	{
		spaces += ' ';

	}

	return spaces;

}

bool readConfirmation()
{
	char n;
	cout << "ARE YOU SURE TO PREOCCED WITH THIS ACTION (Y:YES | N:NO)? ";
	cin >> n;

	return (n == 'Y' || n == 'y');

}

short inputNumberInRange(short from, short to, string message = "")
{
	short number = from - 1;

	if (message == "")
	{
		message = "Please insert a number [" + to_string(from) + " ~ " + to_string(to) + "]:";

	}

	while (number < from || number > to)
	{
		cout << message;
		cin >> number;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

		}
		else
		{
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

		}
	}

	return number;

}
double inputNumberInRange(double from, double to, string message = "")
{
	double number = from - 1;

	if (message == "")
	{
		message = "Please insert a number [" + to_string(from) + " ~ " + to_string(to) + "]:";

	}

	while (number < from || number > to)
	{
		cout << message;
		cin >> number;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

		}
		else
		{
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

		}
	}

	return number;

}

string inputString(short from = 0, short to = numeric_limits<std::streamsize>::max(), string message = "", string warnning = "")
{
	string result;
	bool repeat = true;

	if (message == "")
	{
		message = "Please insert a text: ";

	}
	if (warnning == "")
	{
		warnning = "Invalid String!";

	}

	while (repeat)
	{
		repeat = false;
		cout << message;
		getline(cin >> ws, result);

		if (result.length() < from || result.length() > to)
		{
			repeat = true;
			cout << "==================\n";
			cout << warnning << "\n" << endl;

		}
	}

	return result;

}
vector <string> splitString(string text, string delimiter)
{
	string word;
	vector <string> result;
	size_t pos;

	while ((pos = text.find(delimiter)) != string::npos)
	{
		word = text.substr(0, pos);

		if (text != "")
		{
			result.push_back(word);

		}

		text = text.substr(pos + delimiter.length(), text.length());

	}

	if (text != "")
	{
		result.push_back(text);

	}

	return result;

}
string reverseString(string text)
{
	string result;

	for (char n : text)
	{
		result = n + result;

	}

	return result;

}

// Cryptography Functions
string encryptText(string text, short encryptionKey = '0')
{
	string encryptionResult;
	string word;

	for (char n : text)
	{
		word += n + encryptionKey;

		if (word.length() > 4)
		{
			encryptionResult += reverseString(word);
			word.clear();

		}
	}

	if (word != "")
	{
		encryptionResult += reverseString(word);

	}

	return encryptionResult;

}
string decryptText(string text, short decryptionKey = '0')
{
	string decryptionResult;
	string word;

	for (char n : text)
	{
		word += n - decryptionKey;

		if (word.length() > 4)
		{
			decryptionResult += reverseString(word);
			word.clear();

		}
	}

	if (word != "")
	{
		decryptionResult += reverseString(word);

	}

	return decryptionResult;

}

// Functions
vector <string> readFile(string fileName)
{
	vector <string> fileData;
	fstream file;

	short count = 0;

	file.open(fileName, ios::in);

	if (file.is_open())
	{
		string line;

		while (getline(file, line))
		{
			fileData.push_back(decryptText(line, pow('0', 2) + ++count));

		}

		file.close();

	}
	else
	{
		cout << "Error opening file." << endl;

	}

	return fileData;

}
void saveFile(string fileName, vector <string>& records)
{
	fstream file;

	short count = 0;

	file.open(fileName, ios::out);

	if (file.is_open())
	{
		for (string& record : records)
		{
			if (record != ignoreKeyword)
			{
				file << encryptText(record, pow('0', 2) + ++count) << endl;

			}
		}

		file.close();

	}
	else
	{
		cout << "Error opening file." << endl;

	}
}

void saveBankRecords(vector <string>& bankRecords)
{
	saveFile(fileName, bankRecords);

}
vector <string> getBankRecords()
{
	return readFile(fileName);

}
stBankAccount unpackAccountRecord(string& accountRecord)
{
	stBankAccount bankAccount;
	vector <string> record = splitString(accountRecord, delimiter);

	bankAccount.accountNumber = record[0];
	bankAccount.PINCode = record[1];
	bankAccount.name = record[2];
	bankAccount.phone = record[3];
	bankAccount.balance = stof(record[4]);

	return bankAccount;

}
vector <stBankAccount> unpackBankRecords(vector <string>& bankRecords)
{
	vector <stBankAccount> bankAccounts;
	stBankAccount account;

	for (string& record : bankRecords)
	{
		account = unpackAccountRecord(record);
		bankAccounts.push_back(account);

	}

	return bankAccounts;

}
string packAccountRecord(stBankAccount& account)
{
	string accountRecord = account.accountNumber + delimiter;
	accountRecord += account.PINCode + delimiter;
	accountRecord += account.name + delimiter;
	accountRecord += account.phone + delimiter;
	accountRecord += to_string(account.balance);

	return accountRecord;

}
vector <string> packBankRecords(vector <stBankAccount>& bankAccounts)
{
	vector <string> bankRecords;
	string accountRecord;

	for (stBankAccount& account : bankAccounts)
	{
		if (account.deleteMark)
		{
			continue;

		}

		accountRecord = packAccountRecord(account);
		bankRecords.push_back(accountRecord);

	}

	return bankRecords;

}

void displayOptions(vector <string>& list)
{
	for (short i = 0; i < list.size(); i++)
	{
		cout << spaces(2) << '[' << i + 1 << ']' << spaces(2) << list.at(i) << "\n";

	}
}
void displayOptionsScreen(vector <string> list = secureActions, string title = "ATM Main Menu")
{
	cout << "--------------------------------------\n";
	cout << right << setw(21) << title << '\n';
	cout << "--------------------------------------\n";

	displayOptions(list);

	cout << "--------------------------------------\n";

}

void displayDatabaseHeader(short accountsNumber)
{
	cout << right << setw(60) << "Accounts List (" << accountsNumber << ") Account(s).";
	cout << "\n------------------------------------------------------------------------------------------------------------------------";
	cout << left;
	cout << setw(20) << "| Account Number";
	cout << setw(10) << "| PIN ";
	cout << setw(50) << "| Client Name ";
	cout << setw(20) << "| Phone ";
	cout << setw(20) << "| Balance";
	cout << "\n------------------------------------------------------------------------------------------------------------------------";

}
void displayActionScreen(enSecureTransaction& transaction)
{
	string actionText;

	cout << "--------------------------\n";

	switch (transaction)
	{
	case WIT:
		actionText = "WIT Operation";
		break;

	case DEP:
		actionText = "DEP Operation";
		break;

	}

	cout << setw(20) << actionText << '\n';
	cout << "--------------------------" << endl;

}
void displayAccountData(stBankAccount& bankAccount)
{
	cout << left;
	cout << setw(20) << "| " + bankAccount.accountNumber;
	cout << setw(10) << "| " + bankAccount.PINCode;
	cout << setw(50) << "| " + bankAccount.name;
	cout << setw(20) << "| " + bankAccount.phone;
	cout << setw(20) << "| " + to_string(bankAccount.balance) << endl;

}
void displayAccountBalance(stBankAccount& bankAccount)
{
	cout << left;
	cout << setw(20) << "| " + bankAccount.accountNumber;
	cout << setw(50) << "| " + bankAccount.name;
	cout << setw(20) << "| " + to_string(bankAccount.balance) << endl;

}
void displayBankAccountCard(stBankAccount& bankAccount = *activeUser)
{
	cout << endl;
	displayDatabaseHeader(1);
	displayAccountData(bankAccount);
	cout << "\n------------------------------------------------------------------------------------------------------------------------\n" << endl;

}

void displayLoginScreen()
{
	cout << "--------------------------\n";
	cout << setw(20) << "Log-in Screen" << '\n';
	cout << "--------------------------" << endl;

}

tuple <bool, short> findAccount(vector <stBankAccount>& bankAccounts, string& accNo)
{
	for (int i = 0; i < bankAccounts.size(); i++)
	{
		if (bankAccounts[i].accountNumber == accNo && !bankAccounts[i].deleteMark)
		{
			return make_tuple(true, i);

		}
	}

	return make_tuple(false, 0);

}

void quickWithdraw(stBankAccount& bankAccount = *activeUser)
{
	double withdrawAmount;
	bool confirm;

	short actionsNumber = quickWithdrawAmounts.size();
	short action;

	string MSG = "Choose amount to withdraw [1 ~ ";
	MSG += actionsNumber + '0';
	MSG += "]? (0 to Quit) ";

	displayBankAccountCard(bankAccount);
	displayOptionsScreen(quickWithdrawAmounts, "Quick Withdraw");

	do
	{
		action = inputNumberInRange(0, actionsNumber, MSG);

		if (action == 0)
			break;

		withdrawAmount = stoi(quickWithdrawAmounts[action - 1]);

		if (withdrawAmount > bankAccount.balance)
			cout << "Insufficient Balance!" << endl;

	} while (withdrawAmount > bankAccount.balance);

	if (action == 0)
		return;

	system("cls");

	stBankAccount previewAccount = bankAccount;
	previewAccount.balance -= withdrawAmount;

	displayBankAccountCard(previewAccount);
	confirm = readConfirmation();

	if (confirm)
	{
		bankAccount.balance -= withdrawAmount;

	}
}
void withdraw(stBankAccount& bankAccount = *activeUser)
{
	string withdrawMSG = "Withdraw Amount (Max: " + to_string(bankAccount.balance) + ") (-1 to Quit): ";
	int withdrawAmount;
	bool confirm;

	displayBankAccountCard(bankAccount);

	do
	{
		withdrawAmount = inputNumberInRange(0, bankAccount.balance, withdrawMSG);

		if (withdrawAmount % 5 != 0)
			cout << "Value must be a multiple of 5!\n";

	} while (withdrawAmount % 5 != 0 && withdrawAmount != 0);

	system("cls");

	if (withdrawAmount == 0)
		return;

	stBankAccount previewAccount = bankAccount;
	previewAccount.balance -= withdrawAmount;

	displayBankAccountCard(previewAccount);
	confirm = readConfirmation();

	if (confirm)
	{
		bankAccount.balance -= withdrawAmount;

	}
}
void deposit(stBankAccount& bankAccount = *activeUser)
{
	double depositAmount;
	bool confirm;

	displayBankAccountCard(bankAccount);
	depositAmount = inputNumberInRange(0, DBL_MAX, "Deposit Amount (-1 to Quit): ");

	system("cls");

	if (depositAmount == 0.f)
		return;

	stBankAccount previewAccount = bankAccount;
	previewAccount.balance += depositAmount;

	displayBankAccountCard(previewAccount);
	confirm = readConfirmation();

	if (confirm)
	{
		bankAccount.balance += depositAmount;

	}
}

bool performAction()
{
	short actionsNumber = secureActions.size();
	string MSG = "What action to take [1 ~ ";
	MSG += actionsNumber + '0';
	MSG += "]? ";

	enSecureTransaction action = enSecureTransaction(inputNumberInRange(1, actionsNumber, MSG));

	system("cls");

	if (action == enSecureTransaction::DEP || action == enSecureTransaction::WIT)
	{
		displayActionScreen(action);

	}
	
	switch (action)
	{
	case QWT:
		quickWithdraw();
		break;

	case WIT:
		withdraw();
		break;

	case DEP:
		deposit();
		break;

	case CHK:
		displayBankAccountCard();
		system("pause");
		break;

	case EXT:
	default:
		return false;

	}

	return true;

}

bool logIn(vector <stBankAccount>& bankAccounts)
{
	displayLoginScreen();
	activeUser = &defaultUser;

	string accountNo;
	string PINCode;
	short accountPos;
	bool isFound = false;

	cout << "Enter Account Number: ";
	getline(cin, accountNo);

	cout << "Enter PIN Code: ";
	getline(cin, PINCode);

	tie(isFound, accountPos) = findAccount(bankAccounts, accountNo);

	if (isFound)
	{
		if (bankAccounts.at(accountPos).PINCode == PINCode)
		{
            activeUser = &bankAccounts.at(accountPos);
			system("cls");
			return false;

		}
	}

	return true;

}

void startProgram()
{
	vector <string> bankRecords = getBankRecords();
	vector <stBankAccount> bankAccounts = unpackBankRecords(bankRecords);

	bool relog = true;
	bool repeat = true;

	while (true)
	{
		repeat = true;
		relog = logIn(bankAccounts);

		if (relog)
		{
			system("cls");
			cout << "Invalid Account Number/PINCode!\n" << endl;
			continue;

		}

		while (repeat)
		{
			system("cls");
			displayOptionsScreen();
			repeat = performAction();

			bankRecords = packBankRecords(bankAccounts);

		}

		saveBankRecords(bankRecords);

	}
}

int main()
{
	startProgram();

	return 0;

}
