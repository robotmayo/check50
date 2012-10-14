############################################################################
Summary: Command-line interface to CS50 Check.
Name: check50
Version: 1.7
Release: 0
License: CC BY-NC-SA 3.0
Group: Applications/Productivity
Vendor: CS50
URL: https://manual.cs50.net/Check
BuildRoot: %{_tmppath}/%{name}-root
Requires: nodejs >= 0.6.18
BuildArch: noarch


############################################################################
# ensure RPM is portable by avoiding rpmlib(FileDigests)
# http://lists.rpm.org/pipermail/rpm-list/2009-October/000401.html
%global _binary_filedigest_algorithm 1
%global _source_filedigest_algorithm 1


############################################################################
# ensure RPM is portable by avoiding rpmlib(PayloadIsXz)
# http://www.cmake.org/pipermail/cmake/2010-March/035580.html
%global _binary_payload w9.gzdio


############################################################################
%description
CS50 Check is a command-line tool with which to check code's correctness.


############################################################################
%prep
rm -rf %{_builddir}/*
cp -a %{_sourcedir}/* %{_builddir}/


############################################################################
%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/opt/%{name}
cp -a %{_builddir}/* %{buildroot}/
mkdir -p %{buildroot}%{_bindir}
ln -s /opt/%{name}/bin/%{name} %{buildroot}%{_bindir}/%{name}


############################################################################
%clean
rm -rf %{buildroot}


############################################################################
%files

%defattr(0755,root,root,-)
%{_bindir}/%{name}

%defattr(0644,root,root,0755)
/opt/%{name}/

%defattr(0755,root,root,0755)
/opt/%{name}/bin/
