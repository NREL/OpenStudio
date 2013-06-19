   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*              CLIPS Version 6.20  01/31/02           */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

#include "setup.h"

#if EMACS_EDITOR && ! RUN_TIME

#define _EDSTRUCT_SOURCE_
#include "ed.h"

#include <stdarg.h>

/* ----------------------------
 *   Display management stuff
 * ----------------------------
 */

typedef struct  VIDEO {
        short   v_flag;                 /* Flags */
        char    v_text[1];              /* Screen data. */
}       VIDEO;

#define VFCHG   0x0001                  /* Changed. */

globle int     mpresf  = FALSE;         /* TRUE if message in last line */
globle int     sgarbf  = TRUE;          /* TRUE if screen is garbage */

static int     vtrow   = 0;                    /* Row location of SW cursor */
static int     vtcol   = 0;                    /* Column location of SW cursor */
static int     ttrow   = HUGE;                 /* Row location of HW cursor */
static int     ttcol   = HUGE;                 /* Column location of HW cursor */

static VIDEO  **vscreen;                      /* Virtual screen. */
static VIDEO  **pscreen;                      /* Physical screen. */

/* ----------------------------
 *   Line management stuff
 * ----------------------------
 */

#define NBLOCK  16                      /* Line block chunk size        */
#define KBLOCK  256                     /* Kill buffer block size       */

static char    *kbufp  = NULL;                 /* Kill buffer data             */
static int     kused   = 0;                    /* # of bytes used in KB        */
static int     ksize   = 0;                    /* # of bytes allocated in KB   */

static void int_to_ascii(char [],int,int);

/* ==========================================================================
 *                      BUFFER MANAGEMENT FUNCTIONS
 * ==========================================================================
 */

/*
 * Some of the functions in this section are internal, and some are
 * actually attached to user keys. Like everyone else, they set hints
 * for the display system.
 */

/*
 * Attach a buffer to a window. The
 * values of dot and mark come from the buffer
 * if the use count is 0. Otherwise, they come
 * from some other window.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int usebuffer(
  void *theEnv,
  int f,
  int n)
{
        register BUFFER *bp;
        register WINDOW *wp;
        register int    s;
        char            bufn[NBUFN];
        char            prompt[NBUFN + 15];

        gensprintf(prompt,"Use buffer [%s]: ",lastbufn);
        if ((s=mlreply(theEnv,prompt, bufn, NBUFN)) != TRUE) {
                if (( s == FALSE) && (strlen(lastbufn) != 0))
                   genstrcpy(bufn, lastbufn);
                else
                   return (s);
                }
        if ((bp=bfind(theEnv,bufn, TRUE, 0)) == NULL)
                return (FALSE);
        genstrcpy(lastbufn, curbp->b_bname);       /* Save current bufname */
        if (--curbp->b_nwnd == 0) {             /* Last use.            */
                curbp->b_dotp  = curwp->w_dotp;
                curbp->b_doto  = curwp->w_doto;
                curbp->b_markp = curwp->w_markp;
                curbp->b_marko = curwp->w_marko;
        }
        curbp = bp;                             /* Switch.              */
        curwp->w_bufp  = bp;
        curwp->w_linep = bp->b_linep;           /* For macros, ignored. */
        curwp->w_flag |= WFMODE|WFFORCE|WFHARD; /* Quite nasty.         */
        if (bp->b_nwnd++ == 0) {                /* First use.           */
                curwp->w_dotp  = bp->b_dotp;
                curwp->w_doto  = bp->b_doto;
                curwp->w_markp = bp->b_markp;
                curwp->w_marko = bp->b_marko;
                return (TRUE);
        }
        wp = wheadp;                            /* Look for old.        */
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
        return (TRUE);
}

/*
 * Dispose of a buffer, by name.
 * Ask for the name. Look it up (don't get too
 * upset if it isn't there at all!). Get quite upset
 * if the buffer is being displayed. Clear the buffer (ask
 * if the buffer has been changed). Then free the header
 * line and the buffer header. Bound to "C-X K".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int killbuffer(
  void *theEnv,
  int f,
  int n)
{
        register BUFFER *bp;
        register BUFFER *bp1;
        register BUFFER *bp2;
        register int    s;
        char            bufn[NBUFN];

        if ((s=mlreply(theEnv,"Kill buffer: ", bufn, NBUFN)) != TRUE)
                return (s);
        if ((bp=bfind(theEnv,bufn, FALSE, 0)) == NULL) /* Easy if unknown.     */
                return (TRUE);
        if (bp->b_nwnd != 0) {                  /* Error if on screen.  */
                mlwrite("Buffer is being displayed");
                return (FALSE);
        }
        if ((s=bclear(theEnv,bp)) != TRUE)             /* Blow text away.      */
                return (s);
        genfree(theEnv,(void *) bp->b_linep,(unsigned) (sizeof(LINE) + bp->b_linep->l_size));
        bp1 = NULL;                             /* Find the header.     */
        bp2 = bheadp;
        while (bp2 != bp) {
                bp1 = bp2;
                bp2 = bp2->b_bufp;
        }
        bp2 = bp2->b_bufp;                      /* Next one in chain.   */
        if (bp1 == NULL)                        /* Unlink it.           */
                bheadp = bp2;
        else
                bp1->b_bufp = bp2;
        genfree(theEnv,(void *) bp, (unsigned) sizeof(BUFFER));   /* Release buffer block */
        mlwrite("Buffer Killed!");
        return (TRUE);
}

/*
 * List all of the active
 * buffers. First update the special
 * buffer that holds the list. Next make
 * sure at least 1 window is displaying the
 * buffer list, splitting the screen if this
 * is what it takes. Lastly, repaint all of
 * the windows that are displaying the
 * list. Bound to "C-X C-B".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int listbuffers(
  void *theEnv,
  int f,
  int n)
{
        register WINDOW *wp;
        register BUFFER *bp;
        register int    s;

        if ((s=makelist(theEnv)) != TRUE)
                return (s);
        if (blistp->b_nwnd == 0) {              /* Not on screen yet.   */
                if ((wp=wpopup(theEnv)) == NULL)
                        return (FALSE);
                bp = wp->w_bufp;
                if (--bp->b_nwnd == 0) {
                        bp->b_dotp  = wp->w_dotp;
                        bp->b_doto  = wp->w_doto;
                        bp->b_markp = wp->w_markp;
                        bp->b_marko = wp->w_marko;
                }
                wp->w_bufp  = blistp;
                ++blistp->b_nwnd;
        }
        wp = wheadp;
        while (wp != NULL) {
                if (wp->w_bufp == blistp) {
                        wp->w_linep = lforw(blistp->b_linep);
                        wp->w_dotp  = lforw(blistp->b_linep);
                        wp->w_doto  = 0;
                        wp->w_markp = NULL;
                        wp->w_marko = 0;
                        wp->w_flag |= WFMODE|WFHARD;
                }
                wp = wp->w_wndp;
        }
        return (TRUE);
}

/*
 * This routine rebuilds the
 * text in the special secret buffer
 * that holds the buffer list. It is called
 * by the list buffers command. Return TRUE
 * if everything works. Return FALSE if there
 * is an error (if there is no memory).
 */
globle int makelist(
  void *theEnv)
{
        register char   *cp1;
        register char   *cp2;
        register int    c;
        register BUFFER *bp;
        register LINE   *lp;
        register int    nbytes;
        register int    s;
        char            b[6+1];
        char            line[128];

        blistp->b_flag &= ~BFCHG;               /* Don't complain!      */
        if ((s=bclear(theEnv,blistp)) != TRUE)         /* Blow old text away   */
                return (s);
        genstrcpy(blistp->b_fname, "");
        if (addline(theEnv,blistp,"C   Size Buffer           File") == FALSE
        ||  addline(theEnv,blistp,"-   ---- ------           ----") == FALSE)
                return (FALSE);
        bp = bheadp;                            /* For all buffers      */
        while (bp != NULL) {
                if ((bp->b_flag&BFTEMP) != 0) { /* Skip magic ones.     */
                        bp = bp->b_bufp;
                        continue;
                }
                cp1 = &line[0];                 /* Start at left edge   */
                if ((bp->b_flag&BFCHG) != 0)    /* "*" if changed       */
                        *cp1++ = '*';
                else
                        *cp1++ = ' ';
                *cp1++ = ' ';                   /* Gap.                 */
                nbytes = 0;                     /* Count bytes in buf.  */
                lp = lforw(bp->b_linep);
                while (lp != bp->b_linep) {
                        nbytes += llength(lp)+1;
                        lp = lforw(lp);
                }
                int_to_ascii(b, 6, nbytes);             /* 6 digit buffer size. */
                cp2 = &b[0];
                while ((c = *cp2++) != 0)
                        *cp1++ = (char) c;
                *cp1++ = ' ';                   /* Gap.                 */
                cp2 = &bp->b_bname[0];          /* Buffer name          */
                while ((c = *cp2++) != 0)
                        *cp1++ = (char) c;
                cp2 = &bp->b_fname[0];          /* File name            */
                if (*cp2 != 0) {
                        while (cp1 < &line[1+1+6+1+NBUFN+1])
                                *cp1++ = ' ';
                        while ((c = *cp2++) != 0) {
                                if (cp1 < &line[128-1])
                                        *cp1++ = (char) c;
                        }
                }
                *cp1 = 0;                       /* Add to the buffer.   */
                if (addline(theEnv,blistp,line) == FALSE)
                        return (FALSE);
                bp = bp->b_bufp;
        }
        return (TRUE);                          /* All done             */
}

/*
 * The argument "text" points to
 * a string. Append this line to the
 * buffer. Handcraft the EOL
 * on the end. Return TRUE if it worked and
 * FALSE if you ran out of room.
 */
globle int addline(
  void *theEnv,
  BUFFER *bufferp,
  char    *text)
{
        register LINE   *lp;
        register int    i;
        register int    ntext;

        ntext = strlen(text);
        if ((lp=lalloc(theEnv,ntext)) == NULL)
                return (FALSE);
        for (i=0; i<ntext; ++i)
                lputc(lp, i, text[i]);
        bufferp->b_linep->l_bp->l_fp = lp;       /* Hook onto the end    */
        lp->l_bp = bufferp->b_linep->l_bp;
        bufferp->b_linep->l_bp = lp;
        lp->l_fp = bufferp->b_linep;
        if (bufferp->b_dotp == bufferp->b_linep)  /* If "." is at the end */
                bufferp->b_dotp = lp;            /* move it to new line  */
        return (TRUE);
}

/*
 * Look through the list of
 * buffers. Return TRUE if there
 * are any changed buffers. Buffers
 * that hold magic internal stuff are
 * not considered; who cares if the
 * list of buffer names is hacked.
 * Return FALSE if no buffers
 * have been changed.
 */
globle int anycb()
{
        register BUFFER *bp;

        bp = bheadp;
        while (bp != NULL) {
                if ((bp->b_flag&BFTEMP)==0 && (bp->b_flag&BFCHG)!=0)
                        return (TRUE);
                bp = bp->b_bufp;
        }
        return (FALSE);
}

/*
 * Find a buffer, by name. Return a pointer
 * to the BUFFER structure associated with it. If
 * the named buffer is found, but is a TEMP buffer (like
 * the buffer list) conplain. If the buffer is not found
 * and the "cflag" is TRUE, create it. The "bflag" is
 * the settings for the flags in in buffer.
 */
globle BUFFER  *bfind(
  void *theEnv,
char   *bname,
int cflag,
int bflag)
{
        register BUFFER *bp;
        register LINE   *lp;

        bp = bheadp;
        while (bp != NULL) {
                if (strcmp(bname, bp->b_bname) == 0) {
                        if ((bp->b_flag&BFTEMP) != 0) {
                                mlwrite("Cannot select builtin buffer");
                                return (NULL);
                        }
                        return (bp);
                }
                bp = bp->b_bufp;
        }
        if (cflag != FALSE) {
                if ((bp=(BUFFER *)genalloc(theEnv,(unsigned) sizeof(BUFFER))) == NULL)
                        return (NULL);
                if ((lp=lalloc(theEnv,0)) == NULL) {
                        genfree(theEnv,(void *) bp,(unsigned) sizeof(BUFFER));
                        return (NULL);
                }
                bp->b_bufp  = bheadp;
                bheadp = bp;
                bp->b_dotp  = lp;
                bp->b_doto  = 0;
                bp->b_markp = NULL;
                bp->b_marko = 0;
                bp->b_flag  = (char) bflag;
                bp->b_nwnd  = 0;
                bp->b_linep = lp;
                genstrcpy(bp->b_fname, "");
                genstrcpy(bp->b_bname, bname);
                lp->l_fp = lp;
                lp->l_bp = lp;
        }
        return (bp);
}

/*
 * This routine blows away all of the text
 * in a buffer. If the buffer is marked as changed
 * then we ask if it is ok to blow it away; this is
 * to save the user the grief of losing text. The
 * window chain is nearly always wrong if this gets
 * called; the caller must arrange for the updates
 * that are required. Return TRUE if everything
 * looks good.
 */
globle int bclear(
  void *theEnv,
  BUFFER *bp)
{
        register LINE   *lp;
        register int    s;

        if ((bp->b_flag&BFTEMP) == 0            /* Not scratch buffer.  */
           && (bp->b_flag&BFCHG) != 0              /* Something changed    */
           && (s=mlyesno(theEnv,"Discard changes")) != TRUE)
                return (s);
        bp->b_flag  &= ~BFCHG;                  /* Not changed          */

        while ((lp=lforw(bp->b_linep)) != bp->b_linep)
                lfree(theEnv,lp);
        bp->b_dotp  = bp->b_linep;              /* Fix "."              */
        bp->b_doto  = 0;
        bp->b_markp = NULL;                     /* Invalidate "mark"    */
        bp->b_marko = 0;
        return (TRUE);
}

/* ==========================================================================
 *                         LINE MANAGEMENT FUNCTIONS
 * ==========================================================================
 */

/*
 * The functions in this section are a general set of line management utilities.
 * They are the only routines that touch the text. They also touch the buffer
 * and window structures, to make sure that the necessary updating gets done.
 * There are routines in this section that handle the kill buffer too. It isn't
 * here for any good reason.
 *
 * Note that this code only updates the dot and mark values in the window list.
 * Since all the code acts on the current window, the buffer that we are
 * editing must be being displayed, which means that "b_nwnd" is non zero,
 * which means that the dot and mark values in the buffer headers are nonsense.
 */

/*
 * This routine allocates a block of memory large enough to hold a LINE
 * containing "used" characters. The block is always rounded up a bit. Return
 * a pointer to the new block, or NULL if there isn't any memory left. Print a
 * message in the message line if no space.
 */
globle LINE    *lalloc(
  void *theEnv,
  int    used)
{
        register LINE   *lp;
        register int    size;

        size = (used+NBLOCK-1) & ~(NBLOCK-1);
        if (size == 0)                          /* Assume that an empty */
                size = NBLOCK;                  /* line is for type-in. */
        lp = (LINE *) genalloc(theEnv,(unsigned) sizeof(LINE)+size);
        lp->l_size = size;
        lp->l_used = used;
        return (lp);
}

/*
 * Delete line "lp". Fix all of the links that might point at it (they are
 * moved to offset 0 of the next line. Unlink the line from whatever buffer it
 * might be in. Release the memory. The buffers are updated too; the magic
 * conditions described in the above comments don't hold here.
 */
globle void lfree(
  void *theEnv,
  LINE   *lp)
{
        register BUFFER *bp;
        register WINDOW *wp;

        wp = wheadp;
        while (wp != NULL) {
                if (wp->w_linep == lp)
                        wp->w_linep = lp->l_fp;
                if (wp->w_dotp  == lp) {
                        wp->w_dotp  = lp->l_fp;
                        wp->w_doto  = 0;
                }
                if (wp->w_markp == lp) {
                        wp->w_markp = lp->l_fp;
                        wp->w_marko = 0;
                }
                wp = wp->w_wndp;
        }
        bp = bheadp;
        while (bp != NULL) {
                if (bp->b_nwnd == 0) {
                        if (bp->b_dotp  == lp) {
                                bp->b_dotp = lp->l_fp;
                                bp->b_doto = 0;
                        }
                        if (bp->b_markp == lp) {
                                bp->b_markp = lp->l_fp;
                                bp->b_marko = 0;
                        }
                }
                bp = bp->b_bufp;
        }
        lp->l_bp->l_fp = lp->l_fp;
        lp->l_fp->l_bp = lp->l_bp;
        genfree(theEnv,(void *) lp, (unsigned) (sizeof(LINE) + lp->l_size));
}

/*
 * This routine gets called when a character is changed in place in the current
 * buffer. It updates all of the required flags in the buffer and window
 * system. The flag used is passed as an argument; if the buffer is being
 * displayed in more than 1 window we change EDIT t HARD. Set MODE if the
 * mode line needs to be updated (the "*" has to be set).
 */
globle void lchange(
int    flag)
{
        register WINDOW *wp;

        if (curbp->b_nwnd != 1)                 /* Ensure hard.         */
                flag = WFHARD;
        if ((curbp->b_flag&BFCHG) == 0) {       /* First change, so     */
                flag |= WFMODE;                 /* update mode lines.   */
                curbp->b_flag |= BFCHG;
        }
        wp = wheadp;
        while (wp != NULL) {
                if (wp->w_bufp == curbp)
                        wp->w_flag |= flag;
                wp = wp->w_wndp;
        }
}

/*
 * Insert "n" copies of the character "c" at the current location of dot. In
 * the easy case all that happens is the text is stored in the line. In the
 * hard case, the line has to be reallocated. When the window list is updated,
 * take special care; I screwed it up once. You always update dot in the
 * current window. You update mark, and a dot in another window, if it is
 * greater than the place where you did the insert. Return TRUE if all is
 * well, and FALSE on errors.
 */
globle int linsert(
  void *theEnv,
  int n,
  int c)
{
        register char   *cp1;
        register char   *cp2;
        register LINE   *lp1;
        register LINE   *lp2;
        register LINE   *lp3;
        register int    doto;
        register int    i;
        register WINDOW *wp;

        lchange(WFEDIT);
        lp1 = curwp->w_dotp;                    /* Current line         */
        if (lp1 == curbp->b_linep) {            /* At the end: special  */
                if (curwp->w_doto != 0) {
                        mlwrite("bug: linsert");
                        return (FALSE);
                }
                if ((lp2=lalloc(theEnv,n)) == NULL)    /* Allocate new line    */
                        return (FALSE);
                lp3 = lp1->l_bp;                /* Previous line        */
                lp3->l_fp = lp2;                /* Link in              */
                lp2->l_fp = lp1;
                lp1->l_bp = lp2;
                lp2->l_bp = lp3;
                for (i=0; i<n; ++i)
                        lp2->l_text[i] = (char) c;
                curwp->w_dotp = lp2;
                curwp->w_doto = n;
                return (TRUE);
        }
        doto = curwp->w_doto;                   /* Save for later.      */
        if (lp1->l_used+n > lp1->l_size) {      /* Hard: reallocate     */
                if ((lp2=lalloc(theEnv,lp1->l_used+n)) == NULL)
                        return (FALSE);
                cp1 = &lp1->l_text[0];
                cp2 = &lp2->l_text[0];
                while (cp1 != &lp1->l_text[doto])
                        *cp2++ = *cp1++;
                cp2 += n;
                while (cp1 != &lp1->l_text[lp1->l_used])
                        *cp2++ = *cp1++;
                lp1->l_bp->l_fp = lp2;
                lp2->l_fp = lp1->l_fp;
                lp1->l_fp->l_bp = lp2;
                lp2->l_bp = lp1->l_bp;
                genfree(theEnv,(void *) lp1, (unsigned) (sizeof(LINE) + lp1->l_size));
        } else {                                /* Easy: in place       */
                lp2 = lp1;                      /* Pretend new line     */
                lp2->l_used += n;
                cp2 = &lp1->l_text[lp1->l_used];
                cp1 = cp2-n;
                while (cp1 != &lp1->l_text[doto])
                        *--cp2 = *--cp1;
        }
        for (i=0; i<n; ++i)                     /* Add the characters   */
                lp2->l_text[doto+i] = (char) c;
        wp = wheadp;                            /* Update windows       */
        while (wp != NULL) {
                if (wp->w_linep == lp1)
                        wp->w_linep = lp2;
                if (wp->w_dotp == lp1) {
                        wp->w_dotp = lp2;
                        if (wp==curwp || wp->w_doto>doto)
                                wp->w_doto += n;
                }
                if (wp->w_markp == lp1) {
                        wp->w_markp = lp2;
                        if (wp->w_marko > doto)
                                wp->w_marko += n;
                }
                wp = wp->w_wndp;
        }
        return (TRUE);
}

/*
 * Insert a newline into the buffer at the current location of dot in the
 * current window. The funny ass-backwards way it does things is not a botch;
 * it just makes the last line in the file not a special case. Return TRUE if
 * everything works out and FALSE on error (memory allocation failure). The
 * update of dot and mark is a bit easier then in the above case, because the
 * split forces more updating.
 */
globle int lnewline(
  void *theEnv)
{
        register char   *cp1;
        register char   *cp2;
        register LINE   *lp1;
        register LINE   *lp2;
        register int    doto;
        register WINDOW *wp;

        lchange(WFHARD);
        lp1  = curwp->w_dotp;                   /* Get the address and  */
        doto = curwp->w_doto;                   /* offset of "."        */
        if ((lp2=lalloc(theEnv,doto)) == NULL)         /* New first half line  */
                return (FALSE);
        cp1 = &lp1->l_text[0];                  /* Shuffle text around  */
        cp2 = &lp2->l_text[0];
        while (cp1 != &lp1->l_text[doto])
                *cp2++ = *cp1++;
        cp2 = &lp1->l_text[0];
        while (cp1 != &lp1->l_text[lp1->l_used])
                *cp2++ = *cp1++;
        lp1->l_used -= doto;
        lp2->l_bp = lp1->l_bp;
        lp1->l_bp = lp2;
        lp2->l_bp->l_fp = lp2;
        lp2->l_fp = lp1;
        wp = wheadp;                            /* Windows              */
        while (wp != NULL) {
                if (wp->w_linep == lp1)
                        wp->w_linep = lp2;
                if (wp->w_dotp == lp1) {
                        if (wp->w_doto < doto)
                                wp->w_dotp = lp2;
                        else
                                wp->w_doto -= doto;
                }
                if (wp->w_markp == lp1) {
                        if (wp->w_marko < doto)
                                wp->w_markp = lp2;
                        else
                                wp->w_marko -= doto;
                }
                wp = wp->w_wndp;
        }
        return (TRUE);
}

/*
 * This function deletes "n" bytes, starting at dot. It understands how do deal
 * with end of lines, etc. It returns TRUE if all of the characters were
 * deleted, and FALSE if they were not (because dot ran into the end of the
 * buffer. The "kflag" is TRUE if the text should be put in the kill buffer.
 */
globle int ldelete(
  void *theEnv,
  long n,
  int kflag)
{
        register char   *cp1;
        register char   *cp2;
        register LINE   *dotp;
        register int    doto;
        register int    chunk;
        register WINDOW *wp;

        while (n != 0) {
                dotp = curwp->w_dotp;
                doto = curwp->w_doto;
                if (dotp == curbp->b_linep)     /* Hit end of buffer.   */
                        return (FALSE);
                chunk = dotp->l_used-doto;      /* Size of chunk.       */
                if (chunk > (int) n)
                        chunk = (int) n;
                if (chunk == 0) {               /* End of line, merge.  */
                        lchange(WFHARD);
                        if (ldelnewline(theEnv) == FALSE
                        || (kflag!=FALSE && kinsert(theEnv,'\n')==FALSE))
                                return (FALSE);
                        --n;
                        continue;
                }
                lchange(WFEDIT);
                cp1 = &dotp->l_text[doto];      /* Scrunch text.        */
                cp2 = cp1 + chunk;
                if (kflag != FALSE) {           /* Kill?                */
                        while (cp1 != cp2) {
                                if (kinsert(theEnv,*cp1) == FALSE)
                                        return (FALSE);
                                ++cp1;
                        }
                        cp1 = &dotp->l_text[doto];
                }
                while (cp2 != &dotp->l_text[dotp->l_used])
                        *cp1++ = *cp2++;
                dotp->l_used -= chunk;
                wp = wheadp;                    /* Fix windows          */
                while (wp != NULL) {
                        if (wp->w_dotp==dotp && wp->w_doto>=doto) {
                                wp->w_doto -= chunk;
                                if (wp->w_doto < doto)
                                        wp->w_doto = doto;
                        }
                        if (wp->w_markp==dotp && wp->w_marko>=doto) {
                                wp->w_marko -= chunk;
                                if (wp->w_marko < doto)
                                        wp->w_marko = doto;
                        }
                        wp = wp->w_wndp;
                }
                n -= chunk;
        }
        return (TRUE);
}

/*
 * Delete a newline. Join the current line with the next line. If the next line
 * is the magic header line always return TRUE; merging the last line with the
 * header line can be thought of as always being a successful operation, even
 * if nothing is done, and this makes the kill buffer work "right". Easy cases
 * can be done by shuffling data around. Hard cases require that lines be moved
 * about in memory. Return FALSE on error and TRUE if all looks ok. Called by
 * "ldelete" only.
 */
globle int ldelnewline(
  void *theEnv)
{
        register char   *cp1;
        register char   *cp2;
        register LINE   *lp1;
        register LINE   *lp2;
        register LINE   *lp3;
        register WINDOW *wp;

        lp1 = curwp->w_dotp;
        lp2 = lp1->l_fp;
        if (lp2 == curbp->b_linep) {            /* At the buffer end.   */
                if (lp1->l_used == 0)           /* Blank line.          */
                        lfree(theEnv,lp1);
                return (TRUE);
        }
        if (lp2->l_used <= lp1->l_size-lp1->l_used) {
                cp1 = &lp1->l_text[lp1->l_used];
                cp2 = &lp2->l_text[0];
                while (cp2 != &lp2->l_text[lp2->l_used])
                        *cp1++ = *cp2++;
                wp = wheadp;
                while (wp != NULL) {
                        if (wp->w_linep == lp2)
                                wp->w_linep = lp1;
                        if (wp->w_dotp == lp2) {
                                wp->w_dotp  = lp1;
                                wp->w_doto += lp1->l_used;
                        }
                        if (wp->w_markp == lp2) {
                                wp->w_markp  = lp1;
                                wp->w_marko += lp1->l_used;
                        }
                        wp = wp->w_wndp;
                }
                lp1->l_used += lp2->l_used;
                lp1->l_fp = lp2->l_fp;
                lp2->l_fp->l_bp = lp1;
                genfree(theEnv,(void *) lp2, (unsigned) (sizeof(LINE) + lp2->l_size));
                return (TRUE);
        }
        if ((lp3=lalloc(theEnv,lp1->l_used+lp2->l_used)) == NULL)
                return (FALSE);
        cp1 = &lp1->l_text[0];
        cp2 = &lp3->l_text[0];
        while (cp1 != &lp1->l_text[lp1->l_used])
                *cp2++ = *cp1++;
        cp1 = &lp2->l_text[0];
        while (cp1 != &lp2->l_text[lp2->l_used])
                *cp2++ = *cp1++;
        lp1->l_bp->l_fp = lp3;
        lp3->l_fp = lp2->l_fp;
        lp2->l_fp->l_bp = lp3;
        lp3->l_bp = lp1->l_bp;
        wp = wheadp;
        while (wp != NULL) {
                if (wp->w_linep==lp1 || wp->w_linep==lp2)
                        wp->w_linep = lp3;
                if (wp->w_dotp == lp1)
                        wp->w_dotp  = lp3;
                else if (wp->w_dotp == lp2) {
                        wp->w_dotp  = lp3;
                        wp->w_doto += lp1->l_used;
                }
                if (wp->w_markp == lp1)
                        wp->w_markp  = lp3;
                else if (wp->w_markp == lp2) {
                        wp->w_markp  = lp3;
                        wp->w_marko += lp1->l_used;
                }
                wp = wp->w_wndp;
        }
        genfree(theEnv,(void *) lp1, (unsigned) (sizeof(LINE) + lp1->l_size));
        genfree(theEnv,(void *) lp2, (unsigned) (sizeof(LINE) + lp2->l_size));
        return (TRUE);
}

/*
 * Delete all of the text saved in the kill buffer. Called by commands when a
 * new kill context is being created. The kill buffer array is released, just
 * in case the buffer has grown to immense size. No errors.
 */
globle void kdelete(
  void *theEnv)
{
        if (kbufp != NULL) {
                genfree(theEnv,(void *) kbufp, (unsigned) ksize);
                kbufp = NULL;
                kused = 0;
                ksize = 0;
        }
}

/*
 * Insert a character to the kill buffer, enlarging the buffer if there isn't
 * any room. Always grow the buffer in chunks, on the assumption that if you
 * put something in the kill buffer you are going to put more stuff there too
 * later. Return TRUE if all is well, and FALSE on errors.
 */
globle int kinsert(
  void *theEnv,
  int c)
{
#if  WIN_MVC || WIN_BTC
	char *nbufp;
#else
        register char   *nbufp;
#endif
        register int    i;

        if (kused == ksize) {
                if ((nbufp= (char *) genalloc(theEnv,(unsigned) ksize+KBLOCK)) == NULL)
                        return (FALSE);
                for (i=0; i<ksize; ++i)
                        nbufp[i] = kbufp[i];
                if (kbufp != NULL)
                        genfree(theEnv,(void *) kbufp, (unsigned) ksize);
                kbufp  = nbufp;
                ksize += KBLOCK;
        }
        kbufp[kused++] = (char) c;
        return (TRUE);
}

/*
 * This function gets characters from the kill buffer. If the character index
 * "n" is off the end, it returns "-1". This lets the caller just scan along
 * until it gets a "-1" back.
 */
globle int kremove(
int n)
{
        if (n >= kused)
                return (-1);
        else
                return (kbufp[n] & 0xFF);
}


/* ==========================================================================
 *                        WINDOW MANAGEMENT FUNCTIONS
 * ==========================================================================
 */

/*
 * Again, some of these functions are internal, and some are
 * attached to keys that the user actually types.
 */

/*
 * Reposition dot in the current window to line "n". If the argument is
 * positive, it is that line. If it is negative it is that line from the
 * bottom. If it is 0 the window is centered (this is what the standard
 * redisplay code does). With no argument it defaults to 1. Bound to M-!.
 * Because of the default, it works like in Gosling.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int reposition(
  void *theEnv,
  int f,
  int n)
    {
    curwp->w_force = (char) n;
    curwp->w_flag |= WFFORCE;
    return (TRUE);
    }

/*
 * Refresh the screen. With no argument, it just does the refresh. With an
 * argument it recenters "." in the current window. Bound to "C-L".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int EditorRefresh(
  void *theEnv,
  int f,
  int n)
    {
    if (f == FALSE)
        sgarbf = TRUE;
    else
        {
        curwp->w_force = 0;             /* Center dot. */
        curwp->w_flag |= WFFORCE;
        }

    return (TRUE);
    }

/*
 * The command make the next window (next => down the screen) the current
 * window. There are no real errors, although the command does nothing if
 * there is only 1 window on the screen. Bound to "C-X C-N".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int nextwind(
  void *theEnv,
  int f,
  int n)
    {
    register WINDOW *wp;

    if ((wp = curwp->w_wndp) == NULL)
        wp = wheadp;

    curwp = wp;
    curbp = wp->w_bufp;
    return (TRUE);
    }

/*
 * This command makes the previous window (previous => up the screen) the
 * current window. There arn't any errors, although the command does not do a
 * lot if there is 1 window.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int prevwind(
  void *theEnv,
  int f,
  int n)
    {
    register WINDOW *wp1;
    register WINDOW *wp2;

    wp1 = wheadp;
    wp2 = curwp;

    if (wp1 == wp2)
        wp2 = NULL;

    while (wp1->w_wndp != wp2)
        wp1 = wp1->w_wndp;

    curwp = wp1;
    curbp = wp1->w_bufp;
    return (TRUE);
    }

/*
 * This command moves the current window down by "arg" lines. Recompute the
 * top line in the window. The move up and move down code is almost completely
 * the same; most of the work has to do with reframing the window, and picking
 * a new dot. We share the code by having "move down" just be an interface to
 * "move up". Magic. Bound to "C-X C-N".
 */
globle int mvdnwind(
  void *theEnv,
  int f,
  int n)
    {
    return (mvupwind(theEnv,f, -n));
    }

/*
 * Move the current window up by "arg" lines. Recompute the new top line of
 * the window. Look to see if "." is still on the screen. If it is, you win.
 * If it isn't, then move "." to center it in the new framing of the window
 * (this command does not really move "."; it moves the frame). Bound to
 * "C-X C-P".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int mvupwind(
  void *theEnv,
  int f,
  int n)
    {
    register LINE *lp;
    register int i;

    lp = curwp->w_linep;

    if (n < 0)
        {
        while (n++ && lp!=curbp->b_linep)
            lp = lforw(lp);
        }
    else
        {
        while (n-- && lback(lp)!=curbp->b_linep)
            lp = lback(lp);
        }

    curwp->w_linep = lp;
    curwp->w_flag |= WFHARD;            /* Mode line is OK. */

    for (i = 0; i < curwp->w_ntrows; ++i)
        {
        if (lp == curwp->w_dotp)
            return (TRUE);
        if (lp == curbp->b_linep)
            break;
        lp = lforw(lp);
        }

    lp = curwp->w_linep;
    i  = curwp->w_ntrows/2;

    while (i-- && lp != curbp->b_linep)
        lp = lforw(lp);

    curwp->w_dotp  = lp;
    curwp->w_doto  = 0;
    return (TRUE);
    }

/*
 * This command makes the current window the only window on the screen. Bound
 * to "C-X 1". Try to set the framing so that "." does not have to move on the
 * display. Some care has to be taken to keep the values of dot and mark in
 * the buffer structures right if the distruction of a window makes a buffer
 * become undisplayed.
 */
#if WIN_BTC
#pragma argsused
#endif
globle int onlywind(
  void *theEnv,
  int f,
  int n)
{
        register WINDOW *wp;
        register LINE   *lp;
        register int    i;

        while (wheadp != curwp) {
                wp = wheadp;
                wheadp = wp->w_wndp;
                if (--wp->w_bufp->b_nwnd == 0) {
                        wp->w_bufp->b_dotp  = wp->w_dotp;
                        wp->w_bufp->b_doto  = wp->w_doto;
                        wp->w_bufp->b_markp = wp->w_markp;
                        wp->w_bufp->b_marko = wp->w_marko;
                }
                genfree(theEnv,(void *) wp, (unsigned) sizeof(WINDOW));
        }
        while (curwp->w_wndp != NULL) {
                wp = curwp->w_wndp;
                curwp->w_wndp = wp->w_wndp;
                if (--wp->w_bufp->b_nwnd == 0) {
                        wp->w_bufp->b_dotp  = wp->w_dotp;
                        wp->w_bufp->b_doto  = wp->w_doto;
                        wp->w_bufp->b_markp = wp->w_markp;
                        wp->w_bufp->b_marko = wp->w_marko;
                }
                genfree(theEnv,(void *) wp, (unsigned) sizeof(WINDOW));
        }
        lp = curwp->w_linep;
        i  = curwp->w_toprow;
        while (i!=0 && lback(lp)!=curbp->b_linep) {
                --i;
                lp = lback(lp);
        }
        curwp->w_toprow = 0;
        curwp->w_ntrows = (char) (term.t_nrow-1);
        curwp->w_linep  = lp;
        curwp->w_flag  |= WFMODE|WFHARD;
        return (TRUE);
}

/*
 * Split the current window. A window smaller than 3 lines cannot be split.
 * The only other error that is possible is a "malloc" failure allocating the
 * structure for the new window. Bound to "C-X 2".
 */
#if WIN_BTC
#pragma argsused
#endif
globle int splitwind(
  void *theEnv,
  int f,
  int n)
{
        register WINDOW *wp;
        register LINE   *lp;
        register int    ntru;
        register int    ntrl;
        register int    ntrd;
        register WINDOW *wp1;
        register WINDOW *wp2;

        if (curwp->w_ntrows < 3) {
                mlwrite("Cannot split a %d line window", curwp->w_ntrows);
                return (FALSE);
        }
        if ((wp = (WINDOW *) genalloc(theEnv,(unsigned) sizeof(WINDOW))) == NULL) {
                mlwrite("Cannot allocate WINDOW block");
                return (FALSE);
        }
        ++curbp->b_nwnd;                        /* Displayed twice.     */
        wp->w_bufp  = curbp;
        wp->w_dotp  = curwp->w_dotp;
        wp->w_doto  = curwp->w_doto;
        wp->w_markp = curwp->w_markp;
        wp->w_marko = curwp->w_marko;
        wp->w_flag  = 0;
        wp->w_force = 0;
        ntru = (curwp->w_ntrows-1) / 2;         /* Upper size           */
        ntrl = (curwp->w_ntrows-1) - ntru;      /* Lower size           */
        lp = curwp->w_linep;
        ntrd = 0;
        while (lp != curwp->w_dotp) {
                ++ntrd;
                lp = lforw(lp);
        }
        lp = curwp->w_linep;
        if (ntrd <= ntru) {                     /* Old is upper window. */
                if (ntrd == ntru)               /* Hit mode line.       */
                        lp = lforw(lp);
                curwp->w_ntrows = (char) ntru;
                wp->w_wndp = curwp->w_wndp;
                curwp->w_wndp = wp;
                wp->w_toprow = curwp->w_toprow+ntru+1;
                wp->w_ntrows = (char) ntrl;
        } else {                                /* Old is lower window  */
                wp1 = NULL;
                wp2 = wheadp;
                while (wp2 != curwp) {
                        wp1 = wp2;
                        wp2 = wp2->w_wndp;
                }
                if (wp1 == NULL)
                        wheadp = wp;
                else
                        wp1->w_wndp = wp;
                wp->w_wndp   = curwp;
                wp->w_toprow = curwp->w_toprow;
                wp->w_ntrows = (char) ntru;
                ++ntru;                         /* Mode line.           */
                curwp->w_toprow += (char) ntru;
                curwp->w_ntrows  = (char) ntrl;
                while (ntru--)
                        lp = lforw(lp);
        }
        curwp->w_linep = lp;                    /* Adjust the top lines */
        wp->w_linep = lp;                       /* if necessary.        */
        curwp->w_flag |= WFMODE|WFHARD;
        wp->w_flag |= WFMODE|WFHARD;
        return (TRUE);
}

/*
 * Enlarge the current window. Find the window that loses space. Make sure it
 * is big enough. If so, hack the window descriptions, and ask redisplay to do
 * all the hard work. You don't just set "force reframe" because dot would
 * move. Bound to "C-X Z".
 */
globle int enlargewind(
  void *theEnv,
  int f,
  int n)
{
        register WINDOW *adjwp;
        register LINE   *lp;
        register int    i;

        if (n < 0)
                return (shrinkwind(theEnv,f, -n));
        if (wheadp->w_wndp == NULL) {
                mlwrite("Only one window");
                return (FALSE);
        }
        if ((adjwp=curwp->w_wndp) == NULL) {
                adjwp = wheadp;
                while (adjwp->w_wndp != curwp)
                        adjwp = adjwp->w_wndp;
        }
        if (adjwp->w_ntrows <= (char) n) {
                mlwrite("Impossible change");
                return (FALSE);
        }
        if (curwp->w_wndp == adjwp) {           /* Shrink below.        */
                lp = adjwp->w_linep;
                for (i=0; i<n && lp!=adjwp->w_bufp->b_linep; ++i)
                        lp = lforw(lp);
                adjwp->w_linep  = lp;
                adjwp->w_toprow += (char) n;
        } else {                                /* Shrink above.        */
                lp = curwp->w_linep;
                for (i=0; i<n && lback(lp)!=curbp->b_linep; ++i)
                        lp = lback(lp);
                curwp->w_linep  = lp;
                curwp->w_toprow -= (char) n;
        }
        curwp->w_ntrows += (char) n;
        adjwp->w_ntrows -= (char) n;
        curwp->w_flag |= WFMODE|WFHARD;
        adjwp->w_flag |= WFMODE|WFHARD;
        return (TRUE);
}

/*
 * Shrink the current window. Find the window that gains space. Hack at the
 * window descriptions. Ask the redisplay to do all the hard work. Bound to
 * "C-X C-Z".
 */
globle int shrinkwind(
  void *theEnv,
  int f,
  int n)
{
        register WINDOW *adjwp;
        register LINE   *lp;
        register int    i;

        if (n < 0)
                return (enlargewind(theEnv,f, -n));
        if (wheadp->w_wndp == NULL) {
                mlwrite("Only one window");
                return (FALSE);
        }
        if ((adjwp=curwp->w_wndp) == NULL) {
                adjwp = wheadp;
                while (adjwp->w_wndp != curwp)
                        adjwp = adjwp->w_wndp;
        }
        if (curwp->w_ntrows <= (char) n) {
                mlwrite("Impossible change");
                return (FALSE);
        }
        if (curwp->w_wndp == adjwp) {           /* Grow below.          */
                lp = adjwp->w_linep;
                for (i=0; i<n && lback(lp)!=adjwp->w_bufp->b_linep; ++i)
                        lp = lback(lp);
                adjwp->w_linep  = lp;
                adjwp->w_toprow -= (char) n;
        } else {                                /* Grow above.          */
                lp = curwp->w_linep;
                for (i=0; i<n && lp!=curbp->b_linep; ++i)
                        lp = lforw(lp);
                curwp->w_linep  = lp;
                curwp->w_toprow += (char) n;
        }
        curwp->w_ntrows -= (char) n;
        adjwp->w_ntrows += (char) n;
        curwp->w_flag |= WFMODE|WFHARD;
        adjwp->w_flag |= WFMODE|WFHARD;
        return (TRUE);
}

/*
 * Pick a window for a pop-up. Split the screen if there is only one window.
 * Pick the uppermost window that isn't the current window. An LRU algorithm
 * might be better. Return a pointer, or NULL on error.
 */
globle WINDOW  *wpopup(
  void *theEnv)
{
        register WINDOW *wp;

        if (wheadp->w_wndp == NULL              /* Only 1 window        */
        && splitwind(theEnv,FALSE, 0) == FALSE)        /* and it won't split   */
                return (NULL);
        wp = wheadp;                            /* Find window to use   */
        while (wp!=NULL && wp==curwp)
                wp = wp->w_wndp;
        return (wp);
}


/* ==========================================================================
 *                        DISPLAY MANAGEMENT FUNCTIONS
 * ==========================================================================
 */

/*
 * The functions in this section handle redisplay. There are two halves, the
 * ones that update the virtual display screen, and the ones that make the
 * physical display screen the same as the virtual display screen. These
 * functions use hints that are left in the windows by the commands.
 *
 * REVISION HISTORY:
 *
 * ?    Steve Wilhite, 1-Dec-85
 *      - massive cleanup on code.
 */

#define WFDEBUG 0                       /* Window flag debug. */

/*
 * Initialize the data structures used by the display code. The edge vectors
 * used to access the screens are set up. The operating system's terminal I/O
 * channel is set up. All the other things get initialized at compile time.
 * The original window has "WFCHG" set, so that it will get completely
 * redrawn on the first call to "update".
 */
globle void vtinit(
  void *theEnv)
{
    register int i;

    register VIDEO *vp;

    (*term.t_open)();
    vscreen = (VIDEO **) genalloc(theEnv,(unsigned) term.t_nrow*sizeof(VIDEO *));

    if (vscreen == NULL)
        exit(1);

    pscreen = (VIDEO **) genalloc(theEnv,(unsigned) term.t_nrow*sizeof(VIDEO *));

    if (pscreen == NULL)
        exit(1);

    for (i = 0; i < term.t_nrow; ++i)
        {
        vp = (VIDEO *) genalloc(theEnv,(unsigned) sizeof(VIDEO)+term.t_ncol);

        if (vp == NULL)
            exit(1);

        vscreen[i] = vp;
        vp = (VIDEO *) genalloc(theEnv,(unsigned) sizeof(VIDEO)+term.t_ncol);

        if (vp == NULL)
            exit(1);

        pscreen[i] = vp;
        }
}

/*
 * Clean up the virtual terminal system, in anticipation for a return to the
 * operating system. Move down to the last line and clear it out (the next
 * system prompt will be written in the line). Shut down the channel to the
 * terminal.
 */
globle void vttidy()
{
    movecursor(term.t_nrow, 0);
    (*term.t_eeol)();
    (*term.t_close)();
}

/*
 * Set the virtual cursor to the specified row and column on the virtual
 * screen. There is no checking for nonsense values; this might be a good
 * idea during the early stages.
 */
globle void vtmove(
int row,
int col)
{
    vtrow = row;
    vtcol = col;
}

/*
 * Write a character to the virtual screen. The virtual row and column are
 * updated. If the line is too long put a "$" in the last column. This routine
 * only puts printing characters into the virtual terminal buffers. Only
 * column overflow is checked.
 */
globle void vtputc(
    int c)
{
    register VIDEO      *vp;

    vp = vscreen[vtrow];

    if (vtcol >= term.t_ncol)
        vp->v_text[term.t_ncol - 1] = '$';
    else if (c == '\t')
        {
        do
            {
            vtputc(' ');
            }
        while ((vtcol&0x07) != 0);
        }
    else if (c < 0x20 || c == 0x7F)
        {
        vtputc('^');
        vtputc(c ^ 0x40);
        }
    else
        vp->v_text[vtcol++] = (char) c;
}

/*
 * Erase from the end of the software cursor to the end of the line on which
 * the software cursor is located.
 */
globle void vteeol()
{
    register VIDEO      *vp;

    vp = vscreen[vtrow];
    while (vtcol < term.t_ncol)
        vp->v_text[vtcol++] = ' ';
}

/*
 * Make sure that the display is right. This is a three part process. First,
 * scan through all of the windows looking for dirty ones. Check the framing,
 * and refresh the screen. Second, make sure that "currow" and "curcol" are
 * correct for the current window. Third, make the virtual and physical
 * screens the same.
 */
globle void update()
{
    register LINE *lp;
    register WINDOW *wp;
    register VIDEO *vp1;
    register VIDEO *vp2;
    register int i;
    register int j;
    register int c;

    wp = wheadp;

    while (wp != NULL)
        {
        /* Look at any window with update flags set on. */

        if (wp->w_flag != 0)
            {
            /* If not force reframe, check the framing. */

            if ((wp->w_flag & WFFORCE) == 0)
                {
                lp = wp->w_linep;

                for (i = 0; i < wp->w_ntrows; ++i)
                    {
                    if (lp == wp->w_dotp)
                        goto out;

                    if (lp == wp->w_bufp->b_linep)
                        break;

                    lp = lforw(lp);
                    }
                }

            /* Not acceptable, better compute a new value for the line at the
             * top of the window. Then set the "WFHARD" flag to force full
             * redraw.
             */
            i = wp->w_force;

            if (i > 0)
                {
                --i;

                if (i >= wp->w_ntrows)
                  i = wp->w_ntrows-1;
                }
            else if (i < 0)
                {
                i += wp->w_ntrows;

                if (i < 0)
                    i = 0;
                }
            else
                i = wp->w_ntrows/2;

            lp = wp->w_dotp;

            while (i != 0 && lback(lp) != wp->w_bufp->b_linep)
                {
                --i;
                lp = lback(lp);
                }

            wp->w_linep = lp;
            wp->w_flag |= WFHARD;       /* Force full. */

out:
            /* Try to use reduced update. Mode line update has its own special
             * flag. The fast update is used if the only thing to do is within
             * the line editing.
             */
            lp = wp->w_linep;
            i = wp->w_toprow;

            if ((wp->w_flag & ~WFMODE) == WFEDIT)
                {
                while (lp != wp->w_dotp)
                    {
                    ++i;
                    lp = lforw(lp);
                    }

                vscreen[i]->v_flag |= VFCHG;
                vtmove(i, 0);

                for (j = 0; j < llength(lp); ++j)
                    vtputc(lgetc(lp, j));

                vteeol();
                }
             else if ((wp->w_flag & (WFEDIT | WFHARD)) != 0)
                {
                while (i < wp->w_toprow+wp->w_ntrows)
                    {
                    vscreen[i]->v_flag |= VFCHG;
                    vtmove(i, 0);

                    if (lp != wp->w_bufp->b_linep)
                        {
                        for (j = 0; j < llength(lp); ++j)
                            vtputc(lgetc(lp, j));

                        lp = lforw(lp);
                        }

                    vteeol();
                    ++i;
                    }
                }
#if ~WFDEBUG
            if ((wp->w_flag&WFMODE) != 0)
                modeline(wp);

            wp->w_flag  = 0;
            wp->w_force = 0;
#endif
            }
#if WFDEBUG
        modeline(wp);
        wp->w_flag =  0;
        wp->w_force = 0;
#endif
        wp = wp->w_wndp;
        }

    /* Always recompute the row and column number of the hardware cursor. This
     * is the only update for simple moves.
     */
    lp = curwp->w_linep;
    currow = curwp->w_toprow;

    while (lp != curwp->w_dotp)
        {
        ++currow;
        lp = lforw(lp);
        }

    curcol = 0;
    i = 0;

    while (i < curwp->w_doto)
        {
        c = lgetc(lp, i++);

        if (c == '\t')
            curcol |= 0x07;
        else if (c < 0x20 || c == 0x7F)
            ++curcol;

        ++curcol;
        }
/*sprintf(prompt,"Currow: %d  curcol: %d w_toprow: %d", currow, getccol(FALSE),
curwp->w_toprow);
mlwrite(prompt);*/
    if (curcol >= term.t_ncol)          /* Long line. */
        curcol = term.t_ncol-1;

    /* Special hacking if the screen is garbage. Clear the hardware screen,
     * and update your copy to agree with it. Set all the virtual screen
     * change bits, to force a full update.
     */
    if (sgarbf != FALSE)
        {
        for (i = 0; i < term.t_nrow; ++i)
            {
            vscreen[i]->v_flag |= VFCHG;
            vp1 = pscreen[i];
            for (j = 0; j < term.t_ncol; ++j)
                vp1->v_text[j] = ' ';
            }

        movecursor(0, 0);               /* Erase the screen. */
        (*term.t_eeop)();
        sgarbf = FALSE;                 /* Erase-page clears */
        mpresf = FALSE;                 /* the message area. */
        }

    /* Make sure that the physical and virtual displays agree. Unlike before,
     * the "updateline" code is only called with a line that has been updated
     * for sure.
     */
    for (i = 0; i < term.t_nrow; ++i)
        {
        vp1 = vscreen[i];

        if ((vp1->v_flag&VFCHG) != 0)
            {
            vp1->v_flag &= ~VFCHG;
            vp2 = pscreen[i];
            updateline(i, &vp1->v_text[0], &vp2->v_text[0]);
            }
        }

    /* Finally, update the hardware cursor and flush out buffers. */

    movecursor(currow, curcol);
    (*term.t_flush)();
}

/*
 * Update a single line. This does not know how to use insert or delete
 * character sequences; we are using VT52 functionality. Update the physical
 * row and column variables. It does try an exploit erase to end of line. The
 * RAINBOW version of this routine uses fast video.
 */
globle void updateline(
    int row,
    char vline[],
    char pline[])
{
    register char *cp1;
    register char *cp2;
    register char *cp3;
    register char *cp4;
    register char *cp5;
    register int nbflag;

    cp1 = &vline[0];                    /* Compute left match.  */
    cp2 = &pline[0];

    while (cp1!=&vline[term.t_ncol] && cp1[0]==cp2[0])
        {
        ++cp1;
        ++cp2;
        }

    /* This can still happen, even though we only call this routine on changed
     * lines. A hard update is always done when a line splits, a massive
     * change is done, or a buffer is displayed twice. This optimizes out most
     * of the excess updating. A lot of computes are used, but these tend to
     * be hard operations that do a lot of update, so I don't really care.
     */
    if (cp1 == &vline[term.t_ncol])             /* All equal. */
        return;

    nbflag = FALSE;
    cp3 = &vline[term.t_ncol];          /* Compute right match. */
    cp4 = &pline[term.t_ncol];

    while (cp3[-1] == cp4[-1])
        {
        --cp3;
        --cp4;
        if (cp3[0] != ' ')              /* Note if any nonblank */
            nbflag = TRUE;              /* in right match. */
        }

    cp5 = cp3;

    if (nbflag == FALSE)                /* Erase to EOL ? */
        {
        while (cp5!=cp1 && cp5[-1]==' ')
            --cp5;

        if (cp3-cp5 <= 3)               /* Use only if erase is */
            cp5 = cp3;                  /* fewer characters. */
        }

    movecursor(row, (int) (cp1-&vline[0]));     /* Go to start of line. */

    while (cp1 != cp5)                  /* Ordinary. */
        {
        (*term.t_putchar)(*cp1);
        ++ttcol;
        *cp2++ = *cp1++;
        }

    if (cp5 != cp3)                     /* Erase. */
        {
        (*term.t_eeol)();
        while (cp1 != cp3)
            *cp2++ = *cp1++;
        }
}

/*
 * Redisplay the mode line for the window pointed to by the "wp". This is the
 * only routine that has any idea of how the modeline is formatted. You can
 * change the modeline format by hacking at this routine. Called by "update"
 * any time there is a dirty window.
 */
globle void modeline(
    WINDOW *wp)
{
    register char *cp;
    register int c;
    register int n;
    register BUFFER *bp;

    n = wp->w_toprow+wp->w_ntrows;              /* Location. */
    vscreen[n]->v_flag |= VFCHG;                /* Redraw next time. */
    vtmove(n, 0);                               /* Seek to right line. */
    vtputc('-');
    bp = wp->w_bufp;

    if ((bp->b_flag&BFCHG) != 0)                /* "*" if changed. */
        vtputc('*');
    else
        vtputc('-');

    n  = 2;
    cp = " MicroEMACS -- ";

    while ((c = *cp++) != 0)
        {
        vtputc(c);
        ++n;
        }

    cp = &bp->b_bname[0];

    while ((c = *cp++) != 0)
        {
        vtputc(c);
        ++n;
        }

    vtputc(' ');
    ++n;

    if (bp->b_fname[0] != 0)            /* File name. */
        {
        cp = "-- File: ";

        while ((c = *cp++) != 0)
            {
            vtputc(c);
            ++n;
            }

        cp = &bp->b_fname[0];

        while ((c = *cp++) != 0)
            {
            vtputc(c);
            ++n;
            }

        vtputc(' ');
        ++n;
        }

#if WFDEBUG
    vtputc('-');
    vtputc((wp->w_flag&WFMODE)!=0  ? 'M' : '-');
    vtputc((wp->w_flag&WFHARD)!=0  ? 'H' : '-');
    vtputc((wp->w_flag&WFEDIT)!=0  ? 'E' : '-');
    vtputc((wp->w_flag&WFMOVE)!=0  ? 'V' : '-');
    vtputc((wp->w_flag&WFFORCE)!=0 ? 'F' : '-');
    n += 6;
#endif

    while (n < term.t_ncol)             /* Pad to full width. */
        {
        vtputc('-');
        ++n;
        }
}

/*
 * Send a command to the terminal to move the hardware cursor to row "row"
 * and column "col". The row and column arguments are origin 0. Optimize out
 * random calls. Update "ttrow" and "ttcol".
 */
globle void movecursor(
int row,
int col)
    {
    if (row!=ttrow || col!=ttcol)
        {
        ttrow = row;
        ttcol = col;
        (*term.t_move)(row, col);
        }
    }

/*
 * Erase the message line. This is a special routine because the message line
 * is not considered to be part of the virtual screen. It always works
 * immediately; the terminal buffer is flushed via a call to the flusher.
 */
globle void mlerase()
    {
    movecursor(term.t_nrow, 0);
    (*term.t_eeol)();
    (*term.t_flush)();
    mpresf = FALSE;
    }

/*
 * Ask a yes or no question in the message line. Return either TRUE, FALSE, or
 * ABORT. The ABORT status is returned if the user bumps out of the question
 * with a ^G. Used any time a confirmation is required.
 */
globle int mlyesno(
    void *theEnv,
    char *prompt)
    {
    register int s;
    char buf[64];

    for (;;)
        {
        genstrcpy(buf, prompt);
        genstrcat(buf, " [y/n]? ");
        s = mlreply(theEnv,buf, buf, sizeof(buf));

        if (s == ABORT)
            return (ABORT);

        if (s != FALSE)
            {
            if (buf[0]=='y' || buf[0]=='Y')
                return (TRUE);

            if (buf[0]=='n' || buf[0]=='N')
                return (FALSE);
            }
        }
    }

/*
 * Write a prompt into the message line, then read back a response. Keep
 * track of the physical position of the cursor. If we are in a keyboard
 * macro throw the prompt away, and return the remembered response. This
 * lets macros run at full speed. The reply is always terminated by a
 * carriage return. Handle erase, kill, and abort keys.
 */
globle int mlreply(
   void *theEnv,
    char *prompt,
    char *buf,
    int nbuf)
    {
    register int cpos;
    register int i;
    register int c;

    cpos = 0;

    if (kbdmop != NULL)
        {
        while ((c = *kbdmop++) != '\0')
            buf[cpos++] = (char) c;

        buf[cpos] = 0;

        if (buf[0] == 0)
            return (FALSE);

        return (TRUE);
        }

    mlwrite(prompt);

    for (;;)
        {
        c = (*term.t_getchar)();

        switch (c)
            {
            case 0x0D:                  /* Return, end of line */
                buf[cpos++] = 0;

                if (kbdmip != NULL)
                    {
                    if (kbdmip+cpos > &kbdm[NKBDM-3])
                        {
                        ctrlg(theEnv,FALSE, 0);
                        (*term.t_flush)();
                        return (ABORT);
                        }

                    for (i=0; i<cpos; ++i)
                        *kbdmip++ = buf[i];
                    }

                (*term.t_putchar)('\r');
                ttcol = 0;
                (*term.t_flush)();

                if (buf[0] == 0)
                    return (FALSE);

                return (TRUE);

            case 0x07:                  /* Bell, abort */
                (*term.t_putchar)('^');
                (*term.t_putchar)('G');
                ttcol += 2;
                ctrlg(theEnv,FALSE, 0);
                (*term.t_flush)();
                return (ABORT);

            case 0x7F:                  /* Rubout, erase */
            case 0x08:                  /* Backspace, erase */
                if (cpos != 0)
                    {
                    (*term.t_putchar)('\b');
                    (*term.t_putchar)(' ');
                    (*term.t_putchar)('\b');
                    --ttcol;

                    if (buf[--cpos] < 0x20)
                        {
                        (*term.t_putchar)('\b');
                        (*term.t_putchar)(' ');
                        (*term.t_putchar)('\b');
                        --ttcol;
                        }

                    (*term.t_flush)();
                    }

                break;

            case 0x15:                  /* C-U, kill */
                while (cpos != 0)
                    {
                    (*term.t_putchar)('\b');
                    (*term.t_putchar)(' ');
                    (*term.t_putchar)('\b');
                    --ttcol;

                    if (buf[--cpos] < 0x20)
                        {
                        (*term.t_putchar)('\b');
                        (*term.t_putchar)(' ');
                        (*term.t_putchar)('\b');
                        --ttcol;
                        }
                    }

                (*term.t_flush)();
                break;

            default:
                if (cpos < nbuf-1)
                    {
                    buf[cpos++] = (char) c;

                    if (c < ' ')
                        {
                        (*term.t_putchar)('^');
                        ++ttcol;
                        c ^= 0x40;
                        }

                    (*term.t_putchar)(c);
                    ++ttcol;
                    (*term.t_flush)();
                    }
            }
        }
    }

/*
 * Write a message into the message line. Keep track of the physical cursor
 * position. A small class of printf like format items is handled. Assumes the
 * stack grows down; this assumption is made by the "++" in the argument scan
 * loop. Set the "message line" flag TRUE.
 */

globle void mlwrite(char *fmt,...)
    {
    register int c;
    va_list ap;

    movecursor(term.t_nrow, 0);
    va_start(ap,fmt);
    while ((c = *fmt++) != 0) {
        if (c != '%') {
            (*term.t_putchar)(c);
            ++ttcol;
            }
        else
            {
            c = *fmt++;
            switch (c) {
                case 'd':
                    mlputi(va_arg(ap,int), 10);
                    break;

                case 'o':
                    mlputi(va_arg(ap,int),  8);
                    break;

                case 'x':
                    mlputi(va_arg(ap,int), 16);
                    break;

                case 'D':
                    mlputli(va_arg(ap,long), 10);
                    break;

                case 's':
                    mlputs(va_arg(ap,char *));
                    break;

                default:
                    (*term.t_putchar)(c);
                    ++ttcol;
                }
            }
        }
    va_end(ap);
    (*term.t_eeol)();
    (*term.t_flush)();
    mpresf = TRUE;
    }

/*
 * Write out a string. Update the physical cursor position. This assumes that
 * the characters in the string all have width "1"; if this is not the case
 * things will get screwed up a little.
 */
globle void mlputs(
    char *s)
    {
    register int c;

    while ((c = *s++) != 0)
        {
        (*term.t_putchar)(c);
        ++ttcol;
        }
    }

/*
 * Write out an integer, in the specified radix. Update the physical cursor
 * position. This will not handle any negative numbers; maybe it should.
 */
globle void mlputi(
int i,
int r)
    {
    register int q;
    static char hexdigits[] = "0123456789ABCDEF";

    if (i < 0)
        {
        i = -i;
        (*term.t_putchar)('-');
        }

    q = i/r;

    if (q != 0)
        mlputi(q, r);

    (*term.t_putchar)(hexdigits[i%r]);
    ++ttcol;
    }

/*
 * do the same except as a long integer.
 */
globle void mlputli(
    long l,
    int r)
    {
    register long q;

    if (l < 0)
        {
        l = -l;
        (*term.t_putchar)('-');
        }

    q = l/r;

    if (q != 0)
        mlputli(q, r);

    (*term.t_putchar)((int)(l%r)+'0');
    ++ttcol;
    }


globle void kill_video_buffers(
  void *theEnv)
  {
    int i;

    for (i = 0; i < term.t_nrow; ++i) {
        genfree(theEnv,(void *) vscreen[i],(unsigned) (sizeof(VIDEO)+term.t_ncol));
        genfree(theEnv,(void *) pscreen[i],(unsigned) (sizeof(VIDEO)+term.t_ncol));
        }

    genfree(theEnv,(void *) vscreen, (unsigned) (term.t_nrow*sizeof(VIDEO *)));
    genfree(theEnv,(void *) pscreen, (unsigned) (term.t_nrow*sizeof(VIDEO *)));
   }

static void int_to_ascii(
char   buf[],
int    width,
int    num)
{
        buf[width] = 0;                         /* End of string.       */
        while (num >= 10) {                     /* Conditional digits.  */
                buf[--width] = (char) (num%10) + '0';
                num /= 10;
        }
        buf[--width] = (char) num + '0';        /* Always 1 digit.      */
        while (width != 0)                      /* Pad with blanks.     */
                buf[--width] = ' ';
}

#endif
