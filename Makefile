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

INCLUDE = -Isrc \
	  -I$(GLIB_HEADER_PATH) \
	  -I$(GLIB_HEADER_PATH)/include \
	  -I$(GLIB_CONFIG_PATH)/glib-2.0/include \
	  -I$(RDK_PROJECT_ROOT_PATH)/opensource/include \
	  -I$(RDK_PROJECT_ROOT_PATH)/opensource/include/glib-2.0 \
	  -I$(RDK_PROJECT_ROOT_PATH)/opensource/lib/glib-2.0/include \
	  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/cjson \
	  -I$(RDK_PROJECT_ROOT_PATH)/opensource/usr/local/include/cjson \
	  -I$(RDK_PROJECT_ROOT_PATH)/opensource/cjson \
	  -I$(RDK_PROJECT_ROOT_PATH)/opensource/lib/cjson/include \
	  -I$(RDK_PROJECT_ROOT_PATH)/opensource/lib/glib-2.0/include \
	  -I$(COMMON_HEADER_PATH)

LIBS += -L$(GLIB_CONFIG_PATH) \
	-L$(RDK_PROJECT_ROOT_PATH)/opensource/lib \
	-L$(RDK_PROJECT_ROOT_PATH)/opensource/usr/local/lib \
	$(GLIBS) -lcjson

LDFLAGS += $(LIBS)
CFLAGS += $(INCLUDE)
ifneq ($(XHC1),true)
    CFLAGS += -DSAFEC_DUMMY_API
endif

all:src/dca.c
	$(CC) $(CFLAGS) $(LDFLAGS)  src/dca.c src/dcalist.c src/dcajson.c src/dcaproc.c src/dcautils.c -o dca
	$(CC) $(CFLAGS) src/dcamem.c -o dcamem
	$(CC) $(CFLAGS) src/dcacpu.c -o dcacpu
	$(GXX) $(CFLAGS) src/dcaprocess.cpp -o dcaprocess
build:src/dca.c
	$(CC) $(CFLAGS) $(LDFLAGS) src/dca.c src/dcalist.c src/dcajson.c src/dcaproc.c src/dcautils.c -o dca
	$(CC) $(CFLAGS) src/dcamem.c -o dcamem
	$(CC) $(CFLAGS) src/dcacpu.c -o dcacpu
	$(GXX) $(CFLAGS) src/dcaprocess.cpp -o dcaprocess
clean:
	$(RM) dcamem
	$(RM) dcacpu
	$(RM) dca
	$(RM) dcaprocess

