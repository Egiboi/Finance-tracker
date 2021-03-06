// Projekti object orientated.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <time.h>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <istream>
#define dateformat 2
#define maxDescription 50
#define maxName 40
#define maxAccountValue 9999999
#define maxInitialAccount 1000000
#define maxTransactionValue 500000
#define maxYear 2030
#define minYear 1990
#define format12 12
#define format20 20
#define format8 8
#define formatDecimal 2
#define fileName "account.bin"
using namespace std;

//Altered struct tm for custom constructor
class myTM {
	friend ostream &operator<<(ostream & out, const myTM &myTM);
public:
	myTM(int min, int hour, int day, int month, int year) {
		date.tm_min = min;
		date.tm_hour = hour;
		date.tm_mday = day;
		date.tm_mon = month;
		date.tm_year = year;
	}
	myTM() {};
	myTM(myTM &copy) {
		*this= copy;
	}
	void initialization();
	
private:
	struct tm date;
};


class Transaction {
	friend ostream &operator<<(ostream & out, const Transaction &transaction);
public:
	Transaction(float tValue, myTM &tDate, string tDescription);

	Transaction() {
	};
	void initialization();

	myTM getDate() { return date; }
	float getValue() {return value;}
	string getDescription(){return description;}
private:
	//Date of transaction
	myTM date;
	//Monetery value of transaction
	float value;
	//Description for tranaction
	char description[maxDescription+1];
	//type of transaction. Expense = false, income = true
	bool type;

};

class Account  {
	friend ostream &operator<<(ostream & out, const Account &account);
	friend ostream &operator<<(ofstream & file, Account &account);
	friend istream &operator>>(ifstream & file, Account &account);
public:
	Account(string aName="Test", float aBalance = 0) {
		strncpy_s(name, aName.c_str(),maxName+1);
		balance = aBalance;
	}

	void initialization();
	void addTransaction(Transaction &entry);

private:
	vector<unique_ptr<Transaction>> transactionList;
	float balance;
	char name[maxName+1];
};

Transaction::Transaction(float tValue, myTM &tDate, string tDescription) {
	value = tValue;
	date = tDate;
	strncpy_s(description, tDescription.c_str(), maxDescription+1);
	if (tValue >= 0) {
		type = true;
	}
	else {
		type = false;
	}
}
//Function that adds transactions to vector if max value won't be reached
void Account::addTransaction(Transaction &entry) {
	if ((balance + entry.getValue())<=maxAccountValue && (balance + entry.getValue())>= -maxAccountValue) {
		unique_ptr<Transaction> newEntry(new Transaction((entry)));
		balance += entry.getValue();
		transactionList.push_back(move(newEntry));
		cout << "Transaction added\n" << endl;
	}
	else {
		cout << "Max value for account will be met. Transaction has been discarded\n" << endl;
	}
}


//used for interactive transaction initialization
void Transaction::initialization() {
	string value, description;
	float fvalue;
	myTM date;
	while (true) {
		cout << "Enter transaction sum, value must be +/-"<<maxTransactionValue << endl;
		getline(cin, value);
		try {
			fvalue = stof(value);
		}
		catch (...) {
			cout << "Value not readable, enter float" << endl;
			continue;
		}
		if (fvalue> maxTransactionValue || fvalue < -maxTransactionValue) {
			cout << "Value not inside parameters" << endl;
			continue;
		}
		this->value = fvalue;
		if (fvalue >= 0) {
			this->type = true;
		}
		else {
			this->type = false;
		}
		break;
	}
	while (true) {
		cout << "Enter transaction description, max characters 50" << endl;
		getline(cin, description);
		if (description.length() > maxDescription) {
			cout << "Description too long" << endl;
			continue;
		}
		strncpy_s(this->description, description.c_str(), maxDescription+1);
		break;
	}
	date.initialization();
	this->date = date;

}
//interactive account initialization
void Account::initialization(){
	string name, value;
	while (true) {
		cout << "Enter Account name max characters "<<maxName << endl;
		getline(cin, name);
		if (name.length() > maxName) {
			cout << "Name too long" << endl;
			continue;
		}
		strncpy_s(this->name, name.c_str(),maxName+1);
		break;
	}
	while (true) {
		float val;
		cout << "Enter Account value, initial value must be +/-"<< maxInitialAccount<< endl;
		getline(cin, value);
		try {
			val = stof(value);
		}
		catch (...) {
			cout << "Value not readable, enter float" << endl;
			continue;
		}
		if (val > maxInitialAccount || val<0) {
			cout << "Value must be between "<< maxInitialAccount<<" and 0" << endl;
			continue;
		}
		this->balance = val;
		break;
	}
	
	

}
//Interactive initialization for custom struct TM class
void myTM::initialization(){
	int min, hour, day, month, year, ui;
	bool check = true;

	while(check) {
		cout << "0 to start entering transaction custom date, 1 to enter current time" << endl;
		string line;

		getline(cin, line);
		try {
			ui = stoi(line);
		}
		catch (...) {
			cout << "Incorrect command" << endl;
			continue;
		}
		
		
		time_t timeNow = time(NULL);
		tm currentTime;
		localtime_s(&currentTime, &timeNow);
		//for auto current date
		if (ui == 1) {
			//1900 added is for tm formattin, 0 = 1900
			year = currentTime.tm_year+1900;
			//1 added is for tm formattin, values are 0-11, we want 1-12
			month = currentTime.tm_mon+1;
			day = currentTime.tm_mday;
			hour = currentTime.tm_hour;
			min = currentTime.tm_min;
			check = false;
			*this = myTM(min, hour, day, month, year);
			cout << "Time set" << endl;
		}
		//for custom date
		else if (ui == 0) {
			while (true) {
				cout << "Enter year of transaction, 1990-2030" << endl;
				getline(cin, line);
				year = stoi(line);
				if (year < minYear || year>maxYear) {
					cout << "Input out of bound" << endl;
					continue;
				}

				break;
			}
			while (true) {
				cout << "Enter month of transaction, 1-12" << endl;

				getline(cin, line);
				month= stoi(line);
				if (month < 1 || month>12) {
					cout << "Input out of bound" << endl;
					continue;
				}
				else {
					month--;
				}

				break;
			}
			while (true) {
				cout << "Enter day of transaction, 1-31" << endl;
				getline(cin, line);
				day = stoi(line);
				if (day < 1 || day > 31) {
					cout << "Input out of bound" << endl;
					continue;
				}
				break;
			}
			while (true) {
				cout << "Enter hour of transaction, 0-23" << endl;
				getline(cin, line);
				hour = stoi(line);
				if (hour < 0 || hour > 23) {
					cout << "Input out of bound" << endl;
					continue;
				}
				break;
			}
			while (true) {
				cout << "Enter minute of transaction, 0-59" << endl;
				getline(cin, line);
				min = stoi(line);
				if (min < 0 || min > 59) {
					cout << "Input out of bound" << endl;
					continue;
				}
				break;
			}
			check = false;
			*this = myTM(min, hour, day, month, year);
			cout << "Time set" << endl;
		}
	} 

}



int main()
{
	ifstream myfilein;
	ofstream myfileout;
	string sUI;
	int UI;
	Account account;

	bool run = true, accountReady = false;
	//Main program loop
	while (run) {
		//Initial loop for account creation, shouldn't exit loop without an initialized account
		while (!accountReady) {
			cout << "Welcome to the finance manager" << endl << "Enter 1 to start fresh account, enter 2 to reload old account" << endl;
			getline(cin, sUI);
			try {
				UI = stoi(sUI);
			}
			catch (...) {
				cout << "Enter a numerical command" << endl;
				continue;
			}
			if (UI == 1 || UI == 2) {
				//Initialization of new account
				if (UI == 1) {
					account.initialization();
				}
				//Loads old account
				else if (UI == 2) {
					myfilein >> account;
				}
			}
			else {
				cout << "Incorrect command, enter 1 or 2"<<endl;
				continue;
			}
			accountReady = true;
		}
		
		cout << "Enter 1 to add transaction, 2 to print transactions, 3 to save transactions, 4 to quit (saves on quit)" << endl;
		sUI.clear();
		getline(cin, sUI);
		try {
			UI = stoi(sUI);
		}
		catch (...) {
			cout << "Enter correct command" << endl;
			continue;
		}
		//Add transaction
		if (UI == 1) {
			Transaction transaction;
			transaction.initialization();
			account.addTransaction(transaction);
		}
		//Print account and transactions
		else if (UI == 2) {
			cout << account;
		}
		//Save file manually
		else if (UI == 3) {
			myfileout << account;
		}
		//Quit and save program
		else if (UI == 4) {
			myfileout << account;
			run = false;
		}
		else {
			cout << "Command not accepted, enter, 1, 2, 3 or 4" << endl;
		}

	}
}


//for reading file into account                                                                                                                                               
istream &operator>>(ifstream & file, Account &account) {
	account.transactionList.clear();
	file.open(fileName, ios::in | ios::binary);

	Transaction transaction;
	int i = 0;
	while (!file.eof()) {
		file.seekg(sizeof(Transaction)*i);
		file.read((char*)&transaction, sizeof(Transaction));
		//Error in reading (Full transaction object cannot be read), always ends in this after file ends
		if (!file) {
			file.clear();
			if (!account.transactionList.empty()) {
				Transaction *accountInfo= account.transactionList.back().get();
				strncpy_s(account.name,accountInfo->getDescription().c_str(),40);
				account.balance = accountInfo->getValue();
				account.transactionList.pop_back();
			}
			//If list is empty, back to manual initialization
			else {
				cout << "No existing account, manual initialization started" << endl;
				account.initialization();
			}
			
			break;
		}
		//object read and added
		else {
			account.addTransaction(transaction);
		}
		i++;
	}
	file.close();
	return file;
}



//File write overload
ostream &operator<<(ofstream & file, Account &account) {
	file.open(fileName, ios::out | ios::trunc | ios::binary);
	//Account information is added as a transaction during saving phase to pass account name and initial account balance to file
	myTM placeholderDate(1, 1, 1, 1, 1900);
	Transaction accountNow(account.balance, placeholderDate, account.name);
	account.addTransaction(accountNow);

	cout << "Saving to file" << endl;
	for (auto iter = account.transactionList.begin(); iter != account.transactionList.end(); ++iter) {
		file.write((char*)&**iter,sizeof(Transaction));
	}
	//Pops back extra account info transaction 
	account.transactionList.pop_back();
	file.close();
	return file;

}


//Print overload/formatting block
//Prints all transactions from account
ostream &operator<<(ostream & out, const Account &account) {
	float expense=0;
	cout << endl<<"Account: " << account.name << ", Bank balance: " <<setw(format12)<<fixed<< setprecision(formatDecimal)<< account.balance << setw(format12)<< endl << "TRANSACTIONS:" << endl;
	for (auto iter = account.transactionList.begin(); iter != account.transactionList.end(); ++iter) {
		cout << **iter << endl;
		expense += (**iter).getValue();
	}
	if (expense > 0) {
		cout << endl<<"You made a profit: " << expense<<endl;
	}
	else if (expense < 0) {
		cout << endl<<"You made a loss: " << expense << endl;
	}
	else {
		cout << endl<<"You made it even." << endl;
	}
	return out;
}
//Prints Transaction and calls date print
ostream &operator<<(ostream & out, const Transaction &transaction) {
	string type;
	if (transaction.type == true) {
		type = "Income";
	}
	else {
		type = "Expense";
	}
	cout.precision(2);
	cout << fixed<<left<< setw(format8) << type << right<< setw(format12) << transaction.value << " euro " << transaction.date <<" "<< setw(format20) <<left<< transaction.description;
	return out;
}
//Prints myTM (date)
ostream &operator<<(ostream & out, const myTM &myTM) {
	cout << setfill('0');
	cout <<setw(dateformat)<< myTM.date.tm_hour << ":" << setw(dateformat) << myTM.date.tm_min << " " << setw(dateformat) << myTM.date.tm_mday << "." << setw(dateformat) << myTM.date.tm_mon+1 << "." << setw(dateformat) << myTM.date.tm_year;
	cout << setfill(' ');
	return out;
}

