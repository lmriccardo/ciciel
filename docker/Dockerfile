FROM ubuntu:latest

# Install required dependencies for compiling C++ executables
RUN apt update
RUN apt install -y g++ cmake make libssl-dev gdb gdbserver
RUN apt install -y libc6-dev gcc libtool automake autoconf build-essential
RUN apt install -y libncurses-dev