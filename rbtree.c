/*
 * rbtree.c
 *
 *      Author: Michele
 */

#include "rbtree.h"
#include <stdio.h>


void RBTreeSetup(rbTree* tree, node* sentinella) {
	sentinella->key = -1;
	sentinella->color = black;
	sentinella->p = sentinella->right = sentinella->left = sentinella;
	sentinella->size = 0; //definita in tal modo
	tree->root = tree->nil = sentinella;
}

//Precondizione: RBTreeSetup
void InorderRBTreeWalk(node* x, rbTree tree) {
	if(x != tree.nil) {
		InorderRBTreeWalk(x->left, tree);
		PrintNode(*x);
		InorderRBTreeWalk(x->right, tree);
	}
}

void PrintNode(node x) {
	printf("key = %d; size = %d; color = ", x.key, x.size);
	switch(x.color) {
	case 0 :
		printf("black\n");
		break;
	case 1 :
		printf("red\n");
		break;
	default :
		fprintf(stderr,"inesistente\n");
	}
}

int CalculateSize(node* x) {
	return (x->left->size + x->right->size + 1);
}

//Precondizione: RBTreeSetup
void LeftRotate(rbTree* tree, node* x) {
	node* y = x->right;
	//operazione 1
	x->right = y->left;
	if(y->left != tree->nil)
		y->left->p = x;
	//operazione 2
	y->p = x->p;
	if(x->p == tree->nil)
		tree->root = y;
	else {
		if(x == x->p->left)
			x->p->left = y;
		else
			x->p->right =y;
	}
	//operazione 3
	y->left = x;
	x->p = y;
	x->size = CalculateSize(x);
	y->size = CalculateSize(y);
}

//Precondizione: RBTreeSetup
void RightRotate(rbTree* tree, node* x) {
	node* y = x->left;
	//operazione 1
	x->left = y->right;
	if(y->right != tree->nil)
		y->right->p = x;
	//operazione 2
	y->p = x->p;
	if(x->p == tree->nil)
		tree->root = y;
	else {
		if(x == x->p->right)
			x->p->right = y;
		else
			x->p->left =y;
	}
	//operazione 3
	y->right = x;
	x->p = y;
	x->size = CalculateSize(x);
	y->size = CalculateSize(y);
}

//Precondizione: RBTreeSetup
void RBInsertFixup(rbTree* tree, node* z) {
	while(z->p->color == red){
		if(z->p == z->p->p->left) {
			node* y = z->p->p->right;
			if(y->color == red) {
				z->p->color = black;
				y->color = black;
				z->p->p->color = red;
				z = z->p->p;
			}
			else {
				if(z == z->p->right){
					z = z->p;
					LeftRotate(tree, z);
				}
				z->p->color = black;
				z->p->p->color = red;
				RightRotate(tree, z->p->p);
			}
		}
		else {
			node* y = z->p->p->left;
			if(y->color == red) {
				z->p->color = black;
				y->color = black;
				z->p->p->color = red;
				z = z->p->p;
			}
			else {
				if(z == z->p->left){
					z = z->p;
					RightRotate(tree, z);
				}
				z->p->color = black;
				z->p->p->color = red;
				LeftRotate(tree, z->p->p);
			}
		}
	}
	tree->root->color = black;
}

//Precondizione: RBTreeSetup
void RBInsert(rbTree* tree, node* z) {
	node* y = tree->nil;
	node* x = tree->root;
	while(x != tree->nil) {
		y = x;
		if(z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	z->p = y;
	if(y == tree->nil) //albero vuoto
			tree->root = z;
	else {
		if(z->key < y->key)
			y->left = z;
		else
			y->right = z;
	}
	z->left = tree->nil;
	z->right = tree->nil;
	z->color = red;
	z->size = 1;
	while(y != tree->nil) { //O(h)
		y->size = CalculateSize(y);
		y = y->p;
	}
	RBInsertFixup(tree, z);
}

//OSSelect restituisce un puntatore al nodo che contiene
//l'i-esima chiave piu' piccola nel sottoalbero con
//radice in x
node* OS_Select(node* x, int i, rbTree tree) {
	if(i < 1 || i > x->size)
		return tree.nil;
	int r = x->left->size + 1; //rango di x nel
				//sottoalbero di radice x
//la posizione di x nella sequenza ordinata degli
//elementi del sottoalbero di radice x e' quella che
//segue tutti gli elementi del suo sottoalbero sinistro
	if(i == r)
		return x; //x e' l'i-esimo elemento piu' piccolo
	else {
		if(i < r) //l'i-esimo elemento piu' piccolo e' nel
				  //sottoalbero sinistro
			return OS_Select(x->left, i, tree);
		else //l'i-esimo elemento piu' piccolo e' nel
			//sottoalbero destro
			return OS_Select(x->right, i-r, tree);
//l'i-esimo elemento piu' piccolo nel sottoalbero con
//radice in x e' l'(i-r)-esimo elemento piu' piccolo nel
//sottoalbero con radice x->right, poiche' ci sono r
//elementi che precedono il sottoalbero destro di x
	}
}

