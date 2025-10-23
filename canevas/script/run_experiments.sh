#!/bin/bash

# OBJECTIF :
#
# Réaliser une campagne expérimentale sur une collection des instances.

# PARAMÈTRES DU SCRIPT :
#
# $1 nom du répertoire contenant la collection des instances
# $2 nom du fichier avec les résultats


# GESTION DE PARAMÈTRES
#
# Testez si le nombre de paramètres est correct et affichez un message dans
# le cas contraire.
# Assurez vous que le premier paramètre correspond bien à un répertoire.

if [ $# -ne 2 ]; then
    echo "Nombre incorrect de paramètres"
    exit 0
fi
if [ ! -d "$1" ]; then
    echo "Le premier paramètre n'est pas un répertoire"
    exit 0
fi

# Affectation des paramètres aux variables.

repertoire=$1
out_filename=$2


# EXÉCUTION DE L'EXPÉRIENCE
#
# Pour toutes les instances de la collection, exécutez le programme "./expe".
# Il faut choisir comme sommet de depart le premier sommet du graphe
# (2eme ligne de chaque fichier).
# Pour chaque exécution, vous devez écrire une ligne dans le fichier
# $out_filename avec 6 colonnes :
# Colonne 1 : nombre des sommets du graphe
# Colonne 2 : paramètre concernant la densité du graphe
# Colonne 3 : numéro de l'instance avec ces paramètres
# Colonnes 4-5 : résultats obtenus par l'exécution du programme "./expe"
# Les colonnes sont séparées par une espace.

