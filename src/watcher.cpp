/* ========================================================================= *
 * File: watcher.cpp
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
#include <memnotify/watcher.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
* Class Watcher.
* ========================================================================= */

Watcher :: Watcher(const QSettings& theData, const QString& theName)
  : myName(theName), myType(option(theData, "type")), mySensorPath(option(theData, "sensor")),
    myMemoryFree(memoryOption(theData, "free")), myMemoryUsed(memoryOption(theData, "used")), myMemoryLimit(memoryOption(theData, "limit")),
    myHandler(-1), mySensor(NULL), myState(false), myEventsCounter(0)
{
  uint max_memory_usage = 100;

  /* Now evaluating memory limits */
  if ( myMemoryLimit )
  {
    if ( percents(myMemoryLimit) )
    {
      /* percents should be set relatively to system memory usage*/
      myMemoryLimit = (myMemoryLimit * Platform::defaultObject().memory().total()) / 100;
    }
  }
  else
  {
    myMemoryLimit = Platform::defaultObject().memory().total();
    max_memory_usage = 95;  /* Kernel OOM allows us to use only 97% of memory, so let's keep 2% below killing */
  }

  /* Then deside how we should set theshold - based on Free or Used level. Used is more natural */
  if ( myMemoryUsed )
  {
    if ( percents(myMemoryUsed) )
    {
      /* percents should be set relatively to overall memory limit */
      myMemoryUsed = (myMemoryUsed * myMemoryLimit) / 100;
    }

    /* The memory limits should be sane */
    if (myMemoryUsed < myMemoryLimit)
    {
      myMemoryFree = myMemoryLimit - myMemoryUsed;
    }
    else
    {
      /* that is will be visible later through valid() */
      myMemoryUsed = myMemoryFree = 0;
    }
  }
  else if ( myMemoryFree )
  {
    if ( percents(myMemoryFree) )
    {
      /* percents should be set relatively to overall memory limit */
      myMemoryFree = (myMemoryFree * myMemoryLimit) / 100;
    }

    /* The memory limits should be sane */
    if (myMemoryFree < myMemoryLimit)
    {
      myMemoryUsed = myMemoryLimit - myMemoryFree;
    }
    else
    {
      /* that is will be visible later through valid() */
      myMemoryUsed = myMemoryFree = 0;
    }
  }
  else
  {
    myMemoryUsed = (max_memory_usage * myMemoryLimit) / 100;
    myMemoryFree = myMemoryLimit - myMemoryUsed;
  }
} /* Watcher */

Watcher :: ~Watcher()
{
    if ( enabled() )
      disable();

    if ( mySensor )
      delete mySensor;
}

Size Watcher :: memoryOption(const QSettings& theData, const char* theKey);
{
  QString opt = option(theData, theKey);

  if ( opt.isEmpty() )
    return 0;

  /* Is that value? */
  bool   ok = false;
  ulong val = opt.toULong(&ok);
  if ( !ok )
  {
    /* Might be a file */
    CachedFile f(opt.toAscii().constData(), 0);

    if ( f.load() )
    {
      opt = f.text();
      val = opt.toULong(&ok);
    }
  }

  /* Now we have some value and maybe multiplier in opt */
  if ( ok )
  {
    if ( opt.endsWith('k', Qt::CaseInsensitive) )
      val <<= 10;
    else if ( opt.endsWith('m', Qt::CaseInsensitive) )
      val <<= 20;
    else if ( opt.endsWith('g', Qt::CaseInsensitive) )
      val <<= 30;
  }
  else
  {
    /* unable to parse option, set to 0 */
    val = 0;
  }

  return Size(val);
} /* memoryOption */

void Watcher :: dump() const
{
#if MEMNOTIFY_DUMP
    printf ("Watcher %08x: name '%s' type '%s' sensor '%s' free %u used %u total %u handler %d state %d events %u   ",
            (uint)this, myName.toAscii().constData(), myType.toAscii().constData(), mySensorPath.toAscii().constData(),
            myMemoryFree, myMemoryUsed, myMemoryLimit, myHander, myState, myEventsCounter
    );
    if ( mySensor )
      mySensor->dump();
    printf ("\n");
#endif /* if MEMNOTIFY_DUMP */
}

END_MEMOTIFY_NAMESPACE

/* ====================[ end of file memnotify/watcher.cpp ]=========================== */
