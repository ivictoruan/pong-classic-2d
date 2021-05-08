#include <SDL.h>
#include "SDL_opengl.h"
#include <stdio.h>


#define BLOCOS 20 //quantidade de blocos diretiva de compilação?



//A - elemento 1 - inimigo
//B - elemento 2 - personagem ou outro elemento
bool colisao (float Ax, float Ay, float Acomp, float Aalt, float Bx, float By, float Bcomp, float Balt )
{//quando n colide retorna false, quando colide retorna true
    if (Ay + Aalt < By) return false;//se colide com a parte de cima do personagem
    else if (Ay > By + Balt) return false;// se colide com a parte de baixo de personagem
    else if (Ax + Acomp < Bx)return false;// n houve colisão
    else if (Ax > Bx + Bcomp) return false;

    return true;//então houve colisão
}

struct Bloco//struct de blocos
{
 float x;
 float y;
 float comp;
 float alt;
 bool vivo;
};

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);


    //memória
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_WM_SetCaption("Meu primeiro jogo em SDL", NULL); //nome_janela

    SDL_SetVideoMode(600, 400, 32, SDL_OPENGL); //tamanho_janela

    glClearColor(1, 1, 1, 1); //cor da janela

    glViewport(0, 0, 600, 400); // area exibida na janela

    glShadeModel(GL_SMOOTH); // sombreamento

    glMatrixMode(GL_PROJECTION); // Matriz em 2D (aplica a projeção na matriz)
    glLoadIdentity(); //Desenho geométrico

    glDisable(GL_DEPTH_TEST); //desabilitar modo 3d


    bool executando = true;

    SDL_Event eventos;
    //Variaveis do personagem
    float personX = 300;
    float personY = 375;//fiz modificação na altura, aumentei 15 pixels para baixo no eixo y.
    float personComp = 50;
    float personAlt = 15;

    //variaveis do inimigo
    float inimigoX = 50;
    float inimigoY = 200;
    float inimigoCA = 30;//Comprimento e altura do mesmo tamanho (Quadrado)

    float velX = .2;
    float velY = .2;

    Bloco blocos[BLOCOS];//vetor blocos usar typedef?
    //criação dos blocos
    for (int n=0, x=4, y=10; n < BLOCOS; n++, x += 61)
    {
        if (x>560)
        {
            x = 4;
            y += 23;
        }
        blocos[n].x = x;
        blocos[n].y = y;
        blocos[n].comp = 60;
        blocos[n].alt = 20;
        blocos[n].vivo = true;
    }

    bool esq = false, dir = false;


    while (executando)
    {
        //eventos
        while(SDL_PollEvent(&eventos))
        {
            if (eventos.type == SDL_QUIT)
            {
                executando = false;//fecha com o x da janela
            }

            if (eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_ESCAPE) // quando a tecla esc for solta o jogo fecha
            {
                executando = false;//fecha com esc
            }
            if (eventos.type == SDL_KEYDOWN)//verifica se alguma tecla foi precionada
            {
                if (eventos.key.keysym.sym == SDLK_LEFT)
                    {
                        esq = true;
                    }
                else if (eventos.key.keysym.sym == SDLK_RIGHT)
                {
                    dir = true;
                }
            }
            else if (eventos.type == SDL_KEYUP) // SE A TECLA FOI SOLTA
            {
                if (eventos.key.keysym.sym == SDLK_LEFT)
                {
                    esq = false;
                }
                else if(eventos.key.keysym.sym == SDLK_RIGHT)
                {
                    dir = false;
                }
            }
        }
        //------------Lógica

        //----MOVIMENTOS

        //Movimentos do pesfrsonagem
        if(esq == true)//se a tecla esquerda for pressionada
        {
            personX -= .65; //PRECISEI MUDAR A VELOCIDADE DE 8 P/ .8 (aqui para esquerda)
        }
        else if (dir == true)
        {
            personX += .65 ;//anda .8 para direita
        }
        //lógica personagem
        if (personX < 0)
            {
                personX = 0;
            }
        else if (personX + personComp > 600)
        {
            personX = 600 - personComp;
        }



        //Movimento do inimigo
        inimigoX += velX;
        for (int n=0; n < BLOCOS; n++)
        {
            if (blocos[n].vivo == true)
            {
                if (colisao(inimigoX, inimigoY, inimigoCA, inimigoCA, blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].alt) == true) //Houve colisão com o bloco atual?
                {
                    velX = -velX;
                    blocos[n].vivo = false;
                    break;
                }
            }
        }

        inimigoY += velY;
        for (int n=0; n < BLOCOS; n++)
        {
            if (blocos[n].vivo == true)
            {
                if (colisao(inimigoX, inimigoY, inimigoCA, inimigoCA, blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].alt) == true) // se houve colisao com o bloco
                {
                    velY = -velY;
                    blocos[n].vivo = false;
                    break;
                }
            }
        }
        //logica do inimigo
        if (inimigoX < 0)
            {
                velX = -velX;
            }
        else if (inimigoX + inimigoCA > 600)
            {
                velX = -velX;
            }
        else if (inimigoY < 0)
            {
                velY = -velY;
            }
        else if (inimigoY + inimigoCA > 400)
            {

                velY = -velY;
                executando=false;
            }

        if (colisao(inimigoX, inimigoY, inimigoCA,inimigoCA, personX, personY, personComp, personAlt) == true)
        {
            velY = -velY;
        }


        //-----------Rendeziração
        glClear(GL_COLOR_BUFFER_BIT); //Limpar o buffer (zera-o)

        glPushMatrix();//abre matriz

        glOrtho(0, 600, 400, 0, -1, 1 ); //dimensões da matriz

        glColor4ub(255, 0, 0, 255); //define cores: no caso vermelho

        glBegin(GL_QUADS); //inicia desenho do personagem

        //A ORDEM IMPORTA PARA FORMAR O DESENHO
        glVertex2f(personX, personY); //primeiro ponto
        glVertex2f(personX + personComp, personY);// segundo ponto
        glVertex2f(personX + personComp, personY + personAlt);
        glVertex2f(personX, personY+personAlt);

        glEnd(); // fecha desenho

        glColor4ub(0, 255, 0, 255);
        glBegin(GL_QUADS); // iniciando desenho do inimigo

        glVertex2d(inimigoX, inimigoY);
        glVertex2d(inimigoX + inimigoCA, inimigoY);
        glVertex2d(inimigoX + inimigoCA, inimigoY + inimigoCA);
        glVertex2d(inimigoX, inimigoY + inimigoCA);

        glEnd();


        glBegin(GL_QUADS);

        glColor4ub(0, 0, 255, 255);//cor azul p/o o bloco

        for (int n=0; n < BLOCOS; n++)//criação dos blocos em cima
        {
            if (blocos[n].vivo == true)
            {
                glVertex2f(blocos[n].x, blocos[n].y);
                glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y);
                glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y + blocos[n].alt);
                glVertex2f(blocos[n].x , blocos[n].y + blocos[n].alt);
            }
        }


        glEnd();

        glPopMatrix();//fecha matriz

        SDL_GL_SwapBuffers(); //função de animações


    }//termina loop while


    SDL_GL_SwapBuffers(); //função de animações

    glDisable(GL_BLEND);

    printf("\n Executando \n");

    SDL_Quit();


return 0;
}
