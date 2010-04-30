/* ========================================================================= *
 * File: platform.hpp
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

#ifndef PLATFORM_HPP_USED
#define PLATFORM_HPP_USED

/* ========================================================================= *
* Includes.
* ========================================================================= */

#include <QPair>
#include <QList>

#include <sys/sysinfo.h>
#include <time.h>

#include <memnotify/definitions.hpp>

QT_BEGIN_HEADER
BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
 * Classes.
 * ========================================================================= */


/*!
  \class Platform
  \relates <memnotify/platform.hpp>
  Handles platform-specific information like free/used memory, settings.
*/

class MEMNOTIFY_EXPORT Platform
{
  public:

    /* Units to report sizes of memory */
    typedef unsigned long Size;

    /* Representation of options passed in MEMNOTIFY_ENV_NAME environement variable. Expected be short (below 10 items) */
    typedef QPair<QString,QString> Option;
    typedef QList<Option>          Options;

    /* Class which handles system memory profile. Due to in future may have heavy operations now has actuality support */
    class SystemMemory
    {
      public:

        SystemMemory();
        ~SystemMemory();

        Size ram()   const;
        Size swap()  const;
        Size total() const;
        Size free();
        Size used();

        void dump() const;

      private:

        time_t         myActual;      /* timestamp when information was refreshed  */
        struct sysinfo mySysinfo;     /* system information, loaded on demand      */

      private:

        void refresh();
    }; /* Class SystemMemory */


  public:

    Platform();
    ~Platform();

    /* Information about system memory */
    SystemMemory& memory();

    /* Access to most often used folders */
    const char* mountPoint() const;

    /* Path to specific file - in default folder, home folder or some path */
    /* true if file is exists, false - not hanled or not exists            */
    bool path(const char* name, char* buffer, unsigned size) const;

    /* Options reading */
    const Options& options() const;
    const char*    option(const char* name) const;

    void dump() const;

  public:

    /* Constructs platform object on demand */
    static Platform& defaultObject();

  private:

    SystemMemory   myMemory;      /* Information about system memory                    */
    char*          myMountPoint;  /* mount point of system partition, /syspart          */
    Options        myOptions;     /* parsed options                                     */

  private:

    bool setMountPoint(const char*);
    bool parseOptions();

    static Platform* ourPlatform;
} /* Class Platform */

/* ========================================================================= *
 * Inline methods for Platform::SystemMemory.
 * ========================================================================= */

inline void Platform::SystemMemory :: refresh()
{
  const time_t now = time(NULL);

  if (now != myActual)
  {
    sysinfo(&mySysinfo);
    myActual = now;
  }
} /* refresh */


inline Platform::SystemMemory() : myActual(0)
{
  refresh();
}

inline Platform::~SystemMemory()
{}

inline Size Platform::SystemMemory :: ram() const
{
  return (mySysinfo.totalram + mySysinfo.totalhigh) * mySysinfo.mem_unit;
}

inline Size Platform::SystemMemory :: swap() const
{
  return mySysinfo.totalswap * mySysinfo.mem_unit;
}

inline Size Platform::SystemMemory :: total() const
{
  return ram() + swap();
}

inline Size Platform::SystemMemory :: free()
{
  refresh();
  return (mySysinfo.freeram + mySysinfo.bufferram + mySysinfo.freeswap + mySysinfo.freehigh) * mySysinfo.mem_unit;
}

inline Size Platform::SystemMemory :: used()
{
  return (total() - free());
}

/* ========================================================================= *
* Inline methods for Platform
* ========================================================================= */

inline Platform::SystemMemory& Platform :: memory()
{
  return myMemory;
}

inline const char* Platform :: mountPoint() const
{
  return myMountPoint;
}

inline const Platform::Options& Platform :: options() const
{
  return myOptions;
}

inline const char* Platform :: option(const char* name) const
{
  if (name && *name && myOptions.size() > 0)
  {
    const index = myOptions.indexOf( Option(name,QString()) );
    if (index >= 0)
      return myOptions.at(index).second.toAscii().constData();
  }

  /* Search failed by some reason */
  return NULL;
}

inline Platform& Platform :: defaultObject()
{
  return *(ourPlatform ? ourPlatform : (ourPlatform = new Platform()));
}


END_MEMOTIFY_NAMESPACE
QT_END_HEADER

#endif /* PLATFORM_HPP_USED */
