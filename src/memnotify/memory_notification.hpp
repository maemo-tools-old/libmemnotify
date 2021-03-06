/* ========================================================================= *
 * File: memory_notification.hpp
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

#ifndef MEMORY_NOTIFICATION_HPP_USED
#define MEMORY_NOTIFICATION_HPP_USED

/* ========================================================================= *
* Includes.
* ========================================================================= */

#include <QObject>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <memnotify/definitions.hpp>
#include <memnotify/watcher.hpp>

QT_BEGIN_HEADER
BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
 * Class MemoryNotification - constructs the watchers according to pointed
 * configuration files and handle notification.
 * ========================================================================= */

class MEMNOTIFY_EXPORT MemoryNotification: public QObject
{
  public:

    Q_OBJECT

    /* Memory notification callback which is much faster to handle signals but not thread safe */
    typedef void (*OBSERVER)(const QString& name, const bool state);

  public:

    MemoryNotification();
    ~MemoryNotification();

    /* setup MemoryNotification according to passed path speficiation or using default.mn file */
    /* returns true if all watchers/thresholds enabled sussessfully                            */
    bool setup(const char* pathSpecification=NULL);

    /* starts to listen in automatic mode using aio and temporary thread, required subscription */
    bool poll();

    /* queries events descriptors to make poll outside, required subscription */
    /* return the number of filled file descriptors in fds parameter          */
    uint query(int* fds, uint size);

    /* handles events detected in extenal poll loop. required subscription   */
    /* WARNING: you should pass only fds which have incoming events detected */
    bool process(const int* fds, uint counter);

    /* enables tracking, called automatical when listen() or queryEvents() called */
    bool enable();

    /* disables tracking, called before destruction of object or if you need re-subscribe */
    bool disable();

    /* queries threshold by name to detailed investigation */
    Watcher* watcher(const char* name) const;

    /* if you like to use callbacks you can use these calls to subscribe/unsubscribe */
    bool addObserver(OBSERVER observer);
    bool delObserver(OBSERVER observer);

    /* Statistics of processed events to crossing thresholds */
    uint eventsCounter() const;

    /* validates object and returns true if it created correct */
    bool valid() const;

    /* dumps contents to stdout for debugging/tracing needs */
    void dump() const;

  signals:

    void notified(const QString& name, const bool state);

  public:

    /* Created object with pointed path specification and load all files one by one */
    /* Returns object only in case all files are loaded properly                    */
    static MemoryNotification* create(const char* pathSpecification=NULL);

    /* Created singletone and loaded default or or pointed file(s) */
    static MemoryNotification& defaultObject(const char* pathSpecification=NULL);

  protected:

    /* Remove all watchers from memory */
    void clearWatchers();

    /* Shared singletone data */
    static MemoryNotification*  ourMemoryNotification;

    /* That is instance data */
    QList<OBSERVER> myObservers;  /* list of observers, expected to be 1 or 2 usually, 10+ means design problems            */
    QList<Watcher*> myWatchers;   /* list of watchers/thresholds, expected to be 1 or 2 usually, 10+ means design problems  */
    bool        myEnabled;        /* are we enabled or disabled? Just for fast checks           */
    QThread*    myPoller;         /* the thread to call poll if we are not using the main loop, it is alive from poll till disable call */
    QMutex      myMutex;          /* for handling process, enable, disable correct and prevent crashes */

}; /* Class MemoryNotification */


END_MEMOTIFY_NAMESPACE
QT_END_HEADER

#endif /* MEMORY_NOTIFICATION_HPP_USED */
