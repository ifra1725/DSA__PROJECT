#include <iostream>
#include <cstring>
using namespace std;

/* ================= CONSTANTS ================= */
const int MAX_USERS = 50;
const int MAX_NAME  = 30;

/* ================= USER DATA ================= */
char users[MAX_USERS][MAX_NAME];
int userCount = 0;

/* ================= GRAPH ================= */
struct Node {
    int data;
    Node* next;
};

Node* adj[MAX_USERS];

/* ================= INPUT ================= */
void clearInput() {
    cin.clear();
    cin.ignore(1000, '\n');
}

/* ================= USER UTIL ================= */
int findUser(char name[]) {
    for (int i = 0; i < userCount; i++)
        if (strcmp(users[i], name) == 0)
            return i;
    return -1;
}

void addUser(char name[]) {
    if (userCount == MAX_USERS) {
        cout << "\n[INFO] User limit reached.\n";
        return;
    }
    if (findUser(name) != -1) {
        cout << "\n[INFO] User already exists.\n";
        return;
    }

    strcpy(users[userCount], name);
    adj[userCount++] = NULL;
    cout << "\n[SUCCESS] User added successfully.\n";
}

/* ================= LIST HELPERS ================= */
Node* newNode(int v) {
    Node* n = new Node;
    n->data = v;
    n->next = NULL;
    return n;
}

int isFriend(int u, int v) {
    for (Node* t = adj[u]; t; t = t->next)
        if (t->data == v) return 1;
    return 0;
}

/* ================= FRIEND OPS ================= */
void addFriend(char a[], char b[]) {
    int u = findUser(a);
    int v = findUser(b);

    if (u == -1 || v == -1) {
        cout << "\n[INFO] One or both users do not exist.\n";
        return;
    }
    if (u == v) {
        cout << "\n[INFO] A user cannot befriend themselves.\n";
        return;
    
    }
    if (isFriend(u, v)) {
        cout << "\n[INFO] Users are already friends.\n";
        return;
    }

    Node* n1 = newNode(v);
    n1->next = adj[u];
    adj[u] = n1;

    Node* n2 = newNode(u);
    n2->next = adj[v];
    adj[v] = n2;

    cout << "\n[SUCCESS] Friendship added.\n";
}

void deleteEdge(int u, int v) {
    Node* cur = adj[u];
    Node* prev = NULL;

    while (cur) {
        if (cur->data == v) {
            if (prev == NULL)
                adj[u] = cur->next;
            else
                prev->next = cur->next;
            delete cur;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void removeFriend(char a[], char b[]) {
    int u = findUser(a);
    int v = findUser(b);

    if (u == -1 || v == -1) {
        cout << "\n[INFO] One or both users do not exist.\n";
        return;
    }

    deleteEdge(u, v);
    deleteEdge(v, u);

    cout << "\n[SUCCESS] Friendship removed.\n";
}

/* ================= BFS ================= */
void findConnection(char a[], char b[]) {
    int src = findUser(a);
    int dest = findUser(b);

    if (src == -1 || dest == -1) {
        cout << "\n[INFO] One or both users do not exist.\n";
        return;
    }

    int visited[MAX_USERS] = {0};
    int dist[MAX_USERS];
    for (int i = 0; i < MAX_USERS; i++) dist[i] = -1;

    int q[MAX_USERS];
    int front = 0, rear = 0;

    q[rear++] = src;
    visited[src] = 1;
    dist[src] = 0;

    while (front < rear) {
        int cur = q[front++];

        if (cur == dest) {
            cout << "\n[RESULT] Connection distance: " << dist[cur] << "\n";
            return;
        }

        for (Node* t = adj[cur]; t; t = t->next) {
            if (!visited[t->data]) {
                visited[t->data] = 1;
                dist[t->data] = dist[cur] + 1;
                q[rear++] = t->data;
            }
        }
    }

    cout << "\n[RESULT] No connection exists.\n";
}


/* ================= MUTUAL FRIENDS ================= */
void mutualFriends(char a[], char b[]) {
    int u = findUser(a);
    int v = findUser(b);

    if (u == -1 || v == -1) {
        cout << "\n[INFO] One or both users do not exist.\n";
        return;
    }

    int mark[MAX_USERS] = {0};
    bool found = false;

    for (Node* t = adj[u]; t; t = t->next)
        mark[t->data] = 1;

    cout << "\n[RESULT] Mutual friends: ";
    for (Node* t = adj[v]; t; t = t->next) {
        if (mark[t->data]) {
            cout << users[t->data] << " ";
            found = true;
        }
    }

    if (!found) cout << "None";
    cout << "\n";
}

/* ================= PRIORITY QUEUE ================= */
struct Heap {
    int user[MAX_USERS];
    int mutual[MAX_USERS];
    int size;
};

void initHeap(Heap &h) {
    h.size = 0;
}

void heapInsert(Heap &h, int u, int m) {
    int i = h.size++;
    h.user[i] = u;
    h.mutual[i] = m;

    while (i > 0 && h.mutual[(i-1)/2] < h.mutual[i]) {
        swap(h.mutual[i], h.mutual[(i-1)/2]);
        swap(h.user[i], h.user[(i-1)/2]);
        i = (i-1)/2;
    }
}

int heapPop(Heap &h, int &m) {
    int res = h.user[0];
    m = h.mutual[0];

    h.user[0] = h.user[--h.size];
    h.mutual[0] = h.mutual[h.size];

    int i = 0;
    while (true) {
        int l = 2*i+1, r = 2*i+2, best = i;
        if (l < h.size && h.mutual[l] > h.mutual[best]) best = l;
        if (r < h.size && h.mutual[r] > h.mutual[best]) best = r;
        if (best == i) break;
        swap(h.mutual[i], h.mutual[best]);
        swap(h.user[i], h.user[best]);
        i = best;
    }
    return res;
}

/* ================= RECOMMEND ================= */
void recommendFriends(char name[]) {
    int u = findUser(name);
    if (u == -1) {
        cout << "\n[INFO] User does not exist.\n";
        return;
    }

    int isF[MAX_USERS] = {0};
    for (Node* t = adj[u]; t; t = t->next)
        isF[t->data] = 1;

    Heap h;
    initHeap(h);

    for (int i = 0; i < userCount; i++) {
        if (i == u || isF[i]) continue;

        int cnt = 0;
        for (Node* t = adj[i]; t; t = t->next)
            if (isF[t->data]) cnt++;

        if (cnt > 0)
            heapInsert(h, i, cnt);
    }

    if (h.size == 0) {
        cout << "\n[RESULT] No recommendations available.\n";
        return;
    }

    cout << "\n[RESULT] Friend Recommendations:\n";
    cout << "----------------------------------\n";

    while (h.size) {
        int mutual;
        int idx = heapPop(h, mutual);

        cout << users[idx] << " (Mutual: ";
        bool first = true;
        for (Node* t = adj[idx]; t; t = t->next) {
            if (isF[t->data]) {
                if (!first) cout << ", ";
                cout << users[t->data];
                first = false;
            }
        }
        cout << ")\n";
    }
}

/* ================= MAIN ================= */
int main() {
    for (int i = 0; i < MAX_USERS; i++)
        adj[i] = NULL;

    int ch;
    char a[MAX_NAME], b[MAX_NAME];

    do {
        cout << "\n==================================\n";
        cout << "      SOCIAL NETWORK MENU\n";
        cout << "==================================\n";
        cout << "1. Add User\n";
        cout << "2. Add Friend\n";
        cout << "3. Remove Friend\n";
        cout << "4. Find Connection\n";
        cout << "5. Mutual Friends\n";
        cout << "6. Recommend Friends\n";
        cout << "0. Exit\n";
        cout << "----------------------------------\n";
        cout << "Enter choice: ";

        cin >> ch;
        clearInput();

        switch (ch) {
        case 1:
            cout << "Enter user name: ";
            cin.getline(a, MAX_NAME);
            addUser(a);
            break;
        case 2:
            cout << "Enter first user: ";
            cin.getline(a, MAX_NAME);
            cout << "Enter second user: ";
            cin.getline(b, MAX_NAME);
            addFriend(a, b);
            break;
        case 3:
            cout << "Enter first user: ";
            cin.getline(a, MAX_NAME);
            cout << "Enter second user: ";
            cin.getline(b, MAX_NAME);
            removeFriend(a, b);
            break;
        case 4:
            cout << "Enter source user: ";
            cin.getline(a, MAX_NAME);
            cout << "Enter target user: ";
            cin.getline(b, MAX_NAME);
            findConnection(a, b);
            break;
        case 5:
            cout << "Enter first user: ";
            cin.getline(a, MAX_NAME);
            cout << "Enter second user: ";
            cin.getline(b, MAX_NAME);
            mutualFriends(a, b);
            break;
        case 6:
            cout << "Enter user: ";
            cin.getline(a, MAX_NAME);
            recommendFriends(a);
            break;
        case 0:
            cout << "\nExiting program...\n";
            break;
        default:
            cout << "\n[INFO] Invalid choice.\n";
        }

    } while (ch != 0);

    return 0;
}