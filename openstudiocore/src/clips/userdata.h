   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*                USER DATA HEADER FILE                */
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

#ifndef _H_userdata
#define _H_userdata

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _USERDATA_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

struct userData
  {
   unsigned char dataID;
   struct userData *next;
  };

typedef struct userData USER_DATA;
typedef struct userData * USER_DATA_PTR;
  
struct userDataRecord
  {
   unsigned char dataID;
   void *(*createUserData)(void *);
   void (*deleteUserData)(void *,void *);
  };
  
typedef struct userDataRecord USER_DATA_RECORD;
typedef struct userDataRecord * USER_DATA_RECORD_PTR;

#define MAXIMUM_USER_DATA_RECORDS 100

#define USER_DATA_DATA 56

struct userDataData
  { 
   struct userDataRecord *UserDataRecordArray[MAXIMUM_USER_DATA_RECORDS];
   unsigned char UserDataRecordCount;
  };

#define UserDataData(theEnv) ((struct userDataData *) GetEnvironmentData(theEnv,USER_DATA_DATA))

   LOCALE void                           InitializeUserDataData(void *);
   LOCALE unsigned char                  InstallUserDataRecord(void *,struct userDataRecord *);
   LOCALE struct userData               *FetchUserData(void *,unsigned char,struct userData **);
   LOCALE struct userData               *TestUserData(unsigned char,struct userData *);
   LOCALE void                           ClearUserDataList(void *,struct userData *);
   LOCALE struct userData               *DeleteUserData(void *,unsigned char,struct userData *);

#endif

