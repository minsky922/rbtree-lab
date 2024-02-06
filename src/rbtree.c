#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)malloc(sizeof(node_t)); // nil노드 생성
  nil->color = RBTREE_BLACK;
  p->nil = p->root = nil;
  p->root->right = p->root->left = p->root->parent = nil; // root의 자식, 부모 nil로

  return p;
}

/* 수정할 RBtree와 서브트리의 루트(x)를 받아옴 */
void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
    y->left->parent = x;
  
  y->parent = x->parent;

  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}

// left_rotate에서 left, right 자리만 바꿔서 만듦
void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
    y->right->parent = x;
  
  y->parent = x->parent;

  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;

  y->right = x;
  x->parent = y;
}

void post_order_delete(rbtree *t, node_t *node) { // 후위순회로 노드들 지워줌
  if (node != t->nil){
    post_order_delete(t, node->left);
    post_order_delete(t, node->right);
    free(node);
    node = NULL; // 포인터 NULL로 설정
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  post_order_delete(t, t->root); // 딸려 있는 노드 전체 free
  free(t->nil); // nil노드 free
  t->nil = NULL;
  free(t); // 트리 free
  t = NULL;
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y = NULL; // 삼촌
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else {
      y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = (node_t *)malloc(sizeof(node_t)); // 삽입할 노드 만들어줌
  node_t *y = t->nil; // 삽입될 노드의 부모
  node_t *x = t->root; // temp..?
  z->key = key;

  while (x != t->nil){
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y; // 부모 관계 설정
  if (y==t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;

  z->color = RBTREE_RED;
  z->left = z->right = t->nil;

  rbtree_insert_fixup(t, z);
  return z; // 삽입된 노드의 포인터
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root; // 찾은 노드를 가리키는 포인터
  while (x != t->nil && x->key != key) {
    if (x->key > key)
      x = x->left;
    else
      x = x->right;
  }
  if (x == t->nil) // 찾고자하는 key가 없는 경우
    return NULL;
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *p = t->nil; // x의 부모
  node_t *x = t->root;

  while (x != t->nil) {
    p = x;
    x = x->left;
  }

  return p;
}

// root기준 서브트리에서의 최소 노드 포인터 리턴(successor 구하기 위해 구현)
node_t *subtree_min(rbtree *t, node_t *root) {
  node_t *p = root->parent;
  
  while (root != t->nil) {
    p = root;
    root = root->left;
  }

  return p;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *p = t->nil; // x의 부모
  node_t *x = t->root;

  while (x != t->nil) {
    p = x;
    x = x->right;
  }
  
  return p;
}

// u의 부모와의 관계를 v로 옮겨줌(자식은 건드리지 않음)
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) // u가 root였다면
    t->root = v;
  else if (u == u->parent->left) // u가 왼쪽 자식이면
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t *x) {
  node_t *w = NULL; // x의 형제
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      w = x->parent->right;
      if (w->color == RBTREE_RED) { // 형제가 붉은 색이면(경우 1)
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else { // x가 부모의 오른쪽 자식이면
      w = x->parent->left;
      if (w->color == RBTREE_RED) { // 형제가 붉은 색이면(경우 1)
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

// 삭제할 노드의 포인터를 받아서 rb트리 구조를 유지하며 해당 키 삭제
// 자신을 가리키는 부모가 없다면 그 노드를 free시켜줘야 한다.
int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y = z;
  node_t *x = NULL; // 삭제된 노드의 위치(extra black을 붙이기 위한)를 가리키는 애인듯?
  color_t y_original_color = y->color;
  if (z->left == t->nil) { // 삭제할 노드의 왼쪽 자식이 없는 경우
    x = z->right;
    rbtree_transplant(t, z, z->right); // 삭제될 노드의 오른쪽 자식을 삭제될 노드 자리에 넣어줌 -> z 없어짐
  }
  else if (z->right == t->nil) { // 삭제할 노드의 오른쪽 자식이 없는 경우
    x = z->left;
    rbtree_transplant(t, z, z->left);
  }
  else { // 자식이 둘 다 있는 경우
    y = subtree_min(t, z->right); // successor
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z) // z의 succ이 z의 바로 오른쪽 자식이었던 경우
      x->parent = y;
    else {
      rbtree_transplant(t, y, y->right); // y(succ)의 오른쪽 자식이 y의 자리를 차지(succ은 올라가기 때문에)
      y->right = z->right; // succ(y)을 지우는 노드 위치로 올려주기
      y->right->parent = y;
    }
    rbtree_transplant(t, z, y);
    y->left = z->left; // succ을 삭제할 z자리로 완전히 옮겨줌
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_original_color == RBTREE_BLACK) // 실질적으로 삭제된 노드가 검정색이었다면 고쳐줘야함
    rbtree_delete_fixup(t, x);
  free(z); // 위 코드를 보면 z가 전부 transplant에 의해 부모를 잃었음(정확히는 부모가 z를 가리키지 않음)
  z = NULL;
  return 0;
}

void in_order(const rbtree *t, node_t *node, key_t *arr, int *i) { 
  if (node != t->nil){
    in_order(t, node->left, arr, i);
    arr[*i] = node->key;
    (*i)++;
    in_order(t, node->right, arr, i);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int i  = 0;
  in_order(t, t->root, arr, &i);
  return 0;
}