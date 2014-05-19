#!/bin/bash

# The knowledge for applying native border around window comes from: dscharrer
# http://steamcommunity.com/app/221410/discussions/0/882966056287209213/

# This script is based on the following one: https://gist.github.com/06d6b6a5370c4f6979f3
# Library it uses was build using the original script and is now bundled with the package.


export STEAMWM_FORCE_BORDERS=1
export STEAMWM_PREVENT_MOVE=1
export STEAMWM_FIX_NET_WM_NAME=0
export STEAMWM_GROUP_WINDOWS=1
export STEAMWM_SET_WINDOW_TYPE=1
export STEAMWM_SET_FIXED_SIZE=1
export STEAMWM_MANAGE_ERRORS=1

self="$(readlink -f "$(which "$0")")"

#export LD_PRELOAD="/usr/lib32/steamwm.so"
export LD_PRELOAD="$PWD/lib32/steamwm.so"
exec steam "$@"


