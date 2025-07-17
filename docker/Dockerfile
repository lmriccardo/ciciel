FROM ubuntu:latest

# Install required dependencies for compiling C++ executables
RUN apt update && apt install -y \
    g++ cmake make libssl-dev gdb gdbserver git \
    libc6-dev gcc libtool automake autoconf \ 
    build-essential libncurses-dev

# Install requirements for wide char compatibility
RUN apt install -y locales
RUN locale-gen en_US.UTF-8
RUN update-locale LANG=en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8
