
{Team Names / IDs}  
1. Youssef Yasser Elsayed Hamed - 20230519  
2. Hesham Mohammed Koutp - 20230458  
3. SeifEldeen Hassan - 20230182  
4. AbdelRahman Mohammed - 20230216  
5. Jihad Shaban - 20220535


----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ArcadiaEngine: MMORPG Backend System

Course: CS321 - Algorithms Analysis and Design
University: Cairo University, Faculty of Computing and Artificial Intelligence

Project Overview:

Your team has been hired by Arcadia Games to develop the backend engine for their new Massively Multiplayer Online Role-Playing Game (MMORPG). The game requires a highly efficient and scalable system to handle critical game components such as player data management, inventory systems, world navigation, and server scheduling.

To meet these needs, you will develop the ArcadiaEngine library, consisting of four core subsystems. Each subsystem is designed to solve different algorithmic challenges, utilizing various data structures and algorithms. The subsystems are:

Subsystems:

The Registry

Goal: Efficiently manage player data and leaderboards.

Technologies: Hashing, Skip Lists, Red-Black Trees.

Tasks:

Player Lookup (Hashing): Implement a PlayerTable using custom hash functions and handle collisions using double hashing.

Live Leaderboard (Skip List): Implement a Leaderboard using a skip list to allow efficient ranking and retrieval of top players.

Auction House (Red-Black Tree): Implement an AuctionTree using a Red-Black Tree to store items for sale, ordered by price.

The Inventory System

Goal: Manage loot distribution and storage.

Technologies: Dynamic Programming.

Tasks:

Loot Splitting (Partition Problem): Solve the problem of dividing loot (coins) as evenly as possible between two parties.

Inventory Packer (Knapsack Problem): Implement the 0/1 Knapsack problem to maximize the value a player can carry, given weight constraints.

Chat Autocorrect (String DP): Implement a string dynamic programming solution to fix a buggy chat system, where certain characters are replaced with alternatives.

The Navigator

Goal: Manage travel routes and safe paths.

Technologies: Graph Algorithms.

Tasks:

Safe Passage (Path Existence): Determine if a valid path exists between two cities in the game world using graph traversal techniques (like BFS or DFS).

The Bribe (Min Cost Path/MST variant): Solve the Minimum Spanning Tree (MST) problem with a twist—finding the minimum cost to connect cities while considering different currencies.

Teleporter Network (All-Pairs Shortest Path): Calculate the sum of shortest path distances between all pairs of cities using graph algorithms.

The Kernel

Goal: Manage server task scheduling.

Technologies: Greedy Algorithms.

Tasks:

Server Job Scheduler: Implement a greedy algorithm to schedule CPU tasks while respecting cooling intervals for tasks of the same type.
