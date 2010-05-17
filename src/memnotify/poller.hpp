/* ========================================================================= *
 * File: poller.hpp
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

#ifndef POLLER_HPP_USED
#define POLLER_HPP_USED

/* ========================================================================= *
* Includes.
* ========================================================================= */

#include <poll.h>
#include <QThread>
#include <memnotify/definitions.hpp>

QT_BEGIN_HEADER
BEGIN_MEMNOTIFY_NAMESPACE

/* ========================================================================= *
 * Class Poller - initiate poll/epoll call and invokes process for notification.
 *  the expectation to have below 10 handlers in polling loop and memory
 *  updates will come rarely - so poll() was used for now.
 * ========================================================================= */

class MEMNOTIFY_PRIVATE Poller
{

  public:

    Poller(const QObject* notification, const int* handlers, const uint counter);
    ~Poller();

  protected:

    virtual void run();

  protected:

    struct pollfd*  myHandlers;   /* expected to have below 10 rarely updated handlers -> will use poll() */
    const  nfds_t   myCounter;    /* number of handlers to be polled                                      */

}; /* Class Poller */

END_MEMOTIFY_NAMESPACE
QT_END_HEADER

#endif /* POLLER_HPP_USED */
