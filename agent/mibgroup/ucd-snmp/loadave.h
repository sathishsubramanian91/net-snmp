/*
 *  Loadaveess watching mib group
 */
#ifndef _MIBGROUP_LOADAVE_H
#define _MIBGROUP_LOADAVE_H

config_require(util_funcs)

void	init_loadave __P((void));
unsigned char *var_extensible_loadave __P((struct variable *, oid *, int *, int, int *, int (**write) __P((int, u_char *, u_char, int, u_char *, oid *, int)) ));

/* config file parsing routines */
void loadave_parse_config __P((char *, char *));
void loadave_free_config __P((void));

#include "mibdefs.h"

#define LOADAVE 3
#define LOADMAXVAL 4

#endif /* _MIBGROUP_LOADAVE_H */
