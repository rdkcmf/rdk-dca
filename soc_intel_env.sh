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

export RMF_DIR=${RDK_DIR}/mediaframework
export TOOLCHAINDIR=${RDK_DIR}/sdk/toolchain/staging_dir
export PLATFORM_SDK=${RDK_DIR}/sdk/toolchain/staging_dir
export RAMDISKDIR=${RDK_DIR}/sdk/fsroot/ramdisk
export FSROOT=${RDK_DIR}/sdk/fsroot/ramdisk

export CFLAGS='-I$(RDK_DIR)/rdklogger/include -I$(RDK_DIR)/opensource/include -I$(RDK_DIR)/mediaframework/osal/include -I$(RDK_DIR)/mediaframework/osal/utils/inc -I$(RDK_DIR)/mediaframework/dvr/common/dvrmgr -I$(RDK_DIR)/mediaframework/dvr/common/sharedtsb -I$(RDK_DIR)/mediaframework/dvr/common/dvrmgr/soc/intel -I$(RDK_DIR)/mediaframework/platform/soc/dvrmgr -I$(RDK_DIR)/mediaframework/dvr/common/tasseograph -I$(RDK_DIR)/mediaframework/core -I$(RDK_DIR)/mediaframework/dvr/sink -I$(RDK_DIR)/mediaframework/qamsource/main -I$(RDK_DIR)/mediaframework/ippvsource -I$(RDK_DIR)/mediaframework/trh -g'

export CXXFLAGS=$CFLAGS
#/dvr/common/dvrmgr/soc/intel/dvrmanager-soc.h
#export LDFLAGS="-L${RDK_DIR}/mediaframework/build/lib -ltrh -L${RDK_DIR}/opensource/lib -llog4c -ljansson -luuid -L${RDK_DIR}/rdklogger/build/lib -lrdkloggers -L${RDK_DIR}/mediaframework/build/lib -lrmfosal"

export LD_LIBRARY_PATH="${RDK_FSROOT_PATH}/usr/lib/:${RDK_FSROOT_PATH}/usr/local/lib/:${RDK_FSROOT_PATH}/mnt/nfs/bin/:${RDK_FSROOT_PATH}/mnt/nfs/env/:${RDK_FSROOT_PATH}/mnt/nfs/lib/"
#SETTING CROSS COMPILER
export CROSS_COMPILE=${TOOLCHAINDIR}/bin/i686-cm-linux-
export CC=${CROSS_COMPILE}gcc
export GCC=${CROSS_COMPILE}gcc
export GXX=${CROSS_COMPILE}g++
export CXX=${CROSS_COMPILE}g++
export LD=${CROSS_COMPILE}ld
export M4=${CROSS_COMPILE}m4
export NM=${CROSS_COMPILE}nm
export RANLIB=${CROSS_COMPILE}ranlib
export BISON=${CROSS_COMPILE}bison
export STRIP=${CROSS_COMPILE}strip

#SETTING SOURCE DIRECTORY AND PLATFORM DEPENDENT LIBRARY PATH
#export SH_LIB_PATH=${PWD}/../broadcom/trunk/lib
export SH_LIB_PATH=${PWD}/../lib
export INSTALLDIR=${PWD}/../lib

export PKG_CONFIG_PATH="$PREFIX_FOLDER/lib/pkgconfig/:$RDK_FSROOT_PATH/usr/local/lib/pkgconfig/:$RDK_TOOLCHAIN_PATH/lib/pkgconfig/:$PKG_CONFIG_PATH"
