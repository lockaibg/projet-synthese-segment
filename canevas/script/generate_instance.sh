#!/bin/bash

# OBJECTIF :
#
# Créer un fichier contenant une nouvelle instance du problème,
# c'est-à-dire un ensemble de segments, générée aléatoirement.

# PARAMÈTRES DU SCRIPT :
#
# $1 nom du fichier à créer
# $2 nombre des segments de l'instance
# $3 paramètre utilisé pour définir le spectre des valeurs pour les abscisses et les ordonnées.
#    Il prend des valeurs dans l'intervalle [100,10000]. Alors, les valeurs des abscisses et
#    des ordonnées seront des entiers tirés aléatoirement sur l'intervalle [1, $3].


# FONCTION getNormalDistributionSample
#
# Génère de façon aléatoire un entier dans l'intervalle [min,max]
# selon la distribution normale.
#
# PARAMÈTRES : deux entiers correspondants aux limites de l'intervalle.

getNormalDistributionSample () {
	min=$1
	max=$2
	echo $(( min + (RANDOM % (max-min+1)) ))
}



# GESTION DE PARAMÈTRES
#
# Si :
# - le nombre de paramètres n'est pas correct, ou
# - le fichier indiqué par le paramètre $1 existe déjà, ou
# - le paramètre $3 n'est pas une valeur entière dans l'intervalle [100,10000],
# alors affichez un message d'erreur et arrêtez l'exécution du script.
# S'il n'y a pas d'erreur, affectez les paramètres aux variables globales
# (filename, n, range).

if [ $# -ne 3 ]; then
  echo "Mauvais nombre de parametres" >&2
  exit 1
fi

if [ -e "$1" ]; then
    echo  "Le fichier existe deja"
    exit 1
fi

if [[ $3 -lt 100 || $3 -gt 10000 ]]; then
  echo "Range non valide"
  exit 1
fi

filename="$1"
n=$2
range=$3


# GÉNÉRATION DES SEGMENTS
#
# Dans la suite, cherchez et utilisez la commande "seq".
#
# Créez n segments (paramètre 2 du script). Chaque segment est composé par deux
# points. Écrivez les segments dans le fichier filename (paramètre 1 du script)
# en respectant le format présenté au cours.
#
# Pour créer un point, il faut utiliser la fonction getNormalDistributionSample
# et générer de façon aléatoire deux valeurs entières dans l'intervalle
# [1,range], qui correspondent à son abscisse et à son ordonnée,
# respectivement.
#
# Indication : chaque point créé doit être ajouté dans le tableau Points défini
# ci-dessous. Utilisez le format "abscisse/1,ordonnée/1" (conforme aussi au
# format du fichier à créer) pour stocker le point au tableau Points.
#
# Attention ! Si le point existe déjà dans le tableau, vous devez créer un
# nouveau point. Cela est répété autant de fois que besoin.
#
# Attention ! Si le segment créé est parallèle à l'axe des ordonnées, vous
# devez créer un nouveau segment. Cela est répété autant de fois que besoin.
count=0
tab_valeurs=()
tab_segments=()
for (( i = 0; i < $((n * 2)); i++ )); do
  tab_valeurs[i]=0

done
for ((i = 0;i < n;i++)); do
  while true; do
    for (( j = 0; j < 2; j++ )); do
      while true; do #permet de simuler un do-while
        temp=$(getNormalDistributionSample 1 "$range")
        temp2=$(getNormalDistributionSample 1 "$range")
        pt="$temp/1,$temp2/1"
        trouve=false
        for points in "${tab_valeurs[@]}"; do #verifie que le point n'existe pas deja
          if [ "$points" == "$pt" ]; then
              trouve=true
              break
          fi
        done

        if [ "$trouve" == false ]; then #si le point n'existe pas deja, alors sort de la boucle
           break
        fi

      done
      tab_valeurs[count]=$pt #ajout du point au tableau
      count=$((count + 1))

    done
      v1=$((count - 1))
      v2=$((count - 2))
      x1=$(echo "${tab_valeurs[v1]}" | cut -d'/' -f1)  #permet de recupérer les 2 valeurs de x
      x2=$(echo "${tab_valeurs[v2]}" | cut -d'/' -f1)  # des deux points du segments pour voir si il est parallele a l'axe des ordonnées

      if [ "$x1" == "$x2" ]; then
        count=$((count-2)) #si parallèle, alors re-boucle et remet le count au debut de la boucle
      else
        break
      fi

  done
done

count=0
for (( i = 0; i < 2*n; i+2 )); do
    pt1=${tab_valeurs[i]}
    k=$((i+1))
    pt2=${tab_valeurs[k]}
    tab_segments[count]="$pt1 $pt2"
    count=$((count+1))
done

for points in "${tab_segments[@]}"; do # ecrit tous les segments dans le fichier
      echo "$points" >> "$filename"
done