#!/bin/bash

set -e

g++ -std=c++17 -Wall -Wextra -pedantic main.cpp todo.cpp console_app.cpp -o app
./app
