# IdleCoffeeShop

An **idle coffee shop** game built with **C++ and Unreal Engine 5.5**, with physics-friendly stations and cosmetics themed after **Live Oak Coffee House** on Abbot Road in Midland, MI.

## Requirements

- Unreal Engine **5.5** (tested with 5.5.4)
- Xcode + command-line tools (macOS)
- Apple Silicon or Intel Mac

Default engine path:

```text
/Users/Shared/Epic Games/UE_5.5
```

Override with `UE_ROOT` if your install lives elsewhere.

### Xcode 26+ note

UE 5.5 officially supports Xcode **15.2–16.9**. On Xcode 26 this repo:

1. Relaxes the engine `Apple_SDK.json` max version (backup: `Apple_SDK.json.bak-ue55` next to the original under the engine install).
2. Adds `-Wno-deprecated-literal-operator` in the `.Target.cs` files so engine headers compile under the newer clang.

If you restore the stock SDK JSON, builds will fail with “Platform Mac is not a valid platform” until Xcode is within the supported range.

## Project layout

```text
IdleCoffeeShop.uproject
Config/                 # Engine / game / input defaults
Content/                # Place maps, meshes, UI assets here
Source/IdleCoffeeShop/  # C++ game module
  CoffeeTypes.h                 # Menu items, drink enums, stats
  CoffeeShopGameMode.*          # Spawns manager + starter station
  CoffeeShopManager.*           # Cash, reputation, upgrades, idle income
  CoffeeStation.*               # Physics mesh + auto drink prep
  CoffeeShopPlayerController.*  # U = upgrade, I = status log
Makefile                # make build / open / clean
Scripts/build.sh        # Optional thin shell wrapper
legacy-web/             # Earlier HTML prototype (reference only)
```

## Core gameplay (sample)

| System | Behavior |
|--------|----------|
| **Stations** | Cube props with physics; auto-prepare drinks on a timer |
| **Sales** | Completed orders add cash + reputation |
| **Idle income** | Passive earnings scale with stations, level, and reputation |
| **Upgrades** | Press **U** to level stations (cost scales up) |
| **Status** | Press **I** to log cash / rep / idle rate to the Output Log |

Menu staples (House Drip, Oak Latte, Nitro Cold Brew, etc.) are defined in `CoffeeShopManager::BuildDefaultMenu()`.

## Build & run (`make`)

```bash
make              # build IdleCoffeeShopEditor (Development)
make play         # build, then launch the game window
make run          # launch game only (no rebuild)
make game         # build game target
make shipping     # editor Shipping
make open         # launch Unreal Editor with this project
make clean        # remove Binaries/ Intermediate/
make clean-all    # also Saved/ DerivedDataCache/
make help
```

In-game: **WASD** move, **mouse** look, **U** upgrade stations, **I** status log, **Esc** quit.

Overrides:

```bash
make build CONFIG=DebugGame
make UE_ROOT="/path/to/UE_5.5" build
```

## Next steps

- Author a shop floor map under `Content/Maps/`
- Replace the cube station mesh with Live Oak–style furniture
- Build a UMG HUD bound to `OnStatsChanged`
- Add cosmetics unlocks funded by idle cash
- Save/load `FCoffeeShopStats` via a save-game object

## License

Sample project code — free to use and modify for this game.
