/************************************************************************
	created:		2007-08-17
	updated:		2007-12-23
	filename:		SQLite.h
	author:			Daniel C. Gindi (danielgindi (at) gmail dot com)
	
	purpose:		Wrapper for SQLite3!
				
					There are 2 "public" classes in here,
					  under the namspace SQLite:

					Database	: Which is the main wrapper for an SQLite3
									database.

					Table		: Which is representing a table returned
									from a query on CSQLite

					I'm pretty sure that my classes fully support Unicode.

	Legal notes:	You are free you use these classes for whatever use 
					  you have in mind, even commercial,
					On one condition, don't ever claim you wrote it.
					And if you wanna give me credits, I would like that... :-)
					
	Contact notes:	I can be contacted at (danielgindi (at) gmail dot com)
					If you just wanna say thank you, or better, if you
					  think there's room for improvement for these
					  classes...
*************************************************************************/

#pragma once

#include "sqlite3.h"
#include <string>
#include <vector>

namespace SQLite
{
	//////////////////////////////////////////////////////////////////////////
	// Typedefs
	//////////////////////////////////////////////////////////////////////////

	// Typedef for string
	typedef std::basic_string<TCHAR> stdstring;

	// I use this one for a sequential array in memory,
	//   So I can write directly to it
	typedef std::vector<TCHAR> stdvstring; 
	typedef std::vector<stdvstring> vstrlist;
	typedef vstrlist row;

	//////////////////////////////////////////////////////////////////////////
	// Classes
	//////////////////////////////////////////////////////////////////////////

	class Table; // Forward declaration
	class TablePtr; // Forward declaration

	// Main wrapper
	class Database
	{
	public:
		Database(void);
		virtual ~Database(void);

		// Opens a database, or creates it if it does not exist.
		//
		// Return value is the SQLITE_ defined errors. SQLITE_OK is what you want...
		// You can see the SQLITE_ errors defined in sqlite3.h
		int Open( LPCTSTR strFileName );

		// Closes the databse...
		void Close();

		// Is a database opened? no really, is it?
		bool IsOpen();

		// Returns the sqlite3 pointer.
		// This could be unsafe if you do not know what to do with it,
		//   so it is on your own risk!
		sqlite3 * GetPtr(){ return m_sqlite3; };

		// Every time an error occurs, it will be saved as the last error,
		//   This function will return the last error.
		int GetLastError(){ return m_iLastError; };

		// Puts SQLITE_OK to the last error... :-)
		void ClearError() { m_iLastError=SQLITE_OK; };

		// This will execute an SQL command, and return the results,
		//   if there are any, in a Table class.
		//
		// This is the fastest way, a pointer returned
		//   and you have to free it with delete
		//
		// Returns NULL if no database is opened...
		TablePtr QuerySQL2( LPCTSTR strSQL );

		// This will execute an SQL command, and return the results,
		//   if there are any, in a Table class.
		//
		// You can get more performance with RequestSQLPtr() which
		//   will return a pointer, and spare copying the whole Table
		//   from the return value.
		//
		// Returns an empty Table if no database is opened...
		Table QuerySQL( LPCTSTR strSQL );

		// This will execute an SQL command, and will not expect any results
		//
		// Return value is the SQLITE_ defined errors. SQLITE_OK is what you want...
		// You can see the SQLITE_ errors defined in sqlite3.h
		int ExecuteSQL( LPCTSTR strSQL );

		// This will tell if an SQL command is complete
		//
		// Return value is the SQLITE_ defined errors. SQLITE_OK is what you want...
		// You can see the SQLITE_ errors defined in sqlite3.h
		int IsSQLComplete( LPCTSTR strSQL );

		// This function returns the number of database rows that were changed
		// (or inserted or deleted) by the most recent SQL statement.  Only
		// changes that are directly specified by the INSERT, UPDATE, or
		// DELETE statement are counted.
		int GetLastChangesCount();

		// Reads a BLOB from an SQL statement, 
		// *assuming there's only one result and one field*
		//
		// iLenBlob - A pointer to an int that will receive the length of the BLOB.
		// pData - A pointer to a BYTE* variable.
		//
		// The caller is responsible for freeing the pData memory,
		//   ONLY with free().
		//
		// You should you this function only with SELECT statements.
		// 
		// Return value is the SQLITE_ defined errors. SQLITE_OK is what you want...
		// You can see the SQLITE_ errors defined in sqlite3.h
		int ReadBlob( LPCTSTR strSQL, BYTE ** pData, int * iLenBlob );

		// Writes a BLOB using an SQL statement, 
		// *assuming there's only one field*,
		// You MUST put a ? sign instead of the value in the strSQL
		// ( i.e. "INSERT into blobs (myblob) VALUES(?)" )
		//
		// You should you this function only with INSERT and UPDATE statements.
		//
		// iLenBlob - How many bytes to write.
		// 
		// Return value is the SQLITE_ defined errors. SQLITE_OK is what you want...
		// You can see the SQLITE_ errors defined in sqlite3.h
		int WriteBlob( LPCTSTR strSQL, BYTE * pData, int iLenBlob );

		// Get the Row-ID of the last row inserted using INSERT
		//
		// Returns 0 if not INSERT occured, or if no database is open.
		//
		// Once a trigger terminates, the value returned by this routine 
		//   reverts to the last value inserted before the trigger fired.
		sqlite_int64 GetLastInsertRowID();

		// Begin transaction...
		//
		// Returns false if failed, you may receive error number through GetLastError()
		bool BeginTransaction();

		// Commit transaction...
		//
		// Returns false if failed, you may receive error number through GetLastError()
		bool CommitTransaction();

		// Rollback transaction...
		//
		// Returns false if failed, you may receive error number through GetLastError()
		bool RollbackTransaction();

	private:
		sqlite3 * m_sqlite3;
		int m_iLastError;

		void ConvertUTF8ToString( char * strInUTF8MB, stdvstring & strOut );
	};

	// Table class...
	//
	// I did not implement deleting rows from the table,
	//   because I do no think there will be use for it, at least for me.
	//   for delete records, use SQL... :-)
	// If you still want to implement this, change [m_lstRows] from vector to list
	//   and update [m_iRows] each time you delete a row.
	class Table
	{
		friend class Database; // I allow Database class to enter data directly.
	public:
		Table(void){ m_iRows=m_iCols=0;	m_iPos=-1; };
		virtual ~Table() {};

		
		// Gets the number of columns
		int GetColCount(){ if (this==0) return 0; return m_iCols; };

		// Gets the number of rows
		int GetRowCount(){ if (this==0) return 0; return m_iRows; };

		// Gets the current selected row. -1 when no rows exists.
		int GetCurRow(){ if (this==0) return -1; return m_iPos; };

		// Gets the column name at m_iCol index.
		// Returns null if the index is out of bounds.
		LPCTSTR GetColName( int iCol );

		void ResetRow(){ m_iPos = -1; };

		// Sets the 'iterator' to the first row
		// returns false if fails (no records)
		bool GoFirst();

		// Sets the 'iterator' to the last row
		// returns false if fails (no records)
		bool GoLast();

		// Sets the 'iterator' to the next row
		// returns false if fails (reached end)
		bool GoNext();

		// Sets the 'iterator' to the previous row
		// returns false if fails (reached beginning)
		bool GoPrev();

		// Sets the 'iterator' to the [iRow] row
		// returns false if fails (out of bounds)
		bool GoRow(unsigned int iRow);

		// Gets the value of lpColName column, in the current row
		// returns null if fails (no records)
		LPCTSTR GetValue(LPCTSTR lpColName);

		// Gets the value of iColIndex column, in the current row
		// returns null if fails (no records)
		LPCTSTR GetValue(int iColIndex);

		// Gets the value of lpColName column, in the current row
		// returns null if fails (no records)
		LPCTSTR operator [] (LPCTSTR lpColName);

		// Gets the value of iColIndex column, in the current row
		// returns null if fails (no records)
		LPCTSTR operator [] (int iColIndex);

		// Adds the rows from another table to this table
		// It is the caller's reponsibility to make sure
		// The two tables are matching
		void JoinTable(Table & tblJoin);

	private:
		int m_iRows, m_iCols;

		row m_strlstCols;
		std::vector<row> m_lstRows;

		int m_iPos;
	};

	// A class to contain a pointer to a Table class,
	// Which will spare the user from freeing a pointer.
	class TablePtr
	{
	public:
		// Default constructor
		TablePtr( );

		// Construct from a Table*
		TablePtr( Table * pTable );

		// Copy constructor.
		// Will prevent the original TablePtr from deleting the table.
		// If you have a previous table connected to this class,
		//   you do not have to worry, 
		//   it will commit suicide before eating the new table.
		TablePtr( const TablePtr& cTablePtr );

		// Destructor...
		virtual ~TablePtr();

		// Copy operator.
		// Will prevent the original TablePtr from deleting the table.
		// If you have a previous table connected to this class,
		//   you do not have to worry, 
		//   it will commit suicide before eating the new table.
		void operator =( const TablePtr& cTablePtr );

		// Functor operator, will de-reference the m_pTable member.
		// WARNING: Use with care! Check for non-null m_pTable first!
		Table& operator()(){ return *m_pTable; };

		// bool operator, to check if m_pTable is valid.
		operator bool(){ return m_pTable!=0; };

		// Detaches the class from the Table,
		// and returns the Table that were just detached...
		Table * Detach();

		// Frees the current Table, and attaches the pTable.
		void Attach( Table * pTable );

		// Frees the current Table.
		void Destroy();

		// Pointer to the table.
		// I do not see any reason for encapsulating in Get/Set functions.
		Table * m_pTable;
	};

	// Class for converting TCHAR to Multi-Byte UTF-8
	//   and vice versa
	class UTF8MBSTR
	{
	public:
		UTF8MBSTR(void);
		UTF8MBSTR( LPCTSTR lpStr );
		UTF8MBSTR( UTF8MBSTR& lpStr );
		virtual ~UTF8MBSTR();

		void operator =( LPCTSTR lpStr );
		void operator =( UTF8MBSTR& lpStr );
		operator char* ();
		operator stdstring ();

	private:
		char * m_strUTF8_MultiByte;
		size_t ConvertStringToUTF8( LPCTSTR strIn, char *& strOutUTF8MB );
		static void ConvertUTF8ToString( char * strInUTF8MB, size_t len, LPTSTR & strOut );

		size_t m_iLen;
	};
}
