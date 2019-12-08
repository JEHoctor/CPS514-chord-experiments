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

def start_chord_server_on(host, port_number=chord_port, buddy_ip_address=None, buddy_port=chord_port):
    ip_address = host.cmd('sudo ifconfig | grep inet | head -n1 | cut -d \':\' -f 2 | cut -d \' \' -f 1').strip()
    if buddy_ip_address is None:
        command = '../build/src/chord server '+ip_address+':'+str(port_number)+' &'
        print command
        output = host.cmd(command)
    else:
        command = '../build/src/chord server '+ip_address+':'+str(port_number)+' -t '+buddy_ip_address+':'+str(buddy_port)+' &'
        print command
        output = host.cmd(command)
    return ip_address, port_number


def kill_chord_server_on(host):
    host.cmd('sudo pkill -9 -f \'chord server\'')


def run_chord_client_on(host, server_ip, rest_of_command, port_number=chord_port):
    command = '../build/src/chord client '+server_ip+':'+str(port_number)+' '+rest_of_command
    print command
    output = host.cmd(command)
    return output


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

    h = random.choice(hosts)
    ip_addr, port = host_to_ip_port[h]
    print run_chord_client_on(h, ip_addr, 'get-info')


if __name__ == '__main__':
    main()
