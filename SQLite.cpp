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

#include "StdAfx.h"
#include "SQLite.h"

namespace SQLite
{
	Database::Database(void)
	{
		m_sqlite3=0;
		m_iLastError=SQLITE_OK;
	}

	Database::~Database(void)
	{
		Close();
	}

	int Database::Open( LPCTSTR strFileName )
	{
		Close();

		int iErr=sqlite3_open(UTF8MBSTR(strFileName), &m_sqlite3);

		if (iErr!=SQLITE_OK) m_iLastError=iErr;

		return iErr;
	}

	void Database::Close()
	{
		if (m_sqlite3)
		{
			sqlite3_close(m_sqlite3);
			m_sqlite3=0;
		}
	}

	bool Database::IsOpen()
	{
		if (this==0) return false; // check for valid Database* pointer
		return m_sqlite3!=0; // check for open database
	}

	Table Database::QuerySQL( LPCTSTR strSQL )
	{
		if (!IsOpen()) {
			m_iLastError=SQLITE_ERROR;
			return Table();
		}

		// I do not need to imlpement the whole sqlite3_get_table
		//   with callbacks and all myself, because it won't spare me anything,
		// I'm converting the returned strings to TCHAR anyway, 
		//   so I'm not really copying the strings into the Table class

		char ** retStrings = 0;
		char * errmsg = 0;
		int iRows=0, iCols=0;

		int iErr=sqlite3_get_table(m_sqlite3, UTF8MBSTR(strSQL), &retStrings,
			&iRows, &iCols, &errmsg);

		if (iErr!=SQLITE_OK) m_iLastError=iErr;

#ifdef DEBUG
		if (errmsg) {
			stdvstring errstr;
			ConvertUTF8ToString(errmsg, errstr);
			TRACE(_T("QuerySQL error: %s\r\n"), &errstr[0]);
		}
#endif
		// No need for string errors, 
		//   too costly is the comparision of strings.
		//
		// BTW: sqlite3_free() already checks for null pointer,
		//		  so I do not do double checking.
		//		And I can assume this for later versions, because
		//		  that is generally the point of sqlite3_free()...
		sqlite3_free(errmsg);

		Table retTable;

		if (iRows>0) retTable.m_iPos=0; // Put retTable's cursor on first row

		retTable.m_iCols=iCols;
		retTable.m_iRows=iRows;

		// allocate the memory for the header
		retTable.m_strlstCols.reserve(iCols);

		int iPos=0;

		for (; iPos<iCols; iPos++)
		{
			// Add a string element
			retTable.m_strlstCols.push_back(stdvstring());

			// Convert the UTF8 to TCHAR string
			if (retStrings[iPos])
				ConvertUTF8ToString( retStrings[iPos], retTable.m_strlstCols.back() );
			else retTable.m_strlstCols.back().push_back(_T('\0'));
		}

		retTable.m_lstRows.resize(iRows);
		for (int iRow=0; iRow<iRows; iRow++)
		{
			retTable.m_lstRows[iRow].reserve(iCols);
			for (int iCol=0; iCol<iCols; iCol++)
			{
				// Add a string element
				retTable.m_lstRows[iRow].push_back(stdvstring());
					
				// Convert the UTF8 to TCHAR string
				if (retStrings[iPos])
					ConvertUTF8ToString( retStrings[iPos], retTable.m_lstRows[iRow].back() );
				else retTable.m_lstRows[iRow].back().push_back(_T('\0'));

				iPos++;
			}
		}

		// sqlite3_free_table() also checks for null pointer...
		sqlite3_free_table(retStrings);

		return retTable;
	}

	TablePtr Database::QuerySQL2( LPCTSTR strSQL )
	{
		if (!IsOpen()) {
			m_iLastError=SQLITE_ERROR;
			return 0;
		}

		// I do not need to imlpement the whole sqlite3_get_table
		//   with callbacks and all myself, because it won't spare me anything,
		// I'm converting the returned strings to TCHAR anyway, 
		//   so I'm not really copying the strings into the Table class

		char ** retStrings = 0;
		char * errmsg = 0;
		int iRows=0, iCols=0;

		int iErr=sqlite3_get_table(m_sqlite3, UTF8MBSTR(strSQL), &retStrings,
			&iRows, &iCols, &errmsg);

		if (iErr!=SQLITE_OK) m_iLastError=iErr;

#ifdef DEBUG
		if (errmsg) {
			stdvstring errstr;
			ConvertUTF8ToString(errmsg, errstr);
			TRACE(_T("QuerySQL2 error: %s\r\n"), &errstr[0]);
		}
#endif	
		// No need for string errors, 
		//   too costly is the comparision of strings.
		//
		// BTW: sqlite3_free() already checks for null pointer,
		//		  so I do not do double checking.
		//		And I can assume this for later versions, because
		//		  that is generally the point of sqlite3_free()...
		sqlite3_free(errmsg);

		Table * retTable = new Table();

		if (iRows>0) retTable->m_iPos=0; // Put retTable's cursor on first row

		retTable->m_iCols=iCols;
		retTable->m_iRows=iRows;

		// allocate the memory for the header
		retTable->m_strlstCols.reserve(iCols);

		int iPos=0;

		for (; iPos<iCols; iPos++)
		{
			// Add a string element
			retTable->m_strlstCols.push_back(stdvstring());

			// Convert the UTF8 to TCHAR string
			if (retStrings[iPos])
				ConvertUTF8ToString( retStrings[iPos], retTable->m_strlstCols.back() );
			else retTable->m_strlstCols.back().push_back(_T('\0'));
		}

		retTable->m_lstRows.resize(iRows);
		for (int iRow=0; iRow<iRows; iRow++)
		{
			retTable->m_lstRows[iRow].reserve(iCols);
			for (int iCol=0; iCol<iCols; iCol++)
			{
				// Add a string element
				retTable->m_lstRows[iRow].push_back(stdvstring());

				// Convert the UTF8 to TCHAR string
				if (retStrings[iPos])
					ConvertUTF8ToString( retStrings[iPos], retTable->m_lstRows[iRow].back() );
				else retTable->m_lstRows[iRow].back().push_back(_T('\0'));

				iPos++;
			}
		}

		// sqlite3_free_table() also checks for null pointer...
		sqlite3_free_table(retStrings);

		return TablePtr(retTable);
	}

	void Database::ConvertUTF8ToString( char * strInUTF8MB, stdvstring & strOut )
	{
		int len=(int)strlen(strInUTF8MB)+1;
		strOut.resize(len, 0);

#ifdef UNICODE
		MultiByteToWideChar(CP_UTF8, 0, strInUTF8MB, len, &strOut[0], len);
#else
		WCHAR * wChar=new WCHAR[len];
		wChar[0]=0;
		MultiByteToWideChar(CP_UTF8, 0, strInUTF8MB, len, wChar, len);
		WideCharToMultiByte(CP_ACP, 0, wChar, len, &strOut[0], len, 0, 0);
		delete [] wChar;
#endif
	}

	int Database::ExecuteSQL( LPCTSTR strSQL )
	{
		if (!IsOpen()) {
			m_iLastError=SQLITE_ERROR;
			return SQLITE_ERROR;
		}

		char * errmsg = 0;

		int iErr = sqlite3_exec( m_sqlite3, UTF8MBSTR(strSQL), 0, 0, &errmsg );

		if (iErr!=SQLITE_OK) m_iLastError=iErr;

#ifdef DEBUG
		if (errmsg) {
			stdvstring errstr;
			ConvertUTF8ToString(errmsg, errstr);
			TRACE(_T("ExecuteSQL error: %s\r\n"), &errstr[0]);
		}
#endif	
		// No need for string errors, 
		//   too costly is the comparision of strings.
		//
		// BTW: sqlite3_free() already checks for null pointer,
		//		  so I do not do double checking.
		//		And I can assume this for later versions, because
		//		  that is generally the point of sqlite3_free()...
		sqlite3_free(errmsg);

		return iErr;
	}

	int Database::IsSQLComplete( LPCTSTR strSQL )
	{
		return sqlite3_complete( UTF8MBSTR(strSQL) );
	}

	int Database::GetLastChangesCount()
	{
		return sqlite3_changes(m_sqlite3);
	}

	int Database::ReadBlob( LPCTSTR strSQL, BYTE ** pData, int * iLenBlob )
	{
		if (!IsOpen() || !pData) {
			m_iLastError=SQLITE_ERROR;
			return SQLITE_ERROR;
		}

		sqlite3_stmt *pStmt;

		int iErr = SQLITE_OK;

		// In case  an error occurs, set *pData and *iLenBlob to 0 now.
		*pData = 0;
		*iLenBlob = 0;

		do {
			// Compile the SELECT statement into a compiled statement.
			iErr = sqlite3_prepare(m_sqlite3, UTF8MBSTR(strSQL), -1, &pStmt, 0);
			if( iErr!=SQLITE_OK ){
				m_iLastError=iErr;
				return iErr;
			}

			// Run the compiled statement.
			// We expect only 1 record, so we are only looking for SQLITE_ROW.
			iErr = sqlite3_step(pStmt);
			if( iErr==SQLITE_ROW ){
				// The pointer returned by sqlite3_column_blob() points to memory
				//   that is owned by the statement handle (pStmt).
				// It is only good until the next call to an sqlite3_XXX()
				//   function (e.g. the sqlite3_finalize() below) that involves 
				//   the statement handle. 
				// So we need to make a copy of the blob into memory obtained from 
				//   malloc() to return to the caller.
				*iLenBlob = sqlite3_column_bytes(pStmt, 0);
				*pData = (unsigned char *)malloc(*iLenBlob);
				memcpy(*pData, sqlite3_column_blob(pStmt, 0), *iLenBlob);
			}

			// Finalize the statement ( this releases resources allocated by 
			//   sqlite3_prepare() ).
			iErr = sqlite3_finalize(pStmt);

			// If sqlite3_finalize() returned SQLITE_SCHEMA, then try to execute
			//   the statement all over again.
			// ( SQLITE_SCHEMA means that something in the structure of the
			//   table has changed, so the statement was not valid anymore )
		} while( iErr==SQLITE_SCHEMA );

		return iErr;
	}

	int Database::WriteBlob( LPCTSTR strSQL, BYTE * pData, int iLenBlob )
	{
		if (!IsOpen()) {
			m_iLastError=SQLITE_ERROR;
			return SQLITE_ERROR;
		}

		sqlite3_stmt *pStmt;

		int iErr=SQLITE_OK;

		do {
			// Compile the SQL statement into a compiled statement.
			iErr = sqlite3_prepare(m_sqlite3, UTF8MBSTR(strSQL), -1, &pStmt, 0);
			if( iErr!=SQLITE_OK ){
				m_iLastError=iErr;
				return iErr;
			}

			// Bind the pData to the compiled statement 
			//   (the ? characters in the sql statement)
			sqlite3_bind_blob(pStmt, 1, pData, iLenBlob, SQLITE_STATIC);

			// Call sqlite3_step() to run the compiled statement. 
			iErr = sqlite3_step(pStmt);
			ASSERT( iErr!=SQLITE_ROW );

			// Finalize the statement ( this releases resources allocated by 
			//   sqlite3_prepare() ).
			iErr = sqlite3_finalize(pStmt);

			// If sqlite3_finalize() returned SQLITE_SCHEMA, then try to execute
			//   the statement all over again.
			// ( SQLITE_SCHEMA means that something in the structure of the
			//   table has changed, so the statement was not valid anymore )
		} while( iErr==SQLITE_SCHEMA );

		return iErr;
	}

	sqlite_int64 Database::GetLastInsertRowID()
	{
		if (m_sqlite3==0) return 0; // RowID's starts with 1...

		return sqlite3_last_insert_rowid(m_sqlite3);
	}

	bool Database::BeginTransaction()
	{
		if (!IsOpen())
		{
			m_iLastError=SQLITE_ERROR; 
			return false;
		}
		m_iLastError = ExecuteSQL(_T("BEGIN TRANSACTION"));
		return m_iLastError==SQLITE_OK;
	}

	bool Database::CommitTransaction()
	{
		if (!IsOpen()) 
		{
			m_iLastError=SQLITE_ERROR; 
			return false;
		}
		m_iLastError = ExecuteSQL(_T("COMMIT TRANSACTION"));
		return m_iLastError==SQLITE_OK;
	}

	bool Database::RollbackTransaction()
	{
		if (!IsOpen()) 
		{
			m_iLastError=SQLITE_ERROR;
			return false;
		}
		m_iLastError = ExecuteSQL(_T("ROLLBACK TRANSACTION"));
		return m_iLastError==SQLITE_OK;
	}

	LPCTSTR Table::GetColName( int iCol )
	{
		if (iCol>=0 && iCol<m_iCols)
		{
			return &m_strlstCols[iCol][0];
		}
		return 0;
	}

	bool Table::GoFirst()
	{ 
		if (this==0) return false;
		if (m_lstRows.size()) 
		{
			m_iPos=0; 
			return true;
		}
		return false;
	}

	bool Table::GoLast()
	{ 
		if (m_lstRows.size()) 
		{
			m_iPos=(int)m_lstRows.size()-1; 
			return true;
		}
		return false;
	}

	bool Table::GoNext()
	{ 
		if (m_iPos+1<(int)m_lstRows.size()) 
		{
			m_iPos++; 
			return true;
		}
		return false;
	}

	bool Table::GoPrev()
	{
		if (m_iPos>0)
		{
			m_iPos--;
			return true;
		}
		return false;
	}

	bool Table::GoRow(unsigned int iRow)
	{
		if (this==0) return false;
		if (iRow<m_lstRows.size())
		{
			m_iPos=iRow;
			return true;
		}
		return false;
	}

	LPCTSTR Table::GetValue(LPCTSTR lpColName)
	{
		if (!lpColName) return 0;
		if (m_iPos<0) return 0;
		for (int i=0; i<m_iCols; i++)
		{
			if (!_tcsicmp(&m_strlstCols[i][0],lpColName))
			{
				return &m_lstRows[m_iPos][i][0];
			}
		}
		return 0;
	}

	LPCTSTR Table::GetValue(int iColIndex)
	{
		if (iColIndex<0 || iColIndex>=m_iCols) return 0;
		if (m_iPos<0) return 0;
		return &m_lstRows[m_iPos][iColIndex][0];
	}

	LPCTSTR Table::operator [] (LPCTSTR lpColName)
	{
		if (!lpColName) return 0;
		if (m_iPos<0) return 0;
		for (int i=0; i<m_iCols; i++)
		{
			if (!_tcsicmp(&m_strlstCols[i][0],lpColName))
			{
				return &m_lstRows[m_iPos][i][0];
			}
		}
		return 0;
	}

	LPCTSTR Table::operator [] (int iColIndex)
	{
		if (iColIndex<0 || iColIndex>=m_iCols) return 0;
		if (m_iPos<0) return 0;
		return &m_lstRows[m_iPos][iColIndex][0];
	}

	void Table::JoinTable(Table & tblJoin)
	{
		if (m_iCols==0) {
			*this=tblJoin;
			return;
		}
		if (m_iCols!=tblJoin.m_iCols) return;

		if (tblJoin.m_iRows>0)
		{
			m_iRows+=tblJoin.m_iRows;

			for (std::vector<row>::iterator it=tblJoin.m_lstRows.begin();
				it!=tblJoin.m_lstRows.end(); it++)
			{
				m_lstRows.push_back(*it);
			}
		}
	}

	TablePtr::TablePtr( )
	{ 
		m_pTable=0; 
	}

	TablePtr::TablePtr( Table * pTable )
	{
		m_pTable = pTable; 
	}

	TablePtr::TablePtr( const TablePtr& cTablePtr )
	{
		m_pTable=cTablePtr.m_pTable;

		// Copy constructors and operators, 
		// for a good reason, must take const reference,
		// but here, for a good reason, we must modify the referenced object,
		// so it won't get freed by the original.
		// I'm using a simple technique to bypass the const declaration
		((TablePtr *)&cTablePtr)->m_pTable=0;
	}

	TablePtr::~TablePtr()
	{ 
		if (m_pTable) delete m_pTable; 
	}

	void TablePtr::operator =( const TablePtr& cTablePtr )
	{
		if (m_pTable) delete m_pTable;
		m_pTable=cTablePtr.m_pTable;

		// Copy constructors and operators, 
		// for a good reason, must take const reference,
		// but here, for a good reason, we must modify the referenced object,
		// so it won't get freed by the original.
		// I'm using a simple technique to bypass the const declaration
		((TablePtr *)&cTablePtr)->m_pTable=0;
	}

	Table * TablePtr::Detach()
	{
		Table * pTable=m_pTable;
		m_pTable=0;
		return pTable;
	}

	void TablePtr::Attach( Table * pTable )
	{
		if (m_pTable) delete m_pTable;
		m_pTable=pTable;
	}

	void TablePtr::Destroy()
	{
		if (m_pTable) delete m_pTable;
		m_pTable=0;
	}

	UTF8MBSTR::UTF8MBSTR()
	{
		m_strUTF8_MultiByte=new char[1];
		m_strUTF8_MultiByte[0]=0;
		m_iLen=0;
	}

	UTF8MBSTR::UTF8MBSTR( LPCTSTR lpStr )
	{
		if (lpStr)
			m_iLen=ConvertStringToUTF8(lpStr, m_strUTF8_MultiByte);
		else {
			m_strUTF8_MultiByte=new char[1];
			m_strUTF8_MultiByte[0]=0;
			m_iLen=0;
		}
	}

	UTF8MBSTR::UTF8MBSTR( UTF8MBSTR& lpStr )
	{
		m_iLen=lpStr.m_iLen;
		m_strUTF8_MultiByte=new char[m_iLen+1];
		strncpy_s(m_strUTF8_MultiByte, m_iLen+1, lpStr.m_strUTF8_MultiByte, m_iLen+1);
	}

	UTF8MBSTR::~UTF8MBSTR()
	{
		if (m_strUTF8_MultiByte)
		{
			delete [] m_strUTF8_MultiByte;
		}
	}

	void UTF8MBSTR::operator =( LPCTSTR lpStr )
	{
		if (m_strUTF8_MultiByte)
		{
			delete [] m_strUTF8_MultiByte;
		}
		if (lpStr)
			m_iLen=ConvertStringToUTF8(lpStr, m_strUTF8_MultiByte);
		else {
			m_strUTF8_MultiByte=new char[1];
			m_strUTF8_MultiByte[0]=0;
			m_iLen=0;
		}
	}

	void UTF8MBSTR::operator =( UTF8MBSTR& lpStr )
	{
		if (m_strUTF8_MultiByte)
		{
			delete [] m_strUTF8_MultiByte;
		}
		m_iLen=lpStr.m_iLen;
		m_strUTF8_MultiByte=new char[m_iLen+1];
		strncpy_s(m_strUTF8_MultiByte, m_iLen+1, lpStr.m_strUTF8_MultiByte, m_iLen+1);
	}

	UTF8MBSTR::operator char* ()
	{
		return m_strUTF8_MultiByte;
	}

	UTF8MBSTR::operator stdstring ()
	{
		TCHAR * strRet;
		ConvertUTF8ToString(m_strUTF8_MultiByte, m_iLen+1, strRet);

		stdstring cstrRet(strRet);

		delete [] strRet;

		return cstrRet;
	}

	size_t UTF8MBSTR::ConvertStringToUTF8( LPCTSTR strIn, char *& strOutUTF8MB )
	{
		size_t len=_tcslen(strIn);

#ifdef UNICODE
		int iRequiredSize=WideCharToMultiByte(CP_UTF8, 0, strIn, (int)len+1, 0, 0, 0, 0);

		strOutUTF8MB=new char[iRequiredSize];
		strOutUTF8MB[0]=0;

		WideCharToMultiByte(CP_UTF8, 0, strIn, (int)len+1, strOutUTF8MB, iRequiredSize, 0, 0);
#else
		WCHAR * wChar=new WCHAR[len+1];
		wChar[0]=0;
		MultiByteToWideChar(CP_ACP, 0, strIn, (int)len+1, wChar, (int)len+1);
		int iRequiredSize=WideCharToMultiByte(CP_UTF8, 0, wChar, (int)len+1, 0, 0, 0, 0);
		strOutUTF8MB=new char[iRequiredSize];
		strOutUTF8MB[0]=0;
		WideCharToMultiByte(CP_UTF8, 0, wChar, (int)len+1, strOutUTF8MB, iRequiredSize, 0, 0);
		delete [] wChar;
#endif

		return len;
	}

	void UTF8MBSTR::ConvertUTF8ToString( char * strInUTF8MB, size_t len, LPTSTR & strOut )
	{
		strOut=new TCHAR[len];
		strOut[0]=0;

#ifdef UNICODE
		MultiByteToWideChar(CP_UTF8, 0, strInUTF8MB, (int)len, strOut, (int)len);
#else
		WCHAR * wChar=new WCHAR[len];
		wChar[0]=0;
		MultiByteToWideChar(CP_UTF8, 0, strInUTF8MB, (int)len, wChar, (int)len);
		WideCharToMultiByte(CP_ACP, 0, wChar, (int)len, strOut, (int)len, 0, 0);
		delete [] wChar;
#endif
	}
}