#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// definer une structure de donnée de node d'un jeux demineur 
 typedef struct {
    int value; // will conatain the value of the node soit 0 IF it empty or number of mines that near of the node
    int is_mine;
    int is_boombed;
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
            tab[i][j].is_boombed = 0;
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
                        }

                    }
                }
            }
        }
    }

}
void Show_all(Node **tab,int l, int c){
    // system("cls");
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if(tab[i][j].is_boombed==1) printf("[ * ] ");
            else if (tab[i][j].is_flagged==1)printf("[ f ] ");
            else if (tab[i][j].is_visited==1){
                if(tab[i][j].is_mine==1)printf("[ m ] ");
                else printf("[ %d ] ",tab[i][j].value);
            }
            else printf("[%d:%d] ", i,j);
        }
        printf("\n");
    }
}
int Visit_Node(Node **tab,int l,int c,int x,int y){
    if (tab[x][y].is_mine==1){
        printf("u looose :( \n");
        tab[x][y].is_boombed=1;
        for(int i=0;i<l;i++){
            for(int j=0;j<c;j++)
            tab[i][j].is_visited=1;
        }
        return 0;
    }else if(tab[x][y].is_visited==1){
        printf("u have been here before :( choose other one !\n");
    }else {
        printf(" nice work :) \n");
        tab[x][y].is_visited=1;
        for (int k=x-1;k<=x+1;k++){
                    for(int z=y-1;z<=y+1;z++){
                        if(k >= 0 && k < l && z >= 0 && z < c ){
                            if(tab[k][z].is_mine!=1){
                                tab[k][z].is_visited=1;
                            }
                        }

                    }
                }
    }
    return 1;
}
void AddFlag(Node **tab, int x,int y){
    tab[x][y].is_flagged=1;
}
void destroyFlag(Node **tab, int x,int y){
    tab[x][y].is_flagged=0;
}
void play_game(Node **tab,int l, int c){
    int x,y,is_lose,choose;
    do {
        printf("\n");
        printf("*************************mini menu *************************\n");
        printf("1-add flag \n");
        printf("2-visit the case  \n");
        printf("3-Delete flag  \n");
        printf("*************************fin de mini menu *************************\n");
        printf("give the request 1/2 :");
        scanf("%d",&choose);
        printf("\n enter the x and y of ur request : ");
        scanf("%d %d",&x,&y);

        switch (choose)
        {
        case 1:
            AddFlag(tab,x,y);
            break;
        case 2:
            is_lose=Visit_Node(tab,10,10,x,y);
            break;
        case 3:
            destroyFlag(tab,x,y);
            break;
        
        default: printf(" try again ur request and make sure u put 1 or 2:) \n");
            break;
        }
        
        Show_all(tab,10,10);
    }while(is_lose!=0 );
}


int main(){

    Node **tab=malloc(10 * sizeof(Node*));
    for (int i=0; i<10; i++){
        tab[i]=malloc(10 * sizeof(Node));
    }

    ajouter_matrice(tab,10,10);
    add_the_values_adjacent(tab,10,10);
    Show_all(tab,10,10);
    play_game(tab,10,10);
    free (tab);
    return 0;
}