   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
   /*                                                     */
   /*    OBJECT PATTERN NETWORK CONSTRUCTS-TO-C MODULE    */
   /*******************************************************/

/**************************************************************/
/* Purpose: Saves object pattern network for constructs-to-c  */
/*                                                            */
/* Principal Programmer(s):                                   */
/*      Brian L. Dantes                                       */
/*                                                            */
/* Contributing Programmer(s):                                */
/*                                                            */
/* Revision History:                                          */
/*                                                            */
/*      6.24: Converted INSTANCE_PATTERN_MATCHING to          */
/*            DEFRULE_CONSTRUCT.                              */
/*                                                            */
/*            Added environment parameter to GenClose.        */
/*                                                            */
/**************************************************************/
/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if DEFRULE_CONSTRUCT && OBJECT_SYSTEM && (! RUN_TIME) && CONSTRUCT_COMPILER

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "conscomp.h"
#include "envrnmnt.h"
#include "objrtfnx.h"
#include "objrtmch.h"
#include "pattern.h"
#include "sysdep.h"

#define _OBJRTCMP_SOURCE_
#include "objrtcmp.h"

/* =========================================
   *****************************************
                 MACROS AND TYPES
   =========================================
   ***************************************** */
#define ObjectPNPrefix() ArbitraryPrefix(ObjectReteData(theEnv)->ObjectPatternCodeItem,0)
#define ObjectANPrefix() ArbitraryPrefix(ObjectReteData(theEnv)->ObjectPatternCodeItem,1)

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static void BeforeObjectPatternsToCode(void *);
static OBJECT_PATTERN_NODE *GetNextObjectPatternNode(OBJECT_PATTERN_NODE *);
static void InitObjectPatternsCode(void *,FILE *,int,int);
static int ObjectPatternsToCode(void *,char *,char *,char *,int,FILE *,int,int);
static void IntermediatePatternNodeReference(void *,OBJECT_PATTERN_NODE *,FILE *,int,int);
static int IntermediatePatternNodesToCode(void *,char *,char *,char *,int,FILE *,int,int,int);
static int AlphaPatternNodesToCode(void *,char *,char *,char *,int,FILE *,int,int,int);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***************************************************
  NAME         : ObjectPatternsCompilerSetup
  DESCRIPTION  : Sets up interface for object
                 patterns to construct compiler
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : Code generator item added
  NOTES        : None
 ***************************************************/
globle void ObjectPatternsCompilerSetup(
  void *theEnv)
  {
   ObjectReteData(theEnv)->ObjectPatternCodeItem =
         AddCodeGeneratorItem(theEnv,"object-patterns",0,BeforeObjectPatternsToCode,
                              InitObjectPatternsCode,ObjectPatternsToCode,2);
  }

/***************************************************
  NAME         : ObjectPatternNodeReference
  DESCRIPTION  : Prints out a reference to an
                 object pattern alpha memory for
                 the join network interface to the
                 construct compiler
  INPUTS       : 1) A pointer to the object pattern
                    alpha memory
                 2) A pointer to the output file
                 3) The id of constructs-to-c image
                 4) The maximum number of indices
                    allowed in any single array
                    in the image
  RETURNS      : Nothing useful
  SIDE EFFECTS : Reference to object pattern alpha
                 memory printed
  NOTES        : None
 ***************************************************/
globle void ObjectPatternNodeReference(
  void *theEnv,
  void *theVPattern,
  FILE *theFile,
  int imageID,
  int maxIndices)
  {
   OBJECT_ALPHA_NODE *thePattern;

   if (theVPattern == NULL)
     fprintf(theFile,"NULL");
   else
     {
      thePattern = (OBJECT_ALPHA_NODE *) theVPattern;
      fprintf(theFile,"&%s%d_%d[%d]",
                      ObjectANPrefix(),imageID,
                      (((int) thePattern->bsaveID) / maxIndices) + 1,
                      ((int) thePattern->bsaveID) % maxIndices);
     }
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/*****************************************************
  NAME         : BeforeObjectPatternsToCode
  DESCRIPTION  : Marks all object pattern intermediate
                 and alpha memory nodes with a
                 unique integer id prior to the
                 constructs-to-c execution
  INPUTS       : None
  RETURNS      : Nothing useful
  SIDE EFFECTS : bsaveIDs of nodes set
  NOTES        : None
 *****************************************************/
static void BeforeObjectPatternsToCode(
  void *theEnv)
  {
   long whichPattern;
   OBJECT_PATTERN_NODE *intermediateNode;
   OBJECT_ALPHA_NODE *alphaNode;

   whichPattern = 0L;
   intermediateNode = ObjectNetworkPointer(theEnv);
   while (intermediateNode != NULL)
     {
      intermediateNode->bsaveID = whichPattern++;
      intermediateNode = GetNextObjectPatternNode(intermediateNode);
     }

   whichPattern = 0L;
   alphaNode = ObjectNetworkTerminalPointer(theEnv);
   while (alphaNode != NULL)
     {
      alphaNode->bsaveID = whichPattern++;
      alphaNode = alphaNode->nxtTerminal;
     }
  }

/***************************************************
  NAME         : GetNextObjectPatternNode
  DESCRIPTION  : Grabs the next node in a depth
                 first perusal of the object pattern
                 intermediate nodes
  INPUTS       : The previous node
  RETURNS      : The next node (NULL if done)
  SIDE EFFECTS : None
  NOTES        : Alpha meory nodes are ignored
 ***************************************************/
static OBJECT_PATTERN_NODE *GetNextObjectPatternNode(
  OBJECT_PATTERN_NODE *thePattern)
  {
   if (thePattern->nextLevel != NULL)
     return(thePattern->nextLevel);
   while (thePattern->rightNode == NULL)
     {
      thePattern = thePattern->lastLevel;
      if (thePattern == NULL)
        return(NULL);
     }
   return(thePattern->rightNode);
  }

/***************************************************
  NAME         : InitObjectPatternsCode
  DESCRIPTION  : Prints out run-time initialization
                 code for object patterns
  INPUTS       : 1) A pointer to the output file
                 2) The id of constructs-to-c image
                 3) The maximum number of indices
                    allowed in any single array
                    in the image
  RETURNS      : Nothing useful
  SIDE EFFECTS : Initialization code written
  NOTES        : None
 ***************************************************/
static void InitObjectPatternsCode(
  void *theEnv,
  FILE *initFP,
  int imageID,
  int maxIndices)
  {
   long firstIntermediateNode,firstAlphaNode;

   if (ObjectNetworkPointer(theEnv) != NULL)
     {
      firstIntermediateNode = ObjectNetworkPointer(theEnv)->bsaveID;
      firstAlphaNode = ObjectNetworkTerminalPointer(theEnv)->bsaveID;
      fprintf(initFP,"   SetObjectNetworkPointer(theEnv,&%s%d_%d[%d]);\n",
                       ObjectPNPrefix(),imageID,
                       (int) ((firstIntermediateNode / maxIndices) + 1),
                       (int) (firstIntermediateNode % maxIndices));
      fprintf(initFP,"   SetObjectNetworkTerminalPointer(theEnv,&%s%d_%d[%d]);\n",
                       ObjectANPrefix(),imageID,
                       (int) ((firstAlphaNode / maxIndices) + 1),
                       (int) (firstAlphaNode % maxIndices));
     }
   else
     {
      fprintf(initFP,"   SetObjectNetworkPointer(theEnv,NULL);\n");
      fprintf(initFP,"   SetObjectNetworkTerminalPointer(theEnv,NULL);\n");
     }
  }

/***********************************************************
  NAME         : ObjectPatternsToCode
  DESCRIPTION  : Writes out data structures for run-time
                 creation of object patterns
  INPUTS       : 1) The base image output file name
                 2) The base image file id
                 3) A pointer to the header output file
                 4) The id of constructs-to-c image
                 5) The maximum number of indices
                    allowed in any single array
                    in the image
  RETURNS      : 1 if OK, 0 if could not open a file
  SIDE EFFECTS : Object patterns code written to files
  NOTES        : None
 ***********************************************************/
static int ObjectPatternsToCode(
  void *theEnv,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices)
  {
   int version;
   
   version = IntermediatePatternNodesToCode(theEnv,fileName,pathName,fileNameBuffer,
                                            fileID,headerFP,imageID,maxIndices,1);
   if (version == 0)
     return(0);
   if (! AlphaPatternNodesToCode(theEnv,fileName,pathName,fileNameBuffer,fileID,headerFP,imageID,maxIndices,version))
     return(0);
   return(1);
  }

/***************************************************
  NAME         : IntermediatePatternNodeReference
  DESCRIPTION  : Prints out a reference to an
                 object pattern intermediate node
  INPUTS       : 1) A pointer to the object pattern
                    intermediate node
                 2) A pointer to the output file
                 3) The id of constructs-to-c image
                 4) The maximum number of indices
                    allowed in any single array
                    in the image
  RETURNS      : 1 if OK, 0 if could not open a file
  SIDE EFFECTS : Reference to object pattern alpha
                 memory printed
  NOTES        : None
 ***************************************************/
static void IntermediatePatternNodeReference(
  void *theEnv,
  OBJECT_PATTERN_NODE *thePattern,
  FILE *theFile,
  int imageID,
  int maxIndices)
  {
   if (thePattern == NULL)
     fprintf(theFile,"NULL");
   else
     {
      fprintf(theFile,"&%s%d_%d[%d]",
                    ObjectPNPrefix(),imageID,
                    (((int) thePattern->bsaveID) / maxIndices) + 1,
                    ((int) thePattern->bsaveID) % maxIndices);
     }
  }

/*************************************************************
  NAME         : IntermediatePatternNodesToCode
  DESCRIPTION  : Writes out data structures for run-time
                 creation of object pattern intermediate nodes
  INPUTS       : 1) The base image output file name
                 2) The base image file id
                 3) A pointer to the header output file
                 4) The id of constructs-to-c image
                 5) The maximum number of indices
                    allowed in any single array
                    in the image
  RETURNS      : Next version file to open, 0 if error
  SIDE EFFECTS : Object patterns code written to files
  NOTES        : None
 *************************************************************/
static int IntermediatePatternNodesToCode(
  void *theEnv,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices,
   int version)
  {
   FILE *fp;
   int arrayVersion;
   int newHeader;
   int i;
   OBJECT_PATTERN_NODE *thePattern;

   /* ================
      Create the file.
      ================ */
   if (ObjectNetworkPointer(theEnv) == NULL)
     return(1);

   fprintf(headerFP,"#include \"objrtmch.h\"\n");

   /* =================================
      Dump the pattern node structures.
      ================================= */
   if ((fp = NewCFile(theEnv,fileName,pathName,fileNameBuffer,fileID,version,FALSE)) == NULL)
     return(0);
   newHeader = TRUE;

   arrayVersion = 1;
   i = 1;

   thePattern = ObjectNetworkPointer(theEnv);
   while (thePattern != NULL)
     {
      if (newHeader)
        {
         fprintf(fp,"OBJECT_PATTERN_NODE %s%d_%d[] = {\n",
                     ObjectPNPrefix(),imageID,arrayVersion);
         fprintf(headerFP,"extern OBJECT_PATTERN_NODE %s%d_%d[];\n",
                     ObjectPNPrefix(),imageID,arrayVersion);
         newHeader = FALSE;
        }
      fprintf(fp,"{0,%u,%u,%u,%u,%u,0L,%u,",thePattern->multifieldNode,
                                        thePattern->endSlot,
                                        thePattern->selector,
                                        thePattern->whichField,
                                        thePattern->leaveFields,
                                        thePattern->slotNameID);

      PrintHashedExpressionReference(theEnv,fp,thePattern->networkTest,imageID,maxIndices);
      fprintf(fp,",");
      IntermediatePatternNodeReference(theEnv,thePattern->nextLevel,fp,imageID,maxIndices);
      fprintf(fp,",");
      IntermediatePatternNodeReference(theEnv,thePattern->lastLevel,fp,imageID,maxIndices);
      fprintf(fp,",");
      IntermediatePatternNodeReference(theEnv,thePattern->leftNode,fp,imageID,maxIndices);
      fprintf(fp,",");
      IntermediatePatternNodeReference(theEnv,thePattern->rightNode,fp,imageID,maxIndices);
      fprintf(fp,",");
      ObjectPatternNodeReference(theEnv,(void *) thePattern->alphaNode,fp,imageID,maxIndices);
      fprintf(fp,",0L}");

      i++;
      thePattern = GetNextObjectPatternNode(thePattern);

      if ((i > maxIndices) || (thePattern == NULL))
        {
         fprintf(fp,"};\n");
         GenClose(theEnv,fp);
         i = 1;
         version++;
         arrayVersion++;
         if (thePattern != NULL)
           {
            if ((fp = NewCFile(theEnv,fileName,pathName,fileNameBuffer,fileID,version,FALSE)) == NULL)
              return(0);
            newHeader = TRUE;
           }
        }
      else if (thePattern != NULL)
        { fprintf(fp,",\n"); }
     }

   return(version);
  }

/***********************************************************
  NAME         : AlphaPatternNodesToCode
  DESCRIPTION  : Writes out data structures for run-time
                 creation of object pattern alpha memories
  INPUTS       : 1) The base image output file name
                 2) The base image file id
                 3) A pointer to the header output file
                 4) The id of constructs-to-c image
                 5) The maximum number of indices
                    allowed in any single array
                    in the image
  RETURNS      : Next version file to open, 0 if error
  SIDE EFFECTS : Object patterns code written to files
  NOTES        : None
 ***********************************************************/
static int AlphaPatternNodesToCode(
  void *theEnv,
  char *fileName,
  char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices,
  int version)
  {
   FILE *fp;
   int arrayVersion;
   int newHeader;
   int i;
   OBJECT_ALPHA_NODE *thePattern;

   /* ================
      Create the file.
      ================ */
   if (ObjectNetworkTerminalPointer(theEnv) == NULL)
     return(version);

   /* =================================
      Dump the pattern node structures.
      ================================= */
   if ((fp = NewCFile(theEnv,fileName,pathName,fileNameBuffer,fileID,version,FALSE)) == NULL)
     return(0);
   newHeader = TRUE;

   arrayVersion = 1;
   i = 1;

   thePattern = ObjectNetworkTerminalPointer(theEnv);
   while (thePattern != NULL)
     {
      if (newHeader)
        {
         fprintf(fp,"OBJECT_ALPHA_NODE %s%d_%d[] = {\n",
                    ObjectANPrefix(),imageID,arrayVersion);
         fprintf(headerFP,"extern OBJECT_ALPHA_NODE %s%d_%d[];\n",
                          ObjectANPrefix(),imageID,arrayVersion);
         newHeader = FALSE;
        }

      fprintf(fp,"{");

      PatternNodeHeaderToCode(theEnv,fp,&thePattern->header,imageID,maxIndices);

      fprintf(fp,",0L,");
      PrintBitMapReference(theEnv,fp,thePattern->classbmp);
      fprintf(fp,",");
      PrintBitMapReference(theEnv,fp,thePattern->slotbmp);
      fprintf(fp,",");
      IntermediatePatternNodeReference(theEnv,thePattern->patternNode,fp,imageID,maxIndices);
      fprintf(fp,",");
      ObjectPatternNodeReference(theEnv,thePattern->nxtInGroup,fp,imageID,maxIndices);
      fprintf(fp,",");
      ObjectPatternNodeReference(theEnv,thePattern->nxtTerminal,fp,imageID,maxIndices);
      fprintf(fp,",0L}");

      i++;
      thePattern = thePattern->nxtTerminal;

      if ((i > maxIndices) || (thePattern == NULL))
        {
         fprintf(fp,"};\n");
         GenClose(theEnv,fp);
         i = 1;
         version++;
         arrayVersion++;
         if (thePattern != NULL)
           {
            if ((fp = NewCFile(theEnv,fileName,pathName,fileNameBuffer,fileID,version,FALSE)) == NULL)
              return(0);
            newHeader = TRUE;
           }
        }
      else if (thePattern != NULL)
        { fprintf(fp,",\n"); }
     }

   return(version);
  }

#endif
