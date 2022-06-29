/*******************************************************************
 * $Header: /home/kender/muddev/marxkove/include/RCS/board.h,v 1.2 1995/02/17 22
:01:22 kender Exp $
 *
 * $Revision: 1.1 $
 * $Date: 1996/02/15 01:36:15 $
 * $State: Exp $
 * $Author: kender $
 * $Locker: kender $
 *
 *-----------------------------------------------------------------------
 *                         Kove Development Code
 *
 * $Log: darray.h,v $
 * Revision 1.1  1996/02/15  01:36:15  kender
 * Initial revision
 *
 *
 *********************************************************************/

#ifndef __DARRAY_H__
#define __DARRAY_H__

typedef int (comFunc)(const void*, const void*);

#ifdef __DPRIVATE
typedef struct __darray 
{
  void 		**dataBlock;
  int		size,step,dirty;
  comFunc	*sortFunc,*searchFunc; 
} darray;

typedef darray* pdarray;
#else
typedef char* pdarray;
#endif

/*
 * dynamic array functions
 */

pdarray newArray(int size, int step, comFunc *sort, comFunc *search);
void    delArray(pdarray array);

void*   arrayGet(pdarray array, int index);
void*	arraySet(pdarray array, int index, void *newVal);
void*	arrayAdd(pdarray array, void* info);
void*   arrayRemove(pdarray array, void* info);
void*	arrayFind(pdarray array, void* key);
void*	arrayLFind(pdarray array, void* key);
void*	arrayFindWith(pdarray array, void* key, comFunc *search);
void*	arrayLFindWith(pdarray array, void* key, comFunc *search);
void	arraySort(pdarray array );
int	arrayGetSize(pdarray array);
int	arraySetSize(pdarray array, int newSize);
int	arrayGetStep(pdarray array);
int	arraySetStep(pdarray array, int newStep);

#endif
