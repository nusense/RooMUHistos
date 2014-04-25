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

if [ $ARCHIT = 'Darwin' ]
then
  export DYLD_LIBRARY_PATH=`pwd`/lib:$DYLD_LIBRARY_PATH
  export DYLD_LIBRARY_PATH=`pwd`/lib:$ROOTSYS/lib
else
  export LD_LIBRARY_PATH=`pwd`/lib:$LD_LIBRARY_PATH
  export LD_LIBRARY_PATH=`pwd`/lib:$ROOTSYS/lib
fi

unset ARCHIT


