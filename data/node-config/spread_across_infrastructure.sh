#!/bin/bash

list_of_hosts="$1"

if [ ! -f $list_of_hosts ]; then
    echo "File not found: $list_of_hosts"
    exit 0
fi

source send_to_host.sh
source command_to_host.sh

should_kill=1
should_send=1

ips=()
users=()
passwords=()
should_runs=()

while IFS= read -r line; do
    split_line=($line)
    ips+=(${split_line[0]})
    users+=(${split_line[1]})
    passwords+=(${split_line[2]})
    if [[ ${split_line[3]} == "run" ]]; then
        should_runs+=("run")
    else
        should_runs+=("no_run")
    fi
done < $list_of_hosts

index=0
for item in "${ips[@]}"; do
    ip=$item
    user=${users[index]}
    password=${passwords[index]}
    should_run=${should_runs[index]}
    index=$(( index + 1 ))
    if [[ $should_kill == 1 ]]; then  
        command_to_host $ip $user $password "kill -9 \$(ps -aux | grep VMTD | awk '{print \$2}')"
    fi
    if [[ $should_send == 1 ]]; then
        send_to_host $ip $user $password "./VMTD/VMTD" "/home/$user/VMTD/"
    fi
    if [[ $should_run == "run" ]]; then
        command_to_host $ip $user $password "cd VMTD; export DISPLAY=:0; nohup ./VMTD -tray </dev/null &>/dev/null &"
    fi
done
