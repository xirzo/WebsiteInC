# 🖥️C-Website

[![C Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CMake](https://img.shields.io/badge/Build-CMake-green.svg)](https://cmake.org/)

> [!WARNING]  
> This project was created for educational purposes and demonstration only.
> The code is not production-grade and may have significant limitations, bugs, or security issues.  
> **Do not use this :)**

This project aims to create a fully functional website using the C programming language.

![image](https://github.com/user-attachments/assets/f0fff757-9d7b-452c-851d-24035820b5bd)

## ✨ Features

- **Pure C Implementation**: Built using standard C libraries and minimal dependencies
- **Simple HTTP Server**: Handles basic HTTP requests and serves static content
- **Routing System**: Maps URLs to file resources

## 🛠 Requirements

- ⚙️ GCC compiler (clang won`t work)
- 🛠 CMake (version 3.10 or higher)
- 📦 [Http Parser Library](https://github.com/xirzo/httpparser)

## 🔧 Building the Project

```bash
# Clone the repository
git clone https://github.com/xirzo/C-Website.git
cd C-Website

# Create build directory
mkdir build
cd build

# Configure and build
cmake .. --preset=gcc
make
```

## 🚀 Usage

After building, set the port env and run the server:

```bash
export port=5000
```

```bash
./c_website
```

The server will start on port 5000 by default. You can access the website at `http://localhost:5000`.

## ⚙️ How It Works

The router maps the requested URI to a file. Then the server uses a simple HTTP parser to process incoming requests. When a request comes in, the router provides a file path then server reads the file and sends it back as an HTTP response.

Current routes:

- `/` → `index.html`
- `/style.css` → `style.css`
