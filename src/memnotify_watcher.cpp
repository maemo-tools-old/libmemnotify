/* ========================================================================= *
 * File: memnotify_watcher.cpp
 *
 * This file is part of Memory Notification Library (libmemnotifyqt)
 *
 * Copyright (C) 2012 Nokia Corporation. All rights reserved.
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
#include <memnotify/watcher.hpp>
#include <memnotify/watcher_builder.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
 * Class MemNotifyWatcher: class declaration for tracking /dev/memnotify.
 * ========================================================================= */

class MEMNOTIFY_PRIVATE MemNotifyWatcher: public Watcher
{
  public:

    MemNotifyWatcher(const QSettings& theData, const QString& theName);
    virtual ~MemNotifyWatcher();

    /* change the watcher activity and change watching to enabled, disabled */
    virtual bool enable();
    virtual bool disable();

    /* handle an external event when it is necessary, return true if event handled properly */
    virtual bool process();

    /* validate file status and contents */
    virtual bool valid() const;

    /* dumping internal information to stdout */
    virtual void dump() const;

  protected:

    virtual bool updateState();

  protected:

    const QString myProperty; /* the tracked kind of memory comes from "property" option */

}; /* MemNotifyWatcher */

/* ========================================================================= *
* Class MemNotifyWatcher: class implementation.
* ========================================================================= */

MemNotifyWatcher :: MemNotifyWatcher(const QSettings& theData, const QString& theName)
  : Watcher(theData, theName), myProperty(option(theData, "property"))
{}

MemNotifyWatcher :: ~MemNotifyWatcher()
{
  if ( enabled() )
    disable();
}

/* change the watcher activity and change watching to enabled, disabled */
bool MemNotifyWatcher :: enable()
{
  if (valid() && Watcher::enable())
  {
    char buf[256];

    myHandler = mySensor->handler(); /* Sensor should detect writable /dev/memnotify */
    const ssize_t len = snprintf(buf, sizeof(buf), "%s %lu", myProperty.toAscii().constData(), ulong(memoryUsed() / getpagesize()));
    return (len > 0 && len == write(myHandler, buf, len));
  }

  return false;
} /* enable */

bool MemNotifyWatcher :: disable()
{
  myHandler = -1;
  return Watcher::disable();
} /* disable */


/* handle an external event when it is necessary, return true if event handled properly */
bool MemNotifyWatcher :: process()
{
  /* Should we do something? */
  if (enabled() && updateState())
  {
    myEventsCounter++;
    return true;
  }

  return true;
} /* process */

/* validate file status and contents */
bool MemNotifyWatcher :: valid() const
{
  return (Watcher::valid() && (mySensor ? mySensor->valid() : myHandler < 0));
} /* valid */


bool MemNotifyWatcher :: updateState()
{
  if (mySensor && mySensor->load())
  {
    const char* found = strstr(mySensor->text(), myProperty.toAscii().constData());
    if ( found )
    {
      const Size memoryMeter = (Size)strtoul(found + myProperty.length() + 1, NULL, 10);

      /* Check that data from sensor is parsed correct */
      if (memoryMeter > 0)
      {
        myState = (memoryMeter * getpagesize() >= myMemoryUsed);
        return true;
      }
    }
  }
  return false;
} /* updateState */



void MemNotifyWatcher :: dump() const
{
#if MEMNOTIFY_DUMP
  printf ("MemNotifyWatcher %p { ", this);
  Watcher::dump();
  printf ("tracking for property %s\n", myProperty.toAscii().constData());
  printf ("}\n");
#endif /* if MEMNOTIFY_DUMP */
} /* dump */


/* ========================================================================= *
 * The MemNotifyWatcher registration as "memnotify".
 * ========================================================================= */

ANNOUNCE_WATCHER(MemNotifyWatcher, memnotify);

END_MEMOTIFY_NAMESPACE

/* ==================[ end of file memnotify/memnotify_watcher.cpp ]====================== */
