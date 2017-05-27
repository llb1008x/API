#!/bin/bash
PROJECT_DIR="$1"
TARGET_OUT_DATA_MEDIA="$2/media/"
GN_RESOURCE_DIR=packages/gn_resource
[ ! -d  "$GN_RESOURCE_DIR" ] && echo "not gn resource into sdcard" && exit 0

mkdir -p $TARGET_OUT_DATA_MEDIA 
cp -r ${GN_RESOURCE_DIR}/* ${TARGET_OUT_DATA_MEDIA}

