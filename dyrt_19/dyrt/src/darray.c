#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define __DPRIVATE
#include "darray.h"
#define MAX(a,b) ( (a) > (b) ? (a) : (b) )

/*
 * __resize
 *  This function will resize a dynamic array to the given
 *  size.  Note that data may be lost if the size is shrunk.
 */
static void __resize( pdarray array, int newSize)
{
  void **pBlock;

  int copySize = ( newSize < array->size ) ? newSize : array->size;
  int index;

  if( newSize > 0)
    {
      pBlock = (void**)malloc( sizeof( void* ) * newSize );
      memcpy( pBlock, array->dataBlock, sizeof( void* ) * copySize );
      if( newSize > array->size )
        {
          for( index = array->size;
               index < newSize;
               index++) 
            pBlock[ index ] = NULL;
        }
    }
  else pBlock = NULL;

  free( array->dataBlock );
  array->dataBlock = pBlock;
  array->size = newSize; 
}

/*
 *
 * __compact
 *
 *  This function removes all NULL valued entries from 
 *  an array.  Used as a precussor to sorting, and to act
 *  as garbage collection.
 */
static void __compact( pdarray array )
{
  int index, count;

  for( index = 0, count = 0; index < array->size; index++ )
    if( array->dataBlock[ index ] != NULL ) count++;

  if( count != array->size  && count > 0)
    {
      void **pBlock = (void**)malloc( sizeof( void* ) * count );
      int newIndex;

      for( newIndex = 0, index = 0; 
           index < array->size && newIndex < count;
           index++ )
        if( array->dataBlock[ index ] != NULL ) 
          pBlock[ newIndex++ ] = array->dataBlock[ index ];
      free( array->dataBlock );
      array->dataBlock = pBlock;
      array->size = count;
    }
  
}

/*
 *
 * newArray
 *
 *  Creates and returns a dynamic array object.  sort
 *  is a function with the prototype:
 * 	int comFunc(const void* a, const void* b)
 *  which is used to compare to objects during sorting,
 *  see the man page on qsort for info on the ordering of
 *  arguments.
 *
 *  search is also of type comFunc, but the first item will
 *  be an obejct of the type that the list is made up of,
 *  the second item will be the key you are searching on.
 *  (note: the implimentaions of these search functions may
 *   need to do typecasting.)
 *
 *  size is the initali size of the array
 * 
 *  step is the initial increment for automatically growing the array.
 *
 */
pdarray newArray(int size, int step, comFunc *sort, comFunc *search)
{
  int index;
  darray *narray = (pdarray)malloc( sizeof(darray) );

  narray->dataBlock = (void**)malloc( sizeof(void*) * size );
  narray->size = size;
  narray->step = step;
  narray->dirty = 0;
  narray->sortFunc = sort;
  narray->searchFunc = search;

  for( index = 0; index < size; index++ )
    {
      narray->dataBlock[ index ] = NULL;
    }

  return narray;
}

/*
 *
 * delArray
 *
 *   Frees all data associated with a dynamic array object.
 */
void delArray(pdarray array)
{
  if( array ) 
    {
      if( array->dataBlock ) 
	free( array->dataBlock );
      free( array );
    }
}

/*
 *
 * arrayGet
 *
 *  Gets an item out of a dynamic array at the given index.
 *  Useful for stepping through an array.
 */
void* arrayGet(pdarray array, int index)
{
  if( index < 0 || index >= array->size )
    {
      return NULL;
    }

  return array->dataBlock[ index ];
}

/*
 *
 * arraySet
 *
 *  Set's the value of a given index in the array.
 */
void* arraySet(pdarray array, int index, void *newVal)
{
  if( index < 0 || index >= array->size )
    {
      return NULL;
    }

  array->dirty = 1;
  return array->dataBlock[ index ] = newVal;
}

/*
 *
 * arrayAdd
 *
 *  Adds a value to the arary, resizing the array if necessary.
 */
void* arrayAdd(pdarray array, void* info)
{
  int index,origSize;

  array->dirty = 1;
  origSize = array->size;

  if( (array->size == 0 ) || array->dataBlock[ array->size - 1 ] != NULL ) 
    {
      __resize( array, array->size + array->step );
      return array->dataBlock[ origSize ] = info;
    }

  for( index = MAX( 0, array->size - array->step );
       index < array->size;
       index++ )
    {
      if( array->dataBlock[ index ] == NULL ) 
      {
        return array->dataBlock[ index ] = info; 
      }
    }
  __resize( array, array->size + array->step );
  fprintf(stderr,"DEBUG::::::RESIZE TWO\n");
  return array->dataBlock[ origSize ] = info;
}

/*
 *
 * arrayRemove
 *
 *  Remove's an item from the array.
 */

void* arrayRemove(pdarray array, void* info)
{
  int index;

  array->dirty = 1;
  
  for( index = 0;
       index < array->size;
       index++ )
    {
      if( array->dataBlock[ index ] == info )
        return array->dataBlock[ index ] = NULL;
    } 
  return info;
}

/*
 *
 * arrayLFindWith
 *
 *  Finds an item doing a linear, as opposed to a binary,
 *  search.  This allows searching in a list that hasn't
 *  been sorted yet, to avoid overhead of continuously sorting
 *  the list.  (I.e. adding an item, sorting it, looking for something,
 *  adding an item, sorting it, looking for something, etc... )
 *  Note: this variant on arrayLFind uses a user defined search function.
 */

void* arrayLFindWith(pdarray array, void* key, comFunc *search)
{
  int index;

  if( array->size > 0 )
    {
      for( index = 0; index < array->size; index++ )
        {
          if( array->dataBlock[ index ] )
            {
              if( search( &key, &( array->dataBlock[ index ] ) ) )
                continue;
              return array->dataBlock[ index ];
            }
        }
    }
  return NULL;
}

/*
 *
 * arrayLFind
 *
 *  Finds an item doing a linear, as opposed to a binary,
 *  search.  This allows searching in a list that hasn't
 *  been sorted yet, to avoid overhead of continuously sorting
 *  the list.  (I.e. adding an item, sorting it, looking for something,
 *  adding an item, sorting it, looking for something, etc... )
 */

void* arrayLFind(pdarray array, void* key)
{
  int index;

  if( array->size > 0 )
    {
      for( index = 0; index < array->size; index++ )
        {
          if( array->dataBlock[ index ] )
            {
              if( array->searchFunc( &key, &( array->dataBlock[ index ] ) ) )
                continue;
              return array->dataBlock[ index ];
            }
        }
    }
  return NULL;
}

/*
 *
 * arrayFindWith
 *
 *  Does a binary search on the array to find the given item
 *  based on the key using the given search function.
 */

void* arrayFindWith(pdarray array, void* key, comFunc *search)
{
  void** temp;

  if( array->size == 0 ) return NULL;
  if( array->dirty )
    arraySort(array);
  temp = (void**)bsearch( &key,
                          array->dataBlock,
                          array->size,
                          sizeof( void* ),
                          search );
  if( temp != NULL )
    return *temp;
  return NULL;
}

/*
 *
 * arrayFind
 *
 *  Does a binary search on the array to find the given item
 *  based on the key.
 */
void* arrayFind(pdarray array, void* key)
{
  void** temp;

  if( array->size == 0 ) return NULL;
  if( array->dirty )
    arraySort(array);
  temp = (void**)bsearch( &key,
                          array->dataBlock,
                          array->size,
                          sizeof( void* ),
                          array->searchFunc );
  if( temp != NULL )
    return *temp;
  return NULL;
}

/*
 *
 * arraySort
 *
 *  Sorts the given array.
 *
 */
void arraySort(pdarray array )
{
  __compact(array);
  if( array->size > 0 )
  {
    qsort( array->dataBlock, array->size, sizeof( void* ), 
           array->sortFunc );
  }
  array->dirty = 0;
}

/*
 *
 * arrayGetSize()
 *
 *  Get's the size of the given array.
 */
int arrayGetSize(pdarray array)
{
  return array->size;
}

/*
 *
 * arraySetSize
 *
 *  resets and resize's the given array.
 */
int arraySetSize(pdarray array, int newSize)
{
  __resize( array, newSize );
  return array->size;
}

/*
 *
 * arrayGetStep
 *
 *  gets the increment size of the array.
 */
int arrayGetStep(pdarray array)
{
  return array->step;
}

/*
 *
 * arraySetStep
 *
 *  Sets the incrementing size of the array.
 */

int arraySetStep(pdarray array, int newStep)
{
  return ( array->step = newStep ) ;
}
