/*
 * Copyright (c) 1996, 1998, 1999 University of Utah and the Flux Group.
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

/*
 * Machine dependent inline interrupt definitions.
 */

#ifndef _THREADS_X86_INT_H_
#define _THREADS_X86_INT_H_

#include <oskit/x86/proc_reg.h>
#include <oskit/x86/eflags.h>

#define machine_intr_enable()	    (sti())
#define machine_intr_disable()	    (cli())
#define machine_intr_enabled()      (get_eflags() & EFL_IF)
#define machine_intr_save_disable() \
	({ int ret = get_eflags() & EFL_IF; cli(); ret; })

#endif /* _THREADS_X86_INT_H_ */
