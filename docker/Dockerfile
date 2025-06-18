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


RUN apt install -y libgtest-dev
RUN cd /usr/src/gtest && \
    cmake . && \
    make -j$(nproc) && \
    mkdir -p /usr/local/lib && \
    cp lib/libgtest.a /usr/local/lib/ && \
    cp lib/libgtest_main.a /usr/local/lib/ && \
    cp -r include/gtest /usr/local/include/
