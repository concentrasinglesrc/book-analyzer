# !/bin/sh

set -e
git submodule update --init --remote

cmake -B googletest -S googletest -DCMAKE_BUILD_TYPE=Release
make -C googletest -j all

set +e

autoreconf -i
