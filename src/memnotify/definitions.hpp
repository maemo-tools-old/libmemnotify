/* ========================================================================= *
 * File: definitions.hpp
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

#ifndef DEFINITIONS_HPP_USED
#define DEFINITIONS_HPP_USED

/* ========================================================================= *
* Includes.
* ========================================================================= */

#include <QtCore/QtGlobal>

QT_BEGIN_HEADER

/* ========================================================================= *
 * Definitions.
 * ========================================================================= */

/* The default namespace, later could be re-defined to MemNotifyNameSpace_V1 */
/*!
  \macro MEMNOTIFY
  \relates <memnotify/definitions.hpp>
  This macro should be used to control namespace by default.
  \sa BEGIN_MEMNOTIFY_NAMESPACE
  \sa END_MEMOTIFY_NAMESPACE
*/

#ifndef MEMNOTIFY
#define MEMNOTIFY     MemNotifyNameSpace
namespace MEMNOTIFY   {}
#endif

#define BEGIN_MEMNOTIFY_NAMESPACE   namespace MEMNOTIFY {
#define USE_MEMNOTIFY_NAMESPACE     using namespace ::MEMNOTIFY;
#define END_MEMOTIFY_NAMESPACE      }

BEGIN_MEMNOTIFY_NAMESPACE

/*!
  \macro MEMNOTIFY_SHARED
  \relates <memnotify/definitions.hpp>
  This macro marks a symbol for shared library export by reusing Q_DECL_EXPORT or Q_DECL_IMPORT.
  \sa Q_DECL_IMPORT
  \sa Q_DECL_EXPORT
*/
#ifdef  MEMNOTIFY_SHARED
#define MEMNOTIFY_EXPORT  Q_DECL_EXPORT
#else
#define MEMNOTIFY_EXPORT  Q_DECL_IMPORT
#endif

/*!
  \macro MEMNOTIFY_PRIVATE
  \relates <memnotify/definitions.hpp>
  This macro marks a symbol as non-exportable and non-visible outside.
*/
#ifndef MEMNOTIFY_PRIVATE
#define MEMNOTIFY_PRIVATE __attribute__((__visibility__("hidden")))
#endif

#ifndef MEMNOTIFY_VERSION
#define MEMNOTIFY_VERSION   "0.0.0"
#endif

/*!
  \macro MEMNOTIFY_DUMP
  \relates <memnotify/definitions.hpp>
  This macro marks a symbol as non-exportable and non-visible outside.
  \sa QT_NO_DEBUG
*/
#ifdef QT_NO_DEBUG
#define MEMNOTIFY_DUMP      0
#else
#define MEMNOTIFY_DUMP      1
#endif

/* ------------------------------------------------------------------------- *
 * XXXX
 * ------------------------------------------------------------------------- */

/* ========================================================================= *
 * Methods.
 * ========================================================================= */

/* ------------------------------------------------------------------------- *
 * XXXX
 * parameters:
 *    XXX.
 * returns:
 *    XXX.
 * ------------------------------------------------------------------------- */

END_MEMOTIFY_NAMESPACE
QT_END_HEADER

#endif /* DEFINITIONS_HPP_USED */
