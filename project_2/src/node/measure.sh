#!/bin/bash

# Assuming following format
# TIME NODE# EEBL EEBL_REB BEACON HELLO
#  $1   $2    $3     $4      $5     $6

eebl=$(cat log/m* | awk '{print $3}' | paste -sd+ | bc)
eebl_reb=$(cat log/m* | awk '{print $4}' | paste -sd+ | bc)
beacon=$(cat log/m* | awk '{print $5}' | paste -sd+ | bc)
hello=$(cat log/m* | awk '{print $6}' | paste -sd+ | bc)
ttime=$(echo "("$(cat log/m* | awk '{print $1}' | paste -sd+)")/"$(cat log/m* | wc | awk '{print $1}') | bc)

echo "Up time average by all nodes: "$ttime
echo "Total number of EEBLs sent: "$eebl
echo "Total number of EEBLs rebroacasted: "$eebl_reb
echo "Total number of BEACONs sent: "$beacon
echo "Total number of HELLOs sent: "$hello
