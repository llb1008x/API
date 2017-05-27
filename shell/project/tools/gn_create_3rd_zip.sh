#!/bin/bash
#Gionee <Amigo_KeepApk> <lingfen> <20150131> add for CR01444099 begin
#GN_FACTORY_RESET_KEEP_APK=yes
GN_COMPILE_TOOLS_PATH="gionee/config"
PROJECT_DIR="$1"
TARGET_OUT_DATA_MEDIA="$2"
mkdir -p $TARGET_OUT_DATA_MEDIA 
TCARD_DIR="${PROJECT_DIR}/tcard"
[ ! -d  "$TCARD_DIR" ] && echo "not 3rd installed from tcard" && exit 0

GN_APPS_FILE="${TARGET_OUT_DATA_MEDIA}/gn_apps.zip"

UPDATER_FILE="${GN_COMPILE_TOOLS_PATH}/tools/updater"
META_INF_TREE="${TCARD_DIR}/META-INF/com/google/android"
UPDATER_SCRIPT="${META_INF_TREE}/updater-script"
UPDATE_BINARY="${META_INF_TREE}/update-binary"

TESTKEY1="build/target/product/security/releasekey.x509.pem"
TESTKEY2="build/target/product/security/releasekey.pk8"
TOOLS="out/host/linux-x86/framework/signapk.jar"

create_package_extract_file(){
    #for apk in `find $TCARD_DIR -type f -name "*.apk" -printf "%P\n"`
    local extract_dir_list=""
    for apk in `find $TCARD_DIR -type f -name "*.apk" | awk -F "$TCARD_DIR/" '{print $2}'`
    do
        local name=$(dirname $apk)
        if ! `echo $extract_dir_list | grep -wq $name` ;then
            echo "package_extract_dir(\"$name\", \"/data/app\");"
        fi
        extract_dir_list+=" $name"
    done
}

create_updater_script(){
#适合于5.x mtk 
DEVICE="/dev/block/platform/mtk-msdc.0/by-name/userdata"
MOUNT_POINT="/data"
FS_TYPE="ext4"
PARTITION_TYPE="EMMC"

cat > $1 <<EOF
ui_print("Install GiONEE 3rd Begin");
ifelse(is_mounted("$MOUNT_POINT") == "$MOUNT_POINT", ui_print("$MOUNT_POINT has mounted"),mount("$FS_TYPE","$PARTITION_TYPE", "$DEVICE", "$MOUNT_POINT"));
`create_package_extract_file`
ui_print("Install GiONEE 3rd End");
EOF
}

if [ -d "${TCARD_DIR}" -a "${GN_FACTORY_RESET_KEEP_APK:0:1}" == "y" ];then
    #build META-INF tree 
    mkdir -p ${META_INF_TREE}
    cp ${UPDATER_FILE} ${UPDATE_BINARY}
    create_updater_script ${UPDATER_SCRIPT}
    
    #build 3rd_tmp.zip package
    rm -rf /tmp/3rd_tmp.zip
    pushd ${TCARD_DIR} >/dev/null
    find system -type f | zip -q "/tmp/3rd_tmp.zip" -@
    find META-INF -type f | zip -q "/tmp/3rd_tmp.zip" -@
    popd  >/dev/null

    #Encryption
    java -jar $TOOLS -w $TESTKEY1 $TESTKEY2 "/tmp/3rd_tmp.zip" "${GN_APPS_FILE}"
    rm -rf /tmp/3rd_tmp.zip
fi
#Gionee <Amigo_KeepApk> <lingfen> <20150131> add for CR01444099 end
