typedef struct {
  short weight;
  char* name;
} Node;

typedef Node*(FindNeighbours)(Node* node);

int Traverse(Node* from, Node* to, FindNeighbours find) {
  if (from == to) {
    return 0;
  }
  Node* next = find(from);
  return next->weight + Traverse((Node*)&((Node*)(&next[3].name))->weight, to, find);
}

