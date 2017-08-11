#!/bin/sh
 
##########################################################
# Script Name : log_clean.sh
#
# System      : 
# Purpose     : apache
##########################################################

PATH=/usr/local/bin:/usr/bin:/bin;export PATH

T=${1-"exec"}

#-
#- Initial temporary files
#-
BASE=/root/tools/conf
CONF=${BASE}/log_clean.conf

#-
#- Get remove logs list
#-
exec_find()
{
	DIR=$1
	DAY=$2
	NM=$3
	find ${DIR}/ -name "${NM}" -mtime +${DAY} -ls
}

#-
#- gzip or remove files listed in input file.
#-
exec_cmd()
{
	CMD=$1
	FL=$2
	cat ${FL} | while read LOG; do
		if [ ${T} = "test" ]; then
			echo "${CMD} ${LOG}"
		else
			#echo "EXEC...."
			${CMD} ${LOG}
		fi
	done
}

#-
#- Main
#-
egrep -v "^#" ${CONF} | awk '{
	if ( NF != 4 ) next;
	n=split($4,a,",");
	for (i=1;i<=n;i++){
		print $1,$2,$3,a[i];
	}
	}' | \
	while read LOG_DIR GZ_DAYS RM_DAYS LOG; do

	TMP_GZ_LIST=/tmp/log_clean.gzlist
	TMP_RM_LIST=/tmp/log_clean.rmlist
	#
	exec_find ${LOG_DIR} ${GZ_DAYS} ${LOG}*[0-9] | \
		awk '{print $NF}' > ${TMP_GZ_LIST}
	#
	exec_find ${LOG_DIR} ${RM_DAYS} ${LOG}*.gz  | \
		awk '{print $NF}' > ${TMP_RM_LIST}
	#
	exec_cmd gzip ${TMP_GZ_LIST}
	#
	exec_cmd rm ${TMP_RM_LIST}
done
