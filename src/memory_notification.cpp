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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memnotify/memory_notification.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
* Class MemoryNotification.
* ========================================================================= */

static MemoryNotification*  MemoryNotification :: ourMemoryNotification = NULL;


MemoryNotification :: MemoryNotification()
  : myObservers(), myWatchers(), mySignalCounter(0), myEnabled(false)
{}

MemoryNotification :: ~MemoryNotification()
{
  if ( myEnabled )
    disable();

  /* Clean up watchers with memory removal */
  foreach (Watcher* watcher, myWatchers)
  {
    if (watcher)
      delete watcher;
  }
  myWatchers.clear();

  /* Clean up observers */
  myObservers.clear();

  if (ourMemoryNotification == this)
    ourMemoryNotification = NULL;
} /* ~MemoryNotification */

bool MemoryNotification :: setup(const char* pathSpecification)
{
  return false;
}

bool MemoryNotification :: poll()
{ return false; }

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
{ return false; }

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

void MemoryNotification :: dump()
{
#if MEMNOTIFY_DUMP
  printf ("MemoryNotification %08x {\n", (uint)this);

ZZZ
  printf ("}\n");
#endif /* if MEMNOTIFY_DUMP */
}

END_MEMOTIFY_NAMESPACE

/* =================[ end of file memnotify/memory_notification.cpp ]========================= */
