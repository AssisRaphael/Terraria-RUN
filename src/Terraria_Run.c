#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <stdio.h>
#include<time.h>
#include<string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_mixer.h>

#define true 1
#define false 0

#define DimencaoObstaculoX 50
#define DimencaoObstaculoY 50

#define PosicaoObjetox 1000
#define PosicaoObjetoy 200

#define LarguraObjeto 50
#define AlturaObjeto 50

#define AlturaPlayer 100
#define LarguraPlayer 100

#define VelocidadePulo 60

#define FracaoFrames 0.125
#define FracaoFramesTerrain 0.0008
#define FracaoFramesNuven 0.0002
#define FracaoFramesHorizonte 0.0001
#define FracaoFramesTerrain2 0.00015
#define FracaoFramesFloor 0.0009
#define FracaoFramesSky 0.0009


#define Delay 60

#define MaxObst 4
#define MaxAnimation 3

#define IFrames 20

#define MaxEnemies 8
#define MaxBullets 10

#define PUtimer 100

#define SDellay 5
// Structs ********************************************************************

typedef struct{
    float x,y;
	int damage;
	int existe;
}Projectile;

typedef struct{
	float x,y;
	int largura;
	int altura;
	int type;
	int existe;
}PowerUP;

typedef struct{
	float x;
	float y;
	int hp;
	int velocidadey;
	int largura;
	int altura;
	int side;
	int gravidade;
	int Empe;
	int powerup;
	Projectile Bullets[MaxBullets];
}Player;

typedef struct{
	float x;
	float y;
	int largura;
	int altura;
	int existe;
	int texture;
}Obstacle;

typedef struct{
	float x;
	float y;
	int largura;
	int altura;
	float hp;
	int damage;
	int texture;
	int existe;
}Enemy;

// Fim dos Structs *************************************************************

void Animation();
void LoadTexture(char* File,GLuint* T);
void ApplyTexture(GLuint *Texture, GLuint x, GLuint y, GLuint largura, GLuint altura, GLfloat Alpha);
void inicializaObstaculo();
void InitializeEnemies();
void InitializeProjectiles();
void resetaObjeto(int i);
void resetaObjetoM(int i);
void resetaObjetoSky(int i);
void resetaObjetoSkyM(int i);
void ResetEnemy(int i);
void ResetBullet(int i);
void GroundObstacleGenerator();
void GroundMObstacleGenerator();
void SkyObstacleGenerator();
void SkyMObstacleGenerator();
void EnemyGenerator();
void DesenhaObstaculos(int i);
void GeraPlayer();
void TerrainGenerator();
void SkyGenerator();
void FloorGenerator();
float HeartAlpha(int i);
void DisplayHealth();
void DrawObstacles();
void DrawProjectiles();
void MoveGround();
void MoveGroundM();
void MoveSky();
void MoveSkyM();
void AtualizaBackGround();
void Gravity();
void MoverPlayer();
void PlayerEmPe();
void PlayerAbaixado();
int colidir(GLuint x1, GLuint y1, GLuint largura1, GLuint altura1, GLuint x2, GLuint y2, GLuint largura2, GLuint altura2);
void Shooting();
void InverteGravity();
void ObstacleManager();
void TerrainManager();
void ScoreManager();
void HealthManager();
void EnemyManager();
void ProjectileManager();
void HealthManager();
void ColisionManager();
void AnimationManager();
void GameManager(int i);


//Veriaveis globais *************************************************************

int volume_musica=100;
int fade_out = 100;
Mix_Music *musicMenu=NULL;

GLfloat x = 0, y = 0;
int gravity=10;

GLuint ObstacleTexture[3],EnemiesTexture[3];

int groundindex;
int skyindex;

int Abaixado = 0;

GLuint TerrainTexture, PlayerTexture, TerrenoTexture,SkyTexture, PedraTexture, GameOverTexture, PauseTexture, NuvensTexture, HorizonteTexture;
GLuint TerrainTexture2, CeuTexture, Heart, RegenTexture, RegenIconTexture, IronSkinTexture, IronSkinIconTexture;

float CurrentAnimation;

Player p;

GLuint fps, cont, ShootingDellay;

Obstacle Ground[4],GroundM[4],Sky[4],SkyM[4], backGround;
Enemy Enemies[MaxEnemies];

int AnimaFrames;

float FrameTerrain, FrameNuvens, FrameHorizonte, FrameTerrain2, FrameFloor, FrameSky;

int ObstacleDelay, ObstacleMDelay,ObstacleSkyDelay,ObstacleSkyMDelay,EnemyDelay;
int pause=1;

int Score=0;
int Invincibility=0;

PowerUP PU;

int RegenTimer,IronSkinTimer;

// Fim da globais *************************************************************


void Animation(){
	AnimaFrames++;
	AnimaFrames=AnimaFrames%8;

}


void LoadTexture(char* File,GLuint* T){ // CarreEnemies[i]ga as texturas
	*T = SOIL_load_OGL_texture(File,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
	if(*T == 0)
		printf("Erro do SOIL: '%s'\n",SOIL_last_result());

}

void iniciar_musica(char *music){
	if(!Mix_PlayingMusic()){
		Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,1024);
		musicMenu=Mix_LoadMUS(music);
		Mix_VolumeMusic(volume_musica);
		Mix_PlayMusic(musicMenu,-1);
	}
}
void parar_musica(){
	if(Mix_PlayingMusic()){
		Mix_FadeOutMusic(fade_out);
	}
}

// *************************************************************

void ApplyTexture(GLuint *Texture, GLuint x, GLuint y, GLuint largura, GLuint altura, GLfloat Alpha){
// Recebe como parametro o numero gerado pela LoadTexture que se carrena na inicializa e aplica textura no objeto

    //GLuint x, GLuint y => coordenadas do primeiro ponto
    //GLuint largura, GLuint altura => Largura e altura do retangulo
	glColor4f(1, 1, 1, Alpha);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,*Texture);
	glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);glVertex3f(x, y, 0);
        glTexCoord2f(1, 0);glVertex3f(x+largura, y, 0);
        glTexCoord2f(1, 1); glVertex3f(x+largura, y+altura, 0);
        glTexCoord2f(0, 1);glVertex3f(x,y+altura, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
// *************************************************************

//Initializers **********************************************
void inicializaObstaculo(){
	int i;

	    backGround.x=0;
	    backGround.y=200;
	    backGround.largura=1000;
	    backGround.altura=600;

	for(i=0;i<4;i++){
		Ground[i].x=PosicaoObjetox;
		Ground[i].y=PosicaoObjetoy;
		Ground[i].largura=DimencaoObstaculoX;
		Ground[i].altura=DimencaoObstaculoY;
		Ground[i].texture=0;
		Ground[i].existe=false;
		GroundM[i].x=PosicaoObjetox;
		GroundM[i].y=PosicaoObjetoy;
		GroundM[i].largura=DimencaoObstaculoX;
		GroundM[i].altura=2.5*DimencaoObstaculoY;
		GroundM[i].texture=1;
		GroundM[i].existe=false;
		Sky[i].x=PosicaoObjetox;
		Sky[i].y=PosicaoObjetoy+600-DimencaoObstaculoY;
		Sky[i].largura=DimencaoObstaculoX;
		Sky[i].altura=DimencaoObstaculoY;
		Sky[i].texture=2;
		Sky[i].existe=false;
		SkyM[i].x=PosicaoObjetox;
		SkyM[i].y=PosicaoObjetoy+600-DimencaoObstaculoY-50;
		SkyM[i].largura=DimencaoObstaculoX;
		SkyM[i].altura=DimencaoObstaculoY;
		SkyM[i].texture=2;
		SkyM[i].existe=false;
    }

}
void InitializeEnemies(){
	int i;
	for(i=0;i<MaxEnemies;i++){
    Enemies[i].hp=-1;
		Enemies[i].x=1100;
		Enemies[i].existe=0;
	}

}
void InitializeProjectiles(){
	int i;
	for(i=0;i<MaxBullets;i++){
		p.Bullets[i].x=0;
		p.Bullets[i].y=0;
		p.Bullets[i].existe=0;
	}
}
void InitializePowerUP(){
	PU.x=0;
	PU.y=0;
	PU.largura=50;
	PU.altura=50;
	PU.existe=0;
}
//***********************************************************

//Resets *****************************************

void resetaObjeto(int i){ // reseta o objeto para o canto lateral direito da tela
    Ground[i].x=PosicaoObjetox;
    Ground[i].existe=false;
}
void resetaObjetoM(int i){ // reseta o objeEnemiesTexture[3]to para o canto lateral direito da tela
    GroundM[i].x=PosicaoObjetox;
    GroundM[i].existe=false;
}
void resetaObjetoSky(int i){ // reseta o objeto para o canto lateral direito da tela
    Sky[i].x=PosicaoObjetox;
    Sky[i].existe=false;
}
void resetaObjetoSkyM(int i){ // reseta o objeto para o canto lateral direito da tela
    SkyM[i].x=PosicaoObjetox;
    SkyM[i].existe=false;
}
void ResetEnemy(int i){
  Enemies[i].hp=-1;
	Enemies[i].x=1100;
	Enemies[i].existe=0;
}
void ResetBullet(int i){
	p.Bullets[i].x=0;
	p.Bullets[i].y=0;
	p.Bullets[i].existe=0;
}

//***********************************************

//Generators*************************************

void GroundObstacleGenerator(){
	int i;
	if(ObstacleDelay==0){
		for(i=0;i<4;i++){
			if(Ground[i].existe==0){
				Ground[i].existe=1;
				ObstacleDelay=rand()%Delay+Delay;
				break;
			}
		}
	}
	ObstacleDelay--;
}
void GroundMObstacleGenerator(){
	int i;
	if(ObstacleMDelay==0){

		for(i=0;i<4;i++){
			if(GroundM[i].existe==0){
				GroundM[i].existe=1;
				ObstacleMDelay=rand()%Delay+Delay;
				break;
			}
		}
	}
	ObstacleMDelay--;
}
void SkyObstacleGenerator(){
	int i;
	if(ObstacleSkyDelay==0){
		for(i=0;i<4;i++){
			if(Sky[i].existe==0){
				Sky[i].existe=1;
				ObstacleSkyDelay=rand()%Delay+Delay;
				break;
			}
		}
	}
	ObstacleSkyDelay--;
}
void SkyMObstacleGenerator(){
	int i;
	if(ObstacleSkyMDelay==0){
		for(i=0;i<4;i++){
			if(SkyM[i].existe==0){
				SkyM[i].existe=1;
				ObstacleSkyMDelay=rand()%Delay+Delay;
				break;
			}
		}
	}
	ObstacleSkyMDelay--;
}
void EnemyGenerator(){
	int i,position;
	if(EnemyDelay<=0){
		position=rand()%4;
		if(position==0)
			for(i=0;i<MaxEnemies;i++)
				if(Enemies[i].existe==0){
					Enemies[i].x=1000;
					Enemies[i].y=200;
					Enemies[i].hp=3;
					Enemies[i].largura=50;
					Enemies[i].altura=30;
					Enemies[i].damage=10;
					Enemies[i].texture=0;
					Enemies[i].existe=1;
					EnemyDelay=rand()%Delay+Delay;
					break;
				}
		if(position==1)
			for(i=0;i<MaxEnemies;i++)
				if(Enemies[i].existe==0){
					Enemies[i].x=1000;
					Enemies[i].y=250;
					Enemies[i].hp=5;
					Enemies[i].largura=50;
					Enemies[i].altura=50;
					Enemies[i].damage=20;
					Enemies[i].texture=1;
					Enemies[i].existe=1;
					EnemyDelay=rand()%Delay+Delay;
					break;
				}
		if(position==2)
			for(i=0;i<MaxEnemies;i++)
				if(Enemies[i].existe==0){
					Enemies[i].x=1000;
					Enemies[i].y=700;
					Enemies[i].hp=5;
					Enemies[i].largura=50;
					Enemies[i].altura=50;
					Enemies[i].damage=20;
					Enemies[i].texture=1;
					Enemies[i].existe=1;
					EnemyDelay=rand()%Delay+Delay;
					break;
				}
		if(position==3)
			for(i=0;i<MaxEnemies;i++)
				if(Enemies[i].existe==0){
					Enemies[i].x=1000;
					Enemies[i].y=708;
					Enemies[i].hp=3;
					Enemies[i].largura=92;
					Enemies[i].altura=92;
					Enemies[i].damage=30;
					Enemies[i].texture=2;
					Enemies[i].existe=1;
					EnemyDelay=rand()%Delay+Delay;
					break;
				}
	}
}

//*******************************************

//Drawers *******************************************

void DesenhaObstaculos(int i){
	if(Ground[i].existe)
    	ApplyTexture(ObstacleTexture+Ground[i].texture,Ground[i].x,Ground[i].y,LarguraObjeto,AlturaObjeto,1);
    if(GroundM[i].existe)
    	ApplyTexture(ObstacleTexture+GroundM[i].texture,GroundM[i].x,GroundM[i].y,LarguraObjeto,2.5*AlturaObjeto,1);
    if(Sky[i].existe)
    	ApplyTexture(ObstacleTexture+Sky[i].texture,Sky[i].x,Sky[i].y,LarguraObjeto,AlturaObjeto,1);
    if(SkyM[i].existe)
    	ApplyTexture(ObstacleTexture+SkyM[i].texture,SkyM[i].x,SkyM[i].y,LarguraObjeto,AlturaObjeto,1);

}

void GeraPlayer(){

   glColor3f(1, 1, 1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, PlayerTexture);
	glBegin(GL_POLYGON); // desenha um retangulo de 50x50 no fim da tela
         if(p.gravidade==1){
		     glTexCoord2f(AnimaFrames*FracaoFrames,((float)CurrentAnimation/(float)MaxAnimation)); glVertex3f(p.x , p.y, 0);
		     glTexCoord2f(AnimaFrames*FracaoFrames+FracaoFrames,((float)CurrentAnimation/(float)MaxAnimation)); glVertex3f(p.x + p.largura, p.y , 0);
		     glTexCoord2f(AnimaFrames*FracaoFrames+FracaoFrames,((float)CurrentAnimation/(float)MaxAnimation)+((float)1/(float)MaxAnimation)); glVertex3f(p.x + p.largura , p.y +p.altura, 0);
		     glTexCoord2f(AnimaFrames*FracaoFrames,((float)CurrentAnimation/(float)MaxAnimation)+((float)1/(float)MaxAnimation)); glVertex3f(p.x, p.y + p.altura, 0);
		 }
		 else{
		 	 glTexCoord2f(AnimaFrames*FracaoFrames, ((float)CurrentAnimation/(float)MaxAnimation)+((float)1/(float)MaxAnimation)); glVertex3f(p.x , p.y, 0);
		     glTexCoord2f(AnimaFrames*FracaoFrames+FracaoFrames,((float)CurrentAnimation/(float)MaxAnimation)+((float)1/(float)MaxAnimation)); glVertex3f(p.x + p.largura, p.y , 0);
		     glTexCoord2f(AnimaFrames*FracaoFrames+FracaoFrames,((float)CurrentAnimation/(float)MaxAnimation)); glVertex3f(p.x + p.largura , p.y +p.altura, 0);
		     glTexCoord2f(AnimaFrames*FracaoFrames,((float)CurrentAnimation/(float)MaxAnimation)); glVertex3f(p.x, p.y + p.altura, 0);
		 }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void TerrainGenerator(){

	glColor3f(1, 1, 1);

   //Ceu
   glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, CeuTexture);
    	glBegin(GL_POLYGON); // desenha um retangulo de 50x50 no fim da tela
			glTexCoord2f(0,0); glVertex3f(backGround.x , backGround.y, 0);
			glTexCoord2f(1,0); glVertex3f(backGround.x + backGround.largura, backGround.y, 0);
			glTexCoord2f(1,1); glVertex3f(backGround.x + backGround.largura , backGround.y +backGround.altura, 0);
			glTexCoord2f(0,1); glVertex3f(backGround.x, backGround.y + backGround.altura, 0);
		glEnd();
    glDisable(GL_TEXTURE_2D);

    //Terras voadoras
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, TerrainTexture2);
		glBegin(GL_POLYGON); // desenha um retangulo de 50x50 no fim da tela
			glTexCoord2f(0+FrameTerrain2,0); glVertex3f(backGround.x , backGround.y, 0);
			glTexCoord2f(0.5+FrameTerrain2,0); glVertex3f(backGround.x + backGround.largura, backGround.y , 0);
			glTexCoord2f(0.5+FrameTerrain2,1); glVertex3f(backGround.x + backGround.largura , backGround.y +backGround.altura, 0);
			glTexCoord2f(0+FrameTerrain2,1); glVertex3f(backGround.x, backGround.y + backGround.altura, 0);
		glEnd();
    glDisable(GL_TEXTURE_2D);

    //Horizonte
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, HorizonteTexture);
    	glBegin(GL_POLYGON); // desenha um retangulo de 50x50 no fim da tela
			glTexCoord2f(0+FrameHorizonte,0); glVertex3f(backGround.x , backGround.y, 0);
			glTexCoord2f(0.5+FrameHorizonte,0); glVertex3f(backGround.x + backGround.largura, backGround.y, 0);
			glTexCoord2f(0.5+FrameHorizonte,1); glVertex3f(backGround.x + backGround.largura , backGround.y +backGround.altura, 0);
			glTexCoord2f(0+FrameHorizonte,1); glVertex3f(backGround.x, backGround.y + backGround.altura, 0);
		glEnd();
    glDisable(GL_TEXTURE_2D);

    //Nuvens
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, NuvensTexture);
    	glBegin(GL_POLYGON); // desenha um retangulo de 50x50 no fim da tela
			glTexCoord2f(0+FrameNuvens,0); glVertex3f(backGround.x , backGround.y, 0);
			glTexCoord2f(0.5+FrameNuvens,0); glVertex3f(backGround.x + backGround.largura, backGround.y, 0);
			glTexCoord2f(0.5+FrameNuvens,1); glVertex3f(backGround.x + backGround.largura , backGround.y +backGround.altura, 0);
			glTexCoord2f(0+FrameNuvens,1); glVertex3f(backGround.x, backGround.y + backGround.altura, 0);
		glEnd();
    glDisable(GL_TEXTURE_2D);

   //Arvores
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, TerrainTexture);
		glBegin(GL_POLYGON); // desenha um retangulo de 50x50 no fim da tela
			glTexCoord2f(0+FrameTerrain,0); glVertex3f(backGround.x , backGround.y, 0);
			glTexCoord2f(0.5+FrameTerrain,0); glVertex3f(backGround.x + backGround.largura, backGround.y , 0);
			glTexCoord2f(0.5+FrameTerrain,1); glVertex3f(backGround.x + backGround.largura , backGround.y +backGround.altura, 0);
			glTexCoord2f(0+FrameTerrain,1); glVertex3f(backGround.x, backGround.y + backGround.altura, 0);
		glEnd();
    glDisable(GL_TEXTURE_2D);
}


//cereja
void SkyGenerator(){

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, SkyTexture);
		glBegin(GL_POLYGON); // desenha um retangulo de 50x50 no fim da tela
			glTexCoord2f(0+FrameSky,0); glVertex3f(0, 800, 0);
			glTexCoord2f(0.25+FrameSky,0); glVertex3f(1000, 800, 0);
			glTexCoord2f(0.25+FrameSky,1); glVertex3f(1000, 1000, 0);
			glTexCoord2f(0+FrameSky,1); glVertex3f(0, 1000, 0);
		glEnd();
    glDisable(GL_TEXTURE_2D);

	//ApplyTexture(&SkyTexture,0,800,1000,200,1);
}

void FloorGenerator(){
	// Associamos um canto da textura pgcc  -lSOIL -lglut -lGL -lGLEW -lGLUara cada v�rtice

	//Terras voadoras
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, TerrenoTexture);
		glBegin(GL_POLYGON); // desenha um retangulo de 50x50 no fim da tela
			glTexCoord2f(0+FrameFloor,0); glVertex3f(0, 0, 0);
			glTexCoord2f(0.25+FrameFloor,0); glVertex3f(1000, 0, 0);
			glTexCoord2f(0.25+FrameFloor,1); glVertex3f(1000, 200, 0);
			glTexCoord2f(0+FrameFloor,1); glVertex3f(0, 200, 0);
		glEnd();
    glDisable(GL_TEXTURE_2D);

	//ApplyTexture(&TerrenoTexture,0,0,1000,200,1);

}

void DisplayHealth(){
	int i;
	for(i=0;i<5;i++)
		ApplyTexture(&Heart,790+((4-i)*40),940,40,40,HeartAlpha(i));

}

void DrawObstacles(){
	int i;

	for(i=0;i<MaxObst;i++){
		DesenhaObstaculos(i);
	}
}
void DrawProjectiles(){
	int i;
	glColor3f(0,0,0);
	glPointSize(7);
	for(i=0;i<MaxBullets;i++){
		glBegin(GL_POINTS);
			glVertex3f(p.Bullets[i].x,p.Bullets[i].y,0);
		glEnd();
	}
}
void DisplayPowerUP(){
	if(RegenTimer>0){
		ApplyTexture(&RegenIconTexture,30,850,50,50,1);
		
	}
	if(IronSkinTimer>0){
		ApplyTexture(&IronSkinIconTexture,110,850,50,50,1);
	}
}
void DrawPowerUP(){
	if(PU.existe){
		if(PU.type==1)
			ApplyTexture(&Heart,PU.x,PU.y,PU.largura,PU.altura,1);
		if(PU.type==2)
			ApplyTexture(&RegenTexture,PU.x,PU.y,PU.largura,PU.altura,1);
		if(PU.type==3)
			ApplyTexture(&IronSkinTexture,PU.x,PU.y,PU.largura,PU.altura,1);
	}
}

void texto(void* font, char* s, float x, float y) {
  unsigned int i;
  glRasterPos2f(x, y);
  for (i = 0; i<strlen (s); i++) {// menor que
     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
  }
}

void display() {
    char s[10]="000000000";
  //glClear(GL_COLOR_BUFFER_BIT);
  glColor3f (0, 0, 0);
  texto(GLUT_BITMAP_HELVETICA_18, "Pontuacao:", 380, 900);
  sprintf(s, "%d", Score);
  texto(GLUT_BITMAP_HELVETICA_18, s, 490, 900);
  glFlush();
}

void DrawEnemies(){
  int i;
  for(i=0;i<MaxEnemies;i++)
    if(Enemies[i].existe)
      ApplyTexture(EnemiesTexture+Enemies[i].texture,Enemies[i].x,Enemies[i].y,Enemies[i].largura,Enemies[i].altura,1);
}

void desenhaCena()
{
	if(pause==-1){
		ApplyTexture(&PauseTexture,400,400,200,200,1);
		glFlush();//Coloca desenho na tela
    	return;
    }
    if(p.hp<=0){
    	ApplyTexture(&GameOverTexture,400,400,200,200,1);
    	parar_musica();
		glFlush();//Coloca desenho na tela
    	return;
    }

    // Habilita o uso de texturasApplyTexture(&PauseTexture,400,400,200,200);

	TerrainGenerator();
    FloorGenerator();
	SkyGenerator();
	
	DrawObstacles();
	DrawProjectiles();
  	DrawEnemies();
	DrawPowerUP();
	DisplayHealth();
	DisplayPowerUP();
	display();
	if((Invincibility+1)%2)
		GeraPlayer();

    glFlush();//Coloca desenho na tela
	//PlayerEmPe();
}



// ***************************************************************************

//Movers *********************************************************************

void MoveGround(){

	int i;
	for(i=0;i<4;i++){
		if(Ground[i].existe)
			Ground[i].x= Ground[i].x -20;
			if(Ground[i].x<=-50)

			resetaObjeto(i);
	}
}

void MoveGroundM(){

	int i;
	for(i=0;i<4;i++){
		if(GroundM[i].existe)
			GroundM[i].x= GroundM[i].x -20;
		if(GroundM[i].x<=-50)

			resetaObjetoM(i);
	}
}
void MoveSky(){
	int i;
	for(i=0;i<4;i++){

		if(Sky[i].existe)
			Sky[i].x= Sky[i].x -20;


		if(Sky[i].x<=-50)
			resetaObjetoSky(i);
	}
}
void MoveSkyM(){

	int i;
	for(i=0;i<4;i++){
		if(SkyM[i].existe)
			SkyM[i].x= SkyM[i].x -20;

		if(SkyM[i].x<=-50)

			resetaObjetoSkyM(i);
	}
}

void AtualizaBackGround(){

    FrameTerrain+=FracaoFramesTerrain;
    FrameTerrain2+=FracaoFramesTerrain2;
    FrameNuvens+=FracaoFramesNuven;
    FrameHorizonte+=FracaoFramesHorizonte;
    FrameFloor+=FracaoFramesFloor;
    FrameSky+=FracaoFramesSky;
    if(FrameTerrain>=0.75)
        FrameTerrain=0;
    if(FrameTerrain2>=0.75)
        FrameTerrain2=0;
    if(FrameNuvens>=0.75)
        FrameNuvens=0;
    if(FrameHorizonte>=0.75)
        FrameHorizonte=0;
    if(FrameFloor>=0.75)
        FrameFloor=0;
    if(FrameSky>=0.75)
        FrameSky=0;

}

void MoverPlayer(){
	p.y+=p.velocidadey;
    if(p.y<200){
    	p.velocidadey=0;

    	p.y=200;
    }
    if(p.y>800-p.altura){
    	p.velocidadey=0;
    	p.y=800-p.altura;

    }

    Gravity();



}

// ***************************************************************************


//MiscelLaneous ***************************************************************


void Gravity(){
	if(p.gravidade==1){
		if(p.y>200)
			p.velocidadey-=gravity;
	}
	else{
        if(p.y<800-p.altura)
			p.velocidadey+=gravity;

	}
}



void PlayerEmPe(){
    p.altura=AlturaPlayer; p.largura=LarguraPlayer;
    p.Empe=1;
}






void PlayerAbaixado(){
    p.altura=AlturaPlayer; p.largura=LarguraPlayer;
    p.Empe=0;
}





int colidir(GLuint x1, GLuint y1, GLuint largura1, GLuint altura1, GLuint x2, GLuint y2, GLuint largura2, GLuint altura2){

		if(x1>=x2 && x1<=x2+largura2 && y1>=y2 && y1<=y2+altura2){ // colidiu objetos
			//Invincibility=IFrames;
			return true;

		}

		if(x1+largura1>=x2 && x1+largura1<=x2+largura2 && y1>=y2 && y1<=y2+altura2){ // colidiu objetos
			return true;

		}
		if(x1>=x2 && x1<=x2+largura2 && y1+altura1>=y2 && y1+altura1<=y2+altura2) { // colidiu objetos
			//Invincibility=IFrames;
			return true;

		}
		if(x1+largura1>=x2 && x1+largura1<=x2+largura2 && y1+altura1>=y2 && y1+altura1<=y2+altura2) { // colidiu objetos
			//Invincibility=IFrames;
			return true;

		}
		return false;

}

float HeartAlpha(int i){
		float Alpha=((float)(p.hp-80)/20)+i;
		if(Alpha>1)
			Alpha=1;
		if(Alpha<0)
			Alpha=0;
		return Alpha;
}



void Shooting(){
	int i;
	for(i=0;i<MaxBullets;i++){
		if(0==ShootingDellay){
			if(p.Bullets[i].existe==0){
				p.Bullets[i].x=p.x+100;
				if (p.Empe==1)
					p.Bullets[i].y=p.y+50;
				else
					p.Bullets[i].y=p.y+50-(20*p.gravidade);
				p.Bullets[i].existe=1;
				ShootingDellay=SDellay;
				break;
			}
		}
	}
}




void InverteGravity(){
	p.gravidade*=-1;
}

int PUchecker(int i){
	if(i==2)
		if(RegenTimer>0)
			return 1;
	if(i==3)
		if(IronSkinTimer>0)
			return 1;
	return 0;
}

// ***************************************************************************

//Managers *******************************************************************

void ObstacleManager(){
	GroundObstacleGenerator();
	GroundMObstacleGenerator();
	SkyObstacleGenerator();
	SkyMObstacleGenerator();
}

void TerrainManager(){
	MoveGround();
	MoveGroundM();
	MoveSky();
	MoveSkyM();
}

void ScoreManager(){
	Score++;
}

void EnemyManager(){
	int i;
	EnemyGenerator();
	for(i=0;i<MaxEnemies;i++){
		if(Enemies[i].existe){
			Enemies[i].x-=10+(Enemies[i].texture*10);
	  		if(Enemies[i].x<=-100)
	  			ResetEnemy(i);
	  		if(Enemies[i].hp==0){
				if(PU.existe==0){
						if(0==(rand()%2)){
							PU.x=Enemies[i].x;
							PU.y=Enemies[i].y;
							PU.type=(rand()%3+1);
							PU.existe=1;
						}
				}
	  			ResetEnemy(i);
	  		}
		}
	}
	EnemyDelay--;
}
void ProjectileManager(){
	int i;
	for(i=0;i<MaxBullets;i++){
		if(p.Bullets[i].existe)
			p.Bullets[i].x+=50;
		if(p.Bullets[i].x>1000)
			ResetBullet(i);
	}
  if (ShootingDellay>0)
	 ShootingDellay--;

}
void HealthManager(){
	if(PUchecker(2))
		if(AnimaFrames%2)
			p.hp++;
	DisplayHealth();
	if(Invincibility)
		Invincibility--;
	if(p.hp>=100)
		p.hp=100;
}


void ColisionManager(){
	int i,j;
	if(Invincibility<=0){
		for(i=0;i<MaxObst;i++){
			if(colidir(p.x, p.y, p.largura, p.altura, Ground[i].x, Ground[i].y, Ground[i].largura, Ground[i].altura)){
				p.hp-=(20-5*PUchecker(3));
				Invincibility=IFrames;
				break;
				/*printf("Game over\n,%f\n",p.y);
				exit(0);*/
			}

			if(p.Empe){
				if(colidir(p.x, p.y, p.largura, p.altura, GroundM[i].x, GroundM[i].y, GroundM[i].largura, GroundM[i].altura)){
				p.hp-=(20-5*PUchecker(3));
				Invincibility=IFrames;
				break;
				/*printf("Game over\n,%f\n",p.y);
				exit(0);*/
				}
			}
			if(colidir(p.x, p.y, p.largura, p.altura, Sky[i].x, Sky[i].y, Sky[i].largura, Sky[i].altura)){
				p.hp-=(20-5*PUchecker(3));
				Invincibility=IFrames;
				break;
				/*printf("Game over\n,%f\n",p.y);
				exit(0);*/
			}

			if(p.Empe){
				if(colidir(p.x, p.y, p.largura, p.altura, SkyM[i].x, SkyM[i].y, SkyM[i].largura, SkyM[i].altura)){
					p.hp-=(20-5*PUchecker(3));
					Invincibility=IFrames;
					break;
					/*printf("Game over\n,%f\n",p.y);
					exit(0);*/
				}
			}
	   }
	   for(i=0;i<MaxEnemies;i++){
			if(colidir(p.x, p.y, p.largura, p.altura, Enemies[i].x, Enemies[i].y, Enemies[i].largura, Enemies[i].altura)){
				if(Enemies[i].texture!=1){				
					p.hp-=(Enemies[i].damage-5*PUchecker(3));
					Invincibility=IFrames;
					break;
				}
				else if(p.Empe){
					p.hp-=(Enemies[i].damage-5*PUchecker(3));
					Invincibility=IFrames;
					break;
				}
			}
	   }
	}
	for(j=0;j<MaxBullets;j++){
		for(i=0;i<MaxEnemies;i++){
				if(colidir(p.Bullets[j].x, p.Bullets[j].y,0,0, Enemies[i].x, Enemies[i].y, Enemies[i].largura, Enemies[i].altura)){
					Enemies[i].hp-=1;
					ResetBullet(j);
					break;
				}
		}
	}
	if (colidir(p.x, p.y, p.largura, p.altura, PU.x, PU.y, PU.largura, PU.altura)){
		p.powerup=PU.type;
	}
}
void AnimationManager(){
	if(p.gravidade==1){
		if(p.y<=200){
			if(p.Empe==0)
				CurrentAnimation=0;
			else
				CurrentAnimation=2;
		}
		else
			CurrentAnimation=1;
	}
	else{
		if(p.y>=800-p.altura){
			if(p.Empe==0)
				CurrentAnimation=0;
			else
				CurrentAnimation=2;
		}
		else
			CurrentAnimation=1;
	}
}
void PowerUPManager(){
	if(PU.existe){
		PU.x-=20;
		if(PU.x<=-100)
			InitializePowerUP();
	}
	if(p.powerup==1){
		p.hp+=20;
		p.powerup=0;
	}
	if(p.powerup==2){
		RegenTimer=PUtimer;
		p.powerup=0;
	}
	if(p.powerup==3){
		IronSkinTimer=PUtimer;
		p.powerup=0;
	}
  if(RegenTimer>0)
		RegenTimer--;
	if(IronSkinTimer>0)
		IronSkinTimer--;

}

// ***************************************************************************

void GameManager(int i) {
	/*atualizaAnimacaoPlayer();
	atualizaPosicaoPlayer();
	atualizaCenario();*/




	if(pause==1){
		AnimationManager();
		Animation();
		MoverPlayer();
		ScoreManager();
		TerrainManager();
		ObstacleManager();
		ColisionManager();
		HealthManager();
		AtualizaBackGround();
		ProjectileManager();
		EnemyManager();
		PowerUPManager();

		glutPostRedisplay();
	}
	cont++;

	if(cont==100 && fps>=40){
		fps--;
		cont=0;
	}
	else if (cont==200){
		fps--;
		cont=0;
	}
	glutTimerFunc(fps, GameManager, 0);
}

// ************************************************************************************


// Inicia algumas vari�veis de estado
void inicializa(void){
	int i;
    //glVertex3f(p.x + p.largura , p.y +p.altura, 0);
	srand(time(NULL));

    //Inicializa o Player
    AnimaFrames=0;
    FrameTerrain=0;
    FrameTerrain2=0;
    FrameNuvens=0;
    FrameHorizonte=0;
    FrameFloor=0;
    FrameSky=0;
    CurrentAnimation=2;
    p.x=30;
    p.y=200;
    p.altura=AlturaPlayer;
    p.largura=LarguraPlayer;
    p.side=1;
    p.hp=100;
    p.gravidade=1;
    p.Empe = 1;
    p.powerup=0;

	fps=50;
	cont=0;
	Score=0;
	EnemyDelay=0;

    RegenTimer=0;
    IronSkinTimer=0;

    //Criar o primeiro obstaculo
    ObstacleDelay=0;
    ObstacleMDelay=rand()%Delay+Delay;
	ObstacleSkyDelay=rand()%(Delay/2)+(Delay/2);
	ObstacleSkyMDelay=rand()%Delay+Delay;
	EnemyDelay=rand()%ObstacleSkyMDelay;
	inicializaObstaculo();
	InitializeProjectiles();
	InitializeEnemies();
	InitializePowerUP();


    glClearColor(1, 1, 1, 0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	LoadTexture("..//cenario//Terrain.png",&TerrainTexture);
	LoadTexture("..//cenario//Terrain2.png",&TerrainTexture2);
	LoadTexture("..//cenario//Guide_Frames.png",&PlayerTexture);
	LoadTexture("..//cenario//Ground.png",&TerrenoTexture);//a floor texture;ApplyTexture(&PauseTexture,400,400,200,200);
	LoadTexture("..//cenario//Rock_2.png",ObstacleTexture);
	LoadTexture("..//cenario//RockM.png",ObstacleTexture+1);
	LoadTexture("..//cenario//Star.png",ObstacleTexture+2);
	LoadTexture("..//cenario//Slime.png",EnemiesTexture);
	LoadTexture("..//cenario//Demon_Eye.png",EnemiesTexture+1);
	LoadTexture("..//cenario//Harpy.png",EnemiesTexture+2);
	LoadTexture("..//cenario//gameover.png",&GameOverTexture);//a floor texture;
	LoadTexture("..//cenario//pause.png",&PauseTexture);//a floor texture;
	LoadTexture("..//cenario//Nuvens.png",&NuvensTexture);
	LoadTexture("..//cenario//Horizonte.png",&HorizonteTexture);
	LoadTexture("..//cenario//ceu.png",&CeuTexture);
	LoadTexture("..//cenario//Sky.png",&SkyTexture);
	LoadTexture("..//cenario//Heart.png",&Heart);
	LoadTexture("..//cenario//Regen.png",&RegenTexture);
	LoadTexture("..//cenario//IronSkin.png",&IronSkinTexture);
	LoadTexture("..//cenario//RegenIcon.png",&RegenIconTexture);
	LoadTexture("..//cenario//IronSkinIcon.png",&IronSkinIconTexture);
	iniciar_musica("..//sounds//musica.mp3");
}

// *************************************************************

// Callback de redimensionamento
void redimensiona(int w, int h){
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 1000, 0, 1000, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// *************************************************************

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y){

   switch(key){
      // Tecla ESC
      case 27:
         exit(0);
         break;
      case ' ':
	  	if(p.y==200)
        	p.velocidadey=VelocidadePulo;
        if(p.y==800-p.altura)
        	p.velocidadey=-VelocidadePulo;
        PlayerEmPe();
        break;
      case 's':
      case 'S':
      case 15:
       if(p.gravidade!=1 && p.Empe==0)
       		PlayerEmPe();
       else
      	 if(p.gravidade==1)
           PlayerAbaixado();
          else{
 			 PlayerEmPe();
		     InverteGravity();
	      }
         break;
      case 'p':
      case 'P':
      	pause*=-1;
      	if(pause==1)
			desenhaCena();
         break;
      case 'e':
      case 'E':
      		Shooting();
         break;
      case 'w':
      case 'W':
      case 14:
      if(p.gravidade==1 && p.Empe==0)
       		PlayerEmPe();
       else
		if(p.gravidade==-1)
         	PlayerAbaixado();
        else{
           InverteGravity();
	   PlayerEmPe();
	 }
         break;
      case 'r':
      case 'R':
      		inicializa();
         break;
      default:
         break;

   }
}
// *************************************************************

/*void movimentoMouse(int x, int y) {
    posicaoMouse.x = x;
    posicaoMouse.y = y;
}*/

//void atualizaObjeto(Obstacle *obstaculo) {
    // faz o quadrado andar na dire??o do ponteiro
   //obstaculo->x = (obstaculo->x - 10);
    //*obstaculo.y += (*obstaculo.y - *obstaculo.y)/50.0f;

    // Pede ao GLUT para redesenhar a tela, assim que poss?vel
    //glutPostRedisplay ();
//}

// *************************************************************

// Rotina principal
int main(int argc, char **argv)
{
    // Acordando o GLUT
    glutInit(&argc, argv);

    // Definindo a vers�o do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    // Configura��o inicial da janela do GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("Terraria Game - 3.0");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutTimerFunc(5,GameManager, 0);
    PlayerEmPe();
    glutKeyboardFunc(teclado);
    inicializa();



    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
