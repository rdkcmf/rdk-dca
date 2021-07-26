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
SUBDIRS =
bin_PROGRAMS = dca dcamem dcacpu
dca_SOURCES = dca.c dcalist.c dcautils.c dcaproc.c dcajson.c

dca_CFLAGS = $(GLIB_CFLAGS)
dca_LDFLAGS = $(GLIB_LIBS) $(LIBCJSON_LIBS)

if IS_LIBSYSWRAPPER_ENABLED
dca_CFLAGS += $(LIBSYSWRAPPER_FLAG)
dca_LDFLAGS += -lsecure_wrapper
endif

if ENABLE_TR181_MESSAGEBUS
dca_SOURCES += dcatr181.c
dca_CFLAGS += $(DBUS_CFLAGS) -DUSE_TR181_CCSP_MESSAGEBUS
dca_LDFLAGS += $(DBUS_LIBS)
endif

dcamem_SOURCES = dcamem.c
dcacpu_SOURCES = dcacpu.c

if WITH_GTEST_SUPPORT
SUBDIRS += test
endif
