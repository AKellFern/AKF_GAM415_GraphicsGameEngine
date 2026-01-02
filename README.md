# AKF_GAM415_GraphicsGameEngine

## Overview
This repository contains coursework for **GAM415 – Graphics and Game Engine Development**.  
The project is built using **Unreal Engine** with **C++**, and tracks module-based development, engine workflows, and foundational systems implemented throughout the course.

This repository serves as both:
- A record of incremental progress for coursework submissions
- A structured Unreal Engine C++ project suitable for review and future reference

---

## Engine & Tools
- **Engine:** Unreal Engine 5.x (source-built)
- **Language:** C++
- **IDE:** Visual Studio
- **Version Control:** Git + GitHub
- **Large File Support:** Git LFS (for Unreal binary assets)

---

## Repository Structure
```
AKellFern415/
├── Config/            # Project configuration files
├── Content/           # Unreal Engine assets (.uasset, .umap)
├── Source/            # C++ source code
├── .gitignore         # Git ignore rules for Unreal projects
├── .gitattributes    # Git LFS configuration
└── *.uproject         # Unreal project file
```

---

## Version Control Notes
- Unreal binary assets (`.uasset`, `.umap`) are tracked using **Git LFS**
- Generated folders are excluded from version control:
  - `Binaries/`
  - `Intermediate/`
  - `DerivedDataCache/`
  - `Saved/`
  - `.vs/`

---

## Course Context
This project is developed as part of **GAM415** and will evolve across multiple modules.  
Each module may introduce new systems, assets, or engine-level concepts as required by the course curriculum.

---

### Engine Template Architecture Note (UE 5.7)

Unreal Engine 5.7 restructures the First Person template by separating gameplay mechanics into modular variants rather than embedding them directly in the base template. For example, shooter-specific functionality (projectiles, firing input, weapon logic) is implemented within the `Variant_Shooter` content rather than the default First Person setup.

This modular approach allows multiple gameplay styles (e.g., horror, shooter) to coexist within a single project while sharing core engine systems such as input, movement, and camera control. This design reflects current trends in game engine architecture emphasizing modularity, reusability, and separation of concerns.

---

## Status
🚧 **In Progress** — Active development during the GAM415 term.
