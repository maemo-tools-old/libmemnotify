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
#include "test-memory_notification.hpp"


/* ========================================================================= *
 * Very simple test application
 * ========================================================================= */

static void listenerObserver1(const QString& name, const bool state)
{
  printf ("CALLBACK#1: memory signal %s delivered in state %d\n", name.toAscii().constData(), state);
}

static void listenerObserver2(const QString& name, const bool state)
{
  printf ("CALLBACK#2: memory signal %s delivered in state %d\n", name.toAscii().constData(), state);
}


TestApp::TestApp(int argc, char* argv[])
  : QCoreApplication(argc, argv), myListener(), myNotification(MEMNOTIFY::MemoryNotification::defaultObject(argv[1]))
{
  printf ("* %s %u *\n", __PRETTY_FUNCTION__, __LINE__);
  if ( NULL == (&myNotification) )
  {
    printf ("nulled notification\n");
    ::exit(1);
  }

  printf ("* %s %u *\n", __PRETTY_FUNCTION__, __LINE__);
  if ( !myNotification.addObserver(listenerObserver1) )
  {
    printf ("addObserver(%08x) failed\n", (uint)listenerObserver1);
    ::exit(1);
  }

  printf ("* %s %u *\n", __PRETTY_FUNCTION__, __LINE__);
  if ( !myNotification.addObserver(listenerObserver2) )
  {
    printf ("addObserver(%08x) failed\n", (uint)listenerObserver2);
    ::exit(1);
  }

  printf ("* %s %u *\n", __PRETTY_FUNCTION__, __LINE__);
  connect(&myNotification, SIGNAL(notified(const QString&, const bool)), &myListener, SLOT(notified(const QString&, const bool)));

  printf ("* %s %u *\n", __PRETTY_FUNCTION__, __LINE__);
  if ( !myNotification.valid() )
  {
    printf ("MemoryNotification is not valid\n");
    ::exit(1);
  }

  printf ("* %s %u *\n", __PRETTY_FUNCTION__, __LINE__);
  if ( !myNotification.enable() )
  {
    printf ("MemoryNotification is not enabled\n");
    myNotification.dump();
    ::exit(1);
  }

  printf ("* %s %u => MemoryNotification is enabled *\n", __PRETTY_FUNCTION__, __LINE__);
  if ( !myNotification.poll() )
  {
    printf ("MemoryNotification cannot start polling\n");
    myNotification.dump();
    ::exit(1);
  }

  printf ("MemoryNotification::poll() succeed, ready for incoming events\n");
  myNotification.dump();
  printf ("* %s %u *\n", __PRETTY_FUNCTION__, __LINE__);
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

/* =================[ end of file test-memory_notification.cpp ]====================== */
