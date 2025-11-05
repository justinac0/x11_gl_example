#!/usr/bin/env bash

clang-format -i ./*.c
clang-format -i ./**/*.h
# clang-format -i ./**/*.c
clang-format -i ./**/**/*.c
clang-format -i ./**/**/*.h

