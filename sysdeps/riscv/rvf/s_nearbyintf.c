/* Copyright (C) 2017 Free Software Foundation, Inc.
   Contributed by Andrew Waterman (andrew@sifive.com).
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <math.h>
#include <math_private.h>

float
__nearbyintf (float x)
{
  int flags = riscv_getflags ();
  int nan = isnanf (x);
  float mag = fabsf (x);

  if (nan)
    return x + x;

  if (mag < (1 << __FLT_MANT_DIG__))
    {
      int i;
      float new_x;

      asm volatile ("fcvt.w.s %0, %1" : "=r" (i) : "f" (x));
      asm volatile ("fcvt.s.w %0, %1" : "=f" (new_x) : "r" (i));

      /* nearbyint(-0) == -0, and in general we'll always have the same
	 sign as our input.  */
      x = copysignf (new_x, x);

      riscv_setflags (flags);
    }

  return x;
}

weak_alias (__nearbyintf, nearbyintf)
