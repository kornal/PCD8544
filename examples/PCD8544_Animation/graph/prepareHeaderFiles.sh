#!/bin/bash
for fileName in *.png
do
    ../../../utilities/png2CarrayConverter.py $fileName
done
