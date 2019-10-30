#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "kotok.h"




void addPlayer(p** t, int id, float score, int* n) {
    // n = nbrPlayer
    
    p* player = pCreator(id, score);
    
    if (*n < 5) {
        t[*n] = player; // si tu travailles avec des p** tu ne peux pas realiser une assignation comme ceci (un tableau est un pointeur n'est pas un tableau)
			// plutot realiser une assignation du genre *(t+n-1)=player c'est le soucis de pas travailler avec un tableau c'est que la syntaxe est differente
        		// mais en francais ca veut dire que la valeur assigné a l'addresse t+n-1 vaut l'addresse que p possede et donc **(t+n-1) vaut la valeur de player
    } else {
        t = realloc(t,(*n + 1)*sizeof(p));
        t[*n] = player; //meme remarque du coup il faut bien saisir le principe du pointeur
    }
}


p* pCreator(int id, float score) {
    p* player = malloc(sizeof(p));
    int wins = score/1;
    player->wins = wins; // assignement de l'adressage
    player->score = score;
    player->id = id;
    return player;
}


void setResult(int id1, int id2, float score1, p** t, int* n) {
    //n = nbrPlayers
    short int bool1 = 0;
    short int bool2 = 0;
    
    //Check dans le tableau si on a déja vu les joueurs
    for (int i=1; i <= *n ; i++) {// il y avait une , a la place du deuxieme ; je suppose que tu voulais ecrire i++;
        
        if (t[i]->id == id1) {
            t[i]->id += id1;
            t[i]->score += score1;
            t[i]->wins += score1/1;
            bool1=1;
        } else if (t[i]->id == id2) {
            //score2 = 1-score1
            t[i]->id += id2;
            t[i]->score += 1-score1;
            t[i]->wins += (1-score1)/1;
            bool2=1;
        }
        
    }
    
    //Ajoute les joueurs manquants
    if (!bool1) {
        addPlayer(t, id1, score1, n);
        (*n)++;
    }
    
    if (!bool2) {
        addPlayer(t, id2, 1-score1, n);
        (*n)++;
    }
    
    
    
}


int main(int argc, char* argv[]) {
    // Assignement des variables
    p** t = malloc(5*sizeof(p*));
    for(int i = 0; i < 5; i++)
        t[i] = pCreator(-1,-1);
    int id1=0,id2=0;
    float score1=0;
    int nbrPlayers=0;
    int i=0;
    
    
    //OUVERTURE f_matches
    FILE* f_matches = fopen(argv[1], "r");
    
    rewind(f_matches); //Remets le pointeur au début
    
    
    //Check jusqu'a trouver un bout de ligne
    while (fscanf(f_matches,"%d %d %f", &id1, &id2, &score1) != EOF)  {
        setResult(id1, id2, score1, t, &nbrPlayers);
    }
    
    //FERMETURE f_matches
    fclose(f_matches);
    
    FILE* f_result = fopen(argv[2], "w");
    
    for (i=0; i<nbrPlayers; i++) {
        fprintf(f_result, "%d %.1f %d \n", t[i]->id, t[i]->score, t[i]->wins);
    }
    
    /////////
    
    fclose(f_result);
    //Passage de suppression de la mémoire tas
    for (i=0; i < nbrPlayers; i++) {
        free(t[i]);
    }
    
    free(t);
    
}

