#!/usr/bin/env bash
set -Eeuo pipefail

PROFILE=${1:-build}
case "$PROFILE" in
    build) SDK_CFG=UREL ;;
    debug) SDK_CFG=UDEB ;;
    *) echo "Usage: build_modern_core.sh [build|debug]" >&2; exit 2 ;;
esac

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
WINE_PREFIX=${EASYRPG_WINE_PREFIX:-"$HOME/.wine-symbian"}
SDK_ROOT=${EASYRPG_SYMBIAN_SDK_ROOT:-"$WINE_PREFIX/drive_c/S60/devices/S60_3rd_FP2_SDK_v1.1"}
CLANGXX=${EASYRPG_CORE_CLANGXX:-/usr/bin/clang++}
WINE_BIN=${EASYRPG_WINE_BIN:-}
CSL_BIN="$WINE_PREFIX/drive_c/Program Files/CSL Arm Toolchain/bin"
ASSEMBLER="$CSL_BIN/arm-none-symbianelf-as.exe"
ARCHIVER="$CSL_BIN/arm-none-symbianelf-ar.exe"
NM="$CSL_BIN/arm-none-symbianelf-nm.exe"
BUILD_DIR="$PROJECT_DIR/build/modern-core/$SDK_CFG"
LOCK_DIR="$PROJECT_DIR/build/.modern-core-$SDK_CFG.lock"
mkdir -p "$PROJECT_DIR/build"
if ! mkdir "$LOCK_DIR" 2>/dev/null; then
    existing_pid=$(cat "$LOCK_DIR/pid" 2>/dev/null || true)
    if [[ $existing_pid =~ ^[0-9]+$ ]] && kill -0 "$existing_pid" 2>/dev/null; then
        echo "Another modern-core build is already running for $SDK_CFG (pid $existing_pid)." >&2
        exit 75
    fi
    rm -rf "$LOCK_DIR"
    mkdir "$LOCK_DIR"
fi
printf '%s\n' "$$" > "$LOCK_DIR/pid"
cleanup_modern_core_lock() {
    rm -rf "$LOCK_DIR"
}
trap cleanup_modern_core_lock EXIT INT TERM
LIBRARY="$BUILD_DIR/EasyRpgModernCore.lib"
SDK_LIBRARY_DIR="$SDK_ROOT/epoc32/release/ARMV5/$SDK_CFG"
WIN_BUILD_DIR="C:\\work\\EasyRPG\\build\\modern-core\\$SDK_CFG"
GENERATED_DIR="$PROJECT_DIR/build/generated"
GENERATED_CP932="$GENERATED_DIR/CoreCp932Table.cpp"
GENERATED_READER_STRUCT="$GENERATED_DIR/reader_struct.cpp"
LIBLCF_ROOT="$PROJECT_DIR/third_party/liblcf-0.5.0/src"
LIBLCF_GENERATED_ROOT="$LIBLCF_ROOT/generated"
LIBCXX_COMPAT_INCLUDE="$PROJECT_DIR/inc/libcxx_compat"
PLAYER_ROOT="$PROJECT_DIR/third_party/easyrpg-player-0.5.0/src"
PLAYER_VENDOR_ROOT="$PROJECT_DIR/build/vendor/easyrpg-player-0.5.0/src"
PIXMAN_ROOT="$PROJECT_DIR/third_party/pixman-0.34.0/pixman"
for header in "$PLAYER_ROOT"/*.h; do
    name=$(basename "$header")
    if [[ -f "$PLAYER_VENDOR_ROOT/$name" ]]; then
        cp "$header" "$PLAYER_VENDOR_ROOT/$name"
    fi
done
SOURCES=(
    "$PROJECT_DIR/src/core/ModernCoreProbe.cpp"
    "$PROJECT_DIR/src/core/CoreZipCrypto.cpp"
    "$LIBLCF_ROOT/reader_lcf.cpp"
    "$LIBLCF_ROOT/data.cpp"
    "$LIBLCF_ROOT/rpg_setup.cpp"
    "$LIBLCF_ROOT/rpg_fixup.cpp"
    "$LIBLCF_ROOT/ldb_reader.cpp"
    "$LIBLCF_ROOT/ldb_equipment.cpp"
    "$LIBLCF_ROOT/ldb_eventcommand.cpp"
    "$LIBLCF_ROOT/ldb_parameters.cpp"
    "$LIBLCF_ROOT/lmt_reader.cpp"
    "$LIBLCF_ROOT/lmt_rect.cpp"
    "$LIBLCF_ROOT/lmt_treemap.cpp"
    "$LIBLCF_ROOT/lmu_reader.cpp"
    "$LIBLCF_ROOT/lmu_movecommand.cpp"
    "$LIBLCF_ROOT/lsd_reader.cpp"
    "$LIBLCF_ROOT/ini.cpp"
    "$LIBLCF_ROOT/inireader.cpp"
    "$LIBLCF_ROOT/reader_flags.cpp"
    "$LIBLCF_ROOT/writer_lcf.cpp"
    "$LIBLCF_ROOT/writer_xml.cpp"
    "$GENERATED_READER_STRUCT"
    "$PROJECT_DIR/src/core/LibcxxSharedPtrRuntime.cpp"
    "$PROJECT_DIR/src/core/LiblcfReaderUtil.cpp"
    "$PROJECT_DIR/src/core/LiblcfXmlReader.cpp"
    "$PROJECT_DIR/src/core/UpstreamLmuBridge.cpp"
    "$PROJECT_DIR/src/core/UpstreamLcfLinkAnchor.cpp"
    "$PROJECT_DIR/src/core/Ui.cpp"
    "$PLAYER_ROOT/scene.cpp"
    "$PROJECT_DIR/src/core/RenderDamage.cpp"
    "$PROJECT_DIR/src/core/AsyncHandler.cpp"
    "$PROJECT_DIR/src/core/Audio.cpp"
    "$PROJECT_DIR/src/core/Output.cpp"
    "$PROJECT_DIR/src/core/Aeabi.cpp"
    "$PROJECT_DIR/src/core/Image.cpp"
    "$PROJECT_DIR/src/core/Text.cpp"
    "$PROJECT_DIR/src/core/Font.cpp"
    "$PROJECT_DIR/src/core/GohuFontData.cpp"
    "$PIXMAN_ROOT/pixman-implementation.c"
    "$PIXMAN_ROOT/pixman-general.c"
    "$PIXMAN_ROOT/pixman-utils.c"
    "$PIXMAN_ROOT/pixman-noop.c"
    "$PIXMAN_ROOT/pixman-solid-fill.c"
    "$PIXMAN_ROOT/pixman-arm.c"
    "$PIXMAN_ROOT/pixman-x86.c"
    "$PIXMAN_ROOT/pixman-ppc.c"
    "$PIXMAN_ROOT/pixman-mips.c"
    "$PIXMAN_ROOT/pixman-matrix.c"
    "$PIXMAN_ROOT/pixman-image.c"
    "$PIXMAN_ROOT/pixman-bits-image.c"
    "$PIXMAN_ROOT/pixman-combine32.c"
    "$PROJECT_DIR/build/vendor/pixman-0.34.0/pixman/pixman-fast-path.c"
    "$PROJECT_DIR/build/vendor/pixman-0.34.0/pixman/pixman.c"
    "$PROJECT_DIR/src/core/PixmanCompat.c"
    "$PLAYER_ROOT/color.cpp"
    "$PLAYER_ROOT/rect.cpp"
    "$PLAYER_ROOT/tone.cpp"
    "$PLAYER_ROOT/bitmap.cpp"
    "$PLAYER_ROOT/effects.cpp"
    "$PLAYER_ROOT/tilemap_layer.cpp"
    "$PLAYER_ROOT/tilemap.cpp"
    "$PLAYER_ROOT/sprite.cpp"
    "$PLAYER_ROOT/graphics.cpp"
    "$PLAYER_ROOT/game_character.cpp"
    "$PLAYER_ROOT/game_event.cpp"
    "$PLAYER_ROOT/game_player.cpp"
    "$PLAYER_ROOT/game_commonevent.cpp"
    "$PLAYER_ROOT/game_vehicle.cpp"
    "$PLAYER_ROOT/game_interpreter.cpp"
    "$PLAYER_ROOT/game_interpreter_map.cpp"
    "$PLAYER_ROOT/game_map.cpp"
    "$PLAYER_ROOT/main_data.cpp"
    "$PLAYER_ROOT/game_switches.cpp"
    "$PLAYER_ROOT/game_variables.cpp"
    "$PLAYER_ROOT/game_temp.cpp"
    "$PLAYER_ROOT/game_targets.cpp"
    "$PLAYER_ROOT/game_message.cpp"
    "$PLAYER_ROOT/audio.cpp"
    "$PLAYER_ROOT/game_actors.cpp"
    "$PLAYER_ROOT/game_party_base.cpp"
    "$PLAYER_ROOT/game_system.cpp"
    "$PLAYER_ROOT/game_screen.cpp"
    "$PLAYER_ROOT/game_picture.cpp"
    "$PLAYER_ROOT/game_enemyparty.cpp"
    "$PLAYER_ROOT/player.cpp"
    "$PLAYER_ROOT/input.cpp"
    "$PLAYER_ROOT/input_buttons_desktop.cpp"
    "$PLAYER_ROOT/filefinder.cpp"
    "$PLAYER_VENDOR_ROOT/cache.cpp"
    "$PLAYER_VENDOR_ROOT/game_actor.cpp"
    "$PLAYER_VENDOR_ROOT/game_battler.cpp"
    "$PLAYER_VENDOR_ROOT/game_enemy.cpp"
    "$PLAYER_VENDOR_ROOT/game_party.cpp"
    "$PLAYER_VENDOR_ROOT/game_battle.cpp"
    "$PLAYER_VENDOR_ROOT/game_interpreter_battle.cpp"
    "$PLAYER_VENDOR_ROOT/battle_animation.cpp"
    "$PLAYER_VENDOR_ROOT/scene_logo.cpp"
    "$PLAYER_VENDOR_ROOT/scene_title.cpp"
    "$PLAYER_VENDOR_ROOT/scene_map.cpp"
    "$PLAYER_VENDOR_ROOT/scene_gamebrowser.cpp"
    "$PLAYER_VENDOR_ROOT/scene_actortarget.cpp"
    "$PLAYER_VENDOR_ROOT/scene_battle.cpp"
    "$PLAYER_VENDOR_ROOT/scene_battle_rpg2k.cpp"
    "$PLAYER_VENDOR_ROOT/scene_battle_rpg2k3.cpp"
    "$PLAYER_VENDOR_ROOT/scene_debug.cpp"
    "$PLAYER_VENDOR_ROOT/scene_end.cpp"
    "$PLAYER_VENDOR_ROOT/scene_equip.cpp"
    "$PLAYER_VENDOR_ROOT/scene_file.cpp"
    "$PLAYER_VENDOR_ROOT/scene_gameover.cpp"
    "$PLAYER_VENDOR_ROOT/scene_item.cpp"
    "$PLAYER_VENDOR_ROOT/scene_load.cpp"
    "$PLAYER_VENDOR_ROOT/scene_menu.cpp"
    "$PLAYER_VENDOR_ROOT/scene_name.cpp"
    "$PLAYER_VENDOR_ROOT/scene_order.cpp"
    "$PLAYER_VENDOR_ROOT/scene_save.cpp"
    "$PLAYER_VENDOR_ROOT/scene_shop.cpp"
    "$PLAYER_VENDOR_ROOT/scene_skill.cpp"
    "$PLAYER_VENDOR_ROOT/scene_status.cpp"
    "$PLAYER_VENDOR_ROOT/scene_teleport.cpp"
    "$PLAYER_VENDOR_ROOT/background.cpp"
    "$PLAYER_VENDOR_ROOT/game_battlealgorithm.cpp"
    "$PLAYER_VENDOR_ROOT/audio_secache.cpp"
    "$PLAYER_VENDOR_ROOT/window_about.cpp"
    "$PLAYER_VENDOR_ROOT/window_actorinfo.cpp"
    "$PLAYER_VENDOR_ROOT/window_actorstatus.cpp"
    "$PLAYER_VENDOR_ROOT/window_actortarget.cpp"
    "$PLAYER_VENDOR_ROOT/window_base.cpp"
    "$PLAYER_VENDOR_ROOT/window_battlemessage.cpp"
    "$PLAYER_VENDOR_ROOT/window_battlestatus.cpp"
    "$PLAYER_VENDOR_ROOT/window_command.cpp"
    "$PLAYER_VENDOR_ROOT/window_equip.cpp"
    "$PLAYER_VENDOR_ROOT/window_equipitem.cpp"
    "$PLAYER_VENDOR_ROOT/window_equipstatus.cpp"
    "$PLAYER_VENDOR_ROOT/window_face.cpp"
    "$PLAYER_VENDOR_ROOT/window_gamelist.cpp"
    "$PLAYER_VENDOR_ROOT/window_gold.cpp"
    "$PLAYER_VENDOR_ROOT/window_help.cpp"
    "$PLAYER_VENDOR_ROOT/window_item.cpp"
    "$PLAYER_VENDOR_ROOT/window_keyboard.cpp"
    "$PLAYER_VENDOR_ROOT/window_menustatus.cpp"
    "$PLAYER_VENDOR_ROOT/window_message.cpp"
    "$PLAYER_VENDOR_ROOT/window_name.cpp"
    "$PLAYER_VENDOR_ROOT/window_numberinput.cpp"
    "$PLAYER_VENDOR_ROOT/window_savefile.cpp"
    "$PLAYER_VENDOR_ROOT/window_selectable.cpp"
    "$PLAYER_VENDOR_ROOT/window_shop.cpp"
    "$PLAYER_VENDOR_ROOT/window_shopbuy.cpp"
    "$PLAYER_VENDOR_ROOT/window_shopnumber.cpp"
    "$PLAYER_VENDOR_ROOT/window_shopparty.cpp"
    "$PLAYER_VENDOR_ROOT/window_shopsell.cpp"
    "$PLAYER_VENDOR_ROOT/window_shopstatus.cpp"
    "$PLAYER_VENDOR_ROOT/window_skill.cpp"
    "$PLAYER_VENDOR_ROOT/window_skillstatus.cpp"
    "$PLAYER_VENDOR_ROOT/window_targetstatus.cpp"
    "$PLAYER_VENDOR_ROOT/window_teleport.cpp"
    "$PLAYER_VENDOR_ROOT/window_varlist.cpp"
    "$PLAYER_VENDOR_ROOT/spriteset_map.cpp"
    "$PLAYER_VENDOR_ROOT/spriteset_battle.cpp"
    "$PLAYER_VENDOR_ROOT/sprite_battler.cpp"
    "$PLAYER_VENDOR_ROOT/sprite_character.cpp"
    "$PLAYER_VENDOR_ROOT/sprite_airshipshadow.cpp"
    "$PLAYER_VENDOR_ROOT/sprite_timer.cpp"
    "$PLAYER_VENDOR_ROOT/plane.cpp"
    "$PLAYER_VENDOR_ROOT/screen.cpp"
    "$PLAYER_VENDOR_ROOT/weather.cpp"
    "$PLAYER_VENDOR_ROOT/frame.cpp"
    "$PLAYER_VENDOR_ROOT/rtp_table.cpp"
    "$GENERATED_CP932"
)

while IFS= read -r generated_source; do
    SOURCES+=("$generated_source")
done < <(find "$LIBLCF_GENERATED_ROOT" -maxdepth 1 -type f -name '*.cpp' | sort)

while IFS= read -r upstream_source; do
    upstream_name=$(basename -- "$upstream_source")
    case "$upstream_name" in
        async_handler.cpp|audio_al.cpp|audio_resampler.cpp|audio_sdl.cpp|baseui.cpp|decoder_*.cpp|font.cpp|hslrgb.cpp|image_bmp.cpp|image_png.cpp|image_xyz.cpp|input_buttons_gekko.cpp|input_buttons_opendingux.cpp|input_buttons_psp.cpp|main.cpp|message_overlay.cpp|midisequencer.cpp|midisynth.cpp|output.cpp|registry.cpp|registry_wine.cpp|sdl_ui.cpp|text.cpp|util_win.cpp|window_battlecommand.cpp|window_battleoption.cpp)
            continue
            ;;
    esac
    already_added=false
    for existing_source in "${SOURCES[@]}"; do
        if [[ $(basename -- "$existing_source") == "$upstream_name" ]]; then
            already_added=true
            break
        fi
    done
    if [[ $already_added == false ]]; then
        SOURCES+=("$upstream_source")
    fi
done < <(find "$PLAYER_VENDOR_ROOT" -maxdepth 1 -type f -name '*.cpp' | sort)

find_wine() {
    if [[ -n $WINE_BIN && -x $WINE_BIN ]]; then
        printf '%s\n' "$WINE_BIN"
        return
    fi
    local candidate
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

WINE_BIN=$(find_wine)
if [[ -z $WINE_BIN || ! -x $WINE_BIN ]]; then
    echo "Wine executable not found for CSL binutils." >&2
    exit 1
fi
if [[ ! -x $CLANGXX ]]; then
    echo "Clang compiler not found: $CLANGXX" >&2
    exit 1
fi
for tool in "$ASSEMBLER" "$ARCHIVER" "$NM"; do
    if [[ ! -e $tool ]]; then
        echo "Missing CSL ARM binutil: $tool" >&2
        exit 1
    fi
done
if [[ ! -d $SDK_ROOT/epoc32 ]]; then
    echo "Invalid S60 SDK: $SDK_ROOT" >&2
    exit 1
fi

mkdir -p "$BUILD_DIR" "$SDK_LIBRARY_DIR" "$GENERATED_DIR"
python3 "$PROJECT_DIR/tools/generate_cp932_table.py" --output "$GENERATED_CP932"
python3 "$PROJECT_DIR/tools/generate_reader_struct.py" "$GENERATED_READER_STRUCT"
rm -f "$LIBRARY"

objects=()
win_objects=()
object_index=0
for source in "${SOURCES[@]}"; do
    source_name=$(basename -- "$source")
    base=${source_name%.cpp}
    base=$(printf '%03d_%s' "$object_index" "$base")
    object_index=$((object_index + 1))
    assembly_raw="$BUILD_DIR/$base.clang.s"
    assembly="$BUILD_DIR/$base.gcce.s"
    object="$BUILD_DIR/$base.o"
    win_assembly="$WIN_BUILD_DIR\\$base.gcce.s"
    win_object="$WIN_BUILD_DIR\\$base.o"

    if [[ ! -f $source ]]; then
        echo "Modern core source missing: $source" >&2
        exit 1
    fi

    # GCCE 3.4.3 cannot parse EasyRPG 0.5-era C++11. Modern Clang is used
    # only as frontend/optimizer/code generator. The final object is produced
    # by the original S60 CSL assembler so its ELF/EABI format is linkable by
    # the SDK's arm-none-symbianelf-ld and elf2e32.
    source_define=
    if [[ $source == "$PLAYER_ROOT/filefinder.cpp" || $source == "$PLAYER_VENDOR_ROOT/filefinder.cpp" ]]; then
        source_define='-DUSE_SDL -DNO_SDL_MIXER -DEASYRPG_SYMBIAN -Dfopen=EasyRpgFopen'
    elif [[ $source == "$PLAYER_ROOT/"* || $source == "$PLAYER_VENDOR_ROOT/"* ]]; then
        source_define='-DUSE_SDL -DNO_SDL_MIXER -DEASYRPG_SYMBIAN -Dfopen=EasyRpgFopen'
    elif [[ $source == "$PROJECT_DIR/src/core/Ui.cpp" ||
            $source == "$PROJECT_DIR/src/core/RenderDamage.cpp" ||
            $source == "$PROJECT_DIR/src/core/AsyncHandler.cpp" ||
            $source == "$PROJECT_DIR/src/core/Audio.cpp" ||
            $source == "$PROJECT_DIR/src/core/Output.cpp" ||
            $source == "$PROJECT_DIR/src/core/Aeabi.cpp" ||
            $source == "$PROJECT_DIR/src/core/Image.cpp" ||
            $source == "$PROJECT_DIR/src/core/Text.cpp" ||
            $source == "$PROJECT_DIR/src/core/Font.cpp" ]]; then
        source_define='-DUSE_SDL -DNO_SDL_MIXER -DEASYRPG_SYMBIAN -Dfopen=EasyRpgFopen'
    elif [[ $source == "$LIBLCF_ROOT/"* || $source == "$GENERATED_READER_STRUCT" ]]; then
        source_define='-Dfopen=EasyRpgFopen'
    fi
    language_flag=-xc++
    standard_flag=-std=c++11
    preinclude_flag="-include $PROJECT_DIR/inc/UpstreamCompat.h"
    pixman_define=
    if [[ $source == *.c ]]; then
        language_flag=-xc
        standard_flag=-std=c99
        preinclude_flag=
    fi
    if [[ $source == "$PIXMAN_ROOT/"* || $source == "$PROJECT_DIR/build/vendor/pixman-0.34.0/pixman/"* || $source == "$PROJECT_DIR/src/core/PixmanCompat.c" ]]; then
        pixman_define='-DPACKAGE=1 -DPIXMAN_NO_TLS'
    fi

    "$CLANGXX" \
        --target=arm-none-eabi \
        -march=armv5t \
        -mfloat-abi=soft \
        -fshort-wchar \
        $language_flag \
        $standard_flag \
        -O2 \
        -ffreestanding \
        -fno-exceptions \
        -fno-rtti \
        -fno-threadsafe-statics \
        -fno-c++-static-destructors \
        -fno-stack-protector \
        -fno-unwind-tables \
        -fno-asynchronous-unwind-tables \
        -fno-addrsig \
        -fvisibility=hidden \
        -fdata-sections \
        -ffunction-sections \
        -D_LIBCPP_HAS_NO_THREADS \
        $source_define \
        $pixman_define \
        $preinclude_flag \
        -nostdinc++ \
        -I "$LIBCXX_COMPAT_INCLUDE" \
        -I "$PROJECT_DIR/third_party/libcxx-4.0.1/include" \
        -I "$PROJECT_DIR/inc/libc_compat" \
        -I "$PROJECT_DIR/inc" \
        -I "$PLAYER_ROOT" \
        -I "$PLAYER_VENDOR_ROOT" \
        -I "$PIXMAN_ROOT" \
        -I "$LIBLCF_ROOT" \
        -I "$LIBLCF_GENERATED_ROOT" \
        -S "$source" \
        -o "$assembly_raw"

    # CSL's old assembler predates these metadata directives and some modern
    # GNU assembler spellings. ARM's default stm/ldm alias is increment-after.
    # Clang also emits three-operand register shifts (e.g. lsl r2,r9,r1), while
    # CSL requires the equivalent data-processing spelling (mov r2,r9,lsl r1).
    # Normalize syntax only; instruction semantics remain unchanged.
    sed -E \
        -e '/^[[:space:]]*\.(eabi_attribute|cpu)([[:space:]]|$)/d' \
        -e 's/^([[:space:]]*)stm([[:space:]])/\1stmia\2/' \
        -e 's/^([[:space:]]*)ldm([[:space:]])/\1ldmia\2/' \
        -e 's/^([[:space:]]*)lsl([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, lsl \6/' \
        -e 's/^([[:space:]]*)lsr([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, lsr \6/' \
        -e 's/^([[:space:]]*)asr([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, asr \6/' \
        -e 's/^([[:space:]]*)ror([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, ror \6/' \
        -e 's/^([[:space:]]*)rrx([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,[:space:]]+)[[:space:]]*$/\1mov\2\3\4, \5, rrx/' \
        -e 's/^([[:space:]]*)\.inst([[:space:]]+)/\1.word\2/' \
        -e 's/"awGR"/"awG"/g' \
        "$assembly_raw" > "$assembly"

    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
        "$WINE_BIN" "$ASSEMBLER" -march=armv5t -o "$win_object" "$win_assembly"

    objects+=("$object")
    win_objects+=("$win_object")
done

WIN_LIBRARY="$WIN_BUILD_DIR\\EasyRpgModernCore.lib"
rm -f "$LIBRARY"
chunk_size=32
for ((chunk_start=0; chunk_start<${#win_objects[@]}; chunk_start+=chunk_size)); do
    chunk=("${win_objects[@]:chunk_start:chunk_size}")
    if ((chunk_start == 0)); then
        archive_flags=crs
    else
        archive_flags=rs
    fi
    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
        "$WINE_BIN" "$ARCHIVER" "$archive_flags" "$WIN_LIBRARY" "${chunk[@]}"
done

symbol_output=$(WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
    "$WINE_BIN" "$NM" -g "$WIN_LIBRARY")
for required_symbol in EasyRpgModernCoreProbe EasyRpgCoreCp932ToUtf16 EasyRpgCoreUtf16ToCp932 EasyRpgCoreWinZipAesDecrypt EasyRpgUpstreamLoadLmtSummary EasyRpgUpstreamLoadLdbSummary EasyRpgUpstreamReadLmuSummary EasyRpgUpstreamReadLmuLayers EasyRpgUpstreamLmuLinkAnchor _ZN10ReaderUtil6Recode _ZN9LcfReaderC1 _ZN10LMU_Reader4Load; do
    if ! grep -q "$required_symbol" <<<"$symbol_output"; then
        echo "Modern core archive does not export $required_symbol." >&2
        printf '%s\n' "$symbol_output" >&2
        exit 1
    fi
done

SDK_LIBRARY="$SDK_LIBRARY_DIR/EasyRpgModernCore.lib"
SDK_LIBRARY_TMP="$SDK_LIBRARY_DIR/.EasyRpgModernCore.lib.$$"
cp -f "$LIBRARY" "$SDK_LIBRARY_TMP"
mv -f "$SDK_LIBRARY_TMP" "$SDK_LIBRARY"

printf 'Modern C++11 -> CSL EABI4 core: %s (%s bytes, %s objects)\n' \
    "$LIBRARY" "$(stat -f %z "$LIBRARY")" "${#objects[@]}"
printf 'Verified symbols: upstream LDB/LMT/LMU read path + CP932/ZIP platform adapters\n'
printf 'Installed for ABLD: %s\n' "$SDK_LIBRARY"
