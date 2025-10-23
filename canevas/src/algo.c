#include "algo.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "util.h"
#include "rational.h"
#include "list.h"
#include "tree.h"
#include "geometry.h"

// ATTENTION : Pour simplifier vos algorithmes, pensez à comparer les deux points
// de chaque segment à l'aide de la fonction point_precedes et enregistrer le point
// qui précède sur le membre endpoint1 de la struct Segment, tandis que l'autre
// point sur le membre endpoint2 de la struct Segment.

struct list_t * load_segments(const char * input_filename) {
	FILE *file;
    char buffer[256];
    file = fopen( input_filename, "r");
    if (file == NULL) {
    	perror("Error opening file");
  		return NULL;
    }

    struct list_t * L = new_list();
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
		struct Rational * r = malloc(sizeof(struct Rational));
		int counter = 0;
      	for(int i = 0; i < strlen(buffer); i++)
    	{
			struct Point * p = malloc(sizeof(struct Point));
            set_x(p, *r);
          	/*if(!isdigit(buffer[i]) && buffer[i] != '/' && buffer[i] != ',' && buffer[i] != ' ' && buffer[i] != '\n')
			{
                printf("valeure non souhaité dans le fichier");
             	exit(1);
            }*/

            if(buffer[i] == '/')
        	{

                for(int j = i; j >= 0; j--) {
            		if(j - 1 == -1 || buffer[j - 1] == ' ' || buffer[j - 1] == ',') {
                    	char temp[256];
                    	for(int k = j; k < i; k++) {
                    		temp[k-j] = buffer[k];
                    	}
                        temp[i-j] = '\0';
                    	char *endptr;
                    	long long val = strtoll(temp, &endptr, 10);
                        set_numerator(r, val);
						break;
                    }
                }
            	for(int j = i; j < strlen(buffer); j++)
                {
            		if(j+1 == strlen(buffer) || buffer[j + 1] == ' ' || buffer[j + 1] == ',' || buffer[j + 1] == '\n') {
                    	char temp2[256] = {};
                    	for(int k = i+1; k <= j; k++) {
                    		temp2[k-1-i] = buffer[k];
                    	}
            			temp2[i-j] = '\0';
                    	char *endptr;
                    	long long val = strtoll(temp2, &endptr, 10);
                    	set_denominator(r, val);
                        printf("%lld", val);
                    	break;
                    }
                }
				if(counter == 0) {
                    //printf("\n");
                   	//print_rational(*r);
                    //printf("\n");
                	set_x(p, *r);
                    counter = 1;
				} else {
                    set_y(p, *r);
                    counter = 0;


                    struct list_node_t * current = get_list_head(L);

                   	/*while (current != NULL) {
						//struct Segment * s = new_segment(get_list_node_data(current), get_list_node_data(get_successor(current)));
        				//list_insert_last(retour, s);
        				print_point(get_list_node_data(current));
        				//print_point(get_list_node_data(get_successor(current)));
        				current = get_successor(current);
                    	list_insert_last(L, p);
                    	printf("\n");
                    	print_point(get_list_node_data(get_list_tail(L)));

						printf("\n");
   					}*/
                    //printf("egfi");
                    list_insert_last(L, p);
                    //printf("egggfi");
				}
            }
      	}
    }
    struct list_t * retour = new_list();
    struct list_node_t * current = get_list_head(L);
    printf("\n%d\n", get_list_size(L));
    while (current != NULL) {
		struct Segment * s = new_segment(get_list_node_data(current), get_list_node_data(get_successor(current)));
        list_insert_last(retour, s);
        //print_point(get_list_node_data(current));
        //print_point(get_list_node_data(get_successor(current)));
        current = get_successor(get_successor(current));
    }
	fclose(file);
    return retour;
}

/*void save_intersections(const char * output_filename, const struct list_t * intersections) {
	FILE *file;
	file = fopen( output_filename, "a+");
	if (file == NULL) {
		perror("Error opening file");
	}
    else {
    	struct liste_node_t * current = get_list_head(intersections);
    	while(current != NULL) {
    		long long a = get_numerator(get_x(get_list_node_data(current)));
    		long long b = get_denominator(get_x(get_list_node_data(current)));
    		long long c = get_numerator(get_y(get_list_node_data(current)));
    		long long d = get_denominator(get_y(get_list_node_data(current)));
    		fprintf(file, "%lld/%lld,%lld/%lld\n",a,b,c,d);
    		current = get_successor(current);
    	}
    }
	fclose(file);
}*/
void save_intersections(const char * output_filename, const struct list_t * intersections) {
	FILE * fichier = fopen(output_filename, "w");

	if (fichier == NULL){
		printf("Impossible de charger fichier\n");
	}
	else {
		struct list_node_t * aux = get_list_head(intersections);
		while (aux) {
			struct Point *p = get_list_node_data(aux);
			struct Rational x = get_x(p);
			struct Rational y = get_y(p);

			fprintf(fichier,"%lld/%lld,%lld/%lld \n", get_numerator(x),get_denominator(x),get_numerator(y),get_denominator(y));
			aux = get_successor(aux);
		}
	}
	fclose(fichier);
}


struct list_t * all_pairs(const struct list_t * segments) {
	assert(segments);
	struct Segment *seg1 = get_list_node_data(get_list_head(segments));
	struct list_t * L = new_list();
	for (int i = 0;i < segments->size;i++) {
		struct Segment *seg2 = get_successor(list_find_node(segments,seg1));
		for (int j = i+1; j < segments->size; j++) {
			struct Point *pt_inter= get_intersection_point(seg1,seg2);
			if(pt_inter) {
				list_insert_last(L,pt_inter);
			}
			seg2 = get_successor(list_find_node(segments,seg2));
		}
		seg1 = get_successor(list_find_node(segments,seg1));
	}
	return L;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////// ALGORITHME DE Bentley-Ottmann ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct Event * new_event(int type, struct Point * event_point, struct Segment * s1, struct Segment * s2) {
	struct Event * e = malloc(sizeof(struct Event));
    if(type == 1 || type == 2) {
      	if(s1->endpoint1 == event_point || s1->endpoint2 == event_point) {
    		e->type = type;
    		e->event_point = event_point;
    		e->s1 = s1;
	        e->s2 = NULL;
        }
        else {
        	fprintf(stderr,"erreure pendant la création du event1");
        }
    }
    /*else if(get_intersection_point(s2, s1) != event_point) {
    	fprintf(stderr,"erreure pendant la création du event2");
    }*/
    else
    {
	    e->type = type;
	    e->event_point = event_point;
	    e->s1 = s1;
	    e->s2 = s2;
    }
    return e;
}

int get_event_type(const struct Event * event) {
	assert(event);
	return event->type;
}

struct Point * get_event_point(const struct Event * event) {
	assert(event);
	return event->event_point;
}

struct Segment * get_event_segment1(const struct Event * event) {
	assert(event);
	return event->s1;
}

struct Segment * get_event_segment2(const struct Event * event) {
	assert(event);
	return event->s2;
}

/**
 * Initialise la structure des événements (arbre binaire de recherche) avec les
 * événements connus d'avance (début et fin des segments de la liste
 * \p segments ).
 * Pour rappel, l'arbre est ordonné selon l'ordre impliqué par la fonction
 * point_precedes.
 *
 * @param[in] segments
 * @return l'arbre des événements connus d'avance
 */
struct tree_t * initialize_events(const struct list_t * segments) {
	assert(segments);
	struct tree_t * retour = new_tree();
	struct list_node_t * curr = get_list_head(segments);
    struct Segment * currentSegment = get_list_node_data(get_list_head(segments));
    struct Point * currentPoint = get_endpoint1(currentSegment);
    struct Event * currentEvent = new_event(1, currentPoint, currentSegment, NULL);
	int i = 1;
	while(currentEvent != NULL) {
        i++;
        if(i%2 == 0) {
          if(get_endpoint1(currentSegment) == NULL) {
          }
          if(get_endpoint2(currentSegment) == NULL) {
            print_point(get_endpoint1(currentSegment));
          }
			if(point_precedes(get_endpoint1(currentSegment), get_endpoint2(currentSegment))) {
        		currentPoint = get_endpoint1(currentSegment);
          		currentEvent = new_event(1, currentPoint, currentSegment, currentSegment);
          		tree_insert(retour, currentPoint, currentEvent, point_precedes);
        	}
        	else {
        		currentPoint = get_endpoint2(currentSegment);
            	currentEvent = new_event(1, currentPoint, currentSegment, currentSegment);
          		tree_insert(retour, currentPoint, currentEvent, point_precedes);
        	}
        }
        else {
			if(point_precedes(get_endpoint1(currentSegment), get_endpoint2(currentSegment))) {
        		currentPoint = get_endpoint2(currentSegment);
          		currentEvent = new_event(2, currentPoint, currentSegment, currentSegment);
          		tree_insert(retour, currentPoint, currentEvent, point_precedes);
        	}
        	else {
        		currentPoint = get_endpoint1(currentSegment);
            	currentEvent = new_event(2, currentPoint, currentSegment, currentSegment);
          		tree_insert(retour, currentPoint, currentEvent, point_precedes);
        	}
            curr = get_successor(curr);
            if(curr == NULL) {
            	break;
            }
            currentSegment = get_list_node_data(curr);
        }
	}
    return retour;
}

/**
 * Vérifie si les segments \p s1 et \p s2 s'intersectent après la position de
 * l'événement \p event . Si oui et si cette intersection n'est pas détectée
 * auparavant, elle sera ajoutée dans la structure des événements (\p events ),
 * ainsi que dans la liste des intersections (\p intersections ).
 *
 * @param[in] s1
 * @param[in] s2
 * @param[in] event
 * @param[in,out] events
 * @param[in,out] intersections
 */
void test_and_set_intersection(struct Segment * s1, struct Segment * s2, struct Event * event, struct tree_t * events, struct list_t * intersections) {
	assert(s1);
	assert(s2);
	assert(event);
	assert(events);
	assert(intersections);
	struct Point * intersection = get_intersection_point(s1, s2);
    if(intersection != NULL) {
    	int deja = 0;
        struct list_node_t * curr = get_list_head(intersections);
        //printf("%d\n", get_list_size(intersections));
    	for(int i = 0;i < intersections->size;i++) {
			//print_point(get_list_node_data(curr));
            //print_point(intersection);
        	if(eq(get_x(get_list_node_data(curr)), get_x(intersection)) && eq(get_y(get_list_node_data(curr)), get_y(intersection))) {

                deja = 1;
            }
            curr = get_successor(curr);
    	}
        if(deja == 1) {
        	return;
        }
        struct Event * intersectionEvent = new_event(3, intersection, s1, s2);
        //tree_insert(events, interesctionEvent, intersection, point_precedes);
        tree_insert(events, intersection, intersectionEvent, point_precedes);
        list_insert_last(intersections, intersection);
    }
    else
        printf("aucune intérsection\n");
}



static void comparer_list(struct Event * event, struct list_t * status, struct tree_t * events, struct list_t * intersections) {
	struct list_node_t * currentNode = get_list_head(status);
    if(currentNode != NULL) {
    	struct list_node_t * nextNode = get_successor(currentNode);
  		for(int i = 1; i < get_list_size(status); i++) {
			test_and_set_intersection(get_list_node_data(currentNode), get_list_node_data(nextNode), event, events, intersections);
        	currentNode = nextNode;
        	nextNode = get_successor(currentNode);
		}
    }
}
/**
 * Gestion de l'événement \p event de type intersection.
 * L'état de la ligne de balayage décrite par la liste \p status doit être mis
 * à jour. La structure des événements (\p events ) ainsi que la liste des
 * intersections (\p intersections ) seront éventuellement mises à jour si une
 * nouvelle intersection sera détectée (utilisez la procedure
 * test_and_set_intersection).
 *
 * @param[in] event
 * @param[in,out] status
 * @param[in,out] events
 * @param[in,out] intersections
 */
static void handle_intersection_event(struct Event * event, struct list_t * status, struct tree_t * events, struct list_t * intersections) {
	assert(event);
	assert(status);
	assert(events);
	assert(intersections);
    struct Segment * seg1 = get_event_segment1(event);
    struct Segment * seg2 = get_event_segment2(event);
    list_swap_nodes_data(list_find_node(status, seg1), list_find_node(status, seg2));
    comparer_list(event, status, events, intersections);
}

/**
 * Gestion de l'événement \p event de type begin.
 * L'état de la ligne de balayage décrite par la liste \p status doit être mis
 * à jour. La structure des événements (\p events ) ainsi que la liste des
 * intersections (\p intersections ) seront éventuellement mises à jour si une
 * nouvelle intersection sera détectée (utilisez la procedure
 * test_and_set_intersection).
 *
 * @param[in] event
 * @param[in,out] status
 * @param[in,out] events
 * @param[in,out] intersections
 */
static void handle_begin_event(struct Event * event, struct list_t * status, struct tree_t * events, struct list_t * intersections) {
	assert(event);
	assert(status);
	assert(events);
	assert(intersections);
    struct list_node_t * currentNode = get_list_head(status);
    struct Rational x = get_x(get_event_point(event));
    for(int i = 0; i < get_list_size(status); i++) {
      	if(currentNode == NULL) {
        	//printf("yop");
          	list_insert_first(status, get_event_segment1(event));
            comparer_list(event, status, events, intersections);
            return;
      	}
        else{
		struct Rational currentSegmentP1X = get_x(get_endpoint1(get_list_node_data(currentNode))); //coordonnées x des extrémités du segment
       	struct Rational currentSegmentP2X = get_x(get_endpoint2(get_list_node_data(currentNode)));
        struct Rational partieDejaAvance = valeur_abs(rsub(currentSegmentP1X, x)); //taille de la diff entre le point actuel et le premier point du segment en X
        struct Rational distanceCurrentSegmentX = valeur_abs(rsub(currentSegmentP2X, currentSegmentP1X)); // taille du segment en X
        struct Rational pourcentage = rdiv(partieDejaAvance, distanceCurrentSegmentX); // pourcentage du segment que l'on a deja parcourus
        struct Rational currentSegmentP1Y = get_y(get_endpoint1(get_list_node_data(currentNode))); //coordonnées y des extrémités du segment
       	struct Rational currentSegmentP2Y = get_y(get_endpoint2(get_list_node_data(currentNode)));
        struct Rational distanceCurrentSegmentY = valeur_abs(rsub(currentSegmentP2Y, currentSegmentP1Y)); //taille du segment en Y
        struct Rational currentYcoordonnee = rmul(distanceCurrentSegmentY, pourcentage); //coordonée Y du point actuel
    	if(gt(currentYcoordonnee, x)){
        	currentNode = get_successor(currentNode);
        }
        else if(eq(currentYcoordonnee,x)){
        	struct Rational segmentY1 = get_y(get_endpoint1(get_event_segment1(event)));
            struct Rational segmentY2 = get_y(get_endpoint2(get_event_segment1(event)));
            struct Rational segmentX1 = get_x(get_endpoint1(get_event_segment1(event)));
            struct Rational segmentX2 = get_x(get_endpoint2(get_event_segment1(event)));
            struct Rational distanceSegmentY = valeur_abs(rsub(segmentY1, segmentY2));
            struct Rational distanceSegmentX = valeur_abs(rsub(segmentX1, segmentX2));
        	 if(gt(rdiv(distanceCurrentSegmentY,distanceCurrentSegmentX), rdiv(distanceSegmentY,distanceSegmentX))) {
             	currentNode = get_successor(currentNode);
        	 }
             else {
                //printf("yAp");
                list_insert_after(status, get_event_segment1(event), currentNode);
             	comparer_list(event, status, events, intersections);
                return;
             }
        }
        else {
        	//printf("yup\n");
          	list_insert_after(status, get_event_segment1(event), currentNode);
            //printf("yuuup\n");
            comparer_list(event, status, events, intersections);
            //printf("yuuuu\n");
            return;
        }

        }
    }


	list_insert_first(status, get_event_segment1(event));
    comparer_list(event, status, events, intersections);
}

/**
 * Gestion de l'événement \p event de type end.
 * L'état de la ligne de balayage décrite par la liste \p status doit être mis
 * à jour. La structure des événements (\p events ) ainsi que la liste des
 * intersections (\p intersections ) seront éventuellement mises à jour si une
 * nouvelle intersection sera détectée (utilisez la procedure
 * test_and_set_intersection).
 *
 * @param[in] event
 * @param[in,out] status
 * @param[in,out] events
 * @param[in,out] intersections
 */
static void handle_end_event(struct Event * event, struct list_t * status, struct tree_t * events, struct list_t * intersections) {
	assert(event);
	assert(status);
	assert(events);
	assert(intersections);
    struct list_node_t * founded_node = list_find_node(status, get_event_segment1(event));
    if(founded_node != NULL) {
    	list_remove_node(status, founded_node);
		printf("in");
    }
	else{
    	//printf("out");
        //print_segment(get_event_segment1(event));
    }
}



struct list_t * BentleyOttmann(const struct list_t * segments) {
	assert(segments);
	struct list_t * intersections = new_list(); //liste de retour contient toutes les intérsections trouvés entre les segments
    struct tree_t * events = initialize_events(segments); //arbres des évenement, initialisé avec les évenement connus
    struct tree_node_t * curr = tree_find_min(get_root(events)); // évenement actuel de la pile d'évenements
    struct list_t * status = new_list(); // liste des segments actuels
    while(curr != NULL){
      	struct Event * currEvent = get_tree_node_data(curr);
    	//print_point(get_event_point(currEvent));
        switch(currEvent->type){
          	case 1:
                printf("1\n");
                handle_begin_event(currEvent, status, events, intersections);
                break;
          	case 2:

				printf("case2\n");
                handle_end_event(currEvent, status, events, intersections);
                break;
            case 3:
            	printf("3\n");
              	handle_intersection_event(currEvent, status, events, intersections);
            	break;
    	}
        struct list_node_t * currNode = get_list_tail(status);
		printf("%d\n", get_list_size(intersections));
        /*for(int i = 0; i < get_list_size(status); i++){
        	print_segment(get_list_node_data(currNode));
            currNode = get_successor(currNode);
        }*/
        curr = tree_find_successor(get_root(events), get_event_point(get_tree_node_data(curr)), point_precedes);
    }
    return intersections;
}
