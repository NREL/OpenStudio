   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*              COMMAND LINE HEADER FILE               */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides a set of routines for processing        */
/*   commands entered at the top level prompt.               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Refactored several functions and added         */
/*            additional functions for use by an interface   */
/*            layered on top of CLIPS.                       */
/*                                                           */
/*************************************************************/

#ifndef _H_commline

#define _H_commline

#define COMMANDLINE_DATA 40

struct commandLineData
  { 
   int EvaluatingTopLevelCommand;
   int HaltCommandLoopBatch;
#if ! RUN_TIME
   struct expr *CurrentCommand;
   char *CommandString;
   size_t MaximumCharacters;
   int ParsingTopLevelCommand;
   char *BannerString;
   int (*EventFunction)(void *);
   int (*AfterPromptFunction)(void *);
   int (*BeforeCommandExecutionFunction)(void *);
#endif
  };

#define CommandLineData(theEnv) ((struct commandLineData *) GetEnvironmentData(theEnv,COMMANDLINE_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _COMMLINE_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           InitializeCommandLineData(void *);
   LOCALE int                            ExpandCommandString(void *,int);
   LOCALE void                           FlushCommandString(void *);
   LOCALE void                           SetCommandString(void *,char *);
   LOCALE void                           AppendCommandString(void *,char *);
   LOCALE char                          *GetCommandString(void *);
   LOCALE int                            CompleteCommand(char *);
   LOCALE void                           CommandLoop(void *);
   LOCALE void                           CommandLoopBatch(void *);
   LOCALE void                           CommandLoopBatchDriver(void *);
   LOCALE void                           PrintPrompt(void *);
   LOCALE void                           PrintBanner(void *);
   LOCALE void                           SetAfterPromptFunction(void *,int (*)(void *));
   LOCALE void                           SetBeforeCommandExecutionFunction(void *,int (*)(void *));
   LOCALE intBool                        RouteCommand(void *,char *,int);
   LOCALE int                          (*SetEventFunction(void *,int (*)(void *)))(void *);
   LOCALE intBool                        TopLevelCommand(void *);
   LOCALE void                           AppendNCommandString(void *,char *,unsigned);
   LOCALE void                           SetNCommandString(void *,char *,unsigned);
   LOCALE char                          *GetCommandCompletionString(void *,char *,size_t);
   LOCALE intBool                        ExecuteIfCommandComplete(void *);
   LOCALE void                           CommandLoopOnceThenBatch(void *);
   LOCALE intBool                        CommandCompleteAndNotEmpty(void *);
   LOCALE void                           SetHaltCommandLoopBatch(void *,int);
   LOCALE int                            GetHaltCommandLoopBatch(void *);

#endif





