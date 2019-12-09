# some code is from https://bitbucket.org/huangty/cs144_bufferbloat/src/master/bufferbloat.py
# This must be run in the sim/ directory.

from mininet.topo import Topo
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.net import Mininet
from mininet.log import lg, info
from mininet.util import dumpNodeConnections
from mininet.cli import CLI

import time
import random
from pprint import pprint


def get_node_name(i):
    return 'h%d' % (i+1)


# modified from bufferbloat version
class StarTopo(Topo):
    "Star topology"

    def __init__(self, n=2, cpu=None, bw_host=1000, bw_net=1.5,
                 delay=10, diff=False):
        # Add default members to class.
        super(StarTopo, self ).__init__()

        # Create switch and host nodes
        for i in xrange(n):
            self.addHost( 'h%d' % (i+1), cpu=cpu )

        self.addSwitch('s0', fail_mode='open')

        for i in xrange(0, n):
            self.addLink('h%d' % (i+1), 's0', bw=bw_host)


chord_port = 9001

def start_chord_server_on(host, port_number=chord_port, buddy_ip_address=None, buddy_port=chord_port, print_command=False):
    ip_address = host.cmd('sudo ifconfig | grep inet | head -n1 | cut -d \':\' -f 2 | cut -d \' \' -f 1').strip()
    if buddy_ip_address is None:
        command = '../build/src/chord server '+ip_address+':'+str(port_number)+' >/dev/null &'
    else:
        command = '../build/src/chord server '+ip_address+':'+str(port_number)+' -t '+buddy_ip_address+':'+str(buddy_port)+' >/dev/null &'
    if print_command:
        print command
    host.cmd(command)
    return ip_address, port_number


def kill_chord_server_on(host, print_command=False):
    command = 'sudo pkill -9 -f \'chord server\''
    if print_command:
        print command
    host.cmd(command)


def run_chord_client_on(host, server_ip, rest_of_command, port_number=chord_port, print_command=False):
    command = '../build/src/chord client '+server_ip+':'+str(port_number)+' '+rest_of_command
    if print_command:
        print command
    output = host.cmd(command)
    return output


def get_info(host, ip_addr, port):
    output = run_chord_client_on(host, ip_addr, 'get-info', port_number=port)
    lines = [l for l in output.split('\r\n') if l != '']
    if lines == ['Failed to get the status of that chord server.']:
        return None # failure condition, must check for this
    ret = {}
    if '1' in lines[3]:
        addr, port = lines[1][12:].split(':')
        ret['self'] = {'addr': addr, 'port': int(port), 'ID': int(lines[2][12:])}
    else:
        ret['self'] = None
    if '1' in lines[7]:
        addr, port = lines[5][12:].split(':')
        ret['succ'] = {'addr': addr, 'port': int(port), 'ID': int(lines[6][12:])}
    else:
        ret['succ'] = None
    if '1' in lines[11]:
        addr, port = lines[9][12:].split(':')
        ret['pred'] = {'addr': addr, 'port': int(port), 'ID': int(lines[10][12:])}
    else:
        ret['pred'] = None
    return ret


def sample_topology(hosts):
    pass


def main(n_hosts = 10):
    topo = StarTopo(n=n_hosts, bw_host=1000,
                    delay='%sms' % 50)
    net = Mininet(topo=topo, host=CPULimitedHost, link=TCLink,
                  autoPinCpus=True)
    net.start()
    dumpNodeConnections(net.hosts)
    net.pingAll()

    hosts = [net.getNodeByName(get_node_name(i)) for i in xrange(n_hosts)]

    host_to_ip_port = {}
    buddy_ip_address=None
    for h in hosts:
        if host_to_ip_port:
            buddy_ip_address, _ = random.choice(host_to_ip_port.values())
        ip_address, port_number = start_chord_server_on(h, buddy_ip_address=buddy_ip_address)
        host_to_ip_port[h] = (ip_address, port_number)

    time.sleep(0.5) # give the servers some time before they get bombarded with RPCs

    for h, (ip_addr, port) in host_to_ip_port.items():
        pprint(get_info(h, ip_addr, port))
    print('*****')
    for i in xrange(5):
        for h, (ip_addr, port) in host_to_ip_port.items():
            run_chord_client_on(h, ip_addr, 'stabilize')
    print('*****')
    for h, (ip_addr, port) in host_to_ip_port.items():
        pprint(get_info(h, ip_addr, port))


if __name__ == '__main__':
    main()
