/* ========================================================================= *
 * File: test-memory_notification.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <memnotify/memory_notification.hpp>


/* ========================================================================= *
 * Very simple test application
 * ========================================================================= */

static void myListener(const QString& name, const bool state)
{
  printf ("CALLBACK: memory signal %s delivered in state %d\n", name.toAscii().constData(), state);
}

class clListener: public QObject
{
  public:
    Q_OBJECT

    clListener(QObject* parent): QObject(parent)
    {}

  public slots:

    inline void notified(const QString& name, const bool state)
    {
      printf ("SIGNAL: memory signal %s delivered in state %d\n", name.toAscii().constData(), state);
      exit(0);
    }
};

int main(const int argc, char* argv[])
{
  printf ("sanity testing MemoryNotification\n");
  QCoreApplication app(argc, argv);

  MEMNOTIFY::MemoryNotification* notification = MEMNOTIFY::MemoryNotification::defaultObject();
  if ( !notification )
  {
    printf ("nulled notification\n");
    return 1;
  }

  if ( !notification->addObserver(myListener) )
  {
    printf ("addObserver failed\n");
    return 1;
  }

  clListener listener(app);
  connect(notification, SIGNAL(notified(const QString&, const bool)), &listener, SLOT(notified(const QString&, const bool)));

  notification->dump();

  return QCoreApplication::exec();
} /* main */

/* =================[ end of file memnotify/test-watcher_builder.cpp ]====================== */
