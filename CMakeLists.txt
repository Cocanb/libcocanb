cmake_minimum_required(VERSION 3.0)

project(libcocanb)

set(
  SOURCES
  src/cocanb.c
)

add_library(cocanb STATIC ${SOURCES})

include_directories(include)
