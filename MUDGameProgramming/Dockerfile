FROM ubuntu:focal

RUN apt update && \
    apt install -y vim && \
    apt install -y build-essential gcc-10-doc libstdc++-10-doc && \
    apt install -y libmsgsl-dev

WORKDIR /usr/src/UnMUD
COPY build.sh ./build.sh
RUN chmod +x ./build.sh