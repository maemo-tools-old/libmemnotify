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

#include <QCoreApplication>
#include <memnotify/memory_notification.hpp>


/* ========================================================================= *
 * Very simple test application
 * ========================================================================= */

static void listenerObserver(const QString& name, const bool state)
{
  printf ("CALLBACK: memory signal %s delivered in state %d\n", name.toAscii().constData(), state);
}

class Listener: public QObject
{
  public slots:

    inline void notified(const QString& name, const bool state)
    {
      printf ("SIGNAL: memory signal %s delivered in state %d\n", name.toAscii().constData(), state);
      exit(0);
    }
};

class TestApp: public QCoreApplication
{
  public:

    TestApp(int argc, char* argv[]);
    ~TestApp();

  protected:

    Listener  myListener;
    MEMNOTIFY::MemoryNotification& myNotification;
};


TestApp::TestApp(int argc, char* argv[]): QCoreApplication(argc, argv), myListener(), myNotification(MEMNOTIFY::MemoryNotification::defaultObject())
{
  if ( NULL == (&myNotification) )
  {
    printf ("nulled notification\n");
    exit(1);
  }

  if ( !myNotification.addObserver(listenerObserver) )
  {
    printf ("addObserver(%08x) failed\n", (uint)listenerObserver);
    exit(1);
  }

  connect(&myNotification, SIGNAL(notified(const QString&, const bool)), &myListener, SLOT(notified(const QString&, const bool)));
  myNotification.dump();

  if ( !myNotification.valid() )
  {
    printf ("MemoryNotification is not valid\n");
    exit(1);
  }
}

TestApp::~TestApp()
{
  printf ("before destroying MemoryNotification::defaultObject()\n");
  delete &myNotification;
  printf ("after destroying MemoryNotification::defaultObject()\n");
}

int main(int argc, char* argv[])
{
  printf ("sanity testing MemoryNotification\n");
  TestApp app(argc, argv);

  return app.exec();
} /* main */

/* =================[ end of file memnotify/test-watcher_builder.cpp ]====================== */
