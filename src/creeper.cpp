#include <iostream>
#include <sqlite3.h>
#include <string.h>
#include <windows.h>
#include <Lmcons.h>

using std::cout;
using std::cin;
using std::string;


//Functions
void selectFromDB(sqlite3** db);
void display(sqlite3 *db, string tbname);

//callback
int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    cout << azColName[i] 
          << " = ";
    if( argv[i] )
        cout << argv[i];
    else
        cout << "NULL";
     cout << "\n";
  }
  return 0;
}

//dberr
// This function just display an error message that
// may have been returned by the various Sqlite functions.
void dsperr(char**db_err)
{
    if( *db_err )
    {
        cout << *db_err << "\n";
        sqlite3_free(*db_err); // release (free) memory
        *db_err = 0; // reinitialize the caller's pointer
    }
}


// This function is called from several places to get 
// the table name.  
void GetTablename(string& tbname)
{
    cout << "Enter the table name\n";
    cin >> tbname;
}

//get Username
void GetUsername(string& s)
{
	TCHAR username [UNLEN +1];
	DWORD size = UNLEN +1;

	GetUserName(  (TCHAR*) username, &size );
			s = username;
}

//GetVersion Function
void GetWinVersion(int& ver)
{
	//Get windows version
	DWORD dwVersion = 0;
	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	DWORD dwComVer = 0;

	dwVersion = GetVersion();

	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
   
    ver = dwMajorVersion;
}
int main()
{
	sqlite3* db = 0;
	bool done = false;
	while(!done)

	{
		string answer;
		cout << "\n\nPlease select one of the options\n";
		cout << "1. Connect to DB\n";
		cout << "2. Get Email addresses\n";
		cout << "3. Get Credit Card details\n";
		cout << "4. Get Names\n";
		cout << "5. Get Phone Numbers\n";
		cout << "6. Get Logins\n";
		cout << "7. Quit\n";
		cin >> answer;
		cin.ignore();
		switch(answer[0])	
		{
			case '1':
				selectFromDB(&db);
				break;
			case '2':
				display(db, "autofill_profile_emails");
				break;
			case '3':
				display(db, "credit_cards");
				break;
			case '4':
				display(db, "autofill_profile_names");
				break;
			case '5':
				display(db, "autofill_profile_phones");
				break;
			case '6':
				display(db, "ie7_logins");
				break;
			case '7':
				done = true;
				break;
		}
	}
		sqlite3_close(db);
	

}
void selectFromDB(sqlite3** db)
{
	int ver;
	string s;
	string dbname;
	GetUsername(s);
	GetWinVersion(ver);

	if (ver == 6) {
		dbname = "C:\\Users\\" + s + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Web Data";
	}
	else {
		dbname = "C:\\Documents and Settings\\" + s + "\\Local Settings\\Application Data\\Google\\Chrome\\User Data\\Default\\Web Data";
	}	
	
	int rc = sqlite3_open(dbname.c_str(), db);
    if( rc != SQLITE_OK ){
      cout << "Can't open database	\n";
   }else
      cout << "Opened database successfully\n";	
      cout << dbname;
}
void display(sqlite3* db, string tbname)
{
    char* db_err = 0;
    string select;
    if( tbname.length() == 0)
    {
        GetTablename(tbname);
        if( tbname == "" )
            return;
    }
    if( tbname.length() > 0)
    {
        select = "select * from " + tbname + ";";
        sqlite3_exec(db, select.c_str(), callback, 0, &db_err);
        dsperr(&db_err);
    }
}
