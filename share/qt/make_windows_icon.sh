#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/baudcoin.png
ICON_DST=../../src/qt/res/icons/baudcoin.ico
convert ${ICON_SRC} -resize 16x16 baudcoin-16.png
convert ${ICON_SRC} -resize 32x32 baudcoin-32.png
convert ${ICON_SRC} -resize 48x48 baudcoin-48.png
convert baudcoin-16.png baudcoin-32.png baudcoin-48.png ${ICON_DST}

