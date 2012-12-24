#!/bin/bash - 
#===============================================================================
#
#          FILE:  show-two-result.sh
# 
#         USAGE:  ./show-two-result.sh 
# 
#   DESCRIPTION:  
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: lili, zhao (), lilijreeyGamil.com
#       COMPANY: 
#       CREATED: 12/18/2011 09:50:01 PM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

declare -a reslut
i=0
while (( $i < 100 ))
do
	reslut[$i]=`grep -c ":$i " xx`
	let i+=1
done


i=0 ;
while ((  $i < ${#reslut[*]} ))
do
	echo -e "$i: ${reslut[$i]}"
	let i+=1
done
