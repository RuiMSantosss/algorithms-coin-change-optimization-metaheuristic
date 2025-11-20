# Metaheuristic Optimization: The Coin Change Problem

## Executive Summary
This project utilizes **C** to explore and implement advanced **Metaheuristic** strategies applied to the **Coin Change Problem**.

The challenge is a classic example of combinatorial optimization: finding the minimum cardinality of coins required to reach a specific target value $V$. While traditional approaches use Dynamic Programming, this project investigates the efficiency of **stochastic search algorithms** in navigating the solution space to find near-optimal solutions efficiently.

## Algorithms Implemented
The system features three distinct algorithmic approaches, ranging from trajectory-based methods to population-based logic:

### 1. Local Search (Trajectory-Based)
Focuses on iterative improvement by exploring the "neighborhood" of a current solution.
* **Hill Climbing:** A greedy approach that constantly moves towards better neighboring solutions.
* **Simulated Annealing:** A probabilistic technique that accepts worse solutions early in the process (based on a "temperature" parameter) to escape local optima.

### 2. Evolutionary Algorithm (Population-Based)
Inspired by biological evolution, this method maintains a population of candidate solutions.

* **Mechanisms:** Implements **Tournament Selection**, **Uniform Crossover**, and **Bit-Flip Mutation**.
* **Fitness Function:** Evaluates candidates based on the validity of the sum and the minimization of the coin count.

### 3. Hybrid Algorithm
A composite strategy designed to leverage the strengths of both previous methods.
* **Workflow:** It utilizes the **Evolutionary Algorithm** for broad global exploration of the search space, followed by a **Local Search** refinement phase to intensify exploitation around promising regions.

## Technical Stack
* **Language:** C (Standard C99/C11)
* **IDE:** CLion (JetBrains)
* **Analysis:** Experimental data and performance metrics analyzed via Spreadsheet tools (Excel).

## Experimental Validation
The project includes a comprehensive performance analysis comparing the three approaches. The benchmarks focus on:
* **Convergence Speed:** How fast a solution is found.
* **Solution Quality:** Closeness to the global optimum.
* **Parameter Tuning:** Analysis of different mutation rates, population sizes, and cooling schedules.

*Full datasets and comparative graphs can be found in the `Experiências.xlsx` file.*

---

## Academic Context
This software was engineered as part of the **"Introduction to Artificial Intelligence"** course (2024/2025).

**Team Size:** 2 Members
**Final Evaluation:** 9.8 / 10
