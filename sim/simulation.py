import setup
import chord_client as cc
import time
import random

T = 1000

N = 20

S = 10

ports = list(range(3000, 3000+N))
joined_ports = []

setup.build_proto()

setup.start_servers(ports)

cc.setup_channels(ports)

time.sleep(2)

print("Servers/Channels set up")

print("Have the servers join")

for port in ports:
	joined_ports.append(port)
	if port == 3000: continue
	cc.join(port, 3000)
	print("joining " + str(port))
	for _ in range(0, S):
		futs = []
		for joined in joined_ports:
			futs.append(cc.stabilize(joined))
			futs.append(cc.fixFingers(joined))
		for fut in futs:
			fut.result() # wait for all the futures to complete

(ids, infos) = cc.get_all_info()
#print(infos)

print("Running stabilizations")


(ids, infos) = cc.get_all_info()
intervals = [ (ids[i], ids[(i+1)%N]) for i in range(0, N) ]

print(infos)

print("Verifying stabilization")
for info in infos:
	self = info.self
	pred = info.pred
	succ = info.succ

	selfIdx = ids.index(self.id)
	predIdx = ids.index(pred.id)
	succIdx = ids.index(succ.id)

	assert (predIdx+1)%N == selfIdx 
	assert (selfIdx+1)%N == succIdx 

# it's left-open since we're looking for successors
def is_inside(interval, test):
	(l, r) = interval
	if(l <= r): return l < test and test <= r
	else: return not (r <= test and test < l)

print("Verifying query correctness")
for t in range(0, T):
	key = random.randrange(0, 4294967296)
	succ_id = cc.find_succ(key).id
	for interval in intervals:
		if is_inside(interval, key):
			assert succ_id == interval[1]
		
print("Killing a node")
setup.kill_servers_on([ports[0]])
cc.removeChannel([ports[0]])
joined_ports.remove(ports[0])
N-=1

print("stabilizing")
for _ in range(0, S):
	futs = []
	for joined in joined_ports:
		futs.append(cc.stabilize(joined))
		futs.append(cc.fixFingers(joined))
	for fut in futs:
		fut.result() # wait for all the futures to complete

(ids, infos) = cc.get_all_info()
print(infos);
intervals = [ (ids[i], ids[(i+1)%N]) for i in range(0, N) ]

print("Verifying query correctness")
for t in range(0, T):
	key = random.randrange(0, 4294967296)
	succ_id = cc.find_succ(key).id
	for interval in intervals:
		if is_inside(interval, key):
			assert succ_id == interval[1]

