#!/usr/bin/env bash
set -Eeuo pipefail

usage() {
    cat <<'EOF'
Usage: bash build-wine.sh [build|debug|clean]

build    Incremental GCCE UREL build and unsigned SIS package
debug    Incremental GCCE UDEB build and unsigned debug SIS package
clean    Remove GCCE output, packages, and generated repository output

Environment overrides:
  NARUTO_WINE_PREFIX       Wine prefix (default: ~/.wine-symbian)
  NARUTO_WINE_BIN          wine executable
  NARUTO_WINESERVER_BIN    wineserver executable
  NARUTO_SYMBIAN_SDK_ROOT          host SDK root inside the Wine prefix
  NARUTO_SYMBIAN_SDK_WINDOWS_ROOT  matching Windows SDK root
  NARUTO_PIXEL_BUILD_JOBS          parallel GNU Make jobs (default: up to 6)
EOF
}

BUILD_PROFILE=${1:-build}
case "$BUILD_PROFILE" in
    build|debug|clean)
        ;;
    -h|--help)
        usage
        exit 0
        ;;
    *)
        usage >&2
        exit 2
        ;;
esac
if (($# > 1)); then
    usage >&2
    exit 2
fi

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
WINE_PREFIX=${NARUTO_WINE_PREFIX:-"$HOME/.wine-symbian"}
SDK_HOST_ROOT=${NARUTO_SYMBIAN_SDK_ROOT:-"$WINE_PREFIX/drive_c/S60/devices/S60_3rd_FP2_SDK_v1.1"}
SDK_WINDOWS_ROOT=${NARUTO_SYMBIAN_SDK_WINDOWS_ROOT:-'C:\S60\devices\S60_3rd_FP2_SDK_v1.1'}
PROJECT_WINDOWS_ROOT='C:\work\NarutoBattleSymbian'
PROJECT_LINK="$WINE_PREFIX/drive_c/work/NarutoBattleSymbian"
BUILD_LOCK_DIR="$PROJECT_DIR/sis/.build-wine.lock"
BUILD_LOG_DIR="$PROJECT_DIR/sis/build-logs"
BUILD_LOG="$BUILD_LOG_DIR/wine-${BUILD_PROFILE}.log"
VALIDATION_STAMP="$PROJECT_DIR/sis/.wine-validation.sha256"

find_wine_binary() {
    local candidate
    if [[ -n ${NARUTO_WINE_BIN:-} ]]; then
        printf '%s\n' "$NARUTO_WINE_BIN"
        return
    fi
    for candidate in \
        "$HOME/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine" \
        "/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine"; do
        if [[ -x $candidate ]]; then
            printf '%s\n' "$candidate"
            return
        fi
    done
    command -v wine 2>/dev/null || true
}

WINE_BIN=$(find_wine_binary)
if [[ -z $WINE_BIN || ! -x $WINE_BIN ]]; then
    printf '%s\n' 'Wine executable was not found. Set NARUTO_WINE_BIN.' >&2
    exit 1
fi

if [[ -n ${NARUTO_WINESERVER_BIN:-} ]]; then
    WINESERVER_BIN=$NARUTO_WINESERVER_BIN
else
    WINESERVER_BIN="$(dirname -- "$WINE_BIN")/wineserver"
fi

for required in \
    "$SDK_HOST_ROOT/epoc32/tools/bldmake.bat" \
    "$SDK_HOST_ROOT/epoc32/tools/makesis.exe" \
    "$WINE_PREFIX/drive_c/Perl/bin/perl.exe" \
    "$WINE_PREFIX/drive_c/Program Files/CSL Arm Toolchain/bin/arm-none-symbianelf-g++.exe"; do
    if [[ ! -e $required ]]; then
        printf 'Missing Symbian Wine dependency: %s\n' "$required" >&2
        exit 1
    fi
done

mkdir -p "$BUILD_LOG_DIR" "$(dirname -- "$PROJECT_LINK")"
if [[ -L $PROJECT_LINK ]]; then
    current_target=$(cd -- "$(dirname -- "$PROJECT_LINK")" && cd -- "$(readlink "$PROJECT_LINK")" && pwd)
    if [[ $current_target != "$PROJECT_DIR" ]]; then
        rm "$PROJECT_LINK"
        ln -s "$PROJECT_DIR" "$PROJECT_LINK"
    fi
elif [[ -e $PROJECT_LINK ]]; then
    printf 'Wine project path exists but is not a symlink: %s\n' "$PROJECT_LINK" >&2
    exit 1
else
    ln -s "$PROJECT_DIR" "$PROJECT_LINK"
fi

if ! mkdir "$BUILD_LOCK_DIR" 2>/dev/null; then
    lock_pid=''
    if [[ -f $BUILD_LOCK_DIR/pid ]]; then
        IFS= read -r lock_pid < "$BUILD_LOCK_DIR/pid" || lock_pid=''
    fi
    if [[ -n $lock_pid ]] && kill -0 "$lock_pid" 2>/dev/null; then
        printf 'Another Wine Symbian build is running with PID %s.\n' "$lock_pid" >&2
        exit 1
    fi
    rm -rf "$BUILD_LOCK_DIR"
    mkdir "$BUILD_LOCK_DIR"
fi
printf '%s\n' "$$" > "$BUILD_LOCK_DIR/pid"
release_build_lock() {
    rm -rf "$BUILD_LOCK_DIR"
}
trap release_build_lock EXIT HUP INT TERM

if [[ $BUILD_PROFILE != clean ]]; then
    python3 "$PROJECT_DIR/tools/generate_runtime_assets.py" --resume
    validation_fingerprint=$(python3 "$PROJECT_DIR/tools/build_validation_fingerprint.py")
    cached_validation_fingerprint=''
    if [[ -f $VALIDATION_STAMP ]]; then
        IFS= read -r cached_validation_fingerprint < "$VALIDATION_STAMP" || cached_validation_fingerprint=''
    fi
    if [[ $validation_fingerprint != "$cached_validation_fingerprint" ]]; then
        python3 "$PROJECT_DIR/tools/format_cpp.py" --changed
        python3 "$PROJECT_DIR/tools/format_cpp.py" --check --changed
        python3 "$PROJECT_DIR/tools/validate_symbian_sources.py"
        python3 "$PROJECT_DIR/tools/build_validation_fingerprint.py" > "$VALIDATION_STAMP"
    else
        printf '%s\n' 'Source formatting and validation are current'
    fi
fi

BUILD_OUTPUT_STAMP=''
build_fingerprint=''
if [[ $BUILD_PROFILE != clean ]]; then
    if [[ $BUILD_PROFILE == build ]]; then
        build_target=urel
        package_name=NarutoPixel_S60_3_X_v_1_0_3.sis
    else
        build_target=udeb
        package_name=NarutoPixelDebug_S60_3_X_v_1_0_3.sis
    fi
    interrupted_object_removed=0
    object_dir="$SDK_HOST_ROOT/epoc32/BUILD/work/NarutoBattleSymbian/group/NARUTOPIXEL/GCCE/$build_target"
    if [[ -d $object_dir ]]; then
        while IFS= read -r -d '' object_file; do
            printf 'Removing interrupted zero-byte object: %s\n' "$object_file"
            rm -f "$object_file"
            interrupted_object_removed=1
        done < <(find "$object_dir" -type f -size 0 -print0)
    fi
    BUILD_OUTPUT_STAMP="$PROJECT_DIR/sis/.wine-${BUILD_PROFILE}-output.sha256"
    build_fingerprint=$(python3 "$PROJECT_DIR/tools/build_output_fingerprint.py" \
        "$BUILD_PROFILE" --sdk-root "$SDK_HOST_ROOT" --wine-bin "$WINE_BIN")
    cached_build_fingerprint=''
    if [[ -f $BUILD_OUTPUT_STAMP ]]; then
        IFS= read -r cached_build_fingerprint < "$BUILD_OUTPUT_STAMP" || cached_build_fingerprint=''
    fi
    sdk_resource_dir="$SDK_HOST_ROOT/epoc32/data/z/resource/apps"
    expected_outputs=(
        "$PROJECT_DIR/sis/$package_name"
        "$SDK_HOST_ROOT/epoc32/release/gcce/$build_target/NarutoPixel_0xE3105FE5.exe"
        "$sdk_resource_dir/NarutoPixel_0xE3105FE5.rsc"
        "$SDK_HOST_ROOT/epoc32/data/z/private/10003a3f/apps/NarutoPixel_0xE3105FE5_reg.rsc"
        "$sdk_resource_dir/NarutoPixel_0xE3105FE5.mif"
        "$sdk_resource_dir/NarutoPixelBackground.mbm"
        "$sdk_resource_dir/NarutoPixelFont.mbm"
    )
    while IFS= read -r -d '' runtime_container; do
        expected_outputs+=("$sdk_resource_dir/$(basename "$runtime_container")")
    done < <(find "$PROJECT_DIR/assets/runtime" -maxdepth 1 -type f -name '*.dat' -print0 | sort -z)
    outputs_current=1
    if [[ $interrupted_object_removed == 1 ]]; then
        outputs_current=0
    fi
    for output in "${expected_outputs[@]}"; do
        if [[ ! -f $output ]]; then
            outputs_current=0
            break
        fi
    done
    if [[ $outputs_current == 1 && $build_fingerprint == "$cached_build_fingerprint" ]]; then
        printf '%s\n' 'Native GCCE executable, resources, and SIS are current'
        exit 0
    fi
fi

force_bldmake=0
if [[ ! -f $PROJECT_DIR/group/ABLD.BAT || $PROJECT_DIR/group/bld.inf -nt $PROJECT_DIR/group/ABLD.BAT ]]; then
    force_bldmake=1
fi

if [[ -n ${NARUTO_PIXEL_BUILD_JOBS:-} ]]; then
    build_jobs=$NARUTO_PIXEL_BUILD_JOBS
else
    build_jobs=$(sysctl -n hw.performancecpu 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || printf '4')
    ((build_jobs > 6)) && build_jobs=6
    ((build_jobs < 2)) && build_jobs=2
fi
if [[ ! $build_jobs =~ ^[1-9][0-9]*$ ]] || ((build_jobs > 16)); then
    printf 'Invalid NARUTO_PIXEL_BUILD_JOBS: %s\n' "$build_jobs" >&2
    exit 2
fi

export WINEPREFIX="$WINE_PREFIX"
export WINEDEBUG=-all
export MVK_CONFIG_LOG_LEVEL=0
export DXVK_LOG_LEVEL=none

if [[ -x $WINESERVER_BIN ]]; then
    "$WINESERVER_BIN" -p >/dev/null 2>&1 || true
fi

printf 'Wine prefix: %s\n' "$WINE_PREFIX"
printf 'SDK: %s\n' "$SDK_HOST_ROOT"
printf 'Project: %s\n' "$PROJECT_DIR"
printf 'Profile: %s; parallel jobs: %s\n' "$BUILD_PROFILE" "$build_jobs"
printf 'Log: %s\n' "$BUILD_LOG"

start_seconds=$SECONDS
set +e
NARUTO_PIXEL_ASSET_PIPELINE_VERIFIED=1 \
NARUTO_PIXEL_SDK_ROOT="$SDK_WINDOWS_ROOT" \
NARUTO_PIXEL_BUILD_JOBS="$build_jobs" \
NARUTO_PIXEL_FORCE_BLDMAKE="$force_bldmake" \
    "$WINE_BIN" cmd.exe /d /s /c \
    "${PROJECT_WINDOWS_ROOT}\\tools\\build_symbian.cmd" "$BUILD_PROFILE" >"$BUILD_LOG" 2>&1
build_status=$?
set -e
cat "$BUILD_LOG"

if grep -Eiq '(^|[[:space:]])make(\[[0-9]+\])?: \*\*\*|BLDMAKE ERROR:|ABLD ERROR:|Build stage failed:' "$BUILD_LOG"; then
    build_status=1
fi

if ((build_status != 0)); then
    printf 'Wine Symbian build failed. See %s\n' "$BUILD_LOG" >&2
    exit "$build_status"
fi

if [[ $BUILD_PROFILE == clean ]]; then
    NARUTO_REPOSITORY_CLEANUP=1 \
    NARUTO_REPOSITORY_CLEANUP_PARENT="$$" \
        python3 "$PROJECT_DIR/tools/clean_repository_outputs.py"
    rm -f "$VALIDATION_STAMP" "$PROJECT_DIR"/sis/.wine-*-output.sha256
else
    printf '%s\n' "$build_fingerprint" > "$BUILD_OUTPUT_STAMP"
fi

printf 'Completed in %ss.\n' "$((SECONDS - start_seconds))"
