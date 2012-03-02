#!/bin/bash 

# Get directory from the command
user=$('whoami')
dir=$1

echo "$dir/conf"
NODE=($(awk '{print $3}' $dir/conf))
NUM=($(awk '{print $2}' $dir/conf))
node_size=${#NODE[*]}
node_size=$(expr $node_size - 1)

for n in $(seq 0 $node_size); do echo "Launch process on "${NODE[$n]}; ssh ${NODE[$n]} $dir/*.x ${NUM[$n]} > $dir/${NODE[$n]}-${NUM[$n]} & sleep 1; done
