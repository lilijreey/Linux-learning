#!/bin/bash - 
#===============================================================================
#
#          FILE: update_make.sh
# 
#         USAGE: ./update_make.sh 
#  ORGANIZATION: 更新项目make
#       CREATED: 11/07/2012 01:35:33 PM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

premake4 --cc=gcc --os=linux gmake
exit 0


