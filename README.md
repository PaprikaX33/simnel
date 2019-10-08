# Simnel

A simple TCP tunneling server-client pair written in pure C

The first iteration will be very simple, therefore it will not use a strong encryption method, rather than a simple XOR to 8bit keys determined with diffie helman handshake on connetion

## Overview

### Server

The server is a program that listen from connection from simnel client, and once connected, it will try to initiate a diffie helman handshake, and then try to connect to the determined public server

### Client

The client is a program that wait for incoming conection in a TCP socket, and create a new connection to simnel server for every incoming connection, it will wait for a diffie helman handshake. Once connected, the client will act as a normal public server

### Other

The intent is having a simnel client program that waits for incoming connection from external program, and create a new connection for every incoming connection from the external program. The simnel server should be configured for the target server, and create a new connection for every connection from simnel client.

The simnel client and simnel server are responsible to encrypt it with the key

## License
This project is licensed with WTFPL
