   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  03/06/08            */
   /*                                                     */
   /*                    SCANNER MODULE                   */
   /*******************************************************/

/*************************************************************/
/* Purpose: Routines for scanning lexical tokens from an     */
/*   input source.                                           */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Chris Culbert                                        */
/*      Brian Dantes                                         */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.30: Added UTF-8 support.                           */
/*                                                           */
/*************************************************************/

#define _SCANNER_SOURCE_

#include <ctype.h>
#include <stdio.h>
#define _STDIO_INCLUDED_
#include <string.h>
#include <limits.h>

#include "setup.h"
#include "constant.h"
#include "envrnmnt.h"
#include "router.h"
#include "symbol.h"
#include "utility.h"
#include "memalloc.h"
#include "sysdep.h"

#include "scanner.h"

#include <stdlib.h>

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                   *ScanSymbol(void *,char *,int,unsigned short *);
   static void                   *ScanString(void *,char *);
   static void                    ScanNumber(void *,char *,struct token *);
   static void                    DeallocateScannerData(void *);

/************************************************/
/* InitializeScannerData: Allocates environment */
/*    data for scanner routines.                */
/************************************************/
globle void InitializeScannerData(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,SCANNER_DATA,sizeof(struct scannerData),DeallocateScannerData);
  }
  
/**************************************************/
/* DeallocateScannerData: Deallocates environment */
/*    data for scanner routines.                  */
/**************************************************/
static void DeallocateScannerData(
  void *theEnv)
  {
   if (ScannerData(theEnv)->GlobalMax !=  0)
     { genfree(theEnv,ScannerData(theEnv)->GlobalString,ScannerData(theEnv)->GlobalMax); }
  }

/***********************************************************************/
/* GetToken: Reads next token from the input stream. The pointer to    */
/*   the token data structure passed as an argument is set to contain  */
/*   the type of token (e.g., symbol, string, integer, etc.), the data */
/*   value for the token (i.e., a symbol table location if it is a     */
/*   symbol or string, an integer table location if it is an integer), */
/*   and the pretty print representation.                              */
/***********************************************************************/
globle void GetToken(
 void *theEnv,
 char *logicalName,
 struct token *theToken)
 {
   int inchar;
   unsigned short type;

   /*=======================================*/
   /* Set Unknown default values for token. */
   /*=======================================*/

   theToken->type = UNKNOWN_VALUE;
   theToken->value = NULL;
   theToken->printForm = "unknown";
   ScannerData(theEnv)->GlobalPos = 0;
   ScannerData(theEnv)->GlobalMax = 0;

   /*==============================================*/
   /* Remove all white space before processing the */
   /* GetToken() request.                          */
   /*==============================================*/

   inchar = EnvGetcRouter(theEnv,logicalName);
   while ((inchar == ' ') || (inchar == '\n') || (inchar == '\f') ||
          (inchar == '\r') || (inchar == ';') || (inchar == '\t'))
     {
      /*=======================*/
      /* Remove comment lines. */
      /*=======================*/

      if (inchar == ';')
        {
         inchar = EnvGetcRouter(theEnv,logicalName);
         while ((inchar != '\n') && (inchar != '\r') && (inchar != EOF) )
           { inchar = EnvGetcRouter(theEnv,logicalName); }
        }
      inchar = EnvGetcRouter(theEnv,logicalName);
     }

   /*==========================*/
   /* Process Symbolic Tokens. */
   /*==========================*/

   if (isalpha(inchar) || IsUTF8MultiByteStart(inchar))
     {
      theToken->type = SYMBOL;
      EnvUngetcRouter(theEnv,inchar,logicalName);
      theToken->value = (void *) ScanSymbol(theEnv,logicalName,0,&type);
      theToken->printForm = ValueToString(theToken->value);
     }

   /*===============================================*/
   /* Process Number Tokens beginning with a digit. */
   /*===============================================*/

   else if (isdigit(inchar))
     {
      EnvUngetcRouter(theEnv,inchar,logicalName);
      ScanNumber(theEnv,logicalName,theToken);
     }

   else switch (inchar)
     {
      /*========================*/
      /* Process String Tokens. */
      /*========================*/

      case '"':
         theToken->value = (void *) ScanString(theEnv,logicalName);
         theToken->type = STRING;
         theToken->printForm = StringPrintForm(theEnv,ValueToString(theToken->value));
         break;

      /*=======================================*/
      /* Process Tokens that might be numbers. */
      /*=======================================*/

      case '-':
      case '.':
      case '+':
         EnvUngetcRouter(theEnv,inchar,logicalName);
         ScanNumber(theEnv,logicalName,theToken);
         break;

      /*===================================*/
      /* Process ? and ?<variable> Tokens. */
      /*===================================*/

       case '?':
          inchar = EnvGetcRouter(theEnv,logicalName);
          if (isalpha(inchar) || IsUTF8MultiByteStart(inchar)
#if DEFGLOBAL_CONSTRUCT
              || (inchar == '*'))
#else
              )
#endif
            {
             EnvUngetcRouter(theEnv,inchar,logicalName);
             theToken->value = (void *) ScanSymbol(theEnv,logicalName,0,&type);
             theToken->type = SF_VARIABLE;
#if DEFGLOBAL_CONSTRUCT
             if ((ValueToString(theToken->value)[0] == '*') &&
                 (((int) strlen(ValueToString(theToken->value))) > 1) &&
                 (ValueToString(theToken->value)[strlen(ValueToString(theToken->value)) - 1] == '*'))
               {
                size_t count;

                theToken->type = GBL_VARIABLE;
                theToken->printForm = AppendStrings(theEnv,"?",ValueToString(theToken->value));
                count = strlen(ScannerData(theEnv)->GlobalString);
                ScannerData(theEnv)->GlobalString[count-1] = EOS;
                theToken->value = EnvAddSymbol(theEnv,ScannerData(theEnv)->GlobalString+1);
                ScannerData(theEnv)->GlobalString[count-1] = (char) inchar;

               }
             else
#endif
             theToken->printForm = AppendStrings(theEnv,"?",ValueToString(theToken->value));
            }
          else
            {
             theToken->type = SF_WILDCARD;
             theToken->value = (void *) EnvAddSymbol(theEnv,"?");
             EnvUngetcRouter(theEnv,inchar,logicalName);
             theToken->printForm = "?";
            }
          break;

      /*=====================================*/
      /* Process $? and $?<variable> Tokens. */
      /*=====================================*/

      case '$':
         if ((inchar = EnvGetcRouter(theEnv,logicalName)) == '?')
           {
            inchar = EnvGetcRouter(theEnv,logicalName);
            if (isalpha(inchar) || IsUTF8MultiByteStart(inchar)
#if DEFGLOBAL_CONSTRUCT
                 || (inchar == '*'))
#else
                 )
#endif
              {
               EnvUngetcRouter(theEnv,inchar,logicalName);
               theToken->value = (void *) ScanSymbol(theEnv,logicalName,0,&type);
               theToken->type = MF_VARIABLE;
#if DEFGLOBAL_CONSTRUCT
             if ((ValueToString(theToken->value)[0] == '*') &&
                 ((int) (strlen(ValueToString(theToken->value))) > 1) &&
                 (ValueToString(theToken->value)[strlen(ValueToString(theToken->value)) - 1] == '*'))
               {
                size_t count;

                theToken->type = MF_GBL_VARIABLE;
                theToken->printForm = AppendStrings(theEnv,"$?",ValueToString(theToken->value));
                count = strlen(ScannerData(theEnv)->GlobalString);
                ScannerData(theEnv)->GlobalString[count-1] = EOS;
                theToken->value = EnvAddSymbol(theEnv,ScannerData(theEnv)->GlobalString+1);
                ScannerData(theEnv)->GlobalString[count-1] = (char) inchar;
               }
             else
#endif
               theToken->printForm = AppendStrings(theEnv,"$?",ValueToString(theToken->value));
              }
            else
              {
               theToken->type = MF_WILDCARD;
               theToken->value = (void *) EnvAddSymbol(theEnv,"$?");
               theToken->printForm = "$?";
               EnvUngetcRouter(theEnv,inchar,logicalName);
              }
           }
         else
           {
            theToken->type = SYMBOL;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,'$',ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            EnvUngetcRouter(theEnv,inchar,logicalName);
            theToken->value = (void *) ScanSymbol(theEnv,logicalName,1,&type);
            theToken->printForm = ValueToString(theToken->value);
           }
         break;

      /*============================*/
      /* Symbols beginning with '<' */
      /*============================*/

      case '<':
         theToken->type = SYMBOL;
         ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,'<',ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
         theToken->value = (void *) ScanSymbol(theEnv,logicalName,1,&type);
         theToken->printForm = ValueToString(theToken->value);
         break;

      /*=============================================*/
      /* Process "(", ")", "~", "|", and "&" Tokens. */
      /*=============================================*/

      case '(':
         theToken->type = LPAREN;
         theToken->value = (void *) EnvAddSymbol(theEnv,"(");
         theToken->printForm = "(";
         break;

      case ')':
         theToken->type= RPAREN;
         theToken->value = (void *) EnvAddSymbol(theEnv,")");
         theToken->printForm = ")";
         break;

      case '~':
         theToken->type = NOT_CONSTRAINT;
         theToken->value = (void *) EnvAddSymbol(theEnv,"~");
         theToken->printForm = "~";
         break;

      case '|':
         theToken->type = OR_CONSTRAINT;
         theToken->value = (void *) EnvAddSymbol(theEnv,"|");
         theToken->printForm = "|";
         break;

      case '&':
         theToken->type =  AND_CONSTRAINT;
         theToken->value = (void *) EnvAddSymbol(theEnv,"&");
         theToken->printForm = "&";
         break;

      /*============================*/
      /* Process End-of-File Token. */
      /*============================*/

      case EOF:
      case 0:
      case 3:
         theToken->type = STOP;
         theToken->value = (void *) EnvAddSymbol(theEnv,"stop");
         theToken->printForm = "";
         break;

      /*=======================*/
      /* Process Other Tokens. */
      /*=======================*/

      default:
         if (isprint(inchar))
           {
            EnvUngetcRouter(theEnv,inchar,logicalName);
            theToken->value = (void *) ScanSymbol(theEnv,logicalName,0,&type);
            theToken->type = type;
            theToken->printForm = ValueToString(theToken->value);
           }
         else
           { theToken->printForm = "<<<unprintable character>>>"; }
         break;
     }

   /*===============================================*/
   /* Put the new token in the pretty print buffer. */
   /*===============================================*/

#if (! RUN_TIME) && (! BLOAD_ONLY)
   if (theToken->type == INSTANCE_NAME)
     {
      SavePPBuffer(theEnv,"[");
      SavePPBuffer(theEnv,theToken->printForm);
      SavePPBuffer(theEnv,"]");
     }
   else
     { SavePPBuffer(theEnv,theToken->printForm); }
#endif

   /*=========================================================*/
   /* Return the temporary memory used in scanning the token. */
   /*=========================================================*/

   if (ScannerData(theEnv)->GlobalString != NULL)
     {
      rm(theEnv,ScannerData(theEnv)->GlobalString,ScannerData(theEnv)->GlobalMax);
      ScannerData(theEnv)->GlobalString = NULL;
      ScannerData(theEnv)->GlobalMax = 0;
      ScannerData(theEnv)->GlobalPos = 0;
     }

   return;
  }

/*************************************/
/* ScanSymbol: Scans a symbol token. */
/*************************************/
static void *ScanSymbol(
  void *theEnv,
  char *logicalName,
  int count,
  unsigned short *type)
  {
   int inchar;
#if OBJECT_SYSTEM
   void *symbol;
#endif

   /*=====================================*/
   /* Scan characters and add them to the */
   /* symbol until a delimiter is found.  */
   /*=====================================*/

   inchar = EnvGetcRouter(theEnv,logicalName);
   while ( (inchar != '<') && (inchar != '"') &&
           (inchar != '(') && (inchar != ')') &&
           (inchar != '&') && (inchar != '|') && (inchar != '~') &&
           (inchar != ' ') && (inchar != ';') &&
           (isprint(inchar) ||
            IsUTF8MultiByteStart(inchar) || 
            IsUTF8MultiByteContinuation(inchar)))
     {
      ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);

      count++;
      inchar = EnvGetcRouter(theEnv,logicalName);
     }

   /*===================================================*/
   /* Return the last character scanned (the delimiter) */
   /* to the input stream so it will be scanned as part */
   /* of the next token.                                */
   /*===================================================*/

   EnvUngetcRouter(theEnv,inchar,logicalName);

   /*====================================================*/
   /* Add the symbol to the symbol table and return the  */
   /* symbol table address of the symbol. Symbols of the */
   /* form [<symbol>] are instance names, so the type    */
   /* returned is INSTANCE_NAME rather than SYMBOL.      */
   /*====================================================*/

#if OBJECT_SYSTEM
   if (count > 2)
     {
      if ((ScannerData(theEnv)->GlobalString[0] == '[') ? (ScannerData(theEnv)->GlobalString[count-1] == ']') : FALSE)
        {
         *type = INSTANCE_NAME;
         inchar = ']';
        }
      else
        {
         *type = SYMBOL;
         return(EnvAddSymbol(theEnv,ScannerData(theEnv)->GlobalString));
        }
      ScannerData(theEnv)->GlobalString[count-1] = EOS;
      symbol = EnvAddSymbol(theEnv,ScannerData(theEnv)->GlobalString+1);
      ScannerData(theEnv)->GlobalString[count-1] = (char) inchar;
      return(symbol);
     }
   else
     {
      *type = SYMBOL;
      return(EnvAddSymbol(theEnv,ScannerData(theEnv)->GlobalString));
     }
#else
   *type = SYMBOL;
   return(EnvAddSymbol(theEnv,ScannerData(theEnv)->GlobalString));
#endif
  }

/*************************************/
/* ScanString: Scans a string token. */
/*************************************/
static void *ScanString(
  void *theEnv,
  char *logicalName)
  {
   int inchar;
   size_t pos = 0;
   size_t max = 0;
   char *theString = NULL;
   void *thePtr;

   /*============================================*/
   /* Scan characters and add them to the string */
   /* until the " delimiter is found.            */
   /*============================================*/

   inchar = EnvGetcRouter(theEnv,logicalName);
   while ((inchar != '"') && (inchar != EOF))
     {
      if (inchar == '\\')
        { inchar = EnvGetcRouter(theEnv,logicalName); }

      theString = ExpandStringWithChar(theEnv,inchar,theString,&pos,&max,max+80);
      inchar = EnvGetcRouter(theEnv,logicalName);
     }

   if ((inchar == EOF) && (ScannerData(theEnv)->IgnoreCompletionErrors == FALSE))
     { 
      PrintErrorID(theEnv,"SCANNER",1,TRUE);
      EnvPrintRouter(theEnv,WERROR,"Encountered End-Of-File while scanning a string\n"); 
     }

   /*===============================================*/
   /* Add the string to the symbol table and return */
   /* the symbol table address of the string.       */
   /*===============================================*/

   if (theString == NULL)
     { thePtr = EnvAddSymbol(theEnv,""); }
   else
     {
      thePtr = EnvAddSymbol(theEnv,theString);
      rm(theEnv,theString,max);
     }

   return(thePtr);
  }

/**************************************/
/* ScanNumber: Scans a numeric token. */
/**************************************/
static void ScanNumber(
  void *theEnv,
  char *logicalName,
  struct token *theToken)
  {
   int count = 0;
   int inchar, phase;
   int digitFound = FALSE;
   int processFloat = FALSE;
   double fvalue;
   long long lvalue;
   unsigned short type;

   /* Phases:              */
   /*  -1 = sign           */
   /*   0 = integral       */
   /*   1 = decimal        */
   /*   2 = exponent-begin */
   /*   3 = exponent-value */
   /*   5 = done           */
   /*   9 = error          */

   inchar = EnvGetcRouter(theEnv,logicalName);
   phase = -1;

   while ((phase != 5) && (phase != 9))
     {
      if (phase == -1)
        {
         if (isdigit(inchar))
           {
            phase = 0;
            digitFound = TRUE;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
         else if ((inchar == '+') || (inchar == '-'))
           {
            phase = 0;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
         else if (inchar == '.')
           {
            processFloat = TRUE;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
            phase = 1;
           }
         else if ((inchar == 'E') || (inchar == 'e'))
           {
            processFloat = TRUE;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
            phase = 2;
           }
         else if ( (inchar == '<') || (inchar == '"') ||
                   (inchar == '(') || (inchar == ')') ||
                   (inchar == '&') || (inchar == '|') || (inchar == '~') ||
                   (inchar == ' ') || (inchar == ';') ||
                   (isprint(inchar) == 0) )
           { phase = 5; }
         else
           {
            phase = 9;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
        }
      else if (phase == 0)
        {
         if (isdigit(inchar))
           {
            digitFound = TRUE;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
         else if (inchar == '.')
           {
            processFloat = TRUE;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
            phase = 1;
           }
         else if ((inchar == 'E') || (inchar == 'e'))
           {
            processFloat = TRUE;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
            phase = 2;
           }
         else if ( (inchar == '<') || (inchar == '"') ||
                   (inchar == '(') || (inchar == ')') ||
                   (inchar == '&') || (inchar == '|') || (inchar == '~') ||
                   (inchar == ' ') || (inchar == ';') ||
                   ((isprint(inchar) == 0) && (! IsUTF8MultiByteStart(inchar))) )
           { phase = 5; }
         else
           {
            phase = 9;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
        }
      else if (phase == 1)
        {
         if (isdigit(inchar))
           {
            digitFound = TRUE;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
         else if ((inchar == 'E') || (inchar == 'e'))
           {
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
            phase = 2;
           }
         else if ( (inchar == '<') || (inchar == '"') ||
                   (inchar == '(') || (inchar == ')') ||
                   (inchar == '&') || (inchar == '|') || (inchar == '~') ||
                   (inchar == ' ') || (inchar == ';') ||
                   ((isprint(inchar) == 0) && (! IsUTF8MultiByteStart(inchar))) )
           { phase = 5; }
         else
           {
            phase = 9;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
        }
      else if (phase == 2)
        {
         if (isdigit(inchar))
           {
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
            phase = 3;
           }
         else if ((inchar == '+') || (inchar == '-'))
           {
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
            phase = 3;
           }
         else if ( (inchar == '<') || (inchar == '"') ||
                   (inchar == '(') || (inchar == ')') ||
                   (inchar == '&') || (inchar == '|') || (inchar == '~') ||
                   (inchar == ' ') || (inchar == ';') ||
                   ((isprint(inchar) == 0) && (! IsUTF8MultiByteStart(inchar))) )
           {
            digitFound = FALSE;
            phase = 5;
           }
         else
           {
            phase = 9;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
        }
      else if (phase == 3)
        {
         if (isdigit(inchar))
           {
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
         else if ( (inchar == '<') || (inchar == '"') ||
                   (inchar == '(') || (inchar == ')') ||
                   (inchar == '&') || (inchar == '|') || (inchar == '~') ||
                   (inchar == ' ') || (inchar == ';') ||
                   ((isprint(inchar) == 0) && (! IsUTF8MultiByteStart(inchar))) )
           {
            if ((ScannerData(theEnv)->GlobalString[count-1] == '+') || (ScannerData(theEnv)->GlobalString[count-1] == '-'))
              { digitFound = FALSE; }
            phase = 5;
           }
         else
           {
            phase = 9;
            ScannerData(theEnv)->GlobalString = ExpandStringWithChar(theEnv,inchar,ScannerData(theEnv)->GlobalString,&ScannerData(theEnv)->GlobalPos,&ScannerData(theEnv)->GlobalMax,ScannerData(theEnv)->GlobalMax+80);
            count++;
           }
        }

      if ((phase != 5) && (phase != 9))
        { inchar = EnvGetcRouter(theEnv,logicalName); }
     }

   if (phase == 9)
     {
      theToken->value = (void *) ScanSymbol(theEnv,logicalName,count,&type);
      theToken->type = type;
      theToken->printForm = ValueToString(theToken->value);
      return;
     }

   /*=======================================*/
   /* Stuff last character back into buffer */
   /* and return the number.                */
   /*=======================================*/

   EnvUngetcRouter(theEnv,inchar,logicalName);

   if (! digitFound)
     {
      theToken->type = SYMBOL;
      theToken->value = (void *) EnvAddSymbol(theEnv,ScannerData(theEnv)->GlobalString);
      theToken->printForm = ValueToString(theToken->value);
      return;
     }

   if (processFloat)
     {
      fvalue = atof(ScannerData(theEnv)->GlobalString);
      theToken->type = FLOAT;
      theToken->value = (void *) EnvAddDouble(theEnv,fvalue);
      theToken->printForm = FloatToString(theEnv,ValueToDouble(theToken->value));
     }
   else
     {
#if WIN_MVC
      lvalue = _strtoi64(ScannerData(theEnv)->GlobalString,NULL,10);
#else
      lvalue = strtoll(ScannerData(theEnv)->GlobalString,NULL,10);
#endif
      if ((lvalue == LLONG_MAX) || (lvalue == LLONG_MIN))
        {
         PrintWarningID(theEnv,"SCANNER",1,FALSE);
         EnvPrintRouter(theEnv,WWARNING,"Over or underflow of long long integer.\n");
        }
      theToken->type = INTEGER;
      theToken->value = (void *) EnvAddLong(theEnv,lvalue);
      theToken->printForm = LongIntegerToString(theEnv,ValueToLong(theToken->value));
     }

   return;
  }

/***********************************************************/
/* CopyToken: Copies values of one token to another token. */
/***********************************************************/
globle void CopyToken(
  struct token *destination,
  struct token *source)
  {
   destination->type = source->type;
   destination->value = source->value;
   destination->printForm = source->printForm;
  }

/****************************************/
/* ResetLineCount: Resets the scanner's */
/*   line count to zero.                */
/****************************************/
globle void ResetLineCount(
  void *theEnv)
  {
   ScannerData(theEnv)->LineCount = 0;
  }

/****************************************************/
/* GettLineCount: Returns the scanner's line count. */
/****************************************************/
globle long GetLineCount(
  void *theEnv)
  {
   return(ScannerData(theEnv)->LineCount);
  }

/**********************************/
/* IncrementLineCount: Increments */
/*   the scanner's line count.    */
/**********************************/
globle void IncrementLineCount(
  void *theEnv)
  {
   ScannerData(theEnv)->LineCount++;
  }

/**********************************/
/* DecrementLineCount: Decrements */
/*   the scanner's line count.    */
/**********************************/
globle void DecrementLineCount(
  void *theEnv)
  {
   ScannerData(theEnv)->LineCount--;
  }
