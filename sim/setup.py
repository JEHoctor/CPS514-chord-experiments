import subprocess as sp;

def build_proto():
	sp.run([
			'python3', 
			'-m',
			'grpc_tools.protoc',
			'--plugin=protoc-gen-grpc_python=/opt/grpc/bin/grpc_python_plugin',
			'-I=../protodefs', 
			'--python_out=.', 
			'--grpc_python_out=.', 
			'../protodefs/chord.proto'
	])

def runtime_envs():
	return { "DYLD_LIBRARY_PATH": ".:/opt/grpc/lib:/opt/protobuf/lib" }
	
def start_servers(ports):
	kill_servers_on(ports)

	for port in ports:
		sp.Popen(["../build/src/chord", "localhost", str(port)], env=runtime_envs())

def start_servers_gdb(ports):
	kill_servers_on(ports)

	for port in ports:
		p = sp.Popen(["gdb", "../build/src/chord"], env=runtime_envs())
		p.communicate("localhost", str(port))

def kill_servers_on(ports):
	for port in ports:
		pid = (sp
				.run(["lsof", "-i:{}".format(port), "-t"], capture_output=True, text=True)
				.stdout
				.strip()
		)

		pid and sp.run(["kill", "-TERM", pid])

