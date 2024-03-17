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
    && wget -qO- "$URL" | tar xj \
    && mv gcc-arm-none-eabi-${ARM_GCC_VERSION} gcc-arm

# Download J-Link Software
FROM ubuntu:22.04 as jlink
RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qq --no-install-recommends \
    ca-certificates \
    wget \
    && rm -rf /var/lib/apt/lists/*

RUN POSTDATA="accept_license_agreement=accepted&submit=Download+software" \
    URL="https://www.segger.com/downloads/jlink/JLink_Linux_x86_64.tgz" \
    && wget -qO- --post-data "$POSTDATA" "$URL" | tar xz \
    && mv JLink_Linux_* jlink

# Main stage
FROM ubuntu:22.04

ARG USERNAME=dev
ARG USER_UID=1000
ARG USER_GID=$USER_UID

COPY --from=gcc-arm /gcc-arm /gcc-arm
ENV PATH="/gcc-arm/bin:$PATH"

COPY --from=jlink /jlink /jlink
ENV PATH="/jlink:$PATH"

RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qq --no-install-recommends \
    bash-completion \
    doxygen \
    git \
    libncurses5 \
    nano \
    openocd \
    openssh-client \
    python3-pip \
    scons \
    sudo \
    && rm -rf /var/lib/apt/lists/*

# Setup non-root user
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /usr/bin/bash \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME
WORKDIR /home/$USERNAME
USER $USERNAME

# Install tools (user-space)
RUN pip3 install lbuild pyelftools modm
