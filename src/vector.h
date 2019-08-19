/* Copyright [2017] [Comcast, Corp.]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __VECTOR_H_
#define __VECTOR_H_

#include <stdio.h>

typedef enum
{
    T2ERROR_SUCCESS,
    T2ERROR_FAILURE,
    T2ERROR_INVALID_PROFILE,
    T2ERROR_PROFILE_NOT_FOUND,
    T2ERROR_MAX_PROFILES_REACHED,
    T2ERROR_MEMALLOC_FAILED,
    T2ERROR_INVALID_ARGS,
    T2ERROR_INTERNAL_ERROR
}T2ERROR;

typedef struct _Vector
{
  void** data;
  size_t capacity;
  size_t count;
}Vector;


typedef void (*Vector_Cleanup)(void *);

T2ERROR Vector_Create(Vector** v);
T2ERROR Vector_Destroy(Vector* v, Vector_Cleanup destroyer);
T2ERROR Vector_PushBack(Vector* v, void* item);
T2ERROR Vector_RemoveItem(Vector* v, void* item, Vector_Cleanup destroyer);
void*   Vector_At(Vector* v, size_t index);
size_t  Vector_Size(Vector* v);

#endif
