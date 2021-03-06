/* ========================================================================= *
 * File: cached_file.hpp
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

#ifndef CACHED_FILE_HPP_USED
#define CACHED_FILE_HPP_USED

/* ========================================================================= *
* Includes.
* ========================================================================= */

#include <sys/time.h>
#include <stdlib.h>
#include <memnotify/definitions.hpp>

QT_BEGIN_HEADER
BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
 * Class CachedFile - file which could be re-loaded with pointed interval.
 * ========================================================================= */

/*!
  \class CachedFile
  \relates <memnotify/cached_file.hpp>
  Reloads in the cheapest way pointed file on sysfs/procfs which does not support inotify.
  Knows about refresh rate.
*/

class MEMNOTIFY_EXPORT CachedFile
{
  public:

    /* open pointed file to reloads and specified refresh interval in milliseconds */
    CachedFile(const char* sPath, const uint msUpdateInteval = 1000);
    ~CachedFile();

    /* load or reload file contents, returns true of success */
    bool load();

    /* validate file status and contents */
    bool valid() const;

    /* file attributes: path and handler */
    const char* path() const;
    int handler() const;

    const char* text() const;
    long       value() const;

    /* dumping internal information to stdout */
    void dump() const;

  private:

    char*          myPath;
    int            myHandler;
    char*          myText;
    struct timeval myActual;
    uint           myUpdate;

  private:

    bool readToMemory();
}; /* Class CachedFile */

/* ========================================================================= *
 * Inline methods for CachedFile.
 * ========================================================================= */

/* validate file status and contents */
inline bool CachedFile :: valid() const
{
  return (myPath && myHandler >= 0);
}

/* file attributes: path and handler */
inline const char* CachedFile :: path() const
{
  return myPath;
}

inline int CachedFile :: handler() const
{
  return myHandler;
}

inline const char* CachedFile :: text() const
{
  return myText;
}

inline long CachedFile :: value() const
{
  return (myText && *myText ? atol(myText) : 0);
}


END_MEMOTIFY_NAMESPACE
QT_END_HEADER

#endif /* CACHED_FILE_HPP_USED */
