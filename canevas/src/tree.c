#include "tree.h"

#include <assert.h>
#include <geometry.h>
#include <list.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

/********************************************************************
 * tree_node_t
 ********************************************************************/

/**
 * Construit et initialise un nouveau nœud d'un arbre binaire de recherche.
 * Le facteur d'équilibre est initialisé à zero.
 * 
 * @param[in] key Clé du nouveau nœud.
 * @param[in] data Donnée à affecter au nouveau nœud.
 * @return le nouveau nœud créé
 */
static struct tree_node_t * new_tree_node(void * key, void * data) {
	struct tree_node_t * node = malloc(sizeof(struct tree_node_t));
	node->key = key;
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void * get_tree_node_key(const struct tree_node_t * node) {
	assert(node);
	return node->key;
}

void * get_tree_node_data(const struct tree_node_t * node) {
	assert(node);
	return node->data;
}

struct tree_node_t * get_left(const struct tree_node_t * node) {
	assert(node);
	if (node->left == NULL)
		return NULL;
	else
		return node->left;
}

struct tree_node_t * get_right(const struct tree_node_t * node) {
	assert(node);
	assert(node);
	if (node->right == NULL)
		return NULL;
	else
		return node->right;
}

void set_tree_node_key(struct tree_node_t * node, void * new_key) {
	assert(node);
	node->key = new_key;
}

void set_tree_node_data(struct tree_node_t * node, void * new_data) {
	assert(node);
	node->data = new_data;
}

void set_left(struct tree_node_t * node, struct tree_node_t * new_left) {
	assert(node);
	node->left = new_left;
}

void set_right(struct tree_node_t * node, struct tree_node_t * new_right) {
	assert(node);
	node->right = new_right;
}

/*********************************************************************
 * tree_t
 *********************************************************************/

struct tree_t * new_tree() {
	struct tree_t * T = malloc(sizeof(struct tree_t));
	T-> size = 0;
	T-> root = NULL;
	return T;
}

int tree_is_empty(const struct tree_t * T) {
	assert(T);
	if(T->size == 0) {
		return 1;
	}
	return 0;
}

unsigned int get_tree_size(const struct tree_t * T) {
	assert(T);
	return T->size;
}

struct tree_node_t * get_root(const struct tree_t * T) {
	assert(T);
	return T->root;
}

void increase_tree_size(struct tree_t * T) {
	assert(T);
	T->size++;
}

void decrease_tree_size(struct tree_t * T) {
	assert(T);
	T->size--;
}

void set_root(struct tree_t * T, struct tree_node_t * new_root) {
	assert(T);
	T->root = new_root;
}

/**
 * Libérer récursivement le sous-arbre raciné au nœud \p curr .
 * Dans le cas où le pointeur de fonction \p freeKey (respectivement
 * \p freeData ) n'est pas NULL, la mémoire de la clé (respectivement de la
 * donnée) du nœud actuel est aussi libérée.
 * NB : procédure récursive.
 * 
 * @param[in] curr
 * @param[in] freeKey 
 * @param[in] freeData 
 */
static void delete_tree_node(struct tree_node_t * curr, void (*freeKey)(void *), void (*freeData)(void *)) {
	if (curr == NULL)
	{return;}

	delete_tree_node(get_right(curr), freeKey, freeData);
	delete_tree_node(get_left(curr), freeKey, freeData);

	if (freeKey!=NULL) {freeKey(curr->key);}
	if (freeData!=NULL) {freeData(curr->data);}
	free(curr);
}

// NB : Utiliser la procédure récursive delete_tree_node.
void delete_tree(struct tree_t * T, void (*freeKey)(void *), void (*freeData)(void *)) {
	assert(T);
	delete_tree_node(get_root(T), freeKey, freeData);
	free(T);
}
void viewString(const void * key) {
	assert(key);
	printf("%s", (char*)key);
}

/**
 * Afficher récursivement le sous-arbre raciné au nœud \p curr en utilisant un
 * ordre infixe.
 * NB : procédure récursive.
 *
 * @param[in] curr 
 * @param[in] viewKey 
 * @param[in] viewData 
 */
static void view_tree_inorder(struct tree_node_t * curr, void (*viewKey)(const void *), void (*viewData)(const void *)) {
	if (curr == NULL)
		return;
	else {
		view_tree_inorder(get_left(curr), viewKey, viewData);
		printf("[ ");
		viewKey(get_tree_node_key(curr));
		printf(" / ");
		viewData(get_tree_node_data(curr));
		printf(" ] ");
		view_tree_inorder(get_right(curr), viewKey, viewData);
	}
}

// NB : Utiliser la procédure récursive view_tree_inorder.
void view_tree(const struct tree_t * T, void (*viewKey)(const void * key), void (*viewData)(const void * data)) {
	assert(T);
	view_tree_inorder(T->root, viewKey, viewData);
}

/**
 * Insérer un nouveau nœud de clé \p key et de donnée \p data au sous-arbre
 * binaire de recherche raciné au nœud \p curr .
 * Les clés sont comparées en utilisant le pointeur de fonction \p precedes .
 * Le nœud curant \p curr est en entrée/sortie et doit être mis à jour.
 * NB : fonction récursive.
 * 
 * @param[in,out] curr
 * @param[in] key 
 * @param[in] data 
 * @param[in] precedes
 */
static void insert_tree_node(struct tree_node_t ** curr, void * key, void * data, int (*precedes)(const void *, const void *)) {
	if (*curr == NULL) {
		*curr = new_tree_node(key, data);
	} else if (precedes(key, get_tree_node_key(*curr))) {

		insert_tree_node(&(*curr)->left, key, data, precedes);
	} else {

		insert_tree_node(&(*curr)->right, key, data, precedes);
	}
}
// NB : Utiliser la fonction récursive insert_tree_node.
void tree_insert(struct tree_t * T, void * key, void * data, int (*precedes)(const void * a, const void * b)) {
	assert(T);
	if (tree_is_empty(T)) {
		set_root(T, new_tree_node(key, data));
		increase_tree_size(T);
	}else {
		insert_tree_node(&(T->root), key, data, precedes);
		increase_tree_size(T);
	}
}

struct tree_node_t * tree_find_min(struct tree_node_t * curr) {
	assert(curr);
	if (curr->left == NULL) {
		return curr;
	}
	else
		return tree_find_min(curr->left);
}

struct tree_node_t * tree_find_max(struct tree_node_t * curr) {
	assert(curr);
	if (curr->right == NULL) {
		return curr;
	}
	else
		return tree_find_max(curr->right);
}

struct tree_node_t * tree_find_node(struct tree_node_t * curr, const void * key, int (*precedes)(const void *, const void *)) {
	if (get_tree_node_key(curr)==key)
		return curr;
	if (curr == NULL)
		return NULL;
	else if (precedes(get_tree_node_key(curr), key)==1)
		return tree_find_node(curr->left, key, precedes);
	else
		return tree_find_node(curr->right, key, precedes);

}

struct tree_node_t * tree_find_predecessor(struct tree_node_t * curr, const void * key, int (*precedes)(const void *, const void *)) {
	assert(curr);
    struct tree_node_t *predecessor = NULL;
    if (precedes(get_tree_node_key(curr), key)) {
        predecessor = tree_find_predecessor(get_right(curr), key, precedes);
        if (predecessor == NULL) {
            predecessor = curr;
        }
        return predecessor;
    }
    if (precedes(key, get_tree_node_key(curr))) {
        return tree_find_predecessor(get_left(curr), key, precedes);
    }
    if (get_left(curr) != NULL) {
        return tree_find_max(get_left(curr));
    }
    return NULL;
}

struct tree_node_t * tree_find_successor(struct tree_node_t * curr, const void * key, int (*precedes)(const void *, const void *)) {

  	//assert(curr);
    struct tree_node_t *successor = NULL;
    if (precedes(key, get_tree_node_key(curr))) {
        successor = tree_find_successor(get_left(curr), key, precedes);
        if (successor == NULL) {
            successor = curr;
        }
        return successor;
    }
    if (precedes(get_tree_node_key(curr), key)) {
        return tree_find_successor(get_right(curr), key, precedes);
    }
    if (get_right(curr) != NULL) {
        return tree_find_min(get_right(curr));
    }
    return NULL;
}

/**
 * Supprimer le nœud de clé \p key du sous-arbre binaire de recherche raciné au
 * nœud \p curr . La donnée du nœud supprimé est restituée, tandis que la
 * mémoire du nœud avec clé \p key est libérée (mais pas la mémoire de la clé).
 * Les clés sont comparées en utilisant le pointeur de fonction \p precedes.
 * Le nœud curant \p curr est en entrée/sortie et doit être mis à jour.
 * NB : fonction récursive.
 * 
 * @param[in,out] curr
 * @param[in] key
 * @param[in] precedes
 * @return la donnée du nœud supprimé
 */
static void * remove_tree_node(struct tree_node_t ** curr, void * key, int (*precedes)(const void *, const void *)) {
	assert(curr);
	if (*curr == NULL) {
		return NULL;
	}
	if (precedes(get_tree_node_key(*curr), key)) {
		remove_tree_node(&(*curr)->right, key, precedes);
	}
	else if (precedes(key, get_tree_node_key(*curr))) {
		remove_tree_node(&(*curr)->left, key, precedes);
	}
	else {
		void * data = get_tree_node_data(*curr);
		if (get_left(*curr)==NULL && get_right(*curr)==NULL) {
			free(*curr);
			*curr = NULL;
		}
		else if (get_right(*curr)==NULL) {
			struct tree_node_t * temp = *curr;
			*curr = get_left(*curr);
			free(temp);
		}else if (get_left(*curr)==NULL) {
			struct tree_node_t * temp = *curr;
			*curr = get_right(*curr);
			free(temp);
		}else {
			struct tree_node_t * remplacant = get_right(*curr);
			struct tree_node_t ** temp = &remplacant;
			remplacant=tree_find_successor(remplacant,get_tree_node_key(remplacant),precedes);
			set_tree_node_data(*curr, get_tree_node_data(remplacant));
			set_tree_node_key(*curr, get_tree_node_data(remplacant));
			remove_tree_node(temp,get_tree_node_key(remplacant),precedes);
		}
		return data;
		}

}

// NB : Utiliser la fonction récursive remove_tree_node.
void * tree_remove(struct tree_t * T, void * key, int (*precedes)(const void * a, const void * b)) {
	assert(T);
	void * data = remove_tree_node(&(T->root), key, precedes);
	decrease_tree_size(T);
	return data;
}