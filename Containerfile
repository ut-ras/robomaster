# Download ARM GCC
FROM ubuntu:22.04 as gcc-arm

ARG ARM_GCC_VERSION=10.3-2021.10

RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qq --no-install-recommends \
    bzip2 \
    ca-certificates \
    wget \
    && rm -rf /var/lib/apt/lists/*

RUN ARCH=$(uname -m) \
    URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/${ARM_GCC_VERSION}/gcc-arm-none-eabi-${ARM_GCC_VERSION}-${ARCH}-linux.tar.bz2" \
    && mkdir gcc-arm \
    && wget -qO- "$URL" | tar xj --strip-components=1 -C gcc-arm

# Main stage
FROM ubuntu:24.04

COPY --from=gcc-arm /gcc-arm /gcc-arm
ENV PATH="/gcc-arm/bin:$PATH"

RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qq --no-install-recommends \
    bash-completion \
    doxygen \
    git \
    libncurses6 \
    nano \
    openocd \
    openssh-client \
    python3-pip \
    python-is-python3 \
    sudo \
    && rm -rf /var/lib/apt/lists/*

# Create symlinks for old GCC compilers
RUN cd /usr/lib/x86_64-linux-gnu \
    && ln -s libncurses.so.6.4 libncurses.so.5 \
    && ln -s libtinfo.so.6.4 libtinfo.so.5

# Install tools (user-space)
USER ubuntu
# --break-system-packages is fine because we are not installing these through apt
RUN pip3 install lbuild pyelftools modm scons==3.1.2 --break-system-packages