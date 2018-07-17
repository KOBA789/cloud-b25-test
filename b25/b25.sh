#!/bin/sh

socat TCP-LISTEN:${PORT:-5225},fork,reuseaddr SYSTEM:b25s
