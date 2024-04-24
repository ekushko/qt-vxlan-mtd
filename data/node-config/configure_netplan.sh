#!/bin/bash

node_index=$1

if [ -z "$node_index" ]; then
    echo "Node index is NULL!"
    exit 0
fi

if [[ $EUID -ne 0 ]]; then
    echo "ROOT rights are required!"
    exit 0;
fi

cd /etc/netplan
touch "02-vmtd-1.yaml"
touch "03-vmtd-2.yaml"

chmod 777 *

main_netplan_file="01-network-manager-all.yaml"
> $main_netplan_file
cat <<EOT >> $main_netplan_file
network:
  version: 2
  ethernets:
    ens3:
      addresses:
        - 192.168.1.$node_index/24
      routes:
        - to: default
          via: 192.168.1.1
  vlans:
    vlan.19:
      id: 19
      link: ens3
      mtu: 1440
      addresses: [ "100.100.19.$node_index/24" ]
EOT

netplan apply

