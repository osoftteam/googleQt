#!/bin/csh

find . -name "*.h" -exec bash -c 'expand -t 4 "$0" > /tmp/totabbuff && mv /tmp/totabbuff "$0"' {} \;

find . -name "*.cpp" -exec bash -c 'expand -t 4 "$0" > /tmp/totabbuff && mv /tmp/totabbuff "$0"' {} \;

find . -name "*.h"| xargs dos2unix
find . -name "*.cpp"| xargs dos2unix
