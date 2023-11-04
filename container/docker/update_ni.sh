#! /bin/bash
NI_PATH=$(cd "$(dirname "$0")"; pwd)
echo $NI_PATH

help() {
    echo "Param 1: the exsiting container name."
    echo "Param 2(option): the new image name created by the container with new image."
    echo "e.g. $0 test img"
}

install_libs() {
    local NI_PATH=$1
    local CONTAINER=$2

    set -x
    docker cp $NI_PATH/vendor/lib64/libavcodec.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/libavdevice.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/libavfilter.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/libavformat.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/libavutil.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/libpostproc.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/libswresample.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/libswscale.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/bin/ffmpeg $CONTAINER:vendor/bin/

    docker cp $NI_PATH/vendor/lib64/libandroid.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/libcamera2ndk.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/libmediandk.so $CONTAINER:vendor/lib64/

    docker cp $NI_PATH/vendor/lib64/libxcoder.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/system/lib64/android.hardware.nidec@1.0.so $CONTAINER:vendor/lib64/
    docker cp $NI_PATH/vendor/lib64/hw/android.hardware.nidec@1.0-impl.so $CONTAINER:vendor/lib64/hw/
    docker cp $NI_PATH/vendor/bin/hw/android.hardware.nidec@1.0-service $CONTAINER:vendor/bin/hw
    docker cp $NI_PATH/vendor/bin/ni_rsrc_mon $CONTAINER:vendor/bin/
    docker cp $NI_PATH/vendor/etc/vintf/manifest/android.hardware.nidec@1.0-service.xml $CONTAINER:vendor/etc/vintf/manifest/
    docker cp $NI_PATH/vendor/lib64/hw/android.hardware.nidec@1.0-impl.so $CONTAINER:vendor/lib64/hw/

    docker cp -a $NI_PATH/vendor/etc/init/android.hardware.nidec@1.0-service.rc $CONTAINER:vendor/etc/init/
    docker exec $CONTAINER chown root:root /vendor/etc/init/android.hardware.nidec@1.0-service.rc
    docker exec $CONTAINER chmod 644 /vendor/etc/init/android.hardware.nidec@1.0-service.rc
    set +x
}

if [ $# -lt 1 ] || [ $1 = '-h' ];then
    help
    exit 1
fi
CONTAINER=$1
docker kill $CONTAINER
if [ "$(docker inspect $CONTAINER 2>/dev/null | grep "\"Name\": \"/${CONTAINER}\"" )" != "" ];then
    install_libs $NI_PATH $CONTAINER
else
    echo "Can't detect cotainer, please checke your script, don't use --rm option when run container"
    exit 1
fi
if [ $# -eq 2 ];then
    IMAGE_NAME=$2
    docker commit $CONTAINER $IMAGE_NAME
fi
docker restart $CONTAINER
