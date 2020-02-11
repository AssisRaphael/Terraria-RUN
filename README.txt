_______________________________Terraria Run___________________________________

Terraria Run é um jogo 2D, baseano no famoso dino run, onde o jogador controla um garoto que desvia de
obstaculos e enfrenta inimigos ao longo de seu caminho. Os obstáculo e os inimigos surgem aleatóriamente
tanto no chão quanto no céu. A gravidade do jogo pode ser invertida e o jogador pode permanecer tanto no chão
quanto no céu. O seu objetivo é desviar dos obstáculos e matar o máximo de inimigos em seu caminho. Ao matar
os inimigos alguns itens são gerados e lhe concedem aumento de vida, invencibilidade temporária e aumento 
temporário de velocidade. A dificuldade do jogo almenta com o tempo e tudo o que lhe resta é correr para sobreviver.

Este jogo foi desenvolvido por Raphael de Assis Silva e Gleiston Guimarães de Assis Filho, estudantes
do CEFET-MG como um trabalho prático da disciplina de computação gŕafica, cursada no segundo período
da faculdade de Engenharia de Computação. O jogo é escrito em C e utiliza a biblioteca OpenGL. 
Os modelos desse jogo foram reciclados do jogo Terraria da Re-Logic.

__________________________________________________________________________________

Como Usar:
	O Jogo foi desenvolvido utilizando a linguagem C e foi escrito para rodar em ambiente linux. 
	Para executálo é necessário ter rinstalado as bibliotecas openGL, freeGLUT, glew e SOIL. A biblioteca
	openGL já vem instala por padrão nos sistemas linux e por isso é necessário instalar apenas as 
	outras 3 bibliotecas.
	
Para baixá-las basta executar os seguintes comandos:

sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install freeglut3-dev 			#Instala a freeGLUT
sudo apt-get install libglew-dev 			#instala a glew
sudo apt install libsoil-dev 				#instala a SOIL

Após baixar as bibliotecas, basta acessar a pasta 'src' do projeto no terminal e digitar o comando 'make'. 
Os arquivos do jogo serão compilados. Em seguida basta utilizar o comando 'make run' para rodar o jogo.

__________________________________________________________________________________

Tutorial do jogo:
		O jogador está em um cenário que se move constantemente e a única movimentação permitida é na vertical.
	Similar ao jogo Dino Run do chrome, basta pressionar a tecla 'espaço' para pular. As teclas 'w' e 's' mudam
	a posição do personagem (abaixado / em pé). Essas mesmas teclas são responsáveis por inverter a gravidade.
	Quando o personagem está em pé no chão e pressiona-se a tecla 'w' ele irá mover-se para o céu e a gravidade
	atuará ao contrário. Pressionando-se novamente 'w' o personagem irá abaixar no céu. O mesmo ocorre com a tecla 's'.
		O jogador possui uma arma e pode atirar pressionando a tecla 'e'.
		A tecla 'r' reinicia o jogo e a tecla 'p' pausa. 
		Para fechar o jogo basta precionar 'esc' ou fechar a janela.
		
		
__________________________________________________________________________________
		
Comandos:

espaço: Pula (tanto no chão quanto no céu).

w: Levanta o jogador quando estiver abaixado no chão, abaixa quando estiver em pé no céu e inverte a gravidade quando
	o jogador está em pé no chão.
	
s: Abaixa o jogador quando estiver em pé no chão, levanta quando estiver abaixado no céu e inverte a gravidade quando
	o jogador estiver em pé no céu.	

r: reinicia o jogo.

p: Pausa o jogo.

e: atira nos inimigos.

		  
