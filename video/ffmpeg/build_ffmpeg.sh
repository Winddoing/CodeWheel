#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : build_ffmpeg.sh
#  Author       : wqshao
#  Created Time : 2023-10-23 15:04:05
#  Description  :
##########################################################
TOP=`pwd`
BASE_DIR="$TOP/base"

# FFmpeg配置参数
ENABLE_DEBUG=false;
ENABLE_FFPLAY=false;
ENABLE_FFPROBE=false;
ENABLE_X264=false;
ENABLE_X265=false;
ENABLE_LIBAOM=false;
ENABLE_LIBVPX=false;
ENABLE_LIBXML=false;
ENABLE_LIBSRT=false;
ENABLE_FFNVCODEC=false;
ENABLE_VMAF=false;
ENABLE_SHARED=true;
ENABLE_STATIC=false;
ENABLE_GSTREAMER_SUPPORT=false;

ANDROID_BUILD=true;
ANDROID_ARCH=arm64;
ANDROID_NDK_API=30

CUSTOM_FLAGS="";
EXTRA_CONFIG_FLAGS=""
PRODUCT_LINE_FLAGS=""

ENABLE_LOGAN=false;
ENABLE_QUADRA=false;

if $ANDROID_BUILD; then
	OUTPUT="${TOP}/out/android"
else
	OUTPUT="${TOP}/out/linux"
fi

info()  { echo -e "\e[34m$@\e[0m"; }
warn()  { echo -e "\e[33m$@\e[0m"; }
error() { echo -e "\e[31m$@\e[0m"; }

function generate_ffmpeg_build_parameters()
{
	info "Generate FFmpeg build parameter."

	if $ENABLE_LOGAN; then
		PRODUCT_LINE_FLAGS="--enable-ni_logan --disable-ni_quadra"
	elif $ENABLE_QUADRA; then
		PRODUCT_LINE_FLAGS="--enable-ni_quadra --disable-ni_logan"
	fi

	if $ENABLE_DEBUG; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-optimizations --disable-asm --disable-stripping --enable-debug=3"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-x86asm --disable-debug"
	fi

	if $ENABLE_FFPLAY; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-ffplay"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-ffplay"
	fi

	if $ENABLE_FFPROBE; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-ffprobe"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-ffprobe"
	fi

	if $ENABLE_X264; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-libx264"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-libx264"
	fi

	if $ENABLE_X265; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-libx265"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-libx265"
	fi

	if $ENABLE_LIBAOM; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-libaom"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-libaom"
	fi

	if $ENABLE_LIBVPX; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-libvpx"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-libvpx"
	fi

	if $ENABLE_LIBXML; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-libxml2"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-libxml2"
	fi

	if $ENABLE_LIBSRT; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-libsrt"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-libsrt"
	fi

	if $ENABLE_FFNVCODEC; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --extra-cflags=-I/usr/local/cuda/targets/x86_64-linux/include --extra-ldflags=-L/usr/local/cuda/targets/x86_64-linux/lib --enable-cuda-nvcc --enable-cuda --enable-cuvid --enable-nvdec --enable-nvenc"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-cuda-nvcc --disable-cuda --disable-cuvid --disable-nvdec --disable-nvenc"
	fi

	if $ENABLE_VMAF; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-libvmaf --enable-version3"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-libvmaf"
	fi

	if $ENABLE_SHARED; then
		if $ENABLE_STATIC; then
			echo -e "\e[31mCannot use --shared and --static together. Exiting...\e[0m"
			exit 1
		fi
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --disable-static --enable-shared"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-static --disable-shared"
	fi

	if $ENABLE_STATIC; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --enable-static --disable-shared --extra-cflags=-static --extra-ldflags=-static"
	fi

	if $ENABLE_GSTREAMER_SUPPORT; then
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --extra-cflags=-DNI_DEC_GSTREAMER_SUPPORT"
	else
		EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} --extra-cflags=-UNI_DEC_GSTREAMER_SUPPORT"
	fi

	EXTRA_CONFIG_FLAGS="${EXTRA_CONFIG_FLAGS} ${CUSTOM_FLAGS}"

	warn "-->EXTRA_CONFIG_FLAGS=[$EXTRA_CONFIG_FLAGS]"
	warn "-->PRODUCT_LINE_FLAGS=[$PRODUCT_LINE_FLAGS]"
}

function concatenate_colon()
{
	local OLD_IFS="$IFS"
	local IFS=" "
	local array=($@)
	local val

	for str in ${array[@]}
	do
		if [ "$val" ]; then
			val=$val:$str
		else
			val=$str
		fi
	done

	echo "$val"
}

function add_export_env()
{
	local VAR="$1"
	shift
	local VAL=$(eval echo "\$$VAR")
	if [ "$VAL" ]; then
		VAL=$(concatenate_colon "$@" "$VAL");
	else
		VAL=$(concatenate_colon "$@");
	fi
	echo "export $VAR=\"$VAL\""
	eval "export $VAR=\"$VAL\""
}

function prefix_setup()
{
	local PREFIX="$1"

	add_export_env PATH "$PREFIX/bin"
	add_export_env LD_LIBRARY_PATH "$PREFIX/lib" "$PREFIX/lib/x86_64-linux-gnu" "$PREFIX/lib/aarch64-linux-gnu"  "$PREFIX/lib64"
	add_export_env PKG_CONFIG_PATH "$PREFIX/lib/pkgconfig/" "$PREFIX/lib/x86_64-linux-gnu/pkgconfig/" "$PREFIX/lib/aarch64-linux-gnu/pkgconfig" "$PREFIX/lib64/pkgconfig" "$PREFIX/share/pkgconfig/"
	add_export_env MANPATH "$PREFIX/share/man"
	export ACLOCAL_PATH="$PREFIX/share/aclocal"
	mkdir -p "$ACLOCAL_PATH"
	export ACLOCAL="aclocal -I $ACLOCAL_PATH"
	echo "ACLOCAL=$ALT_LOCAL"
}

function projectshell()
{
	export ALT_LOCAL="${OUTPUT}"
	mkdir -p "$ALT_LOCAL"
	prefix_setup "$ALT_LOCAL"
}

function whether_setenv()
{

	if [ ! -n "$ALT_LOCAL" ]; then
		echo "[ALT_LOCAL] is NULL"
		return 1;
	fi

	local path=$PATH
	local OLD_IFS="$IFS"
	local IFS=":"
	local array=($path)
	local mbin="$OUTPUT/bin"

	for str in ${array[@]}
	do
		if [ x"$str" == x"$mbin" ]; then
			return 0;
		fi
	done

	return 1;
}

function setenv()
{
	info "Config env."
	whether_setenv
	if [ "$?" -eq "0" ]; then
		echo "The environment variable has been initialized ..."
		echo "[ALT_LOCAL] is ${ALT_LOCAL}"
		return 0;
	fi

	projectshell
	echo "[ALT_LOCAL] is ${ALT_LOCAL}"
}

function download_ffmpeg()
{
	local ff_ver=$1
	local git_url="https://ghproxy.com/https://github.com/FFmpeg/FFmpeg.git"

	info "$FUNCNAME Version: $ff_ver"

	if [ ! -d $FFMPEG_DIR ]; then
		echo "GIT URL:$git_url"
		git clone -b $ff_ver --depth=1 $git_url $FFMPEG_DIR
		if [ $? -ne 0 ]; then
			error "git clone $git_url failed."
			return -1
		fi
	else
		cd $FFMPEG_DIR
		git checkout $ff_ver
		git checkout .
		cd -
	fi
	cd $FFMPEG_DIR
	git log -1
	cd -
}

function config_android_env()
{
	if [ -z ${ANDROID_NDK_ROOT} ]; then
		warn "You must set ANDROID_NDK_ROOT environment variable"
		warn " Please download NDK r21d from https://dl.google.com/android/repository/android-ndk-r21d-linux-x86_64.zip"
		return -1
	fi

	TOOLCHAIN=${ANDROID_NDK_ROOT}/toolchains/llvm/prebuilt/linux-x86_64

	info "Config Android ENV, ANDROID_NDK_API=$ANDROID_NDK_API, ANDROID_ARCH=$ANDROID_ARCH"

	if [ "${ANDROID_ARCH}" = "arm" ]; then
		ARCH=arm
		ARCH2=armv7a
		CPU=armv7-a
		CC_SUFFIX=eabi
		CROSS_PREFIX=$TOOLCHAIN/bin/arm-linux-androideabi-
	elif [ "${ANDROID_ARCH}" = "arm64" ]; then
		ARCH=arm64
		ARCH2=aarch64
		CPU=armv8-a
		CROSS_PREFIX=$TOOLCHAIN/bin/$ARCH2-linux-android-
	elif [ "${ANDROID_ARCH}" = "x86" ]; then
		ARCH=x86
		ARCH2=i686
		CPU=i686
		CROSS_PREFIX=$TOOLCHAIN/bin/$ARCH2-linux-android-
	elif [ "${ANDROID_ARCH}" = "x86_64" ]; then
		ARCH=x86_64
		ARCH2=x86_64
		CPU=x86_64
		CROSS_PREFIX=$TOOLCHAIN/bin/$ARCH2-linux-android-
	elif [ "${ANDROID_ARCH}" = "" ]; then
		ARCH=x86_64
		ARCH2=x86_64
		CPU=x86_64
		CROSS_PREFIX=$TOOLCHAIN/bin/$ARCH2-linux-android-
	else
		error "Error - unknown option for --ANDROID_ARCH. Select from: [arm,arm64,x86,x86_64]"
		return -1
	fi

	info "Building android ARCH=${ARCH}"
}

function build_quadra_depend_sw()
{
	info "Build Quadra depend on software."

	echo "Copy: FFMPEG_DIR=$FFMPEG_DIR"
	# 打补丁
	cp ${TOP}/FFmpeg-n4.3_netint_v4.5.3_RC3.diff $FFMPEG_DIR

	cd $FFMPEG_DIR
	git checkout .
	patch -t -p 1 < FFmpeg-n4.3_netint_v4.5.3_RC3.diff
	cd -

	# 编译libxcoder
	local libxcoder_dir=${TOP}/libxcoder

	if [ -z $ALT_LOCAL ]; then
		warn "Please set the environment variables first."
		return -1
	fi

	echo "CROSS_PREFIX=$CROSS_PREFIX"
	cd $libxcoder_dir
	set -x
	./configure --prefix=${ALT_LOCAL} --without-self-kill --with-android
	make
	make install
	# 使用android源码中编译好的库进行替换
	cp ${TOP}/libxcoder.so ${ALT_LOCAL}/lib
	set +x
	cd -
	pkg-config --libs xcoder

}

function build_ffmpeg_for_android()
{
	local ff_ver=$1

	info "Build ffmpeg ${ff_ver} for android."

	echo "ARCH=$ARCH, ARCH2=$ARCH2, CPU=$CPU"
	echo "CROSS_PREFIX=$CROSS_PREFIX"

	if [ -z $CROSS_PREFIX ] || [ -z $ARCH2 ]; then
		warn "Please configure the android compilation environment variable."
		return -1
	fi

	if [ ! -f ${CROSS_PREFIX}ld ]; then
		warn "ANDROID_NDK_ROOT configure error. ANDROID_NDK_ROOT=$ANDROID_NDK_ROOT"
		return -1
	fi

	# for NETINT
	if $ENABLE_QUADRA;then
		build_quadra_depend_sw
	fi

	cd $FFMPEG_DIR
	local PREFIX="${ALT_LOCAL}"
	echo "ALT_LOCAL=[$ALT_LOCAL]"
	set -x

	./configure \
		--prefix=$PREFIX \
		--enable-cross-compile \
		--sysroot=$TOOLCHAIN/sysroot \
		--arch=$ARCH2 \
		--cpu=$CPU \
		--target-os='android' \
		--cc=$TOOLCHAIN/bin/$ARCH2-linux-android${CC_SUFFIX}${ANDROID_NDK_API}-clang \
		--cross-prefix=$CROSS_PREFIX \
		--pkg-config=$(which pkg-config) \
		--pkg-config-flags='--static' \
		--enable-gpl --enable-nonfree \
		--extra-ldflags='-fuse-ld=gold -lm' \
		${PRODUCT_LINE_FLAGS} \
		--extra-libs=-lgcc \
		--enable-pic \
		--extra-cflags='-DANDROID -D_ANDROID -D__ANDROID__' \
		--enable-pthreads \
		--enable-encoders --enable-decoders --enable-avfilter --enable-muxers --enable-demuxers --enable-parsers \
		${EXTRA_CONFIG_FLAGS}

	ret=$?
	set +x

	if [ $ret != 0 ]; then
		error "Configuration failed. Exiting..."
		return -1;
	else
		set -x
		make clean
		make
		make install
		set +x
	fi

	cd -
}

function build_ffmpeg_for_linux()
{
	local ff_ver=$1

	info "TODO: Build ffmpeg for linux. Version=$ff_ver"
}

function build_ffmpeg()
{
	local ff_ver=$1

	info "Build FFmpeg version: $ff_ver"

	if [ -z $ALT_LOCAL ]; then
		warn "Please set the environment variables first."
		return -1
	fi

	# Download
	download_ffmpeg	$ff_ver

	generate_ffmpeg_build_parameters

	if $ANDROID_BUILD; then
		config_android_env
		build_ffmpeg_for_android $ff_ver
	else
		build_ffmpeg_for_linux $ff_ver
	fi

}

function archiving_libs_for_android()
{
	local archiving_dir="${TOP}/android-prebuild"

	info "Archiving FFmpeg lib for android."

	if [ -z $ALT_LOCAL ]; then
		warn "Please set the environment variables first."
		return -1
	fi

	rm $archiving_dir -r
	[ -d $archiving_dir ] || mkdir $archiving_dir

	[ -d $archiving_dir/$ANDROID_ARCH/bin ] || mkdir -p $archiving_dir/$ANDROID_ARCH/bin
	[ -d $archiving_dir/$ANDROID_ARCH/lib ] || mkdir -p $archiving_dir/$ANDROID_ARCH/lib

	set -x
	cp $ALT_LOCAL/bin/ffmpeg $archiving_dir/$ANDROID_ARCH/bin
	cp $ALT_LOCAL/lib/libavcodec.so $archiving_dir/$ANDROID_ARCH/lib
	cp $ALT_LOCAL/lib/libavdevice.so $archiving_dir/$ANDROID_ARCH/lib
	cp $ALT_LOCAL/lib/libavfilter.so $archiving_dir/$ANDROID_ARCH/lib
	cp $ALT_LOCAL/lib/libavformat.so $archiving_dir/$ANDROID_ARCH/lib
	cp $ALT_LOCAL/lib/libavutil.so $archiving_dir/$ANDROID_ARCH/lib
	cp $ALT_LOCAL/lib/libpostproc.so $archiving_dir/$ANDROID_ARCH/lib
	cp $ALT_LOCAL/lib/libswresample.so $archiving_dir/$ANDROID_ARCH/lib
	cp $ALT_LOCAL/lib/libswscale.so $archiving_dir/$ANDROID_ARCH/lib

	# Depend NDK libs
	cp $ANDROID_NDK_ROOT/platforms/android-${ANDROID_NDK_API}/arch-${ANDROID_ARCH}/usr/lib/libcamera2ndk.so $archiving_dir/$ANDROID_ARCH/lib
	cp $ANDROID_NDK_ROOT/platforms/android-${ANDROID_NDK_API}/arch-${ANDROID_ARCH}/usr/lib/libandroid.so $archiving_dir/$ANDROID_ARCH/lib
	cp $ANDROID_NDK_ROOT/platforms/android-${ANDROID_NDK_API}/arch-${ANDROID_ARCH}/usr/lib/libmediandk.so $archiving_dir/$ANDROID_ARCH/lib
	set +x
}


options=("Setup Environment variables" \
	"Download FFmpeg")

options+=("Enable NETINIT Quadra xcoder")

options+=("Build FFmpeg-n4.3")
if $ANDROID_BUILD; then
	options+=("Archiving for Android")
fi
options+=("Quit")

[ ! -d $BASE_DIR ] && mkdir $BASE_DIR
# Main menu loop
while true; do
	cd $BASE_DIR
	echo "Choose an option:"
	select opt in "${options[@]}"; do
		case $opt in
			"Setup Environment variables")
				info "You chose $REPLY which is $opt"
				setenv
				break
				;;
			"Download FFmpeg")
				info "You chose $REPLY which is $opt"
				download_ffmpeg master
				break
				;;
			"Enable NETINIT Quadra xcoder")
				info "You chose $REPLY which is $opt"
				ENABLE_QUADRA=true;
				info "Build for NETINIT. ENABLE_QUADRA=$ENABLE_QUADRA"
				break;
				;;
			Build\ FFmpeg-*)
				info "You chose $REPLY which is $opt"
				FFMPEG_VER=$(echo ${opt} | grep -Poh 'Build FFmpeg-\K.*')
				FFMPEG_DIR=${BASE_DIR}/${FFMPEG_VER}
				echo "FFmpeg version: $FFMPEG_VER, Dir: $FFMPEG_DIR"
				setenv
				build_ffmpeg $FFMPEG_VER
				break
				;;
			"Archiving for Android")
				info "You chose $REPLY which is $opt"
				archiving_libs_for_android
				break
				;;
			"Quit")
				exit
				;;
			*) error "Invalid choice!"
				;;
		esac
	done
done
