#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <SDL2/SDL_mixer.h>

using namespace std;

//global variables
#define W 1420
#define H 700
#define vy 16
#define yFloor 635
#define hPlayer 110
#define wPlayer 80
#define dShoe 50
#define lBall 45
#define g 0.6
#define dampy 0.8
#define dampx 0.9
#define vxShoot 13
#define vyfShoot 10
#define vyuShoot 15
#define vydShoot 10
#define PI 3.14

//global variables
bool run=false,pause=false,game=true,menu=true,options=false,leaderboard=false,music=true;

double x1=W-200-wPlayer,x2=200 ,y1=yFloor-hPlayer,y2=yFloor-hPlayer,x=W/2-lBall/2,y=yFloor-lBall-400;
double dy1=0 , dy2=0,dx1=0,dx2=0,i=0;
double vyBall=0 , vxBall=0 , c=0;
int goal1=0 , goal2=0;
int x4=x1,x3=x1,x5=x2,x6=x2;
bool freez1=false,freez2=false,invisibleball=false,tripleclone1=false,tripleclone2=false,fireball1=false,fireball2=false,power1=true,power2=true;
double tpower1=0, tpower2=0, tLeftPower1=8000 , tLeftPower2=8000;
double tfreeze1=0,tfreeze2=0, tinvisibleball=0, ttripleclone1=0, ttripleclone2=0;
double tLeftFreeze1=3000 , tLeftFreeze2=3000 , tLeftInvisibleball=3000 , tLeftTripleclone1=8000 , tLeftTripleclone2=8000;
double rel,norm,bounce,BALL_SPEED;

int s=0,t=0 ,u1=0,u2=0;
double G1=255 , B1=255 , G2=255 , B2=255;

const char* player1Name="player1";
const char* player2Name="player2";
const char* timerText="";
const char* goal1Text="0";
const char* goal2Text="0";
string goal1str="" , goal2str="";
string temp="";
string name1="player1" , name2="player2";

const char* firstChamp=" ";
const char* secondChamp=" ";
const char* thirdChamp=" ";
string first="" , second="" , third="";
int win1=0 , win2=0 , win3=0;

int tLeft=90 , tNow=0 , tStart=0;

SDL_Event *e = new SDL_Event();

SDL_Color player1Color={0,0,255};
SDL_Color player2Color={255,0,0};
SDL_Color timerColor={180,255,150};
SDL_Color goalColor={200,40,190};
SDL_Color endGameColor={255,128,0};
SDL_Color champColor={218,165,32};

SDL_Window *m_window;
SDL_Renderer *m_renderer;
SDL_Surface* text_surface;
SDL_Texture* Message;
SDL_Texture* player1Tex;
SDL_Texture* player2Tex;
SDL_Texture* goal1Texture;
SDL_Texture* goal2Texture;
SDL_Texture* endGameTexture;
SDL_Texture* backgroundTexture;
SDL_Texture* gateTexture1;
SDL_Texture* gateTexture2;
SDL_Texture* champTexture;
SDL_Texture* tickTexture;


//functions
void texture (SDL_Renderer *m_renderer,int xp,int yp,string addressOfImage,int width,int height);
void endGame (TTF_Font *font);
void background (SDL_Rect player1_rect , SDL_Rect player2_rect , SDL_Rect goal1_rect , SDL_Rect goal2_rect , SDL_Rect timer_rect ,
                                SDL_Rect BG_rect , SDL_Rect gate_rect1 , SDL_Rect gate_rect2 , SDL_Rect tick1_rect , SDL_Rect tick2_rect);
void save ();
void load (TTF_Font *font);
bool isEmpty(ifstream& fin)
{
    return fin.peek() == ifstream::traits_type::eof();
}
void update_leaderboard (string winner);
void read_leaderboard ();
void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian );


int main( int argc, char * argv[] )
{
    //initialization
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_OpenAudio (44100 , MIX_DEFAULT_FORMAT , 2 , 2048);
    Mix_Music* BGsound=Mix_LoadMUS("Khonya-Eros-320.mp3");
    Mix_PlayMusic (BGsound , -1);
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER ;
    Uint32 WND_flags = SDL_WINDOW_SHOWN;// | SDL_WINDOW_FULLSCREEN_DESKTOP;// |SDL_WINDOW_BORDERLESS;
    //Get screen resolution
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    SDL_CreateWindowAndRenderer( W, H, WND_flags, &m_window, &m_renderer );
    //Pass the focus to the drawing window
    SDL_RaiseWindow(m_window);

    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
    SDL_RenderClear( m_renderer );

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", 30);

    SDL_Rect timer_rect;
    timer_rect.x = W/2-50;
    timer_rect.y = 10;
    timer_rect.w = 100;
    timer_rect.h = 50;
    SDL_Rect player1_rect;
    player1_rect.x=W-150-10;
    player1_rect.y=10;
    player1_rect.w=150;
    player1_rect.h=75;
    SDL_Rect player2_rect;
    player2_rect.x=10;
    player2_rect.y=10;
    player2_rect.w=150;
    player2_rect.h=75;
    SDL_Rect goal1_rect;
    goal1_rect.x=W-10-150-50-50;
    goal1_rect.y=10;
    goal1_rect.w=50;
    goal1_rect.h=75;
    SDL_Rect goal2_rect;
    goal2_rect.x=10+150+50;
    goal2_rect.y=10;
    goal2_rect.w=50;
    goal2_rect.h=75;
    SDL_Rect BG_rect;
    BG_rect.x=0;
    BG_rect.y=0;
    BG_rect.w=W;
    BG_rect.h=H;
    SDL_Rect gate_rect1;
    gate_rect1.x=W-200;
    gate_rect1.y=350;
    gate_rect1.w=200;
    gate_rect1.h=300;
    SDL_Rect gate_rect2;
    gate_rect2.x=0;
    gate_rect2.y=350;
    gate_rect2.w=200;
    gate_rect2.h=300;
    SDL_Rect player1Name_rect;
    player1Name_rect.x=W/2-100;
    player1Name_rect.y=120;
    player1Name_rect.w=200;
    player1Name_rect.h=60;
    SDL_Rect player2Name_rect;
    player2Name_rect.x=W/2-100;
    player2Name_rect.y=220;
    player2Name_rect.w=200;
    player2Name_rect.h=60;
    SDL_Rect first_rect;
    first_rect.x=W/2-150;
    first_rect.y=100;
    first_rect.w=300;
    first_rect.h=60;
    SDL_Rect second_rect;
    second_rect.x=W/2-150;
    second_rect.y=200;
    second_rect.w=300;
    second_rect.h=60;
    SDL_Rect third_rect;
    third_rect.x=W/2-150;
    third_rect.y=300;
    third_rect.w=300;
    third_rect.h=60;
    SDL_Rect tick1_rect;
    tick1_rect.x=W-250;
    tick1_rect.y=90;
    tick1_rect.w=50;
    tick1_rect.h=50;
    SDL_Rect tick2_rect;
    tick2_rect.x=210;
    tick2_rect.y=90;
    tick2_rect.w=50;
    tick2_rect.h=50;


    backgroundTexture=IMG_LoadTexture(m_renderer,"background.JPG");
    SDL_RenderCopy( m_renderer, backgroundTexture, NULL, &BG_rect);
    gateTexture1=IMG_LoadTexture(m_renderer ,  "gater-min.png");
    SDL_RenderCopy(m_renderer , gateTexture1 , NULL , &gate_rect1);
    gateTexture2=IMG_LoadTexture(m_renderer ,  "gatel-min.png");
    SDL_RenderCopy(m_renderer , gateTexture2 , NULL , &gate_rect2);
    tickTexture=IMG_LoadTexture(m_renderer , "tick.png");
    text_surface=TTF_RenderText_Solid(font, timerText, timerColor);
    Message = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    text_surface=TTF_RenderText_Solid(font,player1Name, player1Color);
    player1Tex = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    text_surface=TTF_RenderText_Solid(font,player2Name, player2Color);
    player2Tex = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    text_surface=TTF_RenderText_Solid(font,goal1Text, goalColor);
    goal1Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    text_surface=TTF_RenderText_Solid(font,goal2Text, goalColor);
    goal2Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
//    SDL_RenderCopy(m_renderer, Message, NULL, &timer_rect);

while(game)
{
    //menu
    if (menu){
        SDL_RenderCopy( m_renderer, backgroundTexture, NULL, &BG_rect);
        SDL_RenderCopy(m_renderer , gateTexture1 , NULL , &gate_rect1);
        SDL_RenderCopy(m_renderer , gateTexture2 , NULL , &gate_rect2);
        texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
        texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
        texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
        texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
        texture(m_renderer , W/2-100 , 100 , "newgame.png" , 200 , 100);
        texture(m_renderer , W/2-100 , 200 , "loadgame.png" , 200 , 100);
        texture(m_renderer , W/2-100 , 380 , "options.png" , 200 , 50);
        texture(m_renderer , W/2-100 , 500 , "exit.png" , 200 , 100);
        SDL_RenderPresent(m_renderer);
    }
    while(menu){
        SDL_PollEvent(e);
        if (e->type==SDL_MOUSEBUTTONDOWN){
            //new game
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=100 && e->button.y<=200){
                x1=W-200-wPlayer; y1=yFloor-hPlayer;
                x2=200; y2=yFloor-hPlayer;
                x=W/2-lBall/2; y=yFloor-lBall-400;
                dx1=0; dy1=0;
                dx2=0; dy2=0;
                power1=true; power2=true;
                tpower1=0; tpower2=0;
                tLeftPower1=8000; tLeftPower2=8000;
                invisibleball=false;
                tinvisibleball=0;
                tLeftInvisibleball=3000;
                freez1=false; freez2=false;
                tfreeze1=0,tfreeze2=0;
                tLeftFreeze1=3000; tLeftFreeze2=3000;
                tripleclone1=false; tripleclone2=false;
                ttripleclone1=0; ttripleclone2=0;
                tLeftTripleclone1=8000; tLeftTripleclone2=8000;
                fireball1=false; fireball2=false;
                vxBall=0; vyBall=0;
                goal1=0; goal2=0;
                goal1Text="0"; goal2Text="0";
                goal1str="" ; goal2str="";
                timerText=""; temp="";
                tLeft=90; tNow=0 ; tStart=0;
                text_surface=TTF_RenderText_Solid(font,goal1Text, goalColor);
                goal1Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
                text_surface=TTF_RenderText_Solid(font,goal2Text, goalColor);
                goal2Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
                menu=false;
                run=true;
            }

            //load game
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=200 && e->button.y<=300){
                load(font);
                menu=false;
                run=true;
            }

            //options
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=380 && e->button.y<=430){
                menu=false;
                options=true;
            }

            //exit
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=530 && e->button.y<=600){
                menu=false;
                game=false;
            }
        }
    }



    //options
    if (options){
        SDL_RenderCopy( m_renderer, backgroundTexture, NULL, &BG_rect);
        SDL_RenderCopy(m_renderer , gateTexture1 , NULL , &gate_rect1);
        SDL_RenderCopy(m_renderer , gateTexture2 , NULL , &gate_rect2);
        texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
        texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
        texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
        texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
        texture(m_renderer , W/2-100 , 20 , "leaderboard.png" , 200 , 60);
        texture(m_renderer , W/2-100 , 120 , "playerName.png" , 200 , 60);
        texture(m_renderer , W/2-100 , 220 , "playerName.png" , 200 , 60);
        texture(m_renderer , W/2-100 , 315 , "sound.png" , 200 , 50);
        texture(m_renderer , W/2-100 , 420 , "mainmenu.png" , 200 , 100);
        SDL_RenderCopy(m_renderer, player1Tex, NULL, &player1Name_rect);
        SDL_RenderCopy(m_renderer, player2Tex, NULL, &player2Name_rect);
        SDL_RenderPresent(m_renderer);
    }
    while (options){
        SDL_PollEvent(e);
        if (e->type==SDL_MOUSEBUTTONDOWN){
            //leaderboard
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=20 && e->button.y<=80){
                leaderboard=true;
                options=false;
            }

            //player1Name
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=120 && e->button.y<=180){
                SDL_StartTextInput();
                if (SDL_IsTextInputActive()==SDL_TRUE){
                    bool change=false , input=true;
                    while (input){
                        while (SDL_PollEvent(e)){
                            if (e->type==SDL_TEXTINPUT){
                                name1+=e->text.text;
                                change=true;
                            }

                            if (e->type==SDL_KEYDOWN){
                                if (e->key.keysym.sym==SDLK_BACKSPACE && name1.length() > 0 ){
                                    name1.pop_back();
                                    change = true;
                                }

                                if (e->key.keysym.sym==SDLK_RETURN) input=false;
                            }

                            if (change && name1!=""){
                                if (name1[ 0 ]==' ') name1.erase(0,1);
                                player1Name=name1.c_str();
                                text_surface=TTF_RenderText_Solid(font, player1Name , player1Color);
                                player1Tex=SDL_CreateTextureFromSurface(m_renderer , text_surface);

                                SDL_RenderCopy( m_renderer, backgroundTexture, NULL, &BG_rect);
                                SDL_RenderCopy(m_renderer , gateTexture1 , NULL , &gate_rect1);
                                SDL_RenderCopy(m_renderer , gateTexture2 , NULL , &gate_rect2);
                                texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                                texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                                texture(m_renderer , W/2-100 , 20 , "leaderboard.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 120 , "playerName.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 220 , "playerName.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 315 , "sound.png" , 200 , 50);
                                texture(m_renderer , W/2-100 , 420 , "mainmenu.png" , 200 , 100);
                                SDL_RenderCopy(m_renderer, player1Tex, NULL, &player1Name_rect);
                                SDL_RenderCopy(m_renderer, player2Tex, NULL, &player2Name_rect);
                                SDL_RenderPresent(m_renderer);
                            }
                            else if (change && name1=="") {
                                name1+=" ";
                                player1Name=name1.c_str();
                                text_surface=TTF_RenderText_Solid(font, player1Name , player1Color);
                                player1Tex=SDL_CreateTextureFromSurface(m_renderer , text_surface);

                                SDL_RenderCopy( m_renderer, backgroundTexture, NULL, &BG_rect);
                                SDL_RenderCopy(m_renderer , gateTexture1 , NULL , &gate_rect1);
                                SDL_RenderCopy(m_renderer , gateTexture2 , NULL , &gate_rect2);
                                texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                                texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                                texture(m_renderer , W/2-100 , 20 , "leaderboard.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 120 , "playerName.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 220 , "playerName.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 315 , "sound.png" , 200 , 50);
                                texture(m_renderer , W/2-100 , 420 , "mainmenu.png" , 200 , 100);
                                SDL_RenderCopy(m_renderer, player1Tex, NULL, &player1Name_rect);
                                SDL_RenderCopy(m_renderer, player2Tex, NULL, &player2Name_rect);
                                SDL_RenderPresent(m_renderer);
                            }
                        }
                    }
                }
                SDL_StopTextInput();
            }

            //player2Name
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=220 && e->button.y<=280){
                SDL_StartTextInput();
                if (SDL_IsTextInputActive()==SDL_TRUE){
                    bool change=false , input=true;
                    while (input){
                        while (SDL_PollEvent(e)){
                            if (e->type==SDL_TEXTINPUT){
                                name2+=e->text.text;
                                change=true;
                            }

                            if (e->type==SDL_KEYDOWN){
                                if (e->key.keysym.sym==SDLK_BACKSPACE && name2.length() > 0 ){
                                    name2.pop_back();
                                    change = true;
                                }

                                if (e->key.keysym.sym==SDLK_RETURN) input=false;
                            }

                            if (change && name2!=""){
                                if (name2[ 0 ]==' ') name2.erase(0,1);
                                player2Name=name2.c_str();
                                text_surface=TTF_RenderText_Solid(font, player2Name , player2Color);
                                player2Tex=SDL_CreateTextureFromSurface(m_renderer , text_surface);

                                SDL_RenderCopy( m_renderer, backgroundTexture, NULL, &BG_rect);
                                SDL_RenderCopy(m_renderer , gateTexture1 , NULL , &gate_rect1);
                                SDL_RenderCopy(m_renderer , gateTexture2 , NULL , &gate_rect2);
                                texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                                texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                                texture(m_renderer , W/2-100 , 20 , "leaderboard.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 120 , "playerName.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 220 , "playerName.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 315 , "sound.png" , 200 , 50);
                                texture(m_renderer , W/2-100 , 420 , "mainmenu.png" , 200 , 100);
                                SDL_RenderCopy(m_renderer, player1Tex, NULL, &player1Name_rect);
                                SDL_RenderCopy(m_renderer, player2Tex, NULL, &player2Name_rect);
                                SDL_RenderPresent(m_renderer);
                            }
                            else if (change && name2=="") {
                                name2+=" ";
                                player2Name=name2.c_str();
                                text_surface=TTF_RenderText_Solid(font, player2Name , player2Color);
                                player2Tex=SDL_CreateTextureFromSurface(m_renderer , text_surface);

                                SDL_RenderCopy( m_renderer, backgroundTexture, NULL, &BG_rect);
                                SDL_RenderCopy(m_renderer , gateTexture1 , NULL , &gate_rect1);
                                SDL_RenderCopy(m_renderer , gateTexture2 , NULL , &gate_rect2);
                                texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                                texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                                texture(m_renderer , W/2-100 , 20 , "leaderboard.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 120 , "playerName.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 220 , "playerName.png" , 200 , 60);
                                texture(m_renderer , W/2-100 , 315 , "sound.png" , 200 , 50);
                                texture(m_renderer , W/2-100 , 420 , "mainmenu.png" , 200 , 100);
                                SDL_RenderCopy(m_renderer, player1Tex, NULL, &player1Name_rect);
                                SDL_RenderCopy(m_renderer, player2Tex, NULL, &player2Name_rect);
                                SDL_RenderPresent(m_renderer);
                            }
                        }
                    }
                }
                SDL_StopTextInput();
            }

            //sound
            if (e->button.x>=W/2-90 && e->button.x<=W/2+90 && e->button.y>=315 && e->button.y<=365){
                if (music){
                    Mix_FreeMusic(BGsound);
                    music=false;
                    SDL_Delay(100);
                }
                else {
                    Mix_Music* BGsound=Mix_LoadMUS("Khonya-Eros-320.mp3");
                    Mix_PlayMusic(BGsound , -1);
                    music=true;
                    SDL_Delay(100);
                }
            }

            //main menu
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=460 && e->button.y<=500){
                options=false;
                menu=true;
            }
        }
    }



    //leaderboard
    if (leaderboard){
        SDL_RenderCopy( m_renderer, backgroundTexture, NULL, &BG_rect);
        SDL_RenderCopy(m_renderer , gateTexture1 , NULL , &gate_rect1);
        SDL_RenderCopy(m_renderer , gateTexture2 , NULL , &gate_rect2);
        texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
        texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
        texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
        texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
        texture(m_renderer , W/2-150 , 100 , "playerName.png" , 300 , 60);
        texture(m_renderer , W/2-150 , 200 , "playerName.png" , 300 , 60);
        texture(m_renderer , W/2-150 , 300 , "playerName.png" , 300 , 60);
        texture(m_renderer , W/2-100 , 380 , "options.png" , 200 , 60);

        read_leaderboard();
        string champ=first+' '+to_string(win1);
        text_surface=TTF_RenderText_Solid(font, champ.c_str() , champColor);
        champTexture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
        SDL_RenderCopy(m_renderer, champTexture, NULL, &first_rect);
        champ=second+' '+to_string(win2);
        text_surface=TTF_RenderText_Solid(font, champ.c_str() , champColor);
        champTexture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
        SDL_RenderCopy(m_renderer, champTexture, NULL, &second_rect);
        champ=third+' '+to_string(win3);
        text_surface=TTF_RenderText_Solid(font, champ.c_str() , champColor);
        champTexture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
        SDL_RenderCopy(m_renderer, champTexture, NULL, &third_rect);

        SDL_RenderPresent(m_renderer);
    }
    while(leaderboard){
        SDL_PollEvent(e);
        if (e->type==SDL_MOUSEBUTTONDOWN){
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=380 && e->button.y<=440){
                options=true;
                leaderboard=false;
            }
        }
    }



    //game
    tStart=clock();
    if (!power1) tpower1=clock();
    if (!power2) tpower2=clock();
    if (freez1) tfreeze1=clock();
    if (freez2) tfreeze2=clock();
    if (invisibleball) tinvisibleball=clock();
    if (tripleclone1) ttripleclone1=clock();
    if (tripleclone2) ttripleclone2=clock();
    while (run){
        background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect , gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
        texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
        texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
        texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
        texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
        if(tripleclone1)
        {
            //two other clones of the right player
            texture(m_renderer , x4-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
            texture(m_renderer , x4 , y1 , "player1.png" , 80 , 80);
            texture(m_renderer , x3 , y1+dShoe , "shoeR-min.png" , 60 , 60);
            texture(m_renderer , x3+10 , y1 , "player1.png" , 80 , 80);
        }
        if(tripleclone2)
        {
            //two other clones of the left player
            texture(m_renderer , x6+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
            texture(m_renderer , x6 , y2 , "player2.png" , 80 , 80);
            texture(m_renderer , x5+40 , y2+dShoe , "shoeL-min.png" , 60 , 60);
            texture(m_renderer , x5+10 , y2 , "player2.png" , 80 , 80);
        }

        //timer
        tNow=tLeft-(clock()-tStart)/1000;
        temp=to_string(tNow);
        timerText=temp.c_str();
        text_surface=TTF_RenderText_Solid(font, timerText, timerColor);
        Message = SDL_CreateTextureFromSurface(m_renderer, text_surface);

        //keyboard
        SDL_PollEvent(e);
        const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
        if( e->type == SDL_KEYDOWN || e->type==SDL_KEYUP )
        {
            if (e->key.keysym.sym==SDLK_ESCAPE){
                run=false;
                pause=true;
            }

            if (keyboardState[SDL_SCANCODE_D] && !freez2){
                if (!(x2+wPlayer>=W-85||(x2+85>=x1&&x2<=x1&&y2+70>=y1&&y2<=y1+70)))
                {
                    dx2=8;
                    x2+=dx2;
                }
            }

            if (keyboardState[SDL_SCANCODE_A] && !freez2){
                if (!(x2<=115||(x1+80>=x2&&x2>=x1&&y2+70>=y1&&y2<=y1+70)))
                {
                    dx2=-8;
                    x2+=dx2;
                }
            }

            if (keyboardState[SDL_SCANCODE_W] && !freez2){
                if (y2>=yFloor-hPlayer) dy2=-vy;
            }

            if (keyboardState[SDL_SCANCODE_Q] && !freez2){
                background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                texture(m_renderer , x2+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                if(tripleclone2)
                {
                    texture(m_renderer , x6+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                    texture(m_renderer , x6 , y2 , "player2.png" , 80 , 80);
                    texture(m_renderer , x5+40 , y2+dShoe , "image2-min.png" , 60 , 60);
                    texture(m_renderer , x5+10 , y2 , "player2.png" , 80 , 80);
                }
                if(tripleclone1)
                {
                    texture(m_renderer , x4-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x4 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x3 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x3+10 , y1 , "player1.png" , 80 , 80);
                }

                if (x>=x2 && x<=x2+wPlayer+20 && y+lBall>=y2+dShoe+10 && y+lBall<=y2+hPlayer+10){
                    vxBall=vxShoot;
                    vyBall=-vyfShoot;
                }
                if (x>=x2 && x<=x2+wPlayer+20 && y+lBall<y2+dShoe+10 && y+lBall>=y2+dShoe-10){
                    vxBall=vxShoot;
                    vyBall=-vyuShoot;
                }
                if (x>=x2 && x<=x2+wPlayer+20 && y<=y2+hPlayer+5 && y+lBall>y2+hPlayer+10){
                    vxBall=vxShoot;
                    vyBall=-vydShoot;
                }
                //two other clones can shoot the ball
                if(tripleclone2)
                {
                    if (x>=x5 && x<=x5+wPlayer+20 && y+lBall>=y2+dShoe+10 && y+lBall<=y2+hPlayer+10){
                        vxBall=vxShoot;
                        vyBall=-vyfShoot;
                    }
                    if (x>=x5 && x<=x5+wPlayer+20 && y+lBall<y2+dShoe+10 && y+lBall>=y2+dShoe-10){
                        vxBall=vxShoot;
                        vyBall=-vyuShoot;
                    }
                    if (x>=x5 && x<=x5+wPlayer+20 && y<=y2+hPlayer+5 && y+lBall>y2+hPlayer+10){
                        vxBall=vxShoot;
                        vyBall=-vydShoot;
                    }

                    if (x>=x6 && x<=x6+wPlayer+20 && y+lBall>=y2+dShoe+10 && y+lBall<=y2+hPlayer+10){
                        vxBall=vxShoot;
                        vyBall=-vyfShoot;
                    }
                    if (x>=x6 && x<=x6+wPlayer+20 && y+lBall<y2+dShoe+10 && y+lBall>=y2+dShoe-10){
                        vxBall=vxShoot;
                        vyBall=-vyuShoot;
                    }
                    if (x>=x6 && x<=x6+wPlayer+20 && y<=y2+hPlayer+5 && y+lBall>y2+hPlayer+10){
                        vxBall=vxShoot;
                        vyBall=-vydShoot;
                    }
                }
                SDL_RenderPresent(m_renderer);
                SDL_Delay(16);
            }

            if (keyboardState[SDL_SCANCODE_LEFT] && !freez1){
                if(!(x1<=115||(x2<=x1&&x1<=x2+85&&y1+70>=y2&&y1<=y2+70)))
                {
                    dx1=-8;
                    x1+=dx1;
                }
            }

            if (keyboardState[SDL_SCANCODE_RIGHT] && !freez1){
                 if(!(x1+wPlayer>=W-115||(x1+80>=x2&&x2>=x1&&y1+70>=y2&&y1<=y2+70)))
                {
                    dx1=8;
                    x1+=dx1;
                }
            }

            if (keyboardState[SDL_SCANCODE_UP] && !freez1){
                if (y1>=yFloor-hPlayer) dy1=-vy;
            }

            if (keyboardState[SDL_SCANCODE_P] && !freez1){
                background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                texture(m_renderer , x1-10 , y1+dShoe , "image1-min.png" , 60 , 60);
                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                if(tripleclone1)
                {
                    texture(m_renderer , x4-10 , y1+dShoe , "image1-min.png" , 60 , 60);
                    texture(m_renderer , x4 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x3 , y1+dShoe , "image1-min.png" , 60 , 60);
                    texture(m_renderer , x3+10 , y1 , "player1.png" , 80 , 80);
                }
                if(tripleclone2)
                {
                    texture(m_renderer , x6+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                    texture(m_renderer , x6 , y2 , "player2.png" , 80 , 80);
                    texture(m_renderer , x5+40 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                    texture(m_renderer , x5+10 , y2 , "player2.png" , 80 , 80);
                }

                if (x+lBall<=x1+wPlayer && x+lBall>=x1-20 && y+lBall>=y1+dShoe+10 && y+lBall<=y1+hPlayer+10){
                    vxBall=-vxShoot;
                    vyBall=-vyfShoot;
                }
                if (x+lBall<=x1+wPlayer && x+lBall>=x1-20 && y+lBall<y1+dShoe+10 && y+lBall>=y1+dShoe-10){
                    vxBall=-vxShoot;
                    vyBall=-vyuShoot;
                }
                if (x+lBall<=x1+wPlayer && x+lBall>=x1-20 && y<=y1+hPlayer+5 && y+lBall>y1+hPlayer+10){
                    vxBall=-vxShoot;
                    vyBall=vydShoot;
                }
                //two other clones can shoot the ball
                if(tripleclone1)
                {
                    if (x+lBall<=x3+wPlayer && x+lBall>=x3-20 && y+lBall>=y1+dShoe+10 && y+lBall<=y1+hPlayer+10){
                        vxBall=-vxShoot;
                        vyBall=-vyfShoot;
                    }
                    if (x+lBall<=x3+wPlayer && x+lBall>=x3-20 && y+lBall<y1+dShoe+10 && y+lBall>=y1+dShoe-10){
                        vxBall=-vxShoot;
                        vyBall=-vyuShoot;
                    }
                    if (x+lBall<=x3+wPlayer && x+lBall>=x3-20 && y<=y1+hPlayer+5 && y+lBall>y1+hPlayer+10){
                        vxBall=-vxShoot;
                        vyBall=vydShoot;
                    }

                    if (x+lBall<=x4+wPlayer && x+lBall>=x4-20 && y+lBall>=y1+dShoe+10 && y+lBall<=y1+hPlayer+10){
                        vxBall=-vxShoot;
                        vyBall=-vyfShoot;
                    }
                    if (x+lBall<=x4+wPlayer && x+lBall>=x4-20 && y+lBall<y1+dShoe+10 && y+lBall>=y1+dShoe-10){
                        vxBall=-vxShoot;
                        vyBall=-vyuShoot;
                    }
                    if (x+lBall<=x4+wPlayer && x+lBall>=x4-20 && y<=y1+hPlayer+5 && y+lBall>y1+hPlayer+10){
                        vxBall=-vxShoot;
                        vyBall=vydShoot;
                    }
                }
                SDL_RenderPresent(m_renderer);
                SDL_Delay(16);
            }

            //invisible ball power for the right player
            if (keyboardState[SDL_SCANCODE_O] && !freez1 && power1){
                tinvisibleball=clock();
                background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                texture(m_renderer , x1-10 , y1+dShoe , "image1-min.png" , 60 , 60);
                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                if(tripleclone2)
                {
                    texture(m_renderer , x6+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                    texture(m_renderer , x6 , y2 , "player2.png" , 80 , 80);
                    texture(m_renderer , x5+40 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                    texture(m_renderer , x5+10 , y2 , "player2.png" , 80 , 80);
                }

                if (x+lBall<=x1+wPlayer && x+lBall>=x1-20 && y+lBall>=y1+dShoe+10 && y+lBall<=y1+hPlayer+10){
                    vxBall=-vxShoot;
                    vyBall=-vyfShoot;
                    invisibleball=true;
                    power1=false;
                    tpower1=clock();
                    tLeftPower1=3000;
                }
                if (x+lBall<=x1+wPlayer && x+lBall>=x1-20 && y+lBall<y1+dShoe+10 && y+lBall>=y1+dShoe-10){
                    vxBall=-vxShoot;
                    vyBall=-vyuShoot;
                    invisibleball=true;
                    power1=false;
                    tpower1=clock();
                    tLeftPower1=3000;
                }
                if (x+lBall<=x1+wPlayer && x+lBall>=x1-20 && y<=y1+hPlayer+5 && y+lBall>y1+hPlayer+10){
                    vxBall=-vxShoot;
                    vyBall=vydShoot;
                    invisibleball=true;
                    power1=false;
                    tpower1=clock();
                    tLeftPower1=3000;
                }
                SDL_RenderPresent(m_renderer);
                SDL_Delay(16);
            }

            //triple clone power for the right player
            if (keyboardState[SDL_SCANCODE_L] && !freez1 && power1)
            {
                tripleclone1=true;
                ttripleclone1=clock();
                power1=false;
                tpower1=clock();
                tLeftPower1=8000;
            }

            //freeze power for the right player
            if (keyboardState[SDL_SCANCODE_M] && !freez1 && power1){
                background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                texture(m_renderer , x1-10 , y1+dShoe , "image1-min.png" , 60 , 60);
                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                if(tripleclone2)
                {
                    texture(m_renderer , x6+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                    texture(m_renderer , x6 , y2 , "player2.png" , 80 , 80);
                    texture(m_renderer , x5+40 , y2+dShoe , "image2-min.png" , 60 , 60);
                    texture(m_renderer , x5+10 , y2 , "player2.png" , 80 , 80);
                }

                if (x2<=x1 && x2+wPlayer>=x1-20 && y2<=y1+hPlayer && y2+hPlayer>=y1+dShoe-10){
                    freez2=true;
                    tfreeze2=clock();
                    power1=false;
                    tpower1=clock();
                    tLeftPower1=3000;
                }
                SDL_RenderPresent(m_renderer);
                SDL_Delay(16);
            }

            //fireball power for the right player
            if(keyboardState[SDL_SCANCODE_N] && !freez1 && power1)
            {
                background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                texture(m_renderer , x1-10 , y1+dShoe , "image1-min.png" , 60 , 60);
                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                if(tripleclone2)
                {
                    texture(m_renderer , x6+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                    texture(m_renderer , x6 , y2 , "player2.png" , 80 , 80);
                    texture(m_renderer , x5+40 , y2+dShoe , "image2-min.png" , 60 , 60);
                    texture(m_renderer , x5+10 , y2 , "player2.png" , 80 , 80);
                }

                if (x1>=x && x>=x1-80 && y>=yFloor-lBall+5)
                {
                    fireball1=true;
                    vxBall=-95;
                    power1=false;
                    tpower1=clock();
                    tLeftPower1=5000;
                }
                else if ((x1>=x && x>=x1-80 && y+lBall>=y2+dShoe+10 && y+lBall<=y2+hPlayer+10)||(x1>=x && x>=x1-80 && y+lBall<y1+dShoe+10 &&
                            y+lBall>=y1+dShoe-10)||(x1>=x && x>=x1-80 && y<=y1+hPlayer+5 && y+lBall>y1+hPlayer+10))
                {
                    fireball1=true;
                    vxBall=-70;
                    power1=false;
                    tpower1=clock();
                }
                SDL_RenderPresent(m_renderer);
                SDL_Delay(16);
            }

            //invisible ball power for the left player
            if (keyboardState[SDL_SCANCODE_X] && !freez2 && power2){
                tinvisibleball=clock();
                background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                texture(m_renderer , x2+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                if(tripleclone1)
                {
                    texture(m_renderer , x4-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x4 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x3 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x3+10 , y1 , "player1.png" , 80 , 80);
                }

                if (x>=x2 && x<=x2+wPlayer+20 && y+lBall>=y2+dShoe+10 && y+lBall<=y2+hPlayer+10){
                    vxBall=vxShoot;
                    vyBall=-vyfShoot;
                    invisibleball=true;
                    power2=false;
                    tpower2=clock();
                    tLeftPower2=3000;
                }
                if (x>=x2 && x<=x2+wPlayer+20 && y+lBall<y2+dShoe+10 && y+lBall>=y2+dShoe-10){
                    vxBall=vxShoot;
                    vyBall=-vyuShoot;
                    invisibleball=true;
                    power2=false;
                    tpower2=clock();
                    tLeftPower2=3000;
                }
                if (x>=x2 && x<=x2+wPlayer+20 && y<=y2+hPlayer+5 && y+lBall>y2+hPlayer+10){
                    vxBall=vxShoot;
                    vyBall=-vydShoot;
                    invisibleball=true;
                    power2=false;
                    tpower2=clock();
                    tLeftPower2=3000;
                }
                SDL_RenderPresent(m_renderer);
                SDL_Delay(16);
            }

            //triple clone power for the left player
            if (keyboardState[SDL_SCANCODE_Z] && !freez2 && power2)
            {
                tripleclone2=true;
                ttripleclone2=clock();
                power2=false;
                tpower2=clock();
                tLeftPower2=8000;
            }

            //freeze power for the left player
            if (keyboardState[SDL_SCANCODE_E] && !freez2 && power2){
                background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                texture(m_renderer , x2+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                if(tripleclone1)
                {
                    //two other clones of the right player
                    texture(m_renderer , x4-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x4 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x3 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x3+10 , y1 , "player1.png" , 80 , 80);
                }

                if (x1>=x2 && x1<=x2+wPlayer+20 && y1<=y2+hPlayer && y1+hPlayer>=y2+dShoe-10){
                    freez1=true;
                    tfreeze1=clock();
                    power2=false;
                    tpower2=clock();
                    tLeftPower2=3000;
                }
                SDL_RenderPresent(m_renderer);
                SDL_Delay(16);
            }

            //fireball power for the left player
            if(keyboardState[SDL_SCANCODE_R] && !freez2 && power2)
            {
                background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                texture(m_renderer , x2+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                if(tripleclone1)
                {
                    //two other clones of the right player
                    texture(m_renderer , x4-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x4 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x3 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x3+10 , y1 , "player1.png" , 80 , 80);
                }

                if (x>=x2 && x<=x2+115 && y>=yFloor-lBall+5)
                {
                    fireball2=true;
                    vxBall=95;
                    power2=false;
                    tpower2=clock();
                    tLeftPower2=5000;
                }
                else if ((x>=x2 && x<=x2+115 && y+lBall<y2+dShoe+10 && y+lBall>=y2+dShoe-10)||(x>=x2 && x<=x2+115 && y<=y2+hPlayer+5 &&
                            y+lBall>y2+hPlayer+10)||(x>=x2 && x<=x2+115 && y+lBall>=y2+dShoe+10 && y+lBall<=y2+hPlayer+10))
                {
                    fireball2=true;
                    vxBall=70;
                    power2=false;
                    tpower2=clock();
                    tLeftPower2=5000;
                }
                SDL_RenderPresent(m_renderer);
                SDL_Delay(16);
            }

            //thief power for the right player
            //randomly choose one of the powers of the right player
            if (keyboardState[SDL_SCANCODE_K] && !freez1 && power1){
                srand ( time(NULL) );
                s=rand()%4;

                //triple clone
                if(s==0)
                {
                    tripleclone1=true;
                    ttripleclone1=clock();
                    power1=false;
                    tpower1=clock();
                    tLeftPower1=8000;
                }

                //invisible ball
                else if(s==1)
                {
                    tinvisibleball=clock();

                    background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                    texture(m_renderer , x1-10 , y1+dShoe , "image1-min.png" , 60 , 60);
                    texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                    texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                    if(tripleclone2)
                    {
                        //two other clones of the left player
                        texture(m_renderer , x6+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                        texture(m_renderer , x6 , y2 , "player2.png" , 80 , 80);
                        texture(m_renderer , x5+40 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                        texture(m_renderer , x5+10 , y2 , "player2.png" , 80 , 80);
                    }

                    if (x+lBall<=x1+wPlayer && x+lBall>=x1-20 && y+lBall>=y1+dShoe+10 && y+lBall<=y1+hPlayer+10){
                        vxBall=-vxShoot;
                        vyBall=-vyfShoot;
                        invisibleball=true;
                        power1=false;
                        tpower1=clock();
                        tLeftPower1=3000;
                    }
                    if (x+lBall<=x1+wPlayer && x+lBall>=x1-20 && y+lBall<y1+dShoe+10 && y+lBall>=y1+dShoe-10){
                        vxBall=-vxShoot;
                        vyBall=-vyuShoot;
                        invisibleball=true;
                        power1=false;
                        tpower1=clock();
                        tLeftPower1=3000;
                    }
                    if (x+lBall<=x1+wPlayer && x+lBall>=x1-20 && y<=y1+hPlayer+5 && y+lBall>y1+hPlayer+10){
                        vxBall=-vxShoot;
                        vyBall=vydShoot;
                        invisibleball=true;
                        power1=false;
                        tpower1=clock();
                        tLeftPower1=3000;
                    }
                    SDL_RenderPresent(m_renderer);
                    SDL_Delay(16);
                }

                //fireball
                else if(s==2)
                {
                    background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                    texture(m_renderer , x1-10 , y1+dShoe , "image1-min.png" , 60 , 60);
                    texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                    texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                    if(tripleclone2)
                    {
                        //two other clones of the left player
                        texture(m_renderer , x6+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                        texture(m_renderer , x6 , y2 , "player2.png" , 80 , 80);
                        texture(m_renderer , x5+40 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                        texture(m_renderer , x5+10 , y2 , "player2.png" , 80 , 80);
                    }

                    if (x1>=x && x>=x1-80 && y>=yFloor-lBall+5)
                    {
                        fireball1=true;
                        vxBall=-95;
                        power1=false;
                        tpower1=clock();
                        tLeftPower1=5000;
                    }
                    else if ((x1>=x && x>=x1-80 && y+lBall>=y2+dShoe+10 && y+lBall<=y2+hPlayer+10)||(x1>=x && x>=x1-80 && y+lBall<y1+dShoe+10 &&
                                y+lBall>=y1+dShoe-10)||(x1>=x && x>=x1-80 && y<=y1+hPlayer+5 && y+lBall>y1+hPlayer+10))
                    {
                        fireball1=true;
                        vxBall=-70;
                        power1=false;
                        tpower1=clock();
                        tLeftPower1=5000;
                    }
                    SDL_RenderPresent(m_renderer);
                    SDL_Delay(16);
                }

                //freeze
                else
                {
                    background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                    texture(m_renderer , x1-10 , y1+dShoe , "image1-min.png" , 60 , 60);
                    texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x2+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                    texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                    if(tripleclone2)
                    {
                        //two other clones of the left player
                        texture(m_renderer , x6+30 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                        texture(m_renderer , x6 , y2 , "player2.png" , 80 , 80);
                        texture(m_renderer , x5+40 , y2+dShoe , "shoeL-min.png" , 60 , 60);
                        texture(m_renderer , x5+10 , y2 , "player2.png" , 80 , 80);
                    }

                    if (x2<=x1 && x2+wPlayer>=x1-20 && y2<=y1+hPlayer && y2+hPlayer>=y1+dShoe-10){
                        freez2=true;
                        tfreeze2=clock();
                        power1=false;
                        tpower1=clock();
                        tLeftPower1=3000;
                    }
                    SDL_RenderPresent(m_renderer);
                    SDL_Delay(16);
                }
            }

            //thief power for the left player
            //randpmly choose one of the powewrs of the left player
            if (keyboardState[SDL_SCANCODE_F] && !freez2 && power2){
                srand ( time(NULL) );
                t=rand()%4;

                //freeze
                if(t==0){
                    background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                    texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x2+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                    texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                    if(tripleclone1)
                    {
                        //two other clones of the right player
                        texture(m_renderer , x4-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                        texture(m_renderer , x4 , y1 , "player1.png" , 80 , 80);
                        texture(m_renderer , x3 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                        texture(m_renderer , x3+10 , y1 , "player1.png" , 80 , 80);
                    }


                    if (x1>=x2 && x1<=x2+wPlayer+20 && y1<=y2+hPlayer && y1+hPlayer>=y2+dShoe-10){
                        freez1=true;
                        tfreeze1=clock();
                        power2=false;
                        tpower2=clock();
                        tLeftPower2=3000;
                    }
                    SDL_RenderPresent(m_renderer);
                    SDL_Delay(16);
                }

                //fireball
                else if(t==1)
                {
                    background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                    texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x2+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                    texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                    if(tripleclone1)
                    {
                        //two other clones of the right player
                        texture(m_renderer , x4-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                        texture(m_renderer , x4 , y1 , "player1.png" , 80 , 80);
                        texture(m_renderer , x3 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                        texture(m_renderer , x3+10 , y1 , "player1.png" , 80 , 80);
                    }

                    if (x>=x2 && x<=x2+wPlayer+160 && y>=yFloor-lBall+5)
                    {
                        fireball2=true;
                        vxBall=95;
                        power2=false;
                        tpower2=clock();
                        tLeftPower2=5000;
                    }
                    else if (x>=x2 && x<=x2+115 && y+lBall<y2+dShoe+10 && y+lBall>=y2+dShoe-10||(x>=x2 && x<=x2+115 && y<=y2+hPlayer+5 &&
                                y+lBall>y2+hPlayer+10)||(x>=x2 && x<=x2+115 && y+lBall>=y2+dShoe+10 && y+lBall<=y2+hPlayer+10))
                    {
                        fireball2=true;
                        vxBall=70;
                        power2=false;
                        tpower2=clock();
                        tLeftPower2=5000;
                    }
                    SDL_RenderPresent(m_renderer);
                    SDL_Delay(16);
                }

                //tripleclone
                else if(t==2)
                {
                    tripleclone2=true;
                    ttripleclone2=clock();
                    power2=false;
                    tpower2=clock();
                    tLeftPower2=8000;
                }

                //invisibleball
                else
                {
                    tinvisibleball=clock();

                    background(player1_rect , player2_rect , goal1_rect , goal2_rect , timer_rect , BG_rect, gate_rect1 , gate_rect2 , tick1_rect , tick2_rect);
                    texture(m_renderer , x1-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                    texture(m_renderer , x1 , y1 , "player1.png" , 80 , 80);
                    texture(m_renderer , x2+30 , y2+dShoe , "image2-min.png" , 60 , 60);
                    texture(m_renderer , x2 , y2 , "player2.png" , 80 , 80);
                    if(tripleclone1)
                    {
                        texture(m_renderer , x4-10 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                        texture(m_renderer , x4 , y1 , "player1.png" , 80 , 80);
                        texture(m_renderer , x3 , y1+dShoe , "shoeR-min.png" , 60 , 60);
                        texture(m_renderer , x3+10 , y1 , "player1.png" , 80 , 80);
                    }

                    if (x>=x2 && x<=x2+wPlayer+20 && y+lBall>=y2+dShoe+10 && y+lBall<=y2+hPlayer+10){
                        vxBall=vxShoot;
                        vyBall=-vyfShoot;
                        invisibleball=true;
                        power2=false;
                        tpower2=clock();
                        tLeftPower2=3000;
                    }
                    if (x>=x2 && x<=x2+wPlayer+20 && y+lBall<y2+dShoe+10 && y+lBall>=y2+dShoe-10){
                        vxBall=vxShoot;
                        vyBall=-vyuShoot;
                        invisibleball=true;
                        power2=false;
                        tpower2=clock();
                        tLeftPower2=3000;
                    }
                    if (x>=x2 && x<=x2+wPlayer+20 && y<=y2+hPlayer+5 && y+lBall>y2+hPlayer+10){
                        vxBall=vxShoot;
                        vyBall=-vydShoot;
                        invisibleball=true;
                        power2=false;
                        tpower2=clock();
                        tLeftPower2=3000;
                    }
                    SDL_RenderPresent(m_renderer);
                    SDL_Delay(16);
                }
            }
        }


        //power timers
        if (power1==false){
            G1=(clock()-tpower1)*255/tLeftPower1;
            B1=(clock()-tpower1)*255/tLeftPower1;
            rect(m_renderer,W-210,120,200,10,255,255-G1,255-B1,1);
            if (clock()-tpower1>tLeftPower1){
                power1=true;
            }
        }
        if (power2==false){
            G2=(clock()-tpower2)*255/tLeftPower2;
            B2=(clock()-tpower2)*255/tLeftPower2;
            rect(m_renderer,10,120,200,10,255,255-G2,255-B2,1);
            if(clock()-tpower2>tLeftPower2){
                power2=true;
            }
        }

        //freeze
        if (freez1){
            texture(m_renderer,x1+wPlayer/2-15,y1-30,"freeze.png",30,30);
            if (clock()-tfreeze1>tLeftFreeze1){
                freez1=false;
                tLeftFreeze1=3000;
            }
        }
        if (freez2){
            texture(m_renderer,x2+wPlayer/2-15,y2-30,"freeze.png",30,30);
            if (clock()-tfreeze2>tLeftFreeze2){
                freez2=false;
                tLeftFreeze2=3000;
            }
        }

        //invisible ball
        if(clock()-tinvisibleball>tLeftInvisibleball){
            invisibleball=false;
            tLeftInvisibleball=3000;
        }

        //triple clone
        if(clock()-ttripleclone1>tLeftTripleclone1){
            tripleclone1=false;
            tLeftTripleclone1=8000;
            x3=x1;
            x4=x1;
        }
        if(clock()-ttripleclone2>tLeftTripleclone2){
            tripleclone2=false;
            tLeftTripleclone2=8000;
            x5=x2;
            x6=x2;
        }


        //player movement
        if (y1>yFloor-hPlayer){
            dy1=0;
            y1=yFloor-hPlayer;
        }
        else if(y1+80>y2&&y1<y2&&x1<=x2+70&&x2<=x1+35)
        {
            dy1=0;
            y1=yFloor-2*hPlayer+30;
        }
        else dy1+=g;

        if (y2>yFloor-hPlayer){
            dy2=0;
            y2=yFloor-hPlayer;
        }
        else if(y2+80>y1&&y2<y1&&x1<=x2+70&&x2<=x1+35)
        {
            dy2=0;
            y2=yFloor-2*hPlayer+30;
        }
        else dy2+=g;

        y1+=dy1;
        y2+=dy2;

        //ball movement
        if (y+lBall<=yFloor+5) vyBall+=g;
        else if (y+lBall>=yFloor) {
            if (fabs(vxBall)>=4) vxBall*=dampx;
            else vxBall=0;
            if (vyBall>=6) vyBall=-dampy*vyBall;
            else if (vyBall<6 && vyBall>=0)
            {
                vyBall=0;
                y=yFloor-lBall+5;
            }
        }
        //ball reflection
        if(x>W-90-lBall || x<90) vxBall=-vxBall;
        if(y<22.5)
        {
            y=22.5;
            vyBall=-vyBall;
        }
        if(y<350 && y>300 && x>=W-185-lBall && vyBall>0) vyBall=-vyBall;
        if(y<400 && y>350 && x>=W-185-lBall && vyBall<0) vyBall=-vyBall;
        if(y<350 && y>300 && x<=185 && vyBall>0) vyBall=-vyBall;
        if(y<400 && y>350 && x<=185 && vyBall<0) vyBall=-vyBall;
        if(x<=W-90-lBall && x>=W-185-lBall &&  y<=350 && y>300 && vxBall==0) vxBall=-1;
        if(x>=90 && x<=185 && y<350 && y>300 && vxBall==0) vxBall=1;


        //goal
        if(x<90&&y>350)
        {
            goal1++;
            goal1str=to_string(goal1);
            goal1Text=goal1str.c_str();
            text_surface=TTF_RenderText_Solid(font,goal1Text, goalColor);
            goal1Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
            if (goal1<3) SDL_Delay(1500);
            x=W/2-lBall/2;
            y=yFloor-lBall-400;
            vxBall=0;
            vyBall=0;
            x1=W-200-wPlayer;
            x2=200;
            y1=yFloor-hPlayer;
            y2=yFloor-hPlayer;
        }
        if(x>W-90-lBall&&y>350)
        {
            goal2++;
            goal2str=to_string(goal2);
            goal2Text=goal2str.c_str();
            text_surface=TTF_RenderText_Solid(font,goal2Text, goalColor);
            goal2Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
            if (goal2<3) SDL_Delay(1500);
            x=W/2-lBall/2;
            y=yFloor-lBall-400;
            vxBall=0;
            vyBall=0;
            x1=W-200-wPlayer;
            x2=200;
            y1=yFloor-hPlayer;
            y2=yFloor-hPlayer;
        }

        //ball hits right players
        double rel = (x1 + 40) - (x + 22.5);
        double norm = rel / 40;
        double bounce = norm * (5 * PI / 12);
        double BALL_SPEED=sqrt(vyBall*vyBall+vxBall*vxBall);

        if(x-x1<65&&x-x1>-30&&y-y1<=17.5&&y-y1>-30&&fireball2==false)
        {
            vyBall = -BALL_SPEED * cos(bounce)+2*dy1;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
        }
        //player goes 200 pixels backward in fireball power
        else if(x-x1<65&&x-x1>-30&&y-y1<=17.5&&y-y1>-30&&fireball2==true)
        {
            x1+=200;
            if(x1>W-80)
            x1=W-80;
            vyBall = -BALL_SPEED * cos(bounce)+2*dy1;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            freez1=true;
            tfreeze1=clock();
            fireball2=false;
        }

        if(x-x1<65&&x-x1>-30&&y-y1>17.5&&y-y1<65&&fireball2==false)
        {
            vyBall = BALL_SPEED * cos(bounce)+2*dy1;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
        }
        //player goes 200 pixels backward in fireball power
        else if(x-x1<65&&x-x1>-30&&y-y1<65&&y-y1>17.5&&fireball2==true)
        {
            x1+=200;
            if(x1>W-80)
            x1=W-80;
            vyBall = BALL_SPEED * cos(bounce)+2*dy1;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            freez1=true;
            tfreeze1=clock();
            fireball2=false;
        }

        if(x-x1<50&&x-x1>-30&&y-y1>65&&y-y1<90)
        {
            vyBall = BALL_SPEED * cos(bounce)+2*dy1;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
        }

        if(((x1-x<50&&x<x1)||(x-x1<45&&x1<x))&&y-y1>67.5&&y-y1<75&&fireball2==false)
            vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
        //player goes 200 pixels backward in fireball power
        else if(((x1-x<50&&x<x1)||(x-x1<45&&x1<x))&&y-y1>67.5&&y-y1<75&&fireball2==true)
        {
            x1+=200;
            if(x1>W-80)
            x1=W-80;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            freez1=true;
            tfreeze1=clock();
            fireball2=false;
        }

        //ball hits the right clones
        if(tripleclone1){
            rel = (x4 + 40) - (x + 22.5);
            norm = rel / 40;
            bounce = norm * (5 * PI / 12);
            BALL_SPEED=sqrt(vyBall*vyBall+vxBall*vxBall);

            if(x-x4<65&&x-x4>-30&&y-y1<=17.5&&y-y1>-30&&fireball2==false)
            {
                vyBall = -BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            }

            else if(x-x4<65&&x-x4>-30&&y-y1<=17.5&&y-y1>-30&&fireball2==true)
            {
                x1+=200;
                if(x1>W-80)
                    x1=W-80;
                vyBall = -BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
                freez1=true;
                tfreeze1=clock();
                fireball2=false;
            }

           if(x-x4<65&&x-x4>-30&&y-y1>17.5&&y-y1<65&&fireball2==false)
            {
                vyBall = BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            }

            else if(x-x4<65&&x-x4>-30&&y-y1<65&&y-y1>17.5&&fireball2==true)
            {
                x1+=200;
                if(x1>W-80)
                    x1=W-80;
                vyBall = BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
                freez1=true;
                tfreeze1=clock();
                fireball2=false;
            }
            if(x-x4<50&&x-x4>-30&&y-y1>65&&y-y1<90)
            {
                vyBall = BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            }

            if(((x4-x<50&&x<x4)||(x-x4<45&&x4<x))&&y-y1>67.5&&y-y1<75&&fireball2==false){
            vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            }
            else if(((x4-x<50&&x<x4)||(x-x4<45&&x4<x))&&y-y1>67.5&&y-y1<75&&fireball2==true)
            {
                x1+=200;
                if(x1>W-80)
                    x1=W-80;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
                freez1=true;
                tfreeze1=clock();
                fireball2=false;
            }

            rel = (x3 + 40) - (x + 22.5);
            norm = rel / 40;
            bounce = norm * (5 * PI / 12);
            BALL_SPEED=sqrt(vyBall*vyBall+vxBall*vxBall);

            if(x-x3<65&&x-x3>-30&&y-y1<=17.5&&y-y1>-30&&fireball2==false)
            {
                vyBall = -BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            }
            //player goes 200 pixels backward in fireball power
            else if(x-x3<65&&x-x3>-30&&y-y1<=17.5&&y-y1>-30&&fireball2==true)
            {
                x1+=200;
                if(x1>W-80)
                    x1=W-80;
                vyBall = -BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
                freez1=true;
                tfreeze1=clock();
                fireball2=false;
            }

            if(x-x3<65&&x-x3>-30&&y-y1>17.5&&y-y1<65&&fireball2==false)
            {
                vyBall = BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            }
            //player goes 200 pixels backward in fireball power
            else if(x-x3<65&&x-x3>-30&&y-y1<65&&y-y1>17.5&&fireball2==true)
            {
                x1+=200;
                if(x1>W-80)
                    x1=W-80;
                vyBall = BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
                freez1=true;
                tfreeze1=clock();
                fireball2=false;
                }

            if(x-x3<50&&x-x3>-30&&y-y1>65&&y-y1<90)
            {
                vyBall = BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            }

            if(((x3-x<50&&x<x3)||(x-x3<45&&x3<x))&&y-y1>67.5&&y-y1<75)
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            //player goes 200 pixels backward in fireball power
            else if(((x3-x<50&&x<x3)||(x-x3<45&&x3<x))&&y-y1>67.5&&y-y1<75&&fireball2==true)
            {
                x1+=200;
                if(x1>W-80)
                    x1=W-80;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
                freez1=true;
                tfreeze1=clock();
                fireball2=false;
            }

            x3=x1-200;
            x4=x1+190;
        }

        //ball hits left players
        rel = (x2 + 40) - (x + 22.5);
        norm = rel / 40;
        bounce = norm * (5 * PI / 12);
        BALL_SPEED=sqrt(vyBall*vyBall+vxBall*vxBall);

        if(x-x2<65&&x-x2>-30&&y-y2<=17.5&&y-y2>-30&&fireball1==false)
        {
            vyBall = -BALL_SPEED * cos(bounce)+2*dy2;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
        }
        //player goes 200 pixels backward in fireball power
        else if(x-x2<65&&x-x2>-30&&y-y1<=17.5&&y-y1>-30&&fireball1==true)
        {
            x2-=200;
            if(x2<0)
            x2=0;
            vyBall = -BALL_SPEED * cos(bounce)+2*dy2;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
            freez2=true;
            tfreeze2=clock();
            fireball1=false;
        }
        if(x-x2<65&&x-x2>-30&&y-y2>17.5&&y-y2<65&&fireball1==false)
        {
            vyBall = BALL_SPEED * cos(bounce)+2*dy2;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
        }
        //player goes 200 pixels backward in fireball power
        else if(x-x2<65&&x-x2>-30&&y-y2<65&&y-y2>17.5&&fireball1==true)
        {
            x2-=200;
            if(x2<0)
                x2=0;
            vyBall = BALL_SPEED * cos(bounce)+2*dy2;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
            freez2=true;
            tfreeze2=clock();
            fireball1=false;
        }
        if(x-x2<65&&x-x2>-15&&y-y2>65&&y-y2<90)
        {
            vyBall = BALL_SPEED * cos(bounce)+2*dy2;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
        }

        if(((x2-x<12&&x<x2)||(x-x2<85&&x2<x))&&y-y2>67.5&&y-y2<75&&fireball1==false)
            vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
        //player goes 200 pixels backward in fireball power
        else if(((x2-x<12&&x<x2)||(x-x2<85&&x2<x))&&y-y2>67.5&&y-y2<75&&fireball1==true)
        {
            x2-=200;
            if(x2<0)
                x2=0;
            vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
            freez2=true;
            tfreeze2=clock();
            fireball1=false;
        }

        //ball hits the clones
        if(tripleclone2){
            rel = (x6 + 40) - (x + 22.5);
            norm = rel / 40;
            bounce = norm * (5 * PI / 12);
            BALL_SPEED=sqrt(vyBall*vyBall+vxBall*vxBall);

            if(x-x6<65&&x-x6>-30&&y-y2<=17.5&&y-y2>-30&&fireball1==false)
            {
                vyBall = -BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
            }
            //player goes 200 pixels backward in fireball power
            else if(x-x6<65&&x-x6>-30&&y-y1<=17.5&&y-y1>-30&&fireball1==true)
            {
                x2-=200;
                if(x2<0)
                    x2=0;
                vyBall = -BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
                freez2=true;
                tfreeze2=clock();
                fireball1=false;
            }
            if(x-x6<65&&x-x6>-30&&y-y2>17.5&&y-y2<65&&fireball1==false)
            {
                vyBall = BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
            }
            //player goes 200 pixels backward in fireball power
            else if(x-x6<65&&x-x6>-30&&y-y2<65&&y-y2>17.5&&fireball1==true)
            {
                x2-=200;
                if(x2<0)
                    x2=0;
                vyBall = BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
                freez2=true;
                tfreeze2=clock();
                fireball1=false;
            }
            if(x-x6<65&&x-x6>-15&&y-y2>65&&y-y2<90)
            {
                vyBall = BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
            }

            if(((x6-x<12&&x<x6)||(x-x6<85&&x6<x))&&y-y2>67.5&&y-y2<75&&fireball1==false)
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
            //player goes 200 pixels backward in fireball power
            else if(((x2-x<12&&x<x2)||(x-x2<85&&x2<x))&&y-y2>67.5&&y-y2<75&&fireball1==true)
            {
                x2-=200;
                if(x2<0)
                    x2=0;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
                freez2=true;
                tfreeze2=clock();
                fireball1=false;
            }

            if(x-x5<65&&x-x5>-30&&y-y2<=17.5&&y-y2>-30&&fireball1==false)
            {
                vyBall = -BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
            }

            else if(x-x5<65&&x-x5>-30&&y-y1<=17.5&&y-y1>-30&&fireball1==true)
            {
                x2-=200;
                if(x2<0)
                    x2=0;
                vyBall = -BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
                freez2=true;
                tfreeze2=clock();
                fireball1=false;
            }

            if(x-x5<65&&x-x5>-30&&y-y2>17.5&&y-y2<65&&fireball1==false)
            {
                vyBall = BALL_SPEED * cos(bounce)+2*dy1;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx1;
            }

            else if(x-x5<65&&x-x5>-30&&y-y1<=17.5&&y-y1>-30&&fireball1==true)
            {
                x2-=200;
                if(x2<0)
                    x2=0;
                vyBall = -BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
                freez2=true;
                tfreeze2=clock();
                fireball1=false;
            }
            if(x-x5<65&&x-x5>-15&&y-y2>65&&y-y2<90)
            {
                vyBall = BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
            }

            if(((x5-x<2&&x<x5)||(x-x5<95&&x5<x))&&y-y2>67.5&&y-y2<75&&fireball1==false)
            vxBall = BALL_SPEED * -sin(bounce)+2*dx2;

            else if(x-x5<65&&x-x5>-30&&y-y1<=17.5&&y-y1>-30&&fireball1==true)
            {
                x2-=200;
                if(x2<0)
                    x2=0;
                vyBall = -BALL_SPEED * cos(bounce)+2*dy2;
                vxBall = BALL_SPEED * -sin(bounce)+2*dx2;
                freez2=true;
                tfreeze2=clock();
                fireball1=false;
            }

            x5=x2-200;
            x6=x2+190;
        }


        //movement
        y+=vyBall;
        x+=vxBall;
        dx1=0;
        dx2=0;

        endGame(font);
        SDL_RenderPresent(m_renderer);
        SDL_Delay(16);
    }
    tLeft-=(clock()-tStart)/1000;
    if (!power1) tLeftPower1-=clock()-tpower1;
    if (!power2) tLeftPower2-=clock()-tpower2;
    if (freez1) tLeftFreeze1=3000-(clock()-tfreeze1);
    if (freez2) tLeftFreeze2=3000-(clock()-tfreeze2);
    if (invisibleball) tLeftInvisibleball=3000-(clock()-tinvisibleball);
    if (tripleclone1) tLeftTripleclone1=8000-(clock()-ttripleclone1);
    if (tripleclone2) tLeftTripleclone2=8000-(clock()-ttripleclone2);



    //pause
    if (pause){
        texture(m_renderer , W/2-100 , 100 , "resume.png" , 200 , 100);
        texture(m_renderer , W/2-90 , 225 , "save.png" , 180 , 50);
        texture(m_renderer , W/2-95 , 300 , "newgame.png" , 190 , 90);
        texture(m_renderer , W/2-100 , 400 , "mainmenu.png" , 200 , 100);
        texture(m_renderer , W/2-100 , 500 , "exit.png" , 200 , 100);
        SDL_RenderPresent(m_renderer);
    }
    while(pause){
        SDL_PollEvent(e);
        if (e->type==SDL_MOUSEBUTTONDOWN){
            //resume
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=100 && e->button.y<=200){
                pause=false;
                run=true;
            }

            //save
            if (e->button.x>=W/2-90 && e->button.x<=W/2+90 && e->button.y>=225 && e->button.y<=275){
                save();
            }

            //new game
            if (e->button.x>=W/2-95 && e->button.x<=W/2+95 && e->button.y>=300 && e->button.y<=390){
                x1=W-200-wPlayer; y1=yFloor-hPlayer;
                x2=200; y2=yFloor-hPlayer;
                x=W/2-lBall/2; y=yFloor-lBall-400;
                dx1=0; dy1=0;
                dx2=0; dy2=0;
                power1=true; power2=true;
                tpower1=0; tpower2=0;
                tLeftPower1=8000; tLeftPower2=8000;
                invisibleball=false;
                tinvisibleball=0;
                tLeftInvisibleball=3000;
                freez1=false; freez2=false;
                tfreeze1=0,tfreeze2=0;
                tLeftFreeze1=3000; tLeftFreeze2=3000;
                tripleclone1=false; tripleclone2=false;
                ttripleclone1=0; ttripleclone2=0;
                tLeftTripleclone1=8000; tLeftTripleclone2=8000;
                fireball1=false; fireball2=false;
                vxBall=0; vyBall=0;
                goal1=0; goal2=0;
                goal1Text="0"; goal2Text="0";
                goal1str="" ; goal2str="";
                timerText=""; temp="";
                tLeft=90; tNow=0 ; tStart=0;
                text_surface=TTF_RenderText_Solid(font,goal1Text, goalColor);
                goal1Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
                text_surface=TTF_RenderText_Solid(font,goal2Text, goalColor);
                goal2Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
                pause=false;
                run=true;
            }

            //main menu
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=400 && e->button.y<=490){
                pause=false;
                menu=true;
            }

            //exit
            if (e->button.x>=W/2-100 && e->button.x<=W/2+100 && e->button.y>=500 && e->button.y<=600){
                pause=false;
                game=false;
            }
        }
    }
}

    delete e;
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(Message);
    SDL_DestroyTexture(player1Tex);
    SDL_DestroyTexture(player2Tex);
    SDL_DestroyTexture(endGameTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(gateTexture1);
    SDL_DestroyTexture(gateTexture2);
    SDL_DestroyTexture(tickTexture);
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
	IMG_Quit();
	SDL_Quit();
    return 0;
}


void texture (SDL_Renderer *m_renderer,int xp,int yp,string addressOfImage,int width,int height)
{
        int n = addressOfImage.length();
        char char_array[n+1];
        strcpy(char_array, addressOfImage.c_str());
        SDL_Texture *myTexture;
        myTexture = IMG_LoadTexture( m_renderer, char_array);
        int w1, h1;
        //SDL_QueryTexture(myTexture, NULL, NULL, &w1, &h1);
        SDL_Rect texr1;
        texr1.x = xp;
        texr1.y = yp;
        texr1.w = width;
        texr1.h = height;
        SDL_RenderCopy( m_renderer, myTexture, NULL, &texr1);
        SDL_DestroyTexture(myTexture);
}


void endGame (TTF_Font *font)
{
    if (tNow<=0){
        SDL_Rect endGame_rect;
        endGame_rect.x=W/2-350;
        endGame_rect.y=H/2-150;
        endGame_rect.w=700;
        endGame_rect.h=300;
        if (goal1>goal2){
            update_leaderboard(name1);
            string winner="WINNER "+name1;
            text_surface=TTF_RenderText_Solid(font, winner.c_str() , endGameColor);
            endGameTexture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
            SDL_RenderCopy(m_renderer, endGameTexture, NULL, &endGame_rect);
        }
        else if (goal2>goal1){
            update_leaderboard(name2);
            string winner="WINNER "+name2;
            text_surface=TTF_RenderText_Solid(font, winner.c_str() , endGameColor);
            endGameTexture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
            SDL_RenderCopy(m_renderer, endGameTexture, NULL, &endGame_rect);
        }
        else if (goal1==goal2){
            text_surface=TTF_RenderText_Solid(font,"DRAW", endGameColor);
            endGameTexture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
            SDL_RenderCopy(m_renderer, endGameTexture, NULL, &endGame_rect);
        }
        SDL_RenderPresent(m_renderer);
        SDL_Delay(4000);
        run=false;
        menu=true;
    }
    if (goal1>=3){
        SDL_Rect endGame_rect;
        endGame_rect.x=W/2-350;
        endGame_rect.y=H/2-150;
        endGame_rect.w=700;
        endGame_rect.h=300;

        update_leaderboard(name1);
        string winner="WINNER "+name1;
        text_surface=TTF_RenderText_Solid(font, winner.c_str() , endGameColor);
        endGameTexture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
        SDL_RenderCopy(m_renderer, endGameTexture, NULL, &endGame_rect);
        SDL_RenderPresent(m_renderer);
        SDL_Delay(4000);
        run=false;
        menu=true;
    }
    if (goal2>=3){
        SDL_Rect endGame_rect;
        endGame_rect.x=W/2-350;
        endGame_rect.y=H/2-150;
        endGame_rect.w=700;
        endGame_rect.h=300;

        update_leaderboard(name2);
        string winner="WINNER "+name2;
        text_surface=TTF_RenderText_Solid(font, winner.c_str() , endGameColor);
        endGameTexture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
        SDL_RenderCopy(m_renderer, endGameTexture, NULL, &endGame_rect);
        SDL_RenderPresent(m_renderer);
        SDL_Delay(4000);
        run=false;
        menu=true;
    }
}


void background (SDL_Rect player1_rect , SDL_Rect player2_rect , SDL_Rect goal1_rect , SDL_Rect goal2_rect , SDL_Rect timer_rect ,
                                SDL_Rect BG_rect , SDL_Rect gate_rect1 , SDL_Rect gate_rect2 , SDL_Rect tick1_rect , SDL_Rect tick2_rect)
{
        SDL_RenderCopy( m_renderer, backgroundTexture, NULL, &BG_rect);
        SDL_RenderCopy(m_renderer , gateTexture1 , NULL , &gate_rect1);
        SDL_RenderCopy(m_renderer , gateTexture2 , NULL , &gate_rect2);
        //if ball isnt invisible
        if(invisibleball==false)
            texture(m_renderer , x , y , "toop-min.png" , 45 , 45);
        SDL_RenderCopy(m_renderer, player1Tex, NULL, &player1_rect);
        SDL_RenderCopy(m_renderer, player2Tex, NULL, &player2_rect);
        SDL_RenderCopy(m_renderer, goal1Texture, NULL, &goal1_rect);
        SDL_RenderCopy(m_renderer, goal2Texture, NULL, &goal2_rect);
        SDL_RenderCopy(m_renderer, Message, NULL, &timer_rect);
        if(power2==true){
            rect(m_renderer,10,120,200,10,255,0,0,1);
            SDL_RenderCopy(m_renderer , tickTexture , NULL , &tick2_rect);
        }
        if(power1==true){
            rect(m_renderer,W-210,120,200,10,255,0,0,1);
            SDL_RenderCopy(m_renderer , tickTexture , NULL , &tick1_rect);
        }
}


void save ()
{
    ofstream fout ("save\\save.txt");
    //player1 save
    fout<<player1Name<<endl;
    fout<<x1<<' '<<y1<<endl;
    fout<<goal1<<endl;
    fout<<power1<<endl;
    if (!power1) fout<<tLeftPower1<<endl;
    fout<<freez1<<' '<<tripleclone1<<' '<<fireball1<<endl;
    if (freez1) fout<<tLeftFreeze1<<endl;
    if (tripleclone1){
        fout<<x3<<' '<<x4<<endl;
        fout<<tLeftTripleclone1<<endl;
    }
    fout<<endl;

    //player2 save
    fout<<player2Name<<endl;
    fout<<x2<<' '<<y2<<endl;
    fout<<goal2<<endl;
    fout<<power2<<endl;
    if (!power2) fout<<tLeftPower2<<endl;
    fout<<freez2<<' '<<tripleclone2<<' '<<fireball2<<endl;
    if (freez2) fout<<tLeftFreeze2<<endl;
    if (tripleclone2){
        fout<<x5<<' '<<x6<<endl;
        fout<<tLeftTripleclone2<<endl;
    }
    fout<<endl;

    //ball save
    fout<<x<<' '<<y<<endl;
    fout<<vxBall<<' '<<vyBall<<endl;
    fout<<invisibleball<<endl;
    if (invisibleball) fout<<tLeftInvisibleball<<endl;
    fout<<endl;

    //timer save
    fout<<tLeft<<' '<<timerText<<endl;
    fout.close();
}


void load (TTF_Font *font)
{
    ifstream fin ("save\\save.txt");
    if (fin.good()){
        if (isEmpty(fin)) cout<<"save file is empty"<<endl;
        else{
            string loads="";
            //player1 load
            fin>>name1;
            player1Name=name1.c_str();
            text_surface=TTF_RenderText_Solid(font, player1Name , player1Color);
            player1Tex=SDL_CreateTextureFromSurface(m_renderer , text_surface);
            fin>>x1>>y1;
            fin>>goal1;
            loads=to_string(goal1);
            goal1Text=loads.c_str();
            text_surface=TTF_RenderText_Solid(font,goal1Text, goalColor);
            goal1Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
            fin>>power1;
            if (!power1) fin>>tLeftPower1;
            fin>>freez1>>tripleclone1>>fireball1;
            if (freez1) fin>>tLeftFreeze1;
            if (tripleclone1) {
                fin>>x3>>x4;
                fin>>tLeftTripleclone1;
            }

            //player2 load
            fin>>name2;
            player2Name=name2.c_str();
            text_surface=TTF_RenderText_Solid(font, player2Name , player2Color);
            player2Tex=SDL_CreateTextureFromSurface(m_renderer , text_surface);
            fin>>x2>>y2;
            fin>>goal2;
            loads=to_string(goal2);
            goal2Text=loads.c_str();
            text_surface=TTF_RenderText_Solid(font,goal2Text, goalColor);
            goal2Texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
            fin>>power2;
            if (!power2) fin>>tLeftPower2;
            fin>>freez2>>tripleclone2>>fireball2;
            if (freez2) fin>>tLeftFreeze2;
            if (tripleclone2) {
                fin>>x5>>x6;
                fin>>tLeftTripleclone2;
            }

            //ball load
            fin>>x>>y;
            fin>>vxBall>>vyBall;
            fin>>invisibleball;
            if (invisibleball) fin>>tLeftInvisibleball;

            //timer load
            fin>>tLeft>>loads;
            timerText=loads.c_str();
        }
    }
    else cout<<"ERROR\nsave file doesn't exist"<<endl;
    fin.close();
}


void update_leaderboard (string winner)
{
    ofstream fout ("leaderboard\\leaderboard.txt");
    ifstream fin ("leaderboard\\leaderboard.txt");
    if (win1==0) {
        first=winner;
        win1=1;
        fout<<first<<' '<<win1;
    }
    else {
        read_leaderboard();
        //winner already exists
        if (winner==first){
            win1++;
            fout<<first<<' '<<win1<<endl;
            fout<<second<<' '<<win2<<endl;
            fout<<third<<' '<<win3<<endl;
        }
        else if (winner==second){
            win2++;
            if (win2>=win1){
                string tstr=first;
                int tw=win1;
                first=second;
                win1=win2;
                second=tstr;
                win2=tw;
            }
            fout<<first<<' '<<win1<<endl;
            fout<<second<<' '<<win2<<endl;
            fout<<third<<' '<<win3<<endl;
        }
        else if (winner==third){
            win3++;
            if (win3>=win2){
                string tstr=second;
                int tw=win2;
                second=third;
                win2=win3;
                third=tstr;
                win3=tw;
            }
            fout<<first<<' '<<win1<<endl;
            fout<<second<<' '<<win2<<endl;
            fout<<third<<' '<<win3<<endl;
        }
        //new champ
        else {
            if (win1==1){
                if (win2==1){
                    third=second;
                    win3=1;
                }
                second=first;
                win2=1;
                first=winner;
                win1=1;
                fout<<first<<' '<<win1<<endl;
                fout<<second<<' '<<win2<<endl;
                fout<<third<<' '<<win3<<endl;
            }
            else if (win2==1){
                third=second;
                win3=1;
                second=winner;
                win2=1;
                fout<<first<<' '<<win1<<endl;
                fout<<second<<' '<<win2<<endl;
                fout<<third<<' '<<win3<<endl;
            }
            else if (win3==1){
                third=winner;
                win3=1;
                fout<<first<<' '<<win1<<endl;
                fout<<second<<' '<<win2<<endl;
                fout<<third<<' '<<win3<<endl;
            }
        }
    }
    fout.close();
    fin.close();
}


void read_leaderboard ()
{
    ifstream fin ("leaderboard\\leaderboard.txt");
    if (fin.good()){
        if (isEmpty(fin)) cout<<"leaderboard file is empty"<<endl;
        else{
            fin>>first>>win1;
            if (!fin.eof()) fin>>second>>win2;
            if (!fin.eof()) fin>>third>>win3;
        }
    }
    else cout<<"ERROR\nfile doesn't exist"<<endl;
    fin.close();
}


void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian )
{
    SDL_Rect rectangle ;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
    if (fill_boolian==1)
        SDL_RenderFillRect(Renderer, &rectangle);
    SDL_RenderDrawRect(Renderer, &rectangle);
}




