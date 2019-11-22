import setup
import chord_client as cc
import time
import random

T = 100000

N = 5
ports = list(range(3000, 3000+N))

setup.build_proto()

setup.start_servers(ports)

cc.setup_channels(ports)

time.sleep(2)

print("Servers/Channels set up")

print("Have the servers join")

for port in ports:
	if port == 3000: continue
	cc.join(port, 3000)

(ids, infos) = cc.get_all_info()
#print(infos)

print("Running stabilizations")

for _ in range(0, 30):
	for port in ports:
		cc.stabilize(port)
		cc.fixFingers(port)

(ids, infos) = cc.get_all_info()

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

intervals = [ (ids[i], ids[(i+1)%N]) for i in range(0, N) ]

# it's left-open since we're looking for successors
def is_inside(interval, test):
	(l, r) = interval
	if(l <= r): return l < test and test <= r
	else: return not (r <= test and test < l)

print("Verifying query correctness")
for _ in range(0, T):
	key = random.randrange(0, 4294967296)
	print(key)
	succ_id = cc.find_succ(key).id
	for interval in intervals:
		if is_inside(interval, key):
			assert succ_id == interval[1]
			
		
