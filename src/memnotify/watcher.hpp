/* ========================================================================= *
 * File: watcher.hpp
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

#ifndef WATCHER_HPP_USED
#define WATCHER_HPP_USED

/* ========================================================================= *
* Includes.
* ========================================================================= */

#include <QSettings>
#include <QStringList>
#include <memnotify/definitions.hpp>

QT_BEGIN_HEADER
BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
 * Class Watcher - constructs the observer which is listen for memory signals.
 * ========================================================================= */

/*!
  \class Watcher
  \relates <memnotify/watcher.hpp>
  Creates the object which listens for memory signals. Usually instanses will be created by WatcherBuilder.
  \sa WatcherBuilder
*/

class MEMNOTIFY_EXPORT Watcher
{
  public:

    Watcher(const QString& theName, const QSettings& theData);
    virtual ~Watcher();

    /* request for name of this watcher/threshold */
    const QString& name() const;

    /* change the watcher activity and change watching to enabled, disabled */
    virtual bool enable()  = 0;
    virtual bool disable() = 0;
            bool enabled() const;

    /* handle an external event when it is necessary, return true if event handled properly */
    virtual bool handle() = 0;

    /* ask for handler to make detachable poll()/epoll() */
    int   handler() const;

    /* validate current status: true if level reached, false otherwise */
    bool  state() const;

    /* validate file status and contents */
    virtual bool valid() const;

    /* dumping internal information to stdout */
    virtual void dump() const;

  protected:

    QString   myName;
    int       myHandler;
    bool      myState;

  protected:

    QVariant value(const QSettings& theData, const char* theKey, const QVariant& theDefVal = QVariant()) const;

}; /* Class Watcher */

/* ========================================================================= *
 * Inline methods for Watcher.
 * ========================================================================= */

inline const QString& Watcher :: name() const
{
  return myName;
}

inline bool Watcher :: enabled() const
{
  return (myHandler >= 0);
}

inline int Watcher :: handler() const
{
  return myHandler;
}

/* validate current status: true if level reached, false otherwise */
inline bool Watcher :: state() const
{
  return (enabled() && myState);
}

/* validate file status and contents */
inline bool Watcher :: valid() const
{
  return (myHandler >= 0);
}

inline QVariant  Watcher :: value(const QSettings& theData, const char* theKey, const QVariant& theDefVal) const
{
  return theData.value(myName + "/" + QString(theKey), theDefVal);
}

END_MEMOTIFY_NAMESPACE
QT_END_HEADER

#endif /* WATCHER_HPP_USED */
