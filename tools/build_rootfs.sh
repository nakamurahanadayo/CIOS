#!/bin/bash

set -e

ROOTFS="$(pwd)/rootfs"

echo "===================================="
echo "      CIOS ARCH ROOTFS BUILDER"
echo "===================================="


echo
echo "[1/5] Preparing clean filesystem..."

if [ -d "$ROOTFS" ]; then
    echo "Removing old rootfs..."
    rm -rf "$ROOTFS"
fi

mkdir -p "$ROOTFS"


echo
echo "[2/5] Installing Arch base system..."

pacstrap -c "$ROOTFS" \
base \
linux \
linux-firmware \
systemd \
bash \
sudo \
nano \
networkmanager


echo
echo "[3/5] Creating CIOS directories..."

mkdir -p "$ROOTFS/opt/cios/models"
mkdir -p "$ROOTFS/opt/cios/modules"
mkdir -p "$ROOTFS/opt/cios/config"
mkdir -p "$ROOTFS/opt/cios/data"


echo
echo "[4/5] Installing CIOS binary..."

if [ -f "build/cios" ]; then
    cp build/cios "$ROOTFS/usr/bin/cios"
    chmod +x "$ROOTFS/usr/bin/cios"
else
    echo "ERROR: build/cios not found!"
    exit 1
fi


echo
echo "[5/5] Creating CIOS systemd service..."

cat > "$ROOTFS/etc/systemd/system/cios.service" <<EOF
[Unit]
Description=CIOS Intelligence Environment
After=network.target

[Service]
ExecStart=/usr/bin/cios
Restart=always

[Install]
WantedBy=multi-user.target
EOF


echo
echo "===================================="
echo " CIOS ROOTFS COMPLETE"
echo "===================================="
