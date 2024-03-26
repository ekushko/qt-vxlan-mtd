#!/bin/bash

function send_to_host {
    ip="$1"
    user="$2"
    password="$3"
    filepath="$4"

    if [ -d $filepath ]; then
        sshpass -p $password scp -r $filepath $user@$ip:/home/$user
        echo "File sent: $filepath to $ip"
    elif [ -f $filepath ]; then 
        sshpass -p $password scp $filepath $user@$ip:/home/$user
        echo "File sent: $filepath to $ip"
    else
        echo "Not found: $filepath"
    fi

    return 0
}


