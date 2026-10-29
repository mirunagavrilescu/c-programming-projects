# C Programming Projects: Algorithmic Problem Solving & System Design

This repository contains a curated collection of algorithmic and low-level graphics projects developed in C. These projects focus on advanced data processing, dynamic memory management, computational geometry, and bit-level manipulations.

## Tech Stack & Core Competencies
* **Language:** C (Standard Library, `<math.h>`, dynamic allocation)
* **Core Concepts:** Matrix operations, Custom Data Structures (Stacks, Arrays of Pointers), Computational Geometry, Algorithmic Optimization (Greedy), System Design (Undo/Redo, State Management).

---

## Projects Overview

### 1. Window Join (Time-Series Data Processing)
A program designed to process streams of timestamp-value pairs based on a specific time "window". 
* **How it works:** Continuously reads data until a termination condition `(0,0)` is met. It iterates through pairs, identifying elements where the time difference is within the defined window.
* **Math Operations:** Dynamically calculates and outputs the **Least Common Multiple (LCM)** and **Greatest Common Divisor (GCD)** for valid pairs.

### 2. Akari (Grid Puzzle Logic Solver)
An algorithmic implementation of the rules for the logic puzzle "Akari" (Light Up).
* **Light Propagation:** Simulates light rays starting from bulbs `(L)`. The light travels in all four cardinal directions, illuminating empty cells, stopping only at black cells or grid boundaries.
* **Rule Validation:** Verifies that no two bulbs illuminate each other and checks numbered cells for the exact required adjacent bulbs. Outputs validation status based on strict puzzle rules.

### 3. Helicopters (Matrix & Computational Geometry)
A spatial validation program checking the positioning of objects (right isosceles triangles) on a grid terrain.
* **Coordinate Validation:** Validates if coordinates correctly form a right isosceles triangle parallel to matrix diagonals.
* **Terrain Impact:** Calculates the total area of the "helicopter" footprint and the area affected by its shadow to determine landing accuracy (perfect, poor, or valid with shadow).

### 4. Battleships & Data Analysis Engine
A modular implementation of Battleships, featuring dynamic memory management and a statistical analysis module for player accuracy.
* **Core Engine:** Uses custom `structs` for players and ships. Game boards are dynamically allocated. Includes complete logic for placement validation, alternating turns, hit registration, and perimeter outlining upon sinking a ship.
* **Statistical Optimizer:** Extracts real-time match data. Uses a **Greedy algorithmic approach** to calculate the minimum number of match modifications required (MIN/MAX) to hit specific weighted average accuracy targets.

### 5. Fractal Renderer, L-Systems & Graphics Engine
A comprehensive low-level graphics engine capable of generating L-System fractals, rendering custom BDF typography, and performing bit-level image manipulations. 
* **System Architecture:** Centralized `game` state with a custom, memory-efficient **Undo/Redo system** capturing image snapshots or lightweight parameters.
* **L-System Engine:** Parses axioms/rules for O(1) lookups. Highly optimized derivation pre-calculates required byte size for dynamic memory allocation.
* **Turtle Graphics:** Translates strings into visual paths using trigonometric functions and **Bresenham's line algorithm**. Employs a dynamically resizing **LIFO stack** for complex recursive branching.
* **Typography:** Parses `.bdf` fonts, converting hex data to binary matrices, rendering text with accurate bounding boxes and dynamic kerning.
* **Bit-Level Engraving:** Features a bit-scanner using a 4-bit sliding window to detect binary patterns (`0010`, `1101`). Translates absolute bit indices back to 2D coordinates for targeted **XOR bit-flips**, simulating errors without corrupting core data.
