# Content / Objects

Asset home for coffee-shop props. Each object has a folder matching its
**catalog id** (snake_case). Drop meshes, materials, and icons here when
you replace the procedural stand-ins in C++.

```text
Content/Objects/
  Kitchen/           # cups, mugs, jugs, brew tools
    coffee_mug/
    milk_jug/
    espresso_cup/
    latte_glass/
    french_press/
    pour_over/
    sugar_jar/
  Counter/           # front-of-house props
    tip_jar/
    menu_stand/
    pastry_plate/
  Equipment/         # machines / larger gear
    grinder/
    espresso_machine/
  Ingredients/       # bulk goods
    coffee_bag/
```

C++ classes live under `Source/IdleCoffeeShop/Objects/` with the same
category grouping. Catalog ids are defined in `ObjectCatalog`.

Preview any object:

```bash
make showobj coffee_mug
make list-objects
```
