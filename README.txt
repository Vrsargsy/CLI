			Instructions.

Server, which will receive commands from clients and send output to clients.
it should work with max 5 clients (you can change client count in SERVER/server.h -> MAX_CLIENTS 5)

Tested in macOS
	1) Open 2 or more bash terminals
	2) cd <CLONED PATH>
	2) make
	3) run server in one terminal ./server_ <PORT>  // Server ip 127.0.0.1 (localhost)
	4) run client in other terminal ./client_
		Client commands
			~ connect <IP> <PORT> to connect to server
			~ disconnect to disconnect from server
			~ to execute command in server and receive output write some command like <CLIENT> ls -la or <CLIENT> ps aux (many CLI-s have problem with buffer size)
																						ps. dont send commands which waiting input, its not parsed.

Tested in Linux (Ubuntu 22.04 LTS)

	if you haven't readline library please install it.
		sudo apt-get install libreadline-dev

	1) Open 2 or more bash terminals
	2) cd <CLONED PATH>
	2) make
	3) run server in one terminal ./server_ <PORT>  // Server ip 127.0.0.1 (localhost)
	4) run client in other terminal ./client_
		Client commands
			~ connect <IP> <PORT> to connect to server
			~ disconnect to disconnect from server
			~ to execute command in server and receive output write some command like <CLIENT> ls -la or <CLIENT> ps aux (many CLI-s have problem with buffer size)
																						ps. dont send commands which waiting input, its not parsed.

