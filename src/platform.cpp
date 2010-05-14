/* ========================================================================= *
 * File: platform.cpp
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

#include <memnotify/platform.hpp>
#include <memnotify/cached_file.hpp>

BEGIN_MEMNOTIFY_NAMESPACE


/* ========================================================================= *
* Class Platform::SystemMemory.
* ========================================================================= */

void Platform::SystemMemory :: dump() const
{
#if MEMNOTIFY_DUMP
  printf ("Platform::SystemMemory %08x { actual %u uptime %ld memory unit %u\n"
      (uint)this, (uint)myActual, mySysinfo.uptime, mySysinfo.mem_unit
    );

  printf ("   ram: total %u free %u shared %u buffer %u; swap: total %u free %u; high: total %u free %u\n}\n"
      mySysinfo.totalram, mySysinfo.freeram, mySysinfo.sharedram, mySysinfo.bufferram,
      mySysinfo.totalswap, mySysinfo.freeswap, mySysinfo.totalhigh, mySysinfo.freehigh
    );
#endif /* if MEMNOTIFY_DUMP */
} /* dump */

/* ========================================================================= *
 * Class Platform.
 * ========================================================================= */

static Platform* Platform :: ourPlatform = NULL;


bool Platform :: parseOptions()
{
  const char* env = getenv(MEMNOTIFY_ENV_NAME);
  if (env && *env)
  {
    const QStringList list = QString(env).split(":", QString::SkipEmptyParts);

    foreach(QString str, list)
    {
      const QStringList o_v = str.split("=", QString::SkipEmptyParts);

      /* Now select: value or just enabler */
      if (o_v.size() > 1)
        myOptions.append( Option(o_v[0], o_v[1]) );
      else
        myOptions.append( Option(o_v[0], "1") );
    }

    return !list.isEmpty();
  }

  return false;
} /* parseOptions */

bool Platform :: syspart(const char* str)
{
  if (str && *str && 0 == access(str, F_OK))
  {
    if (mySyspart)
      free(mySyspart);
    mySyspart = strdup(str);
    return (NULL != mySyspart);
  }
  else
  {
    return false;
  }
}  /* syspart */

bool Platform :: path(const char* name, char* buffer, uint size) const
{
  /*
    Files could be located:
    - /etc/memnotify folder as a system-wide specification, accessible as "platform.mn"
    - $HOME folder - as user-only specification, accessible as "./quake3/memory.mn"
    - some path which you like, e.g. "/etc/defaults/quake.mn"
    - cgroup:/name_in_cgroup - in current process cgroup
  */
  if (!name || !*name)
    return false;

  if (!buffer || !size)
    return false;

  const char cgroup[] = "cgroup:";
  const uint nlen = strlen(name);
  const uint clen = sizeof(MEMNOTIFY_CONF_PATH) - 1;

  /* Worst case required size estimation + .nm extension */
  if (nlen + clen + 8 >= size)
    return false;

  if ('/' == *name)
  {
    /* absolute path first */
    memcpy(buffer, name, nlen);
    buffer[nlen] = 0;
  }
  else if ('.' == *name)
  {
    /* home folder next */
    const char*    home = getenv("HOME");
    const uint hlen = strlen(home);

    if (nlen + hlen + 4 >= size)
      return false;

    memcpy(buffer, home, hlen);
    memcpy(buffer + hlen, name, nlen);
    buffer[nlen + hlen] = 0;
  }
  else if (0 == strncmp(cgroup, name, sizeof(cgroup)-1))
  {
    /* replacing cgroup:/somefile to /syspart/current_group_path/somefile */
    const char* current_group = strrchr(cgroup(), ':');
    if ( current_group )
      current_group++;
    else
      return false;

    const int rc = snprintf(buffer, size, "%s%s%s", syspart(), current_group, name + sizeof(cgroup) - 1);
    return (rc > 0 && (uint)rc < size && 0 == access(buffer, R_OK));
  }
  else
  {
    /* File located in default folder */
    memcpy(buffer, MEMNOTIFY_CONF_PATH, clen);
    memcpy(buffer + clen, name, nlen);
    buffer[clen + nlen] = 0;
  }

  /* Verify extension */
  if (NULL == strrchr(buffer, '.'))
  {
    strcat(buffer, ".mn");
  }

  return (0 == access(buffer, R_OK));
} /* path */

const char* Platform :: cgroup() const
{
  /* That is common for whole process - we can use static variable opened on demand */
  static CachedFile* cgf = NULL;

  if (NULL == cgf)
    cgf = new CachedFile("/proc/self/cgroup");

  return (cgf && cgf->load() ? cgf->text() : NULL);
} /* cgroup */

void Platform :: dump() const
{
#if MEMNOTIFY_DUMP
  printf ("Platform %08x { mySyspart '%s' ",
          (uint)this, mySyspart
    );

  printf ("options %08x = [", (uint)(&myOptions));
  foreach (Option option, myOptions)
  {
    printf ("%s = %s;", option.first.toAscii().constData(), option.second.toAscii().constData());
  }
  printf ("]\n");

  myMemory.dump();
  printf ("}\n");
#endif /* if MEMNOTIFY_DUMP */
} /* dump */


END_MEMOTIFY_NAMESPACE

/* ====================[ end of file memnotify/platform.cpp ]=========================== */
