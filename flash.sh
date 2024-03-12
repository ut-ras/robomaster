#!/usr/bin/bash
if [ ! -f "Containerfile" ]; then
    echo "Containerfile not found. Please run from the repository folder"
    exit 1
fi

podman build --tag "ut-robomaster" .
podman run -it --rm \
    --userns=keep-id \
    --security-opt label=disable \
    -v /dev/bus/usb:/dev/bus/usb \
    -v .:/robomaster \
    -w /robomaster/ut-robomaster \
    ut-robomaster \
    scons run "$@"
