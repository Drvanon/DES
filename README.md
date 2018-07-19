# DES - Dorstijn Entity System
This is a minimalistic entity component system created by Robin A. Dorstijn, based off the [T=Machine blog](http://t-machine.org). The purpose of this ECS is to provide a backend for games.

## Build
We use our homemade, ninja based, automated build system, [dabs](https://github.com/Drvanon/dabs), to build this directory:
```bash
dabs -l des
```

## TODOS
- Make all ECS tables (structs) dynamic memory
- Add SQL compatibility
- Add helper functions: assemblage_add_components
