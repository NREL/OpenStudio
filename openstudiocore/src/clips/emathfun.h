   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*          EXTENDED MATH FUNCTIONS HEADER FILE        */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for numerous extended math     */
/*   functions including cos, sin, tan, sec, csc, cot, acos, */
/*   asin, atan, asec, acsc, acot, cosh, sinh, tanh, sech,   */
/*   csch, coth, acosh, asinh, atanh, asech, acsch, acoth,   */
/*   mod, exp, log, log10, sqrt, pi, deg-rad, rad-deg,       */
/*   deg-grad, grad-deg, **, and round.                      */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_emathfun

#define _H_emathfun

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _EMATHFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           ExtendedMathFunctionDefinitions(void *theEnv);
#if EXTENDED_MATH_FUNCTIONS
   LOCALE double                         CosFunction(void *);
   LOCALE double                         SinFunction(void *);
   LOCALE double                         TanFunction(void *);
   LOCALE double                         SecFunction(void *);
   LOCALE double                         CscFunction(void *);
   LOCALE double                         CotFunction(void *);
   LOCALE double                         AcosFunction(void *);
   LOCALE double                         AsinFunction(void *);
   LOCALE double                         AtanFunction(void *);
   LOCALE double                         AsecFunction(void *);
   LOCALE double                         AcscFunction(void *);
   LOCALE double                         AcotFunction(void *);
   LOCALE double                         CoshFunction(void *);
   LOCALE double                         SinhFunction(void *);
   LOCALE double                         TanhFunction(void *);
   LOCALE double                         SechFunction(void *);
   LOCALE double                         CschFunction(void *);
   LOCALE double                         CothFunction(void *);
   LOCALE double                         AcoshFunction(void *);
   LOCALE double                         AsinhFunction(void *);
   LOCALE double                         AtanhFunction(void *);
   LOCALE double                         AsechFunction(void *);
   LOCALE double                         AcschFunction(void *);
   LOCALE double                         AcothFunction(void *);
   LOCALE long long                      RoundFunction(void *);
   LOCALE void                           ModFunction(void *,DATA_OBJECT_PTR);
   LOCALE double                         ExpFunction(void *);
   LOCALE double                         LogFunction(void *);
   LOCALE double                         Log10Function(void *);
   LOCALE double                         SqrtFunction(void *);
   LOCALE double                         PiFunction(void *);
   LOCALE double                         DegRadFunction(void *);
   LOCALE double                         RadDegFunction(void *);
   LOCALE double                         DegGradFunction(void *);
   LOCALE double                         GradDegFunction(void *);
   LOCALE double                         PowFunction(void *);
#endif

#endif



