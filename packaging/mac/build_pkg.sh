#!/bin/bash
set -e

APP_NAME="counting_tool"
VERSION="1.0.0"

PKG_DIR="packaging/mac/"

mkdir -p $PKG_DIR/usr/local/bin
mkdir -p $PKG_DIR/usr/local/share/count/docs

rm -rf $PKG_DIR/usr/local/bin/*
rm -rf $PKG_DIR/usr/local/share/count/docs/*

# copy binary
cp build/mac/count $PKG_DIR/usr/local/bin/

# copy docs
cp docs/* $PKG_DIR/usr/local/share/count/docs/

pkgbuild \
  --root $PKG_DIR \
  --identifier com.count.cli \
  --version $VERSION \
  --install-location / \
  $PKG_DIR/${APP_NAME}_${VERSION}.pkg

echo "macOS package created:"
ls $PKG_DIR