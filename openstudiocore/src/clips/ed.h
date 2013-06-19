   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.21  06/15/03            */
   /*                                                     */
   /*                  EMACS HEADER FILE                  */
   /*******************************************************/

/*
 * This file is the general header file for all parts of the MicroEMACS
 * display editor.  It contains definitions used by everyone, and it contains
 * the stuff you have to edit to create a version of the editor for a
 * specific operating system and terminal.
 */

#ifndef _EDITOR_HEADER_
#define _EDITOR_HEADER_

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>
#include <string.h>

#include "argacces.h"
#include "constant.h"
#include "constrct.h"
#include "memalloc.h"
#include "evaluatn.h"
#include "router.h"
#include "scanner.h"
#include "symbol.h"
#include "sysdep.h"

#if     VAX_VMS
#define ANSI    1                       /* Normally used for VAX VMS    */
#define VT52    0                       /* VT52 terminal (Zenith).      */
#define TERMCAP 0                       /* Use TERMCAP                  */
#define IBM_PC  0                       /* Standard IBM PC BIOS         */
#endif

#if     UNIX_7 || UNIX_V || LINUX || DARWIN
#define ANSI    0                       /* Normally used for VAX VMS    */
#define VT52    0                       /* VT52 terminal (Zenith).      */
#define TERMCAP 1                       /* Use TERMCAP                  */
#define IBM_PC  0                       /* Standard IBM PC BIOS         */
#endif

#if     WIN_MVC || WIN_BTC
#define ANSI    0                       /* Normally used for VAX VMS    */
#define VT52    0                       /* VT52 terminal (Zenith).      */
#define TERMCAP 0                       /* Use TERMCAP                  */
#define IBM_PC  1                       /* Standard IBM PC BIOS         */
#endif

#if     WIN_GCC
#define ANSI    0                       /* Normally used for VAX VMS    */
#define VT52    0                       /* VT52 terminal (Zenith).      */
#define TERMCAP 0                       /* Use TERMCAP                  */
#define IBM_PC  1                       /* Standard IBM PC BIOS         */
#endif


#define CVMVAS  1                       /* C-V, M-V arg. in screens.    */

#define VERSION_NUM "3.0"               /* Emacs editor Version number  */

#define NFILEN  256                     /* # of bytes, file name        */
#define NBUFN   16                      /* # of bytes, buffer name      */
#define NLINE   256                     /* # of bytes, line             */
#define NKBDM   256                     /* # of strokes, keyboard macro */
#define NPAT    80                      /* # of bytes, pattern          */
#define HUGE    1000                    /* Huge number                  */

#define AGRAVE  0x60                    /* M- prefix,   Grave (LK201)   */
#define METACH  0x1B                    /* M- prefix,   Control-[, ESC  */
#define CTMECH  0x1C                    /* C-M- prefix, Control-\       */
#define EXITCH  0x1D                    /* Exit level,  Control-]       */
#define COTLCH  0x1E                    /* C- prefix,   Control-^       */
#define HELPCH  0x1F                    /* Help key,    Control-_       */
#define DEL_KEY 0x7F                    /* Del key, most keyboards      */

#define COTL    0x0100                  /* Control flag, or'ed in       */
#define META    0x0200                  /* Meta flag, or'ed in          */
#define CTLX    0x0400                  /* ^X flag, or'ed in            */

#define ABORT   2                       /* Death, ^G, abort, etc.       */

#define FIOSUC  0                       /* File I/O, success.           */
#define FIOFNF  1                       /* File I/O, file not found.    */
#define FIOEOF  2                       /* File I/O, end of file.       */
#define FIOERR  3                       /* File I/O, error.             */

#define CFCPCN  0x0001                  /* Last command was C-P, C-N    */
#define CFKILL  0x0002                  /* Last command was a kill      */

/*
 * There is a window structure allocated for every active display window. The
 * windows are kept in a big list, in top to bottom screen order, with the
 * listhead at "wheadp". Each window contains its own values of dot and mark.
 * The flag field contains some bits that are set by commands to guide
 * redisplay; although this is a bit of a compromise in terms of decoupling,
 * the full blown redisplay is just too expensive to run for every input
 * character.
 */
typedef struct  WINDOW {
        struct  WINDOW *w_wndp;         /* Next window                  */
        struct  BUFFER *w_bufp;         /* Buffer displayed in window   */
        struct  LINE *w_linep;          /* Top line in the window       */
        struct  LINE *w_dotp;           /* Line containing "."          */
        short   w_doto;                 /* Byte offset for "."          */
        struct  LINE *w_markp;          /* Line containing "mark"       */
        short   w_marko;                /* Byte offset for "mark"       */
        char    w_toprow;               /* Origin 0 top row of window   */
        char    w_ntrows;               /* # of rows of text in window  */
        char    w_force;                /* If NZ, forcing row.          */
        char    w_flag;                 /* Flags.                       */
}       WINDOW;

#define WFFORCE 0x01                    /* Window needs forced reframe  */
#define WFMOVE  0x02                    /* Movement from line to line   */
#define WFEDIT  0x04                    /* Editing within a line        */
#define WFHARD  0x08                    /* Better to a full display     */
#define WFMODE  0x10                    /* Update mode line.            */

/*
 * Text is kept in buffers. A buffer header, described below, exists for every
 * buffer in the system. The buffers are kept in a big list, so that commands
 * that search for a buffer by name can find the buffer header. There is a
 * safe store for the dot and mark in the header, but this is only valid if
 * the buffer is not being displayed (that is, if "b_nwnd" is 0). The text for
 * the buffer is kept in a circularly linked list of lines, with a pointer to
 * the header line in "b_linep".
 */
typedef struct  BUFFER {
        struct  BUFFER *b_bufp;         /* Link to next BUFFER          */
        struct  LINE *b_dotp;           /* Link to "." LINE structure   */
        short   b_doto;                 /* Offset of "." in above LINE  */
        struct  LINE *b_markp;          /* The same as the above two,   */
        short   b_marko;                /* but for the "mark"           */
        struct  LINE *b_linep;          /* Link to the header LINE      */
        char    b_nwnd;                 /* Count of windows on buffer   */
        char    b_flag;                 /* Flags                        */
        char    b_fname[NFILEN];        /* File name                    */
        char    b_bname[NBUFN];         /* Buffer name                  */
}       BUFFER;

#define BFTEMP  0x01                    /* Internal temporary buffer    */
#define BFCHG   0x02                    /* Changed since last write     */

/*
 * The starting position of a region, and the size of the region in
 * characters, is kept in a region structure.  Used by the region commands.
 */
typedef struct  {
        struct  LINE *r_linep;          /* Origin LINE address.         */
        short   r_offset;               /* Origin LINE offset.          */
        long    r_size;                 /* Length in characters.        */
}       REGION;

/*
 * All text is kept in circularly linked lists of "LINE" structures. These
 * begin at the header line (which is the blank line beyond the end of the
 * buffer). This line is pointed to by the "BUFFER". Each line contains a the
 * number of bytes in the line (the "used" size), the size of the text array,
 * and the text. The end of line is not stored as a byte; it's implied. Future
 * additions will include update hints, and a list of marks into the line.
 */
typedef struct  LINE {
        struct  LINE *l_fp;             /* Link to the next line        */
        struct  LINE *l_bp;             /* Link to the previous line    */
        short   l_size;                 /* Allocated size               */
        short   l_used;                 /* Used size                    */
        char    l_text[1];              /* A bunch of characters.       */
}       LINE;

#define lforw(lp)       ((lp)->l_fp)
#define lback(lp)       ((lp)->l_bp)
#define lgetc(lp, n)    ((lp)->l_text[(int) (n)]&0xFF)
#define lputc(lp, n, c) ((lp)->l_text[(int) (n)]= (char) (c))
#define llength(lp)     ((lp)->l_used)

/*
 * The editor communicates with the display using a high level interface. A
 * "TERM" structure holds useful variables, and indirect pointers to routines
 * that do useful operations. The low level get and put routines are here too.
 * This lets a terminal, in addition to having non standard commands, have
 * funny get and put character code too. The calls might get changed to
 * "termp->t_field" style in the future, to make it possible to run more than
 * one terminal type.
 */


typedef struct  {
        short   t_nrow;                 /* Number of rows.              */
        short   t_ncol;                 /* Number of columns.           */
        void    (*t_open)(void);        /* Open terminal at the start.  */
        void    (*t_close)(void);       /* Close terminal at end.       */
        int     (*t_getchar)(void);     /* Get character from keyboard. */
        void    (*t_putchar)(int);      /* Put character to display.    */
        void    (*t_flush)(void);       /* Flush output buffers.        */
        void    (*t_move)(int,int);     /* Move the cursor, origin 0.   */
        void    (*t_eeol)(void);        /* Erase to end of line.        */
        void    (*t_eeop)(void);        /* Erase to end of page.        */
        void    (*t_beep)(void);        /* Beep.                        */
}       TERM;


#if IBM_PC || WIN_GCC
/*
 *  This section defines the code returned by all the special keys on
 *  a PC numeric Keypad. It could also be used to define the function
 *  keys if desired.
 */

#define UP_ARROW	256
#define DOWN_ARROW	257
#define RIGHT_ARROW	258
#define LEFT_ARROW	259
#define HOME_KEY	260
#define END_KEY  	261
#define PGUP_KEY	262
#define PGDN_KEY	263

#define COTL_RIGHT_ARROW	264
#define COTL_LEFT_ARROW	 	265

#define COTL_AT_SIGN	266

#endif

#if IBM_PC || VAX_VMS

#define BADKEY		999

#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _EDBASIC_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE int gotobol(void *,int,int);
LOCALE int backchar(void *,int,int);
LOCALE int gotoeol(void *,int,int);
LOCALE int forwchar(void *,int,int);
LOCALE int gotobob(void *,int,int);
LOCALE int gotoeob(void *,int,int);
LOCALE int forwline(void *,int,int);
LOCALE int backline(void *,int,int);
LOCALE int getgoal(LINE *);
LOCALE int forwpage(void *,int,int);
LOCALE int backpage(void *,int,int);
LOCALE int setmark(void *,int,int);
LOCALE int swapmark(void *,int,int);
LOCALE int wrapword(void *);
LOCALE int backword(void *,int,int);
LOCALE int forwword(void *,int,int);
LOCALE int upperword(void *,int,int);
LOCALE int lowerword(void *,int,int);
LOCALE int capword(void *,int,int);
LOCALE int delfword(void *,int,int);
LOCALE int delbword(void *,int,int);
LOCALE int inword(void);
LOCALE int killregion(void *,int,int);
LOCALE int copyregion(void *,int,int);
LOCALE int upperregion(void *,int,int);
LOCALE int lowerregion(void *,int,int);
LOCALE int getregion(REGION *);
LOCALE int fileread(void *,int,int);
LOCALE int filevisit(void *,int,int);
LOCALE int filevisit_guts(void *,char []);
LOCALE int readin(void *,char []);
LOCALE int makename(char [],char []);
LOCALE int filewrite(void *,int,int);
LOCALE int filesave(void *,int,int);
LOCALE int writeout(char *);
LOCALE int filename(void *,int,int);
LOCALE int ffropen(char *);
LOCALE int ffwopen(char *);
LOCALE int ffclose(void);
LOCALE int ffputline(char [],int);
LOCALE int ffgetline(char [],int);
LOCALE void ttopen(void);
LOCALE void ttclose(void);
LOCALE void ttputc(int);
LOCALE void ttflush(void);
LOCALE int ttgetc(void);

#if VAX_VMS
LOCALE int parse_esc_seq(void);
#endif

#undef LOCALE
#ifdef _EDMAIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void edinit(void *,char []);
LOCALE int execute(void *,int,int,int);
LOCALE int getkey(void);
LOCALE int getctl(void);
LOCALE int quickexit(void *,int,int);
LOCALE int edquit(void *,int,int);
LOCALE int temp_quit(void *,int,int);
LOCALE int ctlxlp(void *,int,int);
LOCALE int ctlxrp(void *,int,int);
LOCALE int ctlxe(void *,int,int);
LOCALE int ctrlg(void *,int,int);
LOCALE void full_cleanup(void *);
LOCALE int kill_all_buffers(void *,BUFFER **);
LOCALE int kill_all_windows(void *);
LOCALE int spec_clear(void *,BUFFER *);
LOCALE void EditCommand(void *);
LOCALE void EditorFunctionDefinition(void *);

#ifndef _EDMAIN_SOURCE_

extern int     currow;
extern int     curcol;
extern int     fillcol;
extern int     thisflag;
extern int     lastflag;
extern int     curgoal;
extern BUFFER  *curbp;
extern WINDOW  *curwp;
extern BUFFER  *bheadp;
extern WINDOW  *wheadp;
extern BUFFER  *blistp;
extern short   kbdm[NKBDM];
extern short   *kbdmip;
extern short   *kbdmop;
extern char    pat[NPAT];
extern char    lastbufn[NBUFN];
extern BUFFER *CompileBufferp;

#endif


#undef LOCALE
#ifdef _EDMISC_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE int compile_region(void *,int,int);
LOCALE int compile_file(void *,int,int);
LOCALE int get_compile(void *,char *,char *);
LOCALE int region_fnd(void *,char *);
LOCALE int region_getc(void *,char *);
LOCALE int region_ungetc(void *,int,char *);
LOCALE int buffer_fnd(void *,char *);
LOCALE int buffer_getc(void *,char *);
LOCALE int buffer_ungetc(void *,int,char *);
LOCALE int query_cmp(void *,char *);
LOCALE int print_cmp(void *,char *,char *);
LOCALE void init_cmp_router(void *);
LOCALE void kill_cmp_router(void *);
LOCALE int setfillcol(void *,int,int);
LOCALE int showcpos(void *,int,int);
LOCALE int getccol(int);
LOCALE int getcline(void);
LOCALE int cntlines(void);
LOCALE int gotoline(void *,int,int);
LOCALE int twiddle(void *,int,int);
LOCALE int quote(void *,int,int);
LOCALE int tab(void *,int,int);
LOCALE int openline(void *,int,int);
LOCALE int newline(void *,int,int);
LOCALE int deblank(void *,int,int);
LOCALE int indent(void *,int,int);
LOCALE int forwdel(void *,int,int);
LOCALE int backdel(void *,int,int);
LOCALE int kill_fwd(void *,int,int);
LOCALE int yank(void *,int,int);
LOCALE int forwsearch(void *,int,int);
LOCALE int backsearch(void *,int,int);
LOCALE int bkwrdrpl(void *,int,int);
LOCALE int bkwrdcr(void *,int,int);
LOCALE int frwsr(void *,int,int);
LOCALE int querysr(void *,int,int);
LOCALE int lreplace(void *,char *);
LOCALE int smatchb(void *,int,int);
LOCALE int searchcl(int);
LOCALE int searchop(int);
LOCALE int readpattern(void *,char *);
LOCALE int spawncli(void *,int,int);
LOCALE int spawn(void *,int,int);

#if VAX_VMS
LOCALE int sys(char *);
#endif


#undef LOCALE
#ifdef _EDSTRUCT_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE int usebuffer(void *,int,int);
LOCALE int killbuffer(void *,int,int);
LOCALE int listbuffers(void *,int,int);
LOCALE int makelist(void *);
LOCALE int addline(void *,BUFFER *,char *);
LOCALE int anycb(void);
LOCALE BUFFER *bfind(void *,char *,int,int);
LOCALE int bclear(void *,BUFFER *);
LOCALE LINE *lalloc(void *,int);
LOCALE void lfree(void *,LINE *);
LOCALE void lchange(int);
LOCALE int linsert(void *,int,int);
LOCALE int lnewline(void *);
LOCALE int ldelete(void *,long,int);
LOCALE int ldelnewline(void *);
LOCALE void kdelete(void *);
LOCALE int kinsert(void *,int);
LOCALE int kremove(int);
LOCALE int reposition(void *,int,int);
LOCALE int EditorRefresh(void *,int,int);
LOCALE int nextwind(void *,int,int);
LOCALE int prevwind(void *,int,int);
LOCALE int mvdnwind(void *,int,int);
LOCALE int mvupwind(void *,int,int);
LOCALE int onlywind(void *,int,int);
LOCALE int splitwind(void *,int,int);
LOCALE int enlargewind(void *,int,int);
LOCALE int shrinkwind(void *,int,int);
LOCALE WINDOW *wpopup(void *);
LOCALE void vtinit(void *);
LOCALE void vttidy(void);
LOCALE void vtmove(int,int);
LOCALE void vtputc(int);
LOCALE void vteeol(void);
LOCALE void update(void);
LOCALE void updateline(int,char [],char []);
LOCALE void modeline(WINDOW *);
LOCALE void movecursor(int,int);
LOCALE void mlerase(void);
LOCALE int mlyesno(void *,char *);
LOCALE int mlreply(void *,char *,char *,int);
LOCALE void mlwrite(char *,...);
LOCALE void mlputs(char *);
LOCALE void mlputi(int,int);
LOCALE void mlputli(long,int);
LOCALE void kill_video_buffers(void *);

#ifndef _EDSTRUCT_SOURCE_
extern int     mpresf;
extern int     sgarbf;
#endif

#ifndef _EDTERM_SOURCE_
extern TERM term;
#endif

#endif




