# Lab2CES35

This is the repository for the computer networks class assignment!
Available [here](https://github.com/sadrianorod/Lab2CES35).

## Instructions for compiling

Our project uses CMake as a build automation tool. We require version 3.10 or higher.
To compile the project, create a `build` folder to split source code from compilation related files, then execute `make`.
In sum, do the following:

```[bash]
mkdir build
cd build
cmake ..
make
``` 

Those commands will compile `web-server` and `web-client` and generate binary files into the `build` folder.
To generate only one of those binaries, just do `make web-server` or `make web-client`.

## Binaries

### Server

The web server serves pages and files through a TCP connection. Its invocation requires three arguments:
the server name, its port and the folder in which it will search for files. See the example below to start the program.

```[bash]
# ./web-server [host] [port] [folder]
./web-server 127.0.0.1 8080 ../folder
```  

### Client

The web client fetches files and pages through a TCP connection. To invoke it, pass as arguments the URLs you want to
fetch. There is an example below of how to start the client.

```[bash]
# ./web-client [URL] [URL]
./web-client www.google.com
```
