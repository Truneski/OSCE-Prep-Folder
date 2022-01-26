/*
blowfish.h:  Header file for blowfish.c

Copyright (C) 1997 by Paul Kocher

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

See blowfish.c for more information about this file.
*/

/*
   Modified by Hong Jen Yee (PCMan).
   2005.02.04: Add header guards and extern "C" to be used with C++.
   2005.03.31: Add dll exports for MS Windows.
*/

#ifndef _BLOWFISH_H_
#define _BLOWFISH_H_
#ifdef  __cplusplus
extern "C"{
#endif

#if	defined(BLOWFISH_EXPORTS)
	#define DLL_FUNC	__declspec(dllexport)
#elif	defined(_USRDLL)
	#define DLL_FUNC	__declspec(dllimport)
#else
	#define DLL_FUNC
#endif

typedef struct {
  unsigned long P[16 + 2];
  unsigned long S[4][256];
} BLOWFISH_CTX;

DLL_FUNC void Blowfish_Init(BLOWFISH_CTX *ctx, unsigned char *key, int keyLen);

DLL_FUNC void Blowfish_Encrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);

DLL_FUNC void Blowfish_Decrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);

#ifdef  __cplusplus
}
#endif
#endif // _BLOWFISH_H_

