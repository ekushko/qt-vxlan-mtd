#!/bin/bash

# Проверка прав суперпользователя

echo -e "\n\n---------CHECK ROOT ACCESS---------\n\n"

if [[ $EUID -ne 0 ]]; then
    echo "Root rights not found!"
    exit 0;
fi

echo "Root rights found!"

# Настройка прав для netplan

utils="GROUP_EVE ALL=(ALL) NOPASSWD: /bin/ip, /sbin/ifconfig, /sbin/netplan"
utils_users="User_Alias GROUP_EVE = eve"
utils_file="/etc/sudoers"
echo $utils_users >> $utils_file
echo $utils >> $utils_file

chown -R eve /etc/netplan/
chmod -R 777 /etc/netplan/

# Обновление пакетов

echo -e "\n\n------APT UPDATE AND UPGRADE-------\n\n"

apt-get update && apt-get upgrade -y 

# Установка SSH

echo -e "\n\n------------SSH ENABLE-------------\n\n"

apt-get install -y ssh sshpass

ufw allow ssh

systemctl enable ssh
systemctl start ssh

# Установка пакетов для работы с сетью

echo -e "\n\n--------INSTALL NET TOOLS----------\n\n"

apt-get install -y net-tools inetutils-traceroute iperf vlan wireshark nmap

echo "net.ipv4.ip_forward=1" >> /etc/sysctl.conf
sysctl -w net.ipv4.ip_forward=1

# Установка Qt-библиотек

echo -e "\n\n-------INSTALL QT LIBRARIES--------\n\n"

apt-get install -y make cmake gcc g++ qtbase5-dev qt5-qmake libqt5websockets5 libqt5websockets5-dev

# Скачивание проекта из репозитория

echo -e "\n\n-----------INSTALL VMTD------------\n\n"

cd ../../

qmake VMTD.pro && make

cp -R bin/VMTD ../

chown -R eve:eve ../VMTD

# Установка DAQ

echo -e "\n\n-----------INSTALL DAQ-------------\n\n"

apt-get install -y build-essential libpcre3-dev zlib1g-dev libluajit-5.1-dev libpcap-dev openssl libssl-dev libnfnetlink-dev libmnl-dev libnet1-dev libnetfilter-queue-dev libnghttp2-dev bison flex libdumbnet-dev liblzma-dev libntirpc-dev autotools-dev checkinstall cpputest autoconf libtool pkg-config

cp -R /usr/include/ntirpc/* /usr/include/rpc/
cp -R /usr/include/ntirpc/rpc/* /usr/include/rpc/
cp -R /usr/include/ntirpc/misc/* /usr/include/misc/
cp /usr/include/ntirpc/netconfig.h /usr/include/
cp /usr/include/ntirpc/intrinsic.h /usr/include/
cp /usr/include/ntirpc/reentrant.h /usr/include/

cd data/daq-2.0.7

chmod +x configure

autoreconf -f -i

./configure && make && make install

# Установка SNORT

echo -e "\n\n----------INSTALL SNORT------------\n\n"

cd ../snort-2.9.20

chmod +x configure

./configure --enable-sourcefire --disable-open-appid && make && make install

ldconfig

ln -s /usr/local/bin/snort /usr/sbin/snort

groupadd snort
useradd snort -r -s /sbin/nologin -c SNORT_IDS -g snort

mkdir /etc/snort
mkdir /usr/local/lib/snort_dynamicrules

cp -R configuration/* /etc/snort/

mkdir /var/log/snort
mkdir /var/log/snort/archived_logs

chmod -R 5775 /etc/snort
chmod -R 5775 /var/log/snort
chmod -R 5775 /var/log/snort/archived_logs
chmod -R 5775 /etc/snort/so_rules
chmod -R 5775 /usr/local/lib/snort_dynamicrules

chown -R snort:snort /etc/snort
chown -R snort:snort /var/log/snort
chown -R snort:snort /usr/local/lib/snort_dynamicrules

snort_service_file="/lib/systemd/system/snort.service"
touch $snort_service_file

cat <<EOT >> $snort_service_file
[Unit]
Description=Snort NIDS Daemon
After=syslog.target network.target

[Service]
Type=simple
ExecStart=/usr/local/bin/snort -q -u snort -g snort -c /etc/snort/snort.conf -i ens3

[Install]
WantedBy=multi-user.target
EOT

systemctl enable snort
systemctl start snort

echo -e "\n\n-------FINISHED SUCCESSFULLY-------\n\n"
