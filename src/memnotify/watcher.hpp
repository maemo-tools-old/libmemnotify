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
#include <memnotify/platform.hpp>
#include <memnotify/cached_file.hpp>

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

    typedef Platform::Size  Size;

  public:

    Watcher(const QSettings& theData, const QString& theName);
    virtual ~Watcher();

    /* That is watcher data for decision making */
    const QString& name() const;        /* Name of this watcher/threshold       */
    const QString& type() const;        /* Type of watcher (debug, cgroups etc) */

    /* That is theshold parameters, when we raise the memory change signal */
    Size memoryFree()  const; /* amount of free memory in bytes when should raise trigger */
    Size memoryUsed()  const; /* amount of used memory in bytes when should raise trigger, conflicts with "free" */
    Size memoryLimit() const; /* limit of memory in system to be used in addition to platform value */

    /* change the watcher activity and change watching to enabled, disabled */
    virtual bool enable();
    virtual bool disable();
            bool enabled() const;

    /* handle an external event when it is necessary, return true if event handled properly */
    /* returns true if event proceeed and may change state() if threshold crossed up/down   */
    virtual bool process() = 0;

    /* ask for handler to make detachable poll()/epoll() */
    int   handler() const;

    /* validate current status: true if level reached, false otherwise */
    bool  state() const;

    /* validate file status and contents */
    virtual bool valid() const;

    /* dumping internal information to stdout */
    virtual void dump() const;

  protected:

    QString   myName;       /* Name of theshold, for example "System Memory Low" */
    QString   myType;       /* Type of threshold watcher, e.g. "cgroups_control" */
    QString   mySensorPath; /* Path to sensor file, e.g. /syspart/memory.memsw.memory_usage_in_bytes */

    Size  myMemoryFree;   /* Amount of memory in sensor file which triggers theshold on/off, conflicts "used" */
    Size  myMemoryUsed;   /* Amount of used memory which triggers threshold on/off, conflicts with "free"     */
    Size  myMemoryLimit;  /* Memory limit, by default should be set to Platform::memory().total()             */

    int         myHandler;  /* Handler which is used to track changes (e.g. inotify or eventfd) */
    CachedFile* mySensor;   /* The sensor file if necessary, this file will have actual changes */
    bool        myState;    /* Current state which reflects real theshold value - on/off        */

  protected:

    QString option(const QSettings& theData, const char* theKey, const QVariant& theDefVal = QVariant()) const;
    Size    memoryOption(const QSettings& theData, const char* theKey);
    bool    percents(Size memoryOptionValue);

}; /* Class Watcher */

/* ========================================================================= *
 * Inline methods for Watcher.
 * ========================================================================= */

inline const QString& Watcher :: name() const
{
  return myName;
}

inline const QString& Watcher :: type() const
{
  return myType;
}

inline const QString& Watcher :: sensor() const
{
  return mySensor;
}

inline Size Watcher :: memoryFree()  const
{
  return myMemoryFree;
}

inline Size Watcher :: memoryUsed()  const
{
  return myMemoryUsed;
}

inline Size Watcher :: memoryLimit() const
{
  return myMemoryLimit;
}

inline bool Watcher :: enable()
{
  if (mySensorPath.isEmpty() || mySensor)
    return true;

  mySensor = new CachedFile(mySensorPath);
  return (mySensor && mySensor->valid());
}

inline bool Watcher :: disable()
{
  if (mySensor)
  {
    delete mySensor;
    mySensor = NULL;
  }

  return true;
}

inline bool Watcher :: enabled() const
{
  return (myHandler >= 0 && mySensor);
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

inline bool Watcher :: valid() const
{
  char tmp[256];

  return ( myMemoryFree && myMemoryUsed && myMemoryLimit &&
          Platform::defaultObject().path(mySensorPath.toAscii().constData(), tmp, sizeof(tmp)) );
}

inline QString Watcher :: option(const QSettings& theData, const char* theKey, const QVariant& theDefVal) const
{
  return theData.value(myName + "/" + QString(theKey), theDefVal).toString();
}

/* just some sugar to decision making - percents or real bytes provided */
inline bool Watcher :: percents(Size memoryOptionValue)
{
  return (memoryOptionValue > 0 && memoryOptionValue <= 100);
}

END_MEMOTIFY_NAMESPACE
QT_END_HEADER

#endif /* WATCHER_HPP_USED */
