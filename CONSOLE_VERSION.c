#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define height 5
#define length 9
#define empty '+'
#define n 'X'
#define b 'O'

// Definition des couleurs

void couleur(int ForgC)
{

    // We will need this handle to get the current background attribute
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // we use csbi for the wAttributes word.
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        // Mask out all but the background attribute
        //, and add in the foreground color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

void red()
{
    couleur(12);
}
void yellow()
{
    couleur(14);
}

void bleu()
{
    couleur(11);
}
void white()
{
    couleur(15);
}
void green()
{
    couleur(10);
}
// definition des structures


typedef struct
{
    int row, column; // les rows et les columns du board de game
} location;

int afficher_fichier(char *nom){
    char *nom_du_fichier = nom;
    char lire_chaine[50];
    FILE *fichier = NULL;
    if((fichier = fopen(nom_du_fichier,"r")) == NULL)                       //au cas où un erreur d'ouverture du fichier
    {
        fprintf(stderr,"erreur d'ouvrir le fichier %s\n",nom_du_fichier);
        return -1;
    }
    while(fgets(lire_chaine,sizeof(lire_chaine),fichier) != NULL)            //lecture du contenu du fichier
        printf("%s",lire_chaine);
    fclose(fichier);
    return 1;
}


// Initialisation  et remplissage du plateau

int pos_initial_pawn(char board[height][length])
{
    int i, j;
    int x = 0;
    int y = 0;
    while (y < height)
    {
        x = 0;
        while (x < length)
        {
            if (y < 2)
            {
                board[y][x] = n;
            }
            else if (y > 2)
            {
                board[y][x] = b;
            }
            else
            {
                if (x < 4)
                {
                    if (x % 2 == 0)
                    {
                        board[y][x] = b;
                    }
                    else
                    {
                        board[y][x] = n;
                    }
                }
                else if (x > 4)
                {
                    if (x % 2 == 0)
                    {
                        board[y][x] = n;
                    }
                    else
                    {
                        board[y][x] = b;
                    }
                }
                else
                {
                    board[y][x] = b;
                }
            }
            x++;
        }
        y++;
    }
    board[2][4] = empty;
    return 0;
}

// affichage du plateau du jeu

void game_board(char board[height][length])
{
    int i, j, k;
    red();
    printf("                    _______0___1___2___3___4___5___6___7___8________\n");
    printf("                   |     ");
    green();
    printf("  v   v   v   v   v   v   v   v   v      ");
    red();
    printf("  |\n");
    yellow();
    printf("                   |  ");
    red();
    printf("  _______________________________________ ");
    yellow();
    printf("    |\n");
    red();
    printf("                   |   |                                       |    |\n");

    for (i = 0; i < height; i++)
    {
        if (i != height - 1)
        {
            red();
            printf("                  %d", i);
            green();
            printf(" > ");
            yellow();
            printf(" |   ");

            for (j = 0; j < length; j++)
            {
                if (j != length - 1)
                {
                    yellow();
                    printf("%c", board[i][j]);
                    printf("---");
                }
                else if (j == length - 1)
                    printf("%c   |    |\n", board[i][j]);
            }
            red();
            printf("                   |   |  ");

            for (k = 0; k < length - 1; k++)
            {
                // on a un parcours par ligne dont on doit affecter la structure finale du tableau de jeu Fanorona
                if ((i + k) % 2 == 0)
                    printf(" | \\");

                if ((i + k) % 2 != 0)
                    printf(" | /");
            }
            printf(" |   ");
            red();
            printf("|    |\n"); // La derniere colonne du tableau de jeu
        }

        else if (i == height - 1)
        {
            red();
            printf("                  %d", i);
            green();
            printf(" >");
            yellow();
            printf("  |   "); // La derniere ligne du tableau de jeu
            for (j = 0; j < length; j++)
            {
                if (j != length - 1)
                {
                    printf("%c", board[i][j]);
                    printf("---");
                }
                else if (j == length - 1)
                    printf("%c ", board[i][j]);
            }

            printf("  |    |\n");
        }
    }
    red();
    printf("                   |   |_______________________________________|    |\n");
    yellow();
    printf("                   |                                                |\n");
    red();
    printf("                   |________________________________________________|");
    printf("\n\n");
}

//fonction qui designe aleatoirement le joueur debutant

char frt_player_alea(char player1, char player2)
{
    srand(time(NULL));
    int i = rand() % 2;
    char current_player;
    if (i == 0)
    {
        return player1;
    }
    else
    {
        return player2;
    }
}

//le choix du pion a deplacer

location select_stone(char current_player)
{
    int i, j;
    location p;
    red();
    printf("________________________________________________________________________________________\n");
    yellow();
    printf("\n-Joueur %c, veuillez entrer respectivement le nombre de la row et la column du pion que\n vous voulez deplacer dans le board de game:\n", current_player);
    scanf("%d  %d", &i, &j);
    while ((i < 0 )||( i > 4 )|| (j < 0 )|| (j > 8))
    {
        red();
        printf("________________________________________________________________________________________\n");
        yellow();
        printf("ERREUR! Veuillez entrer respectivement le nombre de la row(0=>4) et la column(0=>8) du \npion que vous voulez deplacer dans le board de game:\n");
        scanf("%d  %d", &i, &j);
    }
    p.row = i;
    p.column = j;

    return (p);
}

//fonction qui teste si le pion choisi par le joueur est le bon pion

int my_pawn(char current_player, char board[height][length],location p )
{

    if (board[p.row][p.column] == current_player)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//le choix du direction du deplacement

int mov_direc(char current_player,location p)
{
    int way;
    if (((p.column) + (p.row)) % 2 != 0)
    {
        red();
        printf("________________________________________________________________________________________\n");
        yellow();
        printf("Vous pouvez deplacer votre pion en 4 directions possibles. Veuillez choisir une: \n\n 0 - EN HAUT \n 2 - A DROITE \n 4 - EN BAS \n 6 - A GAUCHE \n\n");
        scanf("%d", &way);
        while (way !=0 && way !=2 &&way !=4 && way !=6 )

        {
            red();
            printf("________________________________________________________________________________________\n");
            yellow();
            printf("ERREUR! Veuillez choisir une des directions proposees: \n\n  0 - EN HAUT \n 2 - A DROITE \n 4 - EN BAS \n 6 - A GAUCHE \n\n");
            printf("\n");
            scanf("%d", &way);
        }
    }
    else
    {
        red();
        printf("________________________________________________________________________________________\n");
        yellow();
        printf("Vous pouvez deplacer votre pion en 8 directions possibles. Veuillez choisir une: \n\n 0 - EN HAUT \n 1 - EN HAUT DROIT \n 2 - A DROITE \n 3 - EN BAS DROIT \n 4 - EN BAS \n 5 - EN BAS GAUCHE \n 6 - A GAUCHE \n 7 - EN HAUT GAUCHE \n \n ");
        scanf("%d", &way);
        while (way > 7 && way < 0)
        {
            red();
            printf("________________________________________________________________________________________\n");
            yellow();
            printf("ERREUR! Veuillez choisir une des directions proposees: \n\n 0-haut \n 1-bas \n 2-droit \n 3-gauche \n 4-haut droit \n 5-haut gauche \n 6-bas droit \n 7-bas gauche \n");
            printf("\n");
            scanf("%d", &way);
        }
    }

    return way;
}

// fonction qui  donne la position du pion apr�s d�placement

location next_position(location place, int way)
{
    location pl;
    switch (way)
    {
    case 0: // La direction TOP (en haut du pion selectionn�)(x-1 , y)
        pl.row = place.row - 1;
        pl.column = place.column;
        break;

    case 1: // La direction TOP RIGHT(haut droite du pion selectionne)  (x-1 , y+1)
        pl.row = place.row - 1;
        pl.column = place.column + 1;
        break;

    case 2:
        pl.row = place.row;
        pl.column = place.column + 1; // La direction RIGHT (a droite du pion selectionne)(x , y+1)
        break;

    case 3: // La direction BUTTOM RIGHT(bas droite du pion selectionne) (x+1 , y+1)
        pl.row = place.row + 1;
        pl.column = place.column + 1;
        break;

    case 4:
        pl.row = place.row + 1;
        pl.column = place.column; // La direction BUTTOM (en bas du pion selectionn�)(x+1 , y)
        break;

    case 5: // La direction BUTTOM LEFT(bas guache du pion selectionne) (x+1 , y-1)
        pl.row = place.row + 1;
        pl.column = place.column - 1;
        break;

    case 6: // La direction LEFT (a gauche du pion selectionne)(x , y-1)
        pl.row = place.row;
        pl.column = place.column - 1;
        break;

    case 7: // La direction TOP LEFT(haut guache du pion selectionne) (x-1 , y-1)
        pl.row = place.row - 1;
        pl.column = place.column - 1;
        break;
    }
    return pl;
}

//fct qui test si le pion p est a l'exterieur du plateau du jeu.

int frontier(location p)
{
    if ((p.row > 4 || p.row < 0) || (p.column > 8 || p.column < 0))
    {
        return 0;
    }
    return 1;
}

//fonction qui teste si la case p est occupee

int occupied(location p, char board[height][length])
{

    if (board[p.row][p.column] == '+')
    {
        return 1;
    }
    return 0;
}

int pos_dep(location p ,char board[height][length])
{
    int i,j;
    j=0;
    location np;
    if((p.row+p.column)%2==0)
    {
        for (i = 0; i < 8; i++)
        {

            np=next_position(p,i);
            if (frontier(np)==0 || occupied(np,board)==0)
            {
                j++;
            }
        }
        if(j==8)
        {
            return 0;
        }
    }else{
        for (i = 0; i < 4; i++)
        {
            np=next_position(p,2*i);
            if (frontier(np)==0 || occupied(np,board)==0)
            {
                j++;
            }
        }
        if(j==4){
            return 0;
        }
    }
    return 1;

}

//Cette fonction teste la possibilite d'une percussion pour un pion
int percussion(location p, int way, char current_player, char board[height][length])
{
    location p1 = next_position(p, way);
    location p2 = next_position(p1, way);
    char c = board[p2.row][p2.column];

    if ( frontier(p2) == 0 || occupied(p2, board) ==1 || frontier(p1)==0|| occupied(p1,board)==0 || c==current_player)
    {
        return 0;
    }
   return 1;
}

//Detection d'absorption possible d'une maniere generale
//Cette fonction teste la possibilite d'une absorption pour un pion
int absorption(location p,int way, char current_player, char board[height][length])
{
    int oppway = (way + 4) % 8;

    location p1 = next_position(p, oppway);
    location p2 = next_position(p, way);
    char c = board[p1.row][p1.column];
    if (frontier(p1) == 0 || occupied(p1, board) == 1 || frontier(p2)==0 || occupied(p2,board)==0 || c==current_player)
    {
        return 0;
    }
    return 1;
}
//foction qui teste la possibilite de manger une autre fois par le meme pion choisi
int pos_pawn(location p,int way, char board[height][length],char current_player)
{
    int per,abs;
    per=percussion(p,way,current_player,board);
    abs=absorption(p,way,current_player,board);
    if (  per==1 || abs==1)
    {
        return 1;
    }
    return 0;
}
//detection capture par le pion p

int pos_cap_pawn(location p, char current_player, char board[height][length])
{

    int i,ps;
    if((p.row+p.column)%2==0)
    {
        for (i = 0; i < 8; i++)
        {

            ps=pos_pawn(p,i,board,current_player);
            if (ps==1)
            {
                return 1;
            }
        }
    }else{
        for (i = 0; i < 4; i++)
        {
            ps=pos_pawn(p,2*i,board,current_player);
            if (ps==1)
            {
                return 1;
            }
        }
    }
    return 0;
}

//detection capture dans l'ensemble du plateau du jeu

int pos_cap_board(char current_player,char board[height][length])
{
    location p;
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < length; j++)
        {
            p.row = i;
            p.column = j;
            if (board [i][j]==current_player && pos_cap_pawn(p, current_player, board) == 1)
            {
                return 1;
            }
        }
    }
    return 0;
}

//Deplacement du pion dans le plateau du jeu

void dep_pawn(location p, int way, char board[height][length])
{
    location np = next_position(p, way);
    char c = board[p.row][p.column];
    board[p.row][p.column] = empty;
    board[np.row][np.column] = c;
}

//fonction qui retire les pions manges par approche du plateau du jeu


void eaten_pawn_per(location p, char current_player,int way, char board[height][length])
{
    int i, j;
    location np ;
    np= next_position(p, way);
    i = np.row;
    j = np.column;

    do{
        board[i][j] = empty;
        np = next_position(np, way);
        i = np.row;
        j = np.column;
    }while (board[i][j] != empty && board[i][j] != current_player && frontier(np)!=0);
}

//fonction qui retire les pions manges par retrait du plateau du jeu

void eaten_pawn_abs(location p, int current_player, int way, char board[height][length])
{
    int i, j;
    int opway = (way + 4) % 8;
    location opnp;
    opnp= next_position(p, opway);
    i = opnp.row;
    j = opnp.column;
    do{

        board[i][j] = empty;
        opnp = next_position(opnp, opway);
        i = opnp.row;
        j = opnp.column;
    } while (board[i][j] != empty && board[i][j] != current_player && frontier(opnp) != 0);
}


//erreur dans le choix de pion


int il_pawn(location p, char current_player, char board[height][length])
{
    int mp = my_pawn(current_player, board,p);
    int ps=pos_dep(p,board);
    if (mp == 0)
    {
        red();
        printf("________________________________________________________________________________________\n");
        yellow();
        printf("\n             ATTENTION : Vous ne pouvez pas jouer avec un pion adverse \n");
        return 0;
    }
    if(ps==0)
    {
        red();
        printf("________________________________________________________________________________________\n");
        yellow();
        printf("\n             ATTENTION : Vous ne pouvez pas deplacer ce pion  \n");
        return 0;
    }
    return 1;
}

//erreur dans le choix du direction


int il_mvt(location p, int way, char current_player, char board[height][length])
{
    location np = next_position(p, way);
    int frt = frontier(np);
    int occ = occupied(np, board);
    if (frt == 0)
    {
        red();
        printf("________________________________________________________________________________________\n");
        yellow();
        printf("\n             DEPLACEMENT IMPOSSIBLE : le coup effectuee sort du plateau \n");

        return 0;
    }
    else if (occ == 0)
    {
        red();
        printf("________________________________________________________________________________________\n");
        yellow();
        printf("\n       DEPLACEMENT IMPOSSIBLE : La case d'arriv�e de votre pion n'est pas vide \n");
        return 0;
    }
    return 1;
}






int test_pos(location p,char board[height][length], int pos_tr[5][9],char current_player)
{
    location np;
    int i,t,w,occ,frt,lig,col,ps;
    t=0;
    lig=p.row;
    col=p.column;
    if((lig+col)%2==0)
    {
        for (i = 0; i <= 7; i++)
        {
            np=next_position(p,i);
            occ=occupied(np,board);
            frt=frontier(np);
            w=pos_tr[np.row][np.column];
            ps=pos_pawn(p,i,board,current_player);

            if( (frt==0) || (occ==0) || ps==0 || w==1 )
            {
              t++;
            }
        }
        if(t==8)
        {
            return 0;
        }
    }
    else{
        for (i = 0; i <=3; i++)
        {
            np=next_position(p,2*i);
            occ=occupied(np,board);
            frt=frontier(np);
            ps=pos_pawn(p,2*i,board,current_player);
            w=pos_tr[np.row][np.column];
            if( (frt==0) || (occ==0) || ps==0 || w==1)
            {
              t++;
            }
        }
        if(t==4)
        {
            return 0;
        }
    }
    return 1;

}


//fonction qui retire les pions manges par le meme pion choisi du plateau du jeu

int eat_other(location p, location lp, char board[height][length], char current_player)
{
    int i = 0;

    int j,f,x,y;
    int pos_tr[5][9];
    for(x=0;x<5;x++)
    {
        for(y=0;y<9;y++)
        {
            pos_tr[x][y]=0;

        }
    }

    pos_tr[lp.row][lp.column]=1;
    pos_tr[p.row][p.column]=1;
    f=test_pos(p,board,pos_tr,current_player);
    while(f==1 )
    {

        int d;
        red();
        printf("________________________________________________________________________________________\n");
        yellow();
        printf("ATTENTION! vous pouvez faire une autre capture par le pion que tu as deja choisi.\n  -si vous voulez faire cette capture tapez 1.\n   -sinon tapez 2:\n");
        scanf("%d", &d);

        if(d ==2){
            game_board(board);
            return 0;
        }else if (d == 1){

            int way, per, abs,iw,u,v;
            location np;
            np = next_position(p, way);
            u=np.row;
            v=np.column;
            way = mov_direc(current_player,p);
            per = percussion(p, way, current_player, board);
            abs = absorption(p, way, current_player, board);
            np = next_position(p, way);
            u=np.row;
            v=np.column;
            while((per!=1 && abs!=1) || pos_tr[u][v]==1)
            {
                red();
                printf("________________________________________________________________________________________\n\n");
                yellow();
                printf("     ATTENTION! une capture est possible veuillez choisir une direction pour la faire:\n");
                way = mov_direc(current_player,p);
                iw=il_mvt(p,way,current_player, board);
                while(iw==0)
                {
                    way=mov_direc(current_player,p);
                    iw=il_mvt(p,way,current_player, board);
                }
                if(pos_tr[u][v]==1)
                {
                    red();
                    printf("________________________________________________________________________________________\n\n");
                    yellow();
                    printf("     ATTENTION!Cette position est deja occupee, veuillez choisir un autre direction. \n");

                }
                per = percussion(p, way, current_player, board);
                abs = absorption(p, way, current_player, board);
                np = next_position(p, way);
                u=np.row;
                v=np.column;
            }
            if(per==1 && abs==0 )
            {
                pos_tr[p.row][p.column] =1;
                eaten_pawn_per(np, current_player, way, board);
                dep_pawn(p, way, board);
                p = next_position(p, way);
                per = percussion(p, way, current_player, board);
                abs = absorption(p, way, current_player, board);
                game_board(board);

            }
            else if(per ==0 && abs ==1)
            {
                pos_tr[p.row][p.column] =1;
                dep_pawn(p, way, board);
                eaten_pawn_abs(np, current_player, way, board);
                p = next_position(p, way);
                per = percussion(p, way, current_player, board);
                abs = absorption(p, way, current_player, board);
                game_board(board);

            }
            else if (per==1 && abs==1 ){
                int t;
                red();
                printf("________________________________________________________________________________________\n\n");
                yellow();
                printf("   Vous pouvez effectuer une capture par approche ou par retrait. Que choisissez-vous ?\n\t1-approche \n\t2-retrait\n");
                scanf("%d",&t);
                if(t==1){
                    pos_tr[p.row][p.column] =1;
                    eaten_pawn_per(np, current_player, way, board);
                    dep_pawn(p, way, board);
                    p = next_position(p, way);
                    per = percussion(p, way, current_player, board);
                    abs = absorption(p, way, current_player, board);
                    game_board(board);
                }
                else{
                    pos_tr[p.row][p.column] =1;
                    eaten_pawn_abs(p, current_player, way, board);
                    dep_pawn(p, way, board);
                    game_board(board);
                    p = next_position(p, way);
                    per = percussion(p, way, current_player, board);
                    abs = absorption(p, way, current_player, board);

                }
                pos_tr[p.row][p.column]=1;
            }

        }
        f=test_pos(p,board,pos_tr,current_player);
    }
    red();
    printf("________________________________________________________________________________________\n\n");
    yellow();
    printf("                   vous ne pouvez pas deplacer encore ce pion\n");
    return 0;
}
int nb_black(char board[height][length])
{
    int i,j,nbb;
    nbb=0;
    for(i=0;i<height;i++)
    {
        for(j=0;j<length;j++)
        {
            if(board[i][j]==n)
            {
                nbb++;
            }

        }
    }
    return nbb;
}
int nb_white(char board[height][length])
{
    int i,j,nbw;
    nbw=0;
    for(i=0;i<height;i++)
    {
        for(j=0;j<length;j++)
        {
            if(board[i][j]==b)
            {
                nbw++;
            }

        }
    }
    return nbw;
}

void score(char board[height][length])
{
    int nx,no;
    nx = nb_black(board);
    no =nb_white(board);
    red();
    printf("++++++++++++++++++++++++++++++++++++++");
    yellow();
    printf("   score   ");
    red();
    printf("++++++++++++++++++++++++++++++++++++++\n");
    printf("           _______________________                   _______________________\n");
    printf("          |                       |                 |                       |\n");
    printf("          |  ");
    yellow();
    if(nx<10){
        printf("JOUEUR X : 0%d pions  ",nx);
    }else{
        printf("JOUEUR X : %d pions  ",nx);
    }

    red();
    printf("|                 |  ");
    yellow();
    if(no<10){
        printf("JOUEUR X : 0%d pions  ",no);
    }else{
        printf("JOUEUR X : %d pions  ",no);
    }
    red();
    printf("|\n          |_______________________|                 |_______________________|");
    printf("\n\n\n");
}

int joueurvsjoueur()
{

    char current_player = frt_player_alea(n, b);
    char board[height][length];
    pos_initial_pawn(board);
    red();
    printf("________________________________________________________________________________________\n\n");
    yellow();
    printf("Pour jouer, veuillez : \n> D'abord entrer la ligne du pion a deplacer ( de 0 ==>4 ) puis cliquez sur  -Entree-\n> Ensuite,entrer la colonne du pion a deplacer ( de 0 ==>8 ) puis cliquez sur -Entree-\n");
    printf("> Enfin,entrer l,une des directions qui vous seront proposees, puis cliquez -Entree- \n");
    red();
    printf("________________________________________________________________________________________\n\n");
    yellow();
    game_board(board);
    int nb_x, nb_o;
    nb_x =nb_black(board);
    nb_o =nb_white(board);
    score(board);
    yellow();
    printf("C'est votre tour Joueur %c.\n", current_player);
    do
    {
        location p;
        location lp;
        int way, im,tr, pcb, pcp, per, abs;
        p = select_stone(current_player);
        pcb = pos_cap_board(current_player, board);
        tr = il_pawn(p, current_player, board);
        while (tr == 0)
        {
            p = select_stone(current_player);
            tr = il_pawn(p, current_player, board);
        }
        lp=p;
        way=mov_direc(current_player,p);
        im=il_mvt(p,way,current_player, board);
        while(im==0)
        {
            way=mov_direc(current_player,p);
            im=il_mvt(p,way,current_player, board);
        }
        if (pcb == 0)
        {

            dep_pawn(p, way, board);
            game_board(board);
        }
        else if (pcb == 1)
        {
            pcp = pos_cap_pawn(p, current_player, board);
            while(pcp==0){
                red();
                printf("________________________________________________________________________________________\n");
                yellow();
                printf("\n ATTENTION : Une capture est possible dans le jeu. Veuillez en effectuer une. \n");
                red();
                printf("________________________________________________________________________________________\n");
                p = select_stone(current_player);
                tr = il_pawn(p, current_player, board);
                while (tr == 0)
                {
                    p = select_stone(current_player);
                    tr = il_pawn(p, current_player, board);
                }
                lp=p;
                pcp = pos_cap_pawn(p, current_player, board);
                way = mov_direc(current_player,p);
                im = il_mvt(p, way, current_player, board);
                while (im == 0)
                {
                    way = mov_direc(current_player,p);
                    im = il_mvt(p, way, current_player, board);
                }
            }
            per = percussion(p, way, current_player, board);
            abs = absorption(p, way, current_player, board);
            while(per == 0 && abs == 0)
            {
                way = mov_direc(current_player,p);
                per = percussion(p, way, current_player, board);
                abs = absorption(p, way, current_player, board);
            }
            if (per==1 && abs==1){
                int r;
                red();
                printf("________________________________________________________________________________________\n");
                yellow();
                printf("Vous pouvez effectuer une capture par approche ou par retrait. Que choisissez-vous ?\n\t1- Percussion (en approche) \n\t2- Absorptionn (en retrait)\n");
                scanf("%d",&r);
                if(r==1){
                    eaten_pawn_per(p, current_player, way, board);
                    dep_pawn(p, way, board);
                    game_board(board);
                    p = next_position(p, way);
                    eat_other(p,lp,board,current_player);
                    per = percussion(p, way, current_player, board);
                    abs = absorption(p, way, current_player, board);
                }
                else{
                    eaten_pawn_abs(p, current_player, way, board);
                    dep_pawn(p, way, board);
                    game_board(board);
                    p = next_position(p, way);
                    eat_other(p,lp,board,current_player);
                    per = percussion(p, way, current_player, board);
                    abs = absorption(p, way, current_player, board);
                }
            }
            else if (per = 1 && abs == 0)
            {
                eaten_pawn_per(p, current_player, way, board);
                dep_pawn(p, way, board);
                game_board(board);
                p=next_position(p,way);
                eat_other(p,lp,board,current_player);
                per = percussion(p, way, current_player, board);
                abs = absorption(p, way, current_player, board);
            }
            else if (per == 0 && abs == 1)
            {
                eaten_pawn_abs(p, current_player, way, board);
                dep_pawn(p, way, board);
                game_board(board);
                p = next_position(p, way);
                eat_other(p,lp,board,current_player);
                per = percussion(p, way, current_player, board);
                abs = absorption(p, way, current_player, board);
            }
        }
        if(current_player ==n)
        {current_player =b;}
        else{
            current_player =n;
        }
        nb_x = nb_black(board);
        nb_o =nb_white(board);
        score(board);
        yellow();
        if(nb_x!=0 && nb_o!=0)
        {
            printf("    C'est votre tour Joueur %c.\n", current_player);
        }
    }while(nb_x > 0 && nb_o > 0);
    afficher_fichier("win.txt");
    if(nb_x==0)
    {
        printf("\n\n\t\tFelicitation! JoueurX, vous avez gagnez.");
    }else{
        printf("\n\n\t\tFelicitation! JoueurO, vous avez gagnez.");
    }
    red();
    printf("________________________________________________________________________________________\n");
}



//////////////////////////////////////////////machine simple///////////////////////////////////////////////////////////////////////


void ini_tab(int tab[5][9][8])
{
    int i,j,k;

    for(i=0;i<5;i++)
    {
        for(j=0;j<9;j++)
        {

            for(k=0;k<8;k++)
            {
                tab[i][j][k]=0;
            }
        }
    }
}


int test_cap_mach(char mach1 ,char board[height][length] , int tab[5][9][8])
{
    int i,j,k,x,y,z;
    location p;
    z=0;
    for(i=0;i<5;i++)
    {
        for(j=0;j<9;j++)
        {
            p.row=i;
            p.column=j;
            if(board[i][j]==mach1 && pos_dep(p,board)==1)
            {
                if((i+j)%2==0)
                {
                    for(k=0;k<8;k++)
                    {
                        if( pos_pawn(p,k,board,mach1)==1)
                        {
                            tab[i][j][k]=1;
                            z++;
                        }

                    }


                }
                else{

                    for(k=0;k<4;k++)
                    {
                        if( pos_pawn(p,2*k,board,mach1)==1)
                        {
                            tab[i][j][2*k]=1;
                            z++;
                        }
                    }
                }
            }
        }
    }
    return z;
}










void machine1(char mach1,char board[height][length] )
{
    srand(time(NULL));
    int tab[5][9][8];
    int t,a,c,d,e,f,way,i,j,k,per,abs;
    ini_tab(tab);
    t=test_cap_mach(mach1,board,tab);
    a=nb_white(board);
    location p,l,s;

    if(t==0)
    {

        i=0;
        d=0;
        while(d=0 && i<height)
        {
            j=0;
            while (d=0 && j<length)
            {
                k=0;
                while(d=0 && k<8)
                {
                    l.row=i;
                    l.column=j;
                    s=next_position(l,k);
                    if(board[i][j]==mach1 && occupied(s,board)==1 && frontier(s)==1)
                    {
                        p.row=i;
                        p.column=j;
                        way=k;
                        d=1;
                    }
                    k++;

                }
                j++;
            }
            i++;
        }
        dep_pawn(p, way,board);
    }
    else{
        c=rand()%t;
        d=0;
        while(d=0 && i<height)
        {
            while (d=0 && j<length)
            {
                while(d=0 && k<8)
                {
                    if(tab[i][j][k]==1)
                    {
                        c=c-1;
                    }
                    if(c==0)
                    {
                        p.row=i;
                        p.column=j;
                        way=k;
                        d=1;
                    }
                    k++;
                }
                j++;
            }
            i++;
        }
        per = percussion(p, way, mach1, board);
        abs=absorption(p, way, mach1, board);
        if (per==1 && abs==1){
            e=rand()%2;
            if(e==1){
                eaten_pawn_per(p, mach1, way, board);
                dep_pawn(p, way, board);
                game_board(board);
            }
            else{
                eaten_pawn_abs(p,mach1, way, board);
                dep_pawn(p, way, board);
                game_board(board);
            }
        }
        else if (per = 1 && abs == 0)
        {
            eaten_pawn_per(p, mach1,way, board);
            dep_pawn(p, way, board);
            game_board(board);
        }
        else if (per == 0 && abs == 1)
        {
            eaten_pawn_abs(p, mach1, way, board);
            dep_pawn(p, way, board);
            game_board(board);
        }
    }
}
int machinenn(char board[5][9] )
{
    int t=pos_cap_board(b,board);
    int x,y,i,r,d,j,e,per, abs;
    location p,np,lp;
    if(t=0)
    {
        srand(time(NULL));
        x=rand()%5;
        srand(time(NULL));
        y=rand()%9;
        p.row=x;
        p.column=y;
        while(board[x][y]!=b && pos_dep(p,board)==0)
        {
            srand(time(NULL));
            x=rand()%5;
            srand(time(NULL));
            y=rand()%9;
            p.row=x;
            p.column=y;

        }
        if((x+y)%2==0){
            srand(time(NULL));
            i=rand()%8;
            lp=next_position(p,i);
            while(frontier(lp)==0 || occupied(lp,board)==0){
                i=rand()%8;
                lp=next_position(p,i);
            }
            dep_pawn(p,i,board);
            return 1;
        }else{
            srand(time(NULL));
            i=rand()%4;
            lp=next_position(p,2*i);
            while(frontier(lp)==0 || occupied(lp,board)==0){
                i=rand()%4;
                lp=next_position(p,2*i);
            }
            dep_pawn(p,2*i,board);
            return 1;

        }
    }else{
        srand(time(NULL));
        x=rand()%5;
        srand(time(NULL));
        y=rand()%9;
        p.row=x;
        p.column=y;

        while(board[x][y]==empty || board[x][y]!=b || pos_cap_pawn(p,b,board)==0 )
        {
            srand(time(NULL));
            x=rand()%5;
            srand(time(NULL));
            y=rand()%9;
            p.row=x;
            p.column=y;

        }
        if((x+y)%2==0){
            srand(time(NULL));
            i=rand()%8;
            while( pos_pawn(p,i,board,b)==0)
            {
                srand(time(NULL));
                i=rand()%8;

            }
            r=i;
        }else{
            srand(time(NULL));
            i=rand()%4;
            while( pos_pawn(p,2*i,board,b)==0)
            {
                srand(time(NULL));
                i=rand()%4;

            }
            r=2*i;
        }
        per = percussion(p, r, b, board);
        abs=absorption(p, r, b, board);
        if (per==1 && abs==1){
            e=rand()%2;
            if(e==1){
                eaten_pawn_per(p, b, r, board);
                dep_pawn(p, r, board);
                game_board(board);
            }
            else{
                eaten_pawn_abs(p,b, r, board);
                dep_pawn(p, r, board);
                game_board(board);
            }
        }
        else if (per = 1 && abs == 0)
        {
            eaten_pawn_per(p, b,r, board);
            dep_pawn(p, r, board);
            game_board(board);
        }
        else if (per == 0 && abs == 1)
        {
            eaten_pawn_abs(p,b, r, board);
            dep_pawn(p, r, board);
            game_board(board);
        }

    }
}

void machinefacile()
{
    char current_player = frt_player_alea(n,b);
    char board[height][length];
    pos_initial_pawn(board);
    red();
    printf("________________________________________________________________________________________\n\n");
    yellow();
    printf("Pour jouer, veuillez : \n> D'abord entrer la ligne du pion a deplacer ( de 0 ==>4 ) puis cliquez sur  -Entree-\n> Ensuite,entrer la colonne du pion a deplacer ( de 0 ==>8 ) puis cliquez sur -Entree-\n");
    printf("> Enfin,entrer l,une des directions qui vous seront proposees, puis cliquez -Entree- \n");
    red();
    printf("________________________________________________________________________________________\n\n");
    yellow();
    game_board(board);
    int nb_x, nb_o;
    nb_x =nb_black(board);
    nb_o =nb_white(board);
    score(board);
    red();
    printf("________________________________________________________________________________________\n\n");
    do
    {
        if (current_player!=b)
        {

            location p;
            location lp;
            int way, im,tr, pcb, pcp, per, abs;

            p = select_stone(current_player);
            pcb = pos_cap_board(current_player, board);
            tr = il_pawn(p, current_player, board);
            while (tr == 0)
            {
                p = select_stone(current_player);
                tr = il_pawn(p, current_player, board);
            }
            lp=p;
            way=mov_direc(current_player,p);
            im=il_mvt(p,way,current_player, board);
            while(im==0)
            {
                way=mov_direc(current_player,p);
                im=il_mvt(p,way,current_player, board);
            }
            if (pcb == 0)
            {
                dep_pawn(p, way, board);
                game_board(board);
            }
            else if (pcb == 1)
            {
                pcp = pos_cap_pawn(p, current_player, board);
                while(pcp==0){
                    red();
                    printf("________________________________________________________________________________________\n");
                    yellow();
                    printf("\n ATTENTION : Une capture est possible dans le jeu. Veuillez en effectuer une. \n");
                    red();
                    printf("________________________________________________________________________________________\n");
                    p = select_stone(current_player);
                    tr = il_pawn(p, current_player, board);
                    while (tr == 0)
                    {
                        p = select_stone(current_player);
                        tr = il_pawn(p, current_player, board);
                    }
                    lp=p;
                    pcp = pos_cap_pawn(p, current_player, board);

                    way = mov_direc(current_player,p);
                    im = il_mvt(p, way, current_player, board);
                    while (im == 0)
                    {
                        way = mov_direc(current_player,p);
                        im = il_mvt(p, way, current_player, board);
                    }
                }
                per = percussion(p, way, current_player, board);
                abs = absorption(p, way, current_player, board);
                while(per == 0 && abs == 0)
                {
                    way = mov_direc(current_player,p);
                    per = percussion(p, way, current_player, board);
                    abs = absorption(p, way, current_player, board);

                }
                if (per==1 && abs==1){
                    int r;
                    red();
                    printf("________________________________________________________________________________________\n");
                    yellow();
                    printf("Vous pouvez effectuer une capture par approche ou par retrait. Que choisissez-vous ?\n\t1-approche \n\t2-retrait\n");
                    scanf("%d",&r);
                    if(r==1){
                        eaten_pawn_per(p, current_player, way, board);
                        dep_pawn(p, way, board);
                        game_board(board);
                        p = next_position(p, way);
                        eat_other(p,lp,board,current_player);
                        per = percussion(p, way, current_player, board);
                        abs = absorption(p, way, current_player, board);
                    }
                    else{
                        eaten_pawn_abs(p, current_player, way, board);
                        dep_pawn(p, way, board);
                        game_board(board);
                        p = next_position(p, way);
                        eat_other(p,lp,board,current_player);
                        per = percussion(p, way, current_player, board);
                        abs = absorption(p, way, current_player, board);
                    }
                }
                else if (per = 1 && abs == 0)
                {
                    eaten_pawn_per(p, current_player, way, board);
                    dep_pawn(p, way, board);
                    game_board(board);
                    p=next_position(p,way);
                    eat_other(p,lp,board,current_player);
                    per = percussion(p, way, current_player, board);
                    abs = absorption(p, way, current_player, board);
                }
                else if (per == 0 && abs == 1)
                {
                    eaten_pawn_abs(p, current_player, way, board);
                    dep_pawn(p, way, board);
                    game_board(board);
                    p = next_position(p, way);
                    eat_other(p,lp,board,current_player);
                    per = percussion(p, way, current_player, board);
                    abs = absorption(p, way, current_player, board);
                }
                nb_x = nb_black(board);
                nb_o =nb_white(board);
                score(board);
                red();
                printf("________________________________________________________________________________________\n\n");
                yellow();
            }
        }else if (current_player==b){
            yellow();
            printf("\n\t\t\t\tC'est le tour de la machine!\n\n");
            machinenn(board);
            nb_x = nb_black(board);
            nb_o =nb_white(board);
            score(board);
            red();
            printf("________________________________________________________________________________________\n\n");
        }
        if(current_player ==n)
        {current_player =b;}
        else{
            current_player =n;
        }

    }while(nb_x > 0 && nb_o > 0);
    red();
    printf("________________________________________________________________________________________\n\n");
    printf(" __________________________                                   __________________________\n");
    printf("|__________________________| ");
    yellow();
    printf("           fin du jeu           ");
    red();
    printf("|__________________________|\n\n");
    yellow();
    afficher_fichier("FIN.txt");
    if(nb_o==0)
    {
        afficher_fichier("win.txt");
    }else{
        afficher_fichier("lose.txt");
    }
    red();
    printf("________________________________________________________________________________________\n");

}
//////////////////////////////////////////////////////machine moyenne/////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*la file qui va contenir tout les possibilites de mouvement possibles et leur gain;*/
typedef struct test{
    struct test* suivant;
    location p;
    int tab_dir[45];
    int sommet;
    int gain;
}test;

typedef struct{
    test* debut;
    test* fin;

}file;

///fonction qui copie le contenu du tableau tab2 dans le tabeau tab1
void copy_table(int tab1[45],int tab2[45])
{
    int i;
    for(i=0;i<45;i++)
    {
        tab1[i] = tab2[i];
    }
}
//fct qui prend en parametres la positon el le tabeau des direcions et elle retourne la position finale.

location next_dir(location p,int tab1[45],int i)
{
    int j =0;
    location np=p;
    while(j<i)
    {
        np=next_position(np,tab1[i]);
        j++;
    }
    j=tab1[i];
    if ((np.row+np.column)%2 == 0)
    {

    }

}


///fonction qui copie le contenu du tableau tab2 dans le tabeau tab1

void copy_tab(char tab[5][9],char tab2[5][9])
{
    for(int i=0;i<5;i)
    {
        for(int j=0;j<9;j++){
            tab[i][j] = tab2[i][j];
        }

    }
}

//fonction qui insere te dans la file File.

void insert(test te,file File){
    te.suivant=NULL;
    if (File.debut==NULL)
    {
        File.debut=&te;
    }
    else
    {
        test *p=File.debut;
        File.debut=&te;
        te.suivant=p;
    }

}

//fonction de lextraction

test extraire(file File)
{
    test *te;
    test tr;
    te=File.debut;
    if(te!=File.fin)
    {
        te=File.debut->suivant;
    }else{
        te=NULL;
        File.fin=NULL;
    }
    tr=*te;
    free(te);
    return tr;
}

////fonctions qui insere les position qui ont une possibilite de capture dans la file File

void insert_ini_pos(char board[5][9],file File,char current_player)
{

    int i,j;
    location ps;
    test tet;
    for(i=0;i<5;i++)
    {
        for(j=0;j< 9; j++){
            ps.row=i;
            ps.column=j;
            if(board[i][j]==current_player && pos_cap_pawn(ps,current_player, board)==1)
            {
                tet.p=ps;
                tet.sommet=-1;
                insert(tet,File);
            }
        }
    }
}

//////fonction qui prend en parametres le plateau du jeu, la position ,le tableau du suite des directions ainsi que son sommet et qui fait

location  update_board(char board[5][9],test te,char current_player)
{
    location ps;
    ps=te.p;
    int way;
    for(int i=0;i<=te.sommet;i++)
    {
        way=te.tab_dir[i];
        if(way<7)
        {
            way=way%8;
            eaten_pawn_abs(ps,current_player,way,board);
            dep_pawn(ps,way,board);
            ps=next_position(ps,way);
        }else{
            eaten_pawn_per(ps,current_player,way,board);
            dep_pawn(ps,way,board);
            ps=next_position(ps,way);
        }

    }
    return ps;
}

//fonction qui genere et insere la file les successeurs d'une position
void generate_succ(test te,char board[5][9],file File,char current_player)
{
    location pt=update_board(board,te,current_player);
    test t_per,t_abs;
    int lig , col,per,abs;
    lig=pt.row;
    col=pt.column;
    if((lig+col)%2==0)
    {
        for (int i = 0; i <8; i++)
        {
           per=percussion(pt,i,current_player,board);
           abs=absorption(pt,i,current_player,board);
           t_per=te;
           t_abs=te;
           if(per=1)
           {
               t_per.sommet++;
               t_per.tab_dir[t_per.sommet]=i;
               insert(t_per,File);

           }if(abs=1)
           {
               t_abs.sommet++;
               t_abs.tab_dir[t_abs.sommet]=i+8;
               insert(t_abs,File);
           }
        }
    }
    else{
        for (int i = 0; i <3; i++)
        {
           per=percussion(pt,2*i,current_player,board);
           abs=absorption(pt,2*i,current_player,board);
           t_per=te;
           t_abs=te;
           if(per=1){

               t_per.sommet++;
               t_per.tab_dir[t_per.sommet]=2*i;
               insert(t_per,File);
           }if(abs=1){
               t_abs.sommet++;
               t_abs.tab_dir[t_abs.sommet]=2*i+8;
               insert(t_abs,File);

           }
        }
    }

}

// fonction qui donne tout les possibilite des mouvements qui ont une possibilite de capture
file get_all_pos(char board[5][9],char current_player)
{
    file AllPositions,File;
    test start,TEST;
    int som=-1;
    start.sommet=-1;
    start.p.row=-1;
    start.p.column=-1;
    AllPositions.debut=&start;
    AllPositions.fin=&start;
    File.debut=NULL;
    File.fin=NULL;
    char CopyBoard[5][9];
    copy_tab(CopyBoard,board);
    insert_ini_pos(CopyBoard,AllPositions,current_player);

    while(som<9)
    {
        TEST=extraire(AllPositions);
        if(TEST.p.row=-1)
        {
            som++;
            insert(TEST,AllPositions);
        }else{
            generate_succ(TEST,CopyBoard,AllPositions,current_player);
            generate_succ(TEST,CopyBoard,AllPositions,current_player);
            copy_tab(CopyBoard,board);

        }
    }
}

//fonction qui calcule le nombre des pions mangees par percussion
int nb_per(location p,char board[height][length],int way, char current_player)
{
    location np=next_position(p, way);
    int i=0;
    while(frontier(np)==1 && occupied(np,board)==0 && board[np.row][np.column]!=current_player)
    {
        i++;
        np=next_position(np, way);
    }
    return i;
}


//fonction qui calcule le nombre des pions mangees par absorption

int nb_abs(location p,char board[height][length],int way, char current_player)
{
    int opway=(way+4)%8;
    location np=next_position(p, opway);
    int i=0;
    while(frontier(np)==1 && occupied(np,board)==0 && board[np.row][np.column]!=current_player)
    {
        i++;
        np=next_position(np, opway);
    }
    return i;
}

//fonctions qui calcule le nombre totale des pions manges dans un tour
void nbr_eaten_pawn(char board[height][length],char current_player,test tcr)
{
    char CopyBoard[5][9];
    copy_tab(CopyBoard,board);
    location pt=tcr.p;
    tcr.gain=0;
    for(int i = 0; i <=tcr.sommet;i++)
    {
        if(tcr.tab_dir[i]>7)
        {
            tcr.gain=tcr.gain+nb_abs(pt,CopyBoard,tcr.tab_dir[i]-8,current_player);
            eaten_pawn_abs(pt, current_player, tcr.tab_dir[i]-8, CopyBoard);
            dep_pawn(pt, tcr.tab_dir[i]%8, CopyBoard);
            pt = next_position(pt, tcr.tab_dir[i]-8);

        }
        else{
            tcr.gain=tcr.gain+nb_per(pt,CopyBoard,tcr.tab_dir[i],current_player);
            eaten_pawn_per(pt, current_player, tcr.tab_dir[i], CopyBoard);
            dep_pawn(pt, tcr.tab_dir[i], CopyBoard);
            pt = next_position(pt, tcr.tab_dir[i]);
        }
    }


}

///fonction qui retourne la posibilite de mouvement qui a le gain maximale

test maximum(file File)
{
    test* sbr,sbt;
    int max=0;
    for(sbr=File.debut;sbr!=NULL;sbr=sbr->suivant)
    {
        if(sbr->gain>max)
        {
            max=sbr->gain;
            sbt=*sbr;
        }
    }
    free(sbr);
    return sbt;
}

void machinemoyenne()
{
    /*code*/
}

////////////////////////////////////////main/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main(){
    int nb;

    do{
        red();
        printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        yellow();
        afficher_fichier("accueil.txt");
        red();
        printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        printf(" __________________________                                   __________________________\n");
        printf("|__________________________| ");
        yellow();
        printf("          JEU FANORONA          ");
        red();
        printf("|__________________________|\n ");
        printf("________________________________________________________________________________________\n\n");
        printf(" __________________________                                   __________________________\n");
        printf("|__________________________| ");
        yellow();
        printf("BIENVENUE DANS LE JEU E-FANORONA");
        red();
        printf("|__________________________|\n");
        printf("________________________________________________________________________________________\n\n");
        printf(" __________________________                                   __________________________\n");
        printf("|__________________________| ");
        yellow();
        printf("            LE MENU             ");
        red();
        printf("|__________________________|\n\n");
        yellow();
        printf(" \t 1- Jouer \n ");
        printf(" \t 2- Table des regles  \n ");
        printf(" \t 3- Infos  \n");
        printf("Choississez un nombre du menu ci dessus pour demarrer le jeu :\n");
        scanf("    %d",&nb);
        system("cls");
        if (nb == 2)
        {
            red();
            printf("________________________________________________________________________________________\n\n");
            yellow();
            afficher_fichier("regles.txt");
            red();
            printf("\n\n________________________________________________________________________________________\n\n");
            yellow();
            printf("\t> si vous voulez revenir au menu principale, tapez 0:\n\t> si vous voulez commencer a jouer, tapez 1:\n\t");
            scanf("%d", &nb);
            red();
            printf("________________________________________________________________________________________\n\n");
        }
        else if (nb ==3)
        {
            red();
            printf("________________________________________________________________________________________\n\n");
            yellow();
            afficher_fichier("infos.txt");
            red();
            printf("\n\n________________________________________________________________________________________\n\n");
            yellow();
            printf("\t> si vous voulez revenir au menu principale, tapez 0:\n\t> si vous voulez commencer a jouer, tapez 1:\n\t");
            scanf("%d",&nb);
            red();
            printf("________________________________________________________________________________________\n\n");
        }
        system("cls");

    }while(nb!=1);
    red();
    printf(" _______________________________________________________________________________________\n");
    printf("|_______________________________________________________________________________________|\n\n");
    yellow();
    afficher_fichier("jouer.txt");
    red();
    printf("\n\n\n _______________________________________________________________________________________\n");
    printf("|_______________________________________________________________________________________|\n\n");
    yellow();
    printf("\n       > veuillez entrer un nombre du menu si-dessous pour commencer a jouer:");
    scanf("%d",&nb);
    system("cls");
    switch(nb)
    {
        case 1:
            joueurvsjoueur();
            break;
        case 2:
            machinefacile();
            break;
        case 3:
            machinemoyenne();
            break;
    }

}
