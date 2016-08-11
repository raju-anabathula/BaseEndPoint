/*
 * EncryptDecrypt.h
 * Created on	: Jun 21, 2016
 * Author	: Raju Anabathula
 * Purpose	: The main aim of this class is to encrypt the given data using the given key/password and write to the specified file. 
 * 	    	  And also decrypt the data using the same key which has been written to the file specified.
 */

#ifndef ENCRYPTDECRYPT_H_
#define ENCRYPTDECRYPT_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "DatabaseDetails.h"

using namespace std;

class EncryptDecrypt
{

public:

/**********************************************************************************************************
   	Function Name	: encrypt
   	Description  	: This method is used to encrypt the given database details and write to file.
			  Before calling this method, the file name where the database details are stored, should 
			  be assigned to fileName variable by using setFileName(<filename>).
   	Scope        	: public
   
	Parameters   	: databaseDetails - DatabaseDetails object.
		 	  key             - key to encrypt i.e., single character.

   	Return Type	: void
**************************************************************************************************************/
	void encryptDatabaseDetails(DatabaseDetails &databaseDetails);

/*************************************************************************************************************
	Function Name	: decrypt
	Description 	: This method is used to find the tag name in the encrypted file and decrypt the read tag details. 
			  Before calling this method the file name, where the database
			  details are stored, should be assigned to fileName variable by using setFileName(<filename>).
	Scope 		: public
	
   	Parameters	: tagName - The tag name used to fetch the database details.
			  key 	- The key used to decrypt the read tag details (only single character).

	Return Type	: DatabaseDetails - The database details object filled with the actual credentials
***************************************************************************************************************/
	DatabaseDetails* getDatabaseDetails(string tagName);

/*****************************************************************************************
	Function Name	: getFileName
	Description 	: This method the getter method for fileName variable
	Scope 		: public
	
	Parameters	: void
	
	Return type	: const string& - the file name in which the encrypted data is stored.
*******************************************************************************************/
	const string& getFileName() const;

/**************************************************************************************************
	Function Name	: setFileName
   	Description	: The setter method for fileName
	Scope		: public
	
	Parameters 	: const string& - new file name to be assigned

	Return type	: void
*****************************************************************************************************/
	void setFileName(const string& fileName);

	static EncryptDecrypt* getEncryptDecryptInstance();

private:

	EncryptDecrypt();
	virtual ~EncryptDecrypt();

	static EncryptDecrypt *m_instance;

/************************************************************************************************
	Variable 	: m_strFileName
	Description	: This variable holds the file name in which the encrypted data is stored.
	Scope		: private
**************************************************************************************************/
	string m_strFileName;

	map<string, DatabaseDetails*> m_tagsMap;

/*****************************************************************************************
	Function Name	: writeToFile
	Description  	: This method is used to write the encrypted data to the file specified
	Scope 		: private
   
	Parameters 	: encryptedData	- data to encrypt
			  fileName 	- The absolute path of the file name to write the encrypted data
   
	Return type	: void
*******************************************************************************************/
	void writeToFile(string encryptedData, string fileName);

/*********************************************************************************************
   	Function Name	: findTag
	Description	: This method is used to find a tag in the encrypted file
   	Scope		: private

   	Parameters	: fileName - The absolute path of the file to search for the tag
			  tagName  - Tag name to search

   	Return type	: string - returns the encrypted data prefixed with tag name
************************************************************************************************/
	void loadTags(string strFileName, char chKey, bool bIsRefresh = false);

/********************************************************************************
   	Function Name 	: encryptDecrypt
   	Description  	: This method is used for encryption and decryption of the data.
	Scope		: private
   	
	Parameters   	: data 	- data to encrypt/decrypt
			  key 	- the key/password used for encryption/decryption
   
	Return type  	: string - the resultant encrypted/decrypted string
*********************************************************************************/
	string encryptDecrypt(string data, char key);

/***************************************************************************************
   	Function Name	: split
   	Description  	: This method is used to split the given string using the pattern
	Scope        	: private
	
   	Parameters   	: stringToSplit  - string to split
		          pattern 	 - base pattern to split the string

   	Return type  	: vector<string> - returns a vector of strings separated from the given string
****************************************************************************************/
	vector<string> split(string stringToSplit, string pattern);
};

#endif /* ENCRYPTDECRYPT_H_ */
