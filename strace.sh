#!/bin/bash

base=$(basename "$1")
strace -o >(tee strace.$base.out | sed 's,^,tr: ,') -fF "$@"
