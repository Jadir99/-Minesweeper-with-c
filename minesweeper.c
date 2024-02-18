#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// definer une structure de donnée de node d'un jeux demineur 
 typedef struct {
    int value; // will conatain the value of the node soit 0 IF it empty or number of mines that near of the node
    int is_mine;
    int is_visited;// will contain 1 if the node is visited
    int is_flagged;// will contain 1 if the node is flagged 
}Node;

//define the matrix that conatain the nodes of the game
void ajouter_matrice(Node **tab,int l, int c){
    
    // la partie de remplire la matrice avec des valeurs nuls
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            tab[i][j].value = 0;
            tab[i][j].is_mine = 0;
            tab[i][j].is_visited = 0;
            tab[i][j].is_flagged = 0;
        }
    }
    // remplire les cases qui sont des mines
    int ind1,ind2;
    srand(time(NULL));
    for (int i = 0; i < l; i++){
        ind1=rand() % l;
        ind2=rand() % c;
        tab[ind1][ind2].value = -1;// indiquer qu'il y a une mine dans cette case
        tab[ind1][ind2].is_mine = 1;
    }
}

void add_the_values_adjacent(Node **tab,int l, int c){
    // ajouter les valeurs des cases adjacentes
    for (int i=0; i<l; i++){
        for (int j=0;j<c; j++){
            if (tab[i][j].is_mine==1){
                for (int k=i-1;k<=i+1;k++){
                    for(int z=j-1;z<=j+1;z++){
                        if(k >= 0 && k < l && z >= 0 && z < c && k!=z){
                            tab[k][z].value++;
                            tab[k][z].is_visited=1;
                        }

                    }
                }
            }
        }
    }

}

void Show_all(Node **tab,int l, int c){
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (tab[i][j].is_mine==1)printf("[m] ");
            else if (tab[i][j].is_flagged==1)printf("[f] ");
            else if (tab[i][j].is_visited==1)printf("[%d] ",tab[i][j].value);
            else printf("[%d] ", tab[i][j].value);
        }
        printf("\n");
    }
}
int main(){

    Node **tab=malloc(10 * sizeof(Node*));
    for (int i=0; i<10; i++){
        tab[i]=malloc(10 * sizeof(Node));
    }

    ajouter_matrice(tab,10,10);
    add_the_values_adjacent(tab,10,10);
    Show_all(tab,10,10);
    free (tab);
    return 0;
}