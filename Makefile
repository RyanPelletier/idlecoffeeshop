# IdleCoffeeShop — Unreal Engine 5.5 (macOS)
#
# Usage:
#   make            # build editor module (Development)
#   make build      # same
#   make game       # build game target
#   make shipping   # build editor Shipping
#   make open       # open project in Unreal Editor
#   make clean      # remove Binaries/ Intermediate/
#   make clean-all  # also Saved/ DerivedDataCache/
#   make help

PROJECT_ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
UPROJECT     := $(PROJECT_ROOT)/IdleCoffeeShop.uproject

UE_ROOT  ?= /Users/Shared/Epic Games/UE_5.5
PLATFORM ?= Mac
CONFIG   ?= Development

BUILD_SH     := $(UE_ROOT)/Engine/Build/BatchFiles/Mac/Build.sh
EDITOR_APP   := $(UE_ROOT)/Engine/Binaries/Mac/UnrealEditor.app
EDITOR_BIN   := $(EDITOR_APP)/Contents/MacOS/UnrealEditor
RES_X        ?= 1440
RES_Y        ?= 900

EDITOR_TARGET := IdleCoffeeShopEditor
GAME_TARGET   := IdleCoffeeShop

.PHONY: all build game shipping open play run clean clean-all help check-engine

all: build

check-engine:
	@test -f "$(BUILD_SH)" || { \
		echo "ERROR: Unreal build script not found: $(BUILD_SH)"; \
		echo "Set UE_ROOT (current: $(UE_ROOT))"; \
		exit 1; \
	}
	@test -f "$(UPROJECT)" || { \
		echo "ERROR: Missing uproject: $(UPROJECT)"; \
		exit 1; \
	}

## Build editor target (default)
build: check-engine
	@echo "==> Building $(EDITOR_TARGET) ($(PLATFORM) $(CONFIG))"
	@echo "    Project: $(UPROJECT)"
	@echo "    Engine:  $(UE_ROOT)"
	"$(BUILD_SH)" "$(EDITOR_TARGET)" "$(PLATFORM)" "$(CONFIG)" \
		-Project="$(UPROJECT)" -WaitMutex
	@echo "==> Build finished successfully."

## Build standalone game target
game: check-engine
	@echo "==> Building $(GAME_TARGET) ($(PLATFORM) $(CONFIG))"
	"$(BUILD_SH)" "$(GAME_TARGET)" "$(PLATFORM)" "$(CONFIG)" \
		-Project="$(UPROJECT)" -WaitMutex
	@echo "==> Game build finished successfully."

## Build editor in Shipping
shipping:
	$(MAKE) build CONFIG=Shipping

## Open project in Unreal Editor
open: check-engine
	@test -d "$(EDITOR_APP)" || { \
		echo "ERROR: UnrealEditor not found: $(EDITOR_APP)"; \
		exit 1; \
	}
	@echo "==> Opening $(UPROJECT)"
	open "$(EDITOR_APP)" --args "$(UPROJECT)"

## Build editor target, then launch standalone game (-game)
play: build run

## Launch standalone game (requires prior `make build`)
run: check-engine
	@test -x "$(EDITOR_BIN)" || { \
		echo "ERROR: UnrealEditor binary not found: $(EDITOR_BIN)"; \
		exit 1; \
	}
	@echo "==> Launching IdleCoffeeShop ($(RES_X)x$(RES_Y))"
	@echo "    WASD move | mouse look | U upgrade | I status | Esc quit"
	"$(EDITOR_BIN)" "$(UPROJECT)" -game -windowed -ResX=$(RES_X) -ResY=$(RES_Y) \
		-nosplash -log -stdout -FullStdOutLogOutput

## Remove compile outputs
clean:
	@echo "==> Cleaning Binaries/ Intermediate/"
	rm -rf "$(PROJECT_ROOT)/Binaries" "$(PROJECT_ROOT)/Intermediate"
	@echo "==> Clean done."

## Clean build + local engine caches for this project
clean-all: clean
	@echo "==> Cleaning Saved/ DerivedDataCache/"
	rm -rf "$(PROJECT_ROOT)/Saved" "$(PROJECT_ROOT)/DerivedDataCache"
	@echo "==> Clean-all done."

help:
	@echo "IdleCoffeeShop make targets"
	@echo ""
	@echo "  make / make build   Build IdleCoffeeShopEditor (Development)"
	@echo "  make game           Build IdleCoffeeShop game target"
	@echo "  make shipping       Build editor Shipping"
	@echo "  make open           Launch Unreal Editor with this project"
	@echo "  make play           Build, then launch the game window"
	@echo "  make run            Launch game only (no rebuild)"
	@echo "  make clean          Remove Binaries/ Intermediate/"
	@echo "  make clean-all      Also remove Saved/ DerivedDataCache/"
	@echo "  make help           Show this help"
	@echo ""
	@echo "Overrides:"
	@echo "  UE_ROOT=$(UE_ROOT)"
	@echo "  CONFIG=$(CONFIG)   PLATFORM=$(PLATFORM)"
	@echo "  e.g. make build CONFIG=DebugGame"
