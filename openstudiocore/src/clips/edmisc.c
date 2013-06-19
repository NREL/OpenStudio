   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*              CLIPS Version 6.24  06/05/06           */
   /*                                                     */
   /*                                                     */
   /*******************************************************/
   
/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*      6.23: Changed name of variable log to logName        */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Corrected code generating compilation          */
/*            warnings.                                      */
/*                                                           */
/*************************************************************/

/*
 * This file contains the command processing functions for a number of
 * commands, including the search, spawn, and compile functions.
 */

#include "setup.h"

#if  EMACS_EDITOR && ! RUN_TIME

#define _EDMISC_SOURCE_
#include "ed.h"
#include "cstrcpsr.h"

static int     tabsize;                        /* Tab size (0: use real tabs)  */

/* -----------------------------
 *  Spawn function setups
 * -----------------------------
 */

#if     VAX_VMS
#define EFN     0                               /* Event flag.          */

#include        <ssdef.h>                       /* Random headers.      */
#include        <stsdef.h>
#include        <descrip.h>
#include        <iodef.h>

extern  int     oldmode[2];                      /* In "termio.c"        */
extern  int     newmode[2];                      /* In "termio.c"        */
extern  short   iochan;                          /* In "termio.c"        */
#endif

#if     WIN_MVC || WIN_BTC || WIN_GCC
#include        <dos.h>
#endif

#if     UNIX_7 || UNIX_V || LINUX || DARWIN
/*
extern void sleep(int);
*/
#include        <unistd.h>
#include        <signal.h>
#endif

/* =========================================================================
 *                        COMPILE CLIPS RULES FUNCTIONS
 * =========================================================================
 */

#define MAX_COMPILE_LINE 161

static int cur_col;
static long region_size;
static LINE *linep;
static int loffs;
static int CompileSuccess = 1;
static char CompileLine[MAX_COMPILE_LINE];
static int CompileLineIndex = 0;

/********************************************************/
/*    compile a region of a file 			*/
/********************************************************/

#if WIN_BTC
#pragma argsused
#endif
globle int compile_region(
  void *theEnv,
  int f,
  int n)
  {
    register int    s;
    REGION   region;

   if (curbp == CompileBufferp)
     {
      mlwrite("Cannot compile this region!");
      return(0);
     }
   cur_col = 20;

   /* Mark the region   */

   if (( s = getregion(&region)) != TRUE)
      return(s);
   if ((lastflag&CFKILL) == 0)
     kdelete(theEnv);
   thisflag |= CFKILL;
   linep = region.r_linep;
   loffs = region.r_offset;
   region_size =  region.r_size;
   if (region_size == 0)
    {
     mlwrite(" Region is empty ");
     return(0);
    }

   mlwrite("Compiling Region...");

   /* Create IO router for the region (CLIPS.C) */

   EnvAddRouter(theEnv,"emacs_region",90,region_fnd,
		NULL,region_getc,region_ungetc,NULL);

   /* COMPILE */

   if (get_compile(theEnv,"emacs_region","Emacs_region") == 0)
     mlwrite("Error while forming compilation buffer!");
   else
     mlwrite("Compilation done.");
   return (TRUE);
  }


/*****************************************************
 *  This function will compile a file form emacs     *
 *****************************************************/

#if WIN_BTC
#pragma argsused
#endif
globle int compile_file(
  void *theEnv,
  int f,
  int n)
  {
   if (curbp == CompileBufferp)
     {
      mlwrite("Cannot compile this buffer!");
      return(0);
     }

   cur_col = 27;
   mlwrite("Compiling Current Buffer...");
   linep = lforw(curbp->b_linep);
   loffs = 0;

   /*  Create a IO router for the file   (CLIPS.C)  */

   EnvAddRouter(theEnv,"emacs_file",90,buffer_fnd,
		NULL,buffer_getc,buffer_ungetc,NULL);

   /*   COMPILE   */

   if (get_compile(theEnv,"emacs_file","Emacs_buffer") == 0)
     mlwrite("Error while forming compilation buffer!");
   else
     mlwrite("Compilation done.");
   return (TRUE);
  }


/**********************************************************
 *  Compiles the whole buffer or just a region of a buffer*
 **********************************************************/
globle int get_compile(
  void *theEnv,
  char *str1,
  char *str2)
  {
#if (! RUN_TIME) && (! BLOAD_ONLY)
   register WINDOW *wp;
   register BUFFER *bp;

   CompileSuccess = 1;
   CompileBufferp->b_flag &= ~BFCHG;           /* Don't complain!      */
   if (bclear(theEnv,CompileBufferp) != TRUE)         /* Blow old text away   */
     return (0);
   CompileLineIndex = 0;
   CompileLine[0] = '\0';

   EnvActivateRouter(theEnv,str1);	
   EnvActivateRouter(theEnv,"cmp_router");
   SetPrintWhileLoading(theEnv,TRUE);
   LoadConstructsFromLogicalName(theEnv,str2);
   DestroyPPBuffer(theEnv);
   /* Flush last diagnostic line (if any) to buffer */
   if (CompileLineIndex != 0)
     addline(theEnv,CompileBufferp,CompileLine);
   EnvDeactivateRouter(theEnv,str1);	
   EnvDeactivateRouter(theEnv,"cmp_router");
   SetPrintWhileLoading(theEnv,FALSE);
   EnvDeleteRouter(theEnv,str1);

   genstrcpy(CompileBufferp->b_fname, "");
   if (CompileBufferp->b_nwnd == 0) {          /* Not on screen yet.   */
           if ((wp=wpopup(theEnv)) == NULL)
                   return (0);
           bp = wp->w_bufp;
           if (--bp->b_nwnd == 0) {
                   bp->b_dotp  = wp->w_dotp;
                   bp->b_doto  = wp->w_doto;
                   bp->b_markp = wp->w_markp;
                   bp->b_marko = wp->w_marko;
           }
           wp->w_bufp  = CompileBufferp;
           ++CompileBufferp->b_nwnd;
   }
   wp = wheadp;
   while (wp != NULL) {
           if (wp->w_bufp == CompileBufferp) {
                   wp->w_linep = lforw(CompileBufferp->b_linep);
                   wp->w_dotp  = lforw(CompileBufferp->b_linep);
                   wp->w_doto  = 0;
                   wp->w_markp = NULL;
                   wp->w_marko = 0;
                   wp->w_flag |= WFMODE|WFHARD;
           }
           wp = wp->w_wndp;
   }
   return(CompileSuccess);
#else
   return(0);
#endif		
  }

/****************************************************************
 *  This function will compare the logical name with names in 	*
 *  the IO Router list						*
 ****************************************************************/

globle int region_fnd(
  void *theEnv,
  char *log_name)
  {
   if (strcmp("Emacs_region",log_name)== 0)
     { return(TRUE); }
   return(FALSE);
  }

/****************************************************************
 *   This function will return a character from the file which  *
 *   is referenced by the logical name				*
 ****************************************************************/

#if WIN_BTC
#pragma argsused
#endif
globle int region_getc(
  void *theEnv,
char *log_name)
{
  int c;

  if (region_size <= 0)		   /* If end of region then EXIT */
       return(EOF);
  if (loffs == llength(linep))     /* End of line  */
       {
	  c = '\n';		   /* go to next line */
	  linep = lforw(linep);
	  loffs = 0;
       }
  else
       c = lgetc(linep,loffs++);   /* If everything is OK then get a character
					from the file */
  region_size--;
  return(c);
}

/*******************************************************
 * This function will move the cursor back one charater*
 *******************************************************/

#if WIN_BTC
#pragma argsused
#endif
globle int region_ungetc(
  void *theEnv,
int c,
char *log_name)

{
     if (c == EOF)
       return(1);
     if (loffs <= 0)
       {
	 linep = lback(linep);
	 loffs = llength(linep);
       }
     else
	loffs--;
     region_size++;
     return(1);
}

/**************************************************************
 * this function will search through the IO router list and   *
 * find a name that matches with the logical name,which       *
 * represents the buffer to be compiled	.		      *
 **************************************************************/

globle int buffer_fnd(
  void *theEnv,
  char *log_name)
{
    if(strcmp("Emacs_buffer",log_name)== 0)
    	return(TRUE);
    return(FALSE);
 }


/****************************************************************
 *   This function will return a character from the file which  *
 * is referred by the logical name				*
 ****************************************************************/

#if WIN_BTC
#pragma argsused
#endif
globle int buffer_getc(
  void *theEnv,
  char *log_name)
{
  int c;
  if (linep == curbp->b_linep)       /* End of file */
    return(EOF);
  if (loffs == llength(linep))       /* End of line */
     {
	linep = lforw(linep);        /* Move to next line */
	if (linep == curbp->b_linep) /* and if end of file then exit */
            return(EOF);	     /* else reset the cursor */
        loffs = 0;
        c = '\n';
     }
  else

        c = lgetc(linep,loffs++);    /* if everything is OK then get a character
					from the file */
  return(c);
}

/*************************************************************
 * this function will move the cursor back to one character  *
 *************************************************************/

#if WIN_BTC
#pragma argsused
#endif
globle int buffer_ungetc(
  void *theEnv,
  int    c,
  char  *logical_name)
{
    if (c == EOF)
      return(1);
    if (loffs == 0)
      {
	linep = lback(linep);
        loffs = llength(linep);
      }
    else
	loffs--;
    return(1);
}

globle int query_cmp(
  void *theEnv,
  char *logName)
{
   if((strcmp(logName,"wdialog") == 0) ||
      (strcmp(logName,"wtrace")  == 0) ||
      (strcmp(logName,"wwarning")  == 0) ||
      (strcmp(logName,"werror")  == 0))
       return(TRUE);
   else
       return(FALSE);
}

globle int print_cmp(
  void *theEnv,
  char *logName, 
  char *str)
  {
   register int i;

   if (CompileSuccess == 0)
     return(1);
   for (i = 0 ; str[i] != '\0' ; i++)
     {
      if ((str[i] == '\n') || (str[i] == '\r'))
        {
         addline(theEnv,CompileBufferp,CompileLine);
         CompileLineIndex = 0;
         CompileLine[0] = '\0';
        }
      else if (CompileLineIndex < (MAX_COMPILE_LINE-1))
        {
         CompileLine[CompileLineIndex++] = str[i];
         CompileLine[CompileLineIndex] = '\0';
        }
      else
        {
         addline(theEnv,CompileBufferp,CompileLine);
         CompileLineIndex = 1;
         CompileLine[0] = str[i];
         CompileLine[1] = '\0';
        }
     }
   return(1);
  }

globle void init_cmp_router(
  void *theEnv)
  {
   EnvAddRouter(theEnv,"cmp_router",
	      20,
	      query_cmp,
	      print_cmp,
	      NULL,
	      NULL,
	      NULL
	      );
  }

globle void kill_cmp_router(
  void *theEnv)
  {
   EnvDeleteRouter(theEnv,"cmp_router");
  }

/* =========================================================================
 *                              MISC FUNCTIONS
 * =========================================================================
 */

/*
 * Set fill column to n.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int setfillcol(
  void *theEnv,
  int f,
  int n)
  {
   fillcol = n;
   return(TRUE);
  }

/*
 * Display the current position of the cursor; the current
 * column, the current line and the total number of lines in the file.
 * Bound to "C-X =".      CJC, 8-1-86
 */
#if WIN_BTC
#pragma argsused
#endif
globle int showcpos(
  void *theEnv,
  int f,
  int n)
  {
        register int    cline;
        register int    col;
        register int    tline;

        col   = getccol(FALSE);                 /* Get real column.     */
        cline = getcline();                     /* Get real line #      */
        tline = cntlines();                     /* Get total # lines    */
        mlwrite("MicroEMACS Version %s   col: %d  line: %d of %d"
                ,VERSION_NUM, col+1, cline, tline);
        return (TRUE);
  }

/*
 * Return current column.  Stop at first non-blank given TRUE argument.
 */
globle int getccol(
int bflg)
{
        register int c, i, col;
        col = 0;
        for (i=0; i< curwp->w_doto; ++i) {
                c = lgetc(curwp->w_dotp, i);
                if (c!=' ' && c!='\t' && bflg)
                        break;
                if (c == '\t')
                        col |= 0x07;
                else if (c<0x20 || c==0x7F)
                        ++col;
                ++col;
        }
        return(col);
}

/*
 *  Return current line number in file
 */
globle int getcline()
{
   int i;
   struct LINE *clp;

   i = 0;
   clp = curbp->b_linep;
   while(clp != curwp->w_dotp) {
        i++;
        clp = lforw(clp);
        }
   if (i == 0)
     return(cntlines());
   return(i);
}

/*
 *  Return total number of lines in file
 */
globle int cntlines()
{
   int i;
   struct LINE *clp;

   i = 1;
   clp = lforw(curbp->b_linep);
   while( clp != curbp->b_linep) {
        i++;
        clp = lforw(clp);
        }

   return(i);
}

/*
 * Go to a specified line number
 */
#if WIN_BTC
#pragma argsused
#endif
globle int gotoline(
  void *theEnv,
  int f,
  int n)
  {
   register int line;
   char buf[5];
   register int i;
   register int s;
   struct LINE *clp;

   if ((s=mlreply(theEnv,"Goto line: ", buf, 5)) != TRUE)
        return (s);

   if((line = atoi(buf)) <= 0) {
      mlwrite("Invalid line number!");
      return(FALSE);
      }
   else if(line > cntlines()) {
      mlwrite("Not that many lines in buffer!");
      return(FALSE);
      }

   i = 0;
   clp = lforw(curbp->b_linep);
   while(i < line - 1) {
        i++;
        clp = lforw(clp);
        }

   curwp->w_dotp  = clp;
   curwp->w_doto  = 0;
   curwp->w_flag |= WFMOVE;
   return (TRUE);
  }

/*
 * Twiddle the two characters on either side of dot. If dot is at the end of
 * the line twiddle the two characters before it. Return with an error if dot
 * is at the beginning of line; it seems to be a bit pointless to make this
 * work. This fixes up a very common typo with a single stroke. Normally bound
 * to "C-T". This always works within a line, so "WFEDIT" is good enough.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int twiddle(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *dotp;
        register int    doto;
        register int    cl;
        register int    cr;

        dotp = curwp->w_dotp;
        doto = curwp->w_doto;
        if (doto==llength(dotp) && --doto<0)
                return (FALSE);
        cr = lgetc(dotp, doto);
        if (--doto < 0)
                return (FALSE);
        cl = lgetc(dotp, doto);
        lputc(dotp, doto+0, cr);
        lputc(dotp, doto+1, cl);
        lchange(WFEDIT);
        return (TRUE);
  }

/*
 * Quote the next character, and insert it into the buffer. All the characters
 * are taken literally, with the exception of the newline, which always has
 * its line splitting meaning. The character is always read, even if it is
 * inserted 0 times, for regularity. Bound to "M-Q" (for me) and "C-Q" (for
 * Rich, and only on terminals that don't need XON-XOFF).
 */
#if WIN_BTC
#pragma argsused
#endif
globle int quote(
  void *theEnv,
  int f,
  int n)
  {
        register int    s;
        register int    c;

        c = (*term.t_getchar)();
        if (n < 0)
                return (FALSE);
        if (n == 0)
                return (TRUE);
        if (c == '\n') {
                do {
                        s = lnewline(theEnv);
                } while (s==TRUE && --n);
                return (s);
        }
        return (linsert(theEnv,n, c));
  }

/*
 * Set tab size if given non-default argument (n <> 1).  Otherwise, insert a
 * tab into file.  If given argument, n, of zero, change to true tabs.
 * If n > 1, simulate tab stop every n-characters using spaces. This has to be
 * done in this slightly funny way because the tab (in ASCII) has been turned
 * into "C-I" (in 10 bit code) already. Bound to "C-I".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int tab(
  void *theEnv,
  int f,
  int n)
  {
        if (n < 0)
                return (FALSE);
        if (n == 0 || n > 1) {
                tabsize = n;
                return(TRUE);
        }
        if (! tabsize)
                return(linsert(theEnv,1, '\t'));
        return(linsert(theEnv,tabsize - (getccol(FALSE) % tabsize), ' '));
  }

/*
 * Open up some blank space. The basic plan is to insert a bunch of newlines,
 * and then back up over them. Everything is done by the subcommand
 * procerssors. They even handle the looping. Normally this is bound to "C-O".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int openline(
  void *theEnv,
  int f,
  int n)
  {
        register int    i;
        register int    s;

        if (n < 0)
                return (FALSE);
        if (n == 0)
                return (TRUE);
        i = n;                                  /* Insert newlines.     */
        do {
                s = lnewline(theEnv);
        } while (s==TRUE && --i);
        if (s == TRUE)                          /* Then back up overtop */
                s = backchar(theEnv,f, n);             /* of them all.         */
        return (s);
  }

/*
 * Insert a newline. Bound to "C-M". If you are at the end of the line and the
 * next line is a blank line, just move into the blank line. This makes "C-O"
 * and "C-X C-O" work nicely, and reduces the ammount of screen update that
 * has to be done. This would not be as critical if screen update were a lot
 * more efficient.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int newline(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *lp;
        register int    s;

        if (n < 0)
                return (FALSE);
        while (n--) {
                lp = curwp->w_dotp;
                if (llength(lp) == curwp->w_doto
                && lp != curbp->b_linep
                && llength(lforw(lp)) == 0) {
                        if ((s=forwchar(theEnv,FALSE, 1)) != TRUE)
                                return (s);
                } else if ((s=lnewline(theEnv)) != TRUE)
                        return (s);
        }
        return (TRUE);
  }

/*
 * Delete blank lines around dot. What this command does depends if dot is
 * sitting on a blank line. If dot is sitting on a blank line, this command
 * deletes all the blank lines above and below the current line. If it is
 * sitting on a non blank line then it deletes all of the blank lines after
 * the line. Normally this command is bound to "C-X C-O". Any argument is
 * ignored.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int deblank(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *lp1;
        register LINE   *lp2;
        long nld;

        lp1 = curwp->w_dotp;
        while (llength(lp1)==0 && (lp2=lback(lp1))!=curbp->b_linep)
                lp1 = lp2;
        lp2 = lp1;
        nld = 0;
        while ((lp2=lforw(lp2))!=curbp->b_linep && llength(lp2)==0)
                ++nld;
        if (nld == 0)
                return (TRUE);
        curwp->w_dotp = lforw(lp1);
        curwp->w_doto = 0;
        return (ldelete(theEnv,nld,FALSE));
  }

/*
 * Insert a newline, then enough tabs and spaces to duplicate the indentation
 * of the previous line. Assumes tabs are every eight characters. Quite simple.
 * Figure out the indentation of the current line. Insert a newline by calling
 * the standard routine. Insert the indentation by inserting the right number
 * of tabs and spaces. Return TRUE if all ok. Return FALSE if one of the
 * subcomands failed. Normally bound to "C-J".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int indent(
  void *theEnv,
  int f,
  int n)
{
        register int    nicol;
        register int    c;
        register int    i;

        if (n < 0)
                return (FALSE);
        while (n--) {
                nicol = 0;
                for (i=0; i<llength(curwp->w_dotp); ++i) {
                        c = lgetc(curwp->w_dotp, i);
                        if (c!=' ' && c!='\t')
                                break;
                        if (c == '\t')
                                nicol |= 0x07;
                        ++nicol;
                }
                if (lnewline(theEnv) == FALSE
                || ((i=nicol/8)!=0 && linsert(theEnv,i, '\t')==FALSE)
                || ((i=nicol%8)!=0 && linsert(theEnv,i,  ' ')==FALSE))
                        return (FALSE);
        }
        return (TRUE);
}

/*
 * Delete forward. This is real easy, because the basic delete routine does
 * all of the work. Watches for negative arguments, and does the right thing.
 * If any argument is present, it kills rather than deletes, to prevent loss
 * of text if typed with a big argument. Normally bound to "C-D".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int forwdel(
  void *theEnv,
  int f,
  int n)
  {
        if (n < 0)
                return (backdel(theEnv,f, -n));
        if (f != FALSE) {                       /* Really a kill.       */
                if ((lastflag&CFKILL) == 0)
                        kdelete(theEnv);
                thisflag |= CFKILL;
        }
        return (ldelete(theEnv,(long) n, f));
  }

/*
 * Delete backwards. This is quite easy too, because it's all done with other
 * functions. Just move the cursor back, and delete forwards. Like delete
 * forward, this actually does a kill if presented with an argument. Bound to
 * both "RUBOUT" and "C-H".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int backdel(
  void *theEnv,
  int f,
  int n)
  {
        register int    s;

        if (n < 0)
                return (forwdel(theEnv,f, -n));
        if (f != FALSE) {                       /* Really a kill.       */
                if ((lastflag&CFKILL) == 0)
                        kdelete(theEnv);
                thisflag |= CFKILL;
        }
        if ((s=backchar(theEnv,f, n)) == TRUE)
                s = ldelete(theEnv,(long) n, f);
        return (s);
  }

/*
 * Kill text. If called without an argument, it kills from dot to the end of
 * the line, unless it is at the end of the line, when it kills the newline.
 * If called with an argument of 0, it kills from the start of the line to dot.
 * If called with a positive argument, it kills from dot forward over that
 * number of newlines. If called with a negative argument it kills backwards
 * that number of newlines. Normally bound to "C-K".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int kill_fwd(
  void *theEnv,
  int f,
  int n)
  {
        register int    chunk;
        register LINE   *nextp;

        if ((lastflag&CFKILL) == 0)             /* Clear kill buffer if */
                kdelete(theEnv);                      /* last wasn't a kill.  */
        thisflag |= CFKILL;
        if (f == FALSE) {
                chunk = llength(curwp->w_dotp)-curwp->w_doto;
                if (chunk == 0)
                        chunk = 1;
        } else if (n == 0) {
                chunk = curwp->w_doto;
                curwp->w_doto = 0;
        } else if (n > 0) {
                chunk = llength(curwp->w_dotp)-curwp->w_doto+1;
                nextp = lforw(curwp->w_dotp);
                while (--n) {
                        if (nextp == curbp->b_linep)
                                return (FALSE);
                        chunk += llength(nextp)+1;
                        nextp = lforw(nextp);
                }
        } else {
                mlwrite("neg kill");
                return (FALSE);
        }
        return (ldelete(theEnv,(long) chunk, TRUE));
  }

/*
 * Yank text back from the kill buffer. This is really easy. All of the work
 * is done by the standard insert routines. All you do is run the loop, and
 * check for errors. Bound to "C-Y". The blank lines are inserted with a call
 * to "newline" instead of a call to "lnewline" so that the magic stuff that
 * happens when you type a carriage return also happens when a carriage return
 * is yanked back from the kill buffer.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int yank(
  void *theEnv,
  int f,
  int n)
  {
        register int    c;
        register int    i;

        if (n < 0)
                return (FALSE);
        while (n--) {
                i = 0;
                while ((c=kremove(i)) >= 0) {
                        if (c == '\n') {
                                if (newline(theEnv,FALSE, 1) == FALSE)
                                        return (FALSE);
                        } else {
                                if (linsert(theEnv,1, c) == FALSE)
                                        return (FALSE);
                        }
                        ++i;
                }
        }
        return (TRUE);
  }

/* =========================================================================
 *                           SEARCH FUNCTIONS
 * =========================================================================
 */

/*
 * The functions in this section implement commands that search in the forward
 * and backward directions. There are no special characters in the search
 * strings. Probably should have a regular expression search, or something
 * like that.
 *
 * They also implement commands that do the followings
 *  - Replaces all the occurences of a string,
 *    from the current point of cursor to the end of file,
 *    with a new string.
 *  - search for matching bracket
 *
 * REVISION HISTORY:
 *
 * ?    Steve Wilhite, 1-Dec-85
 *      - massive cleanup on code.
 *
 *      Huyen_Anh Vu Ly, 16-Dec-86
 *      - extending the capability of Emacs included the followings:
 *         -*- backward search and replace all the occurences of a string,
 *             C-X R.
 *         -*- backward search and replace some of te occurences of a string,
 *             M-R.
 *         -*- forward search and replace all the occurences of a string,
 *             C-X S.
 *         -*- forward search and replace some occurences of a string,
 *             M-S.
 *         -*- find the matching bracket for : (,),[,],{,},
 *             C-X M.
 */

/*
 * Search forward. Get a search string from the user, and search, beginning at
 * ".", for the string. If found, reset the "." to be just after the match
 * string, and [perhaps] repaint the display. Bound to "C-S".
 */

#if WIN_BTC
#pragma argsused
#endif
globle int forwsearch(
  void *theEnv,
  int f,
  int n)
    {
    register LINE *clp;
    register int cbo;
    register LINE*tlp;
    register int tbo;
    register int c;
    register char *pp;
    register int s;

    if ((s = readpattern(theEnv,"Search")) != TRUE)
        return (s);

    clp = curwp->w_dotp;
    cbo = curwp->w_doto;

    while (clp != curbp->b_linep)
        {
        if (cbo == llength(clp))
            {
            clp = lforw(clp);
            cbo = 0;
            c = '\n';
            }
        else
            c = lgetc(clp, cbo++);

        if (c == pat[0])
            {
            tlp = clp;
            tbo = cbo;
            pp  = &pat[1];

            while (*pp != 0)
                {
                if (tlp == curbp->b_linep)
                    goto fail;

                if (tbo == llength(tlp))
                    {
                    tlp = lforw(tlp);
                    tbo = 0;
                    c = '\n';
                    }
                else
                    c = lgetc(tlp, tbo++);

                if (c != *pp++)
                    goto fail;
                }

            curwp->w_dotp  = tlp;
            curwp->w_doto  = tbo;
            curwp->w_flag |= WFMOVE;
            return (TRUE);
            }
fail:;
        }

    mlwrite("Not found");
    return (FALSE);
    }

/*
 * Reverse search. Get a search string from the user, and search, starting at
 * "." and proceeding toward the front of the buffer. If found "." is left
 * pointing at the first character of the pattern [the last character that was
 * matched. Bound to "C-R".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int backsearch(
  void *theEnv,
  int f,
  int n)
    {
    register LINE *clp;
    register int cbo;
    register LINE *tlp;
    register int tbo;
    register int c;
    register char *epp;
    register char *pp;
    register int s;

    if ((s = readpattern(theEnv,"Reverse search")) != TRUE)
        return (s);

    for (epp = &pat[0]; epp[1] != 0; ++epp)
        ;

    clp = curwp->w_dotp;
    cbo = curwp->w_doto;

    for (;;)
        {
        if (cbo == 0)
            {
            clp = lback(clp);

            if (clp == curbp->b_linep)
                {
                mlwrite("Not found");
                return (FALSE);
                }

            cbo = llength(clp)+1;
            }

        if (--cbo == llength(clp))
            c = '\n';
        else
            c = lgetc(clp, cbo);

        if (c == *epp)
            {
            tlp = clp;
            tbo = cbo;
            pp  = epp;

            while (pp != &pat[0])
                {
                if (tbo == 0)
                    {
                    tlp = lback(tlp);
                    if (tlp == curbp->b_linep)
                        goto fail;

                    tbo = llength(tlp)+1;
                    }

                if (--tbo == llength(tlp))
                    c = '\n';
                else
                    c = lgetc(tlp, tbo);

                if (c != *--pp)
                    goto fail;
                }

            curwp->w_dotp  = tlp;
            curwp->w_doto  = tbo;
            curwp->w_flag |= WFMOVE;
            return (TRUE);
            }
        fail:;
        }
}

/********************************************************
 *   This function will search backward through the file*
 *   and replace all occurences of an old string by a   *
 *   new string.                                        *
 ********************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle int bkwrdrpl(
  void *theEnv,
  int f,
  int n)
    {
    LINE *clp;
    int cbo;
    LINE *tlp;
    int tbo;
    int c,count = 0;
    char *epp,buf[40];
    char *pp;
    char *pat2;
    int s;

    if ((s = readpattern(theEnv,"Rev. replace all occrs of")) != TRUE)
        return (s);
    pat2 = (char *) genalloc(theEnv,(unsigned) strlen (pat) + 1);          /* Pat2 is first pattern */
    genstrcpy(pat2,pat);
    if((s = mlreply(theEnv,"Replace with: ",pat,NPAT)) == ABORT) {
        genfree(theEnv,(void *) pat2, (unsigned) strlen(pat) + 1);
        return(s);
        }
    for (epp = &pat2[0];epp[1] != 0 ; ++epp);
    clp = curwp->w_dotp;
    cbo = curwp->w_doto;
    if (clp == curbp->b_linep)
      {
	clp = lback(clp);
	cbo = llength(clp) + 1;
      }
    while(clp != curbp->b_linep)
      {
	if (cbo <= 0)
	  {
	     clp = lback(clp);
	     if (clp != curbp->b_linep)
		cbo = llength(clp) + 1;
	  }
	if (clp == curbp->b_linep)
	  break;
	if (--cbo == llength(clp))
	  c = '\n';
	else
	  c = lgetc(clp,cbo);
	if(c == *epp)
	   {
	       	tbo = cbo;
	   	tlp = clp;
		pp = epp;
		while(pp != pat2)
		  {
		   if (tbo <= 0)
		    {
			tlp = lback(tlp);
			if (tlp != curbp->b_linep)
			  tbo = llength(tlp) + 1;
		    }
		   if ( tlp == curbp->b_linep)
			break;
		   if(--tbo == llength(tlp))
			c = '\n';
		   else
			c = lgetc(tlp,tbo);
		   if (c != *--pp)
		     break;
		  }
    		 if ((pp == pat2)&&(c == *pp))
	          {
			curwp->w_dotp = clp;
			curwp->w_doto = tbo;
			count++;
			lreplace(theEnv,pat2);
			clp = curwp->w_dotp;
			cbo = curwp->w_doto - strlen(pat);
 		  }
	      }
    	  }
     curwp->w_doto -= strlen(pat);
     curwp->w_flag |= WFMOVE;
     update();
     gensprintf(buf," %d Replacement[s] ",count);
     mlwrite(buf);
     return(TRUE);
   }


/********************************************************************
 *  This function will search backward for the occurences of an old *
 *  string and replace some of them with a new string.              *
 ********************************************************************/
#if WIN_BTC
#pragma argsused
#endif
globle int bkwrdcr(
  void *theEnv,
  int f,
  int n)
    {
    LINE *clp;
    int cbo;
    LINE *tlp;
    int tbo;
    int c;
    char *epp,*pp;
    char *pat2;
    int s;

    if ((s = readpattern(theEnv,"Rev. replace some occrs. of")) != TRUE)
        return (s);
    pat2 = (char *) genalloc(theEnv,(unsigned) strlen (pat) + 1);
    genstrcpy(pat2,pat);
    if((s = mlreply(theEnv,"Replace with: ",pat,NPAT)) == ABORT) {
        genfree(theEnv,(void *) pat2,(unsigned) strlen(pat) + 1);
        return(s);
        }
    for (epp = &pat2[0]; epp[1] != 0; ++epp)
        ;

    clp = curwp->w_dotp;
    cbo = curwp->w_doto;
    if (clp == curbp->b_linep)
        {
	  clp = lback(clp);
          cbo = llength(clp) + 1;
        }
    while (clp != curbp->b_linep)
        {
        if (cbo <= 0)
            {
            clp = lback(clp);

            if (clp != curbp->b_linep)
              cbo = llength(clp)+1;
            }
	if (clp == curbp->b_linep)
	    break;
        if (--cbo == llength(clp))
            c = '\n';
        else
            c = lgetc(clp, cbo);

        if (c == *epp)
            {
            tlp = clp;
            tbo = cbo;
            pp  = epp;

            while (pp != &pat2[0])
                {
                if (tbo <= 0)
                    {
                      tlp = lback(tlp);
                      if (tlp != curbp->b_linep)
	                  tbo = llength(tlp)+1;
                    }
		if (tlp == curbp->b_linep)
		   break;
                if (--tbo == llength(tlp))
                    c = '\n';
                else
                    c = lgetc(tlp, tbo);

                if (c != *--pp)
                    break;
                }
	    if ((pp == pat2)&&(c == *pp))
	      {
            	curwp->w_dotp = clp;
            	curwp->w_doto = tbo;
            	curwp->w_flag |= WFMOVE;

	   /*************************************************
            * Read a character from standard i/o            *
	    * if charater is a blank(comparable to Zmacs)   *
            * or 'y' or 'Y' the replacement will occur.     *
            * Else if character is 'n' or 'N' it will skip  *
 	    * and go to next occurence of string.	    *
 	    * Else exit the function                        *
	    *************************************************/

	    	mlwrite("Do you want to replace this? [y/n]");
            	update();
	    	c = (*term.t_getchar)();
            	if((c ==' ')||(c == 'y')||(c == 'Y'))
               	   {
                 	lreplace(theEnv,pat2);
                 	clp = curwp->w_dotp;
                 	cbo = curwp->w_doto - strlen(pat);
	       	    }
	        else if ((c == 'n')||(c == 'N'))
		    cbo = tbo;
	        else
		   return(TRUE);
               }
           }
       }
     curwp->w_flag |= WFMOVE;
     curwp->w_doto -= strlen(pat);
     update();
     mlwrite("No more occurrences of [%s] in buffer",pat2);
     genfree(theEnv,(void *) pat2,(unsigned) strlen(pat) + 1);
     return(TRUE);
   }


/******************************************************
 *                  FORWARD                           *
 *  Search all the occurences of a string and replace *
 *  with a new string                                 *
 ******************************************************/

#if WIN_BTC
#pragma argsused
#endif
globle int frwsr(
  void *theEnv,
  int f,
  int n)
{
   LINE *clp,*tlp;
   int cbo,tbo,c,s,count = 0 ;
   char *pp,buf[40];
   char *pat2;

 /* Read the string to be replaced */

  if((s = readpattern (theEnv,"Replace the occurences of?")) != TRUE)
    return (s);
  pat2 = (char *) genalloc (theEnv,(unsigned) strlen(pat) + 1);
  genstrcpy (pat2,pat);

 /* Read the string to replace with */

  if((s = mlreply(theEnv,"Replace with: ",pat,NPAT)) == ABORT) {
    genfree(theEnv,(void *) pat2,(unsigned) strlen(pat) + 1);
    return(s);
    }

  clp = curwp->w_dotp;
  cbo = curwp->w_doto;
  while (clp != curbp->b_linep)      /* While not eof ,search for string*/
    {
       if(cbo >= llength(clp))       /* if end of line go to new line */
       {
         clp = lforw(clp);
         cbo = 0;
         c = '\n';
       }
       else                          /* else begin to read in a new character */
         c = lgetc(clp,cbo++);
       if (c == pat2[0])             /* and compare it */
       {                             /* to the string to be replaced */
	 tlp = clp;
	 tbo = cbo;
         pp = &pat2[1];
         while (*pp)
         {
            if(tlp == curbp->b_linep)
              break;
            if(tbo >= llength(tlp))
            {
                 tlp = lforw(tlp);
                 tbo = 0;
                 c = '\n';
            }
            else
                 c = lgetc(tlp,tbo++);
            if(c != *pp)
                 break;
            pp++;
         }
         if(!(*pp))    /* if string is found replace it with new string */
           {
            curwp->w_dotp = clp;
            curwp->w_doto = cbo - 1;
            count++;
            lreplace(theEnv,pat2);
            clp = curwp->w_dotp;
            cbo = curwp->w_doto;
           }
       }

     }
     curwp->w_doto -= strlen(pat);
     curwp->w_flag |= WFMOVE;
     update();
     gensprintf(buf,"%d replacement[s]",count);
     mlwrite(buf);
     return(TRUE);

}

/***************************************************
 * Search all the occurences of a string and       *
 * replace some of them                            *
 ***************************************************/

#if WIN_BTC
#pragma argsused
#endif
globle int querysr(
  void *theEnv,
  int f,
  int n)
  {
   LINE *clp,*tlp;
   int cbo,tbo,c,s;
   char *pp;
   char *pat2;

 /* Read the string to be replaced */

  if((s = readpattern (theEnv,"Query_replace ?")) != TRUE)
    return (s);
  pat2 = (char *) genalloc(theEnv,(unsigned) strlen(pat) + 1);
  genstrcpy (pat2,pat);

 /* Read the string to replace with */

  if((s = mlreply(theEnv,"Replace with: ",pat,NPAT)) == ABORT) {
    genfree(theEnv,(void *) pat2,(unsigned) strlen(pat) + 1);
    return(s);
    }

  clp = curwp->w_dotp;
  cbo = curwp->w_doto;
  while (clp != curbp->b_linep)      /* While not eof ,search for string*/
    {
       if(cbo >= llength(clp))       /* if end of line go to new line */
       {
         clp = lforw(clp);
         cbo = 0;
         c = '\n';
       }
       else                    /* else begin to read in a new character */
         c = lgetc(clp,cbo++);
       if (c == pat2[0])             /* and compare it */
       {                             /* to the string to be replaced */
	 tlp = clp;
	 tbo = cbo;
         pp = &pat2[1];
         while (*pp)
         {
            if(tlp == curbp->b_linep)
              break;
            if(tbo >= llength(tlp))
            {
                 tlp = lforw(tlp);
                 tbo = 0;
                 c = '\n';
            }
            else
                 c = lgetc(tlp,tbo++);
            if(c != *pp)
                 break;
            pp++;
         }
         if(!(*pp))          /* if string is found  */
           {
            curwp->w_dotp = clp;
            curwp->w_doto = cbo - 1;
            curwp->w_flag |= WFMOVE;

	   /*************************************************
            * Read a character from standard i/o            *
	    * if charater is a blank(comparable to Zmacs)   *
            * or 'y' or 'Y' the replacement will occur.     *
            * Else if character is 'n' or 'N' it will skip  *
 	    * and go to next occurence of string.	    *
 	    * Else exit the function                        *
	    *************************************************/

	    mlwrite("Do you want to replace this? [y/n]");
            update();
	    c = (*term.t_getchar)();
            if((c ==' ')||(c == 'y')||(c == 'Y'))
               {
                 lreplace(theEnv,pat2);
                 clp = curwp->w_dotp;
                 cbo = curwp->w_doto;
	       }
	     else if ((c == 'n')||(c == 'N'))
		 cbo = tbo;
	     else
		return(TRUE);
           }
       }

     }
     curwp->w_flag |= WFMOVE;
     curwp->w_doto -= strlen(pat);
     update();
     mlwrite("No more occurrences of [%s] in buffer",pat2);
     genfree(theEnv,(void *) pat2,(unsigned) strlen(pat) + 1);
     return(TRUE);

  }

/***************************************************
 *    Replace old string with new string           *
 ***************************************************/


globle int lreplace(
  void *theEnv,
  char *pat2)
{
      int doto;
      unsigned i;
      char *cp1,*cp2;
      LINE *lp1,*lp2;
      WINDOW *wp;

     lchange(WFEDIT);
     lp1 = curwp->w_dotp;
     doto = curwp->w_doto;

     if((lp2 =lalloc(theEnv,(int) (lp1->l_used - strlen(pat2) + strlen(pat) ))) == NULL)
          return(FALSE);

     cp1 = &lp1->l_text[0];
     cp2 = &lp2->l_text[0];

   /* Copy first part of line which is containing old string to new line */

     while(cp1 != &lp1->l_text[doto])
          *cp2++ = *cp1++;

     /* reseve space for new string in new line */

     cp2 += strlen(pat);
     cp1 += strlen(pat2);

  /* Copy third part of line which is containing old string to new line */

     while (cp1 != &lp1->l_text[lp1->l_used])
          *cp2++ = *cp1++;

     *cp2 = *cp1;

     /* Rearrange pointer, insert new line  and delete old line */
     lp1->l_bp->l_fp = lp2;
     lp2->l_fp = lp1->l_fp;
     lp1->l_fp->l_bp = lp2;
     lp2->l_bp = lp1->l_bp;


     /* copy the new string in to new line */

     for(i=0;i<strlen(pat);i++)
          lp2->l_text[doto + i] = pat[i];

     /* Up-date current pointers and values */

     curwp->w_doto += strlen(pat2);

     wp = wheadp;
     while (wp != NULL)
     {  if (wp->w_linep == lp1)
             wp->w_linep = lp2;
	if(wp->w_dotp == lp1)
               wp->w_dotp = lp2;
	if (wp == curwp)
               wp->w_doto += (strlen(pat) - strlen(pat2));
	if(wp->w_markp ==lp1)
             {
               wp->w_markp = lp2;
               wp->w_marko += (strlen(pat) - strlen(pat2));
             }
        wp = wp->w_wndp;
     }
     genfree(theEnv,(void *) lp1,(unsigned) sizeof(LINE) + lp1->l_size);
     return(TRUE);
}
/********************************************************
 * This function will search for the matching bracket   *
 * of any bracket                                       *
 * It is currently bounded to C-X-^                     *
 ********************************************************/

#if WIN_BTC
#pragma argsused
#endif
globle int smatchb(
  void *theEnv,
  int f,
  int n)
  {
     int cbo,c;
     LINE *clp;

     clp = curwp->w_dotp;
     cbo = curwp->w_doto;
     c = lgetc(clp,cbo);

     if((c == '{')||(c == '(')||(c=='['))
     {
     /*  search forward for matched closing bracket '}'  */
          if(searchcl(c) == FALSE)
            return(FALSE);
     }
     else if((c == '}')||(c == ')')||(c == ']'))
     {
     /* search backward for matched opening bracket '{'*/
          if(searchop(c) == FALSE)
            return(FALSE);
     }

     else
          return(FALSE);

     curwp->w_flag |= WFMOVE;
     return(TRUE);
  }

/***************************************************
 * This function will search for closing bracket,  *
 * including '}' , ')' , ']'                       *
 ***************************************************
 * The mechanism of this function is it will check *
 * every character of the file from the current    *
 * position down and increase i by 1 every time it *
 * find an openning bracket and decrease i by 1    *
 * every time it find a closing bracket.When i = 0 *
 * the matching bracket is found.                  *
 ***************************************************/

globle int searchcl(
int tempch)
{
     LINE *lp;
     int tbo,c,i=1;

     lp = curwp->w_dotp;
     tbo = curwp->w_doto + 1;

     while(i > 0)
     {
          if(lp == curbp->b_linep)
          {
            mlwrite("Matched bracket is not found");
            return(FALSE);
          }
          if(tbo== llength(lp))
          {
            lp = lforw(lp);
            tbo = 0;
            c = '\n';
          }
          else
            c = lgetc(lp,tbo++);
          switch(tempch)
          {
            case '{':
               if (c == '{')
                 i++;

               else if (c =='}')
                 i--;
               break;

            case '(':
               if (c == '(')
                 i++;

               else if (c ==')')
                 i--;
               break;

            case '[':
               if (c == '[')
                i++;

               else if (c ==']')
                i--;
               break;
         }
     }
     curwp->w_dotp = lp;
     curwp->w_doto = tbo - 1;
     return(TRUE);
}

/**************************************************************
 * This function will search backward for the opening bracket,*
 * including '{' , '(' , '[' .                                *
 **************************************************************
 * The mechanism of this function is similiar to the searchcl,*
 * except that it will go through the file backward           *
 **************************************************************/

globle int searchop(
int tempch)
{
     LINE *lp;
     int tbo,c,i=1;

     lp = curwp->w_dotp;
     tbo = curwp->w_doto;
     while(i>0)
     {
          if(tbo == 0)
          {
            lp = lback(lp);
            if(lp == curbp->b_linep)
            {
                 mlwrite("matched bracket is not found");
                 return(FALSE);
            }
            tbo = llength(lp) + 1;
          }
          if(--tbo == llength(lp))
            c = '\n';
          else
            c = lgetc(lp,tbo);

          switch(tempch)
          {
             case '}':
               if(c == '}')
                 i++;
               else if (c == '{')
                 i--;
               break;

            case ')':
               if (c == ')')
                 i++;
               else if (c =='(')
                 i--;
               break;

            case ']':
               if (c == ']')
                i++;
               else if (c =='[')
                 i--;
               break;
          }
     }
     curwp->w_dotp = lp;
     curwp->w_doto = tbo ;
     return(TRUE);
}

/*
 * Read a pattern. Stash it in the external variable "pat". The "pat" is not
 * updated if the user types in an empty line. If the user typed an empty line,
 * and there is no old pattern, it is an error. Display the old pattern, in the
 * style of Jeff Lomicka. There is some do-it-yourself control expansion.
 */
globle int readpattern(
    void *theEnv,
    char *prompt)
    {
    register char *cp1;
    register char *cp2;
    register int c;
    register int s;
    char tpat[NPAT+20];

    cp1 = &tpat[0];                     /* Copy prompt */
    cp2 = prompt;

    while ((c = *cp2++) != '\0')
        *cp1++ = (char) c;

    if (pat[0] != '\0')                 /* Old pattern */
        {
        *cp1++ = ' ';
        *cp1++ = '[';
        cp2 = &pat[0];

        while ((c = *cp2++) != 0)
            {
            if (cp1 < &tpat[NPAT+20-6]) /* "??]: \0" */
                {
                if (c<0x20 || c==0x7F) {
                    *cp1++ = '^';
                    c ^= 0x40;
                    }
                else if (c == '%')      /* Map "%" to */
                    *cp1++ = (char) c;         /* "%%". */

                *cp1++ = (char) c;
                }
            }

        *cp1++ = ']';
        }

    *cp1++ = ':';                       /* Finish prompt */
    *cp1++ = ' ';
    *cp1++ = '\0';
    s = mlreply(theEnv,tpat, tpat, NPAT);      /* Read pattern */

    if (s == TRUE)                      /* Specified */
        genstrcpy(pat, tpat);
    else if (s == FALSE && pat[0] != 0)         /* CR, but old one */
        s = TRUE;

    return (s);
    }

/* =========================================================================
 *                             SPAWN FUNCTIONS
 * =========================================================================
 */

/*
 * The routines in this section are called to create a subjob running a
 * command interpreter.
 */

/*
 * Create a subjob with a copy of the command intrepreter in it. When the
 * command interpreter exits, mark the screen as garbage so that you do a full
 * repaint. Bound to "C-C". The message at the start in VMS puts out a newline.
 * Under some (unknown) condition, you don't get one free when DCL starts up.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int spawncli(
  void *theEnv,
  int f,
  int n)
{
#if     UNIX_7 || UNIX_V || LINUX || DARWIN || WIN_MVC || WIN_BTC || WIN_GCC
        register char *cp;
#endif
	
#if     VAX_VMS
        movecursor(term.t_nrow, 0);             /* In last line.        */
        mlputs("[Starting DCL]\r\n");
        (*term.t_flush)();                      /* Ignore "ttcol".      */
        sgarbf = TRUE;
        return (sys(NULL));                     /* NULL => DCL.         */
#endif

#if     WIN_MVC || WIN_BTC || WIN_GCC
        cp = getenv("COMSPEC");
        if (cp == NULL)
          return(TRUE);
        movecursor(term.t_nrow, 0);             /* Seek to last line.   */
        (*term.t_flush)();
        system(cp);                /* Run CLI.             */
        sgarbf = TRUE;
        return(TRUE);
#endif

#if     UNIX_7 || UNIX_V || LINUX || DARWIN
        movecursor(term.t_nrow, 0);             /* Seek to last line.   */
        (*term.t_flush)();
        ttclose();                              /* stty to old settings */
        if ((cp = getenv("SHELL")) != NULL && *cp != '\0')
                system(cp);
        else
                system("exec /bin/sh");
        sgarbf = TRUE;
        sleep(2);
        ttopen();
        return(TRUE);
#endif
}

/*
 * Run a one-liner in a subjob. When the command returns, wait for a single
 * character to be typed, then mark the screen as garbage so a full repaint is
 * done. Bound to "C-X !".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int spawn(
  void *theEnv,
  int f,
  int n)
{
        register int    s;
        char            line[NLINE];

#if     VAX_VMS
        if ((s=mlreply(theEnv,"DCL command: ", line, NLINE)) != TRUE)
                return (s);
        (*term.t_putchar)('\n');                /* Already have '\r'    */
        (*term.t_flush)();
        s = sys(line);                          /* Run the command.     */
        mlputs("\r\n\n[End]");                  /* Pause.               */
        (*term.t_flush)();
        while ((*term.t_getchar)() != '\r')
                ;
        sgarbf = TRUE;
        return (s);
#endif

#if     WIN_MVC || WIN_BTC || WIN_GCC
        if ((s=mlreply(theEnv,"MS-DOS command: ", line, NLINE)) != TRUE)
                return (s);
        system(line);
        mlwrite("Hit any key to continue");
        (*term.t_getchar)();                 /* Pause.               */
        sgarbf = TRUE;
        return (TRUE);
#endif

#if     UNIX_7 || UNIX_V || LINUX || DARWIN
        if ((s=mlreply(theEnv,"! ", line, NLINE)) != TRUE)
                return (s);
        (*term.t_putchar)('\n');                /* Already have '\r'    */
        (*term.t_flush)();
        ttclose();                              /* stty to old modes    */
        system(line);
        sleep(2);
        ttopen();
        mlputs("[End]");                        /* Pause.               */
        (*term.t_flush)();
        while ((s = (*term.t_getchar)()) != '\r' && s != ' ')
                ;
        sgarbf = TRUE;
        return (TRUE);
#endif
}

#if     VAX_VMS
/*
 * Run a command. The "cmd" is a pointer to a command string, or NULL if you
 * want to run a copy of DCL in the subjob (this is how the standard routine
 * LIB$SPAWN works. You have to do wierd stuff with the terminal on the way in
 * and the way out, because DCL does not want the channel to be in raw mode.
 */
globle int sys(
char   *cmd)
{
        struct  dsc$descriptor  cdsc;
        struct  dsc$descriptor  *cdscp;
        long    status;
        long    substatus;
        long    iosb[2];

        status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
                          oldmode, sizeof(oldmode), 0, 0, 0, 0);
        if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
                return (FALSE);
        cdscp = NULL;                           /* Assume DCL.          */
        if (cmd != NULL) {                      /* Build descriptor.    */
                cdsc.dsc$a_pointer = cmd;
                cdsc.dsc$w_length  = strlen(cmd);
                cdsc.dsc$b_dtype   = DSC$K_DTYPE_T;
                cdsc.dsc$b_class   = DSC$K_CLASS_S;
                cdscp = &cdsc;
        }
        status = LIB$SPAWN(cdscp, 0, 0, 0, 0, 0, &substatus, 0, 0, 0);
        if (status != SS$_NORMAL)
                substatus = status;
        status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
                          newmode, sizeof(newmode), 0, 0, 0, 0);
        if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
                return (FALSE);
        if ((substatus&STS$M_SUCCESS) == 0)     /* Command failed.      */
                return (FALSE);
        return (TRUE);
}
#endif

#endif
