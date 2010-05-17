/* ========================================================================= *
 * File: watcher_builder.hpp
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

#ifndef WATCHER_BUILDER_HPP_USED
#define WATCHER_BUILDER_HPP_USED

/* ========================================================================= *
* Includes.
* ========================================================================= */

#include <fcntl.h>

#include <QString>
#include <QSettings>
#include <memnotify/definitions.hpp>
#include <memnotify/watcher.hpp>

QT_BEGIN_HEADER
BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
 * Class WatcherBuilder - constructs the watchers according to pointed
 * threshold name.
 * ========================================================================= */

/*!
  \macro ANNOUNCE_WATCHER
  \relates <memnotify/watcher_builder.hpp>
  Registration of pointed watcher object to be constructed according to type name and using pointed Watcher class.
  Example:
    ANNOUNCE_WATCHER(CgroupsWatcher, cgroups_control);
    ANNOUNCE_WATCHER(CgroupsWatcher, cgroups);
*/
#define ANNOUNCE_WATCHER(W,N)    static const WatcherBuilder::Announcer<W,__LINE__> announce ## W ## N (#N)


/*!
  \class WatcherBuilder
  \relates <memnotify/watcher_builder.hpp>
  Watcher object construction by name of controller and passed settings.
  \sa WatcherBuilder
*/

class MEMNOTIFY_EXPORT WatcherBuilder
{
  public:

    /* The callback to make a builder by request */
    typedef Watcher* (*_BUILDER_FUNC)(const QSettings&, const QString&);


    /* Structure to create record about how to construct builder */
    struct _BUILDER_INFO
    {
      const char*     myType;   /* name of controller type discovered in settings */
      _BUILDER_FUNC   myFunc;   /* function to construct controller               */
      _BUILDER_INFO*  mySucc;   /* the next entry in list                         */
    };


    /* Internal class which is used to register builder from constructor     */
    /* The integer ID part as line number added to prevent re-using the      */
    /* same template instance for aliaces which might be several in one file */
    template <class W, int L>
    class Announcer
    {
      public:

        inline Announcer(const char* typeName)
        {
          static _BUILDER_INFO info = {typeName, (_BUILDER_FUNC)build, NULL };
          WatcherBuilder::announce(&info);
        }

        inline ~Announcer()
        {}

      private:

        static W* build(const QSettings& theData, const QString& theName)
        {
          return new W(theData, theName);
        }
    }; /* Class Announcer */

  public:

    static Watcher* build(const QSettings& theData, const QString& theName);
    static bool  announce(_BUILDER_INFO* info);
    static void  dump();

  protected:

    static _BUILDER_INFO* ourBuilders;

}; /* Class WatcherBuilder */


/* ========================================================================= *
 * Inline methods for WatcherBuilder.
 * ========================================================================= */

END_MEMOTIFY_NAMESPACE
QT_END_HEADER

#endif /* WATCHER_BUILDER_HPP_USED */
