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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <memnotify/platform.hpp>
#include <memnotify/cached_file.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
* Class CachedFile.
* ========================================================================= */

CachedFile :: CachedFile(const char* sPath, const uint msUpdateInteval)
  : myPath(NULL), myHandler(-1), myText(NULL), myUpdate(msUpdateInteval * 1000)
{
  char buf[PATH_MAX];

  memset(&myActual, 0, sizeof(myActual));
  if ( Platform::defaultObject().path(sPath, buf, sizeof(buf)) )
  {
    /* opening mode will be detected according to permissions, read expected */
    const int mode = (0 == access(buf, R_OK|W_OK) ? O_RDWR : O_RDONLY) | O_CLOEXEC;

    myHandler = open(buf, mode);
    myPath    = strdup(buf);
  }
} /* CachedFile */

CachedFile :: ~CachedFile()
{
  if (myHandler >= 0)
    close(myHandler);

  if (myText)
    free(myText);

  if (myPath)
    free(myPath);
} /* ~CachedFile */

/* load or reload file contents, returns true of success */
bool CachedFile :: load()
{
  /* Should we load always or check update interval? */
  if (myUpdate)
  {
    struct timeval now;

    if ( gettimeofday(&now, NULL) )
    {
      /* failed to load time - read to memory */
      return readToMemory();
    }

    if (!myActual.tv_sec || uint((now.tv_sec - myActual.tv_sec) * 1000000 + now.tv_usec - myActual.tv_usec) >= myUpdate)
    {
      if ( !readToMemory() )
        return false;
      myActual = now;
    }

    /* file is still actual */
    return true;
  }
  else
  {
    return readToMemory();
  }
} /* load */

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
  printf ("CachedFile %p: path '%s' hander %d actual %ld.%06lu update %u text '%s'\n",
          this, myPath, myHandler, myActual.tv_sec, myActual.tv_usec, myUpdate, myText
    );
#endif /* if MEMNOTIFY_DUMP */
}

END_MEMOTIFY_NAMESPACE

/* ====================[ end of file memnotify/cached_file.cpp ]=========================== */
