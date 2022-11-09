#!/usr/bin/sh

# Na primeira vez descomente a linha abaixo
# make depend

make
./verbgen
make

# Obs.: caso tenha problemas com o horário use o comando $ dpkg-reconfigure tzdata
