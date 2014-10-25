#!/bin/bash
#shamelessly stolen from http://github.com/Mudlet/Mudlet
set -ev
BREWS="boost cmake hunspell libzip libzzip lua51 pcre pkg-config qt5"
for i in $BREWS; do
  brew outdated | grep -q $i && brew upgrade $i
done
for i in $BREWS; do
  brew list | grep -q $i || brew install $i
done
