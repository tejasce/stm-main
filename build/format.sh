#!/bin/bash -p

TOPDIR=$(git rev-parse --show-toplevel 2>/dev/null)
PYTHON=python3
DRY_RUN=false

# .clang-format at $TOPDIR specifies style for all but C srcs
SUPPORTED_EXTS=".c .h .cc .hh .cpp .hpp"
# Separate clang-format file for C srcs
C_CLANG_FORMAT_FILE=$TOPDIR/build/clang-format-c
# User may override use of .clang-format at root of workspace
DEFAULT_CLANG_FORMAT_FILE=

fail()
{
    [ $# -gt 0 ] && echo "$*" 1>&2
    exit 1
}

print_clang_format_style()
{
    local file=$(basename $1)
    local clang_format_file= clang_format_style=

    # Override .clang-format file, if requested,
    # and supply it as JSON string to "style=" arg
    if [[ "$file" =~ .*\.[c|h]$ ]]; then
        clang_format_file=$C_CLANG_FORMAT_FILE
    else
        clang_format_file=$DEFAULT_CLANG_FORMAT_FILE
    fi

    if [ -n "$clang_format_file" ]; then
        clang_format_style="$(echo -e "import sys, yaml, json;\nwith open(\"$clang_format_file\", 'r') as yaml_in:\n\tjson.dump(yaml.safe_load(yaml_in), sys.stdout)" | $PYTHON)"
    else
        # Default. Use .clang-format at the root of this workspace
        clang_format_style=file
    fi
    echo "$clang_format_style"
}

clang_format_files()
{
    local files=$1
    local testing=$2
    local rc=0

    local outfile= clang_format_cmd=
    for file in $files; do
        clang_format_cmd="clang-format -style='$(print_clang_format_style $file)' $file"
        if ! diff -qs <(eval "$clang_format_cmd") <(cat "$file") >/dev/null; then
            if [ "$DRY_RUN" = "true" ]; then
                echo "Reformatting needed for $file.."
                [ $rc -eq 0 ] && rc=1
            else
                outfile=$(eval "$clang_format_cmd")
                echo "Reformatting $file.."
                if [ "$testing" = "true" ]; then
                    echo "$outfile"
                else
                    echo "$outfile" > "$file"
                fi;
            fi
        fi
    done
    if [ "$DRY_RUN" = "true" ] && [ $rc -eq 0 ] && [ -n "$files" ]; then
        echo "Reformatting not needed."
    fi
    return $rc
}

clang_format_modified_files()
{
    local modified_files=
    local git_exts=

    for ext in $SUPPORTED_EXTS; do
        git_exts+="'*$ext' "
    done

    # Find modified+newly staged C/C++ files (with "" arg) and all the
    # C/C++ files that are changed between top of master and this branch
    for commit in "" "master..HEAD"; do
        modified_files+="$(eval git diff --name-only $commit $git_exts) "
    done
    clang_format_files "$modified_files"
}

clang_format_dir_files()
{
    local dirlist=$1
    local dir_files=

    for ext in $SUPPORTED_EXTS; do
        dir_files+="$(find "$dirlist" -name "*$ext") "
    done
    clang_format_files "$dir_files"
}

print_help()
{
    local prog_name=$(basename "$1")
    HELPSTR=$(cat <<- EOM
$prog_name [-c <c-clang-format-file>] [-f <"file1[ file2]..">] [-d <"dir1[ dir2]..">] [-m] [-n] [-t] [-h]

    Format src/hdr files using clang-format (Supported: $SUPPORTED_EXTS)
    Uses '.clang-format' at the root of workspace for all but C files
    Assumes C++ (src/hdr) files uses different extension than C files (i.e. .[c|h])

    -C  clang-format file to use instead of .clang-format at the root of workspace
    -c  clang-format file for C files (default: ${C_CLANG_FORMAT_FILE#$TOPDIR/})
    -d  format files under <dirs> (mutually exclusive to '-f')
    -f  format <files>            (mutually exclusive to '-d')
    -m  format modified files on current branch
    -n  dry run
    -t  same as '-f' but for testing with '-c' or '-C'; dumps clang-format output to stdout w/o changing file
    -h  print this message
EOM
)
    echo "$HELPSTR"
}

main()
{
    local opt_m= opt_t=
    local dirlist= filelist=

    while getopts "C:c:d:f:hmnt:" arg; do
        case $arg in
            C) DEFAULT_CLANG_FORMAT_FILE=$OPTARG ;;
            c) C_CLANG_FORMAT_FILE=$OPTARG ;;
            d) dirlist=$OPTARG ;;
            f) filelist=$OPTARG ;;
            h) print_help "$0"; exit 0 ;;
            m) eval "opt_$arg=true";;
            n) DRY_RUN=true ;;
            t) eval "opt_$arg=true"; filelist=$OPTARG ;;
            ?) fail ;;
        esac
    done
    shift $((OPTIND - 1))

    if [ "$opt_m" = "true" ]; then
        clang_format_modified_files
    elif [ -n "$filelist" ]; then
        clang_format_files "$filelist" "$opt_t"
    elif [ -n "$dirlist" ]; then
        clang_format_dir_files "$dirlist"
    else
        print_help "$0"
    fi
}

main "$@"
