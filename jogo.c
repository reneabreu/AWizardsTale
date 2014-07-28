/*
    Universidade Federal Rural do Rio de Janeiro
    Instituto Multidisciplinar
    Curso de Ciência da Computacão
    Alunos: Felipe Palomino e René Abreu
    Professor: Filipe Braida
 */
 
/* ***************** Bibliotecas ***************** */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

/* ***************** Definicões, para facilitar o uso da função para desenhar o mapa ***************** */
#define PAREDE 0
#define SAIDA 1
#define CHAO 2
#define CAIXA 3
#define RANDOM 4
#define HUD 10

 
/* ***************** Definições da configuração do Video ***************** */
#define LARGURA 850 /* Numero total de colunas*largura_da_parede */
#define ALTURA  700 /* Numero total de linhas*altura_da_parede */
#define BPP       0 /* A Flag SDL_ANYFORMAT se encaregara da resolucao */
 
#define VELCHAR 12 /* Velocidade de aceleração player */

/* ***************** Definições de corte do sprite ***************** */
#define FRAME1_X 0
#define FRAME2_X (FRAME1_X + PLAYER_W)
#define FRAME3_X (FRAME1_X + (PLAYER_W * 2 ))
#define FRAME4_X (FRAME1_X + (PLAYER_W * 3 ))

#define FRAME1_Y 300
#define FRAME2_Y (FRAME1_Y + PLAYER_H)
#define FRAME3_Y (FRAME1_Y + (PLAYER_H * 2 ))
#define FRAME4_Y (FRAME1_Y + (PLAYER_H * 3 ))

#define INIMIGOF1_X 0
#define INIMIGOF2_X (INIMIGOF1_X + INIMIGO_W)
#define INIMIGOF3_X (INIMIGOF1_X + (INIMIGO_W * 2 ))
#define INIMIGOF4_X (INIMIGOF1_X + (INIMIGO_W * 3 ))

#define INIMIGOF1_Y 0
#define INIMIGOF2_Y (INIMIGOF1_Y + INIMIGO_H)
#define INIMIGOF3_Y (INIMIGOF1_Y + (INIMIGO_H * 2 ))
#define INIMIGOF4_Y (INIMIGOF1_Y + (INIMIGO_H * 3 ))


#define LADO_QUAD 50

#define PLAYER_H 50
#define PLAYER_W 33

#define INIMIGO_H 38
#define INIMIGO_W 32
 
/* ***************** Criando as Surfaces ***************** */
SDL_Surface * tela, * parede, * player, * bomba, * chao, * segundos, *minutos, *caixa, *hud, *telaMenu1, *telaMenu2, *telaMenu3, *comoJogar, *telaPause1, *telaPause2, *telaPause3, *spritesheetInimigos, *telaGameOver1, *telaGameOver2, *numerosPontos, *numerosVidas;
 
/* ***************** Variáveis Globais ***************** */
int tempodejogo[3], numAleatorio1 = 0, numAleatorio2 = 0, numAleatorio3 = 0, numAleatorio4 = 0, musicajogo = 0, mapa[14][17], tempoMorte, vidainimigo1 = 1, vidainimigo2 = 1, vidainimigo3 = 1, vidainimigo4 = 1, caixaexiste[100], coluna_atual = 1, linha_atual = 3, fim = 0, i =0, r = 0, bomb = 0, tamanhoFogo = 2, vida =  3, botaoMenu = 1, sairMenu = 0, botaoPause = 1, sairGameOver = 1, sairPause = 1, ladoInimigo = 2, fase = 1, telahighscore, reset = 1, pontos = 0;
int aumentaPontos = 0;
/* ***************** Rects Globais ***************** */
SDL_Rect paredeRECT[96], caixaRECT[100], saidaRECT, destinoBomba, destinoExplosao[6], opcaoMenu, opcaoMenu2, destinoMenu = {0,0,0,0}, destinoInimigo = {750, 600, INIMIGO_W + 10, INIMIGO_H}, destinoInimigo2 = {650, 400, INIMIGO_W + 10, INIMIGO_H}, destinoInimigo3 = {450, 300, INIMIGO_W + 10, INIMIGO_H}, destinoInimigo4 = {300, 500, INIMIGO_W + 10, INIMIGO_H}, destinoPontos, dest = {50,100,PLAYER_W,PLAYER_H};
SDL_Rect blitdestino;

/* ***************** Músicas e Efeitos ***************** */
Mix_Music *somJogo, *morteSom;
Mix_Chunk *explosaoSom, *morteInimigo;

/* ***************** Fontes ***************** */
TTF_Font *fonte;

/* ***************** Função para controlar fps ***************** */
void controla_fps(int tempo_inicial)
{
    int fps = 1000/60; // converte 60 FPS para milissegundos
    int tempo_agora = SDL_GetTicks() - tempo_inicial;
 
    if(tempo_agora < fps)
    {
   		SDL_Delay(fps - tempo_agora);
    }
}
 
/* ***************** Função para carregar imagens ***************** */
int carregar_arquivos()
{
    player = IMG_Load("imagens/spritesheet.png");
    if(player == NULL)
    {
        printf("Não carregou spritesheet.");
        return 0;
    }
	
	hud = IMG_Load("imagens/hud.png");
    if(hud == NULL)
    {
        printf("Não carregou hud.");
        return 0;
    }
	
	telaMenu1 = IMG_Load("imagens/menu1.png");
    if(telaMenu1 == NULL)
    {
        printf("Não carregou menu.");
        return 0;
    }
	
	telaMenu2 = IMG_Load("imagens/menu2.png");
    if(telaMenu2 == NULL)
    {
        printf("Não carregou menu.");
        return 0;
    }
	
	telaMenu3 = IMG_Load("imagens/menu3.png");
    if(telaMenu3 == NULL)
    {
        printf("Não carregou menu.");
        return 0;
    }
	
	telaPause1 = IMG_Load("imagens/menupause1.png");
    if(telaPause1 == NULL)
    {
        printf("Não carregou pause.");
        return 0;
    }
	
	telaPause2 = IMG_Load("imagens/menupause2.png");
    if(telaPause2 == NULL)
    {
        printf("Não carregou pause.");
        return 0;
    }
	
	telaPause3 = IMG_Load("imagens/menupause3.png");
    if(telaPause3 == NULL)
    {
        printf("Não carregou pause.");
        return 0;
    }
	
	spritesheetInimigos = IMG_Load("imagens/spritesheetInimigos.png");
    if(spritesheetInimigos == NULL)
    {
        printf("Não carregou botão menu.");
        return 0;
    }
	telaGameOver1 = IMG_Load("imagens/gameOver1.png");
    if(telaGameOver1 == NULL)
    {
        printf("Não carregou Game Over.");
        return 0;
    }
	telaGameOver2 = IMG_Load("imagens/gameOver2.png");
    if(telaGameOver2 == NULL)
    {
        printf("Não carregou Game Over.");
        return 0;
    }
	
	comoJogar = IMG_Load("imagens/comoJogar.png");
    if(comoJogar == NULL)
    {
        printf("Não carregou como jogar.");
        return 0;
    }
	
	/* Carregar audiio */ 
	explosaoSom = Mix_LoadWAV("sons/explosao.wav");
	if(explosaoSom == NULL)
    {
        printf("Não carregou som explosão.");
        return 0;
    }

    morteSom = Mix_LoadMUS("sons/morte.wav");
    if(morteSom == NULL)
    {
    	printf("Não carregou som morte.");
    	return 0;
    }

    morteInimigo = Mix_LoadWAV("sons/morteinimigo.wav");
    if(morteInimigo == NULL)
    {
    	printf("Não carregou som morte inimigo.");
    	return 0;
    }

    somJogo = Mix_LoadMUS("sons/somjogo.wav");
    if(somJogo == NULL)
    {
    	printf("Não carregou som jogo.");
    	return 0;
    }
	
	/* Carregar fontes */ 
	
	fonte = TTF_OpenFont("fonte/font.ttf",50);
 	if(fonte == NULL)
    {
    	printf("Não carregou a fonte.");
    	return 0;
    }
	
 	return 1;
}

/* ***************** Função para cortar o spritesheet ***************** */
 
/* Rect para corte do sprite */
SDL_Rect spritesheet[40], spriteInimigos [12];

void cortarSpriteSheet()
{
	/* Posições de corte do player */ 
    //Baixo(parado)
    spritesheet[ 0 ].x = FRAME1_X;
    spritesheet[ 0 ].y = FRAME1_Y;
    spritesheet[ 0 ].w = PLAYER_W;
    spritesheet[ 0 ].h = PLAYER_H;
 
    //Baixo 1
    spritesheet[ 1 ].x = FRAME2_X;
    spritesheet[ 1 ].y = FRAME1_Y;
    spritesheet[ 1 ].w = PLAYER_W;
    spritesheet[ 1 ].h = PLAYER_H;
 
    //Baixo 2
    spritesheet[ 2 ].x = FRAME3_X;
    spritesheet[ 2 ].y = FRAME1_Y;
    spritesheet[ 2 ].w = PLAYER_W;
    spritesheet[ 2 ].h = PLAYER_H;
 
    //Baixo 3
    spritesheet[ 3 ].x = FRAME4_X;
    spritesheet[ 3 ].y = FRAME1_Y;
    spritesheet[ 3 ].w = PLAYER_W;
    spritesheet[ 3 ].h = PLAYER_H;
	
	//Baixo 4
    spritesheet[ 4 ].x = FRAME1_X;
    spritesheet[ 4 ].y = FRAME1_Y;
    spritesheet[ 4 ].w = PLAYER_W;
    spritesheet[ 4 ].h = PLAYER_H;
	
	//Cima (parado)
    spritesheet[ 5 ].x = FRAME1_X;
    spritesheet[ 5 ].y = FRAME4_Y;
    spritesheet[ 5 ].w = PLAYER_W;
    spritesheet[ 5 ].h = PLAYER_H;
 
    //Cima 1
    spritesheet[ 6 ].x = FRAME2_X;
    spritesheet[ 6 ].y = FRAME4_Y;
    spritesheet[ 6 ].w = PLAYER_W;
    spritesheet[ 6 ].h = PLAYER_H;
 
    //Cima 2
    spritesheet[ 7 ].x = FRAME3_X;
    spritesheet[ 7 ].y = FRAME4_Y;
    spritesheet[ 7 ].w = PLAYER_W;
    spritesheet[ 7 ].h = PLAYER_H;
 
    //Cima 3
    spritesheet[ 8 ].x = FRAME4_X;
    spritesheet[ 8 ].y = FRAME4_Y;
    spritesheet[ 8 ].w = PLAYER_W;
    spritesheet[ 8 ].h = PLAYER_H;
	
	//Cima 4
	spritesheet[ 9 ].x = FRAME1_X;
    spritesheet[ 9 ].y = FRAME4_Y;
    spritesheet[ 9 ].w = PLAYER_W;
    spritesheet[ 9 ].h = PLAYER_H;
	
	//Direita (parado)
    spritesheet[ 10 ].x = FRAME1_X;
    spritesheet[ 10 ].y = FRAME3_Y;
    spritesheet[ 10 ].w = PLAYER_W;
    spritesheet[ 10 ].h = PLAYER_H;
 
    //Direita 1
    spritesheet[ 11 ].x = FRAME2_X;
    spritesheet[ 11 ].y = FRAME3_Y;
    spritesheet[ 11 ].w = PLAYER_W;
    spritesheet[ 11 ].h = PLAYER_H;
 
    //Direita 2
    spritesheet[ 12 ].x = FRAME3_X;
    spritesheet[ 12 ].y = FRAME3_Y;
    spritesheet[ 12 ].w = PLAYER_W;
    spritesheet[ 12 ].h = PLAYER_H;
 
    //Direita 3
    spritesheet[ 13 ].x = FRAME4_X;
    spritesheet[ 13 ].y = FRAME3_Y;
    spritesheet[ 13 ].w = PLAYER_W;
    spritesheet[ 13 ].h = PLAYER_H;
	
	//Direita 4
    spritesheet[ 14 ].x = FRAME1_X;
    spritesheet[ 14 ].y = FRAME3_Y;
    spritesheet[ 14 ].w = PLAYER_W;
    spritesheet[ 14 ].h = PLAYER_H;
	
	//Esquerda (parado)
    spritesheet[ 16 ].x = FRAME1_X;
    spritesheet[ 16 ].y = FRAME2_Y;
    spritesheet[ 16 ].w = PLAYER_W;
    spritesheet[ 16 ].h = PLAYER_H;
 
    //Esquerda 1
    spritesheet[ 17 ].x = FRAME2_X;
    spritesheet[ 17 ].y = FRAME2_Y;
    spritesheet[ 17 ].w = PLAYER_W;
    spritesheet[ 17 ].h = PLAYER_H;
 
    //Esquerda 2
    spritesheet[ 18 ].x = FRAME3_X;
    spritesheet[ 18 ].y = FRAME2_Y;
    spritesheet[ 18 ].w = PLAYER_W;
    spritesheet[ 18 ].h = PLAYER_H;
 
    //Esquerda 3
    spritesheet[ 19 ].x = FRAME4_X;
    spritesheet[ 19 ].y = FRAME2_Y;
    spritesheet[ 19 ].w = PLAYER_W;
    spritesheet[ 19 ].h = PLAYER_H;
	
	//Esquerda 4
    spritesheet[ 20 ].x = FRAME1_X;
    spritesheet[ 20 ].y = FRAME2_Y;
    spritesheet[ 20 ].w = PLAYER_W;
    spritesheet[ 20 ].h = PLAYER_H;
	
	/* Corte de Objetos do mapa */
	//Parede
	spritesheet[ 21 ].x = 200;
    spritesheet[ 21 ].y = 0;
    spritesheet[ 21 ].w = LADO_QUAD;
    spritesheet[ 21 ].h = LADO_QUAD;
	
	//Chão
	spritesheet[ 22 ].x = 250;
    spritesheet[ 22 ].y = 0;
    spritesheet[ 22 ].w = LADO_QUAD;
    spritesheet[ 22 ].h = LADO_QUAD;
	
	//Caixa
	spritesheet[ 23 ].x = 300;
    spritesheet[ 23 ].y = 0;
    spritesheet[ 23 ].w = LADO_QUAD;
    spritesheet[ 23 ].h = LADO_QUAD;
	
	//Bomba
	spritesheet[ 24 ].x = 200;
    spritesheet[ 24 ].y = 50;
    spritesheet[ 24 ].w = LADO_QUAD;
    spritesheet[ 24 ].h = LADO_QUAD;
	
	//Explosão Centro
	spritesheet[ 25 ].x = 300;
    spritesheet[ 25 ].y = 250;
    spritesheet[ 25 ].w = LADO_QUAD;
    spritesheet[ 25 ].h = LADO_QUAD;
	
	//Explosão Centro Direita-Esquerda
	spritesheet[ 26 ].x = 350;
    spritesheet[ 26 ].y = 250;
    spritesheet[ 26 ].w = LADO_QUAD;
    spritesheet[ 26 ].h = LADO_QUAD;
	
	//Explosão Centro Cima-Baixo
	spritesheet[ 27 ].x = 300;
    spritesheet[ 27 ].y = 200;
    spritesheet[ 27 ].w = LADO_QUAD;
    spritesheet[ 27 ].h = LADO_QUAD;
	
	//Explosão Ponta Direita
	spritesheet[ 28 ].x = 400;
    spritesheet[ 28 ].y = 250;
    spritesheet[ 28 ].w = LADO_QUAD;
    spritesheet[ 28 ].h = LADO_QUAD;
	
	//Explosão Ponta Esquerda
	spritesheet[ 29 ].x = 200;
    spritesheet[ 29 ].y = 250;
    spritesheet[ 29 ].w = LADO_QUAD;
    spritesheet[ 29 ].h = LADO_QUAD;
	
	//Explosão Ponta Cima
	spritesheet[ 30 ].x = 300;
    spritesheet[ 30 ].y = 150;
    spritesheet[ 30 ].w = LADO_QUAD;
    spritesheet[ 30 ].h = LADO_QUAD;
	
	//Explosão Ponta Baixo
	spritesheet[ 31 ].x = 300;
    spritesheet[ 31 ].y = 350;
    spritesheet[ 31 ].w = LADO_QUAD;
    spritesheet[ 31 ].h = LADO_QUAD;
	
	//Saída - Fase 1
	spritesheet[ 32 ].x = 250;
    spritesheet[ 32 ].y = 50;
    spritesheet[ 32 ].w = LADO_QUAD;
    spritesheet[ 32 ].h = LADO_QUAD;
	
	//Chão - Fase 2
	spritesheet[ 33 ].x = 350;
    spritesheet[ 33 ].y = 0;
    spritesheet[ 33 ].w = LADO_QUAD;
    spritesheet[ 33 ].h = LADO_QUAD;
	
	//Chão - Fase 3
	spritesheet[ 34 ].x = 400;
    spritesheet[ 34 ].y = 0;
    spritesheet[ 34 ].w = LADO_QUAD;
    spritesheet[ 34 ].h = LADO_QUAD;
	
	//Chão - Fase 4
	spritesheet[ 35 ].x = 450;
    spritesheet[ 35 ].y = 0;
    spritesheet[ 35 ].w = LADO_QUAD;
    spritesheet[ 35 ].h = LADO_QUAD;
	
	//Chão - Fase 4
	spritesheet[ 35 ].x = 450;
    spritesheet[ 35 ].y = 0;
    spritesheet[ 35 ].w = LADO_QUAD;
    spritesheet[ 35 ].h = LADO_QUAD;
	
	//Saída - Fase 2
	spritesheet[ 36 ].x = 350;
    spritesheet[ 36 ].y = 50;
    spritesheet[ 36 ].w = LADO_QUAD;
    spritesheet[ 36 ].h = LADO_QUAD;
	
	//Saída - Fase 3
	spritesheet[ 37 ].x = 400;
    spritesheet[ 37 ].y = 50;
    spritesheet[ 37 ].w = LADO_QUAD;
    spritesheet[ 37 ].h = LADO_QUAD;
	
	//Saída - Fase 4
	spritesheet[ 38 ].x = 450;
    spritesheet[ 38 ].y = 50;
    spritesheet[ 38 ].w = LADO_QUAD;
    spritesheet[ 38 ].h = LADO_QUAD;
	
	//Bomba - Centro D
	spritesheet[ 39 ].x = 450;
    spritesheet[ 39 ].y = 50;
    spritesheet[ 39 ].w = LADO_QUAD;
    spritesheet[ 39 ].h = LADO_QUAD;
	
	
	/* INIMIGOS */
	// Olho - Cima
	spriteInimigos[ 0 ].x = INIMIGOF1_X;
    spriteInimigos[ 0 ].y = INIMIGOF1_Y;
    spriteInimigos[ 0 ].w = INIMIGO_W;
    spriteInimigos[ 0 ].h = INIMIGO_H;
	
	// Olho - Baixo
	spriteInimigos[ 1 ].x = INIMIGOF1_X;
    spriteInimigos[ 1 ].y = INIMIGOF3_Y;
    spriteInimigos[ 1 ].w = INIMIGO_W;
    spriteInimigos[ 1 ].h = INIMIGO_H;
	
	// Olho - Direita
	spriteInimigos[ 2 ].x = INIMIGOF1_X;
    spriteInimigos[ 2 ].y = INIMIGOF4_Y;
    spriteInimigos[ 2 ].w = INIMIGO_W;
    spriteInimigos[ 2 ].h = INIMIGO_H;
	
	// Olho - Esquerda
	spriteInimigos[ 3 ].x = INIMIGOF1_X;
    spriteInimigos[ 3 ].y = INIMIGOF2_Y;
    spriteInimigos[ 3 ].w = INIMIGO_W;
    spriteInimigos[ 3 ].h = INIMIGO_H;
}
	
/* ***************** Função para blitar a imagem cortada ***************** */
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* spritesheet)
{
	//Holds offsets
	SDL_Rect offset;
 
	//Get offsets
	offset.x = x;
	offset.y = y;
 
	//Blit
	SDL_BlitSurface( source, spritesheet, destination, &offset );
}					
FILE * faseAleatoria;
void mapaAleatorio()
{
	faseAleatoria = fopen("mapas/aleatorio.map", "w+");
	int linha, coluna, numrandom, temSaida;
	
	/* Converter os valores do arquivo para uma matriz */ 
    for (linha = 0; linha < 14; linha++)
    {
        for (coluna = 0; coluna < 17; coluna++)
        {
				if(linha == 0 && coluna == 0)
					fprintf(faseAleatoria, "10 ");
				else if(linha == 0)
					fprintf(faseAleatoria, "9 ");
				else if(linha == 1)
					fprintf(faseAleatoria, "0 ");
				else if(linha == 2 && coluna == 1)
					fprintf(faseAleatoria, "2 ");
				else if(linha == 2 && coluna == 2)
					fprintf(faseAleatoria, "2 ");
				else if(linha == 3 && coluna == 1)
					fprintf(faseAleatoria, "2 ");
				else if(linha == 13)
					fprintf(faseAleatoria, "0 ");
				else if(coluna == 0)
					fprintf(faseAleatoria, "0 ");
				else if(coluna == 16)
					fprintf(faseAleatoria, "0 ");
				else
				{
				numrandom = rand() % 4;
					
				fprintf(faseAleatoria, "%d ",numrandom);
				} 
				if(coluna == 16)
					fprintf(faseAleatoria, "\n");
		}
	}
	rewind(faseAleatoria);
	fclose(faseAleatoria);
}

/* ***************** Função para desenhar o mapa e definir os rects para colisão ***************** */
void desenha_mapa()
{
    SDL_Rect destino;
	
	/* Abrir o mapa de um arquivo externo */
	FILE * arquivo;
	if (fase == 1)
		arquivo = fopen("mapas/fase1.map","r");
	if (fase == 2)
		arquivo = fopen("mapas/fase2.map","r");
	if (fase == 3)
		arquivo = fopen("mapas/fase3.map","r");
	if (fase == 4)
		arquivo = fopen("mapas/fase4.map","r");
	
    int linha, coluna, contParede = 0, contCaixa = 0, numrandom;
	
	/* Converter os valores do arquivo para uma matriz */ 
    for (linha = 0; linha < 14; linha++)
    {
        destino.y = (linha) * 50; //Definir a posição do blit da imagem
	
        for (coluna = 0; coluna < 17; coluna++)
        {
            destino.x = coluna * 50; //Definir a posição do blit da imagem
			
			fscanf(faseAleatoria,"%d", &mapa[linha][coluna]); //Ler os dados do arquivo
			
            if (mapa[linha][coluna] == PAREDE)
            {
				paredeRECT[contParede].x = destino.x + 10;
				paredeRECT[contParede].y = destino.y + 5;
				paredeRECT[contParede].w = 40;
				paredeRECT[contParede].h = 40;
				
				/* blitar chão de acordo com a fase */
				if(fase == 1)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 22 ] );
				else if(fase == 2)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 33 ] );
				else if(fase == 3)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 34 ] );
				else if(fase == 4)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 35 ] );
				
				apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 21 ] );
				contParede++;
            }
            else if (mapa[linha][coluna] == CHAO)
            {
				/* blitar chão de acordo com a fase */
				if(fase == 1)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 22 ] );
				else if(fase == 2)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 33 ] );
				else if(fase == 3)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 34 ] );
				else if(fase == 4)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 35 ] );
            }
			else if (mapa[linha][coluna] == SAIDA)
            {
				caixaRECT[contCaixa].x = destino.x + 10;
				caixaRECT[contCaixa].y = destino.y + 5;
				caixaRECT[contCaixa].w = 40;
				caixaRECT[contCaixa].h = 40;
				
				/* blitar chão de acordo com a fase */
                if(fase == 1)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 22 ] );
				else if(fase == 2)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 33 ] );
				else if(fase == 3)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 34 ] );
				else if(fase == 4)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 35 ] );
				
				if(caixaexiste[contCaixa] == 0)
				{
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 23 ] );
				}
				if(caixaexiste[contCaixa] == 1)
				{
				/*Sumir com a caixa */ 
				caixaRECT[contCaixa].x = 0;
				caixaRECT[contCaixa].y = 0;
				caixaRECT[contCaixa].w = 0;
				caixaRECT[contCaixa].h = 0;
				
				/* Criar saída após a caixa deixar de existir */
				saidaRECT.x = destino.x + 10;
				saidaRECT.y = destino.y + 5;
				saidaRECT.w = 35;
				saidaRECT.h = 35;
				
				/* Blitar a saída de acordo com a fase*/
				if(fase == 1)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 32 ] );
				else if(fase == 2)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 36 ] );
				else if(fase == 3)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 37 ] );
				else if(fase == 4)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 38 ] );
				}
				contCaixa++;
            }
			else if (mapa[linha][coluna] == CAIXA)
            {
				caixaRECT[contCaixa].x = destino.x + 10;
				caixaRECT[contCaixa].y = destino.y + 5;
				caixaRECT[contCaixa].w = 40;
				caixaRECT[contCaixa].h = 40;
				
                if(fase == 1)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 22 ] );
				else if(fase == 2)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 33 ] );
				else if(fase == 3)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 34 ] );
				else if(fase == 4)
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 35 ] );
            
				
				if(caixaexiste[contCaixa] == 0)
				{
					apply_surface( destino.x,destino.y, player, tela, &spritesheet[ 23 ] );
				}
				if(caixaexiste[contCaixa] == 1)
				{
				caixaRECT[contCaixa].x = 0;
				caixaRECT[contCaixa].y = 0;
				caixaRECT[contCaixa].w = 0;
				caixaRECT[contCaixa].h = 0;
				}
				contCaixa++;
            }
			else if (mapa[linha][coluna] == HUD)
            {
				SDL_BlitSurface(hud, NULL, tela, &destino);
            }
        }
		
    }
}
  
/* ***************** Colisão ***************** */ 
int collision(SDL_Rect* rect1,SDL_Rect* rect2)
{
    if(rect1->y >=(rect2->y + rect2->h - 10))
    {
    	return 0;
    }
    if(rect1->x >= (rect2->x + rect2->w - 5))
    {
    	return 0;
    }
    if((rect1->y + rect1->h) <= rect2->y)
    {
    	return 0;
    }
    if((rect1->x + rect1->w) <= rect2->x + 5)
    {
        return 0;
    }
    return 1;
}

/* ***************** Função para colocar e explodir a bomba ***************** */
int explodeBomba, blitarexplosao, z = 0;
void soltarBomba(int x, int y, int tempo_inicial)
{
	int contexplosao;
	destinoBomba.w = 50;
	destinoBomba.h = 50;
	
	//Decidir a coluna a ser blitada
	if(((x + (PLAYER_W/2)) >= 50) && ((x + (PLAYER_W/2)) < 100))
	{
		destinoBomba.x = 50;
	}
	else if(((x + (PLAYER_W/2)) >= 100) && ((x + (PLAYER_W/2)) < 150))
	{
		destinoBomba.x = 100;
	}
	else if(((x + (PLAYER_W/2)) >= 150) && ((x + (PLAYER_W/2)) < 200))
	{
		destinoBomba.x = 150;
	}
	else if(((x + (PLAYER_W/2)) >= 200) && ((x + (PLAYER_W/2)) < 250))
	{
		destinoBomba.x = 200;
	}
	else if(((x + (PLAYER_W/2)) >= 250) && ((x + (PLAYER_W/2)) < 300))
	{
		destinoBomba.x = 250;
	}
	else if(((x + (PLAYER_W/2)) >= 300) && ((x + (PLAYER_W/2)) < 350))
	{
		destinoBomba.x = 300;
	}
	else if(((x + (PLAYER_W/2)) >= 350) && ((x + (PLAYER_W/2)) < 400))
	{
		destinoBomba.x = 350;
	}
	else if(((x + (PLAYER_W/2)) >= 400) && ((x + (PLAYER_W/2)) < 450))
	{
		destinoBomba.x = 400;
	}
	else if(((x + (PLAYER_W/2)) >= 450) && ((x + (PLAYER_W/2)) < 500))
	{
		destinoBomba.x = 450;
	}
	else if(((x + (PLAYER_W/2)) >= 500) && ((x + (PLAYER_W/2)) < 550))
	{
		destinoBomba.x = 500;
	}
	else if(((x + (PLAYER_W/2)) >= 550) && ((x + (PLAYER_W/2)) < 600))
	{
		destinoBomba.x = 550;
	}
	else if(((x + (PLAYER_W/2)) >= 600) && ((x + (PLAYER_W/2)) < 650))
	{
		destinoBomba.x = 600;
	}
	else if(((x + (PLAYER_W/2)) >= 650) && ((x + (PLAYER_W/2)) < 700))
	{
		destinoBomba.x = 650;
	}
	else if(((x + (PLAYER_W/2)) >= 700) && ((x + (PLAYER_W/2)) < 750))
	{
		destinoBomba.x = 700;
	}
	else if(((x + (PLAYER_W/2)) >= 750) && ((x + (PLAYER_W/2)) < 800))
	{
		destinoBomba.x = 750;
	}

	//Decidir linha a ser blitada
	if(((y + (PLAYER_H/2)) >= 50) && ((y + (PLAYER_H/2)) < 100))
	{
		destinoBomba.y = 50;
	}
	else if(((y + (PLAYER_H/2)) >= 100) && ((y + (PLAYER_H/2)) < 150))
	{
		destinoBomba.y = 100;
	}
	else if(((y + (PLAYER_H/2)) >= 150) && ((y + (PLAYER_H/2)) < 200))
	{
		destinoBomba.y = 150;
	}
	else if(((y + (PLAYER_H/2)) >= 200) && ((y + (PLAYER_H/2)) < 250))
	{
		destinoBomba.y = 200;
	}
	else if(((y + (PLAYER_H/2)) >= 250) && ((y + (PLAYER_H/2)) < 300))
	{
		destinoBomba.y = 250;
	}
	else if(((y + (PLAYER_H/2)) >= 300) && ((y + (PLAYER_H/2)) < 350))
	{
		destinoBomba.y = 300;
	}
	else if(((y + (PLAYER_H/2)) >= 350) && ((y + (PLAYER_H/2)) < 400))
	{
		destinoBomba.y = 350;
	}
	else if(((y + (PLAYER_H/2)) >= 400) && ((y + (PLAYER_H/2)) < 450))
	{
		destinoBomba.y = 400;
	}
	else if(((y + (PLAYER_H/2)) >= 450) && ((y + (PLAYER_H/2)) < 500))
	{
		destinoBomba.y = 450;
	}
	else if(((y + (PLAYER_H/2)) >= 500) && ((y + (PLAYER_H/2)) < 550))
	{
		destinoBomba.y = 500;
	}
	else if(((y + (PLAYER_H/2)) >= 550) && ((y + (PLAYER_H/2)) < 600))
	{
		destinoBomba.y = 550;
	}
	else if(((y + (PLAYER_H/2)) >= 600) && ((y + (PLAYER_H/2)) < 650))
	{
		destinoBomba.y = 600;
	}
	else if(((y + (PLAYER_H/2)) >= 650) && ((y + (PLAYER_H/2)) < 700))
	{
		destinoBomba.y = 650;
	}
	else if(((y + (PLAYER_H/2)) >= 700) && ((y + (PLAYER_H/2)) < 750))
	{
		destinoBomba.y = 700;
	}

	apply_surface( destinoBomba.x,destinoBomba.y, player, tela, &spritesheet[ 24 ] );

	int c = 0, d, j, tamanho;
	if( tempo_inicial > (explodeBomba + 3000))
	{
		Mix_PlayChannel(-1,explosaoSom,0); // Tocar som da explosão
		
		/* Centro */
		SDL_FreeSurface(bomba);
		destinoExplosao[0].x = destinoBomba.x;
		destinoExplosao[0].y = destinoBomba.y;
		destinoExplosao[0].h = 50;
		destinoExplosao[0].w = 50;
		apply_surface( destinoExplosao[0].x,destinoExplosao[0].y, player, tela, &spritesheet[ 25 ] );
		if(collision(&dest, &destinoExplosao[0]))
		{
						vida -= 1;
						tempoMorte = SDL_GetTicks();
					}
		d = 0;
		j = 0;

		/* Direita */
		for(tamanho = 1; tamanho <= tamanhoFogo; tamanho++)
		{
			destinoExplosao[1].x = destinoBomba.x + ((tamanho)*50);
			destinoExplosao[1].y = destinoBomba.y;
			destinoExplosao[1].h = 50;
			destinoExplosao[1].w = 50;
			for(c = 0; c < 96; c++)
			{
				if(collision(&destinoExplosao[1],&paredeRECT[c]))
				{
					d = 1;
					break;
				}
				for(j = 0; j < 100; j++)
				{
					if(collision(&destinoExplosao[1],&caixaRECT[j]))
					{	
						aumentaPontos = 1;
						z = 1;
						d = 1;
						apply_surface( destinoExplosao[1].x,destinoExplosao[1].y, player, tela, &spritesheet[ 28 ] );
						caixaexiste[j] = 1;
						bomb = 0;
						break;
					}
					if(z = 0)
					{
						break;
					}
				}	
			}
				if(d == 0)
				{
					if(tamanho == tamanhoFogo)
						apply_surface( destinoExplosao[1].x,destinoExplosao[1].y, player, tela, &spritesheet[ 28 ] );
					else
						apply_surface( destinoExplosao[1].x,destinoExplosao[1].y, player, tela, &spritesheet[ 26 ] );
						
					if(collision(&dest, &destinoExplosao[1]))
					{
						vida--;
						tempoMorte = SDL_GetTicks();
					}
					if(collision(&destinoInimigo, &destinoExplosao[1]))
					{
					vidainimigo1--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo.x = 0;
					destinoInimigo.y = 0;
					destinoInimigo.h = 0;
					destinoInimigo.w = 0;
					}
				if(collision(&destinoInimigo2, &destinoExplosao[1]))
					{
					vidainimigo2--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo2.x = 0;
					destinoInimigo2.y = 0;
					destinoInimigo2.h = 0;
					destinoInimigo2.w = 0;
					}
				if(collision(&destinoInimigo3, &destinoExplosao[1]))
				{
					vidainimigo3--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo3.x = 0;
					destinoInimigo3.y = 0;
					destinoInimigo3.h = 0;
					destinoInimigo3.w = 0;
				}
				if(collision(&destinoInimigo4, &destinoExplosao[1]))
					{
					vidainimigo4--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo4.x = 0;
					destinoInimigo4.y = 0;
					destinoInimigo4.h = 0;
					destinoInimigo4.w = 0;
				}
				}
				if (d == 1)
				{
					destinoExplosao[1].x = destinoBomba.x - ((tamanho)*50);
					break;
				}
		}

		/* Esquerda */
		d = 0;
		z = 0;
		for(tamanho = 1; tamanho <= tamanhoFogo; tamanho++)
		{
			destinoExplosao[2].x = destinoBomba.x - ((tamanho)*50);
			destinoExplosao[2].y = destinoBomba.y;
			destinoExplosao[2].h = 50;
			destinoExplosao[2].w = 50;
			for(c = 0; c < 96; c++)
			{
				
				if(collision(&destinoExplosao[2],&paredeRECT[c]))
				{
					d = 1;
					break;
				}
				for(j = 0; j < 100; j++)
				{
					if(collision(&destinoExplosao[2],&caixaRECT[j]))
					{	
						aumentaPontos = 1;
						z = 1;
						d = 1;
						apply_surface( destinoExplosao[2].x,destinoExplosao[2].y, player, tela, &spritesheet[ 29 ] );
						caixaexiste[j] = 1;
						bomb = 0;
						break;
					}
					if(z = 0)
					{
						break;
					}
				}	
			}
				if(d == 0)
				{
					if(tamanho == tamanhoFogo)
						apply_surface( destinoExplosao[2].x,destinoExplosao[2].y, player, tela, &spritesheet[ 29 ] );
					else
						apply_surface( destinoExplosao[2].x,destinoExplosao[2].y, player, tela, &spritesheet[ 26 ] );
					if(collision(&dest, &destinoExplosao[2]))
					{
						vida -= 1;
						tempoMorte = SDL_GetTicks();
					}
					if(collision(&destinoInimigo, &destinoExplosao[2]))
					{
					vidainimigo1--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo.x = 0;
					destinoInimigo.y = 0;
					destinoInimigo.h = 0;
					destinoInimigo.w = 0;
					}
				if(collision(&destinoInimigo2, &destinoExplosao[2]))
				{
					vidainimigo2--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo2.x = 0;
					destinoInimigo2.y = 0;
					destinoInimigo2.h = 0;
					destinoInimigo2.w = 0;
				}
				if(collision(&destinoInimigo3, &destinoExplosao[2]))
					{
					vidainimigo3--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo3.x = 0;
					destinoInimigo3.y = 0;
					destinoInimigo3.h = 0;
					destinoInimigo3.w = 0;
					}
				if(collision(&destinoInimigo4, &destinoExplosao[2]))
					{
					vidainimigo4--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo4.x = 0;
					destinoInimigo4.y = 0;
					destinoInimigo4.h = 0;
					destinoInimigo4.w = 0;
					}
				}
				if(d == 1)
				{
					destinoExplosao[2].x = destinoBomba.x + ((tamanho)*50);
					break;
				}
		}			

		/* Baixo */
		d = 0;
		z = 0;
		for(tamanho = 1; tamanho <= tamanhoFogo; tamanho++)
		{
			destinoExplosao[3].x = destinoBomba.x;
			destinoExplosao[3].y = destinoBomba.y + ((tamanho)*50);
			destinoExplosao[3].h = 50;
			destinoExplosao[3].w = 50;
			
			for(c = 0; c < 96; c++)
			{
				
				if(collision(&destinoExplosao[3],&paredeRECT[c]))
				{
					d = 1;
					break;
				}
				for(j = 0; j < 100; j++)
				{
					if(collision(&destinoExplosao[3],&caixaRECT[j]))
					{	
						aumentaPontos = 1;
						apply_surface( destinoExplosao[3].x,destinoExplosao[3].y, player, tela, &spritesheet[ 31 ] );
						d = 1;
						z = 1;
						caixaexiste[j] = 1;
						bomb = 0;
						break;
					}
					if(z = 0)
					{
						break;
					}
				}	
			}
				if(d == 0)
				{
					if(tamanho == tamanhoFogo)
						apply_surface( destinoExplosao[3].x,destinoExplosao[3].y, player, tela, &spritesheet[ 31 ] );
					else
						apply_surface( destinoExplosao[3].x,destinoExplosao[3].y, player, tela, &spritesheet[ 27 ] );
					if(collision(&dest, &destinoExplosao[3]))
					{
						vida -= 1;
						tempoMorte = SDL_GetTicks();
					}
					if(collision(&destinoInimigo, &destinoExplosao[3]))
				{
					vidainimigo1--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo.x = 0;
					destinoInimigo.y = 0;
					destinoInimigo.h = 0;
					destinoInimigo.w = 0;
				}
				if(collision(&destinoInimigo2, &destinoExplosao[3]))
				{
					vidainimigo2--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo2.x = 0;
					destinoInimigo2.y = 0;
					destinoInimigo2.h = 0;
					destinoInimigo2.w = 0;
				}
				if(collision(&destinoInimigo3, &destinoExplosao[3]))
				{
					vidainimigo3--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo3.x = 0;
					destinoInimigo3.y = 0;
					destinoInimigo3.h = 0;
					destinoInimigo3.w = 0;
				}
				if(collision(&destinoInimigo4, &destinoExplosao[3]))
				{
					vidainimigo4--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo4.x = 0;
					destinoInimigo4.y = 0;
					destinoInimigo4.h = 0;
					destinoInimigo4.w = 0;
				}
				}
				if(d == 1)
				{
					destinoExplosao[3].y = destinoBomba.y - ((tamanho)*50);
					break;
				}
		}
		
		/* Cima */
		d = 0;
		z = 0;
		for(tamanho = 1; tamanho <= tamanhoFogo; tamanho++)
		{
			destinoExplosao[4].x = destinoBomba.x;
			destinoExplosao[4].y = destinoBomba.y - ((tamanho)*50);
			destinoExplosao[4].h = 50;
			destinoExplosao[4].w = 50;
			
			for(c = 0; c < 96; c++)
			{
				
				if(collision(&destinoExplosao[4],&paredeRECT[c]))
				{
					d = 1;
					break;
				}
				for(j = 0; j < 100; j++)
				{
					if(collision(&destinoExplosao[4],&caixaRECT[j]))
					{	
						aumentaPontos = 1;
						d = 1;
						z = 1;
						apply_surface( destinoExplosao[4].x,destinoExplosao[4].y, player, tela, &spritesheet[ 30 ] );
						caixaexiste[j] = 1;
						bomb = 0;
						break;
					}
					if(z = 0)
					{
						break;
					}
				}	
			}
				if(d == 0)
				{
					if(tamanho == tamanhoFogo)
						apply_surface( destinoExplosao[4].x,destinoExplosao[4].y, player, tela, &spritesheet[ 30 ] );
					else
						apply_surface( destinoExplosao[4].x,destinoExplosao[4].y, player, tela, &spritesheet[ 27 ] );
					if(collision(&dest, &destinoExplosao[4]))
					{
						vida -= 1;
						tempoMorte = SDL_GetTicks();
					}
					if(collision(&destinoInimigo, &destinoExplosao[4]))
				{
					vidainimigo1--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo.x = 0;
					destinoInimigo.y = 0;
					destinoInimigo.h = 0;
					destinoInimigo.w = 0;
				}
				if(collision(&destinoInimigo2, &destinoExplosao[4]))
				{
					vidainimigo2--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo2.x = 0;
					destinoInimigo2.y = 0;
					destinoInimigo2.h = 0;
					destinoInimigo2.w = 0;
				}
				if(collision(&destinoInimigo3, &destinoExplosao[4]))
				{
					vidainimigo3--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo3.x = 0;
					destinoInimigo3.y = 0;
					destinoInimigo3.h = 0;
					destinoInimigo3.w = 0;
				}
				if(collision(&destinoInimigo4, &destinoExplosao[4]))
				{
					vidainimigo4--;
					 Mix_PlayChannel(-1,morteInimigo,0); 
					destinoInimigo4.x = 0;
					destinoInimigo4.y = 0;
					destinoInimigo4.h = 0;
					destinoInimigo4.w = 0;
				}
				}
				if(d == 1)
				{
					destinoExplosao[4].y = destinoBomba.y + ((tamanho)*50);
					break;
				}
		}
			
		/* Tirar os Rects */
		if( tempo_inicial > (explodeBomba + 3400))
		{
			bomb = 0;
			
			for(d = 0; d < 5; d++)
			{
				destinoExplosao[d].x = 0;
				destinoExplosao[d].y = 0;
				destinoExplosao[d].h = 0;
				destinoExplosao[d].w = 0;
			}
		}
		if(bomb == 0)
		{
			destinoBomba.h = 0;
			destinoBomba.w = 0;
			destinoBomba.x = 0;
			destinoBomba.y = 0;
		}
	}
}

/* ***************** Função para resetar o jogo ***************** */
void resetajogo()
{
		int cont;
		if(reset == 1)
		{
			dest.x = 50;
			dest.y = 100;
			
			i = 0;
			
			SDL_Delay(100);
			
			destinoInimigo.x = 750;
			destinoInimigo.y = 600;

			destinoInimigo2.x = 650;
			destinoInimigo2.y = 400;

			destinoInimigo3.x = 450;
			destinoInimigo3.y = 300;

			destinoInimigo4.x = 300;
			destinoInimigo4.y = 500;

			for(cont = 0; cont < 100; cont++)
			{
				if(caixaexiste[cont] == 1)
				{
					caixaexiste[cont] = 0;
				}
			}
		reset = 0;
		}
}

/* ***************** Função para criar o Menu Principal ***************** */
void menu( SDL_Event evento)
{
	int opcao, apertou;
	while(sairMenu == 0)
	{
		while(SDL_PollEvent(&evento))
        {
			/* Definição para fim do loop principal e do menu */
            if(evento.type == SDL_QUIT)
            {
            	fim = 1;
				sairMenu = 1;
			}
			
			/* Verifica se o player pessionou algum botão */
            if (evento.type == SDL_KEYDOWN)
            {
				/* Ação para cada botão apertado */
                switch (evento.key.keysym.sym)
                {
					case SDLK_ESCAPE:
                    	fim = 1;
						sairMenu = 1;
                    break;
					
					case SDLK_UP:
						botaoMenu -= 1;
						if(botaoMenu<1)
							botaoMenu = 1;
					break;
					
					case SDLK_DOWN:
						botaoMenu += 1;
						if(botaoMenu>3)
							botaoMenu = 3;
					break;
					
					case SDLK_RETURN:
					if(botaoMenu == 1)
					{
						apertou = SDL_GetTicks();
						opcao = 1;
					}
					if(botaoMenu == 2)
					{
						telahighscore = 1;
						sairMenu = 1;
					}
					if(botaoMenu == 3)
					{
						fim = 1;
						sairMenu = 1;
					}
					break;
					
					default:
					break;
				}
			}
		}
		
		if(botaoMenu == 1)
		{
			SDL_BlitSurface(telaMenu1, NULL, tela, &destinoMenu);
		}
		
		if(botaoMenu == 2)
		{
			SDL_BlitSurface(telaMenu2, NULL, tela, &destinoMenu);
		}
		if(botaoMenu == 3)
		{
			SDL_BlitSurface(telaMenu3, NULL, tela, &destinoMenu);
		}
		if(opcao == 1)
			{	
				SDL_BlitSurface(comoJogar, NULL, tela, &destinoMenu);
				if(SDL_GetTicks() > (apertou + 1500))
				{
					sairMenu = 1;
					opcao = 0;
				}
			}
			
		SDL_UpdateRect(tela, 0,0,0,0); /* Atualiza todo o screen */ 
	}
}

int entrouPause;
/* ***************** Função para criar o Menu de Pause ***************** */
void pause( SDL_Event evento)
{
	while(sairPause == 0)
		{	
			
			while(SDL_PollEvent(&evento))
			{
				/* Definição para fim do loop principal e do menu */
				if(evento.type == SDL_QUIT)
				{
					fim = 1;
					sairPause = 1;
				}
				
				/* Verifica se o player pessionou algum botão */
				if (evento.type == SDL_KEYDOWN)
				{
					/* Ação para cada botão apertado */
					switch (evento.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							sairPause = 1;
						break;
						
						case SDLK_UP:
							botaoPause -= 1;
							if(botaoPause<1)
								botaoPause = 1;
						break;
						
						case SDLK_DOWN:
							botaoPause += 1;
							if(botaoPause > 3)
								botaoPause = 3;
						break;
						
						case SDLK_RETURN:
						if(botaoPause == 1)
						{
							sairPause = 1;
						}
						if(botaoPause == 2)
						{
							reset = 1;
							sairMenu = 0;
							sairPause = 1;
						}
						if(botaoPause == 3)
						{
							fim = 1;
							sairPause = 1;
						}
						break;
						
						default:
						break;
					}
				}
			}
			
			if(botaoPause == 1)
			{
				SDL_BlitSurface(telaPause1, NULL, tela, &destinoMenu);
			}
			
			if(botaoPause == 2)
			{
				SDL_BlitSurface(telaPause2, NULL, tela, &destinoMenu);
			}
			if(botaoPause == 3)
			{
				SDL_BlitSurface(telaPause3, NULL, tela, &destinoMenu);
			}
			
			SDL_UpdateRect(tela, 0,0,0,0); /* Atualiza todo o screen */ 
		}
}

/* ***************** Função para criar a tela de Game Over ***************** */
void gameOver( SDL_Event evento)
{
	while(sairGameOver == 0)
		{
			while(SDL_PollEvent(&evento))
			{
				/* Definição para fim do loop principal e do menu */
				if(evento.type == SDL_QUIT)
				{
					fim = 1;
					sairGameOver = 1;
				}
				
				/* Verifica se o player pessionou algum botão */
				if (evento.type == SDL_KEYDOWN)
				{
					/* Ação para cada botão apertado */
					switch (evento.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							sairGameOver = 1;
						break;
						
						case SDLK_LEFT:
							botaoPause = 1;
						break;
						
						case SDLK_RIGHT:
							botaoPause = 2;
						break;
						
						case SDLK_RETURN:
						if(botaoPause == 1)
						{
							reset = 1;
							sairGameOver = 1;
						}
						if(botaoPause == 2)
						{
							sairMenu = 0;
							sairGameOver = 1;
						}
						break;
						
						default:
						break;
					}
				}
			}
			
			if(botaoPause == 1)
			{
				SDL_BlitSurface(telaGameOver1, NULL, tela, &destinoMenu);
			}
			
			if(botaoPause == 2)
			{
				SDL_BlitSurface(telaGameOver2, NULL, tela, &destinoMenu);
			}		
			SDL_UpdateRect(tela, 0,0,0,0); /* Atualiza todo o screen */ 
		}
}

/* ***************** Função para criar os inimigos ***************** */
void inimigo1()
{
	srand( (unsigned)time(NULL) );
	if(numAleatorio1 == 0)
		numAleatorio1 = rand()%4;
	int c;
		switch(numAleatorio1)
		{
			// Cima
			case 0:
			destinoInimigo.y -= (VELCHAR + 5) ;
			ladoInimigo = 0;
			for(c = 0; c< 96; c++)
				{
					if(collision(&destinoInimigo,&paredeRECT[c]) || collision(&destinoInimigo, &destinoBomba))
					{
						destinoInimigo.y += (VELCHAR + 5);
						numAleatorio1 = 0;
					}
				}
			for(c = 0; c< 100; c++)
				{
					if(collision(&destinoInimigo,&caixaRECT[c]) || collision(&destinoInimigo, &destinoBomba))
					{
						destinoInimigo.y += (VELCHAR + 5);
						numAleatorio1 = 0;
					}
				}
			break;
			// Baixo
			case 1:
			destinoInimigo.y += (VELCHAR + 5);
			ladoInimigo = 1;
			for(c = 0; c< 96; c++)
				{
					if(collision(&destinoInimigo,&paredeRECT[c]) || collision(&destinoInimigo, &destinoBomba))
					{
						destinoInimigo.y -= (VELCHAR + 5);
						numAleatorio1 = 0;
					}
				}
			for(c = 0; c< 100; c++)
				{
					if(collision(&destinoInimigo,&caixaRECT[c]) || collision(&destinoInimigo, &destinoBomba))
					{
						destinoInimigo.y -= (VELCHAR + 5);
						numAleatorio1 = 0;
					}
				}
			break;
			// Direita
			case 2:
			destinoInimigo.x += (VELCHAR + 5);
			ladoInimigo = 2;
			for(c = 0; c< 96; c++)
				{
					if(collision(&destinoInimigo,&paredeRECT[c]) || collision(&destinoInimigo, &destinoBomba))
					{
						destinoInimigo.x -= (VELCHAR + 5);
						numAleatorio1 = 0;
					}
				}
			for(c = 0; c< 100; c++)
				{
					if(collision(&destinoInimigo,&caixaRECT[c]) || collision(&destinoInimigo, &destinoBomba))
					{
						destinoInimigo.x -= (VELCHAR + 5);
						numAleatorio1 = 0;
					}
				}
			break;
			// Esquerda
			case 3:
			destinoInimigo.x -= (VELCHAR + 5);
			ladoInimigo = 3;
			for(c = 0; c< 96; c++)
				{
					if(collision(&destinoInimigo,&paredeRECT[c]) || collision(&destinoInimigo, &destinoBomba))
					{
						destinoInimigo.x += (VELCHAR + 5);
						numAleatorio1 = 0;
					}
				}
			for(c = 0; c< 100; c++)
				{
					if(collision(&destinoInimigo,&caixaRECT[c]) || collision(&destinoInimigo, &destinoBomba))
					{
						destinoInimigo.x += (VELCHAR + 5);
						numAleatorio1 = 0;
					}
				}
			break;
		}
	
}

void inimigo2()
{
	srand( (unsigned)time(NULL) );
	if(numAleatorio2 == 0)
		numAleatorio2 = rand()%4;
	int c;
		switch(numAleatorio2)
		{
			// Cima
			case 0:
			destinoInimigo2.y -= (VELCHAR + 10) ;
			ladoInimigo = 0;
			for(c = 0; c< 96; c++)
				{
					if(collision(&destinoInimigo2,&paredeRECT[c]) || collision(&destinoInimigo2, &destinoBomba))
					{
						destinoInimigo2.y += (VELCHAR + 10);
						numAleatorio2 = 0;
					}
				}
			for(c = 0; c< 100; c++)
				{
					if(collision(&destinoInimigo2,&caixaRECT[c]) || collision(&destinoInimigo2, &destinoBomba))
					{
						destinoInimigo2.y += (VELCHAR + 10);
						numAleatorio2 = 0;
					}
				}
			break;

			// Baixo
			case 1:
			destinoInimigo2.y += (VELCHAR + 10);
			ladoInimigo = 1;
			for(c = 0; c< 96; c++)
				{
					if(collision(&destinoInimigo2,&paredeRECT[c]) || collision(&destinoInimigo2, &destinoBomba))
					{
						destinoInimigo2.y -= (VELCHAR + 10);
						numAleatorio2 = 0;
					}
				}
			for(c = 0; c< 100; c++)
				{
					if(collision(&destinoInimigo2,&caixaRECT[c]) || collision(&destinoInimigo2, &destinoBomba))
					{
						destinoInimigo2.y -= (VELCHAR + 10);
						numAleatorio2 = 0;
					}
				}
			break;

			// Direita
			case 2:
			destinoInimigo2.x += (VELCHAR + 10);
			ladoInimigo = 2;
			for(c = 0; c< 96; c++)
				{
					if(collision(&destinoInimigo2,&paredeRECT[c]) || collision(&destinoInimigo2, &destinoBomba))
					{
						destinoInimigo2.x -= (VELCHAR + 10);
						numAleatorio2 = 0;
					}
				}
			for(c = 0; c< 100; c++)
				{
					if(collision(&destinoInimigo2,&caixaRECT[c]) || collision(&destinoInimigo2, &destinoBomba))
					{
						destinoInimigo2.x -= (VELCHAR + 10);
						numAleatorio2 = 0;
					}
				}
			break;
			// Esquerda

			case 3:
			destinoInimigo2.x -= (VELCHAR + 10);
			ladoInimigo = 3;
			for(c = 0; c< 96; c++)
				{
					if(collision(&destinoInimigo2,&paredeRECT[c]) || collision(&destinoInimigo2, &destinoBomba))
					{
						destinoInimigo2.x += (VELCHAR + 10);
						numAleatorio2 = 0;
					}
				}
			for(c = 0; c< 100; c++)
				{
					if(collision(&destinoInimigo2,&caixaRECT[c]) || collision(&destinoInimigo2,&destinoBomba))
					{
						destinoInimigo2.x += (VELCHAR + 10);
						numAleatorio2 = 0;
					}
				}
			break;
		}
	
}

void inimigo3()
{
	srand( (unsigned)time(NULL) );
	if(numAleatorio3 == 0)
	numAleatorio3 = rand()%4;
	int c;
	switch(numAleatorio3)
	{
		// Cima
		case 0:
		destinoInimigo3.y -= (VELCHAR + 10) ;
		ladoInimigo = 0;
		for(c = 0; c< 96; c++)
			{
				if(collision(&destinoInimigo3,&paredeRECT[c]) || collision(&destinoInimigo3, &destinoBomba))
				{
					destinoInimigo3.y += (VELCHAR + 10);
					numAleatorio3 = 0;
				}
			}
		for(c = 0; c< 100; c++)
			{
				if(collision(&destinoInimigo3,&caixaRECT[c]) || collision(&destinoInimigo3, &destinoBomba))
				{
					destinoInimigo3.y += (VELCHAR + 10);
					numAleatorio3 = 0;
				}
			}
		break;

		// Baixo
		case 1:
		destinoInimigo3.y += (VELCHAR + 10);
		ladoInimigo = 1;
		for(c = 0; c< 96; c++)
			{
				if(collision(&destinoInimigo3,&paredeRECT[c]) || collision(&destinoInimigo3, &destinoBomba))
				{
					destinoInimigo3.y -= (VELCHAR + 10);
					numAleatorio3 = 0;
				}
			}
		for(c = 0; c< 100; c++)
			{
				if(collision(&destinoInimigo3,&caixaRECT[c]) || collision(&destinoInimigo3, &destinoBomba))
				{
					destinoInimigo3.y -= (VELCHAR + 10);
					numAleatorio3 = 0;
				}
			}
		break;

		// Direita
		case 2:
		destinoInimigo3.x += (VELCHAR + 10);
		ladoInimigo = 2;
		for(c = 0; c< 96; c++)
			{
				if(collision(&destinoInimigo3,&paredeRECT[c]) || collision(&destinoInimigo3, &destinoBomba))
				{
					destinoInimigo3.x -= (VELCHAR + 10);
					numAleatorio3 = 0;
				}
			}
		for(c = 0; c< 100; c++)
			{
				if(collision(&destinoInimigo3,&caixaRECT[c]) || collision(&destinoInimigo3, &destinoBomba))
				{
					destinoInimigo3.x -= (VELCHAR + 10);
					numAleatorio3 = 0;
				}
			}
		break;
		// Esquerda

		case 3:
		destinoInimigo3.x -= (VELCHAR + 10);
		ladoInimigo = 3;
		for(c = 0; c< 96; c++)
			{
				if(collision(&destinoInimigo3,&paredeRECT[c]) || collision(&destinoInimigo3, &destinoBomba))
				{
					destinoInimigo3.x += (VELCHAR + 10);
					numAleatorio3 = 0;
				}
			}
		for(c = 0; c< 100; c++)
			{
				if(collision(&destinoInimigo3,&caixaRECT[c]) || collision(&destinoInimigo3,&destinoBomba))
				{
					destinoInimigo3.x += (VELCHAR + 10);
					numAleatorio3 = 0;
				}
			}
		break;
	}
}

void inimigo4()
{
	srand( (unsigned)time(NULL) );
	if(numAleatorio4 == 0)
	numAleatorio4 = rand()%4;
	int c;
	switch(numAleatorio4)
	{
		// Cima
		case 0:
		destinoInimigo4.y -= (VELCHAR + 10) ;
		ladoInimigo = 0;
		for(c = 0; c< 96; c++)
			{
				if(collision(&destinoInimigo4,&paredeRECT[c]) || collision(&destinoInimigo4, &destinoBomba))
				{
					destinoInimigo4.y += (VELCHAR + 10);
					numAleatorio4 = 0;
				}
			}
		for(c = 0; c< 100; c++)
			{
				if(collision(&destinoInimigo4,&caixaRECT[c]) || collision(&destinoInimigo4, &destinoBomba))
				{
					destinoInimigo4.y += (VELCHAR + 10);
					numAleatorio4 = 0;
				}
			}
		break;

		// Baixo
		case 1:
		destinoInimigo4.y += (VELCHAR + 10);
		ladoInimigo = 1;
		for(c = 0; c< 96; c++)
			{
				if(collision(&destinoInimigo4,&paredeRECT[c]) || collision(&destinoInimigo4, &destinoBomba))
				{
					destinoInimigo4.y -= (VELCHAR + 10);
					numAleatorio4 = 0;
				}
			}
		for(c = 0; c< 100; c++)
			{
				if(collision(&destinoInimigo4,&caixaRECT[c]) || collision(&destinoInimigo4, &destinoBomba))
				{
					destinoInimigo4.y -= (VELCHAR + 10);
					numAleatorio4 = 0;
				}
			}
		break;

		// Direita
		case 2:
		destinoInimigo4.x += (VELCHAR + 10);
		ladoInimigo = 2;
		for(c = 0; c< 96; c++)
			{
				if(collision(&destinoInimigo4,&paredeRECT[c]) || collision(&destinoInimigo4, &destinoBomba))
				{
					destinoInimigo4.x -= (VELCHAR + 10);
					numAleatorio4 = 0;
				}
			}
		for(c = 0; c< 100; c++)
			{
				if(collision(&destinoInimigo4,&caixaRECT[c]) || collision(&destinoInimigo4, &destinoBomba))
				{
					destinoInimigo4.x -= (VELCHAR + 10);
					numAleatorio4 = 0;
				}
			}
		break;
		// Esquerda

		case 3:
		destinoInimigo4.x -= (VELCHAR + 10);
		ladoInimigo = 3;
		for(c = 0; c< 96; c++)
			{
				if(collision(&destinoInimigo4,&paredeRECT[c]) || collision(&destinoInimigo4, &destinoBomba))
				{
					destinoInimigo4.x += (VELCHAR + 10);
					numAleatorio4 = 0;
				}
			}
		for(c = 0; c< 100; c++)
			{
				if(collision(&destinoInimigo4,&caixaRECT[c]) || collision(&destinoInimigo4,&destinoBomba))
				{
					destinoInimigo4.x += (VELCHAR + 10);
					numAleatorio4 = 0;
				}
			}
		break;
	}
}

int contador1, contador2;

void cronometro()
{
	tempodejogo[2] = tempodejogo[2] - 1;

	if(tempodejogo[2] == -1)
	{
		tempodejogo[2] = 9;
		tempodejogo[1]--;
		if(tempodejogo[1] == -1)
		{
			tempodejogo[1] = 9;
			tempodejogo[0]--;
		}
	}

	for(contador1 = 0; contador1 < 3; contador1++)
	{
		for(contador2 = 0; contador2 < 9; contador2++)
		{
			if(tempodejogo[2] == contador2)
			{
				//BlitNumero
			}
		}
	}

	if(tempodejogo[0] == 0 && tempodejogo[1] == 0 && tempodejogo[2] == 0)
	{
		vida--;
	}
}
 
 char totalDePontos[8];
 void hud_pontuacao()
 {	
	if(aumentaPontos == 1)
	{
		pontos += 50;
		aumentaPontos = 0;
	}
	sprintf(totalDePontos, "%i", pontos);
	
	destinoPontos.x = 750;
	destinoPontos.y = 0;
	destinoPontos.h = 100;
	destinoPontos.w = 100;
	
	SDL_Color fColor; // Font color (R,G,B)
	fColor.r = 95;
	fColor.g = 3;
	fColor.b = 4;
	
	numerosPontos = TTF_RenderText_Solid( fonte , totalDePontos , fColor );
	
	SDL_BlitSurface( numerosPontos , NULL , tela , &destinoPontos );
 }

void highScore() {

}

int mins = 5, segs, segsM = 0, go = 0;
void hud_tempo(int tempo_inicial) {
	
	char seg[3], min[3];
	segs = 50;
	SDL_Rect destinoSeg, destinoMin;
 	
	if(segs == 0)
		{fim = 1;}
	
	sprintf(seg, "%i", segs);
	sprintf(min, "%i", mins);
	
	destinoSeg.x = 400;
	destinoSeg.y = 0;
	destinoSeg.h = 100;
	destinoSeg.w = 100;
	
	destinoMin.x = 350;
	destinoMin.y = 0;
	destinoMin.h = 100;
	destinoMin.w = 100;
	
	SDL_Color fColor; // Font color (R,G,B)
	fColor.r = 95;
	fColor.g = 3;
	fColor.b = 4;
	
	segundos = TTF_RenderText_Solid( fonte , seg , fColor );
	minutos = TTF_RenderText_Solid( fonte , min , fColor );
	
	SDL_BlitSurface( segundos , NULL , tela , &destinoSeg );
	SDL_BlitSurface( minutos , NULL , tela , &destinoMin );
	
}

void hud_Vidas()
 {	
 
	SDL_Rect destinoVidas;
	char vidas[2];
	sprintf(vidas, "%i", vida);
	
	destinoVidas.x = 100;
	destinoVidas.y = 0;
	destinoVidas.h = 100;
	destinoVidas.w = 100;
	
	SDL_Color fColor; // Font color (R,G,B)
	fColor.r = 95;
	fColor.g = 3;
	fColor.b = 4;
	
	numerosVidas = TTF_RenderText_Solid( fonte , vidas , fColor );
	
	SDL_BlitSurface( numerosVidas , NULL , tela , &destinoVidas );
 }
 
/* ***************** Função principal ***************** */
int main (int argc, char**argv)
{
	TTF_Init(); // inicia o TTF
	
	/*inicializando a SDL (Audio e Video) e verificando possiveis erros */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        printf("Erro: %s\n", SDL_GetError());
        exit(-1);
    }
	
	SDL_ShowCursor(0);
	
	mapaAleatorio();
	
	Mix_OpenAudio(22050,AUDIO_S16SYS,2,4096);
 
    SDL_Event evento; /* para os eventos */
    
    int velX = 0, velY = 0; /* Velocidade X e velocidade Y da imagem */
	
	int b = 0, l, c;
	
	int movimentoInimigo = 0;
 
	/* Nome da Janela */
    SDL_WM_SetCaption("A Wizard's Tale", "A Wizard's Tale");
 
	cortarSpriteSheet();
 
 
	/* Carrega os Arquivos */
    if (carregar_arquivos() == 0) /* Se não carregou uma ou outra imagem */
    {
        return 1; /* encerre o programa */
    }
 
	/* Configura o Video */
    tela = SDL_SetVideoMode(LARGURA, ALTURA, BPP, SDL_SWSURFACE | SDL_ANYFORMAT | SDL_FULLSCREEN);
    if(tela == NULL)
    {
        printf("Erro: %s\n", SDL_GetError());
        return 1; /* encerre o programa */
    }
 
    int tempo_inicial;
	
	/* Loop principal */
    while (fim == 0) /* Enquanto NÃO for o fim */
    {
		menu( evento );
		
		if(musicajogo == 0)
		{
			Mix_PlayMusic(somJogo, 0);
			musicajogo++;
		}
 		/* Para a função controlar fps */
    	tempo_inicial = SDL_GetTicks();
		 
		/* Loop de eventos */
        while(SDL_PollEvent(&evento))
        {

			/* Definição para fim do loop principal */
            if(evento.type == SDL_QUIT)
            {
            	fim = 1;
			}		
			/* Verifica se o player pessionou algum botão */
	        if (evento.type == SDL_KEYDOWN)
	        {
				/* Ação para cada botão apertado */
		        switch (evento.key.keysym.sym)
		        {
					case SDLK_ESCAPE:
						entrouPause = SDL_GetTicks();
		                sairPause = 0;
	                break;
							
					case SDLK_SPACE:
						if(bomb == 0)
						{
							explodeBomba = SDL_GetTicks();
							blitdestino.x = dest.x;
							blitdestino.y = dest.y;
							bomb = 1;
						}
	                break;
							
		                    
					case SDLK_UP:
						if(b == 0)
						{
							b = 1;
							i = 6;
						}
		            break;
							
		            case SDLK_DOWN:
						if(b == 0)
						{
							b = 2;
		                    i = 1;
						}
                    break;
							
					case SDLK_RIGHT:
						if(b == 0)
						{
							b = 3;
							i = 11;
						}
					break;
		 
		            case SDLK_LEFT:
						if(b == 0)
						{
							b = 4;
							i = 17;
						}
		            break;
		 
		            default:
		            break;
		        }
		    }
			
			/* Verifica se o player soltou algum botão */
		    if (evento.type == SDL_KEYUP)
		    {
				/* Ação para o botão que foi solto */
		        switch (evento.key.keysym.sym)
		        {
		            case SDLK_UP:
						if(b == 1)
						{
							b = 0;
							i = 5;
						}
		            break;
		
		            case SDLK_DOWN:
						if(b == 2)
						{
							b = 0;
							i = 0;
						}
		            break;
		 
		            case SDLK_RIGHT:
						if(b == 3)
						{
							b = 0;
							i = 10;
						}
	                break;
		 
	                case SDLK_LEFT:
						if(b == 4)
						{
							b = 0;
							i = 16;
						}
		            break;
		 
		            default:
		            break;
			   	}
		    }
	    }
		
		pause( evento );
		gameOver(evento);
		
		/* Animação */
		switch(i)
		{
			/* Andando para Baixo */
			case 1:
	    		i++;
       	 	break;

	        case 2:
				i++;
		    break;

			case 3:
				i++;
		    break;
			
			case 4:
				i = 1;
	        break;
					
			/* Andando para Cima */
		    case 6:
				i++;
		    break;
					
			case 7:
				i++;
	        break;
					
			case 8:
				i++;
		    break;
					
			case 9:
				i=6;
		    break;
				
			/* Andando para Direita */
			case 11:
				i++;
	        break;
			
			case 12:
				i++;
	        break;
			
			case 13:
				i++;
	        break;
			
			case 14:
				i = 11;
	        break;
				
			/* Andando para Esquerda */
			case 17:
				i++;
	        break;
				
			case 18:
				i++;
	        break;
				
			case 19:
				i++;
	        break;
				
			case 20:
				i = 17;
	        break;
	    }
	 
		      
	    SDL_FillRect(tela, NULL, (255, 255, 255)); /* Pinta todo o screen de preto */
	    desenha_mapa(); /* Desenha o Mapa */
		

		resetajogo();

		/* Colisão do Player */
		/* Cima */ 
		if(b == 1) 
		{
			dest.y -= VELCHAR;
			for(c = 0; c < 96; c++)
			{
				if(collision(&dest,&paredeRECT[c]))
				{
					dest.y += VELCHAR;
				}
			}
			
			for(c = 0; c < 100; c++)
			{
				if(collision(&dest,&caixaRECT[c]))
					{
						dest.y += VELCHAR;
					}
			}
					
			/*if(collision(&dest,&destinoBomba))
			{
				if( tempo_inicial > (explodeBomba + 1000))
				{
					dest.y += VELCHAR;
	        	}
	        }*/
	        if(collision(&dest,&saidaRECT))
				{
					if(fase < 4)
					{
						fase++;
						reset = 1;
					}
				}
	    }	

		/* Baixo */
	    if(b == 2) 
	    {
			dest.y += VELCHAR;

			for(c = 0; c< 96; c++)
			{
				if(collision(&dest,&paredeRECT[c]))
				{
					dest.y -= VELCHAR;
				}
			}

			for(c = 0; c < 100; c++)
			{
				if(collision(&dest,&caixaRECT[c]))
				{
					dest.y -= VELCHAR;
				}
			}
			
			/*if(collision(&dest,&destinoBomba))
			{
				if( tempo_inicial > (explodeBomba + 1000))
				{
					dest.x -= VELCHAR;
	            }
	        }*/
			
	        if(collision(&dest,&saidaRECT))
				{
					if(fase < 4)
					{
						fase++;
						reset = 1;
					}
				}
		}
	
		/* Direita */
	    if(b == 3)
	    {
	        dest.x += VELCHAR;
			for(c = 0; c< 96; c++)
			{
				if(collision(&dest,&paredeRECT[c]))
				{
					dest.x -= VELCHAR;
				}
			}

			for(c = 0; c < 100; c++)
			{
				if(collision(&dest,&caixaRECT[c]))
				{
					dest.x -= VELCHAR;
				}
			}
		
			/*if(collision(&dest,&destinoBomba))
			{
				if( tempo_inicial > (explodeBomba + 1000))
				{
					dest.x -= VELCHAR;
	            }
	        }*/

	        if(collision(&dest,&saidaRECT))
				{
					if(fase < 4)
					{
						fase++;
						reset = 1;
					}
				}
	    }

		/* Esquerda */
	    if(b == 4)
	    {
			dest.x -= VELCHAR;
			for(c = 0; c < 96; c++)
			{
				if(collision(&dest,&paredeRECT[c]))
				{
					dest.x += VELCHAR;
				}
			}
			
			for(c = 0; c < 100; c++)
			{
				if(collision(&dest,&caixaRECT[c]))
				{
					dest.x += VELCHAR;
				}
			}

			/*if(collision(&dest,&destinoBomba))
			{
				if( tempo_inicial > (explodeBomba + 1000))
				{
					dest.x += VELCHAR;
				}	
			}*/
			
			if(collision(&dest,&saidaRECT))
				{
					if(fase < 4)
					{
						fase++;
						reset = 1;
					}
				}
		}

		if(bomb==1)
		{
			soltarBomba(blitdestino.x, blitdestino.y, tempo_inicial);
		}
		
		if(vidainimigo1 == 1)
		{
		inimigo1(); //inimigo
		}
		
		if(vidainimigo1 == 1)
		{
			apply_surface( destinoInimigo.x,destinoInimigo.y, spritesheetInimigos, tela, &spriteInimigos[ ladoInimigo ] );//blita inimigo
			
			if(collision(&dest, &destinoInimigo))
			{
				vida--;
				tempoMorte = SDL_GetTicks();
			}
		}

		if(vidainimigo2 == 1)
		{
		inimigo2(); //inimigo
		}
		
		if(vidainimigo2 == 1)
		{
			apply_surface( destinoInimigo2.x,destinoInimigo2.y, spritesheetInimigos, tela, &spriteInimigos[ ladoInimigo ] );//blita inimigo
			
			if(collision(&dest, &destinoInimigo2))
			{
				vida--;
				tempoMorte = SDL_GetTicks();
			}
		}

		if(vidainimigo3 == 1)
		{
		inimigo3(); //inimigo
		}
		
		if(vidainimigo3 == 1)
		{
			apply_surface( destinoInimigo3.x,destinoInimigo3.y, spritesheetInimigos, tela, &spriteInimigos[ ladoInimigo ] );//blita inimigo
			
			if(collision(&dest, &destinoInimigo3))
			{
				vida--;
				tempoMorte = SDL_GetTicks();
			}
		}

		if(vidainimigo4 == 1)
		{
		inimigo4(); //inimigo
		}
		
		if(vidainimigo4 == 1)
		{
			apply_surface( destinoInimigo4.x,destinoInimigo4.y, spritesheetInimigos, tela, &spriteInimigos[ ladoInimigo ] );//blita inimigo
			
			if(collision(&dest, &destinoInimigo4))
			{
				vida--;
				tempoMorte = SDL_GetTicks();
			}
			
		if(vida <= 0)
		{
			if(tempo_inicial > (tempoMorte + 100))
			{
				 Mix_PlayMusic(morteSom,0);

			}
			if(tempo_inicial > (tempoMorte + 200))
			{
				
				gameOver(evento);
			}
		}	
		
		}				
		hud_pontuacao();
		hud_Vidas();
		hud_tempo(tempo_inicial);
	    apply_surface( dest.x,dest.y, player, tela, &spritesheet[ i ] ); /* Blita o Player */
	    SDL_UpdateRect(tela, 0,0,0,0); /* Atualiza todo o screen */ 

	    SDL_Delay(60); /* Espera 60 milissegundos */
	    controla_fps(tempo_inicial); /* controla o FPS */	
 	}
 
	/* Finalizando o SDL */
	SDL_FreeSurface(tela);
	SDL_FreeSurface(parede);
	SDL_FreeSurface(player);
	SDL_FreeSurface(chao);
	SDL_FreeSurface(caixa);
	SDL_FreeSurface(hud);
	SDL_FreeSurface(telaPause1);
	SDL_FreeSurface(telaPause2);
	SDL_FreeSurface(telaPause3);
	SDL_FreeSurface(spritesheetInimigos);
	SDL_FreeSurface(telaMenu1);
	SDL_FreeSurface(telaMenu2);
	SDL_FreeSurface(telaMenu3);
	Mix_FreeChunk(explosaoSom);
	Mix_FreeMusic(morteSom);
	Mix_FreeChunk(morteInimigo);
	Mix_FreeMusic(somJogo);
	TTF_CloseFont( fonte );
   
	Mix_CloseAudio();
	SDL_Quit();
    return 0;
}