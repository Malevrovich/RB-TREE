#define BLACK false
#define RED true

#include <iostream>
#include <string>

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

	void Erase(const K& key){
		// Не будем удалять и памяти у нас много!
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
