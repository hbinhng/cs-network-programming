#!/bin/sh

echo "Building server source..."
rm -rf ./Server/CMakeFiles
cmake -S ./Server -B ./Server
make -C ./Server

echo "Building client source..."
rm -rf ./Client/CMakeFiles
cmake -S ./Client -B ./Client
make -C ./Client