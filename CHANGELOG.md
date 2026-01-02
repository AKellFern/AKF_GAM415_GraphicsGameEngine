# Changelog

## Module 1 – Project Setup

### Added
- Initialized Unreal Engine project using a source-built Unreal Engine 5.x setup.
- Configured Git, GitHub repository, and Git LFS for Unreal Engine binary assets.
- Established project folder structure and version control exclusions for generated Unreal directories.
- Integrated Shooter-style gameplay framework to support firing and projectile logic required for course activities.
- Placed `ShooterPickup` actor in the base level to allow the player character to acquire and equip a weapon during gameplay.

### Current State / Placeholders
- Projectile mesh currently uses the template-provided material instance `MI_FirstPersonColorway` as a temporary placeholder.
- Template-provided shooting logic is being used as baseline infrastructure and will be incrementally replaced or extended with original implementations in later modules.

### Notes
- Shooter framework is used intentionally for foundational functionality only.
- Visual effects, materials, decals, and particle systems related to projectiles are not considered final at this stage and will be re-authored as part of future coursework.
