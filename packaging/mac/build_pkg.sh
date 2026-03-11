#!/bin/bash
set -e

APP_NAME="counting_tool"
VERSION="1.0.0"

PKG_DIR="packaging/mac"

mkdir -p $PKG_DIR/bin
mkdir -p $PKG_DIR/share/count/docs

rm -rf $PKG_DIR/bin/*
rm -rf $PKG_DIR/share/count/docs/*

# copy binary
cp build/mac/count $PKG_DIR/bin/

# copy docs
cp docs/* $PKG_DIR/share/count/docs/

pkgbuild \
  --root $PKG_DIR \
  --identifier com.count.cli \
  --version $VERSION \
  --install-location /usr/local \
  $PKG_DIR/${APP_NAME}_${VERSION}.pkg

echo "macOS package created:"
ls $PKG_DIR