/*
    PACMAN - TRABALHO FINAL

    UFRGS - ENGENHARIA DE COMPUTACAO
    Introducao a Algoritmos e Programacao
    DEZ/2017
    LEONARDO DROVES SILVEIRA & MATHEUS DUSSIN BAMPI
*/
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>
#include <string.h>
#include <locale.h> //Para usar o idioma PT-BR
#include <time.h> //Para gerar numero aleatorio

//Constantes
#define NUM_FANT 5 //Numero maximo de fantasmas
#define COLUNAS 101 //Numero de colunas do mapa
#define LINHAS 30 //Numero de linhas do mapa
#define ProbDirecaoFantasma 60  //Probabilidade do fantasma ir ao encontro do pacman
#define TEMP 120

//Cores
#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

int aaa;

typedef struct str_pacman
{
    int x_inicial, y_inicial; //x,y inicial no mapa
    int x, y; //x,y atual no mapa
    int past_comidas; //Numero de pastilhas comidas
    int super_past_comidas; //Numero de super-pastilhas comidas
    int vidas; //Numero de vidas do pacman
    int poder; //Boolean - se pacman esta com o poder da super-pastilhas
} Pacman;

typedef struct str_fantasma
{
    int x_inicial, y_inicial; //x,y inicial no mapa
    int x, y; //x,y atual no mapa
    char sob; //Elemento embaixo do fantasma
    int move; //Movimentacao atual do fantasma (1-cima, 2-direita, 3-baixo, 4-esquerda)
} Fantasma;

void main()
{
    setlocale(LC_ALL, "Portuguese"); //Define o idioma para o PT-BR
    SetConsoleSize(101, 45); //Define dimensoes da tela do programa.
    aaa=0;
    char map[LINHAS][COLUNAS]; //Matriz que contém o mapa do jogo.
    Pacman pacman;
    Fantasma fantasmas[NUM_FANT];
    int quant_past; //Variavel que armazena a quantidade de pastilhas total que o mapa possui
    char tecla_anterior = ' ', tecla_atual = ' '; //Variaveis que armazenam as teclas que o jogador apertar
    int pontuacao = 0; //Pontuacao do jogador
    inicio_jogo(&pacman, fantasmas, map);
    quant_past = conta_pastilhas(map); // Funçao que conta a quantidade de pastilhas que o map possui
    int paused = 0;
    int c=1;
    while((quant_past!=(pacman.past_comidas+pacman.super_past_comidas)) && (pacman.vidas>0))
    {
        le_tecla(&tecla_atual);
        paused = verifica_pause(&tecla_atual, &tecla_anterior, paused);
        if(!paused)
        {

            verifica_tecla(&tecla_atual, &tecla_anterior, &pacman, map);
            move_pacman(&pacman, &tecla_atual, &tecla_anterior, map);
            if(c==10){c=1;}
            if(1 && c%2){ move_fantasmas(&pacman, fantasmas, map);}//pacman.poder
            c++;
            gotoxy(0, 32);
            pontuacao = (pacman.past_comidas*10)+(pacman.super_past_comidas*50);
            printf("Pontuação:%d past_comidas:%d x:%d y:%d vidas:%d  c:%d",pontuacao, pacman.past_comidas, pacman.x, pacman.y, pacman.vidas, c);
            Sleep(TEMP);// tempo entre "frames"

        }
    }
    system("cls");
    printf("Pontuação Final: %d", pontuacao);
}

//FUNCOES DE MAPA
void copia_mapa(char map[][COLUNAS])
{
    int i; // Contador.
    char aux_linha[101]; // String auxiliar para copia.
    FILE *file; // Ponteiro do arquivo.
    file = fopen("labirinto3_0.txt", "r"); // Abre o arquivo .txt em modo leitura.
    if (file == NULL)  // Verifica se o arquivo é válido.
    {
        printf("Erro na leitura do arquivo!"); // Informa erro na leitura do arquivo.
        system("pause");
        exit(1);
    }
    for(i=0; i<LINHAS; i++)  // Laço que realiza a cópia do arquivo
    {
        fgets(aux_linha, 101, file);
        strcpy(map[i], aux_linha);
        fgetc(file);
    }
    fclose(file);
}

void imprime_mapa (char map[][COLUNAS])
{
    int i,j;
    for(i=0; i<LINHAS; i++)
        for(j=0; j<COLUNAS; j++)
            print_element(map[i][j]);
}

void procura_pacman(Pacman *pacman, char map[][COLUNAS])
{
    int x, y;
    for(y=0; y<LINHAS; y++)
        for(x=0; x<COLUNAS; x++)
            if((map[y][x]=='C') || (map[y][x]=='c'))
            {
                (*pacman).x_inicial = x;
                (*pacman).y_inicial = y;
                (*pacman).x = x;
                (*pacman).y = y;
            }
}

void procura_fantasmas(Fantasma fantasmas[], char map[][COLUNAS])
{
    int x, y;
    int i = 0;
    for(y=0; y<LINHAS; y++)
        for(x=0; x<COLUNAS; x++)
            if((map[y][x]=='W') || (map[y][x]=='w'))
            {
                fantasmas[i].x_inicial = x;
                fantasmas[i].y_inicial = y;
                fantasmas[i].x = x;
                fantasmas[i].y = y;
                i++;
            }
}

int conta_pastilhas(char mat[][COLUNAS])
{
    int quant_partilhas = 0;
    int x, y;

    for(y=0; y<LINHAS; y++)
        for(x=0; x<COLUNAS; x++)
            if((mat[y][x]=='o') || (mat[y][x]=='*'))
                quant_partilhas++;

    return quant_partilhas;
}

void print_element(char c)
{
    switch(c)
    {
    case 'C':
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), YELLOW | FOREGROUND_INTENSITY);
        break;
    case '#':
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY );
        break;
    case 'W':
    case 'w':
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY );
        break;
    default:
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), WHITE | FOREGROUND_INTENSITY);
    }
    printf("%c", c);
}


//FUNCOES DE JOGO
void verifica_tecla(char *tecla_atual, char *tecla_anterior, Pacman *pacman, char map[][COLUNAS])
{
    int trocou = 0;

    if(*tecla_atual == 's')
        *tecla_anterior = *tecla_atual;
    else if((*tecla_atual=='w' && map[((*pacman).y)-1][(*pacman).x] != '#') ||
            (*tecla_atual=='d' && map[(*pacman).y][((*pacman).x)+1] != '#') ||
            (*tecla_atual=='a' && map[(*pacman).y][((*pacman).x)-1] != '#') ||
            (*tecla_atual=='x' && map[((*pacman).y)+1][(*pacman).x] != '#'))
    {
        *tecla_anterior = *tecla_atual;
        trocou = 1;
    }
    if(!trocou)
    {
        //confere segunda opc
        if((*tecla_anterior=='w' && map[((*pacman).y)-1][(*pacman).x] == '#') ||
                (*tecla_anterior=='d' && map[(*pacman).y][((*pacman).x)+1] == '#') ||
                (*tecla_anterior=='a' && map[(*pacman).y][((*pacman).x)-1] == '#') ||
                (*tecla_anterior=='x' && map[((*pacman).y)+1][(*pacman).x] == '#'))
        {
            *tecla_anterior = 's';
        }
    }
}

void verifica_item(Pacman *pacman, char map[][COLUNAS])
{
    char verif = map[(*pacman).y][(*pacman).x];
    switch (verif)
    {
    case 'o':
        (*pacman).past_comidas+=1;
        break;

    case '*':
        (*pacman).super_past_comidas+=1;
        break;

    case 'W':
        (*pacman).vidas-=1;
        //chamar o reset()
        break;

    case 'w':

        break;
    }
    map[(*pacman).y][(*pacman).x] = ' ';
}

void move_pacman(Pacman *pacman, char *tecla_atual, char *tecla_anterior, char map[][COLUNAS])
{
    switch(*tecla_anterior)
    {
    case 'w':
        gotoxy((*pacman).x,((*pacman).y)-1);
        print_element('C');
        gotoxy((*pacman).x,(*pacman).y);
        printf(" ");
        (*pacman).y-=1;
        break;
    case 'x':
        gotoxy((*pacman).x,((*pacman).y)+1);
        print_element('C');
        gotoxy((*pacman).x,(*pacman).y);
        printf(" ");
        (*pacman).y+=1;
        break;
    case 'a':
        gotoxy(((*pacman).x)-1,(*pacman).y);
        print_element('C');
        gotoxy((*pacman).x,(*pacman).y);
        printf(" ");
        (*pacman).x-=1;
        break;
    case 'd':
        gotoxy(((*pacman).x)+1,(*pacman).y);
        print_element('C');
        gotoxy((*pacman).x,(*pacman).y);
        printf(" ");
        (*pacman).x+=1;
        break;
    case 's':
        gotoxy((*pacman).x,(*pacman).y);
        print_element('C');
        break;
    }
    verifica_item(pacman, map);
}

void move_fantasmas(Pacman *pacman, Fantasma fantasmas[], char map[][COLUNAS])
{
    int i;
    int movex, movey; // Movimento do pacman na direcao x/y;
    int dir; // 1 para x; 0 para y
    int decisao; // Boolean para saber se fantasma precisa tomar decisao ou nao
    srand(time(NULL));
    for(i=0; i<NUM_FANT; i++)   // Repete isso para todos os fantasmas
    {
        if(verifica_decisao(&(fantasmas[i]), map))   // Se o fantasma estiver em bifurcacao ou tiver obstaculo na frente
        {
            // Ve direcao para alcancar fantasma
            if(fantasmas[i].x < (*pacman).x)
                movex = 1; // Vai para direita
            else if(fantasmas[i].x > (*pacman).x)
                movex = -1; // Vai para esquerda
            else
                movex = 0;
            if(fantasmas[i].y < (*pacman).y)
                movey = 1; // Vai para baixo
            else if(fantasmas[i].y > (*pacman).y)
                movey = -1; // Vai para cima
            else
                movey = 0;
            // Decide proximo movimento
            int r_direcao = rand()%100;// Gera numero aleatorio para probabilidade de andar na direcao X ou Y
            int r_prob = rand()%100;// Gera numero aleatorio para probabilidade de andar na direcao do fantasma
            gotoxy(0, 37);
            printf("r_prob: %d, r_direcao: %d", r_prob, r_direcao);
            dir = -1; //Para verificacao de atribuicao de dir, movex, movey
            if(r_prob < ProbDirecaoFantasma)  //Segue fantasma
            {
                if(movex==0)
                {
                    if(map[fantasmas[i].y+movey][fantasmas[i].x] != '#')
                        dir = 0;
                }
                else if(movey==0)
                {
                    if(map[fantasmas[i].y][fantasmas[i].x+movex] != '#')
                        dir = 1;
                }
                else
                {
                    if(r_direcao >= 50)  //prioriza o X
                    {
                        if(map[fantasmas[i].y][fantasmas[i].x+movex] != '#')
                            dir = 1;
                        else if(map[fantasmas[i].y+movey][fantasmas[i].x] != '#')
                            dir = 0;
                    }
                    else   //prioriza o Y
                    {
                        if(map[fantasmas[i].y+movey][fantasmas[i].x] != '#')
                            dir = 0;
                        else if(map[fantasmas[i].y][fantasmas[i].x+movex] != '#')
                            dir = 1;
                    }
                }
            }
            if(dir==-1)   //Move pra direcao aleatoria
            {
                srand(time(NULL));
                int move_rand, dir_rand;
                do move_rand = (rand()%3)-1;
                while(move_rand == 0); //Gera move_rand (1 ou -1)
                movex = move_rand;
                movey = move_rand;
                dir_rand = (rand()%2);
                gotoxy(0, 35);
                printf("movex: %d, movey: %d, dir randomico: %d, rodada: %d",movex, movey, dir, aaa);
                //Direcao x
                if(map[fantasmas[i].y][fantasmas[i].x+movex] != '#')
                {
                    dir = 1;
                }
                else if(map[fantasmas[i].y][fantasmas[i].x-movex] != '#')
                {
                    movex=-movex;
                    dir = 1;
                }
                if(dir_rand==0 || dir == -1)
                {
                    if(map[fantasmas[i].y+movey][fantasmas[i].x] != '#')
                    {
                        dir = 0;
                    }
                    else if(map[fantasmas[i].y-movey][fantasmas[i].x] != '#')
                    {
                        movey=-movey;
                        dir = 0;
                    }
                }
            }
        }
        else      //Segue movimento que estava fazendo
        {
            switch(fantasmas[i].move)
            {
            case 1:
                movey = -1;
                dir = 0;
                break;
            case 2:
                movex = 1;
                dir = 1;
                break;
            case 3:
                movey = 1;
                dir = 0;
                break;
            case 4:
                movex = -1;
                dir = 1;
                break;
            }
        }
        //Imprime o fantasma no movimento ja atribuido
        gotoxy(0, 37+aaa);
        printf("\n| FANTASMA -> x: %d | y: %d | movex: %d | movey: %d | dir: %d | ", fantasmas[i].x, fantasmas[i].y, movex, movey, dir);

        gotoxy((fantasmas[i]).x,(fantasmas[i]).y);
        print_element(fantasmas[i].sob); //Imprime elemento que estava embaixo do fantasma
        map[fantasmas[i].y][fantasmas[i].x] = fantasmas[i].sob; //Atualiza no map[] elemento que estava embaixo do fantasma

        if(dir==1)  //Move o x
        {
            fantasmas[i].x += movex;
            if(movex==1)
                fantasmas[i].move = 2;
            else
                fantasmas[i].move = 4;
        }
        else if(dir==0)   //Move o y
        {
            fantasmas[i].y += movey;
            if(movey==1)
                fantasmas[i].move = 3;
            else
                fantasmas[i].move = 1;
        }
        if(map[fantasmas[i].y][fantasmas[i].x] == 'W')
        {
            int j;
            for(j=0; j<NUM_FANT; j++)
            {
                if(j!=i && fantasmas[i].x == fantasmas[j].x && fantasmas[i].y == fantasmas[j].y)
                    fantasmas[i].sob = fantasmas[j].sob;
            }
        }
        else
        {
            fantasmas[i].sob = map[fantasmas[i].y][fantasmas[i].x]; //Armazena elemento que estara embaixo do fantasma
        }
        map[fantasmas[i].y][fantasmas[i].x] = 'W'; //Atualiza posicao do fantasma no map[]
        gotoxy(fantasmas[i].x, fantasmas[i].y);
        print_element('W'); //Imprime fantasma
    }
}

int verifica_decisao(Fantasma *fantasma, char map[][COLUNAS])
{
    int cont = 0, decisao = 0;
    if((*fantasma).move==0) decisao = 1;
    if(!decisao)
    {
        //Verifica bifurcacao
        if(map[(*fantasma).y][(*fantasma).x+1] != '#')
            cont++;
        if(map[(*fantasma).y][(*fantasma).x-1] != '#')
            cont++;
        if(map[(*fantasma).y+1][(*fantasma).x] != '#')
            cont++;
        if(map[(*fantasma).y-1][(*fantasma).x] != '#')
            cont++;
        if(cont>=3)
            decisao = 1;
        if(!decisao)
        {
            //Verifica obstaculo
            switch((*fantasma).move)
            {
            case 1: //cima
                if(map[(*fantasma).y-1][(*fantasma).x] == '#')
                    decisao = 1;
                break;
            case 2: //direita
                if(map[(*fantasma).y][(*fantasma).x+1] == '#')
                    decisao = 1;
                break;
            case 3: //baixo
                if(map[(*fantasma).y+1][(*fantasma).x] == '#')
                    decisao = 1;
                break;
            case 4: //esquerda
                if(map[(*fantasma).y][(*fantasma).x-1] == '#')
                    decisao = 1;
                break;
            }
        }
    }
    return decisao;
}


//FUNCOES DE CONTROLE DE SISTEMA
void le_tecla(char *tecla)
{
    char caracter = *tecla;

    while(_kbhit())
    {
        caracter = getch();
        caracter = tolower(caracter);
        if ((caracter == 'w') || (caracter == 's') || (caracter == 'a') || (caracter == 'd') || (caracter == 'x') || (caracter == 'p'))
            *tecla = caracter;
    }
}

int verifica_pause(char *tecla_atual, char *tecla_anterior, int paused)
{
    if(*tecla_atual=='p')
    {
        if(!paused)
            *tecla_atual=' ';
        else if(paused)
            *tecla_atual = *tecla_anterior;
        paused = !paused;
    }
    return paused;
}

void inicio_jogo(Pacman *pacman, Fantasma fantasmas[], char map[][COLUNAS])
{
    (*pacman).past_comidas = 0;
    (*pacman).super_past_comidas = 0;
    (*pacman).vidas = 3;
    copia_mapa(map); // Funcao que copia o mapa do arquivo .txt para a matriz "map".
    imprime_mapa(map); // Imprime no monitor a matriz "map".
    procura_pacman(pacman, map);
    procura_fantasmas(fantasmas, map);
    map[(*pacman).y][(*pacman).x] = ' ';
    int i;
    for(i=0; i<NUM_FANT; i++)
    {
        fantasmas[i].sob = ' ';
        fantasmas[i].move = 0;
    }
}

void reset(Fantasma fantasmas[], char map[][COLUNAS], Pacman *pacman)
{
    int i;
    for(i=0; i<NUM_FANT; i++)
    {
        map[fantasmas[i].y][fantasmas[i].x] = fantasmas[i].sob;
        fantasmas[i].sob = ' ';
        fantasmas[i].x = fantasmas[i].x_inicial;
        fantasmas[i].y = fantasmas[i].y_inicial;
        fantasmas[i].move = 0;
        map[fantasmas[i].y][fantasmas[i].x] = 'W';
    }
    (*pacman).x = (*pacman).x_inicial;
    (*pacman).y = (*pacman).y_inicial;
    gotoxy((*pacman).x, (*pacman).y);
    print_element('C');
}

void gotoxy(int x, int y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD)
    {
        x,y
    });
}

void SetConsoleSize(unsigned largura, unsigned altura)
{
    SMALL_RECT r;
    COORD c;
    HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    c.X = largura;
    c.Y = altura;
    SetConsoleScreenBufferSize(stdoutHandle, c); //Ajusta tamanho do buffer de armazenamento da tela

    r.Left = 0;
    r.Top = 0;
    r.Right = largura - 1;
    r.Bottom = altura - 1;
    SetConsoleWindowInfo(stdoutHandle, 1, &r); //Ajusta tamanho da tela
}
