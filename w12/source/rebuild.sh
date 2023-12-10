#!/bin/sh

rm -rf ./out/

mkdir -p ./out/Server
mkdir -p ./out/Client

echo "Building server ..."

find ./Server -name "*.java" -print | xargs javac -d ./out/

echo "Building client ..."

find ./Client -name "*.java" -print | xargs javac -d ./out/