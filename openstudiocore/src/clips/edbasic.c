   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*              CLIPS Version 6.21  06/15/03           */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

#include "setup.h"

#if EMACS_EDITOR && ! RUN_TIME

#define _EDBASIC_SOURCE_
#include "ed.h"

/* -----------------------
 *  Terminal setup stuff
 * -----------------------
 */

#if VAX_VMS
#include	<stsdef.h>
#include	<ssdef.h>
#include	<descrip.h>
#include	<iodef.h>
#include	<ttdef.h>
#include        <tt2def.h>

#define NIBUF	128	    /* Input buffer size		*/
#define NOBUF	1024	/* MM says big buffers win!	*/
#define EFN	0			/* Event flag			*/
#define ESC     '\033'
#define TERM    '~'

static char	obuf[NOBUF];	/* Output buffer		*/
static int	nobuf;		 	/* # of bytes in above	  	*/
static char	ibuf[NIBUF];	/* Input buffer		 	*/
static int	nibuf;			/* # of bytes in above		*/
static int	ibufi;			/* Read index			*/
static int	oldmode[3];		/* Old TTY mode bits		*/
static int	newmode[3];		/* New TTY mode bits		*/
static short	iochan;		 	/* TTY I/O channel		*/
#endif

#if     WIN_MVC || WIN_BTC || WIN_GCC
#include	<dos.h>
#endif

#if     UNIX_7 || UNIX_V || LINUX || DARWIN
#include <sys/ioctl.h>
#include	<sgtty.h>		/* for stty/gtty functions */
static struct  sgttyb  ostate;		        /* saved tty state */
static struct  sgttyb  nstate;		        /* values for editor mode */
#endif

/* =======================================================================
 *                     CURSOR MOVEMENT FUNCTIONS
 * =======================================================================
 */

/*
 * The routines in this section move the cursor around on the screen. They
 * compute a new value for the cursor, then adjust ".". The display code
 * always updates the cursor location, so only moves between lines, or
 * functions that adjust the top line in the window and invalidate the
 * framing, are hard.
 */

/*
 * Move the cursor to the
 * beginning of the current line.
 * Trivial.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int gotobol(
  void *theEnv,
  int f,
  int n)
  {
   curwp->w_doto  = 0;
   return (TRUE);
  }

/*
 * Move the cursor backwards by "n" characters. If "n" is less than zero call
 * "forwchar" to actually do the move. Otherwise compute the new cursor
 * location. Error if you try and move out of the buffer. Set the flag if the
 * line pointer for dot changes.
 */
globle int backchar(
  void *theEnv,
  int f,
  int n)
  {
   register LINE   *lp;

   if (n < 0)
     return (forwchar(theEnv,f, -n));
   while (n--)
     {
      if (curwp->w_doto == 0) 
        {
         if ((lp=lback(curwp->w_dotp)) == curbp->b_linep)
           { return (FALSE); }
         curwp->w_dotp  = lp;
         curwp->w_doto  = llength(lp);
         curwp->w_flag |= WFMOVE;
        }
      else
        { curwp->w_doto--; }
     }
   return (TRUE);
  }

/*
 * Move the cursor to the end of the current line. Trivial. No errors.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int gotoeol(
  void *theEnv,
  int f,
  int n)
  {
   curwp->w_doto  = llength(curwp->w_dotp);
   return (TRUE);
  }

/*
 * Move the cursor forwwards by "n" characters. If "n" is less than zero call
 * "backchar" to actually do the move. Otherwise compute the new cursor
 * location, and move ".". Error if you try and move off the end of the
 * buffer. Set the flag if the line pointer for dot changes.
 */
globle int forwchar(
  void *theEnv,
  int f,
  int n)
  {
        if (n < 0)
                return (backchar(theEnv,f, -n));
        while (n--) {
                if (curwp->w_doto == llength(curwp->w_dotp)) {
                        if (curwp->w_dotp == curbp->b_linep)
                                return (FALSE);
                        curwp->w_dotp  = lforw(curwp->w_dotp);
                        curwp->w_doto  = 0;
                        curwp->w_flag |= WFMOVE;
                } else
                        curwp->w_doto++;
        }
        return (TRUE);
   }

/*
 * Goto the beginning of the buffer. Massive adjustment of dot. This is
 * considered to be hard motion; it really isn't if the original value of dot
 * is the same as the new value of dot. Normally bound to "M-<".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int gotobob(
  void *theEnv,
  int f,
  int n)
  {
   curwp->w_dotp  = lforw(curbp->b_linep);
   curwp->w_doto  = 0;
   curwp->w_flag |= WFHARD;
   return (TRUE);
  }

/*
 * Move to the end of the buffer. Dot is always put at the end of the file
 * (ZJ). The standard screen code does most of the hard parts of update.
 * Bound to "M->".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int gotoeob(
  void *theEnv,
  int f,
  int n)
  {
   curwp->w_dotp  = curbp->b_linep;
   curwp->w_doto  = 0;
   curwp->w_flag |= WFHARD;
   return (TRUE);
  }

/*
 * Move forward by full lines. If the number of lines to move is less than
 * zero, call the backward line function to actually do it. The last command
 * controls how the goal column is set. Bound to "C-N". No errors are
 * possible.
 */
globle int forwline(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *dlp;

        if (n < 0)
                return (backline(theEnv,f, -n));
        if ((lastflag&CFCPCN) == 0)             /* Reset goal if last   */
                curgoal = curcol;               /* not C-P or C-N       */
        thisflag |= CFCPCN;
        dlp = curwp->w_dotp;
        while (n-- && dlp!=curbp->b_linep)
                dlp = lforw(dlp);
        curwp->w_dotp  = dlp;
        curwp->w_doto  = getgoal(dlp);
        curwp->w_flag |= WFMOVE;
        return (TRUE);
  }

/*
 * This function is like "forwline", but goes backwards. The scheme is exactly
 * the same. Check for arguments that are less than zero and call your
 * alternate. Figure out the new line and call "movedot" to perform the
 * motion. No errors are possible. Bound to "C-P".
 */
globle int backline(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *dlp;

        if (n < 0)
                return (forwline(theEnv,f, -n));
        if ((lastflag&CFCPCN) == 0)             /* Reset goal if the    */
                curgoal = curcol;               /* last isn't C-P, C-N  */
        thisflag |= CFCPCN;
        dlp = curwp->w_dotp;
        while (n-- && lback(dlp)!=curbp->b_linep)
                dlp = lback(dlp);
        curwp->w_dotp  = dlp;
        curwp->w_doto  = getgoal(dlp);
        curwp->w_flag |= WFMOVE;
        return (TRUE);
  }

/*
 * This routine, given a pointer to a LINE, and the current cursor goal
 * column, return the best choice for the offset. The offset is returned.
 * Used by "C-N" and "C-P".
 */
globle int getgoal(
LINE   *dlp)
{
        register int    c;
        register int    col;
        register int    newcol;
        register int    dbo;

        col = 0;
        dbo = 0;
        while (dbo != llength(dlp)) {
                c = lgetc(dlp, dbo);
                newcol = col;
                if (c == '\t')
                        newcol |= 0x07;
                else if (c<0x20 || c==0x7F)
                        ++newcol;
                ++newcol;
                if (newcol > curgoal)
                        break;
                col = newcol;
                ++dbo;
        }
        return (dbo);
}

/*
 * Scroll forward by a specified number of lines, or by a full page if no
 * argument. Bound to "C-V". The "2" in the arithmetic on the window size is
 * the overlap; this value is the default overlap value in ITS EMACS. Because
 * this zaps the top line in the display window, we have to do a hard update.
 */
globle int forwpage(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *lp;

        if (f == FALSE) {
                n = curwp->w_ntrows - 2;        /* Default scroll.      */
                if (n <= 0)                     /* Forget the overlap   */
                        n = 1;                  /* if tiny window.      */
        } else if (n < 0)
                return (backpage(theEnv,f, -n));
#if     CVMVAS
        else                                    /* Convert from pages   */
                n *= curwp->w_ntrows;           /* to lines.            */
#endif
        lp = curwp->w_linep;
        while (n-- && lp!=curbp->b_linep)
                lp = lforw(lp);
        curwp->w_linep = lp;
        curwp->w_dotp  = lp;
        curwp->w_doto  = 0;
        curwp->w_flag |= WFHARD;
        return (TRUE);
  }

/*
 * This command is like "forwpage", but it goes backwards. The "2", like
 * above, is the overlap between the two windows. The value is from the ITS
 * EMACS manual. Bound to "M-V". We do a hard update for exactly the same
 * reason.
 */
globle int backpage(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *lp;

        if (f == FALSE) {
                n = curwp->w_ntrows - 2;        /* Default scroll.      */
                if (n <= 0)                     /* Don't blow up if the */
                        n = 1;                  /* window is tiny.      */
        } else if (n < 0)
                return (forwpage(theEnv,f, -n));
#if     CVMVAS
        else                                    /* Convert from pages   */
                n *= curwp->w_ntrows;           /* to lines.            */
#endif
        lp = curwp->w_linep;
        while (n-- && lback(lp)!=curbp->b_linep)
                lp = lback(lp);
        curwp->w_linep = lp;
        curwp->w_dotp  = lp;
        curwp->w_doto  = 0;
        curwp->w_flag |= WFHARD;
        return (TRUE);
  }

/*
 * Set the mark in the current window to the value of "." in the window. No
 * errors are possible. Bound to "M-.".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int setmark(
  void *theEnv,
  int f,
  int n)
  {
   curwp->w_markp = curwp->w_dotp;
   curwp->w_marko = curwp->w_doto;
   mlwrite("[Mark set]");
   return (TRUE);
  }

/*
 * Swap the values of "." and "mark" in the current window. This is pretty
 * easy, bacause all of the hard work gets done by the standard routine
 * that moves the mark about. The only possible error is "no mark". Bound to
 * "C-X C-X".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int swapmark(
  void *theEnv,
  int f,
  int n)
  {
   register LINE   *odotp;
   register int    odoto;

   if (curwp->w_markp == NULL) 
     {
      mlwrite("No mark in this window");
      return (FALSE);
     }

   odotp = curwp->w_dotp;
   odoto = curwp->w_doto;
   curwp->w_dotp  = curwp->w_markp;
   curwp->w_doto  = curwp->w_marko;
   curwp->w_markp = odotp;
   curwp->w_marko = odoto;
   curwp->w_flag |= WFMOVE;
   return (TRUE);
  }

/* =======================================================================
 *                            WORD FUNCTIONS
 * =======================================================================
 */

/*
 * The routines in this section implement commands that work word at a time.
 * There are all sorts of word mode commands.
 */

/* Word wrap on n-spaces. Back-over whatever precedes the point on the current
 * line and stop on the first word-break or the beginning of the line. If we
 * reach the beginning of the line, jump back to the end of the word and start
 * a new line.  Otherwise, break the line at the word-break, eat it, and jump
 * back to the end of the word.
 *      NOTE:  This function may leaving trailing blanks.
 * Returns TRUE on success, FALSE on errors.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int wrapword(
  void *theEnv)
  {
        register int cnt, oldp;
        oldp = (int) curwp->w_dotp;
        cnt = -1;
        do {
                cnt++;
                if (! backchar(theEnv,0, 1))
                        return(FALSE);
        }
        while (! inword());
        if (! backword(theEnv,0, 1))
                return(FALSE);
        if (oldp == (int) (curwp->w_dotp && curwp->w_doto)) {
                if (! backdel(theEnv,0, 1))
                        return(FALSE);
                if (! newline(theEnv,0, 1))
                        return(FALSE);
        }
        return(forwword(theEnv,0, 1) && forwchar(theEnv,0, cnt));
  }

/*
 * Move the cursor backward by "n" words. All of the details of motion are
 * performed by the "backchar" and "forwchar" routines. Error if you try to
 * move beyond the buffers.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int backword(
  void *theEnv,
  int f,
  int n)
  {
        if (n < 0)
                return (forwword(theEnv,f, -n));
        if (backchar(theEnv,FALSE, 1) == FALSE)
                return (FALSE);
        while (n--) {
                while (inword() == FALSE) {
                        if (backchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                }
                while (inword() != FALSE) {
                        if (backchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                }
        }
        return (forwchar(theEnv,FALSE, 1));
  }

/*
 * Move the cursor forward by the specified number of words. All of the motion
 * is done by "forwchar". Error if you try and move beyond the buffer's end.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int forwword(
  void *theEnv,
  int f,
  int n)
  {
        if (n < 0)
                return (backword(theEnv,f, -n));
        while (n--) {
                while (inword() == FALSE) {
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                }
                while (inword() != FALSE) {
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                }
        }
        return (TRUE);
  }

/*
 * Move the cursor forward by the specified number of words. As you move,
 * convert any characters to upper case. Error if you try and move beyond the
 * end of the buffer. Bound to "M-U".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int upperword(
  void *theEnv,
  int f,
  int n)
  {
        register int    c;

        if (n < 0)
                return (FALSE);
        while (n--) {
                while (inword() == FALSE) {
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                }
                while (inword() != FALSE) {
                        c = lgetc(curwp->w_dotp, curwp->w_doto);
                        if (c>='a' && c<='z') {
                                c -= 'a'-'A';
                                lputc(curwp->w_dotp, curwp->w_doto, c);
                                lchange(WFHARD);
                        }
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                }
        }
        return (TRUE);
  }

/*
 * Move the cursor forward by the specified number of words. As you move
 * convert characters to lower case. Error if you try and move over the end of
 * the buffer. Bound to "M-L".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int lowerword(
  void *theEnv,
  int f,
  int n)
  {
        register int    c;

        if (n < 0)
                return (FALSE);
        while (n--) {
                while (inword() == FALSE) {
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                }
                while (inword() != FALSE) {
                        c = lgetc(curwp->w_dotp, curwp->w_doto);
                        if (c>='A' && c<='Z') {
                                c += 'a'-'A';
                                lputc(curwp->w_dotp, curwp->w_doto, c);
                                lchange(WFHARD);
                        }
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                }
        }
        return (TRUE);
  }

/*
 * Move the cursor forward by the specified number of words. As you move
 * convert the first character of the word to upper case, and subsequent
 * characters to lower case. Error if you try and move past the end of the
 * buffer. Bound to "M-C".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int capword(
  void *theEnv,
  int f,
  int n)
  {
        register int    c;

        if (n < 0)
                return (FALSE);
        while (n--) {
                while (inword() == FALSE) {
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                }
                if (inword() != FALSE) {
                        c = lgetc(curwp->w_dotp, curwp->w_doto);
                        if (c>='a' && c<='z') {
                                c -= 'a'-'A';
                                lputc(curwp->w_dotp, curwp->w_doto, c);
                                lchange(WFHARD);
                        }
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                        while (inword() != FALSE) {
                                c = lgetc(curwp->w_dotp, curwp->w_doto);
                                if (c>='A' && c<='Z') {
                                        c += 'a'-'A';
                                        lputc(curwp->w_dotp, curwp->w_doto, c);
                                        lchange(WFHARD);
                                }
                                if (forwchar(theEnv,FALSE, 1) == FALSE)
                                        return (FALSE);
                        }
                }
        }
        return (TRUE);
  }

/*
 * Kill forward by "n" words. Remember the location of dot. Move forward by
 * the right number of words. Put dot back where it was and issue the kill
 * command for the right number of characters. Bound to "M-D".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int delfword(
  void *theEnv,
  int f,
  int n)
  {
        register int    size;
        register LINE   *dotp;
        register int    doto;

        if (n < 0)
                return (FALSE);

        if ((lastflag&CFKILL) == 0)             /* Clear kill buffer if */
                kdelete(theEnv);                      /* last wasn't a kill.  */
        thisflag |= CFKILL;

        dotp = curwp->w_dotp;
        doto = curwp->w_doto;
        size = 0;
        while (n--) {
                while (inword() == FALSE) {
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                        ++size;
                }
                while (inword() != FALSE) {
                        if (forwchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                        ++size;
                }
        }
        curwp->w_dotp = dotp;
        curwp->w_doto = doto;
        return (ldelete(theEnv,(long) size, TRUE));
  }

/*
 * Kill backwards by "n" words. Move backwards by the desired number of words,
 * counting the characters. When dot is finally moved to its resting place,
 * fire off the kill command. Bound to "M-Rubout" and to "M-Backspace".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int delbword(
  void *theEnv,
  int f,
  int n)
  {
        register int    size;

        if (n < 0)
                return (FALSE);
        if (backchar(theEnv,FALSE, 1) == FALSE)
                return (FALSE);

        if ((lastflag&CFKILL) == 0)             /* Clear kill buffer if */
                kdelete(theEnv);                      /* last wasn't a kill.  */
        thisflag |= CFKILL;

        size = 0;
        while (n--) {
                while (inword() == FALSE) {
                        if (backchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                        ++size;
                }
                while (inword() != FALSE) {
                        if (backchar(theEnv,FALSE, 1) == FALSE)
                                return (FALSE);
                        ++size;
                }
        }
        if (forwchar(theEnv,FALSE, 1) == FALSE)
                return (FALSE);
        return (ldelete(theEnv,(long) size, TRUE));
  }

/*
 * Return TRUE if the character at dot is a character that is considered
 * part of a word. The word character list is hard coded. Should be setable.
 */
globle int inword()
{
        register int    c;

        if (curwp->w_doto == llength(curwp->w_dotp))
                return (FALSE);
        c = lgetc(curwp->w_dotp, curwp->w_doto);
        if (c>='a' && c<='z')
                return (TRUE);
        if (c>='A' && c<='Z')
                return (TRUE);
        if (c>='0' && c<='9')
                return (TRUE);
        if (c=='$')                            /* For identifiers      */
                return (TRUE);
        return (FALSE);
}


/* =======================================================================
 *                         REGION FUNCTIONS
 * =======================================================================
 */

/*
 * The routines in this section deal with the region, that magic space
 * between "." and mark. Some functions are commands. Some functions are
 * just for internal use.
 */

/*
 * Kill the region. Ask "getregion"
 * to figure out the bounds of the region.
 * Move "." to the start, and kill the characters.
 * Bound to "C-W".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int killregion(
  void *theEnv,
  int f,
  int n)
  {
   register int    s;
   REGION          region;

   if ((s=getregion(&region)) != TRUE)
     { return (s); }
   if ((lastflag&CFKILL) == 0)             /* This is a kill type  */
     { kdelete(theEnv); }                    /* command, so do magic */
   thisflag |= CFKILL;                     /* kill buffer stuff.   */
   curwp->w_dotp = region.r_linep;
   curwp->w_doto = region.r_offset;
   return (ldelete(theEnv,region.r_size, TRUE));
  }

/*
 * Copy all of the characters in the
 * region to the kill buffer. Don't move dot
 * at all. This is a bit like a kill region followed
 * by a yank. Bound to "M-W".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int copyregion(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *linep;
        register int    loffs;
        register int    s;
        REGION          region;

        if ((s=getregion(&region)) != TRUE)
                return (s);
        if ((lastflag&CFKILL) == 0)             /* Kill type command.   */
                kdelete(theEnv);
        thisflag |= CFKILL;
        linep = region.r_linep;                 /* Current line.        */
        loffs = region.r_offset;                /* Current offset.      */
        while (region.r_size--) {
                if (loffs == llength(linep)) {  /* End of line.         */
                        if ((s=kinsert(theEnv,'\n')) != TRUE)
                                return (s);
                        linep = lforw(linep);
                        loffs = 0;
                } else {                        /* Middle of line.      */
                        if ((s=kinsert(theEnv,lgetc(linep, loffs))) != TRUE)
                                return (s);
                        ++loffs;
                }
        }
        mlwrite("Region copied to buffer");
        return (TRUE);
  }

/*
 * Lower case region. Zap all of the upper
 * case characters in the region to lower case. Use
 * the region code to set the limits. Scan the buffer,
 * doing the changes. Call "lchange" to ensure that
 * redisplay is done in all buffers. Bound to
 * "C-X C-L".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int lowerregion(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *linep;
        register int    loffs;
        register int    c;
        register int    s;
        REGION          region;

        if ((s=getregion(&region)) != TRUE)
                return (s);
        lchange(WFHARD);
        linep = region.r_linep;
        loffs = region.r_offset;
        while (region.r_size--) {
                if (loffs == llength(linep)) {
                        linep = lforw(linep);
                        loffs = 0;
                } else {
                        c = lgetc(linep, loffs);
                        if (c>='A' && c<='Z')
                                lputc(linep, loffs, c+'a'-'A');
                        ++loffs;
                }
        }
        return (TRUE);
  }

/*
 * Upper case region. Zap all of the lower
 * case characters in the region to upper case. Use
 * the region code to set the limits. Scan the buffer,
 * doing the changes. Call "lchange" to ensure that
 * redisplay is done in all buffers. Bound to
 * "C-X C-L".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int upperregion(
  void *theEnv,
  int f,
  int n)
  {
        register LINE   *linep;
        register int    loffs;
        register int    c;
        register int    s;
        REGION          region;

        if ((s=getregion(&region)) != TRUE)
                return (s);
        lchange(WFHARD);
        linep = region.r_linep;
        loffs = region.r_offset;
        while (region.r_size--) {
                if (loffs == llength(linep)) {
                        linep = lforw(linep);
                        loffs = 0;
                } else {
                        c = lgetc(linep, loffs);
                        if (c>='a' && c<='z')
                                lputc(linep, loffs, c-'a'+'A');
                        ++loffs;
                }
        }
        return (TRUE);
  }

/*
 * This routine figures out the
 * bounds of the region in the current window, and
 * fills in the fields of the "REGION" structure pointed
 * to by "rp". Because the dot and mark are usually very
 * close together, we scan outward from dot looking for
 * mark. This should save time. Return a standard code.
 * Callers of this routine should be prepared to get
 * an "ABORT" status; we might make this have the
 * conform thing later.
 */
globle int getregion(
REGION *rp)
{
        register LINE   *flp;
        register LINE   *blp;
        long    fsize;
        long    bsize;

        if (curwp->w_markp == NULL) {
                mlwrite("No mark set in this window");
                return (FALSE);
        }
        if (curwp->w_dotp == curwp->w_markp) {
                rp->r_linep = curwp->w_dotp;
                if (curwp->w_doto < curwp->w_marko) {
                        rp->r_offset = curwp->w_doto;
                        rp->r_size = (long) (curwp->w_marko-curwp->w_doto);
                } else {
                        rp->r_offset = curwp->w_marko;
                        rp->r_size = (long) (curwp->w_doto-curwp->w_marko);
                }
                return (TRUE);
        }
        blp = curwp->w_dotp;
        bsize = (long) curwp->w_doto;
        flp = curwp->w_dotp;
        fsize = (long) (llength(flp)-curwp->w_doto+1);
        while (flp!=curbp->b_linep || lback(blp)!=curbp->b_linep) {
                if (flp != curbp->b_linep) {
                        flp = lforw(flp);
                        if (flp == curwp->w_markp) {
                                rp->r_linep = curwp->w_dotp;
                                rp->r_offset = curwp->w_doto;
                                rp->r_size = fsize+curwp->w_marko;
                                return (TRUE);
                        }
                        fsize += llength(flp)+1;
                }
                if (lback(blp) != curbp->b_linep) {
                        blp = lback(blp);
                        bsize += llength(blp)+1;
                        if (blp == curwp->w_markp) {
                                rp->r_linep = blp;
                                rp->r_offset = curwp->w_marko;
                                rp->r_size = bsize - curwp->w_marko;
                                return (TRUE);
                        }
                }
        }
        mlwrite("Bug: lost mark");
        return (FALSE);
}

/* =======================================================================
 *                   HIGH LEVEL FILE I/O COMMANDS
 * =======================================================================
 */

/*
 * Read a file into the current
 * buffer. This is really easy; all you do it
 * find the name of the file, and call the standard
 * "read a file into the current buffer" code.
 * Bound to "C-X C-R".
 *
 * Changed calling code to "C-X C-V" to be
 * more like the Zmacs editor on Symbolics. CJC 7/28/86
 */
#if WIN_BTC
#pragma argsused
#endif
globle int fileread(
  void *theEnv,
  int f,
  int n)
  {
   register int    s;
   char            fname[NFILEN];

   if ((s=mlreply(theEnv,"Visit file: ", fname, NFILEN)) != TRUE)
     { return (s); }
   return (readin(theEnv,fname));
  }

/*
 * Select a file for editing.
 * Look around to see if you can find the
 * fine in another buffer; if you can find it
 * just switch to the buffer. If you cannot find
 * the file, create a new buffer, read in the
 * text, and switch to the new buffer.
 * Bound to C-X C-V.
 *
 * Changed calling code to "C-X C-F" (Find File) to be
 * more like the Zmacs editor on Symbolics. CJC 7/28/86
 */
#if WIN_BTC
#pragma argsused
#endif
globle int filevisit(
  void *theEnv,
  int f,
  int n)
  {
        register int    s;
        char            fname[NFILEN];

        if ((s=mlreply(theEnv,"Find file: ", fname, NFILEN)) != TRUE)
                return (s);
	filevisit_guts(theEnv,fname);
	return(TRUE);
  }

globle int filevisit_guts(
  void *theEnv,
  char fname[])
{
        register BUFFER *bp;
        register WINDOW *wp;
        register LINE   *lp;
        register int    i;
        register int    s;
        char            bname[NBUFN];

        for (bp=bheadp; bp!=NULL; bp=bp->b_bufp) {
                if ((bp->b_flag&BFTEMP)==0 && strcmp(bp->b_fname, fname)==0) {
                        if (--curbp->b_nwnd == 0) {
                                curbp->b_dotp  = curwp->w_dotp;
                                curbp->b_doto  = curwp->w_doto;
                                curbp->b_markp = curwp->w_markp;
                                curbp->b_marko = curwp->w_marko;
                        }
                        curbp = bp;
                        curwp->w_bufp  = bp;
                        if (bp->b_nwnd++ == 0) {
                                curwp->w_dotp  = bp->b_dotp;
                                curwp->w_doto  = bp->b_doto;
                                curwp->w_markp = bp->b_markp;
                                curwp->w_marko = bp->b_marko;
                        } else {
                                wp = wheadp;
                                while (wp != NULL) {
                                        if (wp!=curwp && wp->w_bufp==bp) {
                                                curwp->w_dotp  = wp->w_dotp;
                                                curwp->w_doto  = wp->w_doto;
                                                curwp->w_markp = wp->w_markp;
                                                curwp->w_marko = wp->w_marko;
                                                break;
                                        }
                                        wp = wp->w_wndp;
                                }
                        }
                        lp = curwp->w_dotp;
                        i = curwp->w_ntrows/2;
                        while (i-- && lback(lp)!=curbp->b_linep)
                                lp = lback(lp);
                        curwp->w_linep = lp;
                        curwp->w_flag |= WFMODE|WFHARD;
                        mlwrite("[Old buffer]");
                        return (TRUE);
                }
        }
        makename(bname, fname);                 /* New buffer name.     */
        while ((bp=bfind(theEnv,bname, FALSE, 0)) != NULL) {
                s = mlreply(theEnv,"Buffer name: ", bname, NBUFN);
                if (s == ABORT)                 /* ^G to just quit      */
                        return (s);
                if (s == FALSE) {               /* CR to clobber it     */
                        makename(bname, fname);
                        break;
                }
        }
        if (bp==NULL && (bp=bfind(theEnv,bname, TRUE, 0))==NULL) {
                mlwrite("Cannot create buffer");
                return (FALSE);
        }
        if (--curbp->b_nwnd == 0) {             /* Undisplay.           */
                curbp->b_dotp = curwp->w_dotp;
                curbp->b_doto = curwp->w_doto;
                curbp->b_markp = curwp->w_markp;
                curbp->b_marko = curwp->w_marko;
        }
        genstrcpy(lastbufn, curbp->b_bname);       /* Set last buufer name */

        curbp = bp;                             /* Switch to it.        */
        curwp->w_bufp = bp;
        curbp->b_nwnd++;
        return (readin(theEnv,fname));                 /* Read it in.          */
}

/*
 * Read file "fname" into the current
 * buffer, blowing away any text found there. Called
 * by both the read and visit commands. Return the final
 * status of the read. Also called by the mainline,
 * to read in a file specified on the command line as
 * an argument.
 */
globle int readin(
  void *theEnv,
  char    fname[])
{
        register LINE   *lp1;
        register LINE   *lp2;
        register int    i;
        register WINDOW *wp;
        register BUFFER *bp;
        register int    s;
        register int    nbytes;
        register int    nline;
        char            line[NLINE];

        bp = curbp;                             /* Cheap.               */
        if ((s=bclear(theEnv,bp)) != TRUE)             /* Might be old.        */
                return (s);
        bp->b_flag &= ~(BFTEMP|BFCHG);
        genstrcpy(bp->b_fname, fname);
        if ((s=ffropen(fname)) == FIOERR)       /* Hard file open.      */
                goto out;
        if (s == FIOFNF) {                      /* File not found.      */
                mlwrite("[New file]");
                goto out;
        }
        mlwrite("[Reading file]");
        nline = 0;
        while ((s=ffgetline(line, NLINE)) == FIOSUC) {
                nbytes = strlen(line);
                if ((lp1=lalloc(theEnv,nbytes)) == NULL) {
                        s = FIOERR;             /* Keep message on the  */
                        break;                  /* display.             */
                }
                lp2 = lback(curbp->b_linep);
                lp2->l_fp = lp1;
                lp1->l_fp = curbp->b_linep;
                lp1->l_bp = lp2;
                curbp->b_linep->l_bp = lp1;
                for (i=0; i<nbytes; ++i)
                        lputc(lp1, i, line[i]);
                ++nline;
        }
        ffclose();                              /* Ignore errors.       */
        if (s == FIOEOF) {                      /* Don't zap message!   */
                if (nline == 1)
                        mlwrite("[Read 1 line]");
                else
                        mlwrite("[Read %d lines]", nline);
        }
out:
        for (wp=wheadp; wp!=NULL; wp=wp->w_wndp) {
                if (wp->w_bufp == curbp) {
                        wp->w_linep = lforw(curbp->b_linep);
                        wp->w_dotp  = lforw(curbp->b_linep);
                        wp->w_doto  = 0;
                        wp->w_markp = NULL;
                        wp->w_marko = 0;
                        wp->w_flag |= WFMODE|WFHARD;
                }
        }
        if (s == FIOERR)                        /* False if error.      */
                return (FALSE);
        return (TRUE);
}

/*
 * Take a file name, and from it
 * fabricate a buffer name. This routine knows
 * about the syntax of file names on the target system.
 * I suppose that this information could be put in
 * a better place than a line of code.
 */
globle int makename(
char    bname[],
char    fname[])
{
        register char   *cp1;
        register char   *cp2;

        cp1 = &fname[0];
        while (*cp1 != 0)
                ++cp1;

#if     VAX_VMS
        while (cp1!=&fname[0] && cp1[-1]!=':' && cp1[-1]!=']')
                --cp1;
#endif

#if     WIN_MVC || WIN_BTC || WIN_GCC
        while (cp1!=&fname[0] && cp1[-1]!=':' && cp1[-1]!='\\')
                --cp1;
#endif
#if     UNIX_7 || UNIX_V || LINUX || DARWIN
        while (cp1!=&fname[0] && cp1[-1]!='/')
                --cp1;
#endif
        cp2 = &bname[0];
        while (cp2!=&bname[NBUFN-1] && *cp1!=0 && *cp1!=';')
                *cp2++ = *cp1++;
        *cp2 = 0;
	return(TRUE);
}

/*
 * Ask for a file name, and write the
 * contents of the current buffer to that file.
 * Update the remembered file name and clear the
 * buffer changed flag. This handling of file names
 * is different from the earlier versions, and
 * is more compatable with Gosling EMACS than
 * with ITS EMACS. Bound to "C-X C-W".
 *
 * Modified to allow current file name as default. CJC 7/28/86
 */
#if WIN_BTC
#pragma argsused
#endif
globle int filewrite(
  void *theEnv,
  int f,
  int n)
  {
        register WINDOW *wp;
        register int    s;
        char            fname[NFILEN];
        char            prompt[NFILEN + 15];

        gensprintf(prompt,"Write file [%s]: ",curbp->b_fname);
        if ((s=mlreply(theEnv,prompt, fname, NFILEN)) != TRUE) {
                if (s == FALSE)
                   genstrcpy(fname, curbp->b_fname);
                else
                   return (s);
                }
        if ((s=writeout(fname)) == TRUE) {
                genstrcpy(curbp->b_fname, fname);
                curbp->b_flag &= ~BFCHG;
                wp = wheadp;                    /* Update mode lines.   */
                while (wp != NULL) {
                        if (wp->w_bufp == curbp)
                                wp->w_flag |= WFMODE;
                        wp = wp->w_wndp;
                }
        }
        return (s);
  }

/*
 * Save the contents of the current
 * buffer in its associatd file. Do nothing
 * if nothing has changed (this may be a bug, not a
 * feature). Error if there is no remembered file
 * name for the buffer. Bound to "C-X C-S". May
 * get called by "C-Z".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int filesave(
  void *theEnv,
  int f,
  int n)
  {
        register WINDOW *wp;
        register int    s;

        if ((curbp->b_flag&BFCHG) == 0)         /* Return, no changes.  */
                return (TRUE);
        if (curbp->b_fname[0] == 0) {           /* Must have a name.    */
                mlwrite("No file name");
                return (FALSE);
        }
        if ((s=writeout(curbp->b_fname)) == TRUE) {
                curbp->b_flag &= ~BFCHG;
                wp = wheadp;                    /* Update mode lines.   */
                while (wp != NULL) {
                        if (wp->w_bufp == curbp)
                                wp->w_flag |= WFMODE;
                        wp = wp->w_wndp;
                }
        }
        return (s);
  }

/*
 * This function performs the details of file
 * writing. Uses the file management routines in the
 * "fileio.c" package. The number of lines written is
 * displayed. Sadly, it looks inside a LINE; provide
 * a macro for this. Most of the grief is error
 * checking of some sort.
 */
globle int writeout(
char    *fn)
{
        register int    s;
        register LINE   *lp;
        register int    nline;

        if ((s=ffwopen(fn)) != FIOSUC)          /* Open writes message. */
                return (FALSE);
        lp = lforw(curbp->b_linep);             /* First line.          */
        nline = 0;                              /* Number of lines.     */
        while (lp != curbp->b_linep) {
                if ((s=ffputline(&lp->l_text[0], llength(lp))) != FIOSUC)
                        break;
                ++nline;
                lp = lforw(lp);
        }
        if (s == FIOSUC) {                      /* No write error.      */
                s = ffclose();
                if (s == FIOSUC) {              /* No close error.      */
                        if (nline == 1)
                                mlwrite("[Wrote 1 line]");
                        else
                                mlwrite("[Wrote %d lines]", nline);
                }
        } else                                  /* Ignore close error   */
                ffclose();                      /* if a write error.    */
        if (s != FIOSUC)                        /* Some sort of error.  */
                return (FALSE);
        return (TRUE);
}

/*
 * The command allows the user
 * to modify the file name associated with
 * the current buffer. It is like the "f" command
 * in UNIX "ed". The operation is simple; just zap
 * the name in the BUFFER structure, and mark the windows
 * as needing an update. You can type a blank line at the
 * prompt if you wish.
 *
 * Bound to "C-X C-R" for Rename buffer!
 * Changed 7/28/86 by CJC.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int filename(
  void *theEnv,
  int f,
  int n)
  {
        register WINDOW *wp;
        register int    s;
        char            fname[NFILEN];

        if ((s=mlreply(theEnv,"Name: ", fname, NFILEN)) == ABORT)
                return (s);
        if (s == FALSE)
                genstrcpy(curbp->b_fname, "");
        else
                genstrcpy(curbp->b_fname, fname);
        wp = wheadp;                            /* Update mode lines.   */
        while (wp != NULL) {
                if (wp->w_bufp == curbp)
                        wp->w_flag |= WFMODE;
                wp = wp->w_wndp;
        }
        return (TRUE);
  }

/* =======================================================================
 *                   Low level File I/O commands
 * =======================================================================
 */

static FILE    *ffp;                 /* File pointer, all functions. */

/*
 * The routines in this section do the low level read and write of ASCII
 * files from the disk. All of the knowledge about files is here. A better
 * message writing scheme should be used.
 */

/*
 * Open a file for reading.
 */
globle int ffropen(
char    *fn)
{
        if ((ffp=fopen(fn, "r")) == NULL)
                return (FIOFNF);
        return (FIOSUC);
}

/*
 * Open a file for writing. Return TRUE if all is well, and FALSE on error
 * (cannot create).
 */
globle int ffwopen(
char    *fn)
{
#if     VAX_VMS
        register int    fd;

        if ((fd=creat(fn, 0666, "rfm=var", "rat=cr")) < 0
        || (ffp=fdopen(fd, "w")) == NULL) {
#else
        if ((ffp=fopen(fn, "w")) == NULL) {
#endif
                mlwrite("Cannot open file for writing");
                return (FIOERR);
        }
        return (FIOSUC);
}

/*
 * Close a file. Should look at the status in all systems.
 */
globle int ffclose()
{
#if     UNIX_7 || UNIX_V || LINUX || DARWIN
        if (fclose(ffp) != FALSE) {
                mlwrite("Error closing file");
                return(FIOERR);
        }
#else
        fclose(ffp);
#endif
        return (FIOSUC);
}

/*
 * Write a line to the already opened file. The "buf" points to the buffer,
 * and the "nbuf" is its length, less the free newline. Return the status.
 * Check only at the newline.
 */
globle int ffputline(
char    buf[],
int nbuf)
{
        register int    i;

        for (i = 0; i < nbuf; ++i)
                fputc(buf[i]&0xFF, ffp);

        fputc('\n', ffp);

        if (ferror(ffp)) {
                mlwrite("Write I/O error");
                return (FIOERR);
        }

        return (FIOSUC);
}

/*
 * Read a line from a file, and store the bytes in the supplied buffer. The
 * "nbuf" is the length of the buffer. Complain about long lines and lines
 * at the end of the file that don't have a newline present. Check for I/O
 * errors too. Return status.
 */
globle int ffgetline(
char   buf[],
int nbuf)
{
        register int    c;
        register int    i;

        i = 0;

        while ((c = fgetc(ffp)) != EOF && c != '\n') {
                if (i >= nbuf-1) {
                        mlwrite("File has long line");
                        return (FIOERR);
                }
                buf[i++] = (char) c;
        }

        if (c == EOF) {
                if (ferror(ffp)) {
                        mlwrite("File read error");
                        return (FIOERR);
                }

                if (i != 0) {
                        mlwrite("File has funny line at EOF");
                        return (FIOERR);
                }
                return (FIOEOF);
        }

        buf[i] = 0;
        return (FIOSUC);
}


/* =======================================================================
 *                    LOW LEVEL TERMINAL I/O COMMANDS
 * =======================================================================
 */

/*
 * The functions in this section negotiate with the operating system for
 * characters, and write characters in a barely buffered fashion on the
 * display. All operating systems.
 */

/*
 * This function is called once to set up the terminal device streams.
 * On VMS, it translates SYS$INPUT until it finds the terminal, then assigns
 * a channel to it and sets it raw.
 */
globle void ttopen()
{
#if	VAX_VMS
	struct  dsc$descriptor  idsc;
	struct  dsc$descriptor  odsc;
	char	oname[40];
	int	iosb[2];
	int	status;

	odsc.dsc$a_pointer = "SYS$INPUT";
	odsc.dsc$w_length  = strlen(odsc.dsc$a_pointer);
	odsc.dsc$b_dtype	= DSC$K_DTYPE_T;
	odsc.dsc$b_class	= DSC$K_CLASS_S;
	idsc.dsc$b_dtype	= DSC$K_DTYPE_T;
	idsc.dsc$b_class	= DSC$K_CLASS_S;
	do {
		idsc.dsc$a_pointer = odsc.dsc$a_pointer;
		idsc.dsc$w_length  = odsc.dsc$w_length;
		odsc.dsc$a_pointer = &oname[0];
		odsc.dsc$w_length  = sizeof(oname);
		status = LIB$SYS_TRNLOG(&idsc, &odsc.dsc$w_length, &odsc);
		if (status!=SS$_NORMAL && status!=SS$_NOTRAN)
			exit(status);
		if (oname[0] == 0x1B) {
			odsc.dsc$a_pointer += 4;
			odsc.dsc$w_length  -= 4;
		}
	} while (status == SS$_NORMAL);
	status = SYS$ASSIGN(&odsc, &iochan, 0, 0);
	if (status != SS$_NORMAL)
		exit(status);
	status = SYS$QIOW(EFN, iochan, IO$_SENSEMODE, iosb, 0, 0,
			  oldmode, sizeof(oldmode), 0, 0, 0, 0);
	if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
		exit(status);
	newmode[0] = oldmode[0];
	newmode[1] = oldmode[1] | TT$M_PASSALL | TT$M_NOECHO;
        newmode[1] &= ~(TT$M_TTSYNC|TT$M_HOSTSYNC);
        newmode[2] = oldmode[2] | TT2$M_PASTHRU;
	status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
			  newmode, sizeof(newmode), 0, 0, 0, 0);
	if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
		exit(status);
#endif

#if	UNIX_7 || UNIX_V || LINUX || DARWIN
        ioctl(1,TIOCGETP,&ostate);
        ioctl(1,TIOCGETP,&nstate);
	/* gtty(1, &ostate);	*/		/* save old state */
	/* gtty(1, &nstate);	*/ 		/* get base of new state */
	nstate.sg_flags |= RAW;
	nstate.sg_flags &= ~(ECHO|CRMOD);	/* no echo for now... */
	/* stty(1, &nstate);	*/		/* set mode */
        ioctl(1,TIOCSETP,&nstate);
#endif
}

/*
 * This function gets called just before we go back home to the command
 * interpreter. On VMS it puts the terminal back in a reasonable state.
 */
globle void ttclose()
{
#if	VAX_VMS
	int	status;
	int	iosb[1];

	ttflush();
	status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
		 oldmode, sizeof(oldmode), 0, 0, 0, 0);
	if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
		exit(status);
	status = SYS$DASSGN(iochan);
	if (status != SS$_NORMAL)
		exit(status);
#endif

#if	UNIX_7 || UNIX_V || LINUX || DARWIN
	/* stty(1, &ostate); */
        ioctl(1,TIOCSETP,&ostate);
#endif
}

/*
 * Write a character to the display. On VMS, terminal output is buffered, and
 * we just put the characters in the big array, after checking for overflow.
 * Ditto on MS-DOS (use the very very raw console output routine).
 */
#if WIN_BTC
#pragma argsused
#endif
globle void ttputc(
int c)
{
#if	VAX_VMS
	if (nobuf >= NOBUF)
		ttflush();
	obuf[nobuf++] = c;
#endif

#if	UNIX_7 || UNIX_V || LINUX || DARWIN
	fputc(c, stdout);
#endif
}

/*
 * Flush terminal buffer. Does real work where the terminal output is buffered
 * up. A no-operation on systems where byte at a time terminal I/O is done.
 */
globle void ttflush()
{
#if	VAX_VMS
	int	status;
	int	iosb[2];

	status = SS$_NORMAL;
	if (nobuf != 0) {
		status = SYS$QIOW(EFN, iochan, IO$_WRITELBLK|IO$M_NOFORMAT,
			 iosb, 0, 0, obuf, nobuf, 0, 0, 0, 0);
		if (status == SS$_NORMAL)
			status = iosb[0] & 0xFFFF;
		nobuf = 0;
	}

#endif

#if	UNIX_7 || UNIX_V || LINUX || DARWIN
	fflush(stdout);
#endif
}


/*
 * Read a character from the terminal, performing no editing and doing no echo
 * at all. More complex in VMS that almost anyplace else, which figures.
 */
globle int ttgetc()
{
#if	VAX_VMS
	int	status;
	int	iosb[2];
    	long term[2];       /*Terminator block for I/O*/

	while (ibufi >= nibuf) {
		ibufi = 0;
		term[0] = 0;
		term[1] = 0;
		status = SYS$QIOW(EFN, iochan,
			  IO$_READLBLK|IO$M_TIMED|IO$M_NOFILTR|IO$M_PURGE,
			 iosb, 0, 0, ibuf, NIBUF, 0, term, 0, 0);
		if (status != SS$_NORMAL)
			exit(status);
		status = iosb[0] & 0xFFFF;
		if (status!=SS$_NORMAL && status!=SS$_TIMEOUT)
			exit(status);
		nibuf = (iosb[0]>>16) + (iosb[1]>>16);
		if (nibuf == 0) {
         	   status = SYS$QIOW(EFN, iochan,IO$_READLBLK|
                                     IO$M_NOFILTR|IO$M_PURGE,
			             iosb, 0, 0, ibuf, 1, 0, term, 0, 0);
		   if (status != SS$_NORMAL)
                     exit(status);
		   status = (iosb[0]&0xFFFF);
		   if (status != SS$_NORMAL)
		     exit(status);
		   nibuf = (iosb[0]>>16) + (iosb[1]>>16);
		}
	}
        if (ibuf[ibufi] == ESC)
          {
           status = SYS$QIOW(EFN,iochan,IO$_READLBLK|IO$M_NOFILTR|IO$M_TIMED,
                             iosb,0,0,ibuf+1,4,0,term,0,0);
           nibuf = (iosb[0]>>16) + (iosb[1]>>16) + 1;
           if ((status == SS$_NORMAL) && (nibuf > 1))
             return(parse_esc_seq());
           if ((status != SS$_NORMAL) && (status != SS$_TIMEOUT))
             exit(status);
          }
        return (ibuf[ibufi++] & 0xFF);	  /* Allow multinational  */
#endif

#if     WIN_MVC || WIN_BTC || WIN_GCC
	return (fgetc(stdin));           /* NOTE: This won't really work */
#endif                              /* See file EDTERM.C for good code */

#if	UNIX_7 || UNIX_V || LINUX || DARWIN
	return(fgetc(stdin));
#endif
}

#if VAX_VMS
/********************************/
/*PARSES ESCAPE SEQUENCES       */
/********************************/
globle int parse_esc_seq() {

	int index, num;

	index = ibufi + 1;
	if ((ibuf[index] != '[') && (ibuf[index] != 'O')) {
	    if ((ibuf[index] >= 'a') && (ibuf[index] <= 'z'))
		ibuf[index] = ibuf[index] - ('a' - 'A');
	    if ((ibuf[index] >= '\000') && (ibuf[index] <= '\037'))
		ibuf[index] = COTL | ibuf[index];
	    ibufi = nibuf + 1;
  	    return (META | ibuf[index]);
	}
	if (ibuf[index] == '[') index++;
	ibufi = nibuf + 1;
	switch (ibuf[index]) {
	case 'A' : return(COTL | 'P');
		   break;
	case 'B' : return(COTL | 'N');
		   break;
	case 'C' : return(COTL | 'F');
		   break;
	case 'D' : return(COTL | 'B');
		   break;
	case '1' : case '2' : case '3' :
	case '4' : case '5' : case '6' :
		   if (ibuf[index + 1] != TERM)
		       num = (ibuf[index] - 48)*10 + (ibuf[++index] -48);
		   else num = ibuf[index] - 48;
		   switch (num) {
                   case 1 : return(COTL | 'S');
			     break;
                   case 2 : return(COTL | 'Y');
			     break;
                   case 3 : return(COTL | 'W');
			     break;
                   case 4 : return(COTL | '@');
			     break;
                   case 5 : return(META | 'V');
			     break;
                   case 6 : return(COTL | 'V');
			     break;
		   default : return BADKEY;
		   }
		   break;
	default  : return BADKEY;
	}	
     return(TRUE);
    }
#endif

#endif



