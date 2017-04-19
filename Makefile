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

all:src/loggrep.c
	$(CC)  src/loggrep.c -o dcafind
	$(CC)  src/logredirect.c -o dcaseek
	$(CC)  src/dcaloadave.c -o dcaloadave
	$(CC)  src/dcamem.c -o dcamem
	$(CC)  src/dcacpu.c -o dcacpu
	$(CC)  src/ipvideo.c -o ipvideo
	$(GXX) $(CFLAGS) src/dcaprocess.cpp -o dcaprocess
build:src/loggrep.c
	$(CC)  src/loggrep.c -o dcafind
	$(CC)  src/logredirect.c -o dcaseek
	$(CC)  src/dcaloadave.c -o dcaloadave
	$(CC)  src/dcamem.c -o dcamem
	$(CC)  src/dcacpu.c -o dcacpu
	$(CC)  src/ipvideo.c -o ipvideo
	$(GXX) $(CFLAGS) src/dcaprocess.cpp -o dcaprocess
clean:
	$(RM) dcafind
	$(RM) dcaseek
	$(RM) dcaloadave
	$(RM) dcamem
	$(RM) dcacpu
	$(RM) dcaprocess
	$(RM) ipvideo

