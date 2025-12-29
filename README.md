# DSA__PROJECT
Social Network Analysis using Graphs and Priority Queues. A C++ Data Structures project that models a social network using graphs, BFS/DFS, linked lists, and priority queues to analyze connections and recommend friends.

# Features
- Add new users to the social network
- Create and remove friendships between users
- Find the connection distance between two users using Breadth-First Search (BFS)
- Display mutual friends between two users
- Recommend friends based on the highest number of mutual connections using a heap
- Menu-driven console interface

# Data Structures & Algorithms Used
- **Graph** (Adjacency List Representation)
- **Linked List** (for storing friendships)
- **Array** (user storage, visited tracking, queues)
- **Queue** (array-based, for BFS traversal)
- **Priority Queue (Max Heap)** for friend recommendation ranking
- **Breadth-First Search (BFS)** for shortest connection distance

# How the Recommendation Works
- Existing friends of the selected user are marked
- Each non-friend user is checked for mutual connections
- Users with one or more mutual friends are inserted into a **max heap**
- Recommendations are displayed in descending order of mutual friends

# BY: Ifra Abdullah Khan (BSCS)-Air University Islamabad
