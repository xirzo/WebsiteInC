# C-Website 🖥️

[![C Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CMake](https://img.shields.io/badge/Build-CMake-green.svg)](https://cmake.org/)

## Overview 💡

This project aims to create a fully functional website using the C programming language.

## Features ✨

- **Pure C Implementation**: Built entirely in C for maximum performance and minimal resource usage
- **File-Based Routing**: Serves content based on requested URI paths

## How It Works 🛠️

The server listens on port 5000 by default and processes incoming HTTP requests. When a client connects, the server:

1. Reads the HTTP request
2. Parses the URI path from the request
3. Maps the URI to a corresponding file on the filesystem
4. Serves the file content with appropriate HTTP headers

For example, requesting `/index.html` will serve the content of the `index.html` file from the server's root directory.

## Installation 📦

### Prerequisites

- C compiler (GCC or Clang recommended)
- CMake (3.10 or higher)
- http_parser library

### Building

```bash
mkdir build
cd build
cmake ..
make
```

## Usage 🚀

Run the compiled executable to start the server:

```bash
./c-website
```

The server will start listening on port 5000. You can then access it by navigating to `http://localhost:5000` in your web browser.
