Name: libmemnotify
Version: 1.2.0
Release: 1%{?dist}
Summary: Memory Notification library Qt-style
Group: Development/Tools
License: GPLv2+
URL: http://www.gitorious.org/+maemo-tools-developers/maemo-tools/libmemnotify
Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-build
BuildRequires: libqt5base-devel, pkg-config

%description

%define _name %{name}qt

%prep
%setup -q -n %{name}

%build
qmake libmemnotifyqt.pro
make

%install
rm -rf %{buildroot}
make install INSTALL_ROOT=%{buildroot}

%clean
rm -rf %{buildroot}

%package -n %{_name}
Summary: Memory Notification library Qt-style
Group: Development/Tools

%description -n %{_name}
 The libmemnotify implements a notification for memory usage events
 according to levels specified in control file.

%files -n %{_name}
%defattr(-, root, root, -)
%{_libdir}/%{_name}.so.*
%doc README COPYING

%post -n %{_name} 
/sbin/ldconfig

%postun -n %{_name}
/sbin/ldconfig

#
# Development package
#
%package -n %{_name}-devel
Summary: Memory Notification library Qt-style
Group: Development/Tools

%description -n %{_name}-devel
 Development package for Memory Notification.

%files -n %{_name}-devel
%defattr(-, root, root, -)
%{_libdir}/%{_name}.so
%{_libdir}/pkgconfig/%{_name}.pc
%{_includedir}/memnotify/*
%doc README COPYING

#
# Documentation package
#
%package -n %{_name}-doc
Summary: Memory Notification library Qt-style
Group: Development/Tools

%description -n %{_name}-doc
 Developer documentation package for Memory Notification.

%files -n %{_name}-doc
%defattr(-, root, root, -)
%{_defaultdocdir}/%{_name}-doc/design.txt
%{_defaultdocdir}/%{_name}-doc/environment.txt
%{_libdir}/pkgconfig/%{_name}.pc

#
# Test package
#
%package -n %{_name}-test
Summary: Memory Notification library Qt-style
Group: Development/Tools

%description -n %{_name}-test
 Tests for Memory Notification.
 
%files -n %{_name}-test
%defattr(-, root, root, -)
%{_libdir}/%{_name}/tests/cached_file
%{_libdir}/%{_name}/tests/memory_notification
%{_libdir}/%{_name}/tests/platform
%{_libdir}/%{_name}/tests/watcher_builder
%{_libdir}/%{_name}/tests/cgroup.mn
%{_libdir}/%{_name}/tests/debug.mn
%{_libdir}/%{_name}/tests/sensor.txt
%doc README COPYING

#
# Default settings package
#
%package -n memnotify-settings-default
Summary: Memory Notification library Qt-style
Group: Development/Tools

%description -n memnotify-settings-default
 Default policy settings for Memory Notification.
 
%files -n memnotify-settings-default
%defattr(-, root, root, -)
%{_sysconfdir}/memnotify/default.mn
%doc README COPYING

%changelog
* Mon Jan 30 2012 Leonid Moiseichuk <leonid.moiseichuk@nokia.com> 1.2.0
  * Making rpm-variant buildable.
  * Replaced Used Memory Meter (UMM) to MemNotify signal source to be compatible with memnotify.ko
  * Added Used Memory Meter (UMM) signal source.
  * Fixing potential problems shown by gcc 4.6

* Wed Oct 05 2011 Eero Tamminen <eero.tamminen@nokia.com> 1.0.1
  * Source package rename required for integration - 

* Wed Aug 31 2011 Eero Tamminen <eero.tamminen@nokia.com> 1.0.0
  * Update version to 1.0.0 due to ABI freeze, new SONAME.
  *  - Rare artificial crash in case simultaneous poll() and
    disable() calls. Test case created by Kimmo Kinnunen.
  * Uninline inlined code in public headers.

* Fri May 06 2011 Eero Tamminen <eero.tamminen@nokia.com> 0.1.6
  * Change MemoryNotification::disable() order to remove problems with closing
    thread to improve bug 250424.
  * Increase system-wide memory limit to 90% due to now we have compressed
    swap and memory stabilized on different level. 

* Fri Apr 15 2011 Eero Tamminen <eero.tamminen@nokia.com> 0.1.5
  * Fix strange issue with unsafe delete reported by valgrind.

* Wed Apr 06 2011 Eero Tamminen <eero.tamminen@nokia.com> 0.1.4
  * Fix issue with handling string data reported by Valgrind. 
  * Reduce default limit to 85% due to 90% seems too high for current leaking
    system and produce visible UI slowdown.

* Mon Sep 13 2010 Eero Tamminen <eero.tamminen@nokia.com> 0.1.3
  * Fix issue with permissions to file access. 

* Wed Aug 04 2010 Eero Tamminen <eero.tamminen@nokia.com> 0.1.2
  * Fix build with HardFP toolchain (GCC 4.4). 

* Thu Mar 19 2010 Leonid Moiseichuk <leonid.moiseichuk@nokia.com> 0.0.1
  * Initial release
