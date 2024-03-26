#!/bin/bash

ip="$1"
user="$2"
password="$3"

source send_to_host.sh
source command_to_host.sh

send_to_host $ip $user $password "../../bin/VMTD"
send_to_host $ip $user $password "send_to_host.sh"
send_to_host $ip $user $password "command_to_host.sh"
send_to_host $ip $user $password "spread_across_infrastructure.sh"
command_to_host $ip $user $password "chmod 777 send_to_host.sh command_to_host.sh spread_across_infrastructure.sh"
command_to_host $ip $user $password "chmod +x send_to_host.sh command_to_host.sh spread_across_infrastructure.sh"
command_to_host $ip $user $password "./spread_across_infrastructure.sh list_of_hosts.txt"

