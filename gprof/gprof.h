/*
 * Copyright (c) 1998 University of Utah and the Flux Group.
 * All rights reserved.
 * 
 * This file is part of the Flux OSKit.  The OSKit is free software, also known
 * as "open source;" you can redistribute it and/or modify it under the terms
 * of the GNU General Public License (GPL), version 2, as published by the Free
 * Software Foundation (FSF).  To explore alternate licensing terms, contact
 * the University of Utah at csl-dist@cs.utah.edu or +1-801-585-3271.
 * 
 * The OSKit is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GPL for more details.  You should have
 * received a copy of the GPL along with the OSKit; see the file COPYING.  If
 * not, write to the FSF, 59 Temple Place #330, Boston, MA 02111-1307, USA.
 */
/*	$NetBSD: gprof.h,v 1.13 1996/04/01 21:54:06 mark Exp $	*/

/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)gprof.h	8.1 (Berkeley) 6/6/93
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <oskit/c/sys/gmon.h>

#ifdef ELF
#include <oskit/exec/elf.h>
#else
#include <a.out.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#if alpha
#   include "alpha.h"
#endif
#if arm32
#   include "arm32.h"
#endif
#if i386
#   include "i386.h"
#endif
#if m68k
#   include "m68k.h"
#endif
#if mips
#   include "mips.h"
#endif
#if ns32k
#   include "ns32k.h"
#endif
#if pmax
#   include "pmax.h"
#endif
#if sparc
#   include "sparc.h"
#endif
#if tahoe
#   include "tahoe.h"
#endif
#if vax
#   include "vax.h"
#endif


    /*
     *	who am i, for error messages.
     */
char	*whoami;

    /*
     * booleans
     */
typedef int	bool;
#define	FALSE	0
#define	TRUE	1

    /*
     *	ticks per second
     */
long	hz;

#if 0
typedef	u_short UNIT;		/* unit of profiling */
#else
typedef unsigned int UNIT;	/* unit of profiling */
#endif
char	*a_outname;
#define	A_OUTNAME		"a.out"

char	*gmonname;
#define	GMONNAME		"gmon.out"
#define	GMONSUM			"gmon.sum"

    /*
     *	a constructed arc,
     *	    with pointers to the namelist entry of the parent and the child,
     *	    a count of how many times this arc was traversed,
     *	    and pointers to the next parent of this child and
     *		the next child of this parent.
     */
struct arcstruct {
    struct nl		*arc_parentp;	/* pointer to parent's nl entry */
    struct nl		*arc_childp;	/* pointer to child's nl entry */
    long		arc_count;	/* num calls from parent to child */
    double		arc_time;	/* time inherited along arc */
    double		arc_childtime;	/* childtime inherited along arc */
    struct arcstruct	*arc_parentlist; /* parents-of-this-child list */
    struct arcstruct	*arc_childlist;	/* children-of-this-parent list */
    struct arcstruct	*arc_next;	/* list of arcs on cycle */
    unsigned short	arc_cyclecnt;	/* num cycles involved in */
    unsigned short	arc_flags;	/* see below */
};
typedef struct arcstruct	arctype;

    /*
     * arc flags
     */
#define	DEADARC	0x01	/* time should not propagate across the arc */
#define	ONLIST	0x02	/* arc is on list of arcs in cycles */

    /*
     * The symbol table;
     * for each external in the specified file we gather
     * its address, the number of calls and compute its share of cpu time.
     */
struct nl {
    char		*name;		/* the name */
    unsigned long	value;		/* the pc entry point */
    unsigned long	svalue;		/* entry point aligned to histograms */
    double		time;		/* ticks in this routine */
    double		childtime;	/* cumulative ticks in children */
    long		ncall;		/* how many times called */
    long		npropcall;	/* times called by live arcs */
    long		selfcalls;	/* how many calls to self */
    double		propfraction;	/* what % of time propagates */
    double		propself;	/* how much self time propagates */
    double		propchild;	/* how much child time propagates */
    short		printflag;	/* should this be printed? */
    short		flags;		/* see below */
    int			index;		/* index in the graph list */
    int			toporder;	/* graph call chain top-sort order */
    int			cycleno;	/* internal number of cycle on */
    int			parentcnt;	/* number of live parent arcs */
    struct nl		*cyclehead;	/* pointer to head of cycle */
    struct nl		*cnext;		/* pointer to next member of cycle */
    arctype		*parents;	/* list of caller arcs */
    arctype		*children;	/* list of callee arcs */
};
typedef struct nl	nltype;

nltype	*nl;			/* the whole namelist */
nltype	*npe;			/* the virtual end of the namelist */
int	nname;			/* the number of function names */

#define	HASCYCLEXIT	0x08	/* node has arc exiting from cycle */
#define	CYCLEHEAD	0x10	/* node marked as head of a cycle */
#define	VISITED		0x20	/* node visited during a cycle */

    /*
     * The cycle list.
     * for each subcycle within an identified cycle, we gather
     * its size and the list of included arcs.
     */
struct cl {
    int		size;		/* length of cycle */
    struct cl	*next;		/* next member of list */
    arctype	*list[1];	/* list of arcs in cycle */
    /* actually longer */
};
typedef struct cl cltype;

arctype	*archead;		/* the head of arcs in current cycle list */
cltype	*cyclehead;		/* the head of the list */
int	cyclecnt;		/* the number of cycles found */
#define	CYCLEMAX	100	/* maximum cycles before cutting one of them */

    /*
     *	flag which marks a nl entry as topologically ``busy''
     *	flag which marks a nl entry as topologically ``not_numbered''
     */
#define	DFN_BUSY	-1
#define	DFN_NAN		0

    /* 
     *	namelist entries for cycle headers.
     *	the number of discovered cycles.
     */
nltype	*cyclenl;		/* cycle header namelist */
int	ncycle;			/* number of cycles discovered */

    /*
     * The header on the gmon.out file.
     * gmon.out consists of a struct phdr (defined in gmon.h)
     * and then an array of ncnt samples representing the
     * discretized program counter values.
     *
     *	Backward compatible old style header
     */
struct ophdr {
    UNIT	*lpc;
    UNIT	*hpc;
    int		ncnt;
};
int	debug;

    /*
     * Each discretized pc sample has
     * a count of the number of samples in its range
     */
UNIT	*samples;

unsigned long	s_lowpc;	/* lowpc from the profile file */
unsigned long	s_highpc;	/* highpc from the profile file */
unsigned lowpc, highpc;		/* range profiled, in UNIT's */
unsigned sampbytes;		/* number of bytes of samples */
int	nsamples;		/* number of samples */
double	actime;			/* accumulated time thus far for putprofline */
double	totime;			/* total time for all routines */
double	printtime;		/* total of time being printed */
double	scale;			/* scale factor converting samples to pc
				   values: each sample covers scale bytes */
char	*strtab;		/* string table in core */
#ifdef ELF
Elf32_Ehdr xbuf;
Elf32_Shdr *stable;
#else
long	ssiz;			/* size of the string table */
struct	exec xbuf;		/* exec header of a.out */
#endif
unsigned char	*textspace;	/* text space of a.out in core */
int	cyclethreshold;		/* with -C, minimum cycle size to ignore */

    /*
     *	option flags, from a to z.
     */
bool	aflag;				/* suppress static functions */
bool	bflag;				/* blurbs, too */
bool	cflag;				/* discovered call graph, too */
bool	Cflag;				/* find cut-set to eliminate cycles */
bool	dflag;				/* debugging options */
bool	eflag;				/* specific functions excluded */
bool	Eflag;				/* functions excluded with time */
bool	fflag;				/* specific functions requested */
bool	Fflag;				/* functions requested with time */
bool	kflag;				/* arcs to be deleted */
bool	sflag;				/* sum multiple gmon.out files */
bool	zflag;				/* zero time/called functions, too */

    /*
     *	structure for various string lists
     */
struct stringlist {
    struct stringlist	*next;
    char		*string;
};
struct stringlist	*elist;
struct stringlist	*Elist;
struct stringlist	*flist;
struct stringlist	*Flist;
struct stringlist	*kfromlist;
struct stringlist	*ktolist;

    /*
     *	function declarations
     */
void            addlist(struct stringlist *, char *);
void		addarc(nltype *parentp, nltype *childp, long count);

int		arccmp();
arctype		*arclookup();
bool            cycleanalyze();

void            asgnsamples();
void		printblurb(char *);
void            printindex();
void		cyclelink();
void            dfn_init();
void		dfn(nltype *);

bool		dfn_busy();
void		dfn_findcycle(nltype *);
bool		dfn_numbered();

void		dfn_post_visit(nltype *);
void		dfn_pre_visit(nltype *);
void		dfn_self_cycle(nltype *);
nltype		**doarcs();

void		done();
void		findcall();

void		flatprofheader();
void		flatprofline(register nltype *);

bool		funcsymbol();

void		getnfile();
void		getpfile(char *filename);
void		getstrtab(FILE *);
void		getsymtab(FILE *);
void		gettextspace(FILE *);
void		gprofheader();
void		gprofline(register nltype *);
/*		main();
*/
unsigned long	max();
int		membercmp();
unsigned long	min();
nltype		*nllookup();
FILE		*openpfile();
long		operandlength();
operandenum	operandmode();
char		*operandname();
void		printchildren();
void		printcycle(nltype *);

void printgprof(nltype **);

void		printmembers(nltype *); 
void		printname(nltype *);
void		printparents(nltype *);
void		printprof();
void		readsamples(FILE *);

unsigned long	reladdr();

void		sortchildren(nltype *);
void		sortmembers(nltype *);
void		sortparents(nltype *);
void		tally(struct rawarc *);
bool            onlist(struct stringlist *, char *);
		
int		timecmp(nltype **npp1, nltype **npp2);
/*	topcmp();
*/
int		totalcmp();
/*
		valcmp();
*/

#define	LESSTHAN	-1
#define	EQUALTO		0
#define	GREATERTHAN	1

#define	DFNDEBUG	1
#define	CYCLEDEBUG	2
#define	ARCDEBUG	4
#define	TALLYDEBUG	8
#define	TIMEDEBUG	16
#define	SAMPLEDEBUG	32
#define	AOUTDEBUG	64
#define	CALLDEBUG	128
#define	LOOKUPDEBUG	256
#define	PROPDEBUG	512
#define	BREAKCYCLE	1024
#define	SUBCYCLELIST	2048
#define	ANYDEBUG	4096
