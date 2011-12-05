/* ========================================================================= *
 * File: umm_watcher.cpp
 *
 * This file is part of Memory Notification Library (libmemnotifyqt)
 *
 * Copyright (C) 2011 Nokia Corporation. All rights reserved.
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
 * Class UmmWatcher: class declaration.
 * ========================================================================= */

class MEMNOTIFY_PRIVATE UmmWatcher: public Watcher
{
  public:

    UmmWatcher(const QSettings& theData, const QString& theName);
    virtual ~UmmWatcher();

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

}; /* UmmWatcher */

/* ========================================================================= *
* Class UmmWatcher: class implementation.
* ========================================================================= */

UmmWatcher :: UmmWatcher(const QSettings& theData, const QString& theName)
  : Watcher(theData, theName)
{}

UmmWatcher :: ~UmmWatcher()
{
  if ( enabled() )
    disable();
}

/* change the watcher activity and change watching to enabled, disabled */
bool UmmWatcher :: enable()
{
  if (valid() && Watcher::enable())
  {
    myHandler = open(mySensor->path(), O_WRONLY);
    if (myHandler > 0)
    {
      char buf[32];
      const ssize_t len = snprintf(buf, sizeof(buf), "%lu", ulong(memoryUsed() / getpagesize()));
      return (len > 0 && len == write(myHandler, buf, len));
    }
  }

  return false;
} /* enable */

bool UmmWatcher :: disable()
{
  if (myHandler > 0)
  {
    close(myHandler);
    myHandler = -1;
  }
  return Watcher::disable();
} /* disable */


/* handle an external event when it is necessary, return true if event handled properly */
bool UmmWatcher :: process()
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
bool UmmWatcher :: valid() const
{
  return (Watcher::valid() && (mySensor ? mySensor->valid() : myHandler < 0));
} /* valid */


bool UmmWatcher :: updateState()
{
  if (mySensor && mySensor->load())
  {
    const Size memoryMeter = (const Size)mySensor->value();

    /* Check that data from sensor is parsed correct */
    if (memoryMeter > 0)
    {
      myState = (myMemoryUsed / getpagesize() < memoryMeter);
      return true;
    }
  }
  return false;
} /* updateState */



void UmmWatcher :: dump() const
{
#if MEMNOTIFY_DUMP
  printf ("UmmWatcher %p { ", this);
  Watcher::dump();
  printf ("}\n");
#endif /* if MEMNOTIFY_DUMP */
} /* dump */


/* ========================================================================= *
 * The UmmWatcher registration as "debug".
 * ========================================================================= */

ANNOUNCE_WATCHER(UmmWatcher, umm);
ANNOUNCE_WATCHER(UmmWatcher, used_memory_meter);

END_MEMOTIFY_NAMESPACE

/* ==================[ end of file memnotify/umm_watcher.cpp ]====================== */
