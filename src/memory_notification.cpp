/* ========================================================================= *
 * File: memory_notification.cpp
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
#include <aio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <QSettings>
#include <memnotify/watcher_builder.hpp>
#include <memnotify/memory_notification.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
* Class MemoryNotification.
* ========================================================================= */

MemoryNotification*  MemoryNotification :: ourMemoryNotification = NULL;

bool MemoryNotification :: setup(const char* pathSpecification)
{
  /* We should disable functionality if it is enabled */
  if (myEnabled && !disable())
    return false;

  /* If no files pointed the default file should be loaded */
  if ( !(pathSpecification && *pathSpecification) )
    pathSpecification = "default";

  /* if we have some watchers in list - they must be removed */
  clearWatchers();

  const QStringList files = QString(pathSpecification).split(":", QString::SkipEmptyParts);

  foreach(QString file, files)
  {
    const QSettings   config(file, QSettings::IniFormat);
    /* array with names of thresholds */
    const QStringList tholds(config.childGroups());

    /* Now trying to build the watchers, one by one */
    foreach(QString thold, tholds)
    {
      Watcher* newcomer = WatcherBuilder::build(config, thold);
      Q_ASSERT(NULL != newcomer);
#if MEMNOTIFY_DUMP
      Q_ASSERT(true == newcomer->valid());
#endif
      if ( newcomer )
        myWatchers.append(newcomer);
    } /* for each thold */
  } /* for each file */

  /* As a result we must have at least one watcher created */
  return !myWatchers.isEmpty();
} /* setup */

bool MemoryNotification :: poll()
{
  /* we should be enabled first */
  if (!myEnabled )
    return false;

  /* that is strange but possible */
  if ( myPoller )
  {
    if ( myPoller->isRunning() )
      return true;
    /* poller needs to be re-created */
    myPoller->terminate();
    delete myPoller;
    myPoller = NULL;
  }

  /* collect the handlers */
  int  handlers[ myWatchers.count() ];
  const uint counter = query(handlers, myWatchers.count());

  if ( !counter )
    return false;

  /* Now construct the poller object */
  myPoller = new Poller(this, handlers, counter);
  return myPoller->isRunning();
} /* poll */

uint MemoryNotification :: query(int* fds, uint size)
{
  uint counter = 0;
  if (myEnabled && fds && myWatchers.count() <= (int)size)
  {
    foreach(const Watcher* watcher, myWatchers)
    {
      const int handler = (watcher ? watcher->handler() : -1);
      if ( handler < 0 )
        return 0;
      fds[counter++] = handler;
    }
  }

  return counter;
} /* query */

bool MemoryNotification :: process(const int* fds, uint counter)
{
  /* we should be enabled */
  if ( !myEnabled )
    return false;

  /* Check the parameters */
  if ( !fds )
    return false;

  if ( !counter )
    return true;

  /* result of processing, should be true if all watchers handled OK */
  bool result = true;
  QList<Watcher*> updated;

  /* And now handling the all incoming events */
  for (uint ifd = 0; ifd < counter; ifd++)
  {
    const int fd = fds[ifd];

    /* First - we should find the watcher */
    foreach(Watcher* watcher, myWatchers)
    {
      Q_ASSERT(NULL != watcher);
      if (watcher && fd == watcher->handler())
      {
        const bool oldState = watcher->state();
        if ( watcher->process() )
        {
          if (oldState != watcher->state())
            updated.append(watcher);
        }
        else
        {
          /* we are failed with processing but we must continue until the end */
          result = false;
        }
      }
    } /* scan for watcher */
  } /* for each file descriptor */

  /* Now we have list of updated watchers -> sending the notifications */
  foreach(const Watcher* watcher, updated)
  {
    Q_ASSERT(NULL != watcher);
    const QString& name  = watcher->name();
    const bool     state = watcher->state();

    /* if necessary - notify callbacks */
    if (myObservers.count() > 0)
    {
      foreach(OBSERVER observer, myObservers)
      {
        Q_ASSERT(NULL != observer);
        if ( observer )
          observer(name, state);
      }
    }

    /* if necessary - notify subscribers */
    if (mySignalCounter > 0)
      emit notified(name, state);
  } /* for each updated */

  return result;
} /* process */

bool MemoryNotification :: enable()
{
  if ( myEnabled )
    return false;

  if (0 == mySignalCounter && myObservers.isEmpty())
    return false;

  if ( myWatchers.isEmpty() )
    return false;

  /* disable watchers one by one */
  int enabled_counter = 0;
  foreach(Watcher* watcher, myWatchers)
  {
    if (watcher && watcher->enable())
    {
      /* item successfully enabled */
      enabled_counter++;
    }
  }

  /* set common enabled flag on only if we have all items enabled */
  myEnabled = (myWatchers.count() == enabled_counter);

  /* function done if no items enabled */
  return (myEnabled);
} /* enable */

bool MemoryNotification :: disable()
{
  if ( !myEnabled )
    return false;

  /* disable watchers one by one */
  int disabled_counter = 0;
  foreach(Watcher* watcher, myWatchers)
  {
    if (watcher && watcher->disable())
    {
      /* item successfully disabled */
      disabled_counter++;
    }
  }

  /* if poller is running - remove it. Likely thread will finished already */
  if ( myPoller )
  {
    myPoller->terminate();
    delete myPoller;
    myPoller = NULL;
  }

  /* set common enabled flag off only if we have all items disabled */
  myEnabled = (myWatchers.count() != disabled_counter);

  /* function done if no items enabled */
  return (! myEnabled);
} /* disable */

bool MemoryNotification :: valid() const
{
  /* first, we must have subscribed items */
  if (0 == mySignalCounter && myObservers.isEmpty())
    return false;

  /* we must have sources for signal generation */
  if ( myWatchers.isEmpty() )
    return false;

  /* All observers must be non-null */
  if ( myObservers.indexOf(NULL) >= 0 )
    return false;

  /* All watchers must be non-null, valid and maybe enabled */
  foreach(const Watcher* watcher, myWatchers)
  {
    if (watcher && watcher->valid() && watcher->enabled() == myEnabled)
    {
      /* check the next item */
      continue;
    }
    return false;
  }

  return true;
} /* valid */

void MemoryNotification :: clearWatchers()
{
  if (myWatchers.count() > 0)
  {
    foreach (Watcher* watcher, myWatchers)
    {
      if (watcher)
        delete watcher;
    }
    myWatchers.clear();
  }
} /* clearWatchers */

void MemoryNotification :: dump() const
{
#if MEMNOTIFY_DUMP
  printf ("MemoryNotification %08x {\n", (uint)this);
  printf ("  outMemoryNotification %08x signal counter %u enabled %d polling thread %08x\n",
          (uint)ourMemoryNotification, mySignalCounter, myEnabled, (uint)myPoller
      );

  if ( myPoller )
    printf ("  polling is running: %d\n", myPoller->isRunning());

  printf ("  observers %d {\n", myObservers.count());
  foreach (OBSERVER observer, myObservers)
  {
    printf ("\t%08x\n", (uint)observer);
  }
  printf ("  }\n");

  printf ("  watchers %d {\n", myWatchers.count());
  foreach (const Watcher* watcher, myWatchers)
  {
    printf ("\t%08x ", (uint)watcher);
    if (watcher)
      watcher->dump();
    else
      printf ("\n");
  }
  printf ("  }\n");


  printf ("}\n");
#endif /* if MEMNOTIFY_DUMP */
}

END_MEMOTIFY_NAMESPACE

/* =================[ end of file memnotify/memory_notification.cpp ]========================= */
