FROM ubuntu:latest

RUN apt-get update && apt-get install -y make gcc

WORKDIR ./build

COPY . .

RUN make

