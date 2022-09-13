#!/bin/bash
# Note:
# OS: ubuntu18.04, centos7.8
# Platform: aarch64, x86_64
# 软件依赖参考：https://www.linuxfromscratch.org/blfs/view/svn/index.html

PWD=`pwd`

QEMU_VERSION=4.2.0
EPOXY_VERSION=1.5.4
VIRGL_VERSION=0.9.1
SPICE_VERSION=0.14.3
SPICE_PROTOCOL_VERSION=0.14.0
LIBUSB_VERSION=1.0.23
USBREDIR_VERSION=0.8.0
SPICE_GTK_VERSION=0.37  #for spicy
LIBVIRT_VERSION=4.9.0

machine=`uname -m`
make_jobs=`cat /proc/cpuinfo | grep "processor" | wc -l`
ninja="ninja"
re_install=0
server=0
ALT_LOCAL=${ALT_LOCAL:-/usr/local/}

_QEMU_FILE=qemu-$QEMU_VERSION.tar.xz
_QEMU_URL=https://download.qemu.org
_QEMU_DIR=qemu-$QEMU_VERSION

_EPOXY_FILE=libepoxy-$EPOXY_VERSION.tar.xz
_EPOXY_URL=https://github.com/anholt/libepoxy/releases/download/$EPOXY_VERSION
_EPOXY_DIR=libepoxy-$EPOXY_VERSION

_VIRGL_FILE=virglrenderer-virglrenderer-$VIRGL_VERSION.tar.gz
_VIRGL_URL=https://gitlab.freedesktop.org/virgl/virglrenderer/-/archive/virglrenderer-$VIRGL_VERSION
_VIRGL_DIR=virglrenderer-virglrenderer-$VIRGL_VERSION

_SPICE_FILE=spice-$SPICE_VERSION.tar.bz2
_SPICE_URL=https://www.spice-space.org/download/releases
_SPICE_DIR=spice-$SPICE_VERSION

_SPICE_PROTOCOL_FILE=spice-protocol-$SPICE_PROTOCOL_VERSION.tar.bz2
_SPICE_PROTOCOL_URL=https://www.spice-space.org/download/releases
_SPICE_PROTOCOL_DIR=spice-protocol-$SPICE_PROTOCOL_VERSION

_LIBUSB_FILE=libusb-$LIBUSB_VERSION.tar.bz2
_LIBUSB_URL=https://github.com/libusb/libusb/releases/download/v$LIBUSB_VERSION
_LIBUSB_DIR=libusb-$LIBUSB_VERSION

_USBREDIR_FILE=usbredir-$USBREDIR_VERSION.tar.bz2
_USBREDIR_URL=https://www.spice-space.org/download/usbredir
_USBREDIR_DIR=usbredir-$USBREDIR_VERSION

_SPICE_GTK_FILE=spice-gtk-$SPICE_GTK_VERSION.tar.bz2
_SPICE_GTK_URL=https://www.spice-space.org/download/gtk
_SPICE_GTK_DIR=spice-gtk-$SPICE_GTK_VERSION

_LIBVIRT_FILE=libvirt-$LIBVIRT_VERSION.tar.xz
_LIBVIRT_URL=https://libvirt.org/sources
_LIBVIRT_DIR=libvirt-$LIBVIRT_VERSION

function install_depend_sw_for_ubuntu() {
    echo "Install Qemu dependent software for ubuntu ..."

    sudo apt install -y libglib2.0-dev libpixman-1-dev libgtk-3-dev \
        libsdl2-dev libspice-server-dev libusbredirhost-dev  \
        libusb-1.0-0-dev flex bison \
        python3 python3-pip ninja-build \
        libgles2-mesa-dev spice-client-gtk \
        autoconf libtool gettext \
        libdrm-dev libgbm-dev
    if [ $? -ne 0 ]; then
        echo "Install dependent software fail!"
        exit
    fi
}
function install_depend_sw_centos() {
    echo "Install Qemu dependent software for Centos ..."

    sudo yum install -y epel-release
    sudo yum install -y python3.$machine python3-pip.noarch ninja-build.$machine \
        libtool.$machine gcc-c++.$machine glib2-devel.$machine zlib-devel.$machine \
        mesa-libGL-devel.$machine mesa-libgbm-devel.$machine SDL2-devel.$machine\
        orc-devel.$machine opus-devel.$machine pixman-devel.$machine \
        systemd-devel.$machine
    # for libvirt
    sudo yum install -y libpciaccess-devel.$machine yajl-devel.$machine \
	device-mapper-devel.$machine gnutls-devel.$machine libnl-devel.$machine \
	libxml2-devel.$machine
    if [ $? -ne 0 ]; then
        echo "Install dependent software fail!"
        exit
    fi
}

function install_build_tools() {
    echo "Install build tool ..."
    #sudo pip3 install --upgrade pip
    sudo pip3 install meson
    if [ $? -ne 0 ]; then
        echo "Install local build tool ..."
        sudo pip3 install --no-index --find-links=$PWD/tools/ meson
    fi
    if [ $? -ne 0 ]; then
        echo "Install build tools fail!"
        exit
    fi
}

function unzip() {
    if [ -f $1 ] ; then
        case $1 in
            *.tar.bz2)   tar xjf $1     ;;
            *.tar.gz)    tar xzf $1     ;;
            *.bz2)       bunzip2 $1     ;;
            *.rar)       unrar e $1     ;;
            *.gz)        gunzip $1      ;;
            *.tar)       tar xf $1      ;;
            *.tar.xz)    tar xf $1      ;;
            *.tbz2)      tar xjf $1     ;;
            *.tgz)       tar xzf $1     ;;
            *.zip)       unzip $1       ;;
            *.Z)         uncompress $1  ;;
            *.7z)        7z x $1        ;;
            *)     echo "'$1' cannot be extracted via extract()" ;;
        esac
    else
        echo "'$1' is not a valid file"
        exit 1
    fi
}

function download_and_unzip_file()
{
    local name=$1
    local url=$2
    local file=$3
    local dir=$4

    sudo -v #update user's timestamp
    echo "Download $name ..."

    if [ -f ${file} ]; then
        echo "$name file exists"
    else
        wget ${url}/${file}
        if [ $? -ne 0 ]; then
            return 1;
        fi
    fi

    echo "Unzip file ${file} ..."
    if [ -d "$dir" ]; then
        echo "$name unzip file exists"
    else
        unzip ${file}
        if [ $? -ne 0 ]; then
            return 2;
        fi
    fi
    sudo -v
    echo "Download complete"
}

function check_need_install() {
    local module=$1
    local version=$2

    if [ $re_install -eq 1 ]; then
        #Not installed
        echo ""
        return 0
    fi

    pkg-config --atleast-version=$version $module
    if [ $? -eq 0 ]; then
        #Installed
        echo "exists"
        return 1
    else
        #Not installed
        echo ""
        return 0
    fi
}

function check_is_success() {
    local ret=$1

    if [ $ret -ne 0 ]; then
        echo "[Error]: Compile and install failed ..."
        exit 1
    fi
}

function install_epoxy() {
    echo "Install libepoxy ..."

    if [ $(check_need_install epoxy $EPOXY_VERSION) ]; then
        echo "libepoxy has been installed!"
        return 0
    fi

    download_and_unzip_file libepoxy $_EPOXY_URL $_EPOXY_FILE $_EPOXY_DIR
    if [ $? -ne 0 ]; then
        echo "Download or Unzip error!"
        return;
    fi

    echo "Build epoxy ..."
    pushd $PWD
    cd $_EPOXY_DIR

    sudo rm _build -rf
    mkdir _build && cd _build
    meson setup --prefix $ALT_LOCAL
    $ninja
    sudo $ninja install
    check_is_success $?
    sudo ldconfig

    echo "Install complete."
    popd
}

function install_virgl() {
    echo "Install virglrenderer ..."

    if [ $(check_need_install virglrenderer $VIRGL_VERSION) ]; then
        echo "virglrenderer has been installed!"
        return 0
    fi

    local virgl_dir=$_VIRGL_DIR

    if [ "$1" == "new" ];then
        virgl_dir=virglrenderer-dev
        if [ -d $virgl_dir ]; then
            echo "virglrenderer-dev directory exists"
        else
            git clone http://172.16.200.191/shaowangquan/virglrenderer-dev.git $virgl_dir
        fi
    else
        download_and_unzip_file virglrenderer $_VIRGL_URL $_VIRGL_FILE $virgl_dir
    fi

    echo "Build virglrenderer ..."
    pushd $PWD
    cd $virgl_dir

    sudo rm _build -rf
    mkdir _build && cd _build
    meson setup --prefix $ALT_LOCAL
    $ninja
    sudo $ninja install

    #sudo make distclean
    #./autogen.sh --enable-autotools
    ##./configure --enable-autotools --enable-gbm-allocation
    #./configure --enable-autotools --prefix=$ALT_LOCAL
    #make -j$make_jobs
    #sudo make install
    check_is_success $?

    sudo ldconfig

    echo "Install complete."
    popd
}

function install_qemu() {
    echo "Install qemu ..."

    local version=`qemu-system-$machine --version | grep "version" | awk '{print $4}'`
    if [ "$QEMU_VERSION" == "$version" ] && [ $re_install -ne 1 ]; then
        echo "***** Qemu $version installed! *****"
        return 0
    fi

    download_and_unzip_file qemu $_QEMU_URL $_QEMU_FILE $_QEMU_DIR

    echo "Build Qemu for $machine  ..."
    pushd $PWD
    cd $_QEMU_DIR

    sudo make distclean
    ./configure --prefix=$ALT_LOCAL \
        --target-list=$machine-softmmu --enable-vhost-vsock \
        --enable-kvm \
        --enable-usb-redir \
        --enable-virglrenderer --enable-sdl
    make -j$make_jobs
    sudo make install
    check_is_success $?
    sudo ldconfig

    echo "Install complete."
    popd
}

function install_spice() {
    echo "Install spice ..."

    if [ $(check_need_install spice-server $SPICE_VERSION) ]; then
        echo "spice server has been installed!"
        return 0
    fi

    download_and_unzip_file spice $_SPICE_URL $_SPICE_FILE $_SPICE_DIR

    echo "Build spice ..."
    pushd $PWD
    cd $_SPICE_DIR

    sudo make distclean
    ./configure --prefix=$ALT_LOCAL  --enable-gstreamer=yes --enable-opus=yes
    make -j$make_jobs
    sudo make install
    check_is_success $?
    sudo ldconfig

    echo "Install complete."
    popd
}

function install_spice_protocol() {
    echo "Install spice protocol ..."

    if [ $(check_need_install spice-protocol $SPICE_PROTOCOL_VERSION) ]; then
        echo "spice-protocol has been installed!"
        return 0
    fi

    download_and_unzip_file spice-protocol $_SPICE_PROTOCOL_URL $_SPICE_PROTOCOL_FILE $_SPICE_PROTOCOL_DIR

    echo "Build spice protocol ..."
    pushd $PWD
    cd $_SPICE_PROTOCOL_DIR

    sudo make distclean
    ./configure --prefix=$ALT_LOCAL
    make -j$make_jobs
    sudo make install
    check_is_success $?
    sudo ldconfig

    echo "Install complete."
    popd
}

function install_libusb() {
    echo "Install libusb ..."

    if [ $(check_need_install libusb-1.0 $LIBUSB_VERSION) ]; then
        echo "libusb has been installed!"
        return 0
    fi

    download_and_unzip_file libusb $_LIBUSB_URL $_LIBUSB_FILE $_LIBUSB_DIR

    echo "Build libusb ..."
    pushd $PWD
    cd $_LIBUSB_DIR

    sudo make distclean
    ./configure --prefix=$ALT_LOCAL
    make -j$make_jobs
    sudo make install
    check_is_success $?
    sudo ldconfig

    echo "Install complete."
    popd
}

function install_usbredir() {
    echo "Install usbredir ..."

    if [ $(check_need_install libusbredirhost $USBREDIR_VERSION) ]; then
        echo "libusbredir has been installed!"
        return 0
    fi

    download_and_unzip_file usbredir $_USBREDIR_URL $_USBREDIR_FILE $_USBREDIR_DIR

    echo "Build usbredir ..."
    pushd $PWD
    cd $_USBREDIR_DIR

    sudo make distclean
    ./configure --prefix=$ALT_LOCAL
    make -j$make_jobs
    sudo make install
    check_is_success $?
    sudo ldconfig

    echo "Install complete."
    popd
}

function install_spice_gtk() {
    echo "Install spice gtk ..."

    if [ $(check_need_install spice-client-gtk-3.0 $USBREDIR_VERSION) ]; then
        echo "spice-client-gtk has been installed!"
        return 0
    fi
    download_and_unzip_file spice-gtk $_SPICE_GTK_URL $_SPICE_GTK_FILE $_SPICE_GTK_DIR

    echo "Build spice-gtk ..."
    pushd $PWD
    cd $_SPICE_GTK_DIR

    sudo make distclean
    ./configure --prefix=$ALT_LOCAL
    make -j$make_jobs
    sudo make install
    check_is_success $?
    sudo ldconfig

    echo "Install complete."
    popd
}

# for centos7.8 test OK
function install_libvirt() {
    echo "Install libvirt ..."

    if [ $(check_need_install libvirt $LIBVIRT_VERSION) ]; then
        echo "libvirt has been installed!"
        return 0
    fi
    download_and_unzip_file libvirt $_LIBVIRT_URL $_LIBVIRT_FILE $_LIBVIRT_DIR

    echo "Build libvirt ..."
    pushd $PWD
    cd $_LIBVIRT_DIR

    sudo make distclean
    #./configure --prefix=$ALT_LOCAL
    ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var
    make -j$make_jobs
    sudo make install
    check_is_success $?
    sudo ldconfig
    sudo systemctl daemon-reload
    sudo systemctl restart libvirtd.service


    echo "Install complete."
    popd
}

function configure_env_var() {
    local bashrc=$HOME/.bashrc
    echo "Configure environment variables ..."
    if [ $is_source ]; then
        echo "Shell {source setenv.sh} has been executed"
        return 1;
    fi

    #Runtime lib
    echo "/usr/local/lib" > .usr-local.conf
    echo "/usr/local/lib64/" >> .usr-local.conf

    sudo cp .usr-local.conf /etc/ld.so.conf.d/usr-local.conf
    rm .usr-local.conf
    sudo ldconfig

    #Build lib
    cmd='export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig:/usr/local/lib64/pkgconfig:/usr/local/share/pkgconfig'
    echo "$cmd"
    $cmd

    grep "$cmd" $bashrc >> /dev/null
    if [ $? -eq 0 ]; then
        return 1;
    fi

    echo "$cmd" >> $bashrc
}

function build_env_for_ubuntu() {
    echo "Build env for ubuntu"

    install_depend_sw_for_ubuntu
    install_build_tools

    install_epoxy
    install_virgl

    if [ $server -eq 1 ]; then
        return 1
    fi

    install_qemu
}

function build_env_for_centos() {
    echo "Build env for Centos"

    install_depend_sw_centos
    install_build_tools
    configure_env_var

    install_epoxy
    install_virgl

    if [ $server -eq 1 ]; then
        return 1
    fi

    #nstall_spice_protocol
    #install_spice
    install_libusb
    install_usbredir

    install_qemu

    #install_spice_gtk #spicy

    #install_libvirt
}

function usage() {
    echo "Usage: $0"
    echo "  -s : build for server"
    echo "  -r : rebuild and install"
    echo "  -h : help"
    exit 0
}

function parse_args() {
    while getopts "hsr" opt
    do
        case $opt in
            s) server=1;;
            r) re_install=1;;
            h) usage;;
            ?) usage;;
        esac
    done
}

function main() {
    echo "Current dir: $PWD"

    parse_args $*

    echo "*********************************************************************"
    echo -e "*    Install Path: \e[01;31m$ALT_LOCAL\e[0m"
    echo "*********************************************************************"

    echo -n "Do you want to continue? [Y/n]"
    read enter_x
    if [ x$enter_x != xy ] && [ x$enter_x != xY ]; then
        echo "Exit the build environment ..."
        exit
    fi

    if (( $(cat /etc/*-release | grep -w "CentOS" | wc -l) > 0 ))
    then
        echo "Centos Entry..."
        ninja="ninja-build"
        build_env_for_centos
    else
        echo "Ubuntu Entry..."
        ninja="ninja"
        build_env_for_ubuntu
    fi
}

sudo -l
main $*
