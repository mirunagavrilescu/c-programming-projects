### 4. Battleships & Data Analysis (C Game Engine)
A modular implementation of the classic Battleships game, featuring dynamic memory management and an advanced statistical analysis module for player accuracy.

* **Architecture:** The project is modularized into three main files: `battleships.c` (main execution), `battleships_functions.c` (core logic implementation), and `battleships.h` (data structures and function prototypes).

#### Task 1: Core Game Engine
* **Data Structures:** Utilizes custom `structs` to manage players and ships. The player struct tracks their defense board, attack board, ship fleet, and destruction counters, while the ship struct stores coordinates, orientation, length, and hit count.
* **Dynamic Memory Management:** Game boards of variable dimensions (N x M) are dynamically allocated and strictly deallocated (`cleanup_joc`) to ensure no memory leaks occur.
* **Game Logic & Validation:** 
  * **Placement:** Validates ship coordinates dynamically to ensure they remain within grid bounds and do not overlap (`nava_valida`).
  * **Combat System:** Alternates turns, registering Misses (`R`), Hits (`H`), or Destroyed Heads (`X`). 
  * **Sinking Mechanics:** Once a ship receives hits equal to its length, it is officially sunk. The algorithm automatically outlines its surrounding perimeter on the grid (`marcheaza_scufundata`) and evaluates the victory condition.

#### Task 2: Statistical Accuracy Optimizer
* **Data Tracking:** Extracts and stores real-time match data (successful hits vs. total valid attacks) during the combat phase.
* **Algorithmic Analysis:** Parses post-game commands using string manipulation (`strncpy`) to calculate the minimum number of match modifications required to hit a specific weighted average target.
* **Optimization Logic (Greedy Approach):**
  * **MIN Optimization:** Sorts matches by potential gain, artificially setting the most impactful matches to 100% accuracy to maximize the global average efficiently.
  * **MAX Optimization:** Sorts matches by impact and sets high-performing matches to 0% accuracy until the average drops below the threshold. Includes an optimal rollback check to ensure the absolute minimum number of modifications is made without overshooting the target.
