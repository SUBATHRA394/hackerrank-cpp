#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node{
   Node* next;
   Node* prev;
   int value;
   int key;
   Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
   Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{
   
   protected: 
   map<int,Node*> mp; //map the key to the node in the linked list
   int cp;  //capacity
   Node* tail; // double linked list tail pointer
   Node* head; // double linked list head pointer
   virtual void set(int, int) = 0; //set function
   virtual int get(int) = 0; //get function

};
class LRUCache : public Cache {
public:
    // Constructor to initialize capacity and pointers
    LRUCache(int capacity) {
        cp = capacity;
        head = NULL;
        tail = NULL;
    }

    // Helper: Removes a node from its current position in the linked list
    void removeNode(Node* node) {
        if (!node) return;
        
        if (node->prev) node->prev->next = node->next;
        else head = node->next;

        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
    }

    // Helper: Adds a node to the very front (Head) of the list
    void setHead(Node* node) {
        node->next = head;
        node->prev = NULL;
        
        if (head) head->prev = node;
        head = node;
        
        if (!tail) tail = head;
    }

    // Implementation of the virtual set function
    void set(int key, int value) override {
        if (mp.find(key) != mp.end()) {
            // Key exists: update value and move to front
            Node* node = mp[key];
            node->value = value;
            removeNode(node);
            setHead(node);
        } else {
            // New key: create node
            Node* newNode = new Node(key, value);
            
            // If at capacity, evict the tail (Least Recently Used)
            if (mp.size() >= cp) {
                mp.erase(tail->key);
                Node* temp = tail;
                removeNode(tail);
                delete temp;
            }
            
            // Add to map and list
            setHead(newNode);
            mp[key] = newNode;
        }
    }

    // Implementation of the virtual get function
    int get(int key) override {
        if (mp.find(key) == mp.end()) {
            return -1;
        }
        
        Node* node = mp[key];
        // Move to head because it was recently accessed
        removeNode(node);
        setHead(node);
        
        return node->value;
    }
};
int main() {
   int n, capacity,i;
   cin >> n >> capacity;
   LRUCache l(capacity);
   for(i=0;i<n;i++) {
      string command;
      cin >> command;
      if(command == "get") {
         int key;
         cin >> key;
         cout << l.get(key) << endl;
      } 
      else if(command == "set") {
         int key, value;
         cin >> key >> value;
         l.set(key,value);
      }
   }
   return 0;
}
