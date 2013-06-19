   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  06/05/06            */
   /*                                                     */
   /*              FILE COMMANDS HEADER FILE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for file commands including    */
/*   batch, dribble-on, dribble-off, save, load, bsave, and  */
/*   bload.                                                  */
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
/*************************************************************/

#ifndef _H_filecom

#define _H_filecom

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FILECOM_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define DribbleActive() EnvDribbleActive(GetCurrentEnvironment())
#define DribbleOn(a) EnvDribbleOn(GetCurrentEnvironment(),a)
#define DribbleOff() EnvDribbleOff(GetCurrentEnvironment())
#define BatchStar(a) EnvBatchStar(GetCurrentEnvironment(),a)

   LOCALE void                           FileCommandDefinitions(void *);
   LOCALE intBool                        EnvDribbleOn(void *,char *);
   LOCALE intBool                        EnvDribbleActive(void *);
   LOCALE intBool                        EnvDribbleOff(void *);
   LOCALE void                           SetDribbleStatusFunction(void *,int (*)(void *,int));
   LOCALE int                            LLGetcBatch(void *,char *,int);
   LOCALE int                            Batch(void *,char *);
   LOCALE int                            OpenBatch(void *,char *,int);
   LOCALE int                            OpenStringBatch(void *,char *,char *,int);
   LOCALE int                            RemoveBatch(void *);
   LOCALE intBool                        BatchActive(void *);
   LOCALE void                           CloseAllBatchSources(void *);
   LOCALE int                            BatchCommand(void *);
   LOCALE int                            BatchStarCommand(void *);
   LOCALE int                            EnvBatchStar(void *,char *);
   LOCALE int                            LoadCommand(void *);
   LOCALE int                            LoadStarCommand(void *);
   LOCALE int                            SaveCommand(void *);
   LOCALE int                            DribbleOnCommand(void *);
   LOCALE int                            DribbleOffCommand(void *);

#endif






