/*
	GLOBAL.H - RSAEURO types and constants

	Copyright (c) J.S.A.Kapp 1994 - 1995.

	RSAEURO - RSA Library compatible with RSAREF(tm) 2.0.

	All functions prototypes are the Same as for RSAREF(tm).
	To aid compatiblity the source and the files follow the
	same naming comventions that RSAREF(tm) uses.  This should aid
	direct importing to your applications.

	This library is legal everywhere outside the US.  And should
	NOT be imported to the US and used there.

	All Trademarks Acknowledged.

	Global types and contants file.

	Revision 1.00 - JSAK 23/6/95, Final Release Version
*/

#ifndef _DLLEMVBASEGLOBAL_H_
#define _DLLEMVBASEGLOBAL_H_


/* PROTOTYPES should be set to one if and only if the compiler supports
		 function argument prototyping.
	 The following makes PROTOTYPES default to 1 if it has not already been
		 defined as 0 with C compiler flags. */

#ifndef PROTOTYPES
#define PROTOTYPES 1
#endif



/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short UINT2;

/* UINT4 defines a four byte word */
typedef unsigned int UINT4;

/* BYTE defines a unsigned character */
//typedef unsigned char BYTE;

/* internal signed value */
typedef signed int signeddigit;

#ifndef NULL_PTR
#define NULL_PTR ((POINTER)0)
#endif

#ifndef UNUSED_ARG
#define UNUSED_ARG(x) x = *(&x);
#endif

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
	 If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
	 returns an empty list. */

#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

#endif /* _GLOBAL_H_ */

