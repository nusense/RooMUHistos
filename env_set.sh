#!/bin/sh


ARCHIT=`uname`

# Check to see if ROOTSYS is set up, and if it isn't, source a generic script. 
if [ -z $ROOTSYS ]; then
  if [ -f $HOME/setup_root ]; then
    source $HOME/setup_root
  else
    echo "Need to set up the ROOTSYS environment variable."
    return 0
  fi
fi

# If there really is no ROOTSYS at this point, bail.
if [ -z $ROOTSYS ]; then
  echo "Need to set up the ROOTSYS environment variable."
  return 0
fi

# Self locate script when sourced
if [ -z "$BASH_VERSION" ]; then
  if [ ! -f env_set.sh ]; then
    echo "ERROR: Could not find absolute path of RooMu."
    echo "       Your shell may not support this."
    echo "       Instead you should cd to the directory of this script, then source it."
    exit 1
  fi
  export ROOMU_SYS=$(pwd)
else
  export ROOMU_SYS=$(cd $(dirname $BASH_SOURCE[0]);pwd)
fi

if [ $ARCHIT = 'Darwin' ]
then
  export DYLD_LIBRARY_PATH=$ROOMU_SYS/lib:$DYLD_LIBRARY_PATH
  export DYLD_LIBRARY_PATH=$ROOTSYS/lib:$DYLD_LIBRARY_PATH
else
  export LD_LIBRARY_PATH=$ROOMU_SYS/lib:$LD_LIBRARY_PATH
  export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
fi
  
export PATH=$ROOMU_SYS/bin:$PATH

unset ARCHIT


