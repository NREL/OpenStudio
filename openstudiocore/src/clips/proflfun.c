   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/02/06            */
   /*                                                     */
   /*         CONSTRUCT PROFILING FUNCTIONS MODULE        */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for profiling the amount of    */
/*   time spent in constructs and user defined functions.    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Modified OutputProfileInfo to allow a before   */
/*            and after prefix so that a string buffer does  */
/*            not need to be created to contain the entire   */
/*            prefix. This allows a buffer overflow problem  */
/*            to be corrected. DR0857.                       */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Added pragmas to remove compilation warnings.  */
/*                                                           */
/*            Corrected code to remove run-time program      */
/*            compiler warnings.                             */
/*                                                           */
/*************************************************************/

#define _PROFLFUN_SOURCE_

#include "setup.h"

#if PROFILING_FUNCTIONS

#include "argacces.h"
#include "classcom.h"
#include "dffnxfun.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "genrccom.h"
#include "genrcfun.h"
#include "memalloc.h"
#include "msgcom.h"
#include "router.h"
#include "sysdep.h"

#include "proflfun.h"

#include <string.h>

#define NO_PROFILE      0
#define USER_FUNCTIONS  1
#define CONSTRUCTS_CODE 2

#define OUTPUT_STRING "%-40s %7ld %15.6f  %8.2f%%  %15.6f  %8.2f%%\n"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static intBool                     OutputProfileInfo(void *,char *,struct constructProfileInfo *,
                                                        char *,char *,char *,char **);
   static void                        OutputUserFunctionsInfo(void *);
   static void                        OutputConstructsCodeInfo(void *);
#if (! RUN_TIME)
   static void                        ProfileClearFunction(void *);
#endif

/******************************************************/
/* ConstructProfilingFunctionDefinitions: Initializes */
/*   the construct profiling functions.               */
/******************************************************/
globle void ConstructProfilingFunctionDefinitions(
  void *theEnv)
  {
   struct userDataRecord profileDataInfo = { 0, CreateProfileData, DeleteProfileData };

   AllocateEnvironmentData(theEnv,PROFLFUN_DATA,sizeof(struct profileFunctionData),NULL);

   memcpy(&ProfileFunctionData(theEnv)->ProfileDataInfo,&profileDataInfo,sizeof(struct userDataRecord));   
   
   ProfileFunctionData(theEnv)->LastProfileInfo = NO_PROFILE;
   ProfileFunctionData(theEnv)->PercentThreshold = 0.0;
   ProfileFunctionData(theEnv)->OutputString = OUTPUT_STRING;

#if ! RUN_TIME
   EnvDefineFunction2(theEnv,"profile",'v', PTIEF ProfileCommand,"ProfileCommand","11w");
   EnvDefineFunction2(theEnv,"profile-info",'v', PTIEF ProfileInfoCommand,"ProfileInfoCommand","01w");
   EnvDefineFunction2(theEnv,"profile-reset",'v', PTIEF ProfileResetCommand,"ProfileResetCommand","00");

   EnvDefineFunction2(theEnv,"set-profile-percent-threshold",'d',
                   PTIEF SetProfilePercentThresholdCommand,
                   "SetProfilePercentThresholdCommand","11n");
   EnvDefineFunction2(theEnv,"get-profile-percent-threshold",'d',
                   PTIEF GetProfilePercentThresholdCommand,
                   "GetProfilePercentThresholdCommand","00");
                   
   ProfileFunctionData(theEnv)->ProfileDataID = InstallUserDataRecord(theEnv,&ProfileFunctionData(theEnv)->ProfileDataInfo);
   
   EnvAddClearFunction(theEnv,"profile",ProfileClearFunction,0);
#endif
  }

/**********************************/
/* CreateProfileData: Allocates a */
/*   profile user data structure. */
/**********************************/
globle void *CreateProfileData(
  void *theEnv)
  {
   struct constructProfileInfo *theInfo;
   
   theInfo = (struct constructProfileInfo *)
             genalloc(theEnv,sizeof(struct constructProfileInfo));

   theInfo->numberOfEntries = 0;
   theInfo->childCall = FALSE;
   theInfo->startTime = 0.0;
   theInfo->totalSelfTime = 0.0;
   theInfo->totalWithChildrenTime = 0.0;
   
   return(theInfo);
  }
  
/**************************************/
/* DeleteProfileData:          */
/**************************************/
globle void DeleteProfileData(
  void *theEnv,
  void *theData)
  {
   genfree(theEnv,theData,sizeof(struct constructProfileInfo));
  }

/**************************************/
/* ProfileCommand: H/L access routine */
/*   for the profile command.         */
/**************************************/
globle void ProfileCommand(
  void *theEnv)
  {
   char *argument;
   DATA_OBJECT theValue;

   if (EnvArgCountCheck(theEnv,"profile",EXACTLY,1) == -1) return;
   if (EnvArgTypeCheck(theEnv,"profile",1,SYMBOL,&theValue) == FALSE) return;

   argument = DOToString(theValue);

   if (! Profile(theEnv,argument))
     {
      ExpectedTypeError1(theEnv,"profile",1,"symbol with value constructs, user-functions, or off");
      return;
     }

   return;
  }

/******************************/
/* Profile: C access routine  */
/*   for the profile command. */
/******************************/
globle intBool Profile(
  void *theEnv,
  char *argument)
  {
   /*======================================================*/
   /* If the argument is the symbol "user-functions", then */
   /* user-defined functions should be profiled. If the    */
   /* argument is the symbol "constructs", then            */
   /* deffunctions, generic functions, message-handlers,   */
   /* and rule RHS actions are profiled.                   */
   /*======================================================*/

   if (strcmp(argument,"user-functions") == 0)
     {
      ProfileFunctionData(theEnv)->ProfileStartTime = gentime();
      ProfileFunctionData(theEnv)->ProfileUserFunctions = TRUE;
      ProfileFunctionData(theEnv)->ProfileConstructs = FALSE;
      ProfileFunctionData(theEnv)->LastProfileInfo = USER_FUNCTIONS;
     }

   else if (strcmp(argument,"constructs") == 0)
     {
      ProfileFunctionData(theEnv)->ProfileStartTime = gentime();
      ProfileFunctionData(theEnv)->ProfileUserFunctions = FALSE;
      ProfileFunctionData(theEnv)->ProfileConstructs = TRUE;
      ProfileFunctionData(theEnv)->LastProfileInfo = CONSTRUCTS_CODE;
     }

   /*======================================================*/
   /* Otherwise, if the argument is the symbol "off", then */
   /* don't profile constructs and user-defined functions. */
   /*======================================================*/

   else if (strcmp(argument,"off") == 0)
     {
      ProfileFunctionData(theEnv)->ProfileEndTime = gentime();
      ProfileFunctionData(theEnv)->ProfileTotalTime += (ProfileFunctionData(theEnv)->ProfileEndTime - ProfileFunctionData(theEnv)->ProfileStartTime);
      ProfileFunctionData(theEnv)->ProfileUserFunctions = FALSE;
      ProfileFunctionData(theEnv)->ProfileConstructs = FALSE;
     }

   /*=====================================================*/
   /* Otherwise, generate an error since the only allowed */
   /* arguments are "on" or "off."                        */
   /*=====================================================*/

   else
     { return(FALSE); }

   return(TRUE);
  }

/******************************************/
/* ProfileInfoCommand: H/L access routine */
/*   for the profile-info command.        */
/******************************************/
globle void ProfileInfoCommand(
  void *theEnv)
  {
   int argCount;
   DATA_OBJECT theValue;
   char buffer[512];
   
   /*===================================*/
   /* The profile-info command expects  */
   /* at most a single symbol argument. */
   /*===================================*/

   if ((argCount = EnvArgCountCheck(theEnv,"profile",NO_MORE_THAN,1)) == -1) return;

   /*===========================================*/
   /* The first profile-info argument indicates */
   /* the field on which sorting is performed.  */
   /*===========================================*/

   if (argCount == 1)
     {
      if (EnvArgTypeCheck(theEnv,"profile",1,SYMBOL,&theValue) == FALSE) return;
     }

   /*==================================*/
   /* If code is still being profiled, */
   /* update the profile end time.     */
   /*==================================*/

   if (ProfileFunctionData(theEnv)->ProfileUserFunctions || ProfileFunctionData(theEnv)->ProfileConstructs)
     {
      ProfileFunctionData(theEnv)->ProfileEndTime = gentime();
      ProfileFunctionData(theEnv)->ProfileTotalTime += (ProfileFunctionData(theEnv)->ProfileEndTime - ProfileFunctionData(theEnv)->ProfileStartTime);
     }
      
   /*==================================*/
   /* Print the profiling information. */
   /*==================================*/
      
   if (ProfileFunctionData(theEnv)->LastProfileInfo != NO_PROFILE)
     {
      gensprintf(buffer,"Profile elapsed time = %g seconds\n",
                      ProfileFunctionData(theEnv)->ProfileTotalTime);
      EnvPrintRouter(theEnv,WDISPLAY,buffer);

      if (ProfileFunctionData(theEnv)->LastProfileInfo == USER_FUNCTIONS)
        { EnvPrintRouter(theEnv,WDISPLAY,"Function Name                            "); }
      else if (ProfileFunctionData(theEnv)->LastProfileInfo == CONSTRUCTS_CODE)
        { EnvPrintRouter(theEnv,WDISPLAY,"Construct Name                           "); }            
      
      EnvPrintRouter(theEnv,WDISPLAY,"Entries         Time           %          Time+Kids     %+Kids\n");

      if (ProfileFunctionData(theEnv)->LastProfileInfo == USER_FUNCTIONS)
        { EnvPrintRouter(theEnv,WDISPLAY,"-------------                            "); }
      else if (ProfileFunctionData(theEnv)->LastProfileInfo == CONSTRUCTS_CODE)
        { EnvPrintRouter(theEnv,WDISPLAY,"--------------                           "); }

      EnvPrintRouter(theEnv,WDISPLAY,"-------        ------        -----        ---------     ------\n");
     }

   if (ProfileFunctionData(theEnv)->LastProfileInfo == USER_FUNCTIONS) OutputUserFunctionsInfo(theEnv);
   if (ProfileFunctionData(theEnv)->LastProfileInfo == CONSTRUCTS_CODE) OutputConstructsCodeInfo(theEnv);
  }

/**********************************************/
/* StartProfile: Initiates bookkeeping needed */
/*   to profile a construct or function.      */
/**********************************************/
globle void StartProfile(
  void *theEnv,
  struct profileFrameInfo *theFrame,
  struct userData **theList,
  intBool checkFlag)
  {
   double startTime, addTime;
   struct constructProfileInfo *profileInfo;

   if (! checkFlag)
     {
      theFrame->profileOnExit = FALSE;
      return;
     }

   profileInfo = (struct constructProfileInfo *) FetchUserData(theEnv,ProfileFunctionData(theEnv)->ProfileDataID,theList);
                
   theFrame->profileOnExit = TRUE;
   theFrame->parentCall = FALSE;

   startTime = gentime();
   theFrame->oldProfileFrame = ProfileFunctionData(theEnv)->ActiveProfileFrame;

   if (ProfileFunctionData(theEnv)->ActiveProfileFrame != NULL)
     {
      addTime = startTime - ProfileFunctionData(theEnv)->ActiveProfileFrame->startTime;
      ProfileFunctionData(theEnv)->ActiveProfileFrame->totalSelfTime += addTime;
     }

   ProfileFunctionData(theEnv)->ActiveProfileFrame = profileInfo;

   ProfileFunctionData(theEnv)->ActiveProfileFrame->numberOfEntries++;
   ProfileFunctionData(theEnv)->ActiveProfileFrame->startTime = startTime;

   if (! ProfileFunctionData(theEnv)->ActiveProfileFrame->childCall)
     {
      theFrame->parentCall = TRUE;
      theFrame->parentStartTime = startTime;
      ProfileFunctionData(theEnv)->ActiveProfileFrame->childCall = TRUE;
     }
  }

/*******************************************/
/* EndProfile: Finishes bookkeeping needed */
/*   to profile a construct or function.   */
/*******************************************/
globle void EndProfile(
  void *theEnv,
  struct profileFrameInfo *theFrame)
  {
   double endTime, addTime;

   if (! theFrame->profileOnExit) return;

   endTime = gentime();

   if (theFrame->parentCall)
     {
      addTime = endTime - theFrame->parentStartTime;
      ProfileFunctionData(theEnv)->ActiveProfileFrame->totalWithChildrenTime += addTime;
      ProfileFunctionData(theEnv)->ActiveProfileFrame->childCall = FALSE;
     }

   ProfileFunctionData(theEnv)->ActiveProfileFrame->totalSelfTime += (endTime - ProfileFunctionData(theEnv)->ActiveProfileFrame->startTime);

   if (theFrame->oldProfileFrame != NULL)
     { theFrame->oldProfileFrame->startTime = endTime; }

   ProfileFunctionData(theEnv)->ActiveProfileFrame = theFrame->oldProfileFrame;
  }

/******************************************/
/* OutputProfileInfo: Prints out a single */
/*   line of profile information.         */
/******************************************/
static intBool OutputProfileInfo(
  void *theEnv,
  char *itemName,
  struct constructProfileInfo *profileInfo,
  char *printPrefixBefore,
  char *printPrefix,
  char *printPrefixAfter,
  char **banner)
  {
   double percent = 0.0, percentWithKids = 0.0;
   char buffer[512];
   
   if (profileInfo == NULL) return(FALSE);
   
   if (profileInfo->numberOfEntries == 0) return(FALSE);

   if (ProfileFunctionData(theEnv)->ProfileTotalTime != 0.0)
     {
      percent = (profileInfo->totalSelfTime * 100.0) / ProfileFunctionData(theEnv)->ProfileTotalTime;
      if (percent < 0.005) percent = 0.0;
      percentWithKids = (profileInfo->totalWithChildrenTime * 100.0) / ProfileFunctionData(theEnv)->ProfileTotalTime;
      if (percentWithKids < 0.005) percentWithKids = 0.0;
     }

   if (percent < ProfileFunctionData(theEnv)->PercentThreshold) return(FALSE);

   if ((banner != NULL) && (*banner != NULL))
     {
      EnvPrintRouter(theEnv,WDISPLAY,*banner);
      *banner = NULL;
     }

   if (printPrefixBefore != NULL)
     { EnvPrintRouter(theEnv,WDISPLAY,printPrefixBefore); }
   
   if (printPrefix != NULL)
     { EnvPrintRouter(theEnv,WDISPLAY,printPrefix); }

   if (printPrefixAfter != NULL)
     { EnvPrintRouter(theEnv,WDISPLAY,printPrefixAfter); }

   if (strlen(itemName) >= 40)
     {
      EnvPrintRouter(theEnv,WDISPLAY,itemName);
      EnvPrintRouter(theEnv,WDISPLAY,"\n");
      itemName = "";
     }

   gensprintf(buffer,ProfileFunctionData(theEnv)->OutputString,
                        itemName,
                        (long) profileInfo->numberOfEntries,

                        (double) profileInfo->totalSelfTime,
                        (double) percent,

                        (double) profileInfo->totalWithChildrenTime,
                        (double) percentWithKids);
   EnvPrintRouter(theEnv,WDISPLAY,buffer);

   return(TRUE);
  }

/*******************************************/
/* ProfileResetCommand: H/L access routine */
/*   for the profile-reset command.        */
/*******************************************/
globle void ProfileResetCommand(
  void *theEnv)
  {
   struct FunctionDefinition *theFunction;
   int i;
#if DEFFUNCTION_CONSTRUCT
   DEFFUNCTION *theDeffunction;
#endif
#if DEFRULE_CONSTRUCT
   struct defrule *theDefrule;
#endif
#if DEFGENERIC_CONSTRUCT
   DEFGENERIC *theDefgeneric;
   unsigned int methodIndex;
   DEFMETHOD *theMethod;
#endif
#if OBJECT_SYSTEM
   DEFCLASS *theDefclass;
   HANDLER *theHandler;
   unsigned handlerIndex;
#endif
   
   ProfileFunctionData(theEnv)->ProfileStartTime = 0.0;
   ProfileFunctionData(theEnv)->ProfileEndTime = 0.0;
   ProfileFunctionData(theEnv)->ProfileTotalTime = 0.0;
   ProfileFunctionData(theEnv)->LastProfileInfo = NO_PROFILE;

   for (theFunction = GetFunctionList(theEnv);
        theFunction != NULL;
        theFunction = theFunction->next)
     { 
      ResetProfileInfo((struct constructProfileInfo *)
                       TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theFunction->usrData));
     }

   for (i = 0; i < MAXIMUM_PRIMITIVES; i++)
     {
      if (EvaluationData(theEnv)->PrimitivesArray[i] != NULL)
        {  
         ResetProfileInfo((struct constructProfileInfo *)
                          TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,EvaluationData(theEnv)->PrimitivesArray[i]->usrData));
        }
     }

#if DEFFUNCTION_CONSTRUCT
   for (theDeffunction = (DEFFUNCTION *) EnvGetNextDeffunction(theEnv,NULL);
        theDeffunction != NULL;
        theDeffunction = (DEFFUNCTION *) EnvGetNextDeffunction(theEnv,theDeffunction))
     { 
      ResetProfileInfo((struct constructProfileInfo *)
                       TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theDeffunction->header.usrData)); 
     }
#endif

#if DEFRULE_CONSTRUCT
   for (theDefrule = (struct defrule *) EnvGetNextDefrule(theEnv,NULL);
        theDefrule != NULL;
        theDefrule = (struct defrule *) EnvGetNextDefrule(theEnv,theDefrule))
     { 
      ResetProfileInfo((struct constructProfileInfo *)
                       TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theDefrule->header.usrData)); 
     }
#endif

#if DEFGENERIC_CONSTRUCT
   for (theDefgeneric = (DEFGENERIC *) EnvGetNextDefgeneric(theEnv,NULL);
        theDefgeneric != NULL;
        theDefgeneric = (DEFGENERIC *) EnvGetNextDefgeneric(theEnv,theDefgeneric))
     {
      ResetProfileInfo((struct constructProfileInfo *)
                       TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theDefgeneric->header.usrData)); 
      
      for (methodIndex = EnvGetNextDefmethod(theEnv,theDefgeneric,0);
           methodIndex != 0;
           methodIndex = EnvGetNextDefmethod(theEnv,theDefgeneric,methodIndex))
        {
         theMethod = GetDefmethodPointer(theDefgeneric,methodIndex);
         ResetProfileInfo((struct constructProfileInfo *)
                          TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theMethod->usrData)); 
        }
     }
#endif

#if OBJECT_SYSTEM
   for (theDefclass = (DEFCLASS *) EnvGetNextDefclass(theEnv,NULL);
        theDefclass != NULL;
        theDefclass = (DEFCLASS *) EnvGetNextDefclass(theEnv,theDefclass))
     {
      ResetProfileInfo((struct constructProfileInfo *)
                       TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theDefclass->header.usrData)); 
      for (handlerIndex = EnvGetNextDefmessageHandler(theEnv,theDefclass,0);
           handlerIndex != 0;
           handlerIndex = EnvGetNextDefmessageHandler(theEnv,theDefclass,handlerIndex))
        {
         theHandler = GetDefmessageHandlerPointer(theDefclass,handlerIndex);
         ResetProfileInfo((struct constructProfileInfo *)
                          TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theHandler->usrData)); 
        }
     }
#endif

  }

/*************************************************/
/* ResetProfileInfo: Sets the initial values for */
/*   a constructProfileInfo data structure.      */
/*************************************************/
globle void ResetProfileInfo(
  struct constructProfileInfo *profileInfo)
  {
   if (profileInfo == NULL) return;
   
   profileInfo->numberOfEntries = 0;
   profileInfo->childCall = FALSE;
   profileInfo->startTime = 0.0;
   profileInfo->totalSelfTime = 0.0;
   profileInfo->totalWithChildrenTime = 0.0;
  }

/*************************************************/
/* OutputUserFunctionsInfo:       */
/*************************************************/
static void OutputUserFunctionsInfo(
  void *theEnv)
  {
   struct FunctionDefinition *theFunction;
   int i;

   for (theFunction = GetFunctionList(theEnv);
        theFunction != NULL;
        theFunction = theFunction->next)
     {
      OutputProfileInfo(theEnv,ValueToString(theFunction->callFunctionName),
                        (struct constructProfileInfo *) 
                           TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,
                        theFunction->usrData),
                        NULL,NULL,NULL,NULL);
     }

   for (i = 0; i < MAXIMUM_PRIMITIVES; i++)
     {
      if (EvaluationData(theEnv)->PrimitivesArray[i] != NULL)
        {
         OutputProfileInfo(theEnv,EvaluationData(theEnv)->PrimitivesArray[i]->name,
                           (struct constructProfileInfo *)
                              TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,
                           EvaluationData(theEnv)->PrimitivesArray[i]->usrData),
                           NULL,NULL,NULL,NULL);
        }
     }
  }

/*************************************************/
/* OutputConstructsCodeInfo:       */
/*************************************************/
#if WIN_BTC && (! DEFFUNCTION_CONSTRUCT) && (! DEFGENERIC_CONSTRUCT) && (! OBJECT_SYSTEM) && (! DEFRULE_CONSTRUCT)
#pragma argsused
#endif
static void OutputConstructsCodeInfo(
  void *theEnv)
  {
#if (! DEFFUNCTION_CONSTRUCT) && (! DEFGENERIC_CONSTRUCT) && (! OBJECT_SYSTEM) && (! DEFRULE_CONSTRUCT)
#pragma unused(theEnv)
#endif
#if DEFFUNCTION_CONSTRUCT
   DEFFUNCTION *theDeffunction;
#endif
#if DEFRULE_CONSTRUCT
   struct defrule *theDefrule;
#endif
#if DEFGENERIC_CONSTRUCT
   DEFGENERIC *theDefgeneric;
   DEFMETHOD *theMethod;
   unsigned methodIndex;
   char methodBuffer[512];
#endif
#if OBJECT_SYSTEM
   DEFCLASS *theDefclass;
   HANDLER *theHandler;
   unsigned handlerIndex;
#endif
#if DEFGENERIC_CONSTRUCT || OBJECT_SYSTEM
   char *prefix, *prefixBefore, *prefixAfter;
#endif
   char *banner;

   banner = "\n*** Deffunctions ***\n\n";

#if DEFFUNCTION_CONSTRUCT
   for (theDeffunction = (DEFFUNCTION *) EnvGetNextDeffunction(theEnv,NULL);
        theDeffunction != NULL;
        theDeffunction = (DEFFUNCTION *) EnvGetNextDeffunction(theEnv,theDeffunction))
     {
      OutputProfileInfo(theEnv,EnvGetDeffunctionName(theEnv,theDeffunction),
                        (struct constructProfileInfo *) 
                          TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theDeffunction->header.usrData),
                        NULL,NULL,NULL,&banner);
     }
#endif

   banner = "\n*** Defgenerics ***\n";
#if DEFGENERIC_CONSTRUCT
   for (theDefgeneric = (DEFGENERIC *) EnvGetNextDefgeneric(theEnv,NULL);
        theDefgeneric != NULL;
        theDefgeneric = (DEFGENERIC *) EnvGetNextDefgeneric(theEnv,theDefgeneric))
     {
      prefixBefore = "\n";
      prefix = EnvGetDefgenericName(theEnv,theDefgeneric);
      prefixAfter = "\n";

      for (methodIndex = EnvGetNextDefmethod(theEnv,theDefgeneric,0);
           methodIndex != 0;
           methodIndex = EnvGetNextDefmethod(theEnv,theDefgeneric,methodIndex))
        {
         theMethod = GetDefmethodPointer(theDefgeneric,methodIndex);

         EnvGetDefmethodDescription(theEnv,methodBuffer,510,theDefgeneric,methodIndex);
         if (OutputProfileInfo(theEnv,methodBuffer,
                               (struct constructProfileInfo *) 
                                  TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theMethod->usrData),
                               prefixBefore,prefix,prefixAfter,&banner))
           {
            prefixBefore = NULL; 
            prefix = NULL; 
            prefixAfter = NULL;
           }
        }
     }
#endif

   banner = "\n*** Defclasses ***\n";
#if OBJECT_SYSTEM
   for (theDefclass = (DEFCLASS *) EnvGetNextDefclass(theEnv,NULL);
        theDefclass != NULL;
        theDefclass = (DEFCLASS *) EnvGetNextDefclass(theEnv,theDefclass))
     {
      prefixAfter = "\n";
      prefix = EnvGetDefclassName(theEnv,theDefclass);
      prefixBefore = "\n";
      
      for (handlerIndex = EnvGetNextDefmessageHandler(theEnv,theDefclass,0);
           handlerIndex != 0;
           handlerIndex = EnvGetNextDefmessageHandler(theEnv,theDefclass,handlerIndex))
        {
         theHandler = GetDefmessageHandlerPointer(theDefclass,handlerIndex);
         if (OutputProfileInfo(theEnv,EnvGetDefmessageHandlerName(theEnv,theDefclass,handlerIndex),
                               (struct constructProfileInfo *) 
                                  TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,
                               theHandler->usrData),
                               prefixBefore,prefix,prefixAfter,&banner))
           {
            prefixBefore = NULL; 
            prefix = NULL; 
            prefixAfter = NULL;
           }
        }

     }
#endif

   banner = "\n*** Defrules ***\n\n";

#if DEFRULE_CONSTRUCT
   for (theDefrule = (struct defrule *) EnvGetNextDefrule(theEnv,NULL);
        theDefrule != NULL;
        theDefrule = (struct defrule *) EnvGetNextDefrule(theEnv,theDefrule))
     {
      OutputProfileInfo(theEnv,EnvGetDefruleName(theEnv,theDefrule),
                        (struct constructProfileInfo *) 
                          TestUserData(ProfileFunctionData(theEnv)->ProfileDataID,theDefrule->header.usrData),
                        NULL,NULL,NULL,&banner);
     }
#endif

  }

/*********************************************************/
/* SetProfilePercentThresholdCommand: H/L access routine */
/*   for the set-profile-percent-threshold command.      */
/*********************************************************/
globle double SetProfilePercentThresholdCommand(
  void *theEnv)
  {
   DATA_OBJECT theValue;
   double newThreshold;
   
   if (EnvArgCountCheck(theEnv,"set-profile-percent-threshold",EXACTLY,1) == -1)
     { return(ProfileFunctionData(theEnv)->PercentThreshold); }

   if (EnvArgTypeCheck(theEnv,"set-profile-percent-threshold",1,INTEGER_OR_FLOAT,&theValue) == FALSE)
      { return(ProfileFunctionData(theEnv)->PercentThreshold); }

   if (GetType(theValue) == INTEGER)
     { newThreshold = (double) DOToLong(theValue); }
   else
     { newThreshold = (double) DOToDouble(theValue); }
     
   if ((newThreshold < 0.0) || (newThreshold > 100.0))
     { 
      ExpectedTypeError1(theEnv,"set-profile-percent-threshold",1,
                         "number in the range 0 to 100");
      return(-1.0); 
     }

   return(SetProfilePercentThreshold(theEnv,newThreshold));
  }

/****************************************************/
/* SetProfilePercentThreshold: C access routine for */
/*   the set-profile-percent-threshold command.     */
/****************************************************/
globle double SetProfilePercentThreshold(
  void *theEnv,
  double value)
  {
   double oldPercentThreshhold;

   if ((value < 0.0) || (value > 100.0))
     { return(-1.0); }
     
   oldPercentThreshhold = ProfileFunctionData(theEnv)->PercentThreshold;

   ProfileFunctionData(theEnv)->PercentThreshold = value;

   return(oldPercentThreshhold);
  }

/*********************************************************/
/* GetProfilePercentThresholdCommand: H/L access routine */
/*   for the get-profile-percent-threshold command.      */
/*********************************************************/
globle double GetProfilePercentThresholdCommand(
  void *theEnv)
  {   
   EnvArgCountCheck(theEnv,"get-profile-percent-threshold",EXACTLY,0);

   return(ProfileFunctionData(theEnv)->PercentThreshold);
  }

/****************************************************/
/* GetProfilePercentThreshold: C access routine for */
/*   the get-profile-percent-threshold command.     */
/****************************************************/
globle double GetProfilePercentThreshold(
  void *theEnv)
  {
   return(ProfileFunctionData(theEnv)->PercentThreshold);
  }
  
/**********************************************************/
/* SetProfileOutputString: Sets the output string global. */
/**********************************************************/
globle char *SetProfileOutputString(
  void *theEnv,
  char *value)
  {
   char *oldOutputString;

   if (value == NULL)
     { return(ProfileFunctionData(theEnv)->OutputString); }
     
   oldOutputString = ProfileFunctionData(theEnv)->OutputString;

   ProfileFunctionData(theEnv)->OutputString = value;

   return(oldOutputString);
  }

#if (! RUN_TIME)  
/******************************************************************/
/* ProfileClearFunction: Profiling clear routine for use with the */
/*   clear command. Removes user data attached to user functions. */
/******************************************************************/
static void ProfileClearFunction(
  void *theEnv)
  {
   struct FunctionDefinition *theFunction;
   int i;

   for (theFunction = GetFunctionList(theEnv);
        theFunction != NULL;
        theFunction = theFunction->next)
     {
      theFunction->usrData = 
        DeleteUserData(theEnv,ProfileFunctionData(theEnv)->ProfileDataID,theFunction->usrData);
     }

   for (i = 0; i < MAXIMUM_PRIMITIVES; i++)
     {
      if (EvaluationData(theEnv)->PrimitivesArray[i] != NULL)
        {
         EvaluationData(theEnv)->PrimitivesArray[i]->usrData = 
           DeleteUserData(theEnv,ProfileFunctionData(theEnv)->ProfileDataID,EvaluationData(theEnv)->PrimitivesArray[i]->usrData);
        }
     }
  }
#endif

#endif /* PROFILING_FUNCTIONS */

