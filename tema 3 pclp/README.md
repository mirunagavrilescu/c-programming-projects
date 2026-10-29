### 5. Fractal Renderer & Graphics Engine
A comprehensive C-based graphics engine capable of generating complex L-System fractals, rendering custom BDF fonts, and performing intricate bit-level image manipulations.

* **Architecture & State Management:** Built around a centralized, unified state structure. Features a custom **Undo/Redo system** that optimizes memory by selectively capturing either full image snapshots or lightweight command parameters, depending on the action.
* **L-System Fractal Generation:** 
  * Parses axioms and production rules, storing them in an ASCII-indexed pointer array for O(1) rule lookups.
  * Uses a highly optimized derivation engine that precisely pre-calculates the required byte size before dynamically allocating memory for the resulting fractal string.
* **Custom Turtle Graphics:** 
  * Translates the generated strings into visual paths using trigonometric transformations and a custom implementation of **Bresenham's line algorithm**.
  * Employs a dynamically resizing **LIFO stack** to push and pop the "turtle's" state (X, Y, direction) to support complex, recursive branching structures.
* **Typography System (BDF Font Parser):** Reads and parses `.bdf` files line-by-line. Converts hexadecimal pixel data into binary matrices and renders text accurately by respecting bounding boxes (`BBX`), X/Y offsets, and dynamic cursor kerning.
* **Bit-Level Image Engraving:** Features an error-simulation scanner that analyzes raw image memory using a 4-bit sliding window to detect vulnerable binary patterns (`0010` or `1101`). It relies on complex mathematical routing to translate absolute bit indices back into 2D pixel coordinates and RGB channels, applying isolated XOR bit-flips without corrupting the underlying image data.
