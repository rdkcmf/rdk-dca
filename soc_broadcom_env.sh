#!/bin/sh
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
#

#SETTING TOOLCHAIN AND RAMDISK PAT
export WORK_DIR=$COMBINED_ROOT/work${RDK_PLATFORM_DEVICE^^}
echo " WORK_DIR= $WORK_DIR "
source $WORK_DIR/../build_scripts/setBCMenv.sh

export PLATFORM_SDK=$BCMAPP
export TOOLCHAINDIR=
export RAMDISKDIR=$COMBINED_ROOT/sdk/fsroot/ramdisk
export RDK_DIR=$COMBINED_ROOT
export CFLAGS="-I${COMBINED_ROOT}/rdklogger/include -I$(RDK_DIR)/opensource/include -I$RDK_DIR/mediaframework/osal/include -I$RDK_DIR/mediaframework/dvr/common/dvrmgr -I$RDK_DIR/mediaframework/platform/soc/dvrmgr -I$RDK_DIR/mediaframework/dvr/common/sharedtsb -I$RDK_DIR/mediaframework/dvr/common/tasseograph -I$RDK_DIR/mediaframework/core -I$RDK_DIR/mediaframework/dvr/sink -I$RDK_DIR/mediaframework/qamsource/main -I$RDK_DIR/mediaframework/ippvsource -I$RDK_DIR/mediaframework/trh -I$WORK_DIR/Refsw/uclinux-rootfs/user/xfsprogs/include"
export CXXFLAGS=$CFLAGS
#export CPPFLAGS=$CFLAGS
export INCLUDES='-I{$COMBINED_ROOT}/rdklogger/include -I$(RDK_DIR)/opensource/include -I$(RDK_DIR)/mediaframework/osal/include -I$(RDK_DIR)/mediaframework/dvr/common/dvrmgr -I$(RDK_DIR)/mediaframework/platform/soc/dvrmgr -I$(RDK_DIR)/mediaframework/dvr/common/tasseograph -I$(RDK_DIR)/mediaframework/core -I$(RDK_DIR)/mediaframework/dvr/sink -I$(RDK_DIR)/mediaframework/qamsource/main'


export CROSS_COMPILE=mipsel-linux-
export CC=${CROSS_COMPILE}gcc
export GCC=${CROSS_COMPILE}gcc
export GXX=${CROSS_COMPILE}g++
export LD=${CROSS_COMPILE}ld

#SETTING SOURCE DIRECTORY AND PLATFORM DEPENDENT LIBRARY PATH
#export SH_LIB_PATH=${PWD}/../broadcom/trunk/lib
export SH_LIB_PATH=${PWD}/../lib
export INSTALLDIR=${PWD}/../lib

export DEFAULT_HOST=mipsel-linux
export PKG_CONFIG_PATH="$COMBINED_ROOT/opensource/lib/pkgconfig/:$DFB_LIB/pkgconfig:$APPLIBS_TARGET_DIR/usr/local/lib/pkgconfig:"
#export PKG_CONFIG_PATH="$PREFIX_FOLDER/lib/pkgconfig/:$RDK_FSROOT_PATH/usr/local/lib/pkgconfig/:$RDK_TOOLCHAIN_PATH/lib/pkgconfig/:$PKG_CONFIG_PATH"
