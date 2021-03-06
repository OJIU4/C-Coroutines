/***************************************************************************
*
* Copyright 2017 by Sean Conner.
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
* License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, see <http://www.gnu.org/licenses/>.
*
* Comments, questions and criticisms can be sent to: sean@conman.org
*
*************************************************************************/

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "coroutine.h"

/**************************************************************************/

static uintptr_t foo(coroutine__s *self,uintptr_t d)
{
  d = coroutine_yield(self,d);
  return d;
}

static uintptr_t routine1(coroutine__s *self,uintptr_t d)
{
  while(true)
  {
    printf("routine1=%" PRIuPTR "\n",d);
    d = foo(self,d + 100);
  }
  
  return 0;
}

/**************************************************************************/

static uintptr_t routine2(coroutine__s *self,uintptr_t d)
{
  while(true)
  {
    printf("routine2=%" PRIuPTR "\n",d);
    d = coroutine_yield(self,d + 1);
  }
  
  return 0;
}

/**************************************************************************/

static uintptr_t routine3(coroutine__s *self,uintptr_t d)
{
  while(true)
  {
    printf("routine3=%" PRIuPTR "\n",d);
    d = coroutine_yield(self,d + 2);
  }
  
  return 0;
}

/**************************************************************************/

static uintptr_t test(coroutine__s *self,uintptr_t d)
{
  printf("Hello %" PRIuPTR "\n",d);
  d = coroutine_yield(self,d + 10);
  printf("Goodbye %" PRIuPTR "\n",d);
  return d + 10;
}

/**************************************************************************/

int main(void)
{
  if (true)
  {
    coroutine__s *co;
    uintptr_t    r = 0;
    
    coroutine_create(&co,0,test);
    
    for (int i = 0 ; i < 10 ; i++)
    {
      r = coroutine_yield(co,r);
      printf("test-%d=%" PRIuPTR "\n",i,r);
    }

    coroutine_free(co);
  }
  
  printf("---------\n");
  
  if (true)
  {
    coroutine__s *co1;
    coroutine__s *co2;
    coroutine__s *co3;
    
    coroutine_create(&co1,0,routine1);
    coroutine_create(&co2,0,routine2);
    coroutine_create(&co3,0,routine3);
    
    for (int i = 0 , r = 0 ; i < 10 ; i++)
    {
      r = coroutine_yield(co1,r);
      r = coroutine_yield(co2,r + 200);
      r = coroutine_yield(co3,r + 300);
    }
    
    coroutine_free(co3);
    coroutine_free(co2);
    coroutine_free(co1);
  }
  
  return 0;
}

/**************************************************************************/
