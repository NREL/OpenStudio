   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  06/05/06          */
   /*                                                     */
   /*                    BLOAD MODULE                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides core routines for loading constructs    */
/*   from a binary file.                                     */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*************************************************************/

#define _BLOAD_SOURCE_

#include "setup.h"

#include "argacces.h"
#include "bsave.h"
#include "constrct.h"
#include "cstrnbin.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "memalloc.h"
#include "router.h"
#include "utility.h"

#include "bload.h"

#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE)

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static struct FunctionDefinition **ReadNeededFunctions(void *,long *,int *);
   static struct FunctionDefinition  *FastFindFunction(void *,char *,struct FunctionDefinition *);
   static int                         ClearBload(void *);
   static void                        AbortBload(void *);
   static int                         BloadOutOfMemoryFunction(void *,size_t);
   static void                        DeallocateBloadData(void *);

/**********************************************/
/* InitializeBloadData: Allocates environment */
/*    data for the bload command.             */
/**********************************************/
globle void InitializeBloadData(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,BLOAD_DATA,sizeof(struct bloadData),NULL);
   AddEnvironmentCleanupFunction(theEnv,"bload",DeallocateBloadData,-1500);

   BloadData(theEnv)->BinaryPrefixID = "\1\2\3\4CLIPS";
   BloadData(theEnv)->BinaryVersionID = "V6.30";
  }
  
/************************************************/
/* DeallocateBloadData: Deallocates environment */
/*    data for the bload command.               */
/************************************************/
static void DeallocateBloadData(
  void *theEnv)
  {   
   DeallocateCallList(theEnv,BloadData(theEnv)->BeforeBloadFunctions);
   DeallocateCallList(theEnv,BloadData(theEnv)->AfterBloadFunctions);
   DeallocateCallList(theEnv,BloadData(theEnv)->ClearBloadReadyFunctions);
   DeallocateCallList(theEnv,BloadData(theEnv)->AbortBloadFunctions);
  }

/******************************/
/* EnvBload: C access routine */
/*   for the bload command.   */
/******************************/
globle int EnvBload(
  void *theEnv,
  char *fileName)
  {
   long numberOfFunctions;
   unsigned long space;
   int error;
   char IDbuffer[20];   
   char constructBuffer[CONSTRUCT_HEADER_SIZE];
   struct BinaryItem *biPtr;
   struct callFunctionItem *bfPtr;

   /*================*/
   /* Open the file. */
   /*================*/

   if (GenOpenReadBinary(theEnv,"bload",fileName) == 0) return(FALSE);

   /*=====================================*/
   /* Determine if this is a binary file. */
   /*=====================================*/

   GenReadBinary(theEnv,IDbuffer,(unsigned long) strlen(BloadData(theEnv)->BinaryPrefixID) + 1);
   if (strcmp(IDbuffer,BloadData(theEnv)->BinaryPrefixID) != 0)
     {
      PrintErrorID(theEnv,"BLOAD",2,FALSE);
      EnvPrintRouter(theEnv,WERROR,"File ");
      EnvPrintRouter(theEnv,WERROR,fileName);
      EnvPrintRouter(theEnv,WERROR," is not a binary construct file.\n");
      GenCloseBinary(theEnv);
      return(FALSE);
     }

   /*=======================================*/
   /* Determine if it's a binary file using */
   /* a format from a different version.    */
   /*=======================================*/

   GenReadBinary(theEnv,IDbuffer,(unsigned long) strlen(BloadData(theEnv)->BinaryVersionID) + 1);
   if (strcmp(IDbuffer,BloadData(theEnv)->BinaryVersionID) != 0)
     {
      PrintErrorID(theEnv,"BLOAD",3,FALSE);
      EnvPrintRouter(theEnv,WERROR,"File ");
      EnvPrintRouter(theEnv,WERROR,fileName);
      EnvPrintRouter(theEnv,WERROR," is an incompatible binary construct file.\n");
      GenCloseBinary(theEnv);
      return(FALSE);
     }
     
   /*====================*/
   /* Clear environment. */
   /*====================*/

   if (BloadData(theEnv)->BloadActive)
     {
      if (ClearBload(theEnv) == FALSE)
        {
         GenCloseBinary(theEnv);
         return(FALSE);
        }
     }

   /*=================================*/
   /* Determine if the KB environment */
   /* was successfully cleared.       */
   /*=================================*/

   if (ClearReady(theEnv) == FALSE)
     {
      GenCloseBinary(theEnv);
      EnvPrintRouter(theEnv,WERROR,"The ");
      EnvPrintRouter(theEnv,WERROR,APPLICATION_NAME);
      EnvPrintRouter(theEnv,WERROR," environment could not be cleared.\n");
      EnvPrintRouter(theEnv,WERROR,"Binary load cannot continue.\n");
      return(FALSE);
     }

   /*==================================*/
   /* Call the list of functions to be */
   /* executed before a bload occurs.  */
   /*==================================*/

   for (bfPtr = BloadData(theEnv)->BeforeBloadFunctions;
        bfPtr != NULL;
        bfPtr = bfPtr->next)
     { 
      if (bfPtr->environmentAware)
        { (*bfPtr->func)(theEnv); }
      else            
        { (* (void (*)(void)) bfPtr->func)(); }
     }

   /*====================================================*/
   /* Read in the functions needed by this binary image. */
   /*====================================================*/

   BloadData(theEnv)->FunctionArray = ReadNeededFunctions(theEnv,&numberOfFunctions,&error);
   if (error)
     {
      GenCloseBinary(theEnv);
      AbortBload(theEnv);
      return(FALSE);
     }

   /*================================================*/
   /* Read in the atoms needed by this binary image. */
   /*================================================*/

   ReadNeededAtomicValues(theEnv);

   /*===========================================*/
   /* Determine the number of expressions to be */
   /* read and allocate the appropriate space   */
   /*===========================================*/

   AllocateExpressions(theEnv);

   /*==========================================================*/
   /* Read in the memory requirements of the constructs stored */
   /* in this binary image and allocate the necessary space    */
   /*==========================================================*/

   for (GenReadBinary(theEnv,constructBuffer,(unsigned long) CONSTRUCT_HEADER_SIZE);
        strncmp(constructBuffer,BloadData(theEnv)->BinaryPrefixID,CONSTRUCT_HEADER_SIZE) != 0;
        GenReadBinary(theEnv,constructBuffer,(unsigned long) CONSTRUCT_HEADER_SIZE))
     {
      intBool found;

      /*================================================*/
      /* Search for the construct type in the list of   */
      /* binary items. If found, allocate the storage   */
      /* needed by the construct for this binary image. */
      /*================================================*/

      found = FALSE;
      for (biPtr = BsaveData(theEnv)->ListOfBinaryItems;
           biPtr != NULL;
           biPtr = biPtr->next)
        {
         if (strncmp(biPtr->name,constructBuffer,CONSTRUCT_HEADER_SIZE) == 0)
           {
            if (biPtr->bloadStorageFunction != NULL)
              {
               (*biPtr->bloadStorageFunction)(theEnv);
               found = TRUE;
              }
            break;
           }
        }

      /*==========================================*/
      /* If the construct type wasn't found, skip */
      /* the storage binary load information for  */
      /* this construct.                          */
      /*==========================================*/

      if (! found)
        {
         GenReadBinary(theEnv,&space,(unsigned long) sizeof(unsigned long));
         GetSeekCurBinary(theEnv,(long) space);
         if (space != 0)
           {
            EnvPrintRouter(theEnv,WDIALOG,"\nSkipping ");
            EnvPrintRouter(theEnv,WDIALOG,constructBuffer);
            EnvPrintRouter(theEnv,WDIALOG," constructs because of unavailibility\n");
           }
        }
     }

   /*======================================*/
   /* Refresh the pointers in expressions. */
   /*======================================*/

   RefreshExpressions(theEnv);

   /*==========================*/
   /* Read in the constraints. */
   /*==========================*/

   ReadNeededConstraints(theEnv);

   /*======================================================*/
   /* Read in the constructs stored in this binary image.  */
   /*======================================================*/

   for (GenReadBinary(theEnv,constructBuffer,(unsigned long) CONSTRUCT_HEADER_SIZE);
        strncmp(constructBuffer,BloadData(theEnv)->BinaryPrefixID,CONSTRUCT_HEADER_SIZE) != 0;
        GenReadBinary(theEnv,constructBuffer,(unsigned long) CONSTRUCT_HEADER_SIZE))
     {
      intBool found;

      /*==================================================*/
      /* Search for the function to load the construct    */
      /* into the previously allocated storage. If found, */
      /* call the function to load the construct.         */
      /*==================================================*/

      found = FALSE;
      for (biPtr = BsaveData(theEnv)->ListOfBinaryItems;
           biPtr != NULL;
           biPtr = biPtr->next)
        {
         if (strncmp(biPtr->name,constructBuffer,CONSTRUCT_HEADER_SIZE) == 0)
           {
            if (biPtr->bloadFunction != NULL)
              {
               (*biPtr->bloadFunction)(theEnv);
               found = TRUE;
              }
            break;
           }
        }

      /*==========================================*/
      /* If the construct type wasn't found, skip */
      /* the binary data for this construct.      */
      /*==========================================*/

      if (! found)
        {
         GenReadBinary(theEnv,&space,(unsigned long) sizeof(unsigned long));
         GetSeekCurBinary(theEnv,(long) space);
        }
     }

   /*=================*/
   /* Close the file. */
   /*=================*/

   GenCloseBinary(theEnv);

   /*========================================*/
   /* Free up temporary storage used for the */
   /* function and atomic value information. */
   /*========================================*/

   if (BloadData(theEnv)->FunctionArray != NULL)
     {
      genfree(theEnv,(void *) BloadData(theEnv)->FunctionArray,
              sizeof(struct FunctionDefinition *) * numberOfFunctions);
     }
   FreeAtomicValueStorage(theEnv);

   /*==================================*/
   /* Call the list of functions to be */
   /* executed after a bload occurs.   */
   /*==================================*/

   for (bfPtr = BloadData(theEnv)->AfterBloadFunctions;
        bfPtr != NULL;
        bfPtr = bfPtr->next)
     {       
      if (bfPtr->environmentAware)
        { (*bfPtr->func)(theEnv); }
      else            
        { (* (void (*)(void)) bfPtr->func)(); }
     }

   /*=======================================*/
   /* Add a clear function to remove binary */
   /* load when a clear command is issued.  */
   /*=======================================*/

   BloadData(theEnv)->BloadActive = TRUE;
   EnvAddClearFunction(theEnv,"bload",(void (*)(void *)) ClearBload,10000);

   /*=============================*/
   /* Return TRUE to indicate the */
   /* binary load was successful. */
   /*=============================*/

   return(TRUE);
  }

/************************************************************
  NAME         : BloadandRefresh
  DESCRIPTION  : Loads and refreshes objects - will bload
                 all objects at once, if possible, but
                 will aslo work in increments if memory is
                 restricted
  INPUTS       : 1) the number of objects to bload and update
                 2) the size of one object
                 3) An update function which takes a bloaded
                    object buffer and the index of the object
                    to refresh as arguments
  RETURNS      : Nothing useful
  SIDE EFFECTS : Objects bloaded and updated
  NOTES        : Assumes binary file pointer is positioned
                 for bloads of the objects
 ************************************************************/
globle void BloadandRefresh(
  void *theEnv,
  long objcnt,
  size_t objsz,
  void (*objupdate)(void *,void *,long))
  {
   register long i,bi;
   char *buf;
   long objsmaxread,objsread;
   size_t space;
   int (*oldOutOfMemoryFunction)(void *,size_t);

   if (objcnt == 0L) return;

   oldOutOfMemoryFunction = EnvSetOutOfMemoryFunction(theEnv,BloadOutOfMemoryFunction);
   objsmaxread = objcnt;
   do
     {
      space = objsmaxread * objsz;
      buf = (char *) genalloc(theEnv,space);
      if (buf == NULL)
        {
         if ((objsmaxread / 2) == 0)
           {
            if ((*oldOutOfMemoryFunction)(theEnv,space) == TRUE)
              {
               EnvSetOutOfMemoryFunction(theEnv,oldOutOfMemoryFunction);
               return;
              }
           }
         else
           objsmaxread /= 2;
        }
     }
   while (buf == NULL);

   EnvSetOutOfMemoryFunction(theEnv,oldOutOfMemoryFunction);

   i = 0L;
   do
     {
      objsread = (objsmaxread > (objcnt - i)) ? (objcnt - i) : objsmaxread;
      GenReadBinary(theEnv,(void *) buf,objsread * objsz);
      for (bi = 0L ; bi < objsread ; bi++ , i++)
        (*objupdate)(theEnv,buf + objsz * bi,i);
     }
   while (i < objcnt);
   genfree(theEnv,(void *) buf,space);
  }

/**********************************************/
/* ReadNeededFunctions: Reads in the names of */
/*   functions needed by the binary image.    */
/**********************************************/
static struct FunctionDefinition **ReadNeededFunctions(
  void *theEnv,
  long int *numberOfFunctions,
  int *error)
  {
   char *functionNames, *namePtr;
   unsigned long int space;
   size_t temp;
   long i;
   struct FunctionDefinition **newFunctionArray, *functionPtr;
   int functionsNotFound = 0;

   /*===================================================*/
   /* Determine the number of function names to be read */
   /* and the space required for them.                  */
   /*===================================================*/

   GenReadBinary(theEnv,numberOfFunctions,(unsigned long) sizeof(long int));
   GenReadBinary(theEnv,&space,(unsigned long) sizeof(unsigned long int));
   if (*numberOfFunctions == 0)
     {
      *error = FALSE;
      return(NULL);
     }

   /*=======================================*/
   /* Allocate area for strings to be read. */
   /*=======================================*/

   functionNames = (char *) genalloc(theEnv,space);
   GenReadBinary(theEnv,(void *) functionNames,space);

   /*====================================================*/
   /* Store the function pointers in the function array. */
   /*====================================================*/

   temp = (unsigned long) sizeof(struct FunctionDefinition *) * *numberOfFunctions;
   newFunctionArray = (struct FunctionDefinition **) genalloc(theEnv,temp);
   namePtr = functionNames;
   functionPtr = NULL;
   for (i = 0; i < *numberOfFunctions; i++)
     {
      if ((functionPtr = FastFindFunction(theEnv,namePtr,functionPtr)) == NULL)
        {
         if (! functionsNotFound)
           {
            PrintErrorID(theEnv,"BLOAD",6,FALSE);
            EnvPrintRouter(theEnv,WERROR,"The following undefined functions are ");
            EnvPrintRouter(theEnv,WERROR,"referenced by this binary image:\n");
           }

         EnvPrintRouter(theEnv,WERROR,"   ");
         EnvPrintRouter(theEnv,WERROR,namePtr);
         EnvPrintRouter(theEnv,WERROR,"\n");
         functionsNotFound = 1;
        }

      newFunctionArray[i] = functionPtr;
      namePtr += strlen(namePtr) + 1;
     }

   /*==========================================*/
   /* Free the memory used by the name buffer. */
   /*==========================================*/

   genfree(theEnv,(void *) functionNames,space);

   /*==================================================*/
   /* If any of the required functions were not found, */
   /* then free the memory used by the function array. */
   /*==================================================*/

   if (functionsNotFound)
     {
      genfree(theEnv,(void *) newFunctionArray,temp);
      newFunctionArray = NULL;
     }

   /*===================================*/
   /* Set globals to appropriate values */
   /* and return the function array.    */
   /*===================================*/

   *error = functionsNotFound;
   return(newFunctionArray);
  }

/*****************************************/
/* FastFindFunction: Search the function */
/*   list for a specific function.       */
/*****************************************/
static struct FunctionDefinition *FastFindFunction(
  void *theEnv,
  char *functionName,
  struct FunctionDefinition *lastFunction)
  {
   struct FunctionDefinition *theList, *theFunction;

   /*========================*/
   /* Get the function list. */
   /*========================*/

   theList = GetFunctionList(theEnv);
   if (theList == NULL) { return(NULL); }

   /*=======================================*/
   /* If we completed a previous function   */
   /* search, start where we last left off. */
   /*=======================================*/

   if (lastFunction != NULL)
     { theFunction = lastFunction->next; }
   else
     { theFunction = theList; }

   /*======================================================*/
   /* Traverse the rest of the function list searching for */
   /* the named function wrapping around if necessary.     */
   /*======================================================*/

   while (strcmp(functionName,ValueToString(theFunction->callFunctionName)) != 0)
     {
      theFunction = theFunction->next;
      if (theFunction == lastFunction) return(NULL);
      if (theFunction == NULL) theFunction = theList;
     }

   /*=======================*/
   /* Return the pointer to */
   /* the found function.   */
   /*=======================*/

   return(theFunction);
  }

/******************************************/
/* Bloaded: Returns TRUE if the current   */
/*   environment is the result of a bload */
/*   command, otherwise returns FALSE.    */
/******************************************/
globle intBool Bloaded(
  void *theEnv)
  {
   return(BloadData(theEnv)->BloadActive);
  }

/*************************************/
/* ClearBload: Clears a binary image */
/*   from the KB environment.        */
/*************************************/
static int ClearBload(
  void *theEnv)
  {
   struct BinaryItem *biPtr;
   struct callFunctionItem *bfPtr;
   int ready,error;

   /*=================================================*/
   /* Make sure it's safe to clear the bloaded image. */
   /*=================================================*/

   error = FALSE;
   for (bfPtr = BloadData(theEnv)->ClearBloadReadyFunctions;
        bfPtr != NULL;
        bfPtr = bfPtr->next)
     {
      if (bfPtr->environmentAware)
        { ready = (* ((int (*)(void *)) bfPtr->func))(theEnv); }
      else            
        { ready = (* ((int (*)(void)) bfPtr->func))(); }

      if (ready == FALSE)
        {
         if (! error)
           {
            PrintErrorID(theEnv,"BLOAD",5,FALSE);
            EnvPrintRouter(theEnv,WERROR,
                       "Some constructs are still in use by the current binary image:\n");
           }
         EnvPrintRouter(theEnv,WERROR,"   ");
         EnvPrintRouter(theEnv,WERROR,bfPtr->name);
         EnvPrintRouter(theEnv,WERROR,"\n");
         error = TRUE;
        }
     }

   /*==================================================*/
   /* If some constructs are still in use and can't be */
   /* cleared, indicate the binary load can't continue */
   /* and return FALSE to indicate this condition.     */
   /*==================================================*/

   if (error == TRUE)
     {
      EnvPrintRouter(theEnv,WERROR,"Binary clear cannot continue.\n");
      return(FALSE);
     }

   /*=============================*/
   /* Call bload clear functions. */
   /*=============================*/

   for (biPtr = BsaveData(theEnv)->ListOfBinaryItems;
        biPtr != NULL;
        biPtr = biPtr->next)
     { if (biPtr->clearFunction != NULL) (*biPtr->clearFunction)(theEnv); }

   /*===========================*/
   /* Free bloaded expressions. */
   /*===========================*/

   ClearBloadedExpressions(theEnv);

   /*===========================*/
   /* Free bloaded constraints. */
   /*===========================*/

   ClearBloadedConstraints(theEnv);

   /*==================================*/
   /* Remove the bload clear function. */
   /*==================================*/

   BloadData(theEnv)->BloadActive = FALSE;
   EnvRemoveClearFunction(theEnv,"bload");

   /*====================================*/
   /* Return TRUE to indicate the binary */
   /* image was successfully cleared.    */
   /*====================================*/

   return(TRUE);
  }

/*************************************************/
/* AbortBload: Cleans up effects of before-bload */
/*   functions in event of failure.              */
/*************************************************/
static void AbortBload(
  void *theEnv)
  {
   struct callFunctionItem *bfPtr;

   for (bfPtr = BloadData(theEnv)->AbortBloadFunctions;
        bfPtr != NULL;
        bfPtr = bfPtr->next)
     { 
      if (bfPtr->environmentAware)
        { (*bfPtr->func)(theEnv); }
      else            
        { (* (void (*)(void)) bfPtr->func)(); }
     }
  }

/********************************************/
/* AddBeforeBloadFunction: Adds a function  */
/*   to the list of functions called before */
/*   a binary load occurs.                  */
/********************************************/
globle void AddBeforeBloadFunction(
  void *theEnv,
  char *name,
  void (*func)(void *),
  int priority)
  {
   BloadData(theEnv)->BeforeBloadFunctions =
     AddFunctionToCallList(theEnv,name,priority,func,BloadData(theEnv)->BeforeBloadFunctions,TRUE);
  }

/*******************************************/
/* AddAfterBloadFunction: Adds a function  */
/*   to the list of functions called after */
/*   a binary load occurs.                 */
/*******************************************/
globle void AddAfterBloadFunction(
  void *theEnv,
  char *name,
  void (*func)(void *),
  int priority)
  {
   BloadData(theEnv)->AfterBloadFunctions =
      AddFunctionToCallList(theEnv,name,priority,func,BloadData(theEnv)->AfterBloadFunctions,TRUE);
  }

/**************************************************/
/* AddClearBloadReadyFunction: Adds a function to */
/*   the list of functions called to determine if */
/*   a binary image can be cleared.               */
/**************************************************/
globle void AddClearBloadReadyFunction(
  void *theEnv,
  char *name,
  int (*func)(void *),
  int priority)
  {
   BloadData(theEnv)->ClearBloadReadyFunctions =
      AddFunctionToCallList(theEnv,name,priority,
                            (void (*)(void *)) func,
                            BloadData(theEnv)->ClearBloadReadyFunctions,TRUE);
  }

/*********************************************/
/* AddAbortBloadFunction: Adds a function to */
/*   the list of functions called if a bload */
/*   has to be aborted.                      */
/*********************************************/
globle void AddAbortBloadFunction(
  void *theEnv,
  char *name,
  void (*func)(void *),
  int priority)
  {
   BloadData(theEnv)->AbortBloadFunctions = AddFunctionToCallList(theEnv,name,priority,func,BloadData(theEnv)->AbortBloadFunctions,TRUE);
  }

/*******************************************************
  NAME         : BloadOutOfMemoryFunction
  DESCRIPTION  : Memory function used by bload to
                   prevent exiting when out of
                   memory - used by BloadandRefresh
  INPUTS       : The memory request size (unused)
  RETURNS      : TRUE (indicates a failure and for
                 the memory functions to simply
                 return a NULL pointer)
  SIDE EFFECTS : None
  NOTES        : None
 *******************************************************/
#if WIN_BTC
#pragma argsused
#endif
static int BloadOutOfMemoryFunction(
  void *theEnv,
  size_t size)
  {
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(size,theEnv)
#endif
   return(TRUE);
  }

/*****************************************************/
/* CannotLoadWithBloadMessage: Generic error message */
/*   for indicating that a construct can't be loaded */
/*   when a binary image is active.                  */
/*****************************************************/
globle void CannotLoadWithBloadMessage(
  void *theEnv,
  char *constructName)
  {
   PrintErrorID(theEnv,"BLOAD",1,TRUE);
   EnvPrintRouter(theEnv,WERROR,"Cannot load ");
   EnvPrintRouter(theEnv,WERROR,constructName);
   EnvPrintRouter(theEnv,WERROR," construct with binary load in effect.\n");
  }

#endif /* (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) */

/**************************************/
/* BloadCommand: H/L access routine   */
/*   for the bload command.           */
/**************************************/
globle int BloadCommand(
  void *theEnv)
  {
#if (! RUN_TIME) && (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE)
   char *fileName;

   if (EnvArgCountCheck(theEnv,"bload",EXACTLY,1) == -1) return(FALSE);
   fileName = GetFileName(theEnv,"bload",1);
   if (fileName != NULL) return(EnvBload(theEnv,fileName));
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif
   return(FALSE);
  }
