   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*                  USER DATA MODULE                   */
   /*******************************************************/

/*************************************************************/
/* Purpose: Routines for attaching user data to constructs,  */
/*   facts, instances, user functions, etc.                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#define _USERDATA_SOURCE_

#include <stdlib.h>

#include "setup.h"

#include "envrnmnt.h"

#include "userdata.h"

/*************************************************/
/* InitializeUserDataData: Allocates environment */
/*    data for user data routines.               */
/*************************************************/
globle void InitializeUserDataData(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,USER_DATA_DATA,sizeof(struct userDataData),NULL);
  }

/******************************************************/
/* InstallUserDataRecord: Installs a user data record */
/*   in the user data record array and returns the    */
/*   integer data ID associated with the record.      */
/******************************************************/
globle unsigned char InstallUserDataRecord(
  void *theEnv,
  struct userDataRecord *theRecord)
  {
   theRecord->dataID = UserDataData(theEnv)->UserDataRecordCount;
   UserDataData(theEnv)->UserDataRecordArray[UserDataData(theEnv)->UserDataRecordCount] = theRecord;
   return(UserDataData(theEnv)->UserDataRecordCount++);
  }
  
/*****************************************************/
/* FetchUserData: Searches for user data information */
/*   from a list of user data structures. A new user */
/*   data structure is created if one is not found.  */
/*****************************************************/
globle struct userData *FetchUserData(
  void *theEnv,
  unsigned char userDataID,
  struct userData **theList)
  {
   struct userData *theData;

   for (theData = *theList;
        theData != NULL;
        theData = theData->next)
     {
      if (theData->dataID == userDataID)
        { return(theData); }
     }
     
   theData = (struct userData *) (*UserDataData(theEnv)->UserDataRecordArray[userDataID]->createUserData)(theEnv);
   theData->dataID = userDataID;
   theData->next = *theList;
   *theList = theData;
   
   return(theData);   
  }

/*****************************************************/
/* TestUserData: Searches for user data information  */
/*   from a list of user data structures. NULL is    */
/*   returned if the appropriate user data structure */
/*   is not found.                                   */
/*****************************************************/
globle struct userData *TestUserData(
  unsigned char userDataID,
  struct userData *theList)
  {
   struct userData *theData;
   
   for (theData = theList;
        theData != NULL;
        theData = theData->next)
     {
      if (theData->dataID == userDataID)
        { return(theData); }
     }
        
   return(NULL);   
  }

/***************************************************************/
/* ClearUserDataList: Deallocates a linked list of user data.  */
/***************************************************************/
globle void ClearUserDataList(
  void *theEnv,
  struct userData *theList)
  {
   struct userData *nextData;
   
   while (theList != NULL)
     {
      nextData = theList->next;
      (*UserDataData(theEnv)->UserDataRecordArray[theList->dataID]->deleteUserData)(theEnv,theList);
      theList = nextData;
     }
  }
  
/*************************************************/
/* DeleteUserData: Removes user data information */
/*   from a list of user data structures.        */
/*************************************************/
globle struct userData *DeleteUserData(
  void *theEnv,
  unsigned char userDataID,
  struct userData *theList)
  {
   struct userData *theData, *lastData = NULL;
   
   for (theData = theList;
        theData != NULL;
        theData = theData->next)
     {
      if (theData->dataID == userDataID)
        { 
         if (lastData == NULL)
           { theList = theData->next; }
         else
           { lastData->next = theData->next; }
            
         (*UserDataData(theEnv)->UserDataRecordArray[userDataID]->deleteUserData)(theEnv,theData);
         return(theList);
        }
        
      lastData = theData;
     }
        
   return(theList);   
  }

