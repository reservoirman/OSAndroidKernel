#!/bin/bash
git ls-tree -r HEAD --name-only | grep "\(`find $1 -name '*.h' -type f | grep -o '[^/]*$' | sed -e 's/\n$//g' | perl -pi -e 's/^(.*)\n$/\\\|\1/g' | perl -pi -e 's/^(\\\.)//g'`\)"
git ls-tree -r HEAD --name-only | grep "\(`find $1 -name '*.c' -type f | grep -o '[^/]*$' | sed -e 's/\n$//g' | perl -pi -e 's/^(.*)\n$/\\\|\1/g' | perl -pi -e 's/^(\\\.)//g'`\)"
