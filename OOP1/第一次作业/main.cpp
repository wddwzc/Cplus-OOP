#include <iostream>
#include <string>
#define MAX_DATA_SIZE 100

using namespace std;

struct Sales_data {
	string bookno;
	unsigned int unit_sold;
	double income;
};

enum Option {
	EXIT = 0,
	READ_RECORDS,
	PRINT_LOGS
};

bool Read(istream &is, Sales_data *pData);
void Print(ostream &os, Sales_data *pData);
int logs(Sales_data *pRec, Sales_data *pLogs, int nSize);

int main() {
	struct Sales_data logs_data[MAX_DATA_SIZE], rec_data;
	int opt = 1;
	while (opt) {
		system("cls");
		cout << "Please select the option: 1:Read 2:Print 0:Exit" << endl;
		cin >> opt;
		switch (opt) {
			case READ_RECORDS: {
				system("cls");
				cout << "Please enter the book record:(enter end to finish)" << endl;
				while (Read(cin, &rec_data)) {
					logs(&rec_data, logs_data, MAX_DATA_SIZE);
				};
				break;
			}
			case PRINT_LOGS: {
				system("cls");
				cout << "The book records is as follows:" << endl;
				Print(cout, logs_data);
				break;
			}
			default: {
				break;
			}
		}
		system("pause");
	}
	return 0;
}

bool Read(istream &is, Sales_data *pData) {
	is >> pData->bookno;
	if (pData->bookno == "end") {
		return false;
	}
	else {
		is >> pData->unit_sold >> pData->income;
		pData->income = pData->unit_sold * pData->income;
		return true;
	}
}

void Print(ostream &os, Sales_data *pData) {
	for (int i = 0; !pData[i].bookno.empty(); ++i) {
		os << pData[i].bookno << " " << pData[i].unit_sold << " " << pData[i].income << endl;
	}
}

int logs(Sales_data *pRec, Sales_data *pLogs, int nSize) {
	int i = 0;
	for (i = 0; i < nSize; ++i) {
		if (!pLogs[i].bookno.empty()) {
			if (pRec->bookno == pLogs[i].bookno) {
				pLogs[i].unit_sold += pRec->unit_sold;
				pLogs[i].income += pRec->income;
				break;
			}
		}
		else {
			pLogs[i] = (*pRec);
			break;
		}
	}
	if (i == nSize) {
		cout << "This record is invalid." << endl;
		return 0;
	}
	return i;
}
