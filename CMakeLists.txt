cmake_minimum_required(VERSION 3.5)
project(DIPhysics VERSION 0.1 DESCRIPTION "Physics module" LANGUAGES CXX)


file( GLOB SOURCES **.cpp )

SetFlags()
SetLibraries( DIDebuger DIUtils )
CreateLibrary( ${PROJECT_NAME} )