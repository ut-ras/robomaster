FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y gdb sudo \
    # vscode-recommended tools
    && apt-get install -y --no-install-recommends apt-utils dialog 2>&1 \
    # misc convenience tools; wget for toolchain download below
    && apt-get install -y --no-install-recommends git wget curl vim gdb clang-format-10 valgrind bash-completion \
    # Python and scons
    && apt-get install -y --no-install-recommends python3 python3-pip scons \
    # documentation generation
    && apt-get install -y --no-install-recommends doxygen \
    # compiler and libs for building tests targeting Linux
    && apt-get install -y build-essential libboost-all-dev gcc-10 g++-10 lcov gcovr \
    && rm -rf /var/lib/apt/lists/*

# Update gcc and g++ symbolic links to point to version 10
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 90 --slave /usr/bin/g++ g++ /usr/bin/g++-10 --slave /usr/bin/gcov gcov /usr/bin/gcov-10
RUN pip3 install lbuild pyelftools modm

# "install" the gcc ARM cross-compiler
RUN wget -q https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2 -O gcc-arm.tar.bz2 \
    && tar -xjf /gcc-arm.tar.bz2 \
    && rm gcc-arm.tar.bz2
ENV PATH="/gcc-arm-none-eabi-10.3-2021.10/bin:$PATH"

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
