# krono

A simple, lightweight web server built from scratch in C, designed to explore the depths of network programming.

## Overview

`krono` is a project born out of a desire to understand the inner workings of web servers and the networking stack. Built specifically for Windows using the Winsock2 library, it focuses on the fundamental concepts of socket programming.

## Features

- **Winsock2 Integration**: Built for Windows environments.
- **TCP Socket Implementation**: Handles low-level socket creation and management.
- **Connection Handling**: Implements basic binding, listening, and accepting of incoming connections.

## Prerequisites

- A C compiler (e.g., GCC/MinGW)
- Windows operating system

## Building and Running

To compile the server, you need to link against the `ws2_32` library:

```bash
gcc webserver.c -o krono -lws2_32
```

To run the server:

```bash
./krono
```
or

```bash
krono
```

The server currently listens on port `8000` by default.

## Progress

- [x] Winsock initialization
- [x] Socket creation
- [x] Binding to local address
- [x] Passive listening state
- [x] Accepting connections

## License

This project is for educational purposes.