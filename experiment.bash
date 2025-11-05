#!/bin/bash

for((B=512; B < 4097; B=B+512)); do
    for((D=256; D < 4000; D=D+256)); do
        make all DISTANCE=$D BRANCHES=$B
    done
done