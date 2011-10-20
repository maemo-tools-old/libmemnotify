Name: libmemnotify
Version: 1.0.0
Release: 1%{?dist}
Summary: Memory Notification library Qt-style
Group: Development/Tools
License: GPLv2+
URL: http://www.gitorious.org/+maemo-tools-developers/maemo-tools/libmemnotify
Source: %{name}_%{version}.tar.gz
BuildRoot: {_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires: libqt4-devel, graphviz, doxygen, pkg-config

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
%defattr(644, root, root, -)
%{_libdir}/%{_name}.so.*
%doc README COPYING

#
# Development package
#
%package -n %{_name}-devel
Summary: Memory Notification library Qt-style
Group: Development/Tools

%description -n %{_name}-devel
 Development package for Memory Notification.

%files -n %{_name}-devel
%defattr(644, root, root, -)
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
%defattr(644, root, root, -)
%{_defaultdocdir}/%{_name}-doc/*.txt
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
%defattr(755, root, root, -)
%{_libdir}/%{_name}/tests/cached_file
%{_libdir}/%{_name}/tests/memory_notification
%{_libdir}/%{_name}/tests/platform
%{_libdir}/%{_name}/tests/watcher_builder
%defattr(644, root, root, -)
%{_libdir}/%{_name}/tests/cgroup.mn
%{_libdir}/%{_name}/tests/debug.mn
%{_libdir}/%{_name}/tests/*.txt
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
%defattr(644, root, root, -)
%{_sysconfdir}/memnotify/default.mn
%doc README COPYING
