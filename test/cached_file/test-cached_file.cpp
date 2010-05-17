/* ========================================================================= *
 * File: test-platform.cpp
 *
 * This file is part of Memory Notification Library (libmemnotifyqt)
 *
 * Copyright (C) 2010 Nokia Corporation. All rights reserved.
 *
 * Contacts: Leonid Moiseichuk <leonid.moiseichuk@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * ========================================================================= */

/* ========================================================================= *
* Includes.
* ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <memnotify/cached_file.hpp>


/* ========================================================================= *
 * Very simple test application
 * ========================================================================= */

int main(const int argc, const char* argv[])
{
  MEMNOTIFY::CachedFile cf("/proc/stat", 1000);

  cf.dump();
  printf ("cf.valid = %d\n", cf.valid());

  for (int index = 0; index < 3; index++)
  {
    if ( cf.load() )
      printf ("=> load: SUCCESS\n");
    else
      printf ("=> load: FAILURE\n");

    cf.dump();
    usleep(1000);
  }

  return 0;
} /* main */

/* =================[ end of file memnotify/test-platform.cpp ]====================== */
