#define BLACK false
#define RED true

#include <iostream>
#include <string>

/*
 * Valgrind
 *
 * ==1866== Memcheck, a memory error detector
 * ==1866== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
 * ==1866== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
 * ==1866== Command: ./main.o
 * ==1866== HEAP SUMMARY:
 * ==1866==     in use at exit: 0 bytes in 0 blocks
 * ==1866==   total heap usage: 1,263,843 allocs, 1,263,843 frees, 68,737,072 bytes allocated
 * ==1866==
 * ==1866== All heap blocks were freed -- no leaks are possible
 * ==1866==
 * ==1866== For lists of detected and suppressed errors, rerun with: -s
 * ==1866== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 */

struct KeyError{
	KeyError(){}

	KeyError(char *_what){
		what = _what;
	}

	char* what;
};

template <typename K, typename V>
struct Node{
	Node *parent;
	Node *left;
	Node *right;
	K key;
	V value;
	bool color;
	bool nil = false;

	bool isNil(){
		return nil;
	}
};

template <typename K, typename V>
struct Nil: public Node<K, V>{
	Nil(){
		this->color = BLACK;
		this->left = NULL;
		this->right = NULL;
		this->parent = NULL;
		this->nil = true;
	}
};

template <typename K, typename V>
class Tree{
public:
	Tree(){
		nil = new Nil<K, V>();
		root = nil;
	}

	~Tree(){
		clear();
	}

	void Insert(const K& key, const V& value){
		Node<K, V> *y = nil;
		Node<K, V> *x = root;

		while(!x->isNil()){
			y = x;
			if(key < x->key){
				x = x->left;
			} else if(key > x->key){
				x = x->right;
			} else{
				x->value = value;
				return;
			}
		}

		Node<K, V> *z = new Node<K, V>;
		z->key = key;
		z->value = value;
		z->parent = y;

		if(y->isNil()){
			root = z;
		} else if(key < y->key){
			y->left = z;
		} else{
			y->right = z;
		}

		z->left = nil;
		z->right = nil;
		z->color = RED;
		Fix(z);
	}

	void erase(Node<K, V>* node); // Что такое erase(), зачем это?

	void clear(){
		clear(root->left);
		clear(root->right);
		delete root;
		delete nil;
	}

	V& Get(const K& key){
		return GetNode(key)->value;
	}

	const V& Get(const K& key) const{
		return GetNode(key)->value;
	}

	V& operator[](const K& key){
		return Get(key);
	}

	const V& operator[](const K& key) const{
		return Get(key);
	}

private:

	void lrot(Node<K, V> *x){
		Node<K, V> *y = x->right;
		x->right = y->left;

		if(!y->left->isNil()){
			y->left->parent = x;
		}
		y->parent = x->parent;

		if(x->parent->isNil()){
			root = y;
		} else if(x == x->parent->left){
			x->parent->left = y;
		} else{
			x->parent->right = y;
		}
		y->left = x;

		x->parent = y;
	}

	void rrot(Node<K, V> *y){
		Node<K, V> *x = y->left;
		y->left = x->right;

		if(!x->right->isNil()){
			x->right->parent = y;
		}
		x->parent = y->parent;

		if(y->parent->isNil()){
			root = x;
		} else if(y == y->parent->left){
			y->parent->left = x;
		} else{
			y->parent->right = x;
		}
		x->right = y;

		y->parent = x;
	}

	void Fix(Node<K, V> *z){
		while(z->parent->color == RED){
			if(z->parent == z->parent->parent->left){
				Node<K, V> *y = z->parent->parent->right;
				if(y->color == RED){
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				} else{
					if(z == z->parent->right){
						z = z->parent;
						lrot(z);
					}
					z->parent->color = BLACK;
					z->parent->parent->color = RED;
					rrot(z->parent->parent);
				}
			} else{
				Node<K, V> *y = z->parent->parent->left;
				if(y->color == RED){
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				} else{
					if(z == z->parent->left){
						z = z->parent;
						rrot(z); //???
					}
					z->parent->color = BLACK;
					z->parent->parent->color = RED;
					lrot(z->parent->parent);
				}
			}
		}
		root->color = BLACK;
	}

	void clear(Node<K, V> *node){
		if(node->isNil()){
			return;
		}
		clear(node->left);
		clear(node->right);
		delete node;
	}

	Node<K, V> *GetNode(const K& key){
		Node<K, V> *x = root;

		while(!x->isNil()){
			if(key < x->key){
				x = x->left;
			} else if(key > x->key){
				x = x->right;
			} else{
				return x;
			}
		}

		throw KeyError();
	}

	Node<K, V> *root;
	Nil<K, V> *nil;
};
