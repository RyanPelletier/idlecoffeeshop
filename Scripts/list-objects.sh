#!/usr/bin/env bash
# Print catalog object ids (mirrors UObjectCatalog + Content/Objects).
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo "IdleCoffeeShop objects"
echo "======================"
printf "%-20s  %-12s  %s\n" "ID" "CATEGORY" "CONTENT PATH"
printf "%-20s  %-12s  %s\n" "--------------------" "------------" "----------------"

list_cat() {
  local cat="$1"
  local dir="${ROOT}/Content/Objects/${cat}"
  [[ -d "${dir}" ]] || return 0
  for d in "${dir}"/*/; do
    [[ -d "${d}" ]] || continue
    local id
    id="$(basename "${d}")"
    printf "%-20s  %-12s  Content/Objects/%s/%s\n" "${id}" "${cat}" "${cat}" "${id}"
  done
}

list_cat Kitchen
list_cat Counter
list_cat Equipment
list_cat Ingredients

echo ""
echo "Preview:  make showobj <id>"
echo "Example:  make showobj milk_jug"
