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
#include <memnotify/definitions.hpp>
#include <memnotify/watcher.hpp>
#include <memnotify/poller.hpp>

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

    /* Created singletone and loaded only default file "default.mn" */
    static MemoryNotification& defaultObject();

  protected:

    /* Overloaded from Object to track subscription of observers */
    virtual void connectNotify(const char* signal);
    virtual void disconnectNotify(const char* signal);

    /* Remove all watchers from memory */
    void clearWatchers();

    /* Shared singletone data */
    static MemoryNotification*  ourMemoryNotification;

    /* That is instance data */
    QList<OBSERVER> myObservers;  /* list of observers, expected to be 1 or 2 usually, 10+ means design problems            */
    QList<Watcher*> myWatchers;   /* list of watchers/thresholds, expected to be 1 or 2 usually, 10+ means design problems  */
    uint        mySignalCounter;  /* how many objects called connect() for this one as a source */
    bool        myEnabled;        /* are we enabled or disabled? Just for fast checks           */
    Poller*     myPoller;         /* the thread to call poll if we are not using the main loop, it is alive from poll till disable call */

}; /* Class MemoryNotification */


/* ========================================================================= *
 * Inline methods for MemoryNotification.
 * ========================================================================= */

inline MemoryNotification :: MemoryNotification()
: myObservers(), myWatchers(), mySignalCounter(0), myEnabled(false), myPoller(NULL)
{}

inline MemoryNotification :: ~MemoryNotification()
{
  if ( myEnabled )
    disable();

  /* Clean up watchers with memory removal */
  clearWatchers();

  /* Clean up observers */
  myObservers.clear();

  if (ourMemoryNotification == this)
    ourMemoryNotification = NULL;
} /* ~MemoryNotification */

inline Watcher* MemoryNotification :: watcher(const char* name) const
{
  if (name && *name)
  {
    const QString scanner(name);
    foreach(Watcher* watcher, myWatchers)
    {
      Q_ASSERT(watcher != NULL);
      if (scanner == watcher->name())
        return watcher;
    }
  }

  return NULL;
} /* watcher */

inline bool MemoryNotification :: addObserver(OBSERVER observer)
{
  if ( !observer )
    return false;

  if (myObservers.count() > 0 && myObservers.indexOf(observer) >= 0)
    return false;

  myObservers.append(observer);
  return true;
} /* addObserver */

inline bool MemoryNotification :: delObserver(OBSERVER observer)
{
  if ( !observer )
    return false;

  if (myObservers.isEmpty())
    return false;

  return myObservers.removeOne(observer);
} /* delObserver */

inline uint MemoryNotification :: eventsCounter() const
{
  uint counter = 0;
  foreach (const Watcher* watcher, myWatchers)
  {
    Q_ASSERT(watcher != NULL);
    counter += watcher->eventsCounter();
  }
  return counter;
} /* eventsCounter */

inline void MemoryNotification :: connectNotify(const char* signal)
{
    mySignalCounter++;
    QObject::connectNotify();
}

inline void MemoryNotification :: disconnectNotify(const char* signal)
{
  if (mySignalCounter > 0)
    mySignalCounter--;
  QObject::disconnectNotify();
}

inline MemoryNotification* MemoryNotification :: create(const char* pathSpecification)
{
  MemoryNotification* instance = new MemoryNotification();

  if ( instance )
  {
    if ( instance->setup(pathSpecification) )
      return instance;
    delete instance;
  }

  /* Here we have a problems */
  return NULL;
} /* create */

inline MemoryNotification& MemoryNotification :: defaultObject()
{
  if ( !ourMemoryNotification )
  {
    ourMemoryNotification = new MemoryNotification();
    Q_ASSERT(ourMemoryNotification != NULL);
    if (ourMemoryNotification)
      ourMemoryNotification->setup(NULL);
  }

  return (*ourMemoryNotification);
} /* defaultObject */


END_MEMOTIFY_NAMESPACE
QT_END_HEADER

#endif /* MEMORY_NOTIFICATION_HPP_USED */
