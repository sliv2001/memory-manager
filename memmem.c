/*
 * memmem.c
 *
 *  Created on: 27 нояб. 2022 г.
 *      Author: ivans
 */

#ifndef _HAS_MEMMEM

#include <stddef.h>
#include <memory.h>

void *memmem(const void *src,int srclen,const void *trg,int trglen)
{
	unsigned char *csrc = (unsigned char *)src;
	unsigned char *ctrg = (unsigned char *)trg;
	unsigned char *tptr,*cptr;
	int searchlen,ndx=0;

	if (src == NULL) return NULL; // or assert(haystack != NULL);
	if (srclen == 0) return NULL;
	if (trg == NULL) return NULL; // or assert(needle != NULL);
	if (trglen == 0) return NULL;

	while (ndx<=srclen) {
		cptr = &csrc[ndx];
		if ((searchlen = srclen-ndx-trglen+1) <= 0) {
			return NULL;
		} /* if */
		if ((tptr = memchr(cptr,*ctrg,searchlen)) == NULL) {
			return NULL;
		} /* if */
		if (memcmp(tptr,ctrg,trglen) == 0) {
			return tptr;
		} /* if */
		ndx += tptr-cptr+1;
	} /* while */
	return NULL;
}

#endif
