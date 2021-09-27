#!/bin/bash -p

DRY_RUN=false
EXEC=eval

fail()
{
    [ $# -gt 0 ] && echo "$*" 1>&2
    exit 1
}

check_file_ext()
{
    local filename=$1
    local filetype=

    case "$filename" in
        *.tar.gz|*.tgz) filetype="gzip compressed data" ;;
        *) return 1 ;;
    esac

    [ "$(file -b $filename | awk -F, '{print $1}')" == "$filetype" ] && return 0 || return 1
}

uname_to_dpkg_arch()
{
    local uname_arch=$1
    local dpkg_arch=

    case "$uname_arch" in
        x86_64|amd64) dpkg_arch=amd64 ;;
        aarch64|arm64) dpkg_arch=arm64 ;;
        arm|armv6l|armv7l|armhf) dpkg_arch=armhf ;;
        *) ;;
    esac

    [ -z "$dpkg_arch" ] && return 1
    echo "$dpkg_arch"
}

print_control()
{
    local pkgname=$1
    local pkgver=$2
    local arch=$3
    local maintainer=$([ -n "$4" ] && echo "Maintainer: $4")
    local depends=$([ -n "$5" ] && echo "Depends: $5")
    local description=$([ -n "$6" ] && echo "Description: $6")
    local license=$([ -n "$7" ] && echo "License: $7")

    local depends_str=
    local description_str=

    local control=$(cat << EOM
Package: $pkgname
$maintainer
Version: $pkgver
Architecture: $arch
$depends
$description
$license
EOM
)
    echo "$control" | sed '/^$/d'
}

gendeb()
{
    local pkgname=$1
    local pkgver=$2
    local arch=$(uname_to_dpkg_arch "$3")
    local tarfile=$(realpath $4)
    local description=${5:-}
    local maintainer=${6:-}
    local depends=${7:-}
    local license=${8:-}
    local outdir=${9:-$(pwd)}

    # Validate package name, version and arch strings
    [[ "$pkgname" =~ ^.*_.*$ ]] && fail "Package name must not contain '_'."
    [[ "$pkgver" =~ ^.*_.*$ ]] && fail "Package version must not contain '_'."
    [ -z "$arch" ] && fail "Invalid ARCH string. Use one emitted from 'uname -m' or 'dpkg --print-architecture'"

    # Validate input tarball
    ! check_file_ext "$tarfile" && fail "Invalid file format. Must be gzip compressed data."

    # Create tmpdir for building .deb and ensure that it's removed when script exits
    local tmpdir=$(mktemp -d)
    trap "rm -rf $tmpdir" INT EXIT

    # Absolute path of generated .deb file
    local pkg_prefix=${pkgname}_${pkgver}_${arch}
    local pkgdir=$tmpdir/$pkg_prefix
    local debdir=$pkgdir/DEBIAN
    local debfile=$tmpdir/$pkg_prefix.deb

    local build_steps=$(cat << EOM
        (cd $tmpdir && \
            mkdir -p $debdir && \
            tar xfz $tarfile -C $pkgdir && \
            print_control "$pkgname" "$pkgver" "$arch" "$maintainer" "$depends" "$description" "$license" > $debdir/control && \
            dpkg-deb --build --root-owner-group $pkgdir)
EOM
)

    local copy_steps=$(cat << EOM
        cp -f $debfile $(realpath $outdir)/.
EOM
)

    $EXEC "$build_steps && $copy_steps"
}

print_help()
{
    local prog_name=$(basename -s .sh "$1")
    local HELPSTR=$(cat <<- EOM
$prog_name -p <pkgname> -a <pkgarch> -v <pkgver> -f <pkgdata.tar.gz> [-s <description>] [-d <depends>] [-l <license>] [-m <maintainer>] [-o <outdir>] [-A <arch-string>] [-h]

    Generate .deb file for a .tar.gz (gzip compressed data) file

    -p  Package name (must not contain '_')
    -a  Target architecture for the package (output of "dpkg --print-architecture" or "uname -m")
    -v  Package version (must not contain '_')
    -f  Gzip compressed data to create .deb for
    -s  Package description string
    -d  Package dependency list as comma separated strings
    -l  Package license string
    -m  Package maintainer string (recommended format: "name <email>")
    -o  Output dir to place generated .deb file in
    -A  Print dpkg-arch value (i.e. "dpkg --print-architecture") for <arch-string>
    -n  Dry run (show commands that will run without executing them)
    -h  Show this message
EOM
)
    echo "$HELPSTR"
}

main()
{
    local arch= tarfile= license= description= depends= maintainer= outdir= pkgname= pkgver=

    OPTIND=1
    while getopts "A:a:d:f:hl:m:no:p:s:v:" arg; do
        case $arg in
            A) uname_to_dpkg_arch "$OPTARG"; exit $? ;;
            a) arch=$OPTARG ;;
            d) depends=$OPTARG ;;
            f) tarfile=$OPTARG ;;
            l) license=$OPTARG ;;
            m) maintainer=$OPTARG ;;
            n) DRY_RUN=true; EXEC=echo ;;
            o) outdir=$OPTARG ;;
            p) pkgname=$OPTARG ;;
            s) description=$OPTARG ;;
            v) pkgver=$OPTARG ;;
            h) print_help "$0"; exit 0 ;;
            ?) fail ;;
        esac
    done
    shift $((OPTIND - 1))

    # Check args
    [ -z "$pkgname" -o -z "$pkgver" -o -z "$arch" -o -z "$tarfile" ] && print_help "$0" && fail
    [ ! -f "$tarfile" ] && fail "'$tarfile' doesn't exist."
    [ -n "$outdir" -a ! -d "$outdir" ] && fail "'$outdir' doesn't exist."

    gendeb "$pkgname" "$pkgver" "$arch" "$tarfile" "$description" "$maintainer" "$depends" "$license" "$outdir"
}

main "$@"
