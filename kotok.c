#include<stdio.h>
#include<stdlib.h>

struct player {
    int id;
    float score;
    int wins;
} p;


char* writer(int id, float score, int wins) {
    char* str;
    
    strcpy(str, (char) (id));
    strcat(str, ' ');
    strcat(str, (char) (score));
    strcat(str, ' ');
    strcat(str, (char) (wins));
    strcat(str, '/n');
    
    return str;
}


void addScore(int id, float score) {
    char* c = NULL;
    char* str = NULL;
    int old_id = 0;
    float old_score = 0;
    int old_wins = 0;
    int wins=score/1;
    
    FILE* f_result = fopen("./result", "wr");
    
    rewind(f_result);
    
    
    //Pas sur que ce soit ok
    while (*fgets(c,5,f_result) != id && c != EOF) {
        //Laisse c != EOF ?
        fseek(f_result, sizeof(c)/sizeof(c[0]), SEEK_CUR);
        // Peut mettre : sizeof(c)/sizeof(char) ??
        // sizeof(c)/sizeof(c[0])  permet d'avoir la taille de c
    }
    
    fscanf(f_result,"%d %f %d", &old_id, &old_score, &old_wins);
    
    str = writer(id+old_id, score+old_score, wins+old_wins);
    fwrite(str , 1 , sizeof(str) , f_result);
    
    fclose(f_result);
}


void addPlayer(p** t, int id, float score, int* n) {
    int wins = score/1;
    char* str = writer(id, score, wins);
    // n = nbrPlayer
    
    p* player = pCreator(id, score);
    
    if (sizeof(t) < *n * sizeof(p)) {
        t[n] = player;
        
    } else {
        t = realloc(t,(*n * 2)*sizeof(p));
        t[n] = player;
    }
    
    
    FILE* f_result = fopen("./result", "a");
    
    fwrite(str , 1 , sizeof(str) , f_result);
    
    fclose(f_result);
    
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
    for (int i=1; i <= *n, i=i+1) {
        
        
        if (t[i]->id == id1) {
            addScore(id1, score1);
            bool1=1;
        } else if (t[i]->id == id2) {
            addScore(id2, 1-score1); //score2 = 1-score1
            bool2=1;
        }
        
    }
    
    //Ajoute les joueurs manquants
    if (!bool1) {
        addPlayer(id1, score1, &n);
        *n=*n+1;
    }
    
    if (!bool2) {
        addPlayer(id2, 1-score1, &n);
        *n=*n+1;
    }
    
    
    
}


int main() {
    // Assignement des variables
    p** t = malloc(5*sizeof(p));
    int id1=0,id2=0;
    float score1=0;
    int nbrPlayers=0;
    
    
    //OUVERTURE f_matches
    FILE* f_matches = fopen("./matches", "r");
    
    rewind(f_matches); //Remets le pointeur au début
    
    
    int charCount = 0; //Permets de retenir le nombre de caractere par ligne
    char c = NULL;
    
    
    //Check jusqu'a trouver un bout de ligne
    for (c = getc(f_matches); c != EOF; charCount=charCount+1)  {
        
        if (c == '\n') {
            //Lit l'information et la place dans les variables
            fscanf(f_matches,"%d %d %f", &id1, &id2, &score1);
            
            //Ecrit les resultats dans le fichier approprie
            setResult(id1, id2, score1, t, &nbrPlayers);
            
            //Bouge le pointeur jusqu a la ligne suivante
            fseek(f_matches, charCount, SEEK_CUR);
            int charCount = 0;
         }
    }
    
    
    //FERMETURE f_matches
    fclose(f_matches);
    
    
    //Passage de suppression de la mémoire tas
    for (int i=0; i < nbrPlayers; i++) {
        free(t[i]);
    }
    
    //free(t);
    
}

