#!/bin/bash
set -e

PKGROOT="packaging/linux/deb/count"
OUTPUT="counting_tool_1.0.0.deb"

echo "Cleaning previous build..."

mkdir -p "$PKGROOT/usr/local/bin"
mkdir -p "$PKGROOT/usr/local/share/count/docs"

rm -rf "$PKGROOT/usr/local/bin/"*
rm -rf "$PKGROOT/usr/local/share/count/docs/"*

echo "Copying binary..."
cp build/linux/count "$PKGROOT/usr/local/bin/"

# Set executable permission
chmod 755 "$PKGROOT/usr/local/bin/count"

echo "Copying documentation..."
cp -r docs/* "$PKGROOT/usr/local/share/count/docs/"

echo "Fixing control directory permissions..."
chmod 755 "$PKGROOT/DEBIAN"

echo "Building .deb package..."
dpkg-deb --build "$PKGROOT" "$OUTPUT"

echo "Package created:"
echo "$OUTPUT"