# Automatically generated by boost-vcpkg-helpers/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/icl
    REF boost-1.73.0
    SHA512 806caed377cf0bb70fd65fe1d9524f5cac834094cbfbec8f874ec1f742887837033c53ddd4036bd733d5758e67ffc0a9f5d389dfc0fa3dc06fda6ab82f21e750
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})