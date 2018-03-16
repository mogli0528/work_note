#!/usr/bin/env bash  
set -e  
  
folder="models"
if [ ! -d "${folder}" ]; then 
  mkdir -p ${folder}
fi

log_dir="log/"  
if [ ! -d "${log_dir}" ]; then 
  mkdir -p ${log_dir}
fi
  
TOOLS=/home/klm/work/gitwork/ssd/build/tools  
  
${TOOLS}/caffe train \
--solver=solver.prototxt 2>&1 | tee -a ${log_dir}/train.log
