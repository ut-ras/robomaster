#!/usr/bin/bash
echo "Attempting connection to [$1]..."
if ssh -fF ssh.config $1 sleep 15; then
    echo "Connected"
else
    echo "Connection failed"
    exit 1
fi