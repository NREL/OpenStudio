   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.24  07/01/05            */
   /*                                                     */
   /*               I/O FUNCTIONS HEADER FILE             */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.24: Added the get-char function.                   */
/*                                                           */
/*            Moved IllegalLogicalNameMessage function to    */
/*            argacces.c.                                    */
/*                                                           */
/*************************************************************/

#ifndef _H_iofun

#define _H_iofun

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _IOFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           IOFunctionDefinitions(void *);
#if IO_FUNCTIONS
   LOCALE intBool                        SetFullCRLF(void *,intBool);
   LOCALE void                           PrintoutFunction(void *);
   LOCALE void                           ReadFunction(void *,DATA_OBJECT_PTR);
   LOCALE int                            OpenFunction(void *);
   LOCALE int                            CloseFunction(void *);
   LOCALE int                            GetCharFunction(void *);
   LOCALE void                           ReadlineFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                          *FormatFunction(void *);
   LOCALE int                            RemoveFunction(void *);
   LOCALE int                            RenameFunction(void *);
   LOCALE void                           SetLocaleFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           ReadNumberFunction(void *,DATA_OBJECT_PTR);
#endif

#endif






