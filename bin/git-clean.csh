#!/bin/csh

find . -name "*.h" -exec bash -c 'expand -t 4 "$0" > $0.tmp && mv $0.tmp "$0"' {} \;

find . -name "*.cpp" -exec bash -c 'expand -t 4 "$0" > $0.tmp && mv $0.tmp "$0"' {} \;

find . -name "*.h"| xargs dos2unix
find . -name "*.cpp"| xargs dos2unix
