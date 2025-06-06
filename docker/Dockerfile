FROM ubuntu:latest

# Install required dependencies for compiling C++ executables
RUN apt update
RUN apt install -y g++ cmake make libssl-dev gdb gdbserver
RUN apt install -y libc6-dev gcc libtool automake autoconf build-essential
RUN apt install -y libncurses-dev
RUN apt install -y git

# Install requirements for wide char compatibility
RUN apt install -y locales
RUN locale-gen en_US.UTF-8
RUN update-locale LANG=en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8