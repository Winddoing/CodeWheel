#!/usr/bin/env bash

target_windows=false;
target_android=false;
debug=false;
enable_ffplay=false;
enable_ffprobe=false;
enable_x264=false;
enable_x265=false;
enable_libaom=false;
enable_libvpx=false;
enable_libxml=false;
enable_libsrt=false;
enable_ffnvcodec=false;
enable_vmaf=false;
enable_shared=false;
enable_static=false;
enable_gstreamer_support=false;
dry_run_mode=false;
android_arch=x86_64;
custom_flags="";
extra_config_flags=""
product_line_flags=""
enable_logan=false;
enable_quadra=false;
enable_ni_drawtext=false;

# on a windows msys2 environment, detect linux symlinks for netint files and replace them with the target of symlink
# this function is primarily used for Netint internal git project where there are symlinks. Netint external releases will not have symlinks.
# this function may only be called when working_directory is top level of FFmpeg folder
# this function only scans for symlinks on this folder level and one level down
# this function only scans for symlinks that begin with '../' or '../../'
replace_symlinks_with_files_on_windows() {
    file_list=( $(find . -type f \( -iname \*ni\*.c -o -iname \*ni\*.h -o -iname \*ni\*.cpp -o -iname \*ni\*.hpp -o -iname \*.sh \)) )
    for file in ${file_list[@]}; do
        if [[ $(head -q -c 6 $file) == "../../" ]]; then
            if stat $(tail -q -c +4 $file) &> /dev/null; then
                # echo "Replaced working symlink: ${file}"
                cp $file $file.symlink
                cp $(tail -q -c +4 $file) $file
            fi
        elif [[ $(head -q -c 3 $file) == "../" ]]; then
            if stat $(cat $file) &> /dev/null; then
                # echo "Replaced working symlink: ${file}"
                cp $file $file.symlink
                cp $(cat $file) $file
            fi
        fi
    done
}

# undo the actions of replace_symlinks_with_files_on_windows() function
# removes ".symlink" suffix from files and replaces pre-existing files with them
restore_symlinks_on_windows() {
    file_list=( $(find . -type f -name \*.symlink ) )
    for file in ${file_list[@]}; do
        mv $file ${file%.*}
    done
}

# parse a flag with an arg in or after it
# $1 flag pattern, $2 entire flag arg, $3 arg after flag arg
# return 1 if path is in second arg (separated by space), else return 0. Store path in $extract_arg_ret
extract_arg () {
    unset extract_arg_ret
    # check valid arg flag
    if [ -n "$(printf "%s" ${2} | grep -Eoh "${1}")" ]; then
        # check if path string is connected by '=' or is in following arg
        if [ -n "$(echo "${2}" | grep -Eoh "${1}=")" ]; then
            arg_str=`printf "%s" "${2}" | grep -Poh "${1}=\K.+"`;
            # trim out leading and trailing quotation marks
            extract_arg_ret=`echo "${arg_str}" | sed -e 's/^\(["'\'']\)//' -e 's/\(["'\'']\)$//'`;
            return 0;
        elif [ -n "$(printf "%s" ${2} | grep -Eoh "^${1}$")" ]; then
            arg_str="${3}";
            # trim out leading and trailing quotation marks
            extract_arg_ret=`printf "%s" "${arg_str}" | sed -e 's/^\(["'\'']\)//' -e 's/\(["'\'']\)$//'`;
            return 1;
        else
            echo "Unknown option '$2', exiting";
            exit 1;
        fi
    else
        echo "Target flag '$1' not found in '$2', exiting"; exit 1;
    fi
}

if [ `whoami` = root ]; then
    read -p "Do you wish to execute with sudo [Y/N]? " -n 1 -r
    echo   
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit
    fi
fi

while [ "$1" != "" ]; do
    case $1 in
        -h | --help) echo "Usage: ./build_ffmpeg.sh --quadra | --logan [OPTION]";
                     echo "Compile FFmpeg for Logan/Quadra.";
                     echo "Example: ./build_ffmpeg.sh --quadra";
                     echo;
                     echo "Options:";
                     echo "-h, --help                    display this help and exit.";
                     echo "-w, --windows                 compile for windows.";
                     echo "-a, --android                 compile for Android NDK.";
                     echo "-g, --gdb                     compile for GDB.";
                     echo "--quadra                      build for Quadra (enabled if neither --quadra or --logan selected).";
                     echo "--logan                       build for Logan.";
                     echo "--nidrawtext                  build for ni_quadra_drawtext filter on Linux (requires libfreetype; recommend libfontconfig and libfribidi).";
                     echo "--ffplay                      compile with ffplay (requires libsdl2 package).";
                     echo "--ffprobe                     compile with ffprobe.";
                     echo "--libx264                     compile with libx264 (requires libx264 package).";
                     echo "--libx265                     compile with libx265 (requires libx265 package).";
                     echo "--libaom                      compile with libaom (requires libaom package).";
                     echo "--libvpx                      compile with libvpx-vp9 (requires libvpx-dev package).";
                     echo "--libxml                      compile with libxml2 (requires libxml2-dev, liblzma-dev package).";
                     echo "--libsrt                      compile with libsrt (requires libsrt-dev, libssl-dev package).";
                     echo "--ffnvcodec                   compile with ffnvcodec (requires ffnvcodec package).";
                     echo "--vmaf                        compile with vmaf (requires libvmaf).";
                     echo "--shared                      compile with shared libFF components.";
                     echo "--static                      compile entirely static (requires glibc-static).";
                     echo "--gstreamer                   compile with Netint GStreamer support (FFmpeg 4.3.1 only).";
                     echo "--dry                         dry run printing configs without building";
                     echo "--android_arch \"<arch>\"       cross compile CPU arch when compiling for --android. [arm,arm64,x86,x86_64(default)]";
                     echo "--custom_flags \"<flags>\"      compile with custom configuration flags";
                     echo "";
                     echo "Quadra required configuration flags:";
                     echo "--enable-ni_quadra --enable-gpl";
                     echo "--extra-ldflags='-lm -ldl'";
                     echo "--enable-pthreads --extra-libs='-lpthread'";
                     echo "--enable-x86asm";
                     echo "";
                     echo "Logan required configuration flags:";
                     echo "--enable-ni_logan --enable-gpl";
                     echo "--extra-ldflags='-lm -ldl'";
                     echo "--enable-pthreads --extra-libs='-lpthread'";
                     echo "--enable-x86asm";
                     exit 0
        ;;
        -w | --windows)  target_windows=true
        ;;
        -a | --android)  target_android=true
        ;;
        -g | --gdb)      debug=true
        ;;
        --quadra)        enable_quadra=true
        ;;
        --logan)         enable_logan=true
        ;;
        --nidrawtext)    enable_ni_drawtext=true
        ;;
        --ffplay)        enable_ffplay=true
        ;;
        --ffprobe)       enable_ffprobe=true
        ;;
        --libx264)       enable_x264=true
        ;;
        --libx265)       enable_x265=true
        ;;
        --libaom)        enable_libaom=true
        ;;
        --libvpx)        enable_libvpx=true
        ;;
        --libxml)        enable_libxml=true
        ;;
        --libsrt)        enable_libsrt=true
        ;;
        --ffnvcodec)     enable_ffnvcodec=true
        ;;
        --vmaf)          enable_vmaf=true
        ;;
        --shared)        enable_shared=true
        ;;
        --static)        enable_static=true
        ;;
        --gstreamer)     enable_gstreamer_support=true
        ;;
        --dry)           dry_run_mode=true
        ;;
        --android_arch | --android_arch=*)  extract_arg "\-\-android_arch" "$1" "$2"; prev_rc=$?;
                                            if [ "$prev_rc" -eq 1 ]; then shift; fi
                                            android_arch=$extract_arg_ret
        ;;
        --custom_flags | --custom_flags=*)  extract_arg "\-\-custom_flags" "$1" "$2"; prev_rc=$?;
                                            if [ "$prev_rc" -eq 1 ]; then shift; fi
                                            custom_flags=$extract_arg_ret
        ;;
        *)               echo "Usage: ./build_ffmpeg.sh [OPTION]...";
                         echo "Try './build_ffmpeg.sh --help' for more information"; exit 1
        ;;
    esac
    shift
done

if [[ $enable_logan == "true" && $enable_quadra == "true" ]]; then
    product_line_flags="--enable-ni_quadra --enable-ni_logan"
elif $enable_logan; then
    product_line_flags="--enable-ni_logan --disable-ni_quadra"
elif $enable_quadra; then
    product_line_flags="--enable-ni_quadra --disable-ni_logan"
elif [[ $enable_logan == "false" && $enable_quadra == "false" ]]; then
    product_line_flags="--enable-ni_quadra --disable-ni_logan"
    enable_quadra=true
    enable_logan=false
fi

if [[ $target_windows == "false" && $target_android == "false" && $enable_ni_drawtext == "true" ]]; then
    extra_config_flags="${extra_config_flags} --enable-libfreetype --enable-libfontconfig"
else
    product_line_flags="${product_line_flags} --disable-filter=drawtext_ni_quadra"
fi

if $debug; then
    extra_config_flags="${extra_config_flags} --disable-optimizations --disable-asm --disable-stripping --enable-debug=3"
else
    extra_config_flags="${extra_config_flags} --enable-x86asm --disable-debug"
fi

if $enable_ffplay; then
    extra_config_flags="${extra_config_flags} --enable-ffplay"
else
    extra_config_flags="${extra_config_flags} --disable-ffplay"
fi

if $enable_ffprobe; then
    extra_config_flags="${extra_config_flags} --enable-ffprobe"
else
    extra_config_flags="${extra_config_flags} --disable-ffprobe"
fi

if $enable_x264; then
    extra_config_flags="${extra_config_flags} --enable-libx264"
else
    extra_config_flags="${extra_config_flags} --disable-libx264"
fi

if $enable_x265; then
    extra_config_flags="${extra_config_flags} --enable-libx265"
else
    extra_config_flags="${extra_config_flags} --disable-libx265"
fi

if $enable_libaom; then
    extra_config_flags="${extra_config_flags} --enable-libaom"
else
    extra_config_flags="${extra_config_flags} --disable-libaom"
fi

if $enable_libvpx; then
    extra_config_flags="${extra_config_flags} --enable-libvpx"
else
    extra_config_flags="${extra_config_flags} --disable-libvpx"
fi

if $enable_libxml; then
    extra_config_flags="${extra_config_flags} --enable-libxml2"
else
    extra_config_flags="${extra_config_flags} --disable-libxml2"
fi

if $enable_libsrt; then
    extra_config_flags="${extra_config_flags} --enable-libsrt"
else
    extra_config_flags="${extra_config_flags} --disable-libsrt"
fi

if $enable_ffnvcodec; then
    extra_config_flags="${extra_config_flags} --extra-cflags=-I/usr/local/cuda/targets/x86_64-linux/include --extra-ldflags=-L/usr/local/cuda/targets/x86_64-linux/lib --enable-cuda-nvcc --enable-cuda --enable-cuvid --enable-nvdec --enable-nvenc"
else
    extra_config_flags="${extra_config_flags} --disable-cuda-nvcc --disable-cuda --disable-cuvid --disable-nvdec --disable-nvenc"
fi

if $enable_vmaf; then
    extra_config_flags="${extra_config_flags} --enable-libvmaf --enable-version3"
else
    extra_config_flags="${extra_config_flags} --disable-libvmaf"
fi

if $enable_shared; then
    if $enable_static; then
        echo -e "\e[31mCannot use --shared and --static together. Exiting...\e[0m"
        exit 1
    fi
    extra_config_flags="${extra_config_flags} --disable-static --enable-shared"
else
    extra_config_flags="${extra_config_flags} --enable-static --disable-shared"
fi

if $enable_static; then
    extra_config_flags="${extra_config_flags} --enable-static --disable-shared --extra-cflags=-static --extra-ldflags=-static"
fi

if $enable_gstreamer_support; then
    extra_config_flags="${extra_config_flags} --extra-cflags=-DNI_DEC_GSTREAMER_SUPPORT"
else
    extra_config_flags="${extra_config_flags} --extra-cflags=-UNI_DEC_GSTREAMER_SUPPORT"
fi

extra_config_flags="${extra_config_flags} ${custom_flags}"

if $target_windows; then
    if [ $(awk '{print}' VERSION) != "4.2.1" ] && [ $(awk '{print}' VERSION) != "4.3.1" ] && [ $(awk '{print}' VERSION) != "4.4" ]; then
        echo -e "\e[33mWarning: Only FFmpeg n4.2.1 n4.3.1 and n4.4 are supported for NI Windows build\e[0m"
    fi

    if $dry_run_mode; then # Dry-run mode args is a separate duplicate of wet-run mode args due to bash quotation passing limitations
        echo ./configure \
        --enable-cross-compile --arch='x86_64' \
        --target-os='mingw32' --cross-prefix='x86_64-w64-mingw32-' \
        --nm='x86_64-w64-mingw32-gcc-nm' \
        --ar='x86_64-w64-mingw32-gcc-ar' \
        --strip='strip' \
        --pkg-config-flags='--static' \
        --enable-gpl --enable-nonfree \
        --extra-ldflags=\'-lm -Wl,--stack 4194304\' \
        ${product_line_flags} \
        --enable-w32threads --extra-libs=\'-lwinpthread -lws2_32\' \
        --enable-encoders --enable-decoders --enable-avfilter --enable-muxers --enable-demuxers --enable-parsers \
        ${extra_config_flags}
    else # Dry-run mode args is a separate duplicate of wet-run mode args due to bash quotation passing limitations
        replace_symlinks_with_files_on_windows
        ./configure \
        --enable-cross-compile --arch='x86_64' \
        --target-os='mingw32' --cross-prefix='x86_64-w64-mingw32-' \
        --nm='x86_64-w64-mingw32-gcc-nm' \
        --ar='x86_64-w64-mingw32-gcc-ar' \
        --strip='strip' \
        --pkg-config-flags='--static' \
        --enable-gpl --enable-nonfree \
        --extra-ldflags='-lm -Wl,--stack 4194304' \
        ${product_line_flags} \
        --enable-w32threads --extra-libs='-lwinpthread -lws2_32' \
        --enable-encoders --enable-decoders --enable-avfilter --enable-muxers --enable-demuxers --enable-parsers \
        ${extra_config_flags}
        if [ $? != 0 ]; then
            echo -e "\e[31mConfiguration failed. Exiting...\e[0m"
            restore_symlinks_on_windows
            exit 1
        else
            make -j $(nproc)
            restore_symlinks_on_windows
            RC=$?
        fi
    fi
elif $target_android; then
    echo "android_arch: ${android_arch}"

    if [ -z ${ANDROID_NDK_ROOT} ]; then
        echo "You must set ANDROID_NDK_ROOT environment variable"
        echo "Please download NDK r21d from https://dl.google.com/android/repository/android-ndk-r21d-linux-x86_64.zip"
        exit -1
    fi

    API=28
    TOOLCHAIN=${ANDROID_NDK_ROOT}/toolchains/llvm/prebuilt/linux-x86_64

    if [ "${android_arch}" = "arm" ]; then
        ARCH=arm
        ARCH2=armv7a
        CPU=armv7-a
        CC_SUFFIX=eabi
        CROSS_PREFIX=$TOOLCHAIN/bin/arm-linux-androideabi-
    elif [ "${android_arch}" = "arm64" ]; then
        ARCH=arm64
        ARCH2=aarch64
        CPU=armv8-a
        CROSS_PREFIX=$TOOLCHAIN/bin/$ARCH2-linux-android-
    elif [ "${android_arch}" = "x86" ]; then
        ARCH=x86
        ARCH2=i686
        CPU=i686
        CROSS_PREFIX=$TOOLCHAIN/bin/$ARCH2-linux-android-
    elif [ "${android_arch}" = "x86_64" ]; then
        ARCH=x86_64
        ARCH2=x86_64
        CPU=x86_64
        CROSS_PREFIX=$TOOLCHAIN/bin/$ARCH2-linux-android-
    elif [ "${android_arch}" = "" ]; then
        ARCH=x86_64
        ARCH2=x86_64
        CPU=x86_64
        CROSS_PREFIX=$TOOLCHAIN/bin/$ARCH2-linux-android-
    else
        echo "Error - unknown option for --android_arch. Select from: [arm,arm64,x86,x86_64]"
        exit -1
    fi

    echo "Building android ARCH=${ARCH}"

    PREFIX=android/$ARCH

    if $dry_run_mode; then # Dry-run mode args is a separate duplicate of wet-run mode args due to bash quotation passing limitations
        echo ./configure \
        --prefix=$PREFIX \
        --enable-cross-compile \
        --sysroot=$TOOLCHAIN/sysroot \
        --arch=$ARCH2 \
        --cpu=$CPU \
        --target-os='android' \
        --cc=$TOOLCHAIN/bin/$ARCH2-linux-android${CC_SUFFIX}${API}-clang \
        --cross-prefix=$CROSS_PREFIX \
        --pkg-config=$(which pkg-config) \
        --pkg-config-flags='--static' \
        --enable-gpl --enable-nonfree \
        --extra-ldflags=\'-fuse-ld=gold -lm\' \
        ${product_line_flags} \
        --extra-libs=-lgcc \
        --enable-pic \
        --extra-cflags=\'-DANDROID -D_ANDROID -D__ANDROID__\' \
        --enable-pthreads \
        --enable-encoders --enable-decoders --enable-avfilter --enable-muxers --enable-demuxers --enable-parsers \
        ${extra_config_flags}
    else # Dry-run mode args is a separate duplicate of wet-run mode args due to bash quotation passing limitations
        ./configure \
        --prefix=$PREFIX \
        --enable-cross-compile \
        --sysroot=$TOOLCHAIN/sysroot \
        --arch=$ARCH2 \
        --cpu=$CPU \
        --target-os='android' \
        --cc=$TOOLCHAIN/bin/$ARCH2-linux-android${CC_SUFFIX}${API}-clang \
        --cross-prefix=$CROSS_PREFIX \
        --pkg-config=$(which pkg-config) \
        --pkg-config-flags='--static' \
        --enable-gpl --enable-nonfree \
        --extra-ldflags='-fuse-ld=gold -lm' \
        ${product_line_flags} \
        --extra-libs=-lgcc \
        --enable-pic \
        --extra-cflags='-DANDROID -D_ANDROID -D__ANDROID__' \
        --enable-pthreads \
        --enable-encoders --enable-decoders --enable-avfilter --enable-muxers --enable-demuxers --enable-parsers \
        ${extra_config_flags}
        if [ $? != 0 ]; then
            echo -e "\e[31mConfiguration failed. Exiting...\e[0m"
            exit 1
        else
            make -j $(nproc)
            RC=$?
        fi
    fi
else
    if $dry_run_mode; then # Dry-run mode args is a separate duplicate of wet-run mode args due to bash quotation passing limitations
        echo ./configure \
        --pkg-config-flags='--static' \
        --enable-gpl --enable-nonfree \
        --extra-ldflags='-lm' --extra-ldflags='-ldl' \
        ${product_line_flags} \
        --enable-pthreads --extra-libs='-lpthread' \
        --enable-encoders --enable-decoders --enable-avfilter --enable-muxers --enable-demuxers --enable-parsers \
        ${extra_config_flags}
    else # Dry-run mode args is a separate duplicate of wet-run mode args due to bash quotation passing limitations
        ./configure \
        --pkg-config-flags='--static' \
        --enable-gpl --enable-nonfree \
        --extra-ldflags='-lm' --extra-ldflags='-ldl' \
        ${product_line_flags}  \
        --enable-pthreads --extra-libs='-lpthread' \
        --enable-encoders --enable-decoders --enable-avfilter --enable-muxers --enable-demuxers --enable-parsers \
        ${extra_config_flags}
        if [ $? != 0 ]; then
            echo -e "\e[31mConfiguration failed. Exiting...\e[0m"
            exit 1
        else
            make -j $(nproc)
            RC=$?
        fi
    fi
fi

if $enable_shared && [ ! -z $RC ] && [ $RC -eq 0 ]; then
    echo "Reminder: after installing FFmpeg, run 'sudo ldconfig' to cache the shared libraries"
fi
exit $RC
