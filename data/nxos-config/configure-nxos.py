import requests
import json
import copy

request_header = { "content-type" : "application/json-rpc" }

request_template = {
    "jsonrpc": "2.0",
    "method": "cli_ascii",
    "params": {
        "cmd": "",
        "version": 1
    },
    "id": 0
}

def run_command(payload, command):
    request_template["params"]["cmd"] = command
    request_template["id"] += 1
    payload.append(copy.deepcopy(request_template))

def configure_vlans(payload, beg_vlan_id, end_vlan_id):
    for i in range(beg_vlan_id, end_vlan_id + 1):
        run_command(payload, "vlan " + str(i))
        run_command(payload, "  vn-segment " + str(i * 100))

def configure_interfaces(payload, beg_index, end_index):
    for i in range(beg_index, end_index + 1):
        run_command(payload, "interface e1/" + str(i))
        run_command(payload, "  switchport")
        run_command(payload, "  switchport mode trunk")
        run_command(payload, "  switchport trunk allowed vlan 17-19")
        run_command(payload, "  no shutdown")

def configure_nve_interface(payload, beg_vlan_id, end_vlan_id, peer_ip):
    run_command(payload, "interface nve 1")
    run_command(payload, "  no shutdown")
    run_command(payload, "  source-interface loopback 0")
    for i in range(beg_vlan_id, end_vlan_id + 1):
        run_command(payload, "  member vni " + str(i * 100))
        run_command(payload, "   ingress-replication protocol static")
        run_command(payload, "     peer-ip " + peer_ip)

def configure_nxos(switchuser, switchpassword, url, peer_ip):
    payload = []
    request_template["id"] = 0
    
    run_command(payload, "conf t")
    configure_vlans(payload, 17, 50)
    configure_interfaces(payload, 2, 7)
    configure_nve_interface(payload, 17, 50, peer_ip)

    run_command(payload, "copy running-config startup-config")

    response = requests.post(url, data=json.dumps(payload), headers=request_header, auth=(switchuser,switchpassword), verify=False).json()

configure_nxos("admin", "admin", "https://192.168.132.98/ins", "20.20.20.20")
configure_nxos("admin", "admin", "https://192.168.132.99/ins", "10.10.10.10")

