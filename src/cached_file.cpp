/* ========================================================================= *
 * File: cached_file.cpp
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

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memnotify/platform.hpp>
#include <memnotify/cached_file.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
* Class CachedFile.
* ========================================================================= */

CachedFile :: CachedFile(const char* sPath, const unsigned msUpdateInteval)
  : myPath(NULL), myHandler(-1), myText(NULL), myActual(0,0), myUpdate(msUpdateInteval * 1000)
{
  char buf[PATH_MAX];

  if ( Platform::defaultObject().path(sPath, buf, sizeof(buf)) )
  {
    myPath    = strdup(buf);
    myHanlder = open(myPath, O_RDONLY|O_DIRECT|O_NOATIME);
  }
} /* CachedFile */

CachedFile :: ~CachedFile()
{
  if (myHander >= 0)
    close(myHandler);

  if (myText)
    free(myText);

  if (myPath)
    free(myPath);
} /* ~CachedFile */


bool CachedFile :: readToMemory()
{
  /* Check the file handler */
  if (myHandler < 0)
    return false;

  /* Indicating transaction started */
  if (myText)
  {
    free(myText);
    myText = NULL;
  }

  /* Can we seek to the beginning of file? */
  if (myActual.tv_sec && lseek(myHandler, 0, SEEK_SET))
    return false;

  /* Now read the contents */
  char          buffer[BUFSIZ];
  const ssize_t loaded = read(myHandler, buffer, sizeof(buffer));

  if (loaded < 0)
    return false;

  /* allocate current buffer with specified size */
  char* space = (char*)malloc(loaded + 1);
  if ( !space )
    return false;

  /* We are succeed */
  memcpy(space, buffer, loaded);
  space[loaded] = 0;
  myText = space;

  return true;
} /* readToMemory */

void CachedFile :: dump() const
{
#if MEMNOTIFY_DUMP
  printf ("CachedFile %08x: path '%s' hander %d actual %u.%06u update %u text '%s'\n",
          (unsigned)this, myPath, myHander, myActual.tv_sec, myActual.tv_usec, myUpdate, myText
    );
#endif /* if MEMNOTIFY_DUMP */
}

END_MEMOTIFY_NAMESPACE

/* ====================[ end of file memnotify/cached_file.cpp ]=========================== */
