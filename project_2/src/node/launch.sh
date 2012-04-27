#!/bin/bash 

# Get directory from the command
user=$('whoami')
dir=$1
file=$2
conf_file=$1/$2
#echo "$dir/conf"
echo "$conf_file"
NODE=($(awk '{print $3}' $conf_file))
NUM=($(awk '{print $2}' $conf_file))
node_size=${#NODE[*]}
node_size=$(expr $node_size - 1)

# redirect to different log files
for n in $(seq 0 $node_size); do
    echo "Launch "${NUM[$n]}" process on "${NODE[$n]}
    ssh ${NODE[$n]} $dir/main.x $conf_file ${NUM[$n]} 2>&1 > $dir/log/${NUM[$n]}-${NODE[$n]}-log &
    sleep 1;
done

# redirect to the same file (should be in the same nfs directory) 
#for n in $(seq 0 $node_size); do echo "Launch process on "${NODE[$n]}; ssh ${NODE[$n]} $dir/*.x $conf_file ${NUM[$n]} > ~/trace.log & sleep 1; done
