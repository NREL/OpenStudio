   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  03/05/08            */
   /*                                                     */
   /*                  I/O ROUTER MODULE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides a centralized mechanism for handling    */
/*   input and output requests.                              */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*      6.24: Removed conversion of '\r' to '\n' from the    */
/*            EnvGetcRouter function.                        */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Added support for passing context information  */ 
/*            to the router functions.                       */
/*                                                           */
/*      6.30: Fixed issues with passing context to routers.  */
/*                                                           */
/*************************************************************/

#define _ROUTER_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>
#include <string.h>

#include "setup.h"

#include "argacces.h"
#include "constant.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "filertr.h"
#include "memalloc.h"
#include "strngrtr.h"
#include "sysdep.h"

#include "router.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static int                     QueryRouter(void *,char *,struct router *);
   static void                    DeallocateRouterData(void *);

/*********************************************************/
/* InitializeDefaultRouters: Initializes output streams. */
/*********************************************************/
globle void InitializeDefaultRouters(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,ROUTER_DATA,sizeof(struct routerData),DeallocateRouterData);

   RouterData(theEnv)->CommandBufferInputCount = 0;
   RouterData(theEnv)->AwaitingInput = TRUE;
   
#if (! RUN_TIME)
   EnvDefineFunction2(theEnv,"exit",    'v', PTIEF ExitCommand,    "ExitCommand", "*1i");
#endif
   InitializeFileRouter(theEnv);
   InitializeStringRouter(theEnv);
  }
  
/*************************************************/
/* DeallocateRouterData: Deallocates environment */
/*    data for I/O routers.                      */
/*************************************************/
static void DeallocateRouterData(
  void *theEnv)
  {
   struct router *tmpPtr, *nextPtr;
   
   tmpPtr = RouterData(theEnv)->ListOfRouters;
   while (tmpPtr != NULL)
     {
      nextPtr = tmpPtr->next;
      genfree(theEnv,tmpPtr->name,strlen(tmpPtr->name) + 1);
      rtn_struct(theEnv,router,tmpPtr);
      tmpPtr = nextPtr;
     }
  }

/*******************************************/
/* EnvPrintRouter: Generic print function. */
/*******************************************/
globle int EnvPrintRouter(
  void *theEnv,
  char *logicalName,
  char *str)
  {
   struct router *currentPtr;

   /*===================================================*/
   /* If the "fast save" option is being used, then the */
   /* logical name is actually a pointer to a file and  */
   /* fprintf can be called directly to bypass querying */
   /* all of the routers.                               */
   /*===================================================*/

   if (((char *) RouterData(theEnv)->FastSaveFilePtr) == logicalName)
     {
      fprintf(RouterData(theEnv)->FastSaveFilePtr,"%s",str);
      return(2);
     }

   /*==============================================*/
   /* Search through the list of routers until one */
   /* is found that will handle the print request. */
   /*==============================================*/

   currentPtr = RouterData(theEnv)->ListOfRouters;
   while (currentPtr != NULL)
     {
      if ((currentPtr->printer != NULL) ? QueryRouter(theEnv,logicalName,currentPtr) : FALSE)
        {
         SetEnvironmentRouterContext(theEnv,currentPtr->context);
         if (currentPtr->environmentAware)
           { (*currentPtr->printer)(theEnv,logicalName,str); }
         else            
           { ((int (*)(char *,char *)) (*currentPtr->printer))(logicalName,str); }
         
         return(1);
        }
      currentPtr = currentPtr->next;
     }

   /*=====================================================*/
   /* The logical name was not recognized by any routers. */
   /*=====================================================*/

   if (strcmp(WERROR,logicalName) != 0) UnrecognizedRouterMessage(theEnv,logicalName);
   return(0);
  }

/**************************************************/
/* EnvGetcRouter: Generic get character function. */
/**************************************************/
globle int EnvGetcRouter(
  void *theEnv,
  char *logicalName)
  {
   struct router *currentPtr;
   int inchar;

   /*===================================================*/
   /* If the "fast load" option is being used, then the */
   /* logical name is actually a pointer to a file and  */
   /* getc can be called directly to bypass querying    */
   /* all of the routers.                               */
   /*===================================================*/

   if (((char *) RouterData(theEnv)->FastLoadFilePtr) == logicalName)
     {
      inchar = getc(RouterData(theEnv)->FastLoadFilePtr);

      if ((inchar == '\r') || (inchar == '\n'))
        {
         if (((char *) RouterData(theEnv)->FastLoadFilePtr) == RouterData(theEnv)->LineCountRouter)
           { IncrementLineCount(theEnv); }
        }

      /* if (inchar == '\r') return('\n'); */

      return(inchar);
     }

   /*===============================================*/
   /* If the "fast string get" option is being used */
   /* for the specified logical name, then bypass   */
   /* the router system and extract the character   */
   /* directly from the fast get string.            */
   /*===============================================*/

   if (RouterData(theEnv)->FastCharGetRouter == logicalName)
     {
      inchar = (unsigned char) RouterData(theEnv)->FastCharGetString[RouterData(theEnv)->FastCharGetIndex];

      RouterData(theEnv)->FastCharGetIndex++;

      if (inchar == '\0') return(EOF);

      if ((inchar == '\r') || (inchar == '\n'))
        {
         if (RouterData(theEnv)->FastCharGetRouter == RouterData(theEnv)->LineCountRouter)
           { IncrementLineCount(theEnv); }
        }

      return(inchar);
     }

   /*==============================================*/
   /* Search through the list of routers until one */
   /* is found that will handle the getc request.  */
   /*==============================================*/

   currentPtr = RouterData(theEnv)->ListOfRouters;
   while (currentPtr != NULL)
     {
      if ((currentPtr->charget != NULL) ? QueryRouter(theEnv,logicalName,currentPtr) : FALSE)
        {
         SetEnvironmentRouterContext(theEnv,currentPtr->context);
         if (currentPtr->environmentAware)
           { inchar = (*currentPtr->charget)(theEnv,logicalName); }
         else            
           { inchar = ((int (*)(char *)) (*currentPtr->charget))(logicalName); }

         if ((inchar == '\r') || (inchar == '\n'))
           {
            if ((RouterData(theEnv)->LineCountRouter != NULL) &&
                (strcmp(logicalName,RouterData(theEnv)->LineCountRouter) == 0))
              { IncrementLineCount(theEnv); }
           }

         return(inchar);
        }
      currentPtr = currentPtr->next;
     }

   /*=====================================================*/
   /* The logical name was not recognized by any routers. */
   /*=====================================================*/

   UnrecognizedRouterMessage(theEnv,logicalName);
   return(-1);
  }

/******************************************************/
/* EnvUngetcRouter: Generic unget character function. */
/******************************************************/
globle int EnvUngetcRouter(
  void *theEnv,
  int ch,
  char *logicalName)
  {
   struct router *currentPtr;

   /*===================================================*/
   /* If the "fast load" option is being used, then the */
   /* logical name is actually a pointer to a file and  */
   /* ungetc can be called directly to bypass querying  */
   /* all of the routers.                               */
   /*===================================================*/

   if (((char *) RouterData(theEnv)->FastLoadFilePtr) == logicalName)
     {
      if ((ch == '\r') || (ch == '\n'))
        {
         if (((char *) RouterData(theEnv)->FastLoadFilePtr) == RouterData(theEnv)->LineCountRouter)
           { DecrementLineCount(theEnv); }
        }

      return(ungetc(ch,RouterData(theEnv)->FastLoadFilePtr));
     }

   /*===============================================*/
   /* If the "fast string get" option is being used */
   /* for the specified logical name, then bypass   */
   /* the router system and unget the character     */
   /* directly from the fast get string.            */
   /*===============================================*/

   if (RouterData(theEnv)->FastCharGetRouter == logicalName)
     {
      if ((ch == '\r') || (ch == '\n'))
        {
         if (RouterData(theEnv)->FastCharGetRouter == RouterData(theEnv)->LineCountRouter)
           { DecrementLineCount(theEnv); }
        }

      if (RouterData(theEnv)->FastCharGetIndex > 0) RouterData(theEnv)->FastCharGetIndex--;
      return(ch);
     }

   /*===============================================*/
   /* Search through the list of routers until one  */
   /* is found that will handle the ungetc request. */
   /*===============================================*/

   currentPtr = RouterData(theEnv)->ListOfRouters;
   while (currentPtr != NULL)
     {
      if ((currentPtr->charunget != NULL) ? QueryRouter(theEnv,logicalName,currentPtr) : FALSE)
        {
         if ((ch == '\r') || (ch == '\n'))
           {
            if ((RouterData(theEnv)->LineCountRouter != NULL) &&
                (strcmp(logicalName,RouterData(theEnv)->LineCountRouter) == 0))
              { DecrementLineCount(theEnv); }
           }
           
         SetEnvironmentRouterContext(theEnv,currentPtr->context);
         if (currentPtr->environmentAware)
           { return((*currentPtr->charunget)(theEnv,ch,logicalName)); }
         else            
           { return(((int (*)(int,char *)) (*currentPtr->charunget))(ch,logicalName)); }
        }

      currentPtr = currentPtr->next;
     }

   /*=====================================================*/
   /* The logical name was not recognized by any routers. */
   /*=====================================================*/

   UnrecognizedRouterMessage(theEnv,logicalName);
   return(-1);
  }

/*****************************************************/
/* ExitCommand: H/L command for exiting the program. */
/*****************************************************/
globle void ExitCommand(
  void *theEnv)
  {
   int argCnt;
   int status;

   if ((argCnt = EnvArgCountCheck(theEnv,"exit",NO_MORE_THAN,1)) == -1) return;
   if (argCnt == 0)
     { EnvExitRouter(theEnv,EXIT_SUCCESS); }
   else
    {
     status = (int) EnvRtnLong(theEnv,1);
     if (GetEvaluationError(theEnv)) return;
     EnvExitRouter(theEnv,status);
    }

   return;
  }

/***********************************************/
/* EnvExitRouter: Generic exit function. Calls */
/*   all of the router exit functions.         */
/***********************************************/
globle void EnvExitRouter(
  void *theEnv,
  int num)
  {
   struct router *currentPtr, *nextPtr;

   RouterData(theEnv)->Abort = FALSE;
   currentPtr = RouterData(theEnv)->ListOfRouters;
   while (currentPtr != NULL)
     {
      nextPtr = currentPtr->next;
      if (currentPtr->active == TRUE)
        { 
         if (currentPtr->exiter != NULL) 
           {
            SetEnvironmentRouterContext(theEnv,currentPtr->context);
            if (currentPtr->environmentAware)
              { (*currentPtr->exiter)(theEnv,num); }
            else            
              { ((int (*)(int))(*currentPtr->exiter))(num); }
           }
        }
      currentPtr = nextPtr;
     }

   if (RouterData(theEnv)->Abort) return;
   genexit(theEnv,num);
  }

/********************************************/
/* AbortExit: Forces ExitRouter to terminate */
/*   after calling all closing routers.     */
/********************************************/
globle void AbortExit(
  void *theEnv)
  {
   RouterData(theEnv)->Abort = TRUE;
  }

#if ALLOW_ENVIRONMENT_GLOBALS
/*********************************************************/
/* AddRouter: Adds an I/O router to the list of routers. */
/*********************************************************/
globle intBool AddRouter(
  char *routerName,
  int priority,
  int (*queryFunction)(char *),
  int (*printFunction)(char *,char *),
  int (*getcFunction)(char *),
  int (*ungetcFunction)(int,char *),
  int (*exitFunction)(int))
  {
   struct router *newPtr, *lastPtr, *currentPtr;
   void *theEnv;
   char *nameCopy;
      
   theEnv = GetCurrentEnvironment();

   newPtr = get_struct(theEnv,router);

   nameCopy = (char *) genalloc(theEnv,strlen(routerName) + 1);
   genstrcpy(nameCopy,routerName);     
   newPtr->name = nameCopy;   
   
   newPtr->active = TRUE;
   newPtr->environmentAware = FALSE;
   newPtr->priority = priority;
   newPtr->context = NULL;
   newPtr->query = (int (*)(void *,char *)) queryFunction;
   newPtr->printer = (int (*)(void *,char *,char *)) printFunction;
   newPtr->exiter = (int (*)(void *,int)) exitFunction;
   newPtr->charget = (int (*)(void *,char *)) getcFunction;
   newPtr->charunget = (int (*)(void *,int,char *)) ungetcFunction;
   newPtr->next = NULL;

   if (RouterData(theEnv)->ListOfRouters == NULL)
     {
      RouterData(theEnv)->ListOfRouters = newPtr;
      return(1);
     }

   lastPtr = NULL;
   currentPtr = RouterData(theEnv)->ListOfRouters;
   while ((currentPtr != NULL) ? (priority < currentPtr->priority) : FALSE)
     {
      lastPtr = currentPtr;
      currentPtr = currentPtr->next;
     }

   if (lastPtr == NULL)
     {
      newPtr->next = RouterData(theEnv)->ListOfRouters;
      RouterData(theEnv)->ListOfRouters = newPtr;
     }
   else
     {
      newPtr->next = currentPtr;
      lastPtr->next = newPtr;
     }

   return(1);
  }
#endif

/************************************************************/
/* EnvAddRouter: Adds an I/O router to the list of routers. */
/************************************************************/
globle intBool EnvAddRouter(
  void *theEnv,
  char *routerName,
  int priority,
  int (*queryFunction)(void *,char *),
  int (*printFunction)(void *,char *,char *),
  int (*getcFunction)(void *,char *),
  int (*ungetcFunction)(void *,int,char *),
  int (*exitFunction)(void *,int))
  {
   return EnvAddRouterWithContext(theEnv,routerName,priority,
                                  queryFunction,printFunction,getcFunction,
                                  ungetcFunction,exitFunction,NULL);
  }

/***********************************************************************/
/* EnvAddRouterWithContext: Adds an I/O router to the list of routers. */
/***********************************************************************/
globle intBool EnvAddRouterWithContext(
  void *theEnv,
  char *routerName,
  int priority,
  int (*queryFunction)(void *,char *),
  int (*printFunction)(void *,char *,char *),
  int (*getcFunction)(void *,char *),
  int (*ungetcFunction)(void *,int,char *),
  int (*exitFunction)(void *,int),
  void *context)
  {
   struct router *newPtr, *lastPtr, *currentPtr;
   char  *nameCopy;

   newPtr = get_struct(theEnv,router);

   nameCopy = (char *) genalloc(theEnv,strlen(routerName) + 1);
   genstrcpy(nameCopy,routerName);     
   newPtr->name = nameCopy;

   newPtr->active = TRUE;
   newPtr->environmentAware = TRUE;
   newPtr->context = context;
   newPtr->priority = priority;
   newPtr->query = queryFunction;
   newPtr->printer = printFunction;
   newPtr->exiter = exitFunction;
   newPtr->charget = getcFunction;
   newPtr->charunget = ungetcFunction;
   newPtr->next = NULL;

   if (RouterData(theEnv)->ListOfRouters == NULL)
     {
      RouterData(theEnv)->ListOfRouters = newPtr;
      return(1);
     }

   lastPtr = NULL;
   currentPtr = RouterData(theEnv)->ListOfRouters;
   while ((currentPtr != NULL) ? (priority < currentPtr->priority) : FALSE)
     {
      lastPtr = currentPtr;
      currentPtr = currentPtr->next;
     }

   if (lastPtr == NULL)
     {
      newPtr->next = RouterData(theEnv)->ListOfRouters;
      RouterData(theEnv)->ListOfRouters = newPtr;
     }
   else
     {
      newPtr->next = currentPtr;
      lastPtr->next = newPtr;
     }

   return(1);
  }

/*****************************************************************/
/* EnvDeleteRouter: Removes an I/O router from the list of routers. */
/*****************************************************************/
globle int EnvDeleteRouter(
  void *theEnv,
  char *routerName)
  {
   struct router *currentPtr, *lastPtr;

   currentPtr = RouterData(theEnv)->ListOfRouters;
   lastPtr = NULL;

   while (currentPtr != NULL)
     {
      if (strcmp(currentPtr->name,routerName) == 0)
        {
         genfree(theEnv,currentPtr->name,strlen(currentPtr->name) + 1);
         if (lastPtr == NULL)
           {
            RouterData(theEnv)->ListOfRouters = currentPtr->next;
            rm(theEnv,currentPtr,(int) sizeof(struct router));
            return(1);
           }
         lastPtr->next = currentPtr->next;
         rm(theEnv,currentPtr,(int) sizeof(struct router));
         return(1);
        }
      lastPtr = currentPtr;
      currentPtr = currentPtr->next;
     }

   return(0);
  }

/*********************************************************************/
/* QueryRouters: Determines if any router recognizes a logical name. */
/*********************************************************************/
globle int QueryRouters(
  void *theEnv,
  char *logicalName)
  {
   struct router *currentPtr;

   currentPtr = RouterData(theEnv)->ListOfRouters;
   while (currentPtr != NULL)
     {
      if (QueryRouter(theEnv,logicalName,currentPtr) == TRUE) return(TRUE);
      currentPtr = currentPtr->next;
     }

   return(FALSE);
  }

/************************************************/
/* QueryRouter: Determines if a specific router */
/*    recognizes a logical name.                */
/************************************************/
static int QueryRouter(
  void *theEnv,
  char *logicalName,
  struct router *currentPtr)
  {
   /*===================================================*/
   /* If the router is inactive, then it can't respond. */
   /*===================================================*/

   if (currentPtr->active == FALSE)
     { return(FALSE); }

   /*=============================================================*/
   /* If the router has no query function, then it can't respond. */
   /*=============================================================*/

   if (currentPtr->query == NULL) return(FALSE);

   /*=========================================*/
   /* Call the router's query function to see */
   /* if it recognizes the logical name.      */
   /*=========================================*/
   
   SetEnvironmentRouterContext(theEnv,currentPtr->context);
   if (currentPtr->environmentAware)
     { 
      if ((*currentPtr->query)(theEnv,logicalName) == TRUE)
        { return(TRUE); }
     }
   else            
     { 
      if (((int (*)(char *)) (*currentPtr->query))(logicalName) == TRUE)
        { return(TRUE); }
     }

   return(FALSE);
  }

/*******************************************************/
/* EnvDeactivateRouter: Deactivates a specific router. */
/*******************************************************/
globle int EnvDeactivateRouter(
  void *theEnv,
  char *routerName)
  {
   struct router *currentPtr;

   currentPtr = RouterData(theEnv)->ListOfRouters;

   while (currentPtr != NULL)
     {
      if (strcmp(currentPtr->name,routerName) == 0)
        {
         currentPtr->active = FALSE;
         return(TRUE);
        }
      currentPtr = currentPtr->next;
     }

   return(FALSE);
  }

/***************************************************/
/* EnvActivateRouter: Activates a specific router. */
/***************************************************/
globle int EnvActivateRouter(
  void *theEnv,
  char *routerName)
  {
   struct router *currentPtr;

   currentPtr = RouterData(theEnv)->ListOfRouters;

   while (currentPtr != NULL)
     {
      if (strcmp(currentPtr->name,routerName) == 0)
        {
         currentPtr->active = TRUE;
         return(TRUE);
        }
      currentPtr = currentPtr->next;
     }

   return(FALSE);
  }

/********************************************************/
/* SetFastLoad: Used to bypass router system for loads. */
/********************************************************/
globle void SetFastLoad(
  void *theEnv,
  FILE *filePtr)
  { 
   RouterData(theEnv)->FastLoadFilePtr = filePtr; 
  }

/********************************************************/
/* SetFastSave: Used to bypass router system for saves. */
/********************************************************/
globle void SetFastSave(
  void *theEnv,
  FILE *filePtr)
  { 
   RouterData(theEnv)->FastSaveFilePtr = filePtr; 
  }

/******************************************************/
/* GetFastLoad: Returns the "fast load" file pointer. */
/******************************************************/
globle FILE *GetFastLoad(
  void *theEnv)
  {
   return(RouterData(theEnv)->FastLoadFilePtr); 
  }

/******************************************************/
/* GetFastSave: Returns the "fast save" file pointer. */
/******************************************************/
globle FILE *GetFastSave(
  void *theEnv)
  {
   return(RouterData(theEnv)->FastSaveFilePtr); 
  }

/*****************************************************/
/* UnrecognizedRouterMessage: Standard error message */
/*   for an unrecognized router name.                */
/*****************************************************/
globle void UnrecognizedRouterMessage(
  void *theEnv,
  char *logicalName)
  {
   PrintErrorID(theEnv,"ROUTER",1,FALSE);
   EnvPrintRouter(theEnv,WERROR,"Logical name ");
   EnvPrintRouter(theEnv,WERROR,logicalName);
   EnvPrintRouter(theEnv,WERROR," was not recognized by any routers\n");
  }

/*****************************************/
/* PrintNRouter: Generic print function. */
/*****************************************/
globle int PrintNRouter(
  void *theEnv,
  char *logicalName,
  char *str,
  unsigned long length)
  {
   char *tempStr;
   int rv;

   tempStr = (char *) genalloc(theEnv,length+1);
   genstrncpy(tempStr,str,length);
   tempStr[length] = 0;
   rv = EnvPrintRouter(theEnv,logicalName,tempStr);
   genfree(theEnv,tempStr,length+1);
   return(rv);
  }
