#!/bin/bash
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

#######################################
#
# Build Framework standard script for
#
# DTCP component

# use -e to fail on any shell issue
# -e is the requirement from Build Framework
set -e

# default PATHs - use `man readlink` for more info
# the path to combined build
export RDK_PROJECT_ROOT_PATH=${RDK_PROJECT_ROOT_PATH-`readlink -m ..`}
export COMBINED_ROOT=$RDK_PROJECT_ROOT_PATH


# path to build script (this script)
export RDK_SCRIPTS_PATH=${RDK_SCRIPTS_PATH-`readlink -m $0 | xargs dirname`}

# path to components sources and target
export RDK_SOURCE_PATH=${RDK_SOURCE_PATH-`readlink -m .`}
export RDK_TARGET_PATH=${RDK_TARGET_PATH-$RDK_SOURCE_PATH}

# fsroot and toolchain (valid for all devices)
export RDK_FSROOT_PATH=${RDK_FSROOT_PATH-`readlink -m $RDK_PROJECT_ROOT_PATH/sdk/fsroot/ramdisk`}
export RDK_TOOLCHAIN_PATH=${RDK_TOOLCHAIN_PATH-`readlink -m $RDK_PROJECT_ROOT_PATH/sdk/toolchain/staging_dir`}

#default component name
export RDK_COMPONENT_NAME=${RDK_COMPONENT_NAME-`basename $RDK_SOURCE_PATH`}

export FSROOT=$RDK_FSROOT_PATH
export COMBINED_ROOT=$RDK_PROJECT_ROOT_PATH
export BUILDS_DIR=$RDK_PROJECT_ROOT_PATH
export RDK_DIR=$BUILDS_DIR
export PLATFORM_SDK=${RDK_TOOLCHAIN_PATH}
if [ "$XCAM_MODEL" == "XHB1" ] || [ "$XCAM_MODEL" == "XHC3" ]; then
    source ${RDK_PROJECT_ROOT_PATH}/build/components/sdk/setenv2
    export CFLAGS="-DENABLE_XCAM_SUPPORT"
    export CXXFLAGS=$CFLAGS
    export CPPFLAGS=$CFLAGS
else
    source $BUILDS_DIR/dca/soc_${RDK_PLATFORM_SOC}_env.sh
fi

if [ "$XCAM_MODEL" == "XHB1" ] || [ "$XCAM_MODEL" == "SCHC2" ] ; then
    export XHC1=true
    export CFLAGS+=" -I${RDK_PROJECT_ROOT_PATH}/opensource/include/libsafec -L${RDK_PROJECT_ROOT_PATH}/opensource/lib -lsafec-3.6.0"
fi

# default component name
export RDK_COMPONENT_NAME=${RDK_COMPONENT_NAME-`basename $RDK_SOURCE_PATH`}
export DCA_PATH=$RDK_SOURCE_PATH

# parse arguments
INITIAL_ARGS=$@

export WORK_DIR=$RDK_PROJECT_ROOT_PATH/work${RDK_PLATFORM_DEVICE^^}
export TOOLCHAIN_ROOT_DIR=${RDK_TOOLCHAIN_PATH}
export INSTALL_DIR=$RDK_SOURCE_PATH/lib

function usage()
{
    set +x
    echo "Usage: `basename $0` [-h|--help] [-v|--verbose] [action]"
    echo "    -h    --help                  : this help"
    echo "    -v    --verbose               : verbose output"
    echo
    echo "Supported actions:"
    echo "      configure, clean, build (DEFAULT), rebuild, install"
}

ARGS=$@


# component-specific vars



# functional modules

function configure()
{
    true #use this function to perform any pre-build configuration
}

function clean()
{
    sh $DCA_PATH/build.sh clean
}

function build()
{

       cd .
       sh $DCA_PATH/build.sh build
       cp $DCA_PATH/dca $RDK_FSROOT_PATH/usr/bin/
       cp $DCA_PATH/dcacpu $RDK_FSROOT_PATH/usr/bin/
       cp $DCA_PATH/dcamem $RDK_FSROOT_PATH/usr/bin/
       cp $DCA_PATH/dcaprocess $RDK_FSROOT_PATH/usr/bin/
}


function rebuild()
{
    clean
    build
}

function install()
{
   echo "Calling Install" 
   cd $RDK_TARGET_PATH

    mkdir -p $RDK_FSROOT_PATH/usr/bin

    echo "Compiled"
    cd $DCA_PATH
    sh $DCA_PATH/build.sh install
    cp $DCA_PATH/dca $RDK_FSROOT_PATH/usr/bin/
    cp $DCA_PATH/dcacpu $RDK_FSROOT_PATH/usr/bin/
    cp $DCA_PATH/dcamem $RDK_FSROOT_PATH/usr/bin/
    cp $DCA_PATH/dcaprocess $RDK_FSROOT_PATH/usr/bin/
    echo "Copied"
    
}
# run the logic

#these args are what left untouched after parse_args
HIT=false

for i in "$ARGS"; do
    case $i in
        configure)  HIT=true; configure ;;
        clean)      HIT=true; clean ;;
        build)      HIT=true; build ;;
        rebuild)    HIT=true; rebuild ;;
        install)    HIT=true; install ;;
        *)
            #skip unknown
        ;;
    esac
done

# if not HIT do build by default
if ! $HIT; then
  build
fi

