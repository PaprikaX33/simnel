# Simnel

Simnel (Simple Tunnel) is A simple TCP tunneling server-client pair written in pure C

The first iteration will be very simple, therefore it will not use a strong encryption method, rather than a simple XOR to 8bit keys determined with diffie helman handshake on connetion

## Overview

### Client

Client opens a listening port, and waits for connection **from** external program, and rely it to the server

#### Steps:

1. Client waits for incoming connect
1. Client connects the server
1. Verification handshake
1. Exchanging key with Diffie Helman
1. Client encrypts and decrypts the communication until one of them disconnected

### Server

Server opens a listening port, and waits for connection from client, and opens a connection **to** the external program.

#### Steps:

1. Server waits for incoming connection from client
1. Server connects to the external program
1. Verification handshake
1. Exchanging key with Diffie Helman
1. Server encrypt and deencrypt the communication until one of them disconnected

### Note

External program is either a TCP server that listens from incoming connection such as web server, IRC server, etc. Or a TCP client progam such as Browser, IRC client, etc.

### Other

The intent is having a simnel client program that waits for incoming connection from external program, and create a new connection for every incoming connection from the external program. The simnel server should be configured for the target server, and create a new connection for every connection from simnel client.

The simnel client and simnel server are responsible to encrypt it with the key

## License
This project is licensed with MIT License
