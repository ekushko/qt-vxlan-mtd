#!/bin/bash

function command_to_host {
    ip="$1"
    user="$2"
    password="$3"
    cmd=$4

    sshpass -p $password ssh -o StrictHostKeyChecking=no $user@$ip $cmd
    echo "Command \"$cmd\" to $ip executed"

    return 0
}

