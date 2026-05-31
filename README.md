![krono](image.png)

# krono

A simple, lightweight web server built from scratch in C, designed to explore the depths of network programming.

## Overview

`krono` is a project born out of a desire to understand the inner workings of web servers and the networking stack. Built specifically for Windows using the Winsock2 library, it focuses on the fundamental concepts of socket programming.

## Features

- **Winsock2 Integration**: Optimized for Windows environments using the Winsock2 library.
- **TCP Socket Implementation**: Handles low-level socket creation and management.
- **HTTP/1.1 Support**: Serves basic HTTP responses to incoming browser requests.
- **Static Content**: Delivers a hardcoded HTML "Hello" page to clients.
- **Activity Logging**: Real-time console output for connection and request status.

## Prerequisites

- A C compiler (e.g., GCC/MinGW)
- Windows operating system

## Building and Running

To compile the server, link against the `ws2_32` library:

```bash
gcc webserver.c -o krono -lws2_32
```

To run the server:

```bash
./krono
```

Once running, open your browser and navigate to:
`http://localhost:8000`

You should see: **"Hello Bro, I am the krono server 😇"**

## Progress

- [x] Winsock initialization
- [x] Socket creation
- [x] Binding to local address
- [x] Passive listening state
- [x] Accepting connections
- [x] Receiving client requests
- [x] Sending HTTP responses
- [x] Connection lifecycle management (closesocket)
- [ ] Multi-threaded request handling
- [ ] Parsing HTTP headers (GET/POST)
- [ ] Serving files from disk (e.g., `index.html`)

## License

This project is for educational purposes.