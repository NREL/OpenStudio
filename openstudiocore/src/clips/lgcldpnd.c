   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  10/19/06            */
   /*                                                     */
   /*             LOGICAL DEPENDENCIES MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provide support routines for managing truth      */
/*   maintenance using the logical conditional element.      */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Removed LOGICAL_DEPENDENCIES compilation flag. */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Rule with exists CE has incorrect activation.  */
/*            DR0867                                         */
/*                                                           */
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*************************************************************/

#define _LGCLDPND_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "setup.h"

#if DEFRULE_CONSTRUCT

#include "memalloc.h"
#include "router.h"
#include "envrnmnt.h"
#include "evaluatn.h"
#include "engine.h"
#include "reteutil.h"
#include "pattern.h"
#include "argacces.h"
#include "factmngr.h"

#if OBJECT_SYSTEM
#include "insfun.h"
#endif

#include "lgcldpnd.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static struct dependency      *DetachAssociatedDependencies(void *,struct dependency *,void *);

/***********************************************************************/
/* AddLogicalDependencies: Adds the logical dependency links between a */
/*   data entity (such as a fact or instance) and the partial match    */
/*   which logically supports that data entity. If a data entity is    */
/*   unconditionally asserted (i.e. the global variable TheLogicalJoin */
/*   is NULL), then existing logical support for the data entity is no */
/*   longer needed and it is removed. If a data entity is already      */
/*   unconditionally supported and that data entity is conditionally   */
/*   asserted (i.e. the global variable TheLogicalJoin is not NULL),   */
/*   then the logical support is ignored. Otherwise, the partial match */
/*   is linked to the data entity and the data entity is linked to the */
/*   partial match. Note that the word assert is used to refer to      */
/*   creating a fact with the assert command and creating an instance  */
/*   with the make-instance command.                                   */
/***********************************************************************/
globle intBool AddLogicalDependencies(
  void *theEnv,
  struct patternEntity *theEntity,
  int existingEntity)
  {
   struct partialMatch *theBinds;
   struct dependency *newDependency;

   /*==============================================*/
   /* If the rule has no logical patterns, then no */
   /* dependencies have to be established.         */
   /*==============================================*/

   if (EngineData(theEnv)->TheLogicalJoin == NULL)
     {
      if (existingEntity) RemoveEntityDependencies(theEnv,theEntity);
      return(TRUE);
     }
   else if (existingEntity && (theEntity->dependents == NULL))
     { return(TRUE); }

   /*===========================================================*/
   /* Retrieve the partial match in the logical join associated */
   /* with activation partial match (retrieved when the run     */
   /* command was initiated). If the partial match's parent     */
   /* links have been removed, then the partial match must have */
   /* been deleted by a previous RHS action and the dependency  */
   /* link should not be added.                                 */
   /*===========================================================*/

   theBinds = EngineData(theEnv)->TheLogicalBind;
   if (theBinds == NULL) return(FALSE);
   if ((theBinds->leftParent == NULL) && (theBinds->rightParent == NULL))
     { return(FALSE); }

   /*==============================================================*/
   /* Add a dependency link between the partial match and the data */
   /* entity. The dependency links are stored in the partial match */
   /* behind the data entities stored in the partial match and the */
   /* activation link, if any.                                     */
   /*==============================================================*/

   newDependency = get_struct(theEnv,dependency);
   newDependency->dPtr = (void *) theEntity;
   newDependency->next = (struct dependency *) theBinds->dependents;
   theBinds->dependents = (void *) newDependency;

   /*================================================================*/
   /* Add a dependency link between the entity and the partialMatch. */
   /*================================================================*/

   newDependency = get_struct(theEnv,dependency);
   newDependency->dPtr = (void *) theBinds;
   newDependency->next = (struct dependency *) theEntity->dependents;
   theEntity->dependents = (void *) newDependency;

   /*==================================================================*/
   /* Return TRUE to indicate that the data entity should be asserted. */
   /*==================================================================*/

   return(TRUE);
  }

/************************************************************************/
/* FindLogicalBind: Finds the partial match associated with the logical */
/*   CE which will provide logical support for a data entity asserted   */
/*   from the currently executing rule. The function is called when     */
/*   creating logical support links between the data entity and         */
/*   supporting partial matches.                                        */
/************************************************************************/
globle struct partialMatch *FindLogicalBind(
  struct joinNode *theJoin,
  struct partialMatch *theBinds)
  {
   struct partialMatch *compPtr;
      
   /*========================================================*/
   /* Follow the parent link of the activation back through  */
   /* the join network until the join containing the logical */
   /* partial match is found. The partial match at this      */
   /* join will have the dependency link assigned to it.     */
   /*========================================================*/
   
   for (compPtr = theBinds;
        compPtr != NULL;
        compPtr = compPtr->leftParent)
     {
      if (compPtr->owner == theJoin)
        { return(compPtr); }
     }

   return(NULL);
  }

/*********************************************************************/
/* RemoveEntityDependencies: Removes all logical support links from  */
/*   a pattern entity that point to partial matches or other pattern */
/*   entities. Also removes the associated links from the partial    */
/*   matches or pattern entities which point back to the pattern     */
/*   entities.                                                       */
/*********************************************************************/
globle void RemoveEntityDependencies(
  void *theEnv,
  struct patternEntity *theEntity)
  {
   struct dependency *fdPtr, *nextPtr, *theList;
   struct partialMatch *theBinds;

   /*===============================*/
   /* Get the list of dependencies. */
   /*===============================*/

   fdPtr = (struct dependency *) theEntity->dependents;

   /*========================================*/
   /* Loop through each of the dependencies. */
   /*========================================*/

   while (fdPtr != NULL)
     {
      /*===============================*/
      /* Remember the next dependency. */
      /*===============================*/

      nextPtr = fdPtr->next;

      /*================================================================*/
      /* Remove the link between the data entity and the partial match. */
      /*================================================================*/

      theBinds = (struct partialMatch *) fdPtr->dPtr;
      theList = (struct dependency *) theBinds->dependents;
      theList = DetachAssociatedDependencies(theEnv,theList,(void *) theEntity);
      theBinds->dependents = (void *) theList;

      /*========================*/
      /* Return the dependency. */
      /*========================*/

      rtn_struct(theEnv,dependency,fdPtr);

      /*=================================*/
      /* Move on to the next dependency. */
      /*=================================*/

      fdPtr = nextPtr;
     }

   /*=====================================================*/
   /* Set the dependency list of the data entity to NULL. */
   /*=====================================================*/

   theEntity->dependents = NULL;
  }
  
/********************************************************************/
/* ReturnEntityDependencies: Removes all logical support links from */
/*   a pattern entity. This is unidirectional. The links from the   */
/*   the partial match to the entity are not removed.               */
/********************************************************************/
globle void ReturnEntityDependencies(
  void *theEnv,
  struct patternEntity *theEntity)
  {
   struct dependency *fdPtr, *nextPtr;

   fdPtr = (struct dependency *) theEntity->dependents;

   while (fdPtr != NULL)
     {
      nextPtr = fdPtr->next;
      rtn_struct(theEnv,dependency,fdPtr);
      fdPtr = nextPtr;
     }

   theEntity->dependents = NULL;
  }

/*******************************************************************/
/* DetachAssociatedDependencies: Removes all logical support links */
/*   which pointer to a pattern entity from a list of dependencies */
/*   (which may be associated with either a partial match or       */
/*   another pattern entity). Does not remove links which point in */
/*   the other direction.                                          */
/*******************************************************************/
static struct dependency *DetachAssociatedDependencies(
  void *theEnv,
  struct dependency *theList,
  void *theEntity)
  {
   struct dependency *fdPtr, *nextPtr, *lastPtr = NULL;

   fdPtr = theList;

   while (fdPtr != NULL)
     {
      if (fdPtr->dPtr == theEntity)
        {
         nextPtr = fdPtr->next;
         if (lastPtr == NULL) theList = nextPtr;
         else lastPtr->next = nextPtr;
         rtn_struct(theEnv,dependency,fdPtr);
         fdPtr = nextPtr;
        }
      else
        {
         lastPtr = fdPtr;
         fdPtr = fdPtr->next;
        }
     }

   return(theList);
  }

/**************************************************************************/
/* RemovePMDependencies: Removes all logical support links from a partial */
/*   match that point to any data entities. Also removes the associated   */
/*   links from the data entities which point back to the partial match.  */
/**************************************************************************/
globle void RemovePMDependencies(
  void *theEnv,
  struct partialMatch *theBinds)
  {
   struct dependency *fdPtr, *nextPtr, *theList;
   struct patternEntity *theEntity;

   fdPtr = (struct dependency *) theBinds->dependents;

   while (fdPtr != NULL)
     {
      nextPtr = fdPtr->next;

      theEntity = (struct patternEntity *) fdPtr->dPtr;

      theList = (struct dependency *) theEntity->dependents;
      theList = DetachAssociatedDependencies(theEnv,theList,(void *) theBinds);
      theEntity->dependents = (void *) theList;

      rtn_struct(theEnv,dependency,fdPtr);
      fdPtr = nextPtr;
     }

   theBinds->dependents = NULL;
  }

/************************************************************/
/* DestroyPMDependencies: Removes all logical support links */
/*   from a partial match that point to any data entities.  */
/************************************************************/
globle void DestroyPMDependencies(
  void *theEnv,
  struct partialMatch *theBinds)
  {
   struct dependency *fdPtr, *nextPtr;

   fdPtr = (struct dependency *) theBinds->dependents;

   while (fdPtr != NULL)
     {
      nextPtr = fdPtr->next;

      rtn_struct(theEnv,dependency,fdPtr);
      fdPtr = nextPtr;
     }

   theBinds->dependents = NULL;
  }

/************************************************************************/
/* RemoveLogicalSupport: Removes the dependency links between a partial */
/*   match and the data entities it logically supports. Also removes    */
/*   the associated links from the data entities which point back to    */
/*   the partial match by calling DetachAssociatedEntityDependencies.   */
/*   If an entity has all of its logical support removed as a result of */
/*   this procedure, the dependency link from the partial match is      */
/*   added to the list of unsupported data entities so that the entity  */
/*   will be deleted as a result of losing its logical support.         */
/************************************************************************/
globle void RemoveLogicalSupport(
  void *theEnv,
  struct partialMatch *theBinds)
  {
   struct dependency *dlPtr, *tempPtr, *theList;
   struct patternEntity *theEntity;

   /*========================================*/
   /* If the partial match has no associated */
   /* dependencies, then return.             */
   /*========================================*/

   if (theBinds->dependents == NULL) return;

   /*=======================================*/
   /* Loop through each of the dependencies */
   /* attached to the partial match.        */
   /*=======================================*/

   dlPtr = (struct dependency *) theBinds->dependents;

   while (dlPtr != NULL)
     {
      /*===============================*/
      /* Remember the next dependency. */
      /*===============================*/

      tempPtr = dlPtr->next;

      /*==========================================================*/
      /* Determine the data entity associated with the dependency */
      /* structure and delete its dependency references to this   */
      /* partial match.                                           */
      /*==========================================================*/

      theEntity = (struct patternEntity *) dlPtr->dPtr;

      theList = (struct dependency *) theEntity->dependents;
      theList = DetachAssociatedDependencies(theEnv,theList,(void *) theBinds);
      theEntity->dependents = (void *) theList;

      /*==============================================================*/
      /* If the data entity has lost all of its logical support, then */
      /* add the dependency structure from the partial match to the   */
      /* list of unsupported data entities to be deleted. Otherwise,  */
      /* just delete the dependency structure.                        */
      /*==============================================================*/

      if (theEntity->dependents == NULL)
        {
         (*theEntity->theInfo->base.incrementBusyCount)(theEnv,theEntity);
         dlPtr->next = EngineData(theEnv)->UnsupportedDataEntities;
         EngineData(theEnv)->UnsupportedDataEntities = dlPtr;
        }
      else
        { rtn_struct(theEnv,dependency,dlPtr); }

      /*==================================*/
      /* Move on to the next dependency.  */
      /*==================================*/

      dlPtr = tempPtr;
     }

   /*=====================================*/
   /* The partial match no longer has any */
   /* dependencies associated with it.    */
   /*=====================================*/

   theBinds->dependents = NULL;
  }

/********************************************************************/
/* ForceLogicalRetractions: Deletes the data entities found on the  */
/*   list of items that have lost their logical support. The delete */
/*   function associated with each data entity is called to delete  */
/*   that data entity. Calling the delete function may in turn      */
/*   add more data entities to the list of data entities which have */
/*   lost their logical support.                                    */
/********************************************************************/
globle void ForceLogicalRetractions(
  void *theEnv)
  {
   struct dependency *tempPtr;
   struct patternEntity *theEntity;

   /*===================================================*/
   /* Don't reenter this function once it's called. Any */
   /* new additions to the list of items to be deleted  */
   /* as a result of losing their logical support will  */
   /* be handled properly.                              */
   /*===================================================*/

   if (EngineData(theEnv)->alreadyEntered) return;
   EngineData(theEnv)->alreadyEntered = TRUE;

   /*=======================================================*/
   /* Continue to delete the first item on the list as long */
   /* as one exists. This is done because new items may be  */
   /* placed at the beginning of the list as other data     */
   /* entities are deleted.                                 */
   /*=======================================================*/

   while (EngineData(theEnv)->UnsupportedDataEntities != NULL)
     {
      /*==========================================*/
      /* Determine the data entity to be deleted. */
      /*==========================================*/

      theEntity = (struct patternEntity *) EngineData(theEnv)->UnsupportedDataEntities->dPtr;

      /*================================================*/
      /* Remove the dependency structure from the list. */
      /*================================================*/

      tempPtr = EngineData(theEnv)->UnsupportedDataEntities;
      EngineData(theEnv)->UnsupportedDataEntities = EngineData(theEnv)->UnsupportedDataEntities->next;
      rtn_struct(theEnv,dependency,tempPtr);

      /*=========================*/
      /* Delete the data entity. */
      /*=========================*/

      (*theEntity->theInfo->base.decrementBusyCount)(theEnv,theEntity);
      (*theEntity->theInfo->base.deleteFunction)(theEnv,theEntity);
     }

   /*============================================*/
   /* Deletion of items on the list is complete. */
   /*============================================*/

   EngineData(theEnv)->alreadyEntered = FALSE;
  }

/****************************************************************/
/* Dependencies: C access routine for the dependencies command. */
/****************************************************************/
globle void Dependencies(
  void *theEnv,
  struct patternEntity *theEntity)
  {
   struct dependency *fdPtr;

   /*=========================================*/
   /* If the data entity has no dependencies, */
   /* then print "None" and return.           */
   /*=========================================*/

   if (theEntity->dependents == NULL)
     {
      EnvPrintRouter(theEnv,WDISPLAY,"None\n");
      return;
     }

   /*============================================*/
   /* Loop through the list of the data entities */
   /* dependencies and print them.               */
   /*============================================*/

   for (fdPtr = (struct dependency *) theEntity->dependents;
        fdPtr != NULL;
        fdPtr = fdPtr->next)
     {
      if (GetHaltExecution(theEnv) == TRUE) return;
      PrintPartialMatch(theEnv,WDISPLAY,(struct partialMatch *) fdPtr->dPtr);
      EnvPrintRouter(theEnv,WDISPLAY,"\n");
     }
  }

/************************************************************/
/* Dependents: C access routine for the dependents command. */
/************************************************************/
globle void Dependents(
  void *theEnv,
  struct patternEntity *theEntity)
  {
   struct patternEntity *entityPtr = NULL;
   struct patternParser *theParser = NULL;
   struct dependency *fdPtr;
   struct partialMatch *theBinds;
   int found = FALSE;
   
   /*=================================*/
   /* Loop through every data entity. */
   /*=================================*/

   for (GetNextPatternEntity(theEnv,&theParser,&entityPtr);
        entityPtr != NULL;
        GetNextPatternEntity(theEnv,&theParser,&entityPtr))
     {
      if (GetHaltExecution(theEnv) == TRUE) return;

      /*====================================*/
      /* Loop through every dependency link */
      /* associated with the data entity.   */
      /*====================================*/

      for (fdPtr = (struct dependency *) entityPtr->dependents;
           fdPtr != NULL;
           fdPtr = fdPtr->next)
        {
         if (GetHaltExecution(theEnv) == TRUE) return;

         /*=====================================================*/
         /* If the data entity which was the argument passed to */
         /* the dependents command is contained in one of the   */
         /* partial matches of the data entity currently being  */
         /* examined, then the data entity being examined is a  */
         /* dependent. Print the data entity and then move on   */
         /* to the next data entity.                            */
         /*=====================================================*/

         theBinds = (struct partialMatch *) fdPtr->dPtr;
         if (FindEntityInPartialMatch(theEntity,theBinds) == TRUE)
           {
            if (found) EnvPrintRouter(theEnv,WDISPLAY,",");
            (*entityPtr->theInfo->base.shortPrintFunction)(theEnv,WDISPLAY,entityPtr);
            found = TRUE;
            break;
           }
        }
     }

   /*=================================================*/
   /* If no dependents were found, then print "None." */
   /* Otherwise print a carriage return after the     */
   /* list of dependents.                             */
   /*=================================================*/

   if (! found) EnvPrintRouter(theEnv,WDISPLAY,"None\n");
   else EnvPrintRouter(theEnv,WDISPLAY,"\n");
  }

#if DEBUGGING_FUNCTIONS

/*********************************************/
/* DependenciesCommand: H/L access routine   */
/*   for the dependencies command.           */
/*********************************************/
globle void DependenciesCommand(
  void *theEnv)
  {
   DATA_OBJECT item;
   void *ptr;

   if (EnvArgCountCheck(theEnv,"dependencies",EXACTLY,1) == -1) return;

   ptr = GetFactOrInstanceArgument(theEnv,1,&item,"dependencies");

   if (ptr == NULL) return;

#if DEFRULE_CONSTRUCT
   Dependencies(theEnv,(struct patternEntity *) ptr);
#else
   EnvPrintRouter(theEnv,WDISPLAY,"None\n");
#endif
  }

/*******************************************/
/* DependentsCommand: H/L access routine   */
/*   for the dependents command.           */
/*******************************************/
globle void DependentsCommand(
  void *theEnv)
  {
   DATA_OBJECT item;
   void *ptr;

   if (EnvArgCountCheck(theEnv,"dependents",EXACTLY,1) == -1) return;

   ptr = GetFactOrInstanceArgument(theEnv,1,&item,"dependents");

   if (ptr == NULL) return;

#if DEFRULE_CONSTRUCT
   Dependents(theEnv,(struct patternEntity *) ptr);
#else
   EnvPrintRouter(theEnv,WDISPLAY,"None\n");
#endif
  }

#endif /* DEBUGGING_FUNCTIONS */

#endif /* DEFRULE_CONSTRUCT */

