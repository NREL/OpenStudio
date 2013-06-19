   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.20  01/31/02          */
   /*                                                     */
   /*             MULTIPLE INHERITANCE PARSER MODULE      */
   /*******************************************************/

/**************************************************************/
/* Purpose: Parsing Routines for Multiple Inheritance         */
/*                                                            */
/* Principal Programmer(s):                                   */
/*      Brian L. Dantes                                       */
/*                                                            */
/* Contributing Programmer(s):                                */
/*                                                            */
/* Revision History:                                          */
/*                                                            */
/**************************************************************/

/* =========================================
   *****************************************
               EXTERNAL DEFINITIONS
   =========================================
   ***************************************** */
#include "setup.h"

#if OBJECT_SYSTEM && (! BLOAD_ONLY) && (! RUN_TIME)

#include "classcom.h"
#include "classfun.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "modulutl.h"
#include "router.h"
#include "scanner.h"

#define _INHERPSR_SOURCE_
#include "inherpsr.h"

/* =========================================
   *****************************************
               MACROS AND TYPES
   =========================================
   ***************************************** */
typedef struct partialOrder PARTIAL_ORDER;
typedef struct successor SUCCESSOR;

struct partialOrder
  {
   DEFCLASS *cls;
   unsigned pre;
   SUCCESSOR *suc;
   struct partialOrder *nxt;
  };

struct successor
  {
   PARTIAL_ORDER *po;
   struct successor *nxt;
  };

/* =========================================
   *****************************************
      INTERNALLY VISIBLE FUNCTION HEADERS
   =========================================
   ***************************************** */

static PARTIAL_ORDER *InitializePartialOrderTable(void *,PARTIAL_ORDER *,PACKED_CLASS_LINKS *);
static void RecordPartialOrders(void *,PARTIAL_ORDER *,DEFCLASS *,PACKED_CLASS_LINKS *,long);
static PARTIAL_ORDER *FindPartialOrder(PARTIAL_ORDER *,DEFCLASS *);
static void PrintPartialOrderLoop(void *,PARTIAL_ORDER *);
static void PrintClassLinks(void *,char *,char *,CLASS_LINK *);

/* =========================================
   *****************************************
          EXTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/**************************************************************
  NAME         : ParseSuperclasses
  DESCRIPTION  : Parses the (is-a <superclass>+) portion of
                 the (defclass ...) construct and returns
                 a list of direct superclasses.  The
                 class "standard-class" is the precedence list
                 for classes with no direct superclasses.
                 The final precedence list (not calculated here)
                 will have the class in question first followed
                 by the merged precedence lists of its direct
                 superclasses.
  INPUTS       : 1) The logical name of the input source
                 2) The symbolic name of the new class
  RETURNS      : The address of the superclass list
                  or NULL if there was an error
  SIDE EFFECTS : None
  NOTES        : Assumes "(defclass <name> [<comment>] ("
                 has already been scanned.

                 All superclasses must be defined before
                 their subclasses.  Duplicates in the (is-a
                 ...) list are are not allowed (a class may only
                 inherits from a superclass once).

                 This routine also checks the class-precedence
                 lists of each of the direct superclasses for
                 an occurrence of the new class - i.e. cycles!
                 This can only happen when a class is redefined
                 (a new class cannot have an unspecified
                 superclass).

                 This routine allocates the space for the list
 ***************************************************************/
globle PACKED_CLASS_LINKS *ParseSuperclasses(
  void *theEnv,
  char *readSource,
  SYMBOL_HN *newClassName)
  {
   CLASS_LINK *clink = NULL,*cbot = NULL,*ctmp;
   DEFCLASS *sclass;
   PACKED_CLASS_LINKS *plinks;

   if (GetType(DefclassData(theEnv)->ObjectParseToken) != LPAREN)
     {
      SyntaxErrorMessage(theEnv,"defclass inheritance");
      return(NULL);
     }
   GetToken(theEnv,readSource,&DefclassData(theEnv)->ObjectParseToken);
   if ((GetType(DefclassData(theEnv)->ObjectParseToken) != SYMBOL) ? TRUE :
       (DefclassData(theEnv)->ObjectParseToken.value != (void *) DefclassData(theEnv)->ISA_SYMBOL))
     {
      SyntaxErrorMessage(theEnv,"defclass inheritance");
      return(NULL);
     }
   SavePPBuffer(theEnv," ");
   GetToken(theEnv,readSource,&DefclassData(theEnv)->ObjectParseToken);
   while (GetType(DefclassData(theEnv)->ObjectParseToken) != RPAREN)
     {
      if (GetType(DefclassData(theEnv)->ObjectParseToken) != SYMBOL)
        {
         SyntaxErrorMessage(theEnv,"defclass");
         goto SuperclassParseError;
        }
      if (FindModuleSeparator(ValueToString(newClassName)))
        {
         IllegalModuleSpecifierMessage(theEnv);
         goto SuperclassParseError;
        }
      if (GetValue(DefclassData(theEnv)->ObjectParseToken) == (void *) newClassName)
        {
         PrintErrorID(theEnv,"INHERPSR",1,FALSE);
         EnvPrintRouter(theEnv,WERROR,"A class may not have itself as a superclass.\n");
         goto SuperclassParseError;
        }
      for (ctmp = clink ; ctmp != NULL ; ctmp = ctmp->nxt)
        {
         if (GetValue(DefclassData(theEnv)->ObjectParseToken) == (void *) ctmp->cls->header.name)
           {
            PrintErrorID(theEnv,"INHERPSR",2,FALSE);
            EnvPrintRouter(theEnv,WERROR,"A class may inherit from a superclass only once.\n");
            goto SuperclassParseError;
           }
        }
      sclass = LookupDefclassInScope(theEnv,ValueToString(GetValue(DefclassData(theEnv)->ObjectParseToken)));
      if (sclass == NULL)
        {
         PrintErrorID(theEnv,"INHERPSR",3,FALSE);
         EnvPrintRouter(theEnv,WERROR,"A class must be defined after all its superclasses.\n");
         goto SuperclassParseError;
        }
      if ((sclass == DefclassData(theEnv)->PrimitiveClassMap[INSTANCE_NAME]) ||
          (sclass == DefclassData(theEnv)->PrimitiveClassMap[INSTANCE_ADDRESS]) ||
          (sclass == DefclassData(theEnv)->PrimitiveClassMap[INSTANCE_NAME]->directSuperclasses.classArray[0]))
        {
         PrintErrorID(theEnv,"INHERPSR",6,FALSE);
         EnvPrintRouter(theEnv,WERROR,"A user-defined class cannot be a subclass of ");
         EnvPrintRouter(theEnv,WERROR,EnvGetDefclassName(theEnv,(void *) sclass));
         EnvPrintRouter(theEnv,WERROR,".\n");
         goto SuperclassParseError;
        }
      ctmp = get_struct(theEnv,classLink);
      ctmp->cls = sclass;
      if (clink == NULL)
        clink = ctmp;
      else
        cbot->nxt = ctmp;
      ctmp->nxt = NULL;
      cbot = ctmp;

      SavePPBuffer(theEnv," ");
      GetToken(theEnv,readSource,&DefclassData(theEnv)->ObjectParseToken);
     }
   if (clink == NULL)
     {
      PrintErrorID(theEnv,"INHERPSR",4,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Must have at least one superclass.\n");
      return(NULL);
     }
   PPBackup(theEnv);
   PPBackup(theEnv);
   SavePPBuffer(theEnv,")");
   plinks = get_struct(theEnv,packedClassLinks);
   PackClassLinks(theEnv,plinks,clink);
   return(plinks);

SuperclassParseError:
   DeleteClassLinks(theEnv,clink);
   return(NULL);
  }

/***************************************************************************
  NAME         : FindPrecedenceList
  DESCRIPTION  : A complete class precedence list is obtained from the
                 list of direct superclasses as follows :

                 Each class and its direct superclasses are recursively
                 entered in order to a list called the partial order table.
                 A class is only entered once.  The order reflects a pre-order
                 depth-first traversal of the classes, and this order will be
                 followed as closely as possible to preserve the "family"
                 heuristic when constructing the class precedence list.

                 Attached to each node is a count indicating the number of
                 classes which must precede this class and a list of classes
                 which must succeed this class (attached via the suc field and
                 linked via nxt fields).  These predecessor counts
                 and successor lists indicate the partial orderings given
                 by the rules of multiple inheritance for the classes:
                 1) a class must precede all its superclasses, and 2) a
                 class determines the precedence of its immediate superclasses.

                 For example, the following class definitions

                 (defclass A (is-a USER))
                 (defclass B (is-a USER))
                 (defclass C (is-a A B))

                 would give the following partial orders:

                 C < A          by Rule 1
                 C < B          by Rule 1
                 A < B          by Rule 2
                 B < USER       by Rule 1
                 A < USER       by Rule 1
                 USER < OBJECT  by Rule 1

                 In turn, these partial orders would be recorded in a
                 sequence table:

                                     C     A      USER    OBJECT    B
                 Predecessor Count   0     1       2        1       2
                 Successor List     A,B  B,USER  OBJECT   <NIL>    USER

                 To generate a precedence list for C, we pick the first
                 class with a predecessor count of 0, append it to the
                 precedence list, and decrement the counts of all its
                 successors.  We continue scanning for a 0 from where
                 we left off.  If we ever scan completely through the
                 table without finding a 0, then we know there is an
                 error.

                 Shown below is the table above after each class is
                 entered onto the precedence list:

                 Precedence list: C
                                     A      USER    OBJECT    B
                 Predecessor Count   0       2        1       1
                 Successor List     B,USER  OBJECT   <NIL>    USER

                 Precedence list: C A
                                    USER    OBJECT    B
                 Predecessor Count   1        1       0
                 Successor List     OBJECT   <NIL>   USER

                 Precedence list: C A B
                                    USER    OBJECT
                 Predecessor Count   0        1
                 Successor List     OBJECT   <NIL>

                 Precedence list: C A B USER
                                    OBJECT
                 Predecessor Count   0
                 Successor List     <NIL>

                 Precedence List: C A B USER OBJECT

                 And since the table is now empty we are done!
  INPUTS       : 1) The old class definition (NULL if it is new)
                 2) The list of direct superclasses
  RETURNS      : The address of the precedence list if successful,
                 NULL otherwise
  SIDE EFFECTS : Precedence list allocated
  NOTES        : WARNING!! - This routine assumes that there are no
                 cyclic dependencies in the given superclass list, i.e.
                 none of the superclasses inherit from the class for
                 which the precedence list is being defined.  (This
                 is verified in ParseDefclasses() in CLASSCOM.C)

                 Every class-precedence list has the class itself on it
                 (implicitly) and a built-in system class on it explicitly
                 (except for the built-in classes).

                 The precedence determination algorithm is a variation on
                 the topological sorting algorithm given in The Art of
                 Computer Programming - Vol. I (Fundamental Algorithms) by
                 Donald Knuth.
 ***************************************************************************/
globle PACKED_CLASS_LINKS *FindPrecedenceList(
  void *theEnv,
  DEFCLASS *cls,
  PACKED_CLASS_LINKS *supers)
  {
   PARTIAL_ORDER *po_table = NULL,*start,*pop,*poprv,*potmp;
   SUCCESSOR *stmp;
   CLASS_LINK *ptop,*pbot,*ptmp;
   PACKED_CLASS_LINKS *plinks;
   long i;

   /* =====================================================================
      Recursively add all superclasses in a pre-order depth-first traversal
      to the partial order table.  There should be only one node per class.
      ===================================================================== */
   po_table = InitializePartialOrderTable(theEnv,po_table,supers);

   /* =============================================================
      If the class already exists, record the rule 1 partial orders
      with the new superclass lists.  This is so that cyclic
      dependencies can be detected.
      ============================================================= */
   if (cls != NULL)
     {
      pop = get_struct(theEnv,partialOrder);
      pop->cls = cls;
      pop->pre = 0;
      pop->suc = NULL;
      pop->nxt = po_table;
      po_table = pop;
      pop = po_table->nxt;
      RecordPartialOrders(theEnv,po_table,cls,supers,0);
     }
   else
     pop = po_table;

   /* ==================================================================
      Record the rule 1 and rule 2 partial orders given by the direct
      superclass lists of the classes in the table.  There is no need to
      recurse since all possible classes have been entered already.

      Be sure to skip the class itself if it was added to the front of
      the table.
      ================================================================== */
   for ( ; pop != NULL ; pop = pop->nxt)
     {
      RecordPartialOrders(theEnv,po_table,pop->cls,&pop->cls->directSuperclasses,0);
      for (i = 0 ; i < pop->cls->directSuperclasses.classCount ; i++)
        RecordPartialOrders(theEnv,po_table,pop->cls->directSuperclasses.classArray[i],
                            &pop->cls->directSuperclasses,i+1);
     }

   /* =============================================================
      Record the rule 2 partial orders given by the superclass list
      ============================================================= */
   for (i = 0 ; i < supers->classCount ; i++)
     RecordPartialOrders(theEnv,po_table,supers->classArray[i],supers,i+1);

   start = NULL;
   poprv = NULL;
   pop = po_table;
   ptop = pbot = NULL;
   while (pop != start)
     {
      /* ==============================================================
         Allow wraparound - happens when the search for a 0 node begins
         somewhere in the middle of the sequence table
         ============================================================== */
      if (pop == NULL)
        {
         poprv = NULL;
         pop = po_table;
         start = start->nxt;
        }

      /* =========================================================
         Search for the first class with no remaining predecessors
         ========================================================= */
      if (pop->pre == 0)
        {
         /* =================================================
            Decrement the predecessor count for all the
            successors of this class and delete the list.

            This is the variation on Knuth's algorithm which
            allows us to preserve the "family" heuristic.
            Since we will pick up scanning for 0's from
            this point, we will be able to keep "family"
            trees together, if possible. BuildPartialOrders()
            entered the classes into the sequence table
            in a pre-order depth traversal order.
            ================================================= */
         while (pop->suc != NULL)
           {
            stmp = pop->suc;
            pop->suc = stmp->nxt;
            stmp->po->pre--;
            rtn_struct(theEnv,successor,stmp);
           }

         /* =============================================
            Append the class to the precedence list and
            remove its entry from the partial order table
            ============================================= */
         potmp = pop;
         if (poprv == NULL)
           po_table = pop->nxt;
         else
           poprv->nxt = pop->nxt;
         pop = pop->nxt;
         start = poprv;
         ptmp = get_struct(theEnv,classLink);
         ptmp->cls = potmp->cls;
         ptmp->nxt = NULL;
         rtn_struct(theEnv,partialOrder,potmp);
         if (ptop == NULL)
           ptop = ptmp;
         else
           pbot->nxt = ptmp;
         pbot = ptmp;
        }
      else
        {
         poprv = pop;
         pop = pop->nxt;
        }
     }

   /* ======================================================================
      If the table of partial orders is not empty and we were unable to find
      a class with no predecessors, then there is no solution! Print out the
      precedence loop in the partial orders. Delete the remaining partial
      order table and the partial precedence list.
      ====================================================================== */
   if (po_table != NULL)
     {
      PrintErrorID(theEnv,"INHERPSR",5,FALSE);
      PrintClassLinks(theEnv,WERROR,"Partial precedence list formed:",ptop);
      PrintPartialOrderLoop(theEnv,po_table);
      while (po_table != NULL)
        {
         while (po_table->suc != NULL)
           {
            stmp = po_table->suc;
            po_table->suc = stmp->nxt;
            rtn_struct(theEnv,successor,stmp);
           }
         potmp = po_table;
         po_table = po_table->nxt;
         rtn_struct(theEnv,partialOrder,potmp);
        }
      DeleteClassLinks(theEnv,ptop);
      return(NULL);
     }

   /* =============================================================================
      If the class already existed, be sure and remove it from its own precedence
      list.  Remember that we stuck it on the table artificially to catch cycles.
      It was first in the table, and, since it started with a predecessor count
      of zero (given that there were no loops), it is first in the precedence list.

      We will leave the dummy node there so that functions which wish to iterate
      over a class and its superclasses may easily do so.
      ============================================================================= */
   if (cls == NULL)
     {
      ptmp = get_struct(theEnv,classLink);
      ptmp->nxt = ptop;
      ptop = ptmp;
     }

   /* ============================================================
      The class pointer will be filled in later by ParseDefclass()
      ============================================================ */
   ptop->cls = NULL;

   plinks = get_struct(theEnv,packedClassLinks);
   PackClassLinks(theEnv,plinks,ptop);
   return(plinks);
  }

/***************************************************
  NAME         : PackClassLinks
  DESCRIPTION  : Writes a list of class links into
                 a contiguous section of memory to
                 reduce overhead (the original list
                 is deleted)
  INPUTS       : 1) The packed list structure to use
                 2) The top of the original list
  RETURNS      : Nothing useful
  SIDE EFFECTS : Packed list allocated and old list
                 deleted
  NOTES        : None
 ***************************************************/
globle void PackClassLinks(
  void *theEnv,
  PACKED_CLASS_LINKS *plinks,
  CLASS_LINK *lptop)
  {
   register unsigned count;
   register CLASS_LINK *lp;

   for (count = 0 , lp = lptop ; lp != NULL ; lp = lp->nxt)
     count++;
   if (count > 0)
     plinks->classArray = (DEFCLASS **) gm2(theEnv,(sizeof(DEFCLASS *) * count));
   else
     plinks->classArray = NULL;
   for (count = 0 , lp = lptop ; lp != NULL ; lp = lp->nxt , count++)
     plinks->classArray[count] = lp->cls;
   DeleteClassLinks(theEnv,lptop);
   plinks->classCount = (unsigned short) count;
  }

/* =========================================
   *****************************************
          INTERNALLY VISIBLE FUNCTIONS
   =========================================
   ***************************************** */

/**************************************************************************
  NAME         : InitializePartialOrderTable
  DESCRIPTION  : This function recursively enters the classes
                 that will be used in a precedence list
                 determination in depth-first pre-order traversal.
                 The predecessor counts and successor list are initialized.

  INPUTS       : 1) The partial order table
                 2) A list of direct superclasses
                 3) The class for which a precedence class is being
                    determined (NULL for new class)
                 4) The class which superclass list is being processed
  RETURNS      : The top of partial order table
  SIDE EFFECTS : The partial order table is initialized.
  NOTES        : None
 **************************************************************************/
static PARTIAL_ORDER *InitializePartialOrderTable(
  void *theEnv,
  PARTIAL_ORDER *po_table,
  PACKED_CLASS_LINKS *supers)
  {
   register PARTIAL_ORDER *pop,*poprv;
   long i;

   for (i = 0 ; i < supers->classCount ; i++)
     {
      /* =================================================
         Append this class at the end of the partial order
         table only if it is not already present
         ================================================= */
      poprv = NULL;
      for (pop = po_table ; pop != NULL ; pop = pop->nxt)
        {
         if (pop->cls == supers->classArray[i])
           break;
         poprv = pop;
        }
      if (pop == NULL)
        {
         pop = get_struct(theEnv,partialOrder);
         pop->cls = supers->classArray[i];
         pop->nxt = NULL;
         pop->suc = NULL;
         pop->pre = 0;
         if (poprv == NULL)
           po_table = pop;
         else
           poprv->nxt = pop;

         /* =============================================================
            Recursively append all its superclasses immediately after it.
            This order will allow us to preserve the "family" heuristic
            in the precedence list.
            ============================================================= */
         po_table = InitializePartialOrderTable(theEnv,po_table,
                     &supers->classArray[i]->directSuperclasses);
        }
     }
   return(po_table);
  }

/***********************************************************************************
  NAME         : RecordPartialOrders
  DESCRIPTION  : Given a predecessor class and a list of successor classes, this
                 function enters a number of partial orders into the table equaling
                 the number of successor classes.
  INPUTS       : 1) The partial order table
                 2) The predecessor class
                 3) An array of successor classes
                 4) A starting index for the successor classes
  RETURNS      : The top of sequence table
  SIDE EFFECTS : The sequence table is built, e.g.:

                 CLASS1 < CLASS2 , CLASS3

                 would be recorded as:

                 PO_TABLE -> NXT     -> NXT       -> NXT ->   <NIL>
                             <CLASS1>   <CLASS2>     <CLASS3>
                             SUC        SUC          SUC
                              |          |            |
                              V          V            V
                             <CLASS2>   <NIL>        <NIL>
                             NXT
                              |
                              V
                             <CLASS3>
                             NXT
                              |
                              V
                             <NIL>

                 The predecessor counts would be 0, 1 and 1 for CLASS1, CLASS2
                 and CLASS3 respectively.
  NOTES        : None
 ***********************************************************************************/
static void RecordPartialOrders(
  void *theEnv,
  PARTIAL_ORDER *po_table,
  DEFCLASS *cls,
  PACKED_CLASS_LINKS *successors,
  long starti)
  {
   register PARTIAL_ORDER *clspo;
   register SUCCESSOR *stmp;

   clspo = FindPartialOrder(po_table,cls);
   while (starti < successors->classCount)
     {
      stmp = get_struct(theEnv,successor);
      stmp->po = FindPartialOrder(po_table,successors->classArray[starti]);
      stmp->nxt = clspo->suc;
      clspo->suc = stmp;
      stmp->po->pre++;
      starti++;
     }
  }

/***************************************************
  NAME         : FindPartialOrder
  DESCRIPTION  : Finds a partial order node
  INPUTS       : 1) The partial order table
                 2) The class to look up
  RETURNS      : The partial order node address
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
static PARTIAL_ORDER *FindPartialOrder(
  PARTIAL_ORDER *po_table,
  DEFCLASS *cls)
  {
   while (po_table != NULL)
     {
      if (po_table->cls == cls)
        break;
      po_table = po_table->nxt;
     }
   return(po_table);
  }

/**************************************************************************
  NAME         : PrintPartialOrderLoop
  DESCRIPTION  : This routine prints a conflicting loop (there may be more
                 than one) in the given sequence table of partial orders.

                 The algorithm works as follows:

                 Given the following class definitions,

                 (defclass A (is-a USER))
                 (defclass B (is-a USER))
                 (defclass C (is-a A B))
                 (defclass D (is-a B A))
                 (defclass E (is-a C D))

                 the partial order table will look as follows after as many
                 classes as possible have been entered onto the precedence
                 list:

                                      A      USER    OBJECT   B
                 Predecessor Count    1       2        1      1
                 Successor List     B,USER  OBJECT   <NIL>   A,USER

                 Construct a new table where each class is linked to one
                 of its predecessors.  For the example above one would be:

                 Class:            A    USER    OBJECT    B
                 Predecessor:      B      A      USER     A

                 This table is actually implemnted using the original
                 partial order table (see the code below for specifics).
                 Now using this table, start with the first node, and visit
                 successive nodes by following the predecessor links.  Mark
                 each node as "visited".  When a previously visited node is
                 encountered, the loop has been found.

                 In the case above, we start with A, goto B and then goto A
                 again which we have already seen.  So starting from where
                 we found the loop (A) we follow the links again, printing
                 the nodes as we go, until we're back where we started:
                 A B A.  Notice that this loop reflects the Rule 2 conflicts
                 between Class C and Class D in Class E's precedence list.

  INPUTS       : The remaining partial order table of conflicting partial
                 orders
  RETURNS      : Nothing useful
  SIDE EFFECTS : The predecessor counts and successor lists are modified to
                 implement the loop detection.
  NOTES        : This algorithm is adopted from one  given in Donald
                 Knuth's The Art of Computer Programming - Vol. I
                 (Fundamental Algorithms).
 **************************************************************************/
static void PrintPartialOrderLoop(
  void *theEnv,
  PARTIAL_ORDER *po_table)
  {
   register PARTIAL_ORDER *pop1,*pop2;
   SUCCESSOR *prc,*stmp;

   /* ====================================================
      Set the predecessor count of every node to 0 so that
      this field can be used as a marker
      ==================================================== */
   for (pop1 = po_table ; pop1 != NULL ; pop1 = pop1->nxt)
     pop1->pre = 0;

   /* =======================================================
      Mark each node in the partial order table with one of
      its predecessors.  If the class has already been marked
      (predecessor count > 0), don't bother.  This is
      accomplished by adding a node to the front of its
      successors' successor lists.

      When the process is finished, all nodes will have one
      predecessor chained to them by their 'suc' field.
      (If any nodes had had no predecessors, they would not
       still be in the table.)
      ======================================================= */
   for (pop1 = po_table ; pop1 != NULL ; pop1 = pop1->nxt)
     {
      if (pop1->pre == 0)
        {
         prc = pop1->suc;
         pop1->suc = NULL;
        }
      else
        {
         prc = pop1->suc->nxt;
         pop1->suc->nxt = NULL;
        }
      while (prc != NULL)
        {
         pop2 = FindPartialOrder(po_table,prc->po->cls);
         if (pop2->pre == 0)
           {
            stmp = get_struct(theEnv,successor);
            stmp->po = pop1;
            stmp->nxt = pop2->suc;
            pop2->suc = stmp;
            pop2->pre = 1;
           }
         stmp = prc;
         prc = prc->nxt;
         rtn_struct(theEnv,successor,stmp);
        }
     }

   /* =================================================
      Set the predecessor count of every node back to 0
      so that this field can be used as a marker again
      ================================================= */
   for (pop1 = po_table ; pop1 != NULL ; pop1 = pop1->nxt)
     pop1->pre = 0;

   /* =========================================================
      Now start with the first node in the partial order table,
      and follow the predecessor links, marking the
      nodes as they are visited.  When we reach a node
      we have been before, we have found a loop!
      Follow all the marked nodes again  starting from the
      CURRENT position to print the loop.
      ========================================================= */
   pop1 = po_table;
   while (pop1->pre == 0)
     {
      pop1->pre = 1;
      pop1 = pop1->suc->po;
     }

   EnvPrintRouter(theEnv,WERROR,"Precedence loop in superclasses:");
   while (pop1->pre == 1)
     {
      EnvPrintRouter(theEnv,WERROR," ");
      PrintClassName(theEnv,WERROR,pop1->cls,FALSE);
      pop1->pre = 0;
      pop1 = pop1->suc->po;
     }
   EnvPrintRouter(theEnv,WERROR," ");
   PrintClassName(theEnv,WERROR,pop1->cls,TRUE);
  }

/***************************************************
  NAME         : PrintClassLinks
  DESCRIPTION  : Displays the names of classes in
                 a list with a title
  INPUTS       : 1) The logical name of the output
                 2) Title string
                 3) List of class links
  RETURNS      : Nothing useful
  SIDE EFFECTS : None
  NOTES        : None
 ***************************************************/
static void PrintClassLinks(
  void *theEnv,
  char *logicalName,
  char *title,
  CLASS_LINK *clink)
  {
   if (title != NULL)
     EnvPrintRouter(theEnv,logicalName,title);
   while (clink != NULL)
     {
      EnvPrintRouter(theEnv,logicalName," ");
      PrintClassName(theEnv,logicalName,clink->cls,FALSE);
      clink = clink->nxt;
     }
   EnvPrintRouter(theEnv,logicalName,"\n");
  }

#endif

