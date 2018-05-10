#!/bin/bash

cd "$(dirname "$0")"

ino build -m nano328
ino upload -m nano328
