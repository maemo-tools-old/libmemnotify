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

#ifndef _TEST_MEMORY_NOTIFICATION_
#define _TEST_MEMORY_NOTIFICATION_
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

class Listener: public QObject
{
  public:

    Q_OBJECT

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

    Q_OBJECT

  public:

    TestApp(int argc, char* argv[]);
    ~TestApp();

  protected:

    Listener  myListener;
    MEMNOTIFY::MemoryNotification& myNotification;
};

#endif /* ifdef _TEST_MEMORY_NOTIFICATION_ */
