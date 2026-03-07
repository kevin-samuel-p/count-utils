#!/bin/bash
set -e

PKGROOT=packaging/linux/deb/count

echo "Cleaning previous build..."
rm -rf $PKGROOT/usr/local/bin/*
rm -rf $PKGROOT/usr/share/count/docs/*

echo "Copying binary..."
cp build/linux/count $PKGROOT/usr/local/bin/

echo "Copying documentation..."
cp -r docs/* $PKGROOT/usr/share/count/docs/

echo "Building .deb package..."
dpkg-deb --build $PKGROOT counting_tool_1.0_amd64.deb

echo "Package created:"
echo "counting_tool_1.0_amd64.deb"