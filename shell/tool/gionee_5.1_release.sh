#!/bin/bash

GN_RELEASE_DIR=0
PRODUCT_OUT_ROOT=0
GN_TARGET_PRODUCT=0

get_release_version(){
    GN_TARGET_PRODUCT=${TARGET_PRODUCT/full_}
    GN_VERSION_FILE_PATH=device/mediatek/${GN_TARGET_PRODUCT}/ProjectConfig.mk
    tmpVersion=$(grep "GIONEE_VERSION=" ${GN_VERSION_FILE_PATH})
    tmpVersion=$(echo ${tmpVersion} | awk -F "=" '{printf("%s",$2)}')
    GN_RELEASE_DIR="release/$tmpVersion"
    #GN_RELEASE_MODEM_DIR="$GN_RELEASE_DIR/bp_image"
}

copy_results_to_release(){
    [ -d "$GN_RELEASE_DIR" ] && rm -rf $GN_RELEASE_DIR
    mkdir -p $GN_RELEASE_DIR
    
    PRODUCT_OUT_ROOT="out/target/product"
    TARGET_PRODUCT_OUT_ROOT="$PRODUCT_OUT_ROOT/${GN_TARGET_PRODUCT}"

    if [ -d "${TARGET_PRODUCT_OUT_ROOT}" ];then
        cp -f ${TARGET_PRODUCT_OUT_ROOT}/*.img $GN_RELEASE_DIR/;
        cp -f ${TARGET_PRODUCT_OUT_ROOT}/*.bin $GN_RELEASE_DIR/;
        cp -f ${TARGET_PRODUCT_OUT_ROOT}/*.txt $GN_RELEASE_DIR/;
        cp -f ${TARGET_PRODUCT_OUT_ROOT}/EBR1 $GN_RELEASE_DIR/;
        cp -f ${TARGET_PRODUCT_OUT_ROOT}/EBR2 $GN_RELEASE_DIR/;
        cp -f ${TARGET_PRODUCT_OUT_ROOT}/MBR $GN_RELEASE_DIR/;
    else
        echo "Wanning: ${TARGET_PRODUCT_OUT_ROOT} does no exist"
    fi

    #copy BLP file
    local bpl_file=$(find $TARGET_PRODUCT_OUT_ROOT/obj/ETC -type f -name "BPL*" 2>/dev/null )
    if [ -f "$bpl_file"  ];then
        cp -r $bpl_file $GN_RELEASE_DIR/
    fi

    #copy modem files
    find $TARGET_PRODUCT_OUT_ROOT/obj -type f -name "APDB*" -exec cp \{} $GN_RELEASE_DIR \;
    find $TARGET_PRODUCT_OUT_ROOT/system/etc/mddb -type f -name "BP*" -exec cp \{} $GN_RELEASE_DIR \;

    #copy symbols to gn_backup
    #if [ -d "$TARGET_PRODUCT_OUT_ROOT/symbols" ];then
    #    mkdir -p $PRODUCT_OUT_ROOT/gionee_backup/
    #    rsync -avq "$TARGET_PRODUCT_OUT_ROOT/symbols" "$PRODUCT_OUT_ROOT/gionee_backup/"
    #fi
    
    #copy vmlinux for debug
    #if [ -f "$TARGET_PRODUCT_OUT_ROOT/obj/KERNEL_OBJ/vmlinux" ];then
    #    cp "$TARGET_PRODUCT_OUT_ROOT/obj/KERNEL_OBJ/vmlinux" "$GN_RELEASE_DIR"
    #fi
}

main()
{
	get_release_version
	copy_results_to_release	
}

main

