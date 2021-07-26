##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2016 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
AC_PREREQ([2.53])
AC_INIT(dca, 1.0, Vargheese_Baby@cable.comcast.com)
AC_CONFIG_SRCDIR([src])
AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-bzip2 1.9])
LT_INIT
LIBSYSWRAPPER_FLAG=" "
IS_LIBSYSWRAPPER_ENABLED=" "

GTEST_ENABLE_FLAG = ""

AC_ARG_ENABLE([gtestapp],
             AS_HELP_STRING([--enable-gtestapp],[enable Gtest support (default is no)]),
             [
               case "${enableval}" in
                yes) GTEST_SUPPORT_ENABLED=true
                     GTEST_ENABLE_FLAG="-DGTEST_ENABLE"
                     m4_if(m4_sysval,[0],[AC_CONFIG_FILES([src/test/Makefile])]);;
                no) GTEST_SUPPORT_ENABLED=false AC_MSG_ERROR([Gtest support is disabled]);;
                 *) AC_MSG_ERROR([bad value ${enableval} for --enable-gtestapp ]);;
               esac
             ],
             [echo "Gtestapp is disabled"])
AM_CONDITIONAL([WITH_GTEST_SUPPORT], [test x$GTEST_SUPPORT_ENABLED = xtrue])

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_CPP
AC_PROG_CXXCPP

dnl use pretty build output with automake >= 1.11
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])],
  [AM_DEFAULT_VERBOSITY=1
   AC_SUBST(AM_DEFAULT_VERBOSITY)])

AC_ARG_ENABLE([libsyswrapper],
        AS_HELP_STRING([--enable-wrapper],[enables system wrapper replacement (default is no)]),
        [
          case "${enableval}" in
           yes) IS_LIBSYSWRAPPER_ENABLED=true
                LIBSYSWRAPPER_FLAG=" -DLIBSYSWRAPPER_BUILD ";;
           no)  IS_LIBSYSWRAPPER_ENABLED=false ;;
          *) AC_MSG_ERROR([bad value ${enableval} for --enable-wrapper]) ;;
           esac
           ],
        [echo "libsyswrapper is disabled"])
AM_CONDITIONAL([IS_LIBSYSWRAPPER_ENABLED], [test x$IS_LIBSYSWRAPPER_ENABLED = xtrue])
AC_SUBST(LIBSYSWRAPPER_FLAG)

dnl **********************************
dnl checks for dependencies
dnl **********************************
AC_HEADER_STDC

dnl Checks for header files.
AC_CHECK_HEADERS([limits.h memory.h stdlib.h string.h sys/socket.h unistd.h])

ENABLE_TR181_MESSAGEBUS=false

dnl Checks for typedefs, structures, and compiler characteristics.
dnl AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT8_T
dnl Checks for library functions.
AC_FUNC_MALLOC

PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.24.0])
PKG_CHECK_MODULES([LIBCJSON], [libcjson >= 1.0])
#PKG_CHECK_MODULES([DBUS], [dbus-1  >= 0.28])

AC_ARG_ENABLE([tr181messagebus],
              AS_HELP_STRING([--enable-tr181messagebus],[enable tr181 ccsp message bus (default is no)]),
              [
                case "${enableval}" in
                 yes) ENABLE_TR181_MESSAGEBUS=true;;
                  no) AC_MSG_ERROR([tr181 ccsp message bus support is disabled]) ;;
                   *) AC_MSG_ERROR([bad value ${enableval} for --enable-tr181messagebus]) ;;
                esac
              ],
              [echo "tr181 ccsp message bus support is disabled"])

AM_CONDITIONAL([ENABLE_TR181_MESSAGEBUS], [test x$ENABLE_TR181_MESSAGEBUS = xtrue])


AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_SUBST(GTEST_ENABLE_FLAG)
AC_OUTPUT
