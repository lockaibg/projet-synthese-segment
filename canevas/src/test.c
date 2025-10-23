#include <assert.h>
 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "rational.h"
#include "list.h"
#include "tree.h"
#include "geometry.h"
#include "algo.h"

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////// TEST LIST /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static int compare_lists(struct list_t * l1, void * l2[], unsigned int size) {
  	if (get_list_size(l1) != size)
    {
		return 0;
	}
	if (get_list_size(l1) != 0 && get_list_head(l1) == NULL)
    {
        return 0;
	}
	if (get_list_head(l1) == NULL)
		return 1;

	struct list_node_t * curr = get_list_head(l1);
	unsigned int i = 0;
	while (curr != NULL && l2) {
		if (get_list_node_data(curr) != l2[i])
        {
            return 0;
        }
		curr = get_successor(curr);
		i++;
	}

	curr = get_list_tail(l1);
	i = size-1;
	while (curr != NULL && l2) {
		if (get_list_node_data(curr) != l2[i])
        {
			return 0;
        }
		curr = get_predecessor(curr);
		i--;
	}
	return 1;
}

void test_list_insert_last() {
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;

	struct list_t * L = new_list();
	int * tab[3];
	tab[0] = i1; tab[1] = i2; tab[2] = i3;

	list_insert_last(L, i1);
	if (compare_lists(L, (void*) tab, 1) == 0) printf("probleme21");
	list_insert_last(L, i2);
	if (compare_lists(L, (void*) tab, 2) == 0) printf("probleme23");
	list_insert_last(L, i3);
	if (compare_lists(L, (void*) tab, 3) == 0) printf("probleme22");
}


void test_list_insert_first() {
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;

	struct list_t * L = new_list();
	int * tab[3];
	tab[0] = i1;

	list_insert_first(L, i1);
	if (compare_lists(L, (void*) tab, 1) == 0) printf("probleme1");
	list_insert_first(L, i2);
	tab[0] = i2; tab[1] = i1;
	if (compare_lists(L, (void*) tab, 2) == 0) printf("probleme2");
	list_insert_first(L, i3);
	tab[2] = i1; tab[1] = i2; tab[0] = i3;
	if (compare_lists(L, (void*) tab, 3) == 0) printf("probleme3");
}

void test_list_is_empty()
{
  	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;

	struct list_t * L = new_list();
    list_insert_first(L, i1);list_insert_first(L, i2);list_insert_first(L, i3);
    if(list_is_empty(L) == 1) printf("probleme15");
	delete_list(L, freeInt);
    if(list_is_empty(L) == 0) printf("probleme25");
}
void test_setter()
{
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;

	struct list_t * L = new_list();
	struct list_t * L1 = new_list();
    list_insert_first(L, i1);list_insert_first(L, i2);list_insert_first(L, i3);
	struct list_node_t * curr;
	struct list_node_t * temp = malloc(sizeof(struct list_node_t));
    for(int i = 0; i < get_list_size(L); i++)
    {
        if(i == 0)
        {
         	curr = get_list_head(L);
        }
        else
        {
         	curr = get_successor(curr);
        }
	    set_successor(temp, get_successor(curr));
	    set_predecessor(temp, get_predecessor(curr));
	    set_list_node_data(temp, get_list_node_data(curr));
        list_insert_first(L1, temp);
    }
    if(get_list_head(L) == get_list_head(L1) || get_successor(get_list_head(L)) == get_successor(get_list_head(L1)) || get_list_tail(L) == get_list_tail(L1))
    {
     	printf("probleme10000");
    }
}

void test_fichiers()
{
 	struct list_t * L = new_list();
	L = load_segments("liste_segments.txt");
    struct Rational r = get_x(get_list_node_data(get_successor(get_list_head(L))));
	struct Rational rationel = get_y(get_list_node_data(get_list_tail(L)));
    long long x = get_denominator(r);
    long long y = get_numerator(r);
	long long x1 = get_denominator(rationel);
    long long y1 = get_numerator(rationel);
    printf("%lld\n%lld\n", x, y);
	printf("%lld\n%lld", x1, y1);
	save_intersections("result.txt", L);
}
void test_list()
{
	int * i1 = malloc(sizeof(int));

	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;

	struct list_t * L = new_list();
	struct list_t * L1 = new_list();
	list_insert_first(L1, i1);list_insert_first(L1, i2);list_insert_after(L1, i3, L1->head->successor);
	list_insert_first(L, i1); list_insert_first(L, i2); list_insert_last(L, i3);

	test_list_insert_last();

	test_list_insert_first();
	test_list_is_empty();
	view_list(L, viewInt);
	view_list(L1, viewInt);
	test_setter();

    test_list_insert_first();
    test_list_is_empty();
    view_list(L, viewInt);
    view_list(L1, viewInt);
	test_setter();
}
void test_rational_geometry()
{
	struct Rational *r1= malloc(sizeof(struct Rational));
	set_denominator(r1,1);
	set_numerator(r1,1);
	struct Rational *r2= malloc(sizeof(struct Rational));
	set_denominator(r2,1);
	set_numerator(r2,3);


	struct Point *p1 =new_point(*r1,*r2);
	print_point(p1);
	struct Rational *r3 =malloc(sizeof(struct Rational));
	set_denominator(r3,1);
	set_numerator(r3,3);
	struct Rational* r4 = malloc(sizeof(struct Rational));
	set_denominator(r4,1);
	set_numerator(r4,1);

	struct Point *p2 =new_point(*r3,*r4);
	print_point(p2);
	struct Segment * seg1 =new_segment(p1,p2);

	struct Rational *r5= malloc(sizeof(struct Rational));
	set_denominator(r5,1);
	set_numerator(r5,1);
	struct Rational *r6= malloc(sizeof(struct Rational));
	set_denominator(r6,1);
	set_numerator(r6,1);

	struct Point *p3 =new_point(*r5,*r6);
	print_point(p3);
	struct Rational *r7= malloc(sizeof(struct Rational));
	set_denominator(r7,1);
	set_numerator(r7,3);
	struct Rational *r8 =  malloc(sizeof(struct Rational));
	set_denominator(r8,1);
	set_numerator(r8,3);

	struct Point *p4 =new_point(*r7,*r8);
	print_point(p4);
	struct Segment * seg2 =new_segment(p3,p4);

	print_segment(seg1);
	print_segment(seg2);
	struct Point *inter = get_intersection_point(seg1,seg2);
	print_point(inter);
	printf("d");

	struct Point *p =new_point(*r1,*r2);
	print_point(p);
}

void test_tree() {
	struct tree_t *tree = new_tree();
	assert(tree_is_empty(tree));

	/**struct Rational *r1= malloc(sizeof(struct Rational));
	set_denominator(r1,1);
	set_numerator(r1,5);
	struct Point *p1 =new_point(*r1,*r1);
	set_numerator(r1,3);
	tree_insert(tree,p1 ,"a",point_precedes);
	struct Rational *r2 =malloc(sizeof(struct Rational));
	set_denominator(r2,1);
	set_numerator(r2,5);
	struct Point *p2 =new_point(*r2,*r1);
	tree_insert(tree,p2 ,"a",point_precedes);
	set_numerator(r2,1);
	struct Point *p3 =new_point(*r2,*r1);
	tree_insert(tree,p3 ,"a",point_precedes);
	assert(tree->root->left->left !=NULL);
	assert(get_tree_size(tree)==3);**/
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	int * i4 = malloc(sizeof(int));

	*i1 = 1;
	*i2 = 2;
	*i3 = 3;
	*i4 = 4;
	tree_insert(tree,i4,i4,intSmallerThan);
	tree_insert(tree,i3,i3,intSmallerThan);
	tree_insert(tree,i1,i1,intSmallerThan);
	tree_insert(tree,i2,i2,intSmallerThan);
	printf("Affichage de l'arbre : ");
	view_tree(tree,viewInt,viewInt);
	printf("\n");
	assert(tree->root->left->left->right !=NULL);
	tree_remove(tree,i3,intSmallerThan);
	printf("Affichage de l'arbre apres la suppression : ");
	view_tree(tree,viewInt,viewInt);
	assert(get_tree_size(tree)==3);
	//delete_tree(tree,freeInt,freeInt);
}
void test_algo(){
	struct Rational * r1 = malloc(sizeof(struct Rational));
	struct Rational * r2 = malloc(sizeof(struct Rational));
	struct Rational * r3 = malloc(sizeof(struct Rational));
	struct Rational * r4 = malloc(sizeof(struct Rational));
	set_denominator(r1,1);
	set_denominator(r2,1);
	set_denominator(r3,1);
	set_denominator(r4,1);
	set_numerator(r1,1);
	set_numerator(r2,2);
	set_numerator(r3,3);
	set_numerator(r4,4);
	struct Point *p1 = new_point(*r1,*r2);
	struct Point *p2 = new_point(*r3,*r4);
	struct Point *p3 = new_point(*r3,*r2);
	struct Point *p4 = new_point(*r1,*r4);

	struct Segment * s1 = new_segment(p1,p2);
	struct Segment * s2 = new_segment(p3,p4);
	struct list_t * segments = new_list();
	list_insert_last(segments,s1);
	list_insert_last(segments,s2);


    //test initialize_events
	struct tree_t * events = initialize_events(segments);
	if(events->root!=NULL){
	if(get_event_point(get_tree_node_data(tree_find_min(events->root))) == p4){
		if(get_event_point(get_tree_node_data(tree_find_max(events->root))) == p3){
		}
		else {
			fprintf(stderr, "maximum de l'arbre invalide\n");
			return;
		}
	}else {
		fprintf(stderr, "minimum de l'arbre invalide\n");
		return;
	}
	if(get_event_point(get_tree_node_data(tree_find_successor(events->root, p4, point_precedes))) == p1){
		if(get_event_point(get_tree_node_data(tree_find_successor(events->root, p1, point_precedes))) == p2){
		}
		else {
            print_point(tree_find_successor(events->root, p1, point_precedes));
            print_point(p2);
			fprintf(stderr, "3e noeud de l'arbre invalide");
			return;
		}
	}
	else {
		fprintf(stderr, "2e noeud de l'arbre invalide");
		return;
	}
    }


    //test test_and_set_intersection
	struct list_t * intersections = new_list();
    struct tree_t * temp = events;
    struct Event * event = get_tree_node_data(get_root(temp));
	test_and_set_intersection(s1, s2, event, temp, intersections);
    if(get_list_size(intersections) == 1){
    	//printf("gg1");
        //print_point(get_list_node_data(get_list_head(intersections)));
    }
	event = get_tree_node_data(tree_find_successor(get_root(temp), get_tree_node_key(get_root(temp)), point_precedes));
    test_and_set_intersection(s1, s2, event, temp, intersections);
    if(get_list_size(intersections) == 1){
        //printf("gg2");
        //print_point(get_list_node_data(get_list_head(intersections)));
    }

    //test all
	struct list_t * intersections2 = BentleyOttmann(segments);
    if(intersections2 == intersections){
    	printf("gg3");
    }
    else
    {
    	printf("%d\n", get_list_size(intersections2));
		struct list_node_t * curr = get_list_head(intersections2);
        for(int i=0;i<get_list_size(intersections2);i++){
			print_point(get_list_node_data(curr));
            curr = get_successor(curr);
        }
    }


    //test bentley ottman/fichiers
    struct list_t * L = new_list();
	L = load_segments("liste_segments.txt");
	print_segment(get_list_node_data(get_list_head(L)));
    struct list_t * result = BentleyOttmann(L);

    struct list_node_t * currL = get_list_head(L);

   	for(int i=0;i<get_list_size(L);i++){
        printf("\n\n\n");
		print_point(get_list_node_data(currL));
        currL = get_successor(currL);
    }

}
void test_find_successor(){
	struct Rational * r1 = malloc(sizeof(struct Rational));
	struct Rational * r2 = malloc(sizeof(struct Rational));
	struct Rational * r3 = malloc(sizeof(struct Rational));
	struct Rational * r4 = malloc(sizeof(struct Rational));
	set_denominator(r1,1);
	set_denominator(r2,1);
	set_denominator(r3,1);
	set_denominator(r4,1);
	set_numerator(r1,1);
	set_numerator(r2,2);
	set_numerator(r3,3);
	set_numerator(r4,4);
	struct Point *p1 = new_point(*r1,*r2);
	struct Point *p2 = new_point(*r3,*r4);
	struct Point *p3 = new_point(*r2,*r3);
	struct Point *p4 = new_point(*r1,*r4);

	struct tree_t * events = new_tree();
    tree_insert(events, p1, p1, point_precedes);
    tree_insert(events, p2, p2, point_precedes);
    tree_insert(events, p3, p3, point_precedes);
    tree_insert(events, p4, p4, point_precedes);
    if(get_tree_node_data(tree_find_successor(events->root, p1, point_precedes)) == p3){
      	printf("tf ");
    	if(get_tree_node_data(tree_find_successor(events->root, p4, point_precedes)) == p1){
          printf("tf2");
    		if(get_tree_node_data(tree_find_successor(events->root, p3, point_precedes)) == p2){
    	  		printf("good");
    		}
                else {
                  print_point(get_tree_node_data(tree_find_successor(events->root, p3, point_precedes)));
                  printf("bad1");
                }
    	}
        else {
          print_point(get_tree_node_data(tree_find_successor(events->root, p2, point_precedes)));
                  printf("bad2");
                }
    }
    else {
      print_point(get_tree_node_data(tree_find_successor(events->root, p1, point_precedes)));
                  printf("bad3");
                }
}

///////////////////////////////////////////////////////////////////////
//////////////////////////////// MAIN /////////////////////////////////
///////////////////////////////////////////////////////////////////////

int main() {

	test_algo();
    return 0;

}