#! /bin/bash
UMD_PATH=$(cd "$(dirname "$0")"; pwd)
echo $UMD_PATH
help() {
    echo "Param 1: the exsiting container name."
    echo "Param 2(option): the new image name created by the container with new umd."
    echo "e.g. ./update_umd test img"
}

install_libs() {
    local UMD_PATH=$1
    local CONTAINER=$2

    docker cp $UMD_PATH/vendor/lib64/hw/gralloc.gbm.so $CONTAINER:vendor/lib64/hw/gralloc.gbm.so
    docker cp $UMD_PATH/vendor/lib64/hw/gralloc.dri.so $CONTAINER:vendor/lib64/hw/gralloc.dri.so
    docker cp $UMD_PATH/vendor/lib64/libgbm.so $CONTAINER:vendor/lib64/libgbm.so
    docker cp $UMD_PATH/vendor/lib64/libgbm.so.1 $CONTAINER:vendor/lib64/
    docker cp $UMD_PATH/vendor/lib64/dri/ $CONTAINER:vendor/lib64/
    docker cp $UMD_PATH/vendor/lib64/egl/ $CONTAINER:vendor/lib64/
    docker cp $UMD_PATH/vendor/lib64/libLLVM12.so $CONTAINER:vendor/lib64/libLLVM12.so
    docker cp $UMD_PATH/vendor/lib64/libdrm.so $CONTAINER:vendor/lib64/libdrm.so
    docker cp $UMD_PATH/vendor/lib64/libdrm.so $CONTAINER:system/lib64/libdrm.so
    docker cp $UMD_PATH/vendor/lib64/libdrm_amdgpu.so $CONTAINER:vendor/lib64/libdrm_amdgpu.so
    docker cp $UMD_PATH/vendor/lib64/libdrm_radeon.so $CONTAINER:vendor/lib64/libdrm_radeon.so
    docker cp $UMD_PATH/vendor/lib64/libglapi.so $CONTAINER:vendor/lib64/libglapi.so
    docker cp $UMD_PATH/vendor/lib64/libglapi.so.0 $CONTAINER:vendor/lib64/

    docker cp $UMD_PATH/vendor/lib64/libOMX_Core.so $CONTAINER:/vendor/lib64/libOMX_Core.so
    docker cp $UMD_PATH/vendor/lib64/libomx_mesa.so $CONTAINER:vendor/lib64/
    docker cp $UMD_PATH/vendor/lib64/libomx_mesa.so.0 $CONTAINER:vendor/lib64/
    docker cp $UMD_PATH/vendor/lib64/libstagefrighthw.so $CONTAINER:vendor/lib64/

    docker cp $UMD_PATH/vendor/lib/hw/gralloc.gbm.so $CONTAINER:vendor/lib/hw/gralloc.gbm.so
    docker cp $UMD_PATH/vendor/lib/hw/gralloc.dri.so $CONTAINER:vendor/lib/hw/gralloc.dri.so
    docker cp $UMD_PATH/vendor/lib/libgbm.so $CONTAINER:vendor/lib/libgbm.so
    docker cp $UMD_PATH/vendor/lib/libgbm.so.1 $CONTAINER:vendor/lib/
    docker cp $UMD_PATH/vendor/lib/dri/ $CONTAINER:vendor/lib/
    docker cp $UMD_PATH/vendor/lib/egl/ $CONTAINER:vendor/lib/
    docker cp $UMD_PATH/vendor/lib/libLLVM12.so $CONTAINER:vendor/lib/libLLVM12.so
    docker cp $UMD_PATH/vendor/lib/libdrm.so $CONTAINER:vendor/lib/libdrm.so
    docker cp $UMD_PATH/vendor/lib/libdrm.so $CONTAINER:system/lib/libdrm.so
    docker cp $UMD_PATH/vendor/lib/libdrm_amdgpu.so $CONTAINER:vendor/lib/libdrm_amdgpu.so
    docker cp $UMD_PATH/vendor/lib/libdrm_radeon.so $CONTAINER:vendor/lib/libdrm_radeon.so
    docker cp $UMD_PATH/vendor/lib/libglapi.so $CONTAINER:vendor/lib/libglapi.so
    docker cp $UMD_PATH/vendor/lib/libglapi.so.0 $CONTAINER:vendor/lib/

    docker cp $UMD_PATH/vendor/lib/libOMX_Core.so $CONTAINER:/vendor/lib/libOMX_Core.so
    docker cp $UMD_PATH/vendor/lib/libomx_mesa.so $CONTAINER:vendor/lib/
    docker cp $UMD_PATH/vendor/lib/libomx_mesa.so.0 $CONTAINER:vendor/lib/
    docker cp $UMD_PATH/vendor/lib/libstagefrighthw.so $CONTAINER:vendor/lib/

    docker cp $UMD_PATH/vendor/etc/media_codecs.xml $CONTAINER:vendor/etc/media_codecs.xml
    docker cp $UMD_PATH/vendor/bin/omxregister-bellagio $CONTAINER:vendor/bin/omxregister-bellagio
    docker cp $UMD_PATH/vendor/etc/.omxregister $CONTAINER:vendor/etc/.omxregister
    docker cp $UMD_PATH/vendor/etc/seccomp_policy/mediacodec.policy $CONTAINER:/vendor/etc/seccomp_policy/mediacodec.policy
}

set -x
if [ $# -lt 1 ] || [ $1 = '-h' ];then
    help
    exit 1
fi
CONTAINER=$1
docker kill $CONTAINER
if [ "$(docker inspect $CONTAINER 2>/dev/null | grep "\"Name\": \"/${CONTAINER}\"" )" != "" ];then
    install_libs $UMD_PATH $CONTAINER
else
    echo "Can't detect cotainer, please checke your script, don't use --rm option when run container"
    exit 1
fi
if [ $# -eq 2 ];then
    IMAGE_NAME=$2
    docker commit $CONTAINER $IMAGE_NAME
fi
docker restart $CONTAINER
