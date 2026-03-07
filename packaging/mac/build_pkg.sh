#!/bin/bash
set -e

APP_NAME="counting_tool"
VERSION="1.0"

BUILD_DIR="mac_build"
PKG_DIR="mac_pkg"

rm -rf $BUILD_DIR $PKG_DIR
mkdir -p $BUILD_DIR/usr/local/bin
mkdir -p $BUILD_DIR/usr/local/share/count/docs

# copy binary
cp build/count $BUILD_DIR/usr/local/bin/

# copy docs
cp docs/* $BUILD_DIR/usr/local/share/count/docs/

pkgbuild \
  --root $BUILD_DIR \
  --identifier com.count.cli \
  --version $VERSION \
  --install-location / \
  $PKG_DIR/${APP_NAME}.pkg

echo "macOS package created:"
ls $PKG_DIR