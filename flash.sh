#!/usr/bin/bash
if [ ! -d "ut-robomaster" ]; then
    echo "ut-robomaster not found. Please run from the repository folder"
    exit 1
fi

podman run -it --rm \
    --userns=keep-id:uid=1000,gid=1000 \
    --security-opt label=disable \
    -v /dev/bus/usb:/dev/bus/usb \
    -v .:/code \
    -w /code/ut-robomaster \
    ghcr.io/ut-ras/robomaster-firmware \
    scons run "$@"
