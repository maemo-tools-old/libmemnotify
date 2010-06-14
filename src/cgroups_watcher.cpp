/* ========================================================================= *
 * File: cgroups_watcher.cpp
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
#include <sys/eventfd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memnotify/watcher.hpp>
#include <memnotify/watcher_builder.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
 * Class CgroupsWatcher: class declaration.
 * ========================================================================= */

class MEMNOTIFY_PRIVATE CgroupsWatcher: public Watcher
{
  public:

    CgroupsWatcher(const QSettings& theData, const QString& theName);
    virtual ~CgroupsWatcher();

    /* change the watcher activity and change watching to enabled, disabled */
    virtual bool enable();
    virtual bool disable();

    /* handle an external event when it is necessary, return true if event handled properly */
    virtual bool process();

    /* validate file status and contents */
    virtual bool valid() const;

    /* dumping internal information to stdout */
    virtual void dump() const;

  private:

    QString myControlPath;  /* path to $CGROUP/cgroup.event_control file */

}; /* CgroupsWatcher */

/* ========================================================================= *
* Class CgroupsWatcher: class implementation.
* ========================================================================= */

CgroupsWatcher :: CgroupsWatcher(const QSettings& theData, const QString& theName)
  : Watcher(theData, theName), myControlPath( option(theData,"control") )
{}

CgroupsWatcher :: ~CgroupsWatcher()
{
  if ( enabled() )
    disable();
}

/* change the watcher activity and change watching to enabled, disabled */
bool CgroupsWatcher :: enable()
{
  if (valid() && Watcher::enable() )
  {
    const int efd = eventfd(0, 0);

    if (efd >= 0)
    {
      /* Sensor file and event control are OK - write parameters to event_control file */
      char cfp[PATH_MAX];

      if ( Platform::defaultObject().path(myControlPath.toAscii().constData(), cfp, sizeof(cfp)) )
      {
        const int cfd = open(cfp, O_WRONLY);

        if ( cfd >= 0 )
        {
          const bool succ = (dprintf(cfd, "%d %d %u", efd, mySensor->handler(), (uint)myMemoryUsed) > 0);
          close(cfd);
          if ( succ )
          {
            myHandler = efd;
            return true;
          }
        }
#if MEMNOTIFY_DUMP
        else
        {
          printf ("Error: cannot open '%s' resolved as '%s'\n", myControlPath.toAscii().constData(), cfp);
        }
#endif
      }
#if MEMNOTIFY_DUMP
      else
      {
        printf ("Error: path for '%s' cannot be resolved as '%s'\n", myControlPath.toAscii().constData(), cfp);
      }
#endif

      close(efd);
    } /* if efd is OK */
  }

  return false;
} /* enable */

bool CgroupsWatcher :: disable()
{
  if (myHandler >= 0)
  {
    close(myHandler);
    myHandler = -1;
  }

  return Watcher::disable();
} /* disable */

/* handle an external event when it is necessary, return true if event handled properly */
bool CgroupsWatcher :: process()
{
  /* Should we do something? */
  if ( !enabled() )
    return false;

  /* prepare data for handling */
  char buf[BUFSIZ];
  ssize_t loaded = read(myHandler, buf, sizeof(buf));
  const quint64* cursor = (const quint64*)buf;
  uint handled = 0;

  while (loaded >= (ssize_t) sizeof(*cursor))
  {
#if MEMNOTIFY_DUMP
    printf ("=> %s: watcher %08x eventfd handler %d evendfd %016llx\n",
            __PRETTY_FUNCTION__, (uint)this, myHandler, *cursor
        );
#endif
    loaded -= sizeof(*cursor);
    cursor++;
    handled++;
  } /* while */

  /* Increase number of handled events */
  myEventsCounter += handled;

  /* Now if we had events - need to re-load sensor file */
  return (handled && updateState());
} /* process */

/* validate file status and contents */
bool CgroupsWatcher :: valid() const
{
  char tmp[PATH_MAX];
  if (Platform::defaultObject().path(myControlPath.toAscii().constData(), tmp, sizeof(tmp)) && Watcher::valid())
  {
    return (mySensor ? mySensor->valid() && myHandler >= 0 : myHandler < 0);
  }
  return false;
} /* valid */

void CgroupsWatcher :: dump() const
{
#if MEMNOTIFY_DUMP
  printf ("CgroupsWatcher %08x { ", (uint)this);
  Watcher::dump();
  printf ("control '%s'\n}\n", myControlPath.toAscii().constData());
#endif /* if MEMNOTIFY_DUMP */
} /* dump */


/* ========================================================================= *
 * The CgroupsWatcher registration as "cgroups" and "cgroups_control".
 * ========================================================================= */

ANNOUNCE_WATCHER(CgroupsWatcher, cgroups);
ANNOUNCE_WATCHER(CgroupsWatcher, cgroups_control);  /* Alias to be correct with documentation */

END_MEMOTIFY_NAMESPACE

/* ==================[ end of file memnotify/cgroups_watcher.cpp ]====================== */
