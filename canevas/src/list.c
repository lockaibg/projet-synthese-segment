#include "list.h"
#include "rational.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "geometry.h"
/********************************************************************
 * list_node_t
 ********************************************************************/

/**
 * Construit et initialise un nouveau nœud d'une liste doublement chaînée.
 * Le paramètre \p data correspond à la donnée à affecter au nouveau nœud.
 * 
 * @param[in] data
 * @return le nouveau nœud créé
 */
static struct list_node_t * new_list_node(void * data) {//tested
	struct list_node_t *new_node = (struct list_node_t *)malloc(sizeof(struct list_node_t));

	new_node->data = data;
    new_node->successor = NULL;
    new_node->predecessor = NULL;
    return new_node;
}

void * get_list_node_data(const struct list_node_t * node) {//tested
	assert(node);
	return node->data;
}

struct list_node_t * get_successor(const struct list_node_t* node) {//tested
	assert(node);
	return node->successor;
}

struct list_node_t * get_predecessor(const struct list_node_t * node) {//tested
	assert(node);
	return node->predecessor;
}

void set_list_node_data(struct list_node_t * node, void * new_data) {//tested
	assert(node);
    assert(new_data);
	node->data = new_data;
}

void set_successor(struct list_node_t * node, struct list_node_t * new_successor) {//tested
	assert(node);
	node->successor = new_successor;
}

void set_predecessor(struct list_node_t * node, struct list_node_t * new_predecessor) {//tested
	assert(node);
	node->predecessor = new_predecessor;
}

/********************************************************************
 * list_t (Double-Linked List)
 ********************************************************************/

struct list_t * new_list() {
	struct list_t *new_list = (struct list_t *)malloc(sizeof(struct list_t));
	new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    return new_list;
}

int list_is_empty(const struct list_t * L) {//tested
	if(L == NULL)
          return 1;
	if(L->size == 0 || (L->head == NULL && L->tail == NULL))
         return 1;
	else
		return 0;
}

unsigned int get_list_size(const struct list_t * L) {//tested
	assert(L);
	return L->size;
}

struct list_node_t * get_list_head(const struct list_t * L) {//tested
	assert(L);
	return L->head;
}

struct list_node_t * get_list_tail(const struct list_t * L) {//tested
	assert(L);
	return L->tail;
}

void increase_list_size(struct list_t * L) {//tested
	assert(L);
	L->size++;
}

void decrease_list_size(struct list_t * L) {//tested
	assert(L);
	L->size--;
}

void set_list_size(struct list_t * L, unsigned int new_size) {
	assert(L);
	L->size = new_size;
}

void set_list_head(struct list_t * L, struct list_node_t * new_head) {
	assert(L);
	L->head = new_head;
}

void set_list_tail(struct list_t * L, struct list_node_t * new_tail) {
	assert(L);
	L->tail = new_tail;
}

void delete_list(struct list_t * L, void (*freeData)(void *)) {//tested
  	assert(L);
	while(L->head != NULL) {
		struct list_node_t * temp = L->head;
		L->head = L->head->successor;
        if(freeData)
        {
			freeData(temp->data);
       	}
		free(temp);
	}
   	L->tail = NULL;
	free(L);
}

void view_list(const struct list_t * L, void (*viewData)(const void *)) {//tested
	assert(L);
    struct list_node_t * temp = L->head;
	while(temp != NULL) {
    	viewData(temp->data);
        temp = temp->successor;
    }
    free(temp);
}


void list_insert_first(struct list_t * L, void * data) {//tested
	assert(L);
    struct list_node_t * new_node = new_list_node(data);
    printf("\n");
    if(L->head != NULL) {
      print_point(get_list_node_data(get_list_head(L)));
    }
        print_point(get_list_node_data(new_node));

	if(L->head == NULL) {
        printf("hey qu'est ce que je fais la moi ? ");
		increase_list_size(L);
		L->head = new_node;
		L->tail = new_node;
	}else {
		struct list_node_t * temp = L->head;
          //      printf("\n");
		//print_point(get_list_node_data(temp));

		increase_list_size(L);
		set_successor(new_node, temp);
		set_predecessor(temp, new_node);
        set_list_head(L,new_node);
		//print_point(get_list_node_data(get_predecessor(temp)));
		//printf("\n");
	}

}

void list_insert_last(struct list_t * L, void * data) {//tested
	assert(L);
	struct list_node_t * new_node = new_list_node(data);
	if(L->head == NULL) {
        increase_list_size(L);
		L->head = new_node;
		L->tail = new_node;
	}else {
		increase_list_size(L);
		struct list_node_t * temp = L->tail;

		set_successor(temp, new_node);
		set_predecessor(new_node, temp);
        L->tail = new_node;
	}
}

void list_insert_after(struct list_t * L, void * data, struct list_node_t * node) {//tested
	assert(L);
	assert(node);
    struct list_node_t * temp = L->head;
	for(int i = 0; i < get_list_size(L); ++i) {
        if(temp == node) {
        	printf("a");
          	if(temp != L->tail) {
          		printf("a");
            	struct list_node_t * new_node = new_list_node(data);
            	set_successor(new_node, temp->successor);
            	set_successor(temp, new_node);
            	set_predecessor(new_node, temp);
            	set_predecessor(new_node->successor, new_node);
          	}
            else
            {
            	printf("a");
            	struct list_node_t * new_node = new_list_node(data);
            	set_successor(temp, new_node);
            	set_predecessor(new_node, temp);
                set_list_tail(L, new_node);
            }
        }
        temp = temp->successor;
    }
    increase_list_size(L);
}

void * list_remove_first(struct list_t * L) {
	assert(L);
	assert(get_list_head(L) && get_list_tail(L));
	struct list_node_t * temp = L->head;
	L->head = get_successor(L->head);
    decrease_list_size(L);
    return temp;
}

void * list_remove_last(struct list_t * L) {
	assert(L);
	assert(get_list_head(L) && get_list_tail(L));
    struct list_node_t * temp = L->tail;
	L->tail = get_predecessor(L->tail);
	decrease_list_size(L);
    return temp;
}

void * list_remove_node(struct list_t * L, struct list_node_t * node) {
	assert(L);
	assert(get_list_head(L) && get_list_tail(L));
    struct list_node_t * temp = L->head;
	for(int i = 0; i < get_list_size(L); ++i) {
		printf("for\n");
    	if(temp == node) {
        	printf("a\n");
          	if(temp != L->tail) {
          		printf("b\n");
                if(temp == L->head) {
                	printf("c\n");
                  	set_list_head(L, temp->successor);
                    break;
                }
                else {
                	printf("d\n");
        			set_successor(temp->predecessor, temp->successor);
           		 	set_predecessor(temp->successor, temp->predecessor);
                	break;
                }
            }
            else if(temp == L->tail)
            {
            	printf("e\n");
            	if(temp == L->head) {
            		printf("f\n");
            		set_list_head(L, NULL);
                    set_list_tail(L, NULL);
            		break;
            	}
                else {
                	printf("g\n");
            		set_list_tail(L, temp->predecessor);
                	break;
                }
            }
        }
        if(temp != NULL){
        	temp = temp->predecessor;
        }
    }
    decrease_list_size(L);
    return temp;
}

struct list_node_t * list_find_node(struct list_t * L, void * data) {
	assert(L);
	struct list_node_t * temp = L->head;

	for(int i = 0; i < get_list_size(L); ++i) {
		if(temp->data == data) {
             return temp;
        }
        temp = temp->successor;
    }
    return NULL;
}

void list_swap_nodes_data(struct list_node_t * node1, struct list_node_t * node2) {
	assert(node1);
	assert(node2);
	void * temp = node1->data;
	set_list_node_data(node1, node2->data);
    set_list_node_data(node2, temp);
}