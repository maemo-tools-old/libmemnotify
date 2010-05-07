/* ========================================================================= *
 * File: debug_watcher.cpp
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
#include <sys/inotify.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memnotify/watcher.hpp>
#include <memnotify/watcher_builder.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
 * Class DebugWatcher: class declaration.
 * ========================================================================= */

class MEMNOTIFY_PRIVATE DebugWatcher: public Watcher
{
  public:

    DebugWatcher(const QSettings& theData, const QString& theName);
    virtual ~DebugWatcher();

    /* change the watcher activity and change watching to enabled, disabled */
    virtual bool enable();
    virtual bool disable();

    /* handle an external event when it is necessary, return true if event handled properly */
    virtual bool process();

    /* validate file status and contents */
    virtual bool valid() const;

  private:

    int myWatcher;

}; /* DebugWatcher */

/* ========================================================================= *
* Class DebugWatcher: class implementation.
* ========================================================================= */

DebugWatcher :: DebugWatcher(const QSettings& theData, const QString& theName)
  : Watcher(theData, theName), myWatcher(-1)
{}

DebugWatcher :: ~DebugWatcher()
{
  if ( enabled() )
    disable();
}

/* change the watcher activity and change watching to enabled, disabled */
bool DebugWatcher :: enable()
{
  if ( Watcher::enable() )
  {
    const int ifd = inotify_init();

    if (ifd >= 0)
    {
      const int wfd = inotify_add_watch(ifd, mySensor->path(), IN_ATTRIB|IN_CLOSE_WRITE);
      if (wfd >= 0)
      {
        /* seems watch was opened correct */
        myHandler = ifd;
        myWatcher = wfd;
        return true;
      }
      else
      {
        close(ifd);
      }
    }
  }

  return false;
} /* enable */

bool DebugWatcher :: disable()
{
  if (myHandler >= 0)
  {
    close(myHandler);
    myHandler = -1;
    myWatcher = -1;
  }

  return Watcher::disable();
} /* disable */

/* handle an external event when it is necessary, return true if event handled properly */
bool DebugWatcher :: process()
{
  /* Should we do something? */
  if ( !enabled() )
    return false;

  /* prepare data for handling */
  char buf[BUFSIZ];
  ssize_t loaded = read(myHandle, buf, sizeof(buf));
  const unsigned char* cursor = (const unsigned char*)buf;
  unsigned handled = 0;

  while (loaded >= sizeof(struct inotify_event))
  {
    const struct inotify_event* ep = (const struct inotify_event*)cursor;
    const ssize_t es = sizeof(*ep) + ep->len;

    if ( es >= loaded )
    {
#if MEMNOTIFY_DUMP
      const char* name = (ep->len ? ep->name : "");
      printf ("=> %s: inotify event for %d { wd %d mask %u cookie %u name '%s'}\n",
              __PRETTY_FUNCTION__, myHandler, ep->wd, ep->mask, ep->cookie, name
          );
#endif
      cursor += es;
      loaded -= es;
      handled ++;
    }
    else
      break;
  } /* while */

  /* Now if we had events - need to re-load sensor file */
  if (handled && mySensor->load())
  {
    const Size memoryMeter = (const Size)mySensor->value();

    /* Check that data from sensor is parsed correct */
    if (memoryMeter > 0)
    {
      myState = (memoryMeter > myMemoryUsed);
      return true;
    }
  }

  return false;
} /* process */

/* validate file status and contents */
bool DebugWatcher :: valid() const
{
  if ( Watcher::valid() )
  {
    return (mySensor ? mySensor->valid() && myHandler >= 0 : myHandler < 0);
  }
  return false;
} /* valid */

/* ========================================================================= *
 * The DebugWatcher registration.
 * ========================================================================= */

ANNOUNCE_WATCHER("debug",DebugWather);

END_MEMOTIFY_NAMESPACE

/* ==================[ end of file memnotify/debug_watcher.cpp ]====================== */
