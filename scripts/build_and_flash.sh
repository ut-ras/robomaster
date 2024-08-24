#!/usr/bin/bash

# Build the source code and flash it to a robot via the dev container.
# This script is useful if you are unable to use the dev container during
# development; for example, if using an IDE that does not support them.
#
# Syntax: scripts/build_and_flash.sh [SSH destination] [scons arguments]

if [ ! -d "ut-robomaster" ]; then
    echo "ut-robomaster not found. Please run from the repository folder"
    exit 1
fi

run_cmd="
    ~/.local/bin/scons build ${@:2} &&
    cd .. &&
    scripts/connect_to_robot.sh $1 &&
    cd ut-robomaster &&
    ~/.local/bin/scons run ${@:2}
"

podman run -it --rm \
    --userns=keep-id:uid=1000,gid=1000 \
    --security-opt label=disable \
    -v .:/code \
    -w /code/ut-robomaster \
    ghcr.io/ut-ras/robomaster-firmware \
    sh -c "$run_cmd"