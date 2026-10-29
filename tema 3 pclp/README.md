### 3. Fractal Renderer, L-Systems & Graphics Engine (C)
A comprehensive low-level graphics engine built in C, capable of generating complex L-System fractals, rendering custom BDF typography, and performing intricate bit-level image manipulations. 

#### System Architecture & Memory Management
* **Unified State & Undo/Redo:** The entire application state (current image, L-System rules, history, typography) is managed through a single centralized `game` structure passed by reference for high performance.
* **Optimized History Tracking:** Features a custom, memory-efficient Undo/Redo system. Instead of constantly deep-copying the entire state, it selectively captures full image snapshots (`img_state`) during rendering or just lightweight parameters during file loads to prevent memory bloat.

#### Core Modules
* **L-System Fractal Engine:** 
  * Parses input files to extract axioms and production rules, storing them in an ASCII-indexed pointer array for rapid **O(1) rule lookups**.
  * Uses a highly optimized derivation function (`derive`) that pre-calculates the exact required byte size before dynamically allocating memory for the resulting fractal string.
* **Turtle Graphics & Bresenham's Algorithm:** 
  * Translates the derived fractal strings into visual paths using trigonometric transformations (sine/cosine via `<math.h>`) and a custom implementation of **Bresenham's line algorithm** (`draw_line`) for optimal pixel plotting.
  * Employs a dynamically resizing **LIFO stack** to push and pop the "turtle's" state (X, Y coordinates, and trajectory angle modulo 360) when parsing `[` and `]` symbols. This allows for the generation of highly complex recursive branching structures without fixed limitations.
* **Typography System (BDF Font Parser):** 
  * Parses `.bdf` font files line-by-line, detecting structural markers like `STARTCHAR` and `BBX`.
  * Converts raw hexadecimal pixel data into binary matrices bit-by-bit (`hex_to_bin`).
  * Renders text to the grid accurately by respecting bounding boxes, X/Y coordinate offsets, and dynamic cursor kerning (`dx`) to ensure characters do not overlap.
* **Bit-Level Image Engraving:** 
  * Features a custom bit-scanner (`bitcheck`) that traverses raw image memory using a 4-bit sliding window to detect specific binary patterns (`0010` or `1101`). 
  * Utilizes reverse mathematical routing to translate a continuous absolute bit index back into its corresponding byte, pixel index, specific RGB channel, and 2D visual coordinates (X, Y).
  * Simulates visual artifacts using targeted **XOR bit-flips** on the unstable bits, displaying the result without corrupting the underlying primary image data in memory.
