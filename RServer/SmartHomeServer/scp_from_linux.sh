#!/bin/bash


#FILES=src/ inc/ lib/ Makefile

if [ $# -eq 2 ];then
scp -r root@$1:$2 .
else
echo Usage: $0 ip_addr dest_dir
fi