/* ========================================================================= *
 * File: poller.cpp
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

#include <memnotify/poller.hpp>
#include <memnotify/memory_notification.hpp>

BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
* Class Poller.
* ========================================================================= */

Poller :: Poller(QObject* notification, const int* handlers, const uint counter)
  : QThread(notification), myHandlers(NULL), myCounter((const nfds_t)counter)
{
  if (handlers && counter)
  {
    myHandlers = new pollfd[counter];
    if (myHandlers)
    {
      for (uint index = 0; index < counter; index++)
      {
        myHandlers[index].fd = handlers[index];
        myHandlers[index].events = POLLIN|POLLPRI|POLLOUT;
        myHandlers[index].revents = 0;
      }
      setTerminationEnabled(true);
      start();
    }
  }
} /* Poller */

Poller :: ~Poller()
{
  wait();
  if ( myHandlers )
  {
    delete myHandlers;
  }
}

#include <errno.h>

void Poller :: run()
{
  forever
  {
    /* block in the poll() call, the descriptors might be closed during that */
    const int retcode = poll(myHandlers, myCounter, -1);
    if (retcode <= 0)
    {
      printf ("poll failed with retcode %d error %s\n", retcode, strerror(errno));
      return;
    }

    /* scans the incoming event */
    int  incoming[ myCounter ];
    uint iget;
    uint iput;

    for (iget = 0, iput = 0; iget < myCounter; iget++)
    {
      /* anything ourside mask means we should exit */
      if (0 == (myHandlers[iget].events & myHandlers[iget].revents))
        return;
      /* something as expected, this fd should be re-checked for detailed info */
      myHandlers[iget].revents = 0;
      incoming[iput++] = myHandlers[iget].fd;
    }

    /* Notify the parent object */
    if ( !parent() )
      return;

    if ( !reinterpret_cast<MemoryNotification*>(parent())->process(incoming, iput) )
      return;
  } /* forever */
} /* run */

END_MEMOTIFY_NAMESPACE

/* ==================[ end of file memnotify/poller.cpp ]=================== */
