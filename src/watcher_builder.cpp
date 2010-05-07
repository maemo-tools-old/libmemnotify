/* ========================================================================= *
 * File: watcher_builder.cpp
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
#include <memnotify/watcher_builder.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
* Class WatcherBuilder.
* ========================================================================= */

static _BUILDER_INFO* WatcherBuilder :: ourBuilders = NULL;

Watcher* WatcherBuilder :: build(const QSettings& theData, const QString& theName)
{
  const QString scanner( theData.value(theName + "/type").toString() );

  /* Make a lookup only if controller type name looks sane */
  if (scanner.length() > 1)
  {
    for (const _BUILDER_INFO* cursor = ourBuilders; cursor; cursor = cursor->mySucc)
    {
      Q_ASSERT(cursor->myType);
      Q_ASSERT(cursor->myFunc);
      if (scanner == cursor->myType)
        return cursor->myFunc(theName, theData);
    } /* for */
  }

  /* Nothing found */
  return NULL;
} /* build */

bool WatcherBuilder :: announce(_BUILDER_INFO* info)
{
  if (info && info->myType && info->myFunc)
  {
    info->mySucc = ourBuilders;
    ourBuilders  = info;
    return true;
  }

  return false;
} /* announce */

void WatcherBuilder :: dump()
{
#if MEMNOTIFY_DUMP
  printf ("WatcherBuilder %08x {\n", (unsigned)ourBuilders);

  for (const _BUILDER_INFO* cursor = ourBuilders; cursor; cursor = cursor->mySucc)
  {
    printf ("   _BUILDER_INFO %08x: type '%s' func %08x succ %08x\n",
          (unsigned)cursor, cursor->myType, (unsigned)(cursor->myFunc), (unsigned)(cursor->mySucc)
      );
  }

  printf ("}\n");
#endif /* if MEMNOTIFY_DUMP */
}

END_MEMOTIFY_NAMESPACE

/* =================[ end of file memnotify/watcher_builder.cpp ]========================= */
