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

static MemoryNotification*  MemoryNotification :: ourMemoryNotification = NULL;


bool MemoryNotification :: setup(const char* pathSpecification)
{
  const bool wasEnabled = myEnabled;

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
    /* array with data to create watchers */
    const QStringList data(config.allKeys());

    /* Now trying to build the watchers, one by one */
    foreach(QString thold, tholds)
    {
      Watcher* newcomer = WatcherBuilder::build(data, thold);
      Q_ASSERT(NULL != newcomer);
#if MEMNOTIFY_DUMP
      Q_ASSERT(true == newcomer->valid());
#endif
      if ( newcomer )
        myWatchers.append(newcomer);
    } /* for each thold */
  } /* for each file */

  /* As a result we must have at least one watcher created */
  if ( myWatchers.isEmpty() )
    return false;

  /* We should enable functionality back if it was enabled */
  return (!wasEnabled || enable());
} /* setup */

bool MemoryNotification :: poll()
{
  /* FIXME: QThread-based solution for epoll expected. AIO doesn't work as necessary */
  return false;
} /* poll */

uint MemoryNotification :: query(int* fds, uint size)
{
  uint index = 0;
  if (myEnabled && fd && myWatchers->count() <= (int)size)
  {
    foreach(const Watcher* watcher, myWatchers)
    {
      const int handler = (watcher ? watcher->handler() : -1);
      if ( handler < 0 )
        return 0;
      fds[index++] = handler;
    }
  }

  return index;
} /* query */

bool MemoryNotification :: process(const int* fds, uint counter)
{
  /* we should be enabled */
  if ( !myEnable )
    return false;

  /* Check the parameters */
  if ( !fd )
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
    foreach(Watcher* cursor, myWatchers)
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
  myEnabled = (myWatchers->count() == enabled_counter);

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

  /* set common enabled flag off only if we have all items disabled */
  myEnabled = (myWatchers->count() != disabled_counter);

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

void MemoryNotification :: dump()
{
#if MEMNOTIFY_DUMP
  printf ("MemoryNotification %08x {\n", (uint)this);
  printf ("  outMemoryNotification %08x signal counter %u enabled %d poll thread %08x\n",
          (uint)ourMemoryNotification, mySignalCounter, myEnabled, (uint)myThread
      );

  printf ("  observers %d {\n", myObservers.count());
  for (const OBSERVER observer, myObservers)
  {
    printf ("\t%08x\n", (uint)observer);
  }
  printf ("  }\n");

  printf ("  watchers %d {\n", myWatchers.count());
  for (const Watchers watcher, myWatchers)
  {
    printf ("\t%08x ", (uint)watcher);
    if (watcher)
      watcher->dump();
    else
      printf ("\n");
  }
  printf ("  }\n");

  if ( myThread )
    ((const PollThread*)myThread)->dump();

  printf ("}\n");
#endif /* if MEMNOTIFY_DUMP */
}

END_MEMOTIFY_NAMESPACE

/* =================[ end of file memnotify/memory_notification.cpp ]========================= */
