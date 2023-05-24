FROM ubuntu:focal

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y vim make && \
    apt install -y build-essential gcc-10-doc libstdc++-10-doc && \
    apt install -y libmsgsl-dev && \
    apt install -y cppcheck clang clang-tidy clang-format && \
    apt install -y libgtest-dev gcovr && \
    apt install -y postgresql-client && \
    apt install -y libpqxx-dev libpqxx-doc
