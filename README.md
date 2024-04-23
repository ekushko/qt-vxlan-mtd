# Развертывание ПО на тестовом стенде в виртуальной инфраструктуре EVE-NG

## Подготовка инфраструктуры
1. [Cкачать](https://www.eve-ng.net/index.php/download/) и [установить](https://www.eve-ng.net/index.php/documentation/installation/virtual-machine-install/) EVE-NG.
2. [Установить](https://www.youtube.com/watch?v=9HTLqPaUNJ0) Cisco Nexus 9k в EVE-NG.
3. [Установить](https://www.eve-ng.net/index.php/documentation/howtos/howto-create-own-linux-host-image/) образ [Lubuntu](https://lubuntu.me/jammy-released/) или [Ubuntu](https://releases.ubuntu.com/jammy/) 22.04 в EVE-NG (Работа под другими версиями или другими дистрибутивами НЕ гарантируется! Теоретически возможна работа в других Ubuntu-based системах, в других может потребоваться доработка кода).
4. [Создать Lab](https://www.eve-ng.net/index.php/documentation/howtos-video/create-lab-and-connect-nodes-in-the-eve/).
5. Добавить Node со значениями Template=Linux, Image=<имя образа lubuntu/ubuntu 22.04> и Name/prefix=Node-1.
6. Добавить Network со значением Type=Management(Cloud0) и соединить с интерфейсом Node-1.
7. Запустить Node-1 и выполнить базовую настройку системы. В качестве имени пользователя выбрать *eve* и пароля *eve@123*.
8. После установки запустить терминал и выполнить команду: *git clone https://github.com/ekushko/qt-vxlan-mtd.git*.
9. Перейти в папку *~/qt-vxlan-mtd/data/node-config* и запустить скрипт *cofigure_workspace.sh* с правами суперпользователя.
10. После завершения выполнения скрипта остановить Node-1 и выполнить commit образа в EVE-NG в соответствии с [пунктами 11-17 инструкции](https://www.eve-ng.net/index.php/documentation/howtos/howto-create-own-linux-host-image/).

## Сборка собственной инфраструктуры
1. Собрать инфраструктуру, состоящую из коммутаторов Cisco Nexus 9k (Template=Cisco NX-OSv 9K, Image=<имя образа nxos9k> и Name/prefix=NXOS-<индекс>) и оконечных устройств на базе Ubuntu/Lubuntu 22.04 (Template=Linux, Image=<имя образа lubuntu/ubuntu 22.04> и Name/prefix=Node-<индекс>) путем добавления соответствующих Node. 
2. Проверить достижимость каждого узла с каждым при помощи ping.

## Сборка авторской инфраструктуры
1. to be continued...
