# C Programming Projects: Algorithmic Problem Solving

This repository contains a collection of algorithmic projects developed in C, focusing on data processing, matrix manipulation, and computational logic. 

## Tech Stack
* **Language:** C
* **Core Concepts:** Matrix operations, Data structures, Computational geometry, Algorithmic validation.

---

## Projects Overview

### 1. Window Join (Time-Series Data Processing)
A program designed to process streams of timestamp-value pairs based on a specific time "window". 
* **How it works:** It continuously reads data until a termination condition `(0,0)` is met. It then iterates through the pairs, identifying elements where the time difference is within the defined window.
* **Math Operations:** For valid pairs, it dynamically calculates and outputs the **Least Common Multiple (LCM)** and **Greatest Common Divisor (GCD)**.

### 2. Akari (Grid Puzzle Logic Solver)
An algorithmic implementation of the rules for the logic puzzle "Akari" (Light Up).
* **Light Propagation:** Simulates light rays starting from bulbs `(L)`. The light travels in all four cardinal directions, illuminating empty cells `(-)` and turning them into `(x)`, stopping only at black cells or grid boundaries.
* **Rule Validation:** 
  * Verifies that no two bulbs illuminate each other.
  * Checks numbered cells to ensure they have the exact required number of adjacent bulbs.
  * Outputs `ichi` (1) if the board meets all puzzle rules, and `zero` (0) otherwise.

### 3. Helicopters (Matrix & Computational Geometry)
A spatial validation program that checks the positioning of objects (represented as right isosceles triangles) on a grid terrain.
* **Coordinate Validation:** Reads coordinates for the hypotenuse and the direction of the vertex. It validates if the coordinates correctly form a right isosceles triangle parallel to the matrix diagonals.
* **Terrain Impact Calculation:** Calculates the total area of the "helicopter" footprint and the area affected by its shadow.
* **Decision Logic:** Evaluates the landing accuracy based on the terrain cells (0 or 1). It categorizes each placement as a perfect landing, a poor landing (more than half the cells affected by shadow), or a valid placement that only casts a shadow, outputting a sorted report of the invalid placements.
