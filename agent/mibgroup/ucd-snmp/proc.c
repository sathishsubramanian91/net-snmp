#include <config.h>

#ifdef solaris2
#define _KMEMUSER	/* Needed by <sys/user.h> */
#include <sys/types.h>   /* helps define struct rlimit */
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <math.h>
#include <ctype.h>
#include <sys/types.h>
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include "mibincl.h"
#include "struct.h"
#include "proc.h"
#ifdef USING_UCD_SNMP_ERRORMIB_MODULE
#include "errormib.h"
#else
#define setPerrorstatus(x) perror(x)
#endif
#include "util_funcs.h"
#include "read_config.h"
#include "mib_module_config.h"
#include "../../../snmplib/system.h"

struct myproc *get_proc_instance __P((struct myproc *,int));
struct myproc *procwatch = NULL;
static struct extensible fixproc;
int numprocs=0;

void init_proc(void) {

/* define the structure we're going to ask the agent to register our
   information at */
  struct variable2 extensible_proc_variables[] = {
    {MIBINDEX, ASN_INTEGER, RONLY, var_extensible_proc, 1, {MIBINDEX}},
    {ERRORNAME, ASN_OCTET_STR, RONLY, var_extensible_proc, 1, {ERRORNAME}}, 
    {PROCMIN, ASN_INTEGER, RONLY, var_extensible_proc, 1, {PROCMIN}}, 
    {PROCMAX, ASN_INTEGER, RONLY, var_extensible_proc, 1, {PROCMAX}},
    {PROCCOUNT, ASN_INTEGER, RONLY, var_extensible_proc, 1, {PROCCOUNT}},
    {ERRORFLAG, ASN_INTEGER, RONLY, var_extensible_proc, 1, {ERRORFLAG}},
    {ERRORMSG, ASN_OCTET_STR, RONLY, var_extensible_proc, 1, {ERRORMSG}},
    {ERRORFIX, ASN_INTEGER, RWRITE, var_extensible_proc, 1, {ERRORFIX }}
  };

/* Define the OID pointer to the top of the mib tree that we're
   registering underneath */
  oid proc_variables_oid[] = { EXTENSIBLEMIB,PROCMIBNUM,1 };

  /* register ourselves with the agent to handle our mib tree */
  REGISTER_MIB("ucd-snmp/proc", extensible_proc_variables, variable2, \
               proc_variables_oid);

  snmpd_register_config_handler("proc", proc_parse_config, proc_free_config,
                                "process-name [max-num] [min-num]");
}


/* Define snmpd.conf reading routines first.  They get called
   automatically by the invocation of a macro in the proc.h file. */

void proc_free_config __P((void)) {
  struct myproc *ptmp, *ptmp2;
  
  for (ptmp = procwatch; ptmp != NULL;) {
    ptmp2 = ptmp;
    ptmp = ptmp->next;
    free(ptmp2);
  }
  procwatch = NULL;
  numprocs = 0;
}

void proc_parse_config(word,cptr)
  char *word;
  char *cptr;
{
  /* skip past used ones */
  struct myproc **procp = &procwatch;
  while (*procp != NULL)
    procp = &((*procp)->next);
  
  (*procp) = (struct myproc *) malloc(sizeof(struct myproc));
  (*procp)->next = NULL;
  numprocs++;
  /* not blank and not a comment */
  copy_word(cptr,(*procp)->name);
  cptr = skip_not_white(cptr);
  if ((cptr = skip_white(cptr))) 
    {
      (*procp)->max = atoi(cptr);
      cptr = skip_not_white(cptr);
      if ((cptr = skip_white(cptr)))
        (*procp)->min = atoi(cptr);
      else 
        (*procp)->min = 0;
    }
  else
    {
      (*procp)->max = 0;
      (*procp)->min = 0;
    }
  DEBUGMSGTL(("ucd-snmp/proc", "Read:  %s (%d) (%d)\n",
              (*procp)->name, (*procp)->max, (*procp)->min));
}

/* The routine that handles everything */

unsigned char *var_extensible_proc(vp, name, length, exact, var_len, write_method)
    register struct variable *vp;
/* IN - pointer to variable entry that points here */
    register oid	*name;
/* IN/OUT - input name requested, output name found */
    register int	*length;
/* IN/OUT - length of input and output oid's */
    int			exact;
/* IN - TRUE if an exact match was requested. */
    int			*var_len;
/* OUT - length of variable or 0 if function returned. */
    int			(**write_method) __P((int, u_char *,u_char, int, u_char *, oid *, int));
/* OUT - pointer to function to set variable, otherwise 0 */
{

  struct myproc *proc;
  static long long_ret;
  static char errmsg[300];


  if (!checkmib(vp,name,length,exact,var_len,write_method,numprocs))
    return(NULL);
  
  if ((proc = get_proc_instance(procwatch,name[*length-1]))) {
    switch (vp->magic) {
      case MIBINDEX:
        long_ret = name[*length-1];
        return((u_char *) (&long_ret));
      case ERRORNAME:   /* process name to check for */
        *var_len = strlen(proc->name);
        return((u_char *) (proc->name));
      case PROCMIN:
        long_ret = proc->min;
        return((u_char *) (&long_ret));
      case PROCMAX:
        long_ret = proc->max;
        return ((u_char *) (&long_ret));
      case PROCCOUNT:
        long_ret = sh_count_procs(proc->name);
        return ((u_char *) (& long_ret));
      case ERRORFLAG:
        long_ret = sh_count_procs(proc->name);
        if (long_ret >= 0 &&
            ((proc->min && long_ret < proc->min) || 
            (proc->max && long_ret > proc->max) ||
            (proc->min == 0 && proc->max == 0 && long_ret < 1))) {
          long_ret = 1;
        }
        else {
          long_ret = 0;
        }
        return ((u_char *) (& long_ret));
      case ERRORMSG:
        long_ret = sh_count_procs(proc->name);
        if (long_ret < 0) {
          errmsg[0] = 0;   /* catch out of mem errors return 0 count */
        } else if (proc->min && long_ret < proc->min) {
          sprintf(errmsg,"Too few %s running (# = %d)",
                  proc->name, (int) long_ret);
        }
        else if (proc->max && long_ret > proc->max) {
          sprintf(errmsg,"Too many %s running (# = %d)",
                  proc->name, (int) long_ret);
        }
        else if (proc->min == 0 && proc->max == 0 && long_ret < 1) {
          sprintf(errmsg,"No %s process running.", proc->name);
        }
        else {
          errmsg[0] = 0;
        }
        *var_len = strlen(errmsg);
        return((u_char *) errmsg);
      case ERRORFIX:
        *write_method = fixProcError;
        long_return = fixproc.result;
        return ((u_char *) &long_return);
    }
    return NULL;
  }
  return NULL;
}

int
fixProcError(action, var_val, var_val_type, var_val_len, statP, name, name_len)
   int      action;
   u_char   *var_val;
   u_char   var_val_type;
   int      var_val_len;
   u_char   *statP;
   oid      *name;
   int      name_len;
{
  
  struct myproc *proc;
  long tmp=0;
  int tmplen=1000;

  if ((proc = get_proc_instance(procwatch,name[10]))) {
    if (var_val_type != ASN_INTEGER) {
      printf("Wrong type != int\n");
      return SNMP_ERR_WRONGTYPE;
    }
    asn_parse_int(var_val,&tmplen,&var_val_type,&tmp,sizeof(int));
    if (tmp == 1 && action == COMMIT) {
#ifdef PROCFIXCMD
      sprintf(fixproc.command,PROCFIXCMD,proc->name);
      exec_command(&fixproc);
#endif
    } 
    return SNMP_ERR_NOERROR;
  }
  return SNMP_ERR_WRONGTYPE;
}

struct myproc *get_proc_instance(proc,inst)
     int inst;
     struct myproc *proc;
{
  int i;
  
  if (proc == NULL) return(NULL);
  for (i=1;i != inst && proc != NULL; i++) proc = proc->next;
  return(proc);
}

#ifdef bsdi2
#include <sys/param.h>
#include <sys/sysctl.h>

#define PP(pp, field) ((pp)->kp_proc . field)
#define EP(pp, field) ((pp)->kp_eproc . field)
#define VP(pp, field) ((pp)->kp_eproc.e_vm . field)

/* these are for keeping track of the proc array */

static int nproc = 0;
static int onproc = -1;
static struct kinfo_proc *pbase = 0;

int sh_count_procs(procname)
  char *procname;
{
  register int i,ret = 0;
  register struct kinfo_proc *pp;
  static int mib[] = { CTL_KERN, KERN_PROC , KERN_PROC_ALL };

  if (sysctl(mib, 3, NULL, &nproc, NULL, 0) < 0) return 0;

  if(nproc > onproc || !pbase) {
    if((pbase = (struct kinfo_proc*) realloc(pbase, 
                                             nproc + sizeof(struct kinfo_proc))) == 0) return -1;
    onproc = nproc;
    memset(pbase,0,nproc + sizeof(struct kinfo_proc));
  }

  if (sysctl(mib, 3, pbase, &nproc, NULL, 0) < 0) return -1;
   
  for (pp = pbase, i = 0; i < nproc / sizeof(struct kinfo_proc); pp++, i++)
    {
      if (PP(pp, p_stat) != 0 && (((PP(pp, p_flag) & P_SYSTEM) == 0)))
	{
          if (PP(pp, p_stat) != SZOMB && !strcmp(PP(pp,p_comm),procname)) ret++;
	}
    }
  return ret;
}
#elif OSTYPE == ULTRIXID

#define	NPROCS		32		/* number of proces to read at once */

static struct user *getuser __P((struct proc *));
static int getword __P((off_t));
static int getstruct __P((off_t, char *, off_t, int));

extern int kmem, mem, swap;

#include <sys/user.h>
#include <sys/proc.h>
#include <sys/file.h>
#include <sys/vm.h>
#include <machine/pte.h>
#ifdef HAVE_NLIST_H
#include <nlist.h>
#endif

static struct nlist proc_nl[] = {
	{ "_nproc" },
#define X_NPROC		0
	{ "_proc" },
#define X_PROC		1
	{ "_proc_bitmap" },
#define X_PROC_BITMAP	2
	{ NULL }
};

int
sh_count_procs(procname)
	char	*procname;
{
	int total, proc_active, nproc;
	int thisproc = 0;
	int absolute_proc_number = -1;
	struct user *auser;
	struct proc *aproc, *procp;
	unsigned bitmap;
	struct proc procs[NPROCS], *procsp;
	static int inited = 0;

	procp = (struct proc *)getword(proc_nl[X_PROC].n_value);
	nproc = getword(proc_nl[X_NPROC].n_value);

	total = 0;
	for (;;) {
		do {
			while (thisproc == 0) {
				int nread;
				int psize;

				if (nproc == 0)
					return(total);
			
				thisproc = MIN(NPROCS, nproc);
				psize = thisproc * sizeof(struct proc);
				nproc -= thisproc;
				if (lseek(kmem, (off_t)procp, L_SET) == -1 ||
				    (nread = read(kmem, (char *)procs, psize)) < 0) {
					/* warn("read proc"); */
					return(total);
				}
				else if (nread != psize) {
					thisproc = nread / sizeof(struct proc);
					nproc = 0;
					/* warn("read proc: short read"); */
				}
				procsp = procs;
				procp += thisproc;
			}
	
			aproc = procsp++;
			thisproc--;

			absolute_proc_number++;
			if ((absolute_proc_number % 32) == 0)
				bitmap = getword((unsigned int)proc_nl[X_PROC_BITMAP].n_value
				    + ((absolute_proc_number / 32) * 4));
			proc_active = (bitmap & (1 << (absolute_proc_number % 32))) != 0;
			if (proc_active && aproc->p_stat != SZOMB && !(aproc->p_type & SWEXIT))
				auser = getuser(aproc);
		} while (!proc_active || auser == NULL);

		if (strcmp(auser->u_comm, procname) == 0)
			total ++;
	}
}

#define	SW_UADDR	dtob(getword(dmap.dm_ptdaddr))
#define	SW_UBYTES	sizeof(struct user)

#define	SKRD(file, src, dst, size)			\
	(lseek(file, (off_t)(src), L_SET) == -1) ||	\
	(read(file, (char *)(dst), (size)) != (size))

static struct user *
getuser(aproc)
	struct proc *aproc;
{
	static union {
		struct user user;
		char upgs[UPAGES][NBPG];
	} u;
	static struct pte uptes[UPAGES];
	static struct dmap dmap;
	int i, nbytes;

	/*
	 * If process is not in core, we simply snarf it's user struct
	 * from the swap device.
	 */
	if ((aproc->p_sched & SLOAD) == 0) {
		if (!getstruct((off_t)aproc->p_smap, "aproc->p_smap", &dmap,
		    sizeof(dmap))) {
			/* warnx("can't read dmap for pid %d from %s", aproc->p_pid,
			    _PATH_DRUM); */
			return(NULL);
		}
		if (SKRD(swap, SW_UADDR, &u.user, SW_UBYTES)) {
			/* warnx("can't read u for pid %d from %s", aproc->p_pid, _PATH_DRUM); */
			return(NULL);
		}
		return (&u.user);
	}

	/*
	 * Process is in core.  Follow p_addr to read in the page
	 * table entries that map the u-area and then read in the
	 * physical pages that comprise the u-area.
	 *
	 * If at any time, an lseek() or read() fails, print a warning
	 * message and return NULL.
	 */
	if (SKRD(kmem, aproc->p_addr, uptes, sizeof(uptes))) {
		/* warnx("can't read user pt for pid %d from %s", aproc->p_pid, _PATH_DRUM); */
		return(NULL);
	}
	
	nbytes = sizeof(struct user);
	for (i = 0; i < UPAGES && nbytes > 0; i++) {
		if (SKRD(mem, ptob(uptes[i].pg_pfnum), u.upgs[i], NBPG)) {
			/* warnx("can't read user page %u for pid %d from %s",
			    uptes[i].pg_pfnum, aproc->p_pid, _PATH_MEM); */
			return(NULL);
		}
		nbytes -= NBPG;
	}
	return(&u.user);
}

static int
getword(loc)
	off_t loc;
{
	int val;

	if (SKRD(kmem, loc, &val, sizeof(val)))
		exit(1);
	return(val);
}

static int
getstruct(loc, name, dest, size)
	off_t loc;
	char *name;
	off_t dest;
	int size;
{
	if(SKRD(kmem,loc,dest,size))
		return(0);
	return(1);
}
#elif OSTYPE == SOLARISID

#define _KMEMUSER	/* Needed by <sys/user.h> */

#include <kvm.h>
#include <fcntl.h>
#include <sys/user.h>
#include <sys/proc.h>

int
sh_count_procs(procname)
	char *procname;
{
	static kvm_t *kd = NULL;
	struct proc *p;
	struct user *u;
	int total;

	if (kd == NULL) {
		kd = kvm_open(NULL, NULL, NULL, O_RDONLY, "sunps");
                if( !kd ) {
                  return(-1);
                }
		/* error check! */
	}
        if( kvm_setproc(kd) < 0 ) {
          return( -1 );
        }
	kvm_setproc(kd);
	total = 0;
	while ((p = kvm_nextproc(kd)) != NULL) {
                if( !p ) {
                        return( -1 );
                }
		u = kvm_getu(kd, p);
                /* Skip this entry if u or u->u_comm is a NULL pointer */
                if( !u ) {
                        continue;
                }
		if (strcmp(procname, u->u_comm) == 0)
			total++;
	}
	return(total);
}
#else
int sh_count_procs(procname)
     char *procname;
{
  char line[STRMAX], *cptr;
  int ret=0, fd;
  FILE *file;
#ifndef EXCACHETIME
#endif
  struct extensible ex;
  
  if ((fd = get_ps_output(&ex)) > 0) {
    if ((file = fdopen(fd,"r")) == NULL) {
      setPerrorstatus("fdopen");
      return (-1);
    }
    while(fgets(line,STRMAX,file) != NULL)
      {
        if ((cptr = find_field(line,LASTFIELD)) == NULL)
          continue;
        copy_word(cptr,line);
        if (!strcmp(line,procname)) ret++;
      }
    if (ftell(file) < 2) {
#ifdef USING_UCD_SNMP_ERRORMIB_MODULE
      seterrorstatus("process list unreasonable short (mem?)",2);
#endif
      ret = -1;
    }
    fclose(file);
    close(fd);
    wait_on_exec(&ex);
    ex.pid = 0;
  } else {
    ret = -1;
  }
  return(ret);
}
#endif

int get_ps_output(ex)
  struct extensible *ex;
{
  int fd;

  strcpy(ex->command,PSCMD);
  fd = get_exec_output(ex);
  return(fd);
} 

