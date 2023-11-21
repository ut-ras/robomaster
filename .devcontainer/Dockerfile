FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y \
    gdb sudo openocd usbutils apt-utils dialog git wget curl vim \
    clang-format valgrind bash-completion python3 python3-pip scons doxygen \
    build-essential libboost-all-dev gcc g++ lcov gcovr libncurses5 \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install lbuild pyelftools modm

ARG ARM_GCC_VERSION=10.3-2021.10
RUN wget -q https://developer.arm.com/-/media/Files/downloads/gnu-rm/${ARM_GCC_VERSION}/gcc-arm-none-eabi-${ARM_GCC_VERSION}-x86_64-linux.tar.bz2 -O gcc-arm.tar.bz2 \
    && tar -xjf /gcc-arm.tar.bz2 \
    && rm gcc-arm.tar.bz2
ENV PATH="/gcc-arm-none-eabi-${ARM_GCC_VERSION}/bin:$PATH"

ARG USERNAME=dev
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Create non-root user
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /usr/bin/bash \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

# [Optional] Set the default user. Omit if you want to keep the default as root.
USER $USERNAME

ENV DEBIAN_FRONTEND=dialog
