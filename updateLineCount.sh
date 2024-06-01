updateLineCount.sh#!/bin/bash

test=$(find . -name "*.*pp" | xargs wc -l | sed -n 's/ *\([0-9]*\) total/\1/p')
sed -i "s/\/Lines-[^-]*-/\/Lines-$test-/g" README.md
