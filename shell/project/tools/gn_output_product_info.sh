#!/bin/bash

echo "[VER]" > $2/Gionee_info.ini
echo "product_info=$1" >> $2/Gionee_info.ini
chmod 777 $2/Gionee_info.ini
