#!/usr/bin/env bash
# Launch object showroom for one catalog prop.
# Usage: ./Scripts/showobj.sh coffee_mug
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
UPROJECT="${ROOT}/IdleCoffeeShop.uproject"
UE_ROOT="${UE_ROOT:-/Users/Shared/Epic Games/UE_5.5}"
EDITOR_BIN="${UE_ROOT}/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor"
RES_X="${RES_X:-1440}"
RES_Y="${RES_Y:-900}"

OBJ_RAW="${1:-}"
if [[ -z "${OBJ_RAW}" ]]; then
  echo "Usage: make showobj <object_id>"
  echo "Example: make showobj coffee_mug"
  echo ""
  echo "Known objects:"
  "${ROOT}/Scripts/list-objects.sh" 2>/dev/null || true
  exit 1
fi

# Normalize: CoffeeMug / coffee-mug / coffee mug → coffee_mug
# Note: do not use `tr '- /' ...` on macOS — BSD tr treats leading '-' as a flag.
OBJ="$(
  printf '%s' "${OBJ_RAW}" \
    | sed -E 's/([a-z0-9])([A-Z])/\1_\2/g' \
    | tr 'A-Z' 'a-z' \
    | sed -E 's|[ /.-]+|_|g; s|_+|_|g; s|^_||; s|_$||'
)"

MAP_URL="/Engine/Maps/Templates/OpenWorld?game=/Script/IdleCoffeeShop.ObjectShowGameMode?Object=${OBJ}"

if [[ ! -x "${EDITOR_BIN}" ]]; then
  echo "ERROR: UnrealEditor not found at ${EDITOR_BIN}"
  exit 1
fi

echo "==> Object showroom: ${OBJ}"
echo "    Controls: LMB grab/release | E interact | R reset | F info | WASD+mouse"
echo "    Map: ${MAP_URL}"

exec "${EDITOR_BIN}" "${UPROJECT}" "${MAP_URL}" \
  -game -windowed -ResX="${RES_X}" -ResY="${RES_Y}" \
  -nosplash -log -stdout -FullStdOutLogOutput
