/* This file was generated by mib2c and is intended for use as
   a mib module for the ucd-snmp snmpd agent. */


/* This should always be included first before anything else */
#include <config.h>

#include <sys/types.h>
#if HAVE_WINSOCK_H
#include <winsock.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif


/* minimal include directives */
#include "mibincl.h"
#include "header_complex.h"
#include "snmpNotifyFilterTable.h"
#include "snmp-tc.h"


/* 
 * snmpNotifyFilterTable_variables_oid:
 *   this is the top level oid that we want to register under.  This
 *   is essentially a prefix, with the suffix appearing in the
 *   variable below.
 */


oid snmpNotifyFilterTable_variables_oid[] = { 1,3,6,1,6,3,13,1,3 };


/* 
 * variable2 snmpNotifyFilterTable_variables:
 *   this variable defines function callbacks and type return information 
 *   for the snmpNotifyFilterTable mib section 
 */


struct variable2 snmpNotifyFilterTable_variables[] = {
/*  magic number        , variable type , ro/rw , callback fn  , L, oidsuffix */
#define   SNMPNOTIFYFILTERMASK  4
  { SNMPNOTIFYFILTERMASK, ASN_OCTET_STR , RWRITE, var_snmpNotifyFilterTable, 2, { 1,2 } },
#define   SNMPNOTIFYFILTERTYPE  5
  { SNMPNOTIFYFILTERTYPE, ASN_INTEGER   , RWRITE, var_snmpNotifyFilterTable, 2, { 1,3 } },
#define   SNMPNOTIFYFILTERSTORAGETYPE  6
  { SNMPNOTIFYFILTERSTORAGETYPE, ASN_INTEGER   , RWRITE, var_snmpNotifyFilterTable, 2, { 1,4 } },
#define   SNMPNOTIFYFILTERROWSTATUS  7
  { SNMPNOTIFYFILTERROWSTATUS, ASN_INTEGER   , RWRITE, var_snmpNotifyFilterTable, 2, { 1,5 } },

};
/*    (L = length of the oidsuffix) */


/* global storage of our data, saved in and configured by header_complex() */
static struct header_complex_index *snmpNotifyFilterTableStorage = NULL;




/*
 * init_snmpNotifyFilterTable():
 *   Initialization routine.  This is called when the agent starts up.
 *   At a minimum, registration of your variables should take place here.
 */
void init_snmpNotifyFilterTable(void) {
  DEBUGMSGTL(("snmpNotifyFilterTable", "initializing...  "));


  /* register ourselves with the agent to handle our mib tree */
  REGISTER_MIB("snmpNotifyFilterTable", snmpNotifyFilterTable_variables, variable2,
               snmpNotifyFilterTable_variables_oid);


  /* register our config handler(s) to deal with registrations */
  snmpd_register_config_handler("snmpNotifyFilterTable", parse_snmpNotifyFilterTable, NULL, NULL);


  /* we need to be called back later to store our data */
  snmp_register_callback(SNMP_CALLBACK_LIBRARY, SNMP_CALLBACK_STORE_DATA,
                         store_snmpNotifyFilterTable, NULL);


  /* place any other initialization junk you need here */


  DEBUGMSGTL(("snmpNotifyFilterTable", "done.\n"));
}


/* 
 * snmpNotifyFilterTable_add(): adds a structure node to our data set 
 */
int
snmpNotifyFilterTable_add(struct snmpNotifyFilterTable_data *thedata) {
  struct variable_list *vars = NULL;


  DEBUGMSGTL(("snmpNotifyFilterTable", "adding data...  "));
 /* add the index variables to the varbind list, which is 
    used by header_complex to index the data */

  snmp_varlist_add_variable(&vars, NULL, 0, ASN_OCTET_STR, (u_char *) thedata->snmpNotifyFilterProfileName, thedata->snmpNotifyFilterProfileNameLen);
  snmp_varlist_add_variable(&vars, NULL, 0, ASN_PRIV_IMPLIED_OBJECT_ID, (u_char *) thedata->snmpNotifyFilterSubtree, thedata->snmpNotifyFilterSubtreeLen*sizeof(oid)); /* snmpNotifyFilterSubtree */

  header_complex_add_data(&snmpNotifyFilterTableStorage, vars, thedata);
  DEBUGMSGTL(("snmpNotifyFilterTable","registered an entry\n"));

  DEBUGMSGTL(("snmpNotifyFilterTable", "done.\n"));
  return SNMPERR_SUCCESS;
}


/*
 * parse_snmpNotifyFilterTable():
 *   parses .conf file entries needed to configure the mib.
 */
void
parse_snmpNotifyFilterTable(const char *token, char *line) {
  size_t tmpint;
  struct snmpNotifyFilterTable_data *StorageTmp = SNMP_MALLOC_STRUCT(snmpNotifyFilterTable_data);


    DEBUGMSGTL(("snmpNotifyFilterTable", "parsing config...  "));


  if (StorageTmp == NULL) {
    config_perror("malloc failure");
    return;
  }


  line = read_config_read_data(ASN_OCTET_STR, line, &StorageTmp->snmpNotifyFilterProfileName, &StorageTmp->snmpNotifyFilterProfileNameLen);
  if (StorageTmp->snmpNotifyFilterProfileName == NULL) {
    config_perror("invalid specification for snmpNotifyFilterProfileName");
    return;
  }

  line = read_config_read_data(ASN_OBJECT_ID, line, &StorageTmp->snmpNotifyFilterSubtree, &StorageTmp->snmpNotifyFilterSubtreeLen);
  if (StorageTmp->snmpNotifyFilterSubtree == NULL) {
    config_perror("invalid specification for snmpNotifyFilterSubtree");
    return;
  }
  
  line = read_config_read_data(ASN_OCTET_STR, line, &StorageTmp->snmpNotifyFilterMask, &StorageTmp->snmpNotifyFilterMaskLen);

  line = read_config_read_data(ASN_INTEGER, line, &StorageTmp->snmpNotifyFilterType, &tmpint);

  line = read_config_read_data(ASN_INTEGER, line, &StorageTmp->snmpNotifyFilterStorageType, &tmpint);

  line = read_config_read_data(ASN_INTEGER, line, &StorageTmp->snmpNotifyFilterRowStatus, &tmpint);

  snmpNotifyFilterTable_add(StorageTmp);

  DEBUGMSGTL(("snmpNotifyFilterTable", "done.\n"));
}




/*
 * store_snmpNotifyFilterTable():
 *   stores .conf file entries needed to configure the mib.
 */
int
store_snmpNotifyFilterTable(int majorID, int minorID, void *serverarg, void *clientarg) {
  char line[SNMP_MAXBUF];
  char *cptr;
  size_t tmpint;
  struct snmpNotifyFilterTable_data *StorageTmp;
  struct header_complex_index *hcindex;


  DEBUGMSGTL(("snmpNotifyFilterTable", "storing data...  "));


  for(hcindex=snmpNotifyFilterTableStorage; hcindex != NULL; 
      hcindex = hcindex->next) {
    StorageTmp = (struct snmpNotifyFilterTable_data *) hcindex->data;


    if (StorageTmp->snmpNotifyFilterStorageType == ST_NONVOLATILE) {

        memset(line,0,sizeof(line));
        strcat(line, "snmpNotifyFilterTable ");
        cptr = line + strlen(line);

        cptr = read_config_store_data(ASN_OCTET_STR, cptr, &StorageTmp->snmpNotifyFilterProfileName, &StorageTmp->snmpNotifyFilterProfileNameLen);
        cptr = read_config_store_data(ASN_OBJECT_ID, cptr, &StorageTmp->snmpNotifyFilterSubtree, &StorageTmp->snmpNotifyFilterSubtreeLen);
        cptr = read_config_store_data(ASN_OCTET_STR, cptr, &StorageTmp->snmpNotifyFilterMask, &StorageTmp->snmpNotifyFilterMaskLen);
        cptr = read_config_store_data(ASN_INTEGER, cptr, &StorageTmp->snmpNotifyFilterType, &tmpint);
        cptr = read_config_store_data(ASN_INTEGER, cptr, &StorageTmp->snmpNotifyFilterStorageType, &tmpint);
        cptr = read_config_store_data(ASN_INTEGER, cptr, &StorageTmp->snmpNotifyFilterRowStatus, &tmpint);



        snmpd_store_config(line);
    }
  }
  DEBUGMSGTL(("snmpNotifyFilterTable", "done.\n"));
  return 0;
}




/*
 * var_snmpNotifyFilterTable():
 *   Handle this table separately from the scalar value case.
 *   The workings of this are basically the same as for var_snmpNotifyFilterTable above.
 */
unsigned char *
var_snmpNotifyFilterTable(struct variable *vp,
    	    oid     *name,
    	    size_t  *length,
    	    int     exact,
    	    size_t  *var_len,
    	    WriteMethod **write_method)
{


struct snmpNotifyFilterTable_data *StorageTmp = NULL;


  DEBUGMSGTL(("snmpNotifyFilterTable", "var_snmpNotifyFilterTable: Entering...  \n"));
  /* 
   * this assumes you have registered all your data properly
   */
  if ((StorageTmp =
       header_complex(snmpNotifyFilterTableStorage, vp,name,length,exact,
                      var_len,write_method)) == NULL) {
      if (vp->magic == SNMPNOTIFYFILTERROWSTATUS)
          *write_method = write_snmpNotifyFilterRowStatus;
      return NULL;
  }


  /* 
   * this is where we do the value assignments for the mib results.
   */
  switch(vp->magic) {


    case SNMPNOTIFYFILTERMASK:
        *write_method = write_snmpNotifyFilterMask;
        *var_len = StorageTmp->snmpNotifyFilterMaskLen;
        return (u_char *) StorageTmp->snmpNotifyFilterMask;

    case SNMPNOTIFYFILTERTYPE:
        *write_method = write_snmpNotifyFilterType;
        *var_len = sizeof(StorageTmp->snmpNotifyFilterType);
        return (u_char *) &StorageTmp->snmpNotifyFilterType;

    case SNMPNOTIFYFILTERSTORAGETYPE:
        *write_method = write_snmpNotifyFilterStorageType;
        *var_len = sizeof(StorageTmp->snmpNotifyFilterStorageType);
        return (u_char *) &StorageTmp->snmpNotifyFilterStorageType;

    case SNMPNOTIFYFILTERROWSTATUS:
        *write_method = write_snmpNotifyFilterRowStatus;
        *var_len = sizeof(StorageTmp->snmpNotifyFilterRowStatus);
        return (u_char *) &StorageTmp->snmpNotifyFilterRowStatus;


    default:
      ERROR_MSG("");
  }
  return NULL;
}




int
write_snmpNotifyFilterMask(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t    name_len)
{
  static char * tmpvar;
  struct snmpNotifyFilterTable_data *StorageTmp = NULL;
  static size_t tmplen;
  size_t newlen=name_len - (sizeof(snmpNotifyFilterTable_variables_oid)/sizeof(oid) + 3 - 1);

  DEBUGMSGTL(("snmpNotifyFilterTable", "write_snmpNotifyFilterMask entering action=%d...  \n", action));
  if ((StorageTmp =
       header_complex(snmpNotifyFilterTableStorage, NULL,
                      &name[sizeof(snmpNotifyFilterTable_variables_oid)/sizeof(oid) + 3 - 1], 
                      &newlen, 1, NULL, NULL)) == NULL)
      return SNMP_ERR_NOSUCHNAME; /* remove if you support creation here */


  switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_OCTET_STR){
              fprintf(stderr, "write to snmpNotifyFilterMask not ASN_OCTET_STR\n");
              return SNMP_ERR_WRONGTYPE;
          }
          break;


        case RESERVE2:
             /* memory reseveration, final preparation... */
          break;


        case FREE:
             /* Release any resources that have been allocated */
          break;


        case ACTION:
             /* The variable has been stored in string for
             you to use, and you have just been asked to do something with
             it.  Note that anything done here must be reversable in the UNDO case */
             tmpvar = StorageTmp->snmpNotifyFilterMask;
             tmplen = StorageTmp->snmpNotifyFilterMaskLen;
             memdup((u_char **) &StorageTmp->snmpNotifyFilterMask, var_val, var_val_len);
             StorageTmp->snmpNotifyFilterMaskLen = var_val_len;
          break;


        case UNDO:
             /* Back out any changes made in the ACTION case */
             SNMP_FREE(StorageTmp->snmpNotifyFilterMask);
             StorageTmp->snmpNotifyFilterMask = tmpvar;
             StorageTmp->snmpNotifyFilterMaskLen = tmplen;
          break;


        case COMMIT:
             /* Things are working well, so it's now safe to make the change
             permanently.  Make sure that anything done here can't fail! */
     SNMP_FREE(tmpvar);
          break;
  }
  return SNMP_ERR_NOERROR;
}



int
write_snmpNotifyFilterType(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t    name_len)
{
  static int tmpvar;
  struct snmpNotifyFilterTable_data *StorageTmp = NULL;
  size_t newlen=name_len - (sizeof(snmpNotifyFilterTable_variables_oid)/sizeof(oid) + 3 - 1);


  DEBUGMSGTL(("snmpNotifyFilterTable", "write_snmpNotifyFilterType entering action=%d...  \n", action));
  if ((StorageTmp =
       header_complex(snmpNotifyFilterTableStorage, NULL,
                      &name[sizeof(snmpNotifyFilterTable_variables_oid)/sizeof(oid) + 3 - 1], 
                      &newlen, 1, NULL, NULL)) == NULL)
      return SNMP_ERR_NOSUCHNAME; /* remove if you support creation here */


  switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_INTEGER){
              fprintf(stderr, "write to snmpNotifyFilterType not ASN_INTEGER\n");
              return SNMP_ERR_WRONGTYPE;
          }
          break;


        case RESERVE2:
             /* memory reseveration, final preparation... */
          break;


        case FREE:
             /* Release any resources that have been allocated */
          break;


        case ACTION:
             /* The variable has been stored in long_ret for
             you to use, and you have just been asked to do something with
             it.  Note that anything done here must be reversable in the UNDO case */
             tmpvar = StorageTmp->snmpNotifyFilterType;
             StorageTmp->snmpNotifyFilterType = *((long *) var_val);
          break;


        case UNDO:
             /* Back out any changes made in the ACTION case */
             StorageTmp->snmpNotifyFilterType = tmpvar;
          break;


        case COMMIT:
             /* Things are working well, so it's now safe to make the change
             permanently.  Make sure that anything done here can't fail! */

          break;
  }
  return SNMP_ERR_NOERROR;
}



int
write_snmpNotifyFilterStorageType(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t    name_len)
{
  static int tmpvar;
  struct snmpNotifyFilterTable_data *StorageTmp = NULL;
  size_t newlen=name_len - (sizeof(snmpNotifyFilterTable_variables_oid)/sizeof(oid) + 3 - 1);


  DEBUGMSGTL(("snmpNotifyFilterTable", "write_snmpNotifyFilterStorageType entering action=%d...  \n", action));
  if ((StorageTmp =
       header_complex(snmpNotifyFilterTableStorage, NULL,
                      &name[sizeof(snmpNotifyFilterTable_variables_oid)/sizeof(oid) + 3 - 1], 
                      &newlen, 1, NULL, NULL)) == NULL)
      return SNMP_ERR_NOSUCHNAME; /* remove if you support creation here */


  switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_INTEGER){
              fprintf(stderr, "write to snmpNotifyFilterStorageType not ASN_INTEGER\n");
              return SNMP_ERR_WRONGTYPE;
          }
          break;


        case RESERVE2:
             /* memory reseveration, final preparation... */
          break;


        case FREE:
             /* Release any resources that have been allocated */
          break;


        case ACTION:
             /* The variable has been stored in long_ret for
             you to use, and you have just been asked to do something with
             it.  Note that anything done here must be reversable in the UNDO case */
             tmpvar = StorageTmp->snmpNotifyFilterStorageType;
             StorageTmp->snmpNotifyFilterStorageType = *((long *) var_val);
          break;


        case UNDO:
             /* Back out any changes made in the ACTION case */
             StorageTmp->snmpNotifyFilterStorageType = tmpvar;
          break;


        case COMMIT:
             /* Things are working well, so it's now safe to make the change
             permanently.  Make sure that anything done here can't fail! */

          break;
  }
  return SNMP_ERR_NOERROR;
}






int
write_snmpNotifyFilterRowStatus(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t    name_len)
{
  struct snmpNotifyFilterTable_data *StorageTmp = NULL;
  static struct snmpNotifyFilterTable_data *StorageNew, *StorageDel;
  size_t newlen=name_len - (sizeof(snmpNotifyFilterTable_variables_oid)/sizeof(oid) + 3 - 1);
  static int old_value;
  int set_value;
  static struct variable_list *vars, *vp;
  struct header_complex_index *hciptr;


  StorageTmp =
    header_complex(snmpNotifyFilterTableStorage, NULL,
                   &name[sizeof(snmpNotifyFilterTable_variables_oid)/sizeof(oid) + 3 - 1], 
                   &newlen, 1, NULL, NULL);
  

  

  if (var_val_type != ASN_INTEGER || var_val == NULL){
    fprintf(stderr, "write to snmpNotifyFilterRowStatus not ASN_INTEGER\n");
    return SNMP_ERR_WRONGTYPE;
  }
  set_value = *((long *) var_val);


  /* check legal range, and notReady is reserved for us, not a user */
  if (set_value < 1 || set_value > 6 || set_value == RS_NOTREADY)
    return SNMP_ERR_INCONSISTENTVALUE;
    

  switch ( action ) {
        case RESERVE1:
  /* stage one: test validity */
          if (StorageTmp == NULL) {
            /* create the row now? */


            /* ditch illegal values now */
            if (set_value == RS_ACTIVE || set_value == RS_NOTINSERVICE)
              return SNMP_ERR_INCONSISTENTVALUE;
    

            /* destroying a non-existent row is actually legal */
            if (set_value == RS_DESTROY) {
              return SNMP_ERR_NOERROR;
            }


            /* illegal creation values */
            if (set_value == RS_ACTIVE || set_value == RS_NOTINSERVICE) {
              return SNMP_ERR_INCONSISTENTVALUE;
            }
          } else {
            /* row exists.  Check for a valid state change */
            if (set_value == RS_CREATEANDGO || set_value == RS_CREATEANDWAIT) {
              /* can't create a row that exists */
              return SNMP_ERR_INCONSISTENTVALUE;
            }
    /* XXX: interaction with row storage type needed */
          }
          break;




        case RESERVE2:
          /* memory reseveration, final preparation... */
          if (StorageTmp == NULL) {
            /* creation */
            vars = NULL;

  	  snmp_varlist_add_variable(&vars, NULL, 0, ASN_OCTET_STR, NULL, 0); /* snmpNotifyFilterSubtree */
  	  snmp_varlist_add_variable(&vars, NULL, 0, ASN_PRIV_IMPLIED_OBJECT_ID, NULL, 0); /* snmpNotifyFilterSubtree */

            if (header_complex_parse_oid(&(name[sizeof(snmpNotifyFilterTable_variables_oid)/sizeof(oid)+2]), newlen, vars) != SNMPERR_SUCCESS) {
              /* XXX: free, zero vars */
              return SNMP_ERR_INCONSISTENTNAME;
            }
            vp = vars;

            StorageNew = SNMP_MALLOC_STRUCT(snmpNotifyFilterTable_data);
            memdup((u_char **) &(StorageNew->snmpNotifyFilterProfileName), 
                   (u_char *) vp->val.string, vp->val_len);
            StorageNew->snmpNotifyFilterProfileNameLen = vp->val_len;
            vp = vp->next_variable;
            memdup((u_char **) &(StorageNew->snmpNotifyFilterSubtree), 
                   (u_char *) vp->val.objid, vp->val_len);
            StorageNew->snmpNotifyFilterSubtreeLen = vp->val_len/sizeof(oid);

            StorageNew->snmpNotifyFilterMask = calloc(1,1);
            StorageNew->snmpNotifyFilterMaskLen = 0;
            StorageNew->snmpNotifyFilterType = SNMPNOTIFYFILTERTYPE_INCLUDED;
            StorageNew->snmpNotifyFilterStorageType = ST_NONVOLATILE;
            StorageNew->snmpNotifyFilterRowStatus = set_value;
            /* XXX: free, zero vars, no longer needed? */
          }
          

          break;




        case FREE:
          /* XXX: free, zero vars */
          /* Release any resources that have been allocated */
          break;




        case ACTION:
             /* The variable has been stored in set_value for you to
             use, and you have just been asked to do something with
             it.  Note that anything done here must be reversable in
             the UNDO case */
             

             if (StorageTmp == NULL) {
               /* row creation, so add it */
               if (StorageNew != NULL)
                 snmpNotifyFilterTable_add(StorageNew);
               /* XXX: ack, and if it is NULL? */
             } else if (set_value != RS_DESTROY) {
               /* set the flag? */
               old_value = StorageTmp->snmpNotifyFilterRowStatus;
               StorageTmp->snmpNotifyFilterRowStatus = *((long *) var_val);
             } else {
               /* destroy...  extract it for now */
               hciptr =
                 header_complex_find_entry(snmpNotifyFilterTableStorage,
                                           StorageTmp);
               StorageDel =
                 header_complex_extract_entry(&snmpNotifyFilterTableStorage,
                                              hciptr);
             }
          break;




        case UNDO:
             /* Back out any changes made in the ACTION case */
             if (StorageTmp == NULL) {
               /* row creation, so remove it again */
               hciptr =
                 header_complex_find_entry(snmpNotifyFilterTableStorage,
                                           StorageTmp);
               StorageDel =
                 header_complex_extract_entry(&snmpNotifyFilterTableStorage,
                                              hciptr);
               /* XXX: free it */
             } else if (StorageDel != NULL) {
               /* row deletion, so add it again */
               snmpNotifyFilterTable_add(StorageDel);
             } else {
               StorageTmp->snmpNotifyFilterRowStatus = old_value;
             }
          break;




        case COMMIT:
             /* Things are working well, so it's now safe to make the change
             permanently.  Make sure that anything done here can't fail! */
          if (StorageDel != NULL) {
            StorageDel = NULL;
            /* XXX: free it, its dead */
          } else {
              if (StorageTmp && StorageTmp->snmpNotifyFilterRowStatus == RS_CREATEANDGO) {
                  StorageTmp->snmpNotifyFilterRowStatus = RS_ACTIVE;
              } else if (StorageTmp &&
                         StorageTmp->snmpNotifyFilterRowStatus == RS_CREATEANDWAIT) {
                  StorageTmp->snmpNotifyFilterRowStatus = RS_NOTINSERVICE;
              }
          }
          break;
  }
  return SNMP_ERR_NOERROR;
}




