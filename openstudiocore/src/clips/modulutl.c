   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*              DEFMODULE UTILITY MODULE               */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for parsing module/construct   */
/*   names and searching through modules for specific        */
/*   constructs.                                             */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#define _MODULUTL_SOURCE_

#include "setup.h"

#include "memalloc.h"
#include "router.h"
#include "envrnmnt.h"
#include "sysdep.h"

#include "modulpsr.h"
#include "modulutl.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                      *SearchImportedConstructModules(void *,struct symbolHashNode *,
                                              struct defmodule *,
                                              struct moduleItem *,struct symbolHashNode *,
                                              int *,int,struct defmodule *);

/********************************************************************/
/* FindModuleSeparator: Finds the :: separator which delineates the */
/*   boundary between a module name and a construct name. The value */
/*   zero is returned if the separator is not found, otherwise the  */
/*   position of the second colon within the string is returned.    */
/********************************************************************/
globle unsigned FindModuleSeparator(
  char *theString)
  {
   unsigned i, foundColon;

   for (i = 0, foundColon = FALSE; theString[i] != EOS; i++)
     {
      if (theString[i] == ':')
        {
         if (foundColon) return(i);
         foundColon = TRUE;
        }
      else
        { foundColon = FALSE; }
     }

   return(FALSE);
  }

/*******************************************************************/
/* ExtractModuleName: Given the position of the :: separator and a */
/*   module/construct name joined using the separator, returns a   */
/*   symbol reference to the module name (or NULL if a module name */
/*   cannot be extracted).                                         */
/*******************************************************************/
globle SYMBOL_HN *ExtractModuleName(
  void *theEnv,
  unsigned thePosition,
  char *theString)
  {
   char *newString;
   SYMBOL_HN *returnValue;

   /*=============================================*/
   /* Return NULL if the :: is in a position such */
   /* that a module name can't be extracted.      */
   /*=============================================*/

   if (thePosition <= 1) return(NULL);

   /*==========================================*/
   /* Allocate storage for a temporary string. */
   /*==========================================*/

   newString = (char *) gm2(theEnv,thePosition);

   /*======================================================*/
   /* Copy the entire module/construct name to the string. */
   /*======================================================*/

   genstrncpy(newString,theString,
           (STD_SIZE) thePosition - 1);

   /*========================================================*/
   /* Place an end of string marker where the :: is located. */
   /*========================================================*/

   newString[thePosition-1] = EOS;

   /*=====================================================*/
   /* Add the module name (the truncated module/construct */
   /* name) to the symbol table.                          */
   /*=====================================================*/

   returnValue = (SYMBOL_HN *) EnvAddSymbol(theEnv,newString);

   /*=============================================*/
   /* Return the storage of the temporary string. */
   /*=============================================*/

   rm(theEnv,newString,thePosition);

   /*=============================================*/
   /* Return a pointer to the module name symbol. */
   /*=============================================*/

   return(returnValue);
  }

/********************************************************************/
/* ExtractConstructName: Given the position of the :: separator and */
/*   a module/construct name joined using the separator, returns a  */
/*   symbol reference to the construct name (or NULL if a construct */
/*   name cannot be extracted).                                     */
/********************************************************************/
globle SYMBOL_HN *ExtractConstructName(
  void *theEnv,
  unsigned thePosition,
  char *theString)
  {
   size_t theLength;
   char *newString;
   SYMBOL_HN *returnValue;

   /*======================================*/
   /* Just return the string if it doesn't */
   /* contain the :: symbol.               */
   /*======================================*/

   if (thePosition == 0) return((SYMBOL_HN *) EnvAddSymbol(theEnv,theString));

   /*=====================================*/
   /* Determine the length of the string. */
   /*=====================================*/

   theLength = strlen(theString);

   /*=================================================*/
   /* Return NULL if the :: is at the very end of the */
   /* string (and thus there is no construct name).   */
   /*=================================================*/

   if (theLength <= (thePosition + 1)) return(NULL);

   /*====================================*/
   /* Allocate a temporary string large  */
   /* enough to hold the construct name. */
   /*====================================*/

   newString = (char *) gm2(theEnv,theLength - thePosition);

   /*================================================*/
   /* Copy the construct name portion of the         */
   /* module/construct name to the temporary string. */
   /*================================================*/

   genstrncpy(newString,&theString[thePosition+1],
           (STD_SIZE) theLength - thePosition);

   /*=============================================*/
   /* Add the construct name to the symbol table. */
   /*=============================================*/

   returnValue = (SYMBOL_HN *) EnvAddSymbol(theEnv,newString);

   /*=============================================*/
   /* Return the storage of the temporary string. */
   /*=============================================*/

   rm(theEnv,newString,theLength - thePosition);

   /*================================================*/
   /* Return a pointer to the construct name symbol. */
   /*================================================*/

   return(returnValue);
  }

/****************************************************/
/* ExtractModuleAndConstructName: Extracts both the */
/*   module and construct name from a string. Sets  */
/*   the current module to the specified module.    */
/****************************************************/
globle char *ExtractModuleAndConstructName(
  void *theEnv,
  char *theName)
  {
   unsigned separatorPosition;
   SYMBOL_HN *moduleName, *shortName;
   struct defmodule *theModule;

   /*========================*/
   /* Find the :: separator. */
   /*========================*/

   separatorPosition = FindModuleSeparator(theName);
   if (! separatorPosition) return(theName);

   /*==========================*/
   /* Extract the module name. */
   /*==========================*/

   moduleName = ExtractModuleName(theEnv,separatorPosition,theName);
   if (moduleName == NULL) return(NULL);

   /*====================================*/
   /* Check to see if the module exists. */
   /*====================================*/

   theModule = (struct defmodule *) EnvFindDefmodule(theEnv,ValueToString(moduleName));
   if (theModule == NULL) return(NULL);

   /*============================*/
   /* Change the current module. */
   /*============================*/

   EnvSetCurrentModule(theEnv,(void *) theModule);

   /*=============================*/
   /* Extract the construct name. */
   /*=============================*/

   shortName = ExtractConstructName(theEnv,separatorPosition,theName);
   if (shortName == NULL) return(NULL);
   return(ValueToString(shortName));
  }

/************************************************************/
/* FindImportedConstruct: High level routine which searches */
/*   a module and other modules from which it imports       */
/*   constructs for a specified construct.                  */
/************************************************************/
globle void *FindImportedConstruct(
  void *theEnv,
  char *constructName,
  struct defmodule *matchModule,
  char *findName,
  int *count,
  int searchCurrent,
  struct defmodule *notYetDefinedInModule)
  {
   void *rv;
   struct moduleItem *theModuleItem;

   /*=============================================*/
   /* Set the number of references found to zero. */
   /*=============================================*/

   *count = 0;

   /*===============================*/
   /* The :: should not be included */
   /* in the construct's name.      */
   /*===============================*/

   if (FindModuleSeparator(findName)) return(NULL);

   /*=============================================*/
   /* Remember the current module since we'll be  */
   /* changing it during the search and will want */
   /* to restore it once the search is completed. */
   /*=============================================*/

   SaveCurrentModule(theEnv);

   /*==========================================*/
   /* Find the module related access functions */
   /* for the construct type being sought.     */
   /*==========================================*/

   if ((theModuleItem = FindModuleItem(theEnv,constructName)) == NULL)
     {
      RestoreCurrentModule(theEnv);
      return(NULL);
     }

   /*===========================================*/
   /* If the construct type doesn't have a find */
   /* function, then we can't look for it.      */
   /*===========================================*/

   if (theModuleItem->findFunction == NULL)
     {
      RestoreCurrentModule(theEnv);
      return(NULL);
     }

   /*==================================*/
   /* Initialize the search by marking */
   /* all modules as unvisited.        */
   /*==================================*/

   MarkModulesAsUnvisited(theEnv);

   /*===========================*/
   /* Search for the construct. */
   /*===========================*/

   rv = SearchImportedConstructModules(theEnv,(SYMBOL_HN *) EnvAddSymbol(theEnv,constructName),
                                       matchModule,theModuleItem,
                                       (SYMBOL_HN *) EnvAddSymbol(theEnv,findName),count,
                                       searchCurrent,notYetDefinedInModule);

   /*=============================*/
   /* Restore the current module. */
   /*=============================*/

   RestoreCurrentModule(theEnv);

   /*====================================*/
   /* Return a pointer to the construct. */
   /*====================================*/

   return(rv);
  }

/*********************************************************/
/* AmbiguousReferenceErrorMessage: Error message printed */
/*   when a reference to a specific construct can be     */
/*   imported from more than one module.                 */
/*********************************************************/
globle void AmbiguousReferenceErrorMessage(
  void *theEnv,
  char *constructName,
  char *findName)
  {
   EnvPrintRouter(theEnv,WERROR,"Ambiguous reference to ");
   EnvPrintRouter(theEnv,WERROR,constructName);
   EnvPrintRouter(theEnv,WERROR," ");
   EnvPrintRouter(theEnv,WERROR,findName);
   EnvPrintRouter(theEnv,WERROR,".\nIt is imported from more than one module.\n");
  }

/****************************************************/
/* MarkModulesAsUnvisited: Used for initializing a  */
/*   search through the module heirarchies. Sets    */
/*   the visited flag of each module to FALSE.      */
/****************************************************/
globle void MarkModulesAsUnvisited(
  void *theEnv)
  {
   struct defmodule *theModule;

   DefmoduleData(theEnv)->CurrentModule->visitedFlag = FALSE;
   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     { theModule->visitedFlag = FALSE; }
  }

/***********************************************************/
/* SearchImportedConstructModules: Low level routine which */
/*   searches a module and other modules from which it     */
/*   imports constructs for a specified construct.         */
/***********************************************************/
static void *SearchImportedConstructModules(
  void *theEnv,
  struct symbolHashNode *constructType,
  struct defmodule *matchModule,
  struct moduleItem *theModuleItem,
  struct symbolHashNode *findName,
  int *count,
  int searchCurrent,
  struct defmodule *notYetDefinedInModule)
  {
   struct defmodule *theModule;
   struct portItem *theImportList, *theExportList;
   void *rv, *arv = NULL;
   int searchModule, exported;
   struct defmodule *currentModule;

   /*=========================================*/
   /* Start the search in the current module. */
   /* If the current module has already been  */
   /* visited, then return.                   */
   /*=========================================*/

   currentModule = ((struct defmodule *) EnvGetCurrentModule(theEnv));
   if (currentModule->visitedFlag) return(NULL);

   /*=======================================================*/
   /* The searchCurrent flag indicates whether the current  */
   /* module should be included in the search. In addition, */
   /* if matchModule is non-NULL, the current module will   */
   /* only be searched if it is the specific module from    */
   /* which we want the construct imported.                 */
   /*=======================================================*/

   if ((searchCurrent) &&
       ((matchModule == NULL) || (currentModule == matchModule)))
     {
      /*===============================================*/
      /* Look for the construct in the current module. */
      /*===============================================*/

      rv = (*theModuleItem->findFunction)(theEnv,ValueToString(findName));

      /*========================================================*/
      /* If we're in the process of defining the construct in   */
      /* the module we're searching then go ahead and increment */
      /* the count indicating the number of modules in which    */
      /* the construct was found.                               */
      /*========================================================*/

      if (notYetDefinedInModule == currentModule)
        {
         (*count)++;
         arv = rv;
        }

      /*=========================================================*/
      /* Otherwise, if the construct is in the specified module, */
      /* increment the count only if the construct actually      */
      /* belongs to the module. [Some constructs, like the COOL  */
      /* system classes, can be found in any module, but they    */
      /* actually belong to the MAIN module.]                    */
      /*=========================================================*/

      else if (rv != NULL)
        {
         if (((struct constructHeader *) rv)->whichModule->theModule == currentModule)
           { (*count)++; }
         arv = rv;
        }
     }

   /*=====================================*/
   /* Mark the current module as visited. */
   /*=====================================*/

   currentModule->visitedFlag = TRUE;

   /*===================================*/
   /* Search through all of the modules */
   /* imported by the current module.   */
   /*===================================*/

   theModule = ((struct defmodule *) EnvGetCurrentModule(theEnv));
   theImportList = theModule->importList;

   while (theImportList != NULL)
     {
      /*===================================================*/
      /* Determine if the module should be searched (based */
      /* upon whether the entire module, all constructs of */
      /* a specific type, or specifically named constructs */
      /* are imported).                                    */
      /*===================================================*/

      searchModule = FALSE;
      if ((theImportList->constructType == NULL) ||
          (theImportList->constructType == constructType))
        {
         if ((theImportList->constructName == NULL) ||
             (theImportList->constructName == findName))
           { searchModule = TRUE; }
        }

      /*=================================*/
      /* Determine if the module exists. */
      /*=================================*/

      if (searchModule)
        {
         theModule = (struct defmodule *)
                     EnvFindDefmodule(theEnv,ValueToString(theImportList->moduleName));
         if (theModule == NULL) searchModule = FALSE;
        }

      /*=======================================================*/
      /* Determine if the construct is exported by the module. */
      /*=======================================================*/

      if (searchModule)
        {
         exported = FALSE;
         theExportList = theModule->exportList;
         while ((theExportList != NULL) && (! exported))
           {
            if ((theExportList->constructType == NULL) ||
                (theExportList->constructType == constructType))
              {
               if ((theExportList->constructName == NULL) ||
                   (theExportList->constructName == findName))
                 { exported = TRUE; }
               }

            theExportList = theExportList->next;
           }

         if (! exported) searchModule = FALSE;
        }

      /*=================================*/
      /* Search in the specified module. */
      /*=================================*/

      if (searchModule)
        {
         EnvSetCurrentModule(theEnv,(void *) theModule);
         if ((rv = SearchImportedConstructModules(theEnv,constructType,matchModule,
                                                  theModuleItem,findName,
                                                  count,TRUE,
                                                  notYetDefinedInModule)) != NULL)
           { arv = rv; }
        }

      /*====================================*/
      /* Move on to the next imported item. */
      /*====================================*/

      theImportList = theImportList->next;
     }

   /*=========================*/
   /* Return a pointer to the */
   /* last construct found.   */
   /*=========================*/

   return(arv);
  }

/**************************************************************/
/* ConstructExported: Returns TRUE if the specified construct */
/*   is exported from the specified module.                   */
/**************************************************************/
globle intBool ConstructExported(
  void *theEnv,
  char *constructTypeStr,
  struct symbolHashNode *moduleName,
  struct symbolHashNode *findName)
  {
   struct symbolHashNode *constructType;
   struct defmodule *theModule;
   struct portItem *theExportList;
   
   constructType = FindSymbolHN(theEnv,constructTypeStr);
   theModule = (struct defmodule *) EnvFindDefmodule(theEnv,ValueToString(moduleName));
   
   if ((constructType == NULL) || (theModule == NULL) || (findName == NULL))
     { return(FALSE); }
   
   theExportList = theModule->exportList;
   while (theExportList != NULL)
     {
      if ((theExportList->constructType == NULL) ||
          (theExportList->constructType == constructType))
       {
        if ((theExportList->constructName == NULL) ||
            (theExportList->constructName == findName))
          { return TRUE; }
       }

      theExportList = theExportList->next;
     }

   return FALSE;
  }
         



/***************************************/
/* ListItemsDriver: Driver routine for */
/*   listing items in a module.        */
/***************************************/
globle void ListItemsDriver(
  void *theEnv,
  char *logicalName,
  struct defmodule *theModule,
  char *singleName,
  char *pluralName,
  void *(*nextFunction)(void *,void *),
  char *(*nameFunction)(void *),
  void (*printFunction)(void *,char *,void *),
  int (*doItFunction)(void *,void *))
  {
   void *constructPtr;
   char *constructName;
   long count = 0;
   int allModules = FALSE;
   int doIt;

   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*======================*/
   /* Print out the items. */
   /*======================*/

   if (theModule == NULL)
     {
      theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
      allModules = TRUE;
     }

   while (theModule != NULL)
     {
      if (allModules)
        {
         EnvPrintRouter(theEnv,logicalName,EnvGetDefmoduleName(theEnv,theModule));
         EnvPrintRouter(theEnv,logicalName,":\n");
        }

      EnvSetCurrentModule(theEnv,(void *) theModule);
      constructPtr = (*nextFunction)(theEnv,NULL);
      while (constructPtr != NULL)
        {
         if (EvaluationData(theEnv)->HaltExecution == TRUE) return;

         if (doItFunction == NULL) doIt = TRUE;
         else doIt = (*doItFunction)(theEnv,constructPtr);

         if (! doIt) {}
         else if (nameFunction != NULL)
           {
            constructName = (*nameFunction)(constructPtr);
            if (constructName != NULL)
              {
               if (allModules) EnvPrintRouter(theEnv,logicalName,"   ");
               EnvPrintRouter(theEnv,logicalName,constructName);
               EnvPrintRouter(theEnv,logicalName,"\n");
              }
           }
         else if (printFunction != NULL)
           {
            if (allModules) EnvPrintRouter(theEnv,logicalName,"   ");
            (*printFunction)(theEnv,logicalName,constructPtr);
            EnvPrintRouter(theEnv,logicalName,"\n");
           }

         constructPtr = (*nextFunction)(theEnv,constructPtr);
         count++;
        }

      if (allModules) theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule);
      else theModule = NULL;
     }

   /*=================================================*/
   /* Print the tally and restore the current module. */
   /*=================================================*/

   if (singleName != NULL) PrintTally(theEnv,logicalName,count,singleName,pluralName);

   RestoreCurrentModule(theEnv);
  }

/********************************************************/
/* DoForAllModules: Executes an action for all modules. */
/********************************************************/
globle long DoForAllModules(
  void *theEnv,
  void (*actionFunction)(struct defmodule *,void *),
  int interruptable,
  void *userBuffer)
  {
   void *theModule;
   long moduleCount = 0L;

   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*==================================*/
   /* Loop through all of the modules. */
   /*==================================*/

   for (theModule = EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = EnvGetNextDefmodule(theEnv,theModule), moduleCount++)
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      if ((interruptable) && GetHaltExecution(theEnv))
        {
         RestoreCurrentModule(theEnv);
         return(-1L);
        }

      (*actionFunction)((struct defmodule *) theModule,userBuffer);
     }

   /*=============================*/
   /* Restore the current module. */
   /*=============================*/

   RestoreCurrentModule(theEnv);

   /*=========================================*/
   /* Return the number of modules traversed. */
   /*=========================================*/

   return(moduleCount);
  }



