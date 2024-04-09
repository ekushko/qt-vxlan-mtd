#!/bin/bash

function send_to_host {
    ip="$1"
    user="$2"
    password="$3"
    filepath="$4"
    distpath="$5"

    if [ -d $filepath ]; then
        sshpass -p $password scp -r $filepath $user@$ip:$distpath
        echo "File sent: $filepath to $user@$ip:$distpath"
    elif [ -f $filepath ]; then 
        sshpass -p $password scp $filepath $user@$ip:$distpath
        echo "File sent: $filepath to $user@$ip:$distpath"
    else
        echo "Not found: $filepath"
    fi

    return 0
}


