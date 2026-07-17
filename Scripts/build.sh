#!/usr/bin/env bash
# Thin wrapper — prefer: make / make build
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${ROOT}"
if [[ $# -eq 0 ]]; then
  exec make build
fi
# Pass-through: ./Scripts/build.sh [Target] [Platform] [Config]
TARGET="${1:-IdleCoffeeShopEditor}"
PLATFORM="${2:-Mac}"
CONFIG="${3:-Development}"
if [[ "${TARGET}" == "IdleCoffeeShop" ]]; then
  exec make game PLATFORM="${PLATFORM}" CONFIG="${CONFIG}"
fi
exec make build PLATFORM="${PLATFORM}" CONFIG="${CONFIG}"
