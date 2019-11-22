import grpc
import random

import chord_pb2
import chord_pb2_grpc

channels = {}

def getRandomChannel():
	return random.choice(list(channels.values()))

def getChannel(port):
	return channels[str(port)]

def makeNodeObj(port):
	return chord_pb2.Node(addr='localhost:{}'.format(port), is_valid=True)

def join(port_to_join, port_existing): 
	stub = chord_pb2_grpc.ChordStub(getChannel(port_to_join))
	stub.join(chord_pb2.JoinReq(buddy=makeNodeObj(port_existing)))

def get_all_info():
	infos = [get_info(port).info for port in channels.keys()]
	ids = [info.self.id for info in infos]
	return (sorted(ids), infos)

def get_info(port):
	stub = chord_pb2_grpc.ChordStub(getChannel(port))
	return stub.getInfo(chord_pb2.GetInfoReq())

def get_closest_finger(key, port):
	stub = chord_pb2_grpc.ChordStub(getChannel(port))
	return stub.closestPredFinger(chord_pb2.ClosestPredFingerReq(key=key))

def find_pred(key):
	stub = chord_pb2_grpc.ChordStub(getRandomChannel())
	return stub.findPred(chord_pb2.FindPredReq(key=key))

def find_succ(key):
	stub = chord_pb2_grpc.ChordStub(getRandomChannel())
	return stub.findSucc(chord_pb2.FindSuccReq(key=key)).succ

def stabilize(port):
	stub = chord_pb2_grpc.ChordStub(getRandomChannel())
	return stub.stabilize(chord_pb2.StabilizeReq())

def fixFingers(port):
	stub = chord_pb2_grpc.ChordStub(getRandomChannel())
	return stub.fixFingers(chord_pb2.FixFingersReq())

def setup_channels(ports):
	global channels
	channels = { 
		str(port): grpc.insecure_channel('localhost:{}'.format(port))
		for port in ports 
	}

def close_channels():
	global channels
	for ch in channels.values(): ch.close()

def print_channels():
	global channels
	print(channels)
