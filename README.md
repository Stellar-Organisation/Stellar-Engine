# Stellar-Engine
Stellar Engine is a 2D game engine written in C++ and OpenGL. It is designed to be simple and easy to usTe, while still being powerful and flexible. It is currently in development and is not yet ready for usTe in production.

# Github guide
## Branch naming
- `{ticket-number}/feature/{feature-name}`: For new features
    - Example: `STE-1/feature/physics-engine`
- `{ticket-number}/fix/{feature-name}/{fix-name}`: For bug fixes
    - Example: `STE-2/fix/physics-engine/fix-collision-bug`
- `{ticket-number}/refactor/{feature-name}/{refactor-name}`: For refactoring
    - Example: `STE-3/refactor/physics-engine/refactor-collision-code`
- `{ticket-number}/test/{feature-name}/{test-name}`: For writing tests
    - Example: `STE-4/test/physics-engine/write-collision-tests`
- `{ticket-number}/docs/{feature-name}/{docs-name}`: For documentation
    - Example: `STE-5/docs/physics-engine/write-collision-docs`

## Commit messages
- `{ticket-number}: {message} {optional-description} {MAJOR|MINOR|PATCH}`
    - Example: `STE-1: Add physics engine MAJOR`
    - Example: `STE-2: Fix collision bug PATCH`
    - Example: `STE-3: Refactor collision code MINOR`
    - Example: `STE-4: Write collision tests MINOR`
    - Example: `STE-5: Write collision docs MINOR`

## Pull requests
- `{ticket-number}: {short-description}`
    - Example: `STE-1: Add physics engine`
    - Example: `STE-2: Fix collision bug`
    - Example: `STE-3: Refactor collision code`
    - Example: `STE-4: Write collision tests`
    - Example: `STE-5: Write collision docs`
