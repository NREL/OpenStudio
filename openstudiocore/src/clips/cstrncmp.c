   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  07/01/05            */
   /*                                                     */
   /*          CONSTRAINT CONSTRUCTS-TO-C MODULE          */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the constructs-to-c feature for       */
/*    constraint records.                                    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Added allowed-classes slot facet.              */
/*                                                           */
/*            Added environment parameter to GenClose.       */
/*                                                           */
/*************************************************************/

#define _CSTRNCMP_SOURCE_

#include "setup.h"

#if CONSTRUCT_COMPILER && (! RUN_TIME)

#include "constant.h"

#include "conscomp.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"
#include "sysdep.h"

#include "cstrncmp.h"

/***********************************************/
/* ConstraintsToCode: Produces the constraint  */
/*   record code for a run-time module created */
/*   using the constructs-to-c function.       */
/***********************************************/
globle int ConstraintsToCode(
  void *theEnv,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices)
  {
   int i, j, count;
   int newHeader = TRUE;
   FILE *fp;
   int version = 1;
   int arrayVersion = 1;
   unsigned short numberOfConstraints = 0;
   CONSTRAINT_RECORD *tmpPtr;

   /*===============================================*/
   /* Count the total number of constraint records. */
   /*===============================================*/

   for (i = 0 ; i < SIZE_CONSTRAINT_HASH; i++)
     {
      for (tmpPtr = ConstraintData(theEnv)->ConstraintHashtable[i];
           tmpPtr != NULL;
           tmpPtr = tmpPtr->next)
        { tmpPtr->bsaveIndex = numberOfConstraints++; }
     }

   /*=====================================================*/
   /* If dynamic constraint checking is disabled, then    */
   /* contraints won't be saved. If there are constraints */
   /* which could be saved, then issue a warning message. */
   /*=====================================================*/

   if ((! EnvGetDynamicConstraintChecking(theEnv)) && (numberOfConstraints != 0))
     {
      numberOfConstraints = 0;
      PrintWarningID(theEnv,"CSTRNCMP",1,FALSE);
      EnvPrintRouter(theEnv,WWARNING,"Constraints are not saved with a constructs-to-c image\n");
      EnvPrintRouter(theEnv,WWARNING,"  when dynamic constraint checking is disabled.\n");
     }

   if (numberOfConstraints == 0) return(-1);

   /*=================================================*/
   /* Print the extern definition in the header file. */
   /*=================================================*/

   for (i = 1; i <= (numberOfConstraints / maxIndices) + 1 ; i++)
     { fprintf(headerFP,"extern CONSTRAINT_RECORD C%d_%d[];\n",imageID,i); }

   /*==================*/
   /* Create the file. */
   /*==================*/

   if ((fp = NewCFile(theEnv,fileName,pathName,fileNameBuffer,fileID,version,FALSE)) == NULL) return(-1);

   /*===================*/
   /* List the entries. */
   /*===================*/

   j = 0;
   count = 0;

   for (i = 0; i < SIZE_CONSTRAINT_HASH; i++)
     {
      for (tmpPtr = ConstraintData(theEnv)->ConstraintHashtable[i];
           tmpPtr != NULL;
           tmpPtr = tmpPtr->next)
        {
         if (newHeader)
           {
            fprintf(fp,"CONSTRAINT_RECORD C%d_%d[] = {\n",imageID,arrayVersion);
            newHeader = FALSE;
           }

         fprintf(fp,"{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                 tmpPtr->anyAllowed,
                 tmpPtr->symbolsAllowed,
                 tmpPtr->stringsAllowed,
                 tmpPtr->floatsAllowed,
                 tmpPtr->integersAllowed,
                 tmpPtr->instanceNamesAllowed,
                 tmpPtr->instanceAddressesAllowed,
                 tmpPtr->externalAddressesAllowed,
                 tmpPtr->factAddressesAllowed,
                 0, /* void allowed */
                 tmpPtr->anyRestriction,
                 tmpPtr->symbolRestriction,
                 tmpPtr->stringRestriction,
                 tmpPtr->floatRestriction,
                 tmpPtr->integerRestriction,
                 tmpPtr->classRestriction,
                 tmpPtr->instanceNameRestriction,
                 tmpPtr->multifieldsAllowed,
                 tmpPtr->singlefieldsAllowed);

         fprintf(fp,",0,"); /* bsaveIndex */

         PrintHashedExpressionReference(theEnv,fp,tmpPtr->classList,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->restrictionList,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->minValue,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->maxValue,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->minFields,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->maxFields,imageID,maxIndices);

         /* multifield slot */

         fprintf(fp,",NULL");

         /* next slot */

         if (tmpPtr->next == NULL)
           { fprintf(fp,",NULL,"); }
         else
           {
            if ((j + 1) >= maxIndices)
              { fprintf(fp,",&C%d_%d[%d],",imageID,arrayVersion + 1,0); }
            else
              { fprintf(fp,",&C%d_%d[%d],",imageID,arrayVersion,j + 1); }
           }

         fprintf(fp,"%d,%d",tmpPtr->bucket,tmpPtr->count + 1);

         count++;
         j++;

         if ((count == numberOfConstraints) || (j >= maxIndices))
           {
            fprintf(fp,"}};\n");
            GenClose(theEnv,fp);
            j = 0;
            version++;
            arrayVersion++;
            if (count < numberOfConstraints)
              {
               if ((fp = NewCFile(theEnv,fileName,pathName,fileNameBuffer,1,version,FALSE)) == NULL) return(0);
               newHeader = TRUE;
              }
           }
         else
           { fprintf(fp,"},\n"); }
        }
     }

   return(version);
  }

/**********************************************************/
/* PrintConstraintReference: Prints C code representation */
/*   of a constraint record data structure reference.     */
/**********************************************************/
globle void PrintConstraintReference(
  void *theEnv,
  FILE *fp,
  CONSTRAINT_RECORD *cPtr,
  int imageID,
  int maxIndices)
  {
   if ((cPtr == NULL) || (! EnvGetDynamicConstraintChecking(theEnv)))
     { fprintf(fp,"NULL"); }
   else fprintf(fp,"&C%d_%d[%d]",imageID,
                                 (int) (cPtr->bsaveIndex / maxIndices) + 1,
                                 (int) cPtr->bsaveIndex % maxIndices);
  }

#endif /* CONSTRUCT_COMPILER && (! RUN_TIME) */



