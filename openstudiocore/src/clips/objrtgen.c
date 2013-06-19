   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  05/17/06          */
   /*                                                     */
   /*    INFERENCE ENGINE OBJECT PARSING ROUTINES MODULE  */
   /*******************************************************/

/**************************************************************/
/* Purpose: RETE Network Parsing Interface for Objects        */
/*                                                            */
/* Principal Programmer(s):                                   */
/*      Brian L. Dantes                                       */
/*                                                            */
/* Contributing Programmer(s):                                */
/*                                                            */
/* Revision History:                                          */
/*      6.23: Changed name of variable exp to theExp          */
/*            because of Unix compiler warnings of shadowed   */
/*            definitions.                                    */
/*                                                            */
/*      6.24: Converted INSTANCE_PATTERN_MATCHING to          */
/*            DEFRULE_CONSTRUCT.                              */
/*                                                            */
/*            Renamed BOOLEAN macro type to intBool.          */
/*                                                            */
/**************************************************************/
/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if DEFRULE_CONSTRUCT && OBJECT_SYSTEM && (! RUN_TIME) && (! BLOAD_ONLY)

#ifndef _STDIO_INCLUDED_
#include <stdio.h>
#define _STDIO_INCLUDED_
#endif

#include "classfun.h"
#include "envrnmnt.h"
#include "objrtfnx.h"

#define _OBJRTGEN_SOURCE_
#include "objrtgen.h"

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static void GenObjectGetVar(void *,int,EXPRESSION *,struct lhsParseNode *,int);
static intBool IsSimpleSlotVariable(struct lhsParseNode *);
static EXPRESSION *GenerateSlotComparisonTest(void *,int,int,struct lhsParseNode *,struct lhsParseNode *);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/**********************************************
  Build functions used by AddPatternParser() to
  provide object access to the join nertwork
 **********************************************/
globle void ReplaceGetJNObjectValue(
  void *theEnv,
  EXPRESSION *theItem,
  struct lhsParseNode *theNode,
  int side)
  {
   GenObjectGetVar(theEnv,TRUE,theItem,theNode,side);
  }

globle EXPRESSION *GenGetJNObjectValue(
  void *theEnv,
  struct lhsParseNode *theNode,
  int side)
  {
   EXPRESSION *theItem;

   theItem = GenConstant(theEnv,0,NULL);
   GenObjectGetVar(theEnv,TRUE,theItem,theNode,side);
   return(theItem);
  }

globle EXPRESSION *ObjectJNVariableComparison(
  void *theEnv,
  struct lhsParseNode *selfNode,
  struct lhsParseNode *referringNode,
  int isNand)
  { 
   return(GenerateSlotComparisonTest(theEnv,TRUE,isNand,selfNode,referringNode));
  }

/**********************************************
  Build functions used by AddPatternParser() to
  provide object access to the pattern network
 **********************************************/
globle EXPRESSION *GenObjectPNConstantCompare(
  void *theEnv,
  struct lhsParseNode *theNode)
  {
   struct ObjectCmpPNConstant hack;
   EXPRESSION *theExp;
   unsigned short tmpType;

   /* ===============================================================
      If the value of a single field slot (or relation name) is being
      compared against a constant, then use specialized routines for
      doing the comparison.

      If a constant comparison is being done within a multifield slot
      and the constant's position has no multifields to the left or
      no multifields to the right, then use the same routine used for
      the single field slot case, but include the offset from either
      the beginning or end of the slot.

      Otherwise, use a general eq/neq test.
      =============================================================== */
   ClearBitString((void *) &hack,(int) sizeof(struct ObjectCmpPNConstant));
   if (theNode->negated)
     hack.fail = 1;
   else
     hack.pass = 1;
   if (((theNode->withinMultifieldSlot == FALSE) ||
        (theNode->multiFieldsAfter == 0) ||
        (theNode->multiFieldsBefore == 0)) &&
       (theNode->slotNumber != ISA_ID) && (theNode->slotNumber != NAME_ID))
     {
      if (theNode->withinMultifieldSlot == FALSE)
        hack.fromBeginning = TRUE;
      else if (theNode->multiFieldsBefore == 0)
        {
         hack.fromBeginning = TRUE;
         hack.offset = theNode->singleFieldsBefore;
        }
      else
        hack.offset = theNode->singleFieldsAfter;
      theExp = GenConstant(theEnv,OBJ_PN_CONSTANT,EnvAddBitMap(theEnv,(void *) &hack,
                                        (int) sizeof(struct ObjectCmpPNConstant)));
      theExp->argList = GenConstant(theEnv,theNode->type,theNode->value);
     }
   else
     {
      hack.general = 1;
      theExp = GenConstant(theEnv,OBJ_PN_CONSTANT,EnvAddBitMap(theEnv,(void *) &hack,
                                        (int) sizeof(struct ObjectCmpPNConstant)));
      theExp->argList = GenConstant(theEnv,0,NULL);
      tmpType = theNode->type;
      theNode->type = SF_VARIABLE;
      GenObjectGetVar(theEnv,FALSE,theExp->argList,theNode,-1);
      theNode->type = tmpType;
      theExp->argList->nextArg = GenConstant(theEnv,theNode->type,theNode->value);
     }
   return(theExp);
  }

globle void ReplaceGetPNObjectValue(
  void *theEnv,
  EXPRESSION *theItem,
  struct lhsParseNode *theNode)
  {
   GenObjectGetVar(theEnv,FALSE,theItem,theNode,-1);
  }

globle EXPRESSION *GenGetPNObjectValue(
  void *theEnv,
  struct lhsParseNode *theNode)
  {
   EXPRESSION *theItem;

   theItem = GenConstant(theEnv,0,NULL);
   GenObjectGetVar(theEnv,FALSE,theItem,theNode,-1);
   return(theItem);
  }

globle EXPRESSION *ObjectPNVariableComparison(
  void *theEnv,
  struct lhsParseNode *selfNode,
  struct lhsParseNode *referringNode)
  {
   return(GenerateSlotComparisonTest(theEnv,FALSE,FALSE,selfNode,referringNode));
  }

/****************************************************
  NAME         : GenObjectLengthTest
  DESCRIPTION  : Generates a test on the cardinality
                 of a slot matching an object pattern
  INPUTS       : The first lhsParseNode for a slot
                 in an object pattern
  RETURNS      : Nothing useful
  SIDE EFFECTS : The lhsParseNode network test is
                 modified to include the length test
  NOTES        : None
 ****************************************************/
globle void GenObjectLengthTest(
  void *theEnv,
  struct lhsParseNode *theNode)
  {
   struct ObjectMatchLength hack;
   EXPRESSION *theTest;

   if ((theNode->singleFieldsAfter == 0) &&
       (theNode->type != SF_VARIABLE) &&
       (theNode->type != SF_WILDCARD))
     return;

   ClearBitString((void *) &hack,(int) sizeof(struct ObjectMatchLength));

   if ((theNode->type != MF_VARIABLE) &&
       (theNode->type != MF_WILDCARD) &&
       (theNode->multiFieldsAfter == 0))
     hack.exactly = 1;
   else
     hack.exactly = 0;

   if ((theNode->type == SF_VARIABLE) || (theNode->type == SF_WILDCARD))
     hack.minLength = 1 + theNode->singleFieldsAfter;
   else
     hack.minLength = theNode->singleFieldsAfter;

   theTest = GenConstant(theEnv,OBJ_SLOT_LENGTH,EnvAddBitMap(theEnv,(void *) &hack,
                                         (int) sizeof(struct ObjectMatchLength)));
                                         
   if (theNode->constantSelector != NULL)
     { theNode->constantSelector->nextArg = CopyExpression(theEnv,theTest); }

   theNode->networkTest = CombineExpressions(theEnv,theTest,theNode->networkTest);
  }

/****************************************************
  NAME         : GenObjectZeroLengthTest
  DESCRIPTION  : Generates a test on the cardinality
                 of a slot matching an object pattern
  INPUTS       : The first lhsParseNode for a slot
                 in an object pattern
  RETURNS      : Nothing useful
  SIDE EFFECTS : The lhsParseNode network test is
                 modified to include the length test
  NOTES        : None
 ****************************************************/
globle void GenObjectZeroLengthTest(
  void *theEnv,
  struct lhsParseNode *theNode)
  {
   struct ObjectMatchLength hack;
   EXPRESSION *theTest;

   ClearBitString((void *) &hack,(int) sizeof(struct ObjectMatchLength));
   hack.exactly = 1;
   hack.minLength = 0;
   theTest = GenConstant(theEnv,OBJ_SLOT_LENGTH,EnvAddBitMap(theEnv,(void *) &hack,
                                         (int) sizeof(struct ObjectMatchLength)));
   theNode->networkTest = CombineExpressions(theEnv,theTest,theNode->networkTest);
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/***************************************************
  NAME         : GenObjectGetVar
  DESCRIPTION  : Generates the expressions necessary
                 to access object pattern variables
  INPUTS       : 1) An integer code indicating if
                    this is a join network reference
                    or a pattern network reference
                 2) The expression for which to set
                    the type and value
                 3) The lhsParseNode for the
                    variable reference
                 4) For a join reference, the side from
                    which the variable must be retrieved.
  RETURNS      : Nothing useful
  SIDE EFFECTS : The value is a packed long holding
                 pattern index, slot number,
                 field index, etc.
  NOTES        : None
 ***************************************************/
static void GenObjectGetVar(
  void *theEnv,
  int joinReference,
  EXPRESSION *theItem,
  struct lhsParseNode *theNode,
  int side)
  {
   struct ObjectMatchVar1 hack1;
   struct ObjectMatchVar2 hack2;

   ClearBitString((void *) &hack1,(int) sizeof(struct ObjectMatchVar1));
   ClearBitString((void *) &hack2,(int) sizeof(struct ObjectMatchVar2));

   if (joinReference)
     {
      if (side == LHS)
        { 
         hack1.lhs = 1; 
         hack2.lhs = 1; 
         hack1.whichPattern = (unsigned short) theNode->joinDepth;
         hack2.whichPattern = (unsigned short) theNode->joinDepth;
        }
      else if (side == RHS)
        { 
         hack1.rhs = 1; 
         hack2.rhs = 1; 
         hack1.whichPattern = (unsigned short) 0;
         hack2.whichPattern = (unsigned short) 0;
        }
      else if (side == NESTED_RHS)
        { 
         hack1.rhs = 1; 
         hack2.rhs = 1; 
         hack1.whichPattern = (unsigned short) theNode->joinDepth;
         hack2.whichPattern = (unsigned short) theNode->joinDepth;
        }
      else
        { 
         hack1.whichPattern = (unsigned short) theNode->joinDepth; 
         hack2.whichPattern = (unsigned short) theNode->joinDepth; 
        }
     }

   /* ========================
      Access an object address
      ======================== */
   if (theNode->slotNumber < 0)
     {
      hack1.objectAddress = 1;
      SetpType(theItem,(joinReference ? OBJ_GET_SLOT_JNVAR1 : OBJ_GET_SLOT_PNVAR1));
      theItem->value = EnvAddBitMap(theEnv,(void *) &hack1,(int) sizeof(struct ObjectMatchVar1));
      return;
     }

   /* ======================================
      Access the entire contents of the slot
      ====================================== */
   if ((theNode->singleFieldsBefore == 0) &&
       (theNode->singleFieldsAfter == 0) &&
       (theNode->multiFieldsBefore == 0) &&
       (theNode->multiFieldsAfter == 0) &&
       ((theNode->withinMultifieldSlot == FALSE) ||
        (theNode->type == MF_VARIABLE) ||
        (theNode->type == MF_WILDCARD)))
     {
      hack1.allFields = 1;
      hack1.whichSlot = (unsigned short) theNode->slotNumber;
      theItem->type = (unsigned short) (joinReference ? OBJ_GET_SLOT_JNVAR1 : OBJ_GET_SLOT_PNVAR1);
      theItem->value = EnvAddBitMap(theEnv,(void *) &hack1,(int) sizeof(struct ObjectMatchVar1));
      return;
     }

   /* =============================================================
      Access a particular field(s) in a multifield slot pattern
      containing at most one multifield variable and at least
      one (or two if no multifield variables) single-field variable
      ============================================================= */
   if (((theNode->type == SF_WILDCARD) || (theNode->type == SF_VARIABLE) || ConstantType(theNode->type)) &&
       ((theNode->multiFieldsBefore == 0) || (theNode->multiFieldsAfter == 0)))
     {
      hack2.whichSlot = (unsigned short) theNode->slotNumber;
      if (theNode->multiFieldsBefore == 0)
        {
         hack2.fromBeginning = 1;
         hack2.beginningOffset = theNode->singleFieldsBefore;
        }
      else
        {
         hack2.fromEnd = 1;
         hack2.endOffset = theNode->singleFieldsAfter;
        }
      theItem->type = (unsigned short) (joinReference ? OBJ_GET_SLOT_JNVAR2 : OBJ_GET_SLOT_PNVAR2);
      theItem->value = EnvAddBitMap(theEnv,(void *) &hack2,sizeof(struct ObjectMatchVar2));
      return;
     }

   if (((theNode->type == MF_WILDCARD) || (theNode->type == MF_VARIABLE) || ConstantType(theNode->type)) &&
       (theNode->multiFieldsBefore == 0) &&
       (theNode->multiFieldsAfter == 0))
     {
      hack2.whichSlot = (unsigned short) theNode->slotNumber;
      hack2.fromBeginning = 1;
      hack2.fromEnd = 1;
      hack2.beginningOffset = theNode->singleFieldsBefore;
      hack2.endOffset = theNode->singleFieldsAfter;
      theItem->type = (unsigned short) (joinReference ? OBJ_GET_SLOT_JNVAR2 : OBJ_GET_SLOT_PNVAR2);
      theItem->value = EnvAddBitMap(theEnv,(void *) &hack2,sizeof(struct ObjectMatchVar2));
      return;
     }

   /* ==================================================
      General slot field access using multifield markers
      ================================================== */
   hack1.whichSlot = (unsigned short) theNode->slotNumber;
   hack1.whichField = (unsigned short) theNode->index;
   theItem->type = (unsigned short) (joinReference ? OBJ_GET_SLOT_JNVAR1 : OBJ_GET_SLOT_PNVAR1);
   theItem->value = EnvAddBitMap(theEnv,(void *) &hack1,sizeof(struct ObjectMatchVar1));
  }

/****************************************************************
  NAME         : IsSimpleSlotVariable
  DESCRIPTION  : Determines if a slot pattern variable
                 references a single-field slot or a single-field
                 in a multifield slot which does not require
                 use of multifield markers
                 (Object addresses are not simple variables)
  INPUTS       : The intermediate parse node
  RETURNS      : TRUE if the variable is simple,
                 FALSE otherwise
  SIDE EFFECTS : None
  NOTES        : None
 ****************************************************************/
static intBool IsSimpleSlotVariable(
  struct lhsParseNode *node)
  {
   if ((node->type == MF_WILDCARD) || (node->type == MF_VARIABLE))
     return(FALSE);
   if ((node->slotNumber < 0) ||
       (node->slotNumber == ISA_ID) ||
       (node->slotNumber == NAME_ID))
     return(FALSE);
   if (node->withinMultifieldSlot == FALSE)
     return(TRUE);
   if (node->multifieldSlot == TRUE)
     return(FALSE);
   if ((node->multiFieldsBefore == 0) || (node->multiFieldsAfter == 0))
     return(TRUE);
   return(FALSE);
  }

/***************************************************************
  NAME         : GenerateSlotComparisonTest
  DESCRIPTION  : Generates pattern and join network
                 expressions for comparing object
                 pattern variables
  INPUTS       : 1) A flag indicating if this is a
                    pattern or join network test
                 2) For a join test, a flag indicating
                    if it is a nand join
                 3) The intermediate parse node
                    for the first variable
                 4) The intermediate parse node
                    for the second variable
  RETURNS      : An expression for comparing the
                 variables
  SIDE EFFECTS : Expression and bitmaps generated
  NOTES        : The following tests are generated
                 for the following scenarios:

                 SF slot w/ SF slot: PN_1 or JN_1
                 Example: (foo ?x) with (bar ?xy)

                 SF slot w/ SF reference in MF slot: PN_2 or JN_2
                 Example: (foo ?x) (bar ? ?x ? ?)

                 SF reference w/ SF reference: PN_3 or JN_3
                 Example: (foo ? ?x ?) and (bar ? ? ? ?x)

                 All other cases: EQ/NEQ general test
                 Example: (foo $? ?x $?) and (bar ?x)
 ***************************************************************/
static EXPRESSION *GenerateSlotComparisonTest(
  void *theEnv,
  int joinTest,
  int isNand,
  struct lhsParseNode *selfNode,
  struct lhsParseNode *referringNode)
  {
   EXPRESSION *theExp;
   struct ObjectCmpPNSingleSlotVars1 phack1;
   struct ObjectCmpPNSingleSlotVars2 phack2;
   struct ObjectCmpPNSingleSlotVars3 phack3;
   struct ObjectCmpJoinSingleSlotVars1 jhack1;
   struct ObjectCmpJoinSingleSlotVars2 jhack2;
   struct ObjectCmpJoinSingleSlotVars3 jhack3;

   /* =========================================================
      If we are comparing two single-field slot variables that
      don't require multifield markers for lookup, use
      a quick comparison.  Otherwise, use a general eq/neq with
      the pattern variable access routines
      ========================================================= */
   if (IsSimpleSlotVariable(selfNode) && IsSimpleSlotVariable(referringNode))
     {
      /* ==============================
         Compare two single-field slots
         ============================== */
      if ((selfNode->withinMultifieldSlot == FALSE) &&
          (referringNode->withinMultifieldSlot == FALSE))
        {
         ClearBitString((void *) &phack1,(int) sizeof(struct ObjectCmpPNSingleSlotVars1));
         ClearBitString((void *) &jhack1,(int) sizeof(struct ObjectCmpJoinSingleSlotVars1));
         if (selfNode->negated)
           phack1.fail = jhack1.fail = 1;
         else
           phack1.pass = jhack1.pass = 1;
         phack1.firstSlot = jhack1.firstSlot = (unsigned short) selfNode->slotNumber;
         phack1.secondSlot = jhack1.secondSlot = (unsigned short) referringNode->slotNumber;
         if (joinTest)
           {
            if (isNand)
              { jhack1.firstPattern = (unsigned short) selfNode->joinDepth; }
            else
              { jhack1.firstPattern = 0; }
            jhack1.firstPatternRHS = TRUE;

            if (isNand && (selfNode->beginNandDepth == referringNode->beginNandDepth))
              { jhack1.secondPatternRHS = TRUE; }
            else
              { jhack1.secondPatternLHS = TRUE; }
            jhack1.secondPattern = (unsigned short) referringNode->joinDepth; 
            
            theExp = GenConstant(theEnv,OBJ_JN_CMP1,EnvAddBitMap(theEnv,(void *) &jhack1,
                                           (int) sizeof(struct ObjectCmpJoinSingleSlotVars1)));
           }
         else
           theExp = GenConstant(theEnv,OBJ_PN_CMP1,EnvAddBitMap(theEnv,(void *) &phack1,
                                           (int) sizeof(struct ObjectCmpPNSingleSlotVars1)));
        }
      /* ============================================
         Compare a single-field slot with a
         single-field in a multifield slot (make sure
         the multifield slot reference is first
         ============================================ */
      else if ((selfNode->withinMultifieldSlot == FALSE) ||
               (referringNode->withinMultifieldSlot == FALSE))
        {
         ClearBitString((void *) &phack2,(int) sizeof(struct ObjectCmpPNSingleSlotVars2));
         ClearBitString((void *) &jhack2,(int) sizeof(struct ObjectCmpJoinSingleSlotVars2));
         if (selfNode->negated)
           phack2.fail = jhack2.fail = 1;
         else
           phack2.pass = jhack2.pass = 1;
         if (selfNode->withinMultifieldSlot == TRUE)
           {
            phack2.firstSlot = jhack2.firstSlot = (unsigned short) selfNode->slotNumber;
            phack2.secondSlot = jhack2.secondSlot = (unsigned short) referringNode->slotNumber;
            if (joinTest)
              {
               if (isNand)
                 { jhack2.firstPattern = (unsigned short) selfNode->joinDepth; }
               else
                 { jhack2.firstPattern = 0; }
               jhack2.firstPatternRHS = TRUE;

               if (isNand && (selfNode->beginNandDepth == referringNode->beginNandDepth))
                 { jhack2.secondPatternRHS = TRUE; }
               else
                 { jhack2.secondPatternLHS = TRUE; }
               jhack2.secondPattern = (unsigned short) referringNode->joinDepth; 
              }
            if (selfNode->multiFieldsBefore == 0)
              {
               phack2.fromBeginning = jhack2.fromBeginning = 1;
               phack2.offset = jhack2.offset = selfNode->singleFieldsBefore;
              }
            else
              phack2.offset = jhack2.offset = selfNode->singleFieldsAfter;
           }
         else
           {
            phack2.firstSlot = jhack2.firstSlot = (unsigned short) referringNode->slotNumber;
            phack2.secondSlot = jhack2.secondSlot = (unsigned short) selfNode->slotNumber;
            if (joinTest)
              {
               if (isNand)
                 { jhack2.secondPattern = (unsigned short) selfNode->joinDepth; }
               else
                 { jhack2.secondPattern = 0; }
               jhack2.secondPatternRHS = TRUE;

               if (isNand && (selfNode->beginNandDepth == referringNode->beginNandDepth))
                 { jhack2.firstPatternRHS = TRUE; }
               else
                 { jhack2.firstPatternLHS = TRUE; }
               jhack2.firstPattern = (unsigned short) referringNode->joinDepth; 
              }
            if (referringNode->multiFieldsBefore == 0)
              {
               phack2.fromBeginning = jhack2.fromBeginning = 1;
               phack2.offset = jhack2.offset = referringNode->singleFieldsBefore;
              }
            else
              phack2.offset = jhack2.offset = referringNode->singleFieldsAfter;
           }
         if (joinTest)
           theExp = GenConstant(theEnv,OBJ_JN_CMP2,EnvAddBitMap(theEnv,(void *) &jhack2,
                                           (int) sizeof(struct ObjectCmpJoinSingleSlotVars2)));
         else
           theExp = GenConstant(theEnv,OBJ_PN_CMP2,EnvAddBitMap(theEnv,(void *) &phack2,
                                           (int) sizeof(struct ObjectCmpPNSingleSlotVars2)));
        }

      /* ===================================
         Compare two single-field references
         within multifield slots
         =================================== */
      else
        {
         ClearBitString((void *) &phack3,(int) sizeof(struct ObjectCmpPNSingleSlotVars3));
         ClearBitString((void *) &jhack3,(int) sizeof(struct ObjectCmpJoinSingleSlotVars3));
         if (selfNode->negated)
           phack3.fail = jhack3.fail = 1;
         else
           phack3.pass = jhack3.pass = 1;
         phack3.firstSlot = jhack3.firstSlot = (unsigned short) selfNode->slotNumber;
         phack3.secondSlot = jhack3.secondSlot = (unsigned short) referringNode->slotNumber;
         if (selfNode->multiFieldsBefore == 0)
           {
            phack3.firstFromBeginning = jhack3.firstFromBeginning = 1;
            phack3.firstOffset = jhack3.firstOffset = selfNode->singleFieldsBefore;
           }
         else
           phack3.firstOffset = jhack3.firstOffset = selfNode->singleFieldsAfter;
         if (referringNode->multiFieldsBefore == 0)
           {
            phack3.secondFromBeginning = jhack3.secondFromBeginning = 1;
            phack3.secondOffset = jhack3.secondOffset = referringNode->singleFieldsBefore;
           }
         else
           phack3.secondOffset = jhack3.secondOffset = referringNode->singleFieldsAfter;
         if (joinTest)
           {
            if (isNand)
              { jhack3.firstPattern = (unsigned short) selfNode->joinDepth; }
            else
              { jhack3.firstPattern = 0; }
            jhack3.firstPatternRHS = TRUE;

            if (isNand && (selfNode->beginNandDepth == referringNode->beginNandDepth))
              { jhack3.secondPatternRHS = TRUE; }
            else
              { jhack3.secondPatternLHS = TRUE; }
            jhack3.secondPattern = (unsigned short) referringNode->joinDepth; 

            theExp = GenConstant(theEnv,OBJ_JN_CMP3,EnvAddBitMap(theEnv,(void *) &jhack3,
                                         (int) sizeof(struct ObjectCmpJoinSingleSlotVars3)));
           }
         else
           theExp = GenConstant(theEnv,OBJ_PN_CMP3,EnvAddBitMap(theEnv,(void *) &phack3,
                                           (int) sizeof(struct ObjectCmpPNSingleSlotVars3)));
        }
     }

   /* ==================================================
      General comparison for multifield slot references,
      references which require multifield markers, and
      object addresses
      ================================================== */
   else
     {
      theExp = GenConstant(theEnv,FCALL,selfNode->negated ? ExpressionData(theEnv)->PTR_NEQ : ExpressionData(theEnv)->PTR_EQ);
      theExp->argList = GenConstant(theEnv,0,NULL);
      
      if (isNand)
        { GenObjectGetVar(theEnv,joinTest,theExp->argList,selfNode,NESTED_RHS); }
      else
        { GenObjectGetVar(theEnv,joinTest,theExp->argList,selfNode,RHS); }
      
      theExp->argList->nextArg = GenConstant(theEnv,0,NULL);
      
      if (isNand && (selfNode->beginNandDepth == referringNode->beginNandDepth))
        { GenObjectGetVar(theEnv,joinTest,theExp->argList->nextArg,referringNode,NESTED_RHS); }
      else
        { GenObjectGetVar(theEnv,joinTest,theExp->argList->nextArg,referringNode,LHS); }
     }
   return(theExp);
  }

#endif

/***************************************************
  NAME         :
  DESCRIPTION  :
  INPUTS       :
  RETURNS      :
  SIDE EFFECTS :
  NOTES        :
 ***************************************************/
