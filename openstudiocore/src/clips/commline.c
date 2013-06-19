   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*                COMMAND LINE MODULE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides a set of routines for processing        */
/*   commands entered at the top level prompt.               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Refactored several functions and added         */
/*            additional functions for use by an interface   */
/*            layered on top of CLIPS.                       */
/*                                                           */
/*      6.30: Local variables set with the bind function     */
/*            persist until a reset/clear command is issued. */
/*                                                           */
/*************************************************************/

#define _COMMLINE_SOURCE_

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <string.h>
#include <ctype.h>

#include "setup.h"
#include "constant.h"

#include "argacces.h"
#include "constrct.h"
#include "cstrcpsr.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "filecom.h"
#include "memalloc.h"
#include "prcdrfun.h"
#include "prcdrpsr.h"
#include "router.h"
#include "scanner.h"
#include "strngrtr.h"
#include "symbol.h"
#include "sysdep.h"
#include "utility.h"

#include "commline.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if ! RUN_TIME
   static int                     DoString(char *,int,int *);
   static int                     DoComment(char *,int);
   static int                     DoWhiteSpace(char *,int);
   static int                     DefaultGetNextEvent(void *);
#endif
   static void                    DeallocateCommandLineData(void *);

/****************************************************/
/* InitializeCommandLineData: Allocates environment */
/*    data for command line functionality.          */
/****************************************************/
globle void InitializeCommandLineData(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,COMMANDLINE_DATA,sizeof(struct commandLineData),DeallocateCommandLineData);

#if ! RUN_TIME   
   CommandLineData(theEnv)->BannerString = BANNER_STRING;
   CommandLineData(theEnv)->EventFunction = DefaultGetNextEvent;
#endif
  }
  
/*******************************************************/
/* DeallocateCommandLineData: Deallocates environment */
/*    data for the command line functionality.        */
/******************************************************/
static void DeallocateCommandLineData(
  void *theEnv)
  {
#if ! RUN_TIME
   if (CommandLineData(theEnv)->CommandString != NULL) 
     { rm(theEnv,CommandLineData(theEnv)->CommandString,CommandLineData(theEnv)->MaximumCharacters); }
     
   if (CommandLineData(theEnv)->CurrentCommand != NULL) 
     { ReturnExpression(theEnv,CommandLineData(theEnv)->CurrentCommand); }
#else
#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }

#if ! RUN_TIME

/***************************************************/
/* ExpandCommandString: Appends a character to the */
/*   command string. Returns TRUE if the command   */
/*   string was successfully expanded, otherwise   */
/*   FALSE. Expanding the string also includes     */
/*   adding a backspace character which reduces    */
/*   string's length.                              */
/***************************************************/
globle int ExpandCommandString(
  void *theEnv,
  int inchar)
  {
   size_t k;

   k = RouterData(theEnv)->CommandBufferInputCount;
   CommandLineData(theEnv)->CommandString = ExpandStringWithChar(theEnv,inchar,CommandLineData(theEnv)->CommandString,&RouterData(theEnv)->CommandBufferInputCount,
                                        &CommandLineData(theEnv)->MaximumCharacters,CommandLineData(theEnv)->MaximumCharacters+80);
   return((RouterData(theEnv)->CommandBufferInputCount != k) ? TRUE : FALSE);
  }

/******************************************************************/
/* FlushCommandString: Empties the contents of the CommandString. */
/******************************************************************/
globle void FlushCommandString(
  void *theEnv)
  {
   if (CommandLineData(theEnv)->CommandString != NULL) rm(theEnv,CommandLineData(theEnv)->CommandString,CommandLineData(theEnv)->MaximumCharacters);
   CommandLineData(theEnv)->CommandString = NULL;
   CommandLineData(theEnv)->MaximumCharacters = 0;
   RouterData(theEnv)->CommandBufferInputCount = 0;
   RouterData(theEnv)->AwaitingInput = TRUE;
  }

/*********************************************************************************/
/* SetCommandString: Sets the contents of the CommandString to a specific value. */
/*********************************************************************************/
globle void SetCommandString(
  void *theEnv,
  char *str)
  {
   size_t length;

   FlushCommandString(theEnv);
   length = strlen(str);
   CommandLineData(theEnv)->CommandString = (char *)
                   genrealloc(theEnv,CommandLineData(theEnv)->CommandString,(unsigned) CommandLineData(theEnv)->MaximumCharacters,
                              (unsigned) CommandLineData(theEnv)->MaximumCharacters + length + 1);

   genstrcpy(CommandLineData(theEnv)->CommandString,str);
   CommandLineData(theEnv)->MaximumCharacters += (length + 1);
   RouterData(theEnv)->CommandBufferInputCount += (int) length;
  }

/*************************************************************/
/* SetNCommandString: Sets the contents of the CommandString */
/*   to a specific value up to N characters.                 */
/*************************************************************/
globle void SetNCommandString(
  void *theEnv,
  char *str,
  unsigned length)
  {
   FlushCommandString(theEnv);
   CommandLineData(theEnv)->CommandString = (char *)
                   genrealloc(theEnv,CommandLineData(theEnv)->CommandString,(unsigned) CommandLineData(theEnv)->MaximumCharacters,
                              (unsigned) CommandLineData(theEnv)->MaximumCharacters + length + 1);

   genstrncpy(CommandLineData(theEnv)->CommandString,str,length);
   CommandLineData(theEnv)->CommandString[CommandLineData(theEnv)->MaximumCharacters + length] = 0;
   CommandLineData(theEnv)->MaximumCharacters += (length + 1);
   RouterData(theEnv)->CommandBufferInputCount += (int) length;
  }

/******************************************************************************/
/* AppendCommandString: Appends a value to the contents of the CommandString. */
/******************************************************************************/
globle void AppendCommandString(
  void *theEnv,
  char *str)
  {
   CommandLineData(theEnv)->CommandString = AppendToString(theEnv,str,CommandLineData(theEnv)->CommandString,&RouterData(theEnv)->CommandBufferInputCount,&CommandLineData(theEnv)->MaximumCharacters);
  }

/************************************************************/
/* AppendNCommandString: Appends a value up to N characters */
/*   to the contents of the CommandString.                  */
/************************************************************/
globle void AppendNCommandString(
  void *theEnv,
  char *str,
  unsigned length)
  {
   CommandLineData(theEnv)->CommandString = AppendNToString(theEnv,str,CommandLineData(theEnv)->CommandString,length,&RouterData(theEnv)->CommandBufferInputCount,&CommandLineData(theEnv)->MaximumCharacters);
  }

/*****************************************************************************/
/* GetCommandString: Returns a pointer to the contents of the CommandString. */
/*****************************************************************************/
globle char *GetCommandString(
  void *theEnv)
  {
   return(CommandLineData(theEnv)->CommandString);
  }

/**************************************************************************/
/* CompleteCommand: Determines whether a string forms a complete command. */
/*   A complete command is either a constant, a variable, or a function   */
/*   call which is followed (at some point) by a carriage return. Once a  */
/*   complete command is found (not including the parenthesis),           */
/*   extraneous parenthesis and other tokens are ignored. If a complete   */
/*   command exists, then 1 is returned. 0 is returned if the command was */
/*   not complete and without errors. -1 is returned if the command       */
/*   contains an error.                                                   */
/**************************************************************************/
globle int CompleteCommand(
  char *mstring)
  {
   int i;
   char inchar;
   int depth = 0;
   int moreThanZero = 0;
   int complete;
   int error = 0;

   if (mstring == NULL) return(0);

   /*===================================================*/
   /* Loop through each character of the command string */
   /* to determine if there is a complete command.      */
   /*===================================================*/

   i = 0;
   while ((inchar = mstring[i++]) != EOS)
     {
      switch(inchar)
        {
         /*======================================================*/
         /* If a carriage return or line feed is found, there is */
         /* at least one completed token in the command buffer,  */
         /* and parentheses are balanced, then a complete        */
         /* command has been found. Otherwise, remove all white  */
         /* space beginning with the current character.          */
         /*======================================================*/

         case '\n' :
         case '\r' :
           if (error) return(-1);
           if (moreThanZero && (depth == 0)) return(1);
           i = DoWhiteSpace(mstring,i);
           break;

         /*=====================*/
         /* Remove white space. */
         /*=====================*/

         case ' ' :
         case '\f' :
         case '\t' :
           i = DoWhiteSpace(mstring,i);
           break;

         /*======================================================*/
         /* If the opening quotation of a string is encountered, */
         /* determine if the closing quotation of the string is  */
         /* in the command buffer. Until the closing quotation   */
         /* is found, a complete command can not be made.        */
         /*======================================================*/

         case '"' :
           i = DoString(mstring,i,&complete);
           if ((depth == 0) && complete) moreThanZero = TRUE;
           break;

         /*====================*/
         /* Process a comment. */
         /*====================*/

         case ';' :
           i = DoComment(mstring,i);
           if (moreThanZero && (depth == 0) && (mstring[i] != EOS))
             {
              if (error) return(-1);
              else return(1);
             }
           else if (mstring[i] != EOS) i++;
           break;

         /*====================================================*/
         /* A left parenthesis increases the nesting depth of  */
         /* the current command by 1. Don't bother to increase */
         /* the depth if the first token encountered was not   */
         /* a parenthesis (e.g. for the command string         */
         /* "red (+ 3 4", the symbol red already forms a       */
         /* complete command, so the next carriage return will */
         /* cause evaluation of red--the closing parenthesis   */
         /* for "(+ 3 4" does not have to be found).           */
         /*====================================================*/

         case '(' :
           if ((depth > 0) || (moreThanZero == FALSE))
             {
              depth++;
              moreThanZero = TRUE;
             }
           break;

         /*====================================================*/
         /* A right parenthesis decreases the nesting depth of */
         /* the current command by 1. If the parenthesis is    */
         /* the first token of the command, then an error is   */
         /* generated.                                         */
         /*====================================================*/

         case ')' :
           if (depth > 0) depth--;
           else if (moreThanZero == FALSE) error = TRUE;
           break;

         /*=====================================================*/
         /* If the command begins with any other character and  */
         /* an opening parenthesis hasn't yet been found, then  */
         /* skip all characters on the same line. If a carriage */
         /* return or line feed is found, then a complete       */
         /* command exists.                                     */
         /*=====================================================*/

         default:
           if (depth == 0)
             {
              if (isprint(inchar) || IsUTF8MultiByteStart(inchar))
                {
                 while ((inchar = mstring[i++]) != EOS)
                   {
                    if ((inchar == '\n') || (inchar == '\r'))
                      {
                       if (error) return(-1);
                       else return(1);
                      }
                   }
                 return(0);
                }
             }
           break;
        }
     }

   /*====================================================*/
   /* Return 0 because a complete command was not found. */
   /*====================================================*/

   return(0);
  }

/***********************************************************/
/* DoString: Skips over a string contained within a string */
/*   until the closing quotation mark is encountered.      */
/***********************************************************/
static int DoString(
  char *str,
  int pos,
  int *complete)
  {
   int inchar;

   /*=================================================*/
   /* Process the string character by character until */
   /* the closing quotation mark is found.            */
   /*=================================================*/

   inchar = str[pos];
   while (inchar  != '"')
     {
      /*=====================================================*/
      /* If a \ is found, then the next character is ignored */
      /* even if it is a closing quotation mark.             */
      /*=====================================================*/

      if (inchar == '\\')
        {
         pos++;
         inchar = str[pos];
        }

      /*===================================================*/
      /* If the end of input is reached before the closing */
      /* quotation mark is found, the return the last      */
      /* position that was reached and indicate that a     */
      /* complete string was not found.                    */
      /*===================================================*/

      if (inchar == EOS)
        {
         *complete = FALSE;
         return(pos);
        }

      /*================================*/
      /* Move on to the next character. */
      /*================================*/

      pos++;
      inchar = str[pos];
     }

   /*======================================================*/
   /* Indicate that a complete string was found and return */
   /* the position of the closing quotation mark.          */
   /*======================================================*/

   pos++;
   *complete = TRUE;
   return(pos);
  }

/*************************************************************/
/* DoComment: Skips over a comment contained within a string */
/*   until a line feed or carriage return is encountered.    */
/*************************************************************/
static int DoComment(
  char *str,
  int pos)
  {
   int inchar;

   inchar = str[pos];
   while ((inchar != '\n') && (inchar != '\r'))
     {
      if (inchar == EOS)
        { return(pos); }

      pos++;
      inchar = str[pos];
     }

   return(pos);
  }

/**************************************************************/
/* DoWhiteSpace: Skips over white space consisting of spaces, */
/*   tabs, and form feeds that is contained within a string.  */
/**************************************************************/
static int DoWhiteSpace(
  char *str,
  int pos)
  {
   int inchar;

   inchar = str[pos];
   while ((inchar == ' ') || (inchar == '\f') || (inchar == '\t'))
     {
      pos++;
      inchar = str[pos];
     }

   return(pos);
  }

/********************************************************************/
/* CommandLoop: Endless loop which waits for user commands and then */
/*   executes them. The command loop will bypass the EventFunction  */
/*   if there is an active batch file.                              */
/********************************************************************/
globle void CommandLoop(
  void *theEnv)
  {
   int inchar;

   EnvPrintRouter(theEnv,WPROMPT,CommandLineData(theEnv)->BannerString);
   SetHaltExecution(theEnv,FALSE);
   SetEvaluationError(theEnv,FALSE);
   PeriodicCleanup(theEnv,TRUE,FALSE);
   PrintPrompt(theEnv);
   RouterData(theEnv)->CommandBufferInputCount = 0;
   RouterData(theEnv)->AwaitingInput = TRUE;

   while (TRUE)
     {
      /*===================================================*/
      /* If a batch file is active, grab the command input */
      /* directly from the batch file, otherwise call the  */
      /* event function.                                   */
      /*===================================================*/

      if (BatchActive(theEnv) == TRUE)
        {
         inchar = LLGetcBatch(theEnv,"stdin",TRUE);
         if (inchar == EOF)
           { (*CommandLineData(theEnv)->EventFunction)(theEnv); }
         else
           { ExpandCommandString(theEnv,(char) inchar); }
        }
      else
        { (*CommandLineData(theEnv)->EventFunction)(theEnv); }

      /*=================================================*/
      /* If execution was halted, then remove everything */
      /* from the command buffer.                        */
      /*=================================================*/

      if (GetHaltExecution(theEnv) == TRUE)
        {
         SetHaltExecution(theEnv,FALSE);
         SetEvaluationError(theEnv,FALSE);
         FlushCommandString(theEnv);
#if ! WINDOW_INTERFACE
         fflush(stdin);
#endif
         EnvPrintRouter(theEnv,WPROMPT,"\n");
         PrintPrompt(theEnv);
        }

      /*=========================================*/
      /* If a complete command is in the command */
      /* buffer, then execute it.                */
      /*=========================================*/

      ExecuteIfCommandComplete(theEnv);
     }
  }
  
/***********************************************************/
/* CommandLoopBatch: Loop which waits for commands from a  */
/*   batch file and then executes them. Returns when there */
/*   are no longer any active batch files.                 */
/***********************************************************/
globle void CommandLoopBatch(
  void *theEnv)
  {
   SetHaltExecution(theEnv,FALSE);
   SetEvaluationError(theEnv,FALSE);
   PeriodicCleanup(theEnv,TRUE,FALSE);
   PrintPrompt(theEnv);
   RouterData(theEnv)->CommandBufferInputCount = 0;
   RouterData(theEnv)->AwaitingInput = TRUE;

   CommandLoopBatchDriver(theEnv);
  }

/************************************************************/
/* CommandLoopOnceThenBatch: Loop which waits for commands  */
/*   from a batch file and then executes them. Returns when */
/*   there are no longer any active batch files.            */
/************************************************************/
globle void CommandLoopOnceThenBatch(
  void *theEnv)
  {
   if (! ExecuteIfCommandComplete(theEnv)) return;

   CommandLoopBatchDriver(theEnv);
  }
  
/*********************************************************/
/* CommandLoopBatchDriver: Loop which waits for commands */
/*   from a batch file and then executes them. Returns   */
/*   when there are no longer any active batch files.    */
/*********************************************************/
globle void CommandLoopBatchDriver(
  void *theEnv)
  {
   int inchar;

   while (TRUE)
     {
      if (GetHaltCommandLoopBatch(theEnv) == TRUE)
        { 
         CloseAllBatchSources(theEnv);
         SetHaltCommandLoopBatch(theEnv,FALSE);
        }
        
      /*===================================================*/
      /* If a batch file is active, grab the command input */
      /* directly from the batch file, otherwise call the  */
      /* event function.                                   */
      /*===================================================*/

      if (BatchActive(theEnv) == TRUE)
        {
         inchar = LLGetcBatch(theEnv,"stdin",TRUE);
         if (inchar == EOF)
           { return; }
         else
           { ExpandCommandString(theEnv,(char) inchar); }
        }
      else
        { return; }

      /*=================================================*/
      /* If execution was halted, then remove everything */
      /* from the command buffer.                        */
      /*=================================================*/

      if (GetHaltExecution(theEnv) == TRUE)
        {
         SetHaltExecution(theEnv,FALSE);
         SetEvaluationError(theEnv,FALSE);
         FlushCommandString(theEnv);
#if ! WINDOW_INTERFACE
         fflush(stdin);
#endif
         EnvPrintRouter(theEnv,WPROMPT,"\n");
         PrintPrompt(theEnv);
        }

      /*=========================================*/
      /* If a complete command is in the command */
      /* buffer, then execute it.                */
      /*=========================================*/

      ExecuteIfCommandComplete(theEnv);
     }
  }

/**********************************************************/
/* ExecuteIfCommandComplete: Checks to determine if there */
/*   is a completed command and if so executes it.        */
/**********************************************************/
globle intBool ExecuteIfCommandComplete(
  void *theEnv)
  {
   if ((CompleteCommand(CommandLineData(theEnv)->CommandString) == 0) || 
       (RouterData(theEnv)->CommandBufferInputCount == 0) ||
       (RouterData(theEnv)->AwaitingInput == FALSE))
     { return FALSE; }
     
   if (CommandLineData(theEnv)->BeforeCommandExecutionFunction != NULL)
     { 
      if (! (*CommandLineData(theEnv)->BeforeCommandExecutionFunction)(theEnv))
        { return FALSE; }
     }
       
   FlushPPBuffer(theEnv);
   SetPPBufferStatus(theEnv,OFF);
   RouterData(theEnv)->CommandBufferInputCount = 0;
   RouterData(theEnv)->AwaitingInput = FALSE;
   RouteCommand(theEnv,CommandLineData(theEnv)->CommandString,TRUE);
   FlushPPBuffer(theEnv);
   SetHaltExecution(theEnv,FALSE);
   SetEvaluationError(theEnv,FALSE);
   FlushCommandString(theEnv);
   PeriodicCleanup(theEnv,TRUE,FALSE);
   PrintPrompt(theEnv);
         
   return TRUE;
  }

/*******************************************/
/* CommandCompleteAndNotEmpty: */
/*******************************************/
globle intBool CommandCompleteAndNotEmpty(
  void *theEnv)
  {
   if ((CompleteCommand(CommandLineData(theEnv)->CommandString) == 0) || 
       (RouterData(theEnv)->CommandBufferInputCount == 0) ||
       (RouterData(theEnv)->AwaitingInput == FALSE))
     { return FALSE; }
     
   return TRUE;
  }
       
/*******************************************/
/* PrintPrompt: Prints the command prompt. */
/*******************************************/
globle void PrintPrompt(
   void *theEnv)
   {
    EnvPrintRouter(theEnv,WPROMPT,COMMAND_PROMPT);

    if (CommandLineData(theEnv)->AfterPromptFunction != NULL)
      { (*CommandLineData(theEnv)->AfterPromptFunction)(theEnv); }
   }

/*****************************************/
/* PrintBanner: Prints the CLIPS banner. */
/*****************************************/
globle void PrintBanner(
   void *theEnv)
   {
    EnvPrintRouter(theEnv,WPROMPT,CommandLineData(theEnv)->BannerString);
   }

/************************************************/
/* SetAfterPromptFunction: Replaces the current */
/*   value of AfterPromptFunction.              */
/************************************************/
globle void SetAfterPromptFunction(
  void *theEnv,
  int (*funptr)(void *))
  {
   CommandLineData(theEnv)->AfterPromptFunction = funptr;
  }

/***********************************************************/
/* SetBeforeCommandExecutionFunction: Replaces the current */
/*   value of BeforeCommandExecutionFunction.              */
/***********************************************************/
globle void SetBeforeCommandExecutionFunction(
  void *theEnv,
  int (*funptr)(void *))
  {
   CommandLineData(theEnv)->BeforeCommandExecutionFunction = funptr;
  }
  
/************************************************/
/* RouteCommand: Processes a completed command. */
/************************************************/
globle intBool RouteCommand(
  void *theEnv,
  char *command,
  int printResult)
  {
   DATA_OBJECT result;
   struct expr *top;
   char *commandName;
   struct token theToken;

   if (command == NULL)
     { return(0); }

   /*========================================*/
   /* Open a string input source and get the */
   /* first token from that source.          */
   /*========================================*/

   OpenStringSource(theEnv,"command",command,0);

   GetToken(theEnv,"command",&theToken);

   /*=====================*/
   /* Evaluate constants. */
   /*=====================*/

   if ((theToken.type == SYMBOL) || (theToken.type == STRING) ||
       (theToken.type == FLOAT) || (theToken.type == INTEGER) ||
       (theToken.type == INSTANCE_NAME))
     {
      CloseStringSource(theEnv,"command");
      if (printResult)
        {
         PrintAtom(theEnv,"stdout",theToken.type,theToken.value);
         EnvPrintRouter(theEnv,"stdout","\n");
        }
      return(1);
     }

   /*=====================*/
   /* Evaluate variables. */
   /*=====================*/

   if ((theToken.type == GBL_VARIABLE) ||
       (theToken.type == SF_VARIABLE) ||
       (theToken.type == MF_VARIABLE))
     {
      CloseStringSource(theEnv,"command");
      top = GenConstant(theEnv,theToken.type,theToken.value);
      EvaluateExpression(theEnv,top,&result);
      rtn_struct(theEnv,expr,top);
      if (printResult)
        {
         PrintDataObject(theEnv,"stdout",&result);
         EnvPrintRouter(theEnv,"stdout","\n");
        }
      return(1);
     }

   /*========================================================*/
   /* If the next token isn't the beginning left parenthesis */
   /* of a command or construct, then whatever was entered   */
   /* cannot be evaluated at the command prompt.             */
   /*========================================================*/

   if (theToken.type != LPAREN)
     {
      PrintErrorID(theEnv,"COMMLINE",1,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Expected a '(', constant, or variable\n");
      CloseStringSource(theEnv,"command");
      return(0);
     }

   /*===========================================================*/
   /* The next token must be a function name or construct type. */
   /*===========================================================*/

   GetToken(theEnv,"command",&theToken);
   if (theToken.type != SYMBOL)
     {
      PrintErrorID(theEnv,"COMMLINE",2,FALSE);
      EnvPrintRouter(theEnv,WERROR,"Expected a command.\n");
      CloseStringSource(theEnv,"command");
      return(0);
     }

   commandName = ValueToString(theToken.value);

   /*======================*/
   /* Evaluate constructs. */
   /*======================*/

#if (! RUN_TIME) && (! BLOAD_ONLY)
   {
    int errorFlag;

    errorFlag = ParseConstruct(theEnv,commandName,"command");
    if (errorFlag != -1)
      {
       CloseStringSource(theEnv,"command");
       if (errorFlag == 1)
         {
          EnvPrintRouter(theEnv,WERROR,"\nERROR:\n");
          PrintInChunks(theEnv,WERROR,GetPPBuffer(theEnv));
          EnvPrintRouter(theEnv,WERROR,"\n");
         }
       DestroyPPBuffer(theEnv);
       return(errorFlag);
      }
   }
#endif

   /*========================*/
   /* Parse a function call. */
   /*========================*/

   CommandLineData(theEnv)->ParsingTopLevelCommand = TRUE;
   top = Function2Parse(theEnv,"command",commandName);
   CommandLineData(theEnv)->ParsingTopLevelCommand = FALSE;
   ClearParsedBindNames(theEnv);

   /*================================*/
   /* Close the string input source. */
   /*================================*/

   CloseStringSource(theEnv,"command");

   /*=========================*/
   /* Evaluate function call. */
   /*=========================*/

   if (top == NULL) return(0);
   
   ExpressionInstall(theEnv,top);
   
   CommandLineData(theEnv)->EvaluatingTopLevelCommand = TRUE;
   CommandLineData(theEnv)->CurrentCommand = top;
   EvaluateExpression(theEnv,top,&result);
   CommandLineData(theEnv)->CurrentCommand = NULL;
   CommandLineData(theEnv)->EvaluatingTopLevelCommand = FALSE;
   
   ExpressionDeinstall(theEnv,top);
   ReturnExpression(theEnv,top);
   
   if ((result.type != RVOID) && printResult)
     {
      PrintDataObject(theEnv,"stdout",&result);
      EnvPrintRouter(theEnv,"stdout","\n");
     }

   return(1);
  }

/*****************************************************************/
/* DefaultGetNextEvent: Default event-handling function. Handles */
/*   only keyboard events by first calling GetcRouter to get a   */
/*   character and then calling ExpandCommandString to add the   */
/*   character to the CommandString.                             */
/*****************************************************************/
static int DefaultGetNextEvent(
  void *theEnv)
  {
   int inchar;

   inchar = EnvGetcRouter(theEnv,"stdin");

   if (inchar == EOF) inchar = '\n';

   ExpandCommandString(theEnv,(char) inchar);
   
   return 0;
  }

/*************************************/
/* SetEventFunction: Replaces the    */
/*   current value of EventFunction. */
/*************************************/
globle int (*SetEventFunction(void *theEnv,int (*theFunction)(void *)))(void *)
  {
   int (*tmp_ptr)(void *);

   tmp_ptr = CommandLineData(theEnv)->EventFunction;
   CommandLineData(theEnv)->EventFunction = theFunction;
   return(tmp_ptr);
  }

/****************************************/
/* TopLevelCommand: Indicates whether a */
/*   top-level command is being parsed. */
/****************************************/
globle intBool TopLevelCommand(
  void *theEnv)
  {
   return(CommandLineData(theEnv)->ParsingTopLevelCommand);
  }

/***********************************************************/
/* GetCommandCompletionString: Returns the last token in a */
/*   string if it is a valid token for command completion. */
/***********************************************************/
globle char *GetCommandCompletionString(
  void *theEnv,
  char *theString,
  size_t maxPosition)
  {
   struct token lastToken;
   struct token theToken;
   char lastChar;
   char *rs;
   size_t length;

   /*=========================*/
   /* Get the command string. */
   /*=========================*/

   if (theString == NULL) return("");

   /*=========================================================================*/
   /* If the last character in the command string is a space, character       */
   /* return, or quotation mark, then the command completion can be anything. */
   /*=========================================================================*/

   lastChar = theString[maxPosition - 1];
   if ((lastChar == ' ') || (lastChar == '"') ||
       (lastChar == '\t') || (lastChar == '\f') ||
       (lastChar == '\n') || (lastChar == '\r'))
     { return(""); }

   /*============================================*/
   /* Find the last token in the command string. */
   /*============================================*/

   OpenTextSource(theEnv,"CommandCompletion",theString,0,maxPosition);
   ScannerData(theEnv)->IgnoreCompletionErrors = TRUE;
   GetToken(theEnv,"CommandCompletion",&theToken);
   CopyToken(&lastToken,&theToken);
   while (theToken.type != STOP)
     {
      CopyToken(&lastToken,&theToken);
      GetToken(theEnv,"CommandCompletion",&theToken);
     }
   CloseStringSource(theEnv,"CommandCompletion");
   ScannerData(theEnv)->IgnoreCompletionErrors = FALSE;

   /*===============================================*/
   /* Determine if the last token can be completed. */
   /*===============================================*/

   if (lastToken.type == SYMBOL)
     {
      rs = ValueToString(lastToken.value);
      if (rs[0] == '[') return (&rs[1]);
      return(ValueToString(lastToken.value));
     }
   else if (lastToken.type == SF_VARIABLE)
     { return(ValueToString(lastToken.value)); }
   else if (lastToken.type == MF_VARIABLE)
     { return(ValueToString(lastToken.value)); }
   else if ((lastToken.type == GBL_VARIABLE) || (lastToken.type == MF_GBL_VARIABLE) ||
            (lastToken.type == INSTANCE_NAME))
     { return(NULL); }
   else if (lastToken.type == STRING)
     {
      length = strlen(ValueToString(lastToken.value));
      return(GetCommandCompletionString(theEnv,ValueToString(lastToken.value),length));
     }
   else if ((lastToken.type == FLOAT) || (lastToken.type == INTEGER))
     { return(NULL); }

   return("");
  }

/****************************************************************/
/* SetHaltCommandLoopBatch: Sets the HaltCommandLoopBatch flag. */
/****************************************************************/
globle void SetHaltCommandLoopBatch(
  void *theEnv,
  int value)
  { 
   CommandLineData(theEnv)->HaltCommandLoopBatch = value; 
  }

/*******************************************************************/
/* GetHaltCommandLoopBatch: Returns the HaltCommandLoopBatch flag. */
/*******************************************************************/
globle int GetHaltCommandLoopBatch(
  void *theEnv)
  {
   return(CommandLineData(theEnv)->HaltCommandLoopBatch);
  }

#endif

