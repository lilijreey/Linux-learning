#!/bin/bash - 
#===============================================================================
#
#          FILE:  install_zhao_include.sh
# 
#         USAGE:  ./install_zhao_include.sh 
# 
#   DESCRIPTION:  install zhao_include symble /usr/include link ./zhao_include
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: YOUR NAME (), 
#       COMPANY: 
#       CREATED: 09/18/2011 09:58:37 PM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error
cp -r ./zhao /usr/local/include/
exit 0 ;

