/*******************************************************************************************************************************
*	$MCD Módulo de implementação: Módulo Interface
*	Arquivo:	INTERFACE.H
*	Letras identificadoras:		ITFC
*
*	Autores:	rrc - Rafael Rubim Cabral
*
*	$HA Histórico de evolução:
*		Versâo	Autor	Data		Observações
*		1.00	rrc		16/12/2016	Começo de implementação, implementação da maioria das funções do módulo Função
*									ITFC_CarregarTexturaBMP foi parcialmente copiada da internet. Créditos para o usuário Dinesh Subedi:
*									http://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture
*		2.00	rrc		18/12/2016	Fim da implementação de módulo
*******************************************************************************************************************************/

#include "definicoes.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "partida.h"
#include "tabuleiro.h"
#include "glut.h"

#define INTERFACE_OWN
	#include "interface.h"
#undef INTERFACE_OWN


/*******************************************************************************************************************************
*	$TC Tipo de dados: ITFC Fluxo de execução
*
*	$ED Descrição do tipo:
*		Representa os determinados momentos em que o fluxo de execução do loop da janela pode estar.
*******************************************************************************************************************************/
typedef enum {
	TELA_BRANCA,
		/* Desenha tela branca antes de começar o jogo */

	INICIO,
		/* Criação da partida do jogo */

	INTERFACE,
		/* Loop da interface, durante a partida do jogo */

	FIM
		/* Após fim da partida do jogo */
} ITFC_tpFluxoExecucao;

/*******************************************************************************************************************************
*	Dados encapsulados no módulo:
*******************************************************************************************************************************/

static DEF_tpBool interfaceConfigurada = False;
	/* Para saber se a interface já foi configurada ou não */

static DEF_tpBool janelaConfigurada = False;
	/* Para saber se a janela já foi configurada ou não */

static int larguraTela;
	/* Largura da tela do computador do usuário */

static int alturaTela;
	/* Altura da tela do computador do usuário */

static void (*FuncaoInicioLoopJanela)() = NULL;
	/* Início do loop de janela do usuário */

static void (*FuncaoFimLoopJanela)() = NULL;
	/* Fim do loop de janela do usuário */

static int (*FuncaoCondicaoFimLoopJanela)() = NULL;
	/* Condição do fim do loop de janela do usuário */

static void (*FuncaoLoopInterface)() = NULL;
	/* Loop da interface do usuário */

static int (*FuncaoCondicaoFimLoopInterface)() = NULL;
	/* Condição do fim do loop de interface do usuário */

static DEF_tpBool janelaIniciada = False;
	/* Para saber se o loop principal iniciou ou não  */

static ITFC_tpFluxoExecucao fluxoExecucao = TELA_BRANCA;
	/* Estado do fluxo de execução da janela */

static GLuint texturaTabuleiro;
	/* Textura do tabuleiro */

static GLuint texturaTelaBranca;
	/* Textura da tela em branco */

/*******************************************************************************************************************************
*	Protótipos de funções encapsuladas no módulo:
*******************************************************************************************************************************/

static void ITFC_LoopPrincipal();

static void ITFC_LoopDisplay();

static ITFC_tpCondRet ITFC_Display();

static ITFC_tpCondRet ITFC_DisplayBranco();

static ITFC_tpCondRet ITFC_CarregarTexturaBMP(const char *pathArquivo, GLuint* indiceTexturaRet);

static ITFC_tpCondRet ITFC_DesenharTextura(GLuint textura, float xEsquerda, float yBaixo, float largura, float altura);

static ITFC_tpCondRet ITFC_DesenharCirculo(float xEsquerda, float yBaixo, float diametro);

static void ITFC_ApresentarErro(const char* msg);

static ITFC_tpCondRet ITFC_DefinirCor(DEF_tpCor cor);

static ITFC_tpCondRet ITFC_DesenharPeoes();

/*******************************************************************************************************************************
*	Código de funções exportadas pelo módulo:
*******************************************************************************************************************************/


/*******************************************************************************************************************************
*	Função: ITFC_ConfigurarInterface
*/
ITFC_tpCondRet ITFC_ConfigurarInterface(int* pArgc, char** argv) {

	if (interfaceConfigurada == True) {
		return ITFC_CondRetInterfaceJaConfigurada;
	}

	glutInit(pArgc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	/* Definir limites do plano cartesiano da janela: todas as coordenadas, a propósito de desenho, estão entre -1 e 1 (da esquerda
	pra direita da janela) horizontalmente e entre -1 e 1 (de baixo para cima da janela) verticalmente. */
	gluOrtho2D(-1, 1, -1, 1);

	larguraTela = glutGet(GLUT_SCREEN_WIDTH);
	alturaTela = glutGet(GLUT_SCREEN_HEIGHT);

	interfaceConfigurada = True;

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_ConfigurarInterface */

/*******************************************************************************************************************************
*	Função: ITFC_ConfigurarJanela
*/
ITFC_tpCondRet ITFC_ConfigurarJanela(int larguraJanela, int alturaJanela) {

	int janelaXInicial = (larguraTela * 3 / 4) - (larguraJanela / 2);
	int janelaYInicial = (alturaTela - alturaJanela) / 2;

	if (interfaceConfigurada == False) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}
	if (janelaConfigurada == True) {
		return ITFC_CondRetJanelaJaConfigurada;
	}

	glutInitWindowSize(larguraJanela, alturaJanela);
	glutInitWindowPosition(janelaXInicial, janelaYInicial);
	
	glutCreateWindow("LUDO 10/10");

	glutDisplayFunc(ITFC_LoopDisplay);

	janelaConfigurada = True;

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_ConfigurarJanela */

/*******************************************************************************************************************************
*	Função: ITFC_ConfigurarLoopJanela
*/
ITFC_tpCondRet ITFC_ConfigurarLoopJanela(void (*inicioLoopJanela)(), void (*fimLoopJanela)(), int (*condicaoFimLoop)()) {

	if (!interfaceConfigurada) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}

	FuncaoInicioLoopJanela = inicioLoopJanela;
	FuncaoFimLoopJanela = fimLoopJanela;
	FuncaoCondicaoFimLoopJanela = condicaoFimLoop;

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_ConfigurarLoopJanela */

/*******************************************************************************************************************************
*	Função: ITFC_CarregarImagens
*/
ITFC_tpCondRet ITFC_CarregarImagens() {
	ITFC_tpCondRet debugInterface;

	/* Carregar imagem do tabuleiro */
	debugInterface = ITFC_CarregarTexturaBMP("tabuleiro.bmp", &texturaTabuleiro);
	/* Se não retornou OK, erro */
	if (debugInterface) {
		return debugInterface;
	}

	/* Carregar imagem da tela em branco */
	debugInterface = ITFC_CarregarTexturaBMP("telaBranca.bmp", &texturaTelaBranca);
	/* Se não retornou OK, erro */
	if (debugInterface) {
		return debugInterface;
	}
	return ITFC_CondRetOK;
}	/* Fim Função ITFC_CarregarImagens */

/*******************************************************************************************************************************
*	Função: ITFC_ConfigurarLoopInterface
*/
ITFC_tpCondRet ITFC_ConfigurarLoopInterface(void (*loopInterface)(), int (*condicaoFimLoop)()) {

	if (!interfaceConfigurada) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}

	FuncaoLoopInterface = loopInterface;
	FuncaoCondicaoFimLoopInterface = condicaoFimLoop;

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_ConfigurarLoopInterface */

/*******************************************************************************************************************************
*	Função: ITFC_IniciarJanela
*/
ITFC_tpCondRet ITFC_IniciarJanela() {

	if (interfaceConfigurada == False) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}
	if (janelaConfigurada == False) {
		return ITFC_CondRetJanelaNaoConfigurada;
	}

	janelaIniciada = True;

	glutMainLoop();

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_IniciarJanela */

/*******************************************************************************************************************************
*	Código de funções encapsuladas no módulo:
*******************************************************************************************************************************/

/*******************************************************************************************************************************
*	$FC Função: ITFC_LoopDisplay
*
*	$ED Descrição da função:
*		Loop do glutDisplayFunc
*******************************************************************************************************************************/
static void ITFC_LoopDisplay() {

	int retorno;
	ITFC_tpCondRet debugInterface;

	/* Testar onde está fluxo de execução do programa */
	switch(fluxoExecucao) {
		case TELA_BRANCA:
			/* Desenhar tela em branco */
			debugInterface = ITFC_DisplayBranco();
			/* Se não retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro em display da interface.");
			}
			/* Mudar fluxo de execução */
			fluxoExecucao = INICIO;


		case INICIO:

			/* Criar partida */
			FuncaoInicioLoopJanela();

			/* Mudar fluxo de execução */
			fluxoExecucao = INTERFACE;

			/* Começar a desenhar partida */
			debugInterface = ITFC_Display();
			/* Se não retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro em display da interface.");
			}

			break;
		case INTERFACE:

			/* Executar turno da partida */
			FuncaoLoopInterface();

			/* Testar se partida acabou */
			retorno = FuncaoCondicaoFimLoopInterface();

			/* Se partida acabar, mudar fluxo de execução */
			if (retorno) {
				fluxoExecucao = FIM;
			}

			/* Desenhar partida */
			debugInterface = ITFC_Display();
			/* Se não retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro em display da interface.");
			}

			break;
		case FIM:

			/* Finalizar partida */
			FuncaoFimLoopJanela();

			/* Testar se jogo deve ser reiniciado */
			retorno = FuncaoCondicaoFimLoopJanela();
			if (retorno) {
					/* Em caso negativo, fechar jogo */

				exit(0);
			} else {
					/* Em caso positivo, mudar fluxo de execução */

				fluxoExecucao = INICIO;
			}

			/* Desenhar tela em branco */
			debugInterface = ITFC_DisplayBranco();
			/* Se não retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro em display da interface.");
			}

			break;
		default:
			ITFC_ApresentarErro("Erro: fluxo de execucao inesperado.");
		}	/* switch */

	glutPostRedisplay();
}	/* Fim Função ITFC_LoopDisplay */

/*******************************************************************************************************************************
*	$FC Função: ITFC_LoopDisplay
*
*	$ED Descrição da função:
*		Desenha tabuleiro e peões na tela. Essa função deve ser chamada dentro do loop de display.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*******************************************************************************************************************************/
static ITFC_tpCondRet ITFC_Display() {

	ITFC_tpCondRet debugInterface;

	/* Limpar buffer secundário */
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Desenhar tabuleiro */
	debugInterface = ITFC_DesenharTextura(texturaTabuleiro, -1, -1, 2, 2);
	/* Se não retornou OK, erro */
	if (debugInterface) {
		ITFC_ApresentarErro("Erro em desenho de tabuleiro.");
	}

	/* Desenhar todos os peões de cada jogador */
	debugInterface = ITFC_DesenharPeoes();
	/* Se não retornou OK, erro */
	if (debugInterface) {
		ITFC_ApresentarErro("Erro em desenho de peoes.");
	}
	
	/* Imprimir buffer secundário */
	glutSwapBuffers();

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_Display */

/*******************************************************************************************************************************
*	$FC Função: ITFC_DisplayBranco
*
*	$ED Descrição da função:
*		Desenha imagem em branco. Essa função deve ser chamada dentro do loop de display.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*******************************************************************************************************************************/
static ITFC_tpCondRet ITFC_DisplayBranco() {

	ITFC_tpCondRet debugInterface;

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Desenhar tabuleiro */
	debugInterface = ITFC_DesenharTextura(texturaTelaBranca, -1, -1, 2, 2);
	/* Se não retornou OK, erro */
	if (debugInterface) {
		ITFC_ApresentarErro("Erro em desenho de tabuleiro.");
	}

	glutSwapBuffers();

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_DisplayBranco */

/*******************************************************************************************************************************
*	$FC Função: ITFC_CarregarTexturaBMP
*
*	$ED Descrição da função:
*		Recebe o caminho de uma imagem no formato .bmp, carrega-a como textura no openGL e retorna índice da textura.
*
*	$EP Parâmetros:
*		$P pathArquivo		-	caminho para arquivo em formato .bmp da imagem.
*		$P indiceTexturaRet	-	parâmetro que receberá índice da textura carregada. Este parâmetro é passado como referência.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetErroCarregamentoImagem
*		ITFC_CondRetSemMemoria
*******************************************************************************************************************************/
static ITFC_tpCondRet ITFC_CarregarTexturaBMP(const char *pathArquivo, GLuint* indiceTexturaRet) {
	GLuint textura;
	unsigned char* vetorImagem;
	unsigned char* ponteiro;
	unsigned char header[54];
	FILE * imagem;
	int i;
	int larguraImagem = 0;
	int alturaImagem = 0;

	/* Abrir arquivo da imagem como binário para leitura */
	imagem = fopen( pathArquivo, "rb" );
	/* Se não carregar, erro */
	if (imagem == NULL) {
		return ITFC_CondRetErroCarregamentoImagem;
	}

	/* Segundo a internet, headers de imagens .bmp têm 54 bytes (se está na internet só pode ser verdade) */
	/* Guardar header em buffer */
	fread(header, 54, 1, imagem);

	/* Carregar largura da imagem a partir do header (valor inteiro de 4 bytes que começa no 19º byte, segundo a internet) */
	/* Copiar de header para variável, byte a byte */
	ponteiro = (unsigned char*) &larguraImagem;
	for (i = 0; i < 4; i++) {
		*ponteiro = header[18+i];
		ponteiro++;
	}
	/* Carregar altura da imagem a partir do header (valor inteiro de 4 bytes que começa no 23º byte, segundo a internet) */
	/* Copiar de header para variável, byte a byte */
	ponteiro = (unsigned char*) &alturaImagem;
	for (i = 0; i < 4; i++) {
		*ponteiro = header[22+i];
		ponteiro++;
	}

	/* Carregar buffer pra guardar imagem */
	vetorImagem = (unsigned char*) malloc(larguraImagem*alturaImagem*3);
	/* Se não houve espaço, erro */
	if (vetorImagem == NULL) {
		return ITFC_CondRetSemMemoria;
	}
	
	/* Carregar imagem em buffer */
	fread(vetorImagem, larguraImagem*alturaImagem*3, 1, imagem);

	/* Fechar arquivo binário */
	fclose(imagem);

	/* Inverte R e B porque imagens .bmp são guardadas na memória em formato BGR ao invés de RGB */
	for(i = 0; i < larguraImagem*alturaImagem; i++) {
		int indice = i*3;
		unsigned char B,R;

		B = vetorImagem[indice];
		R = vetorImagem[indice+2];

		vetorImagem[indice] = R;
		vetorImagem[indice+2] = B;
	}

	/* Criar textura no openGL */
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);

	/* Carregar buffer da imagem em textura (é tudo copiado da internet) */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, larguraImagem, alturaImagem, GL_RGB, GL_UNSIGNED_BYTE, vetorImagem);

	/* Liberar buffer */
	free(vetorImagem);

	/* Retornar índice da textura */
	*indiceTexturaRet = textura;

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_CarregarTexturaBMP */

/*******************************************************************************************************************************
*	$FC Função: ITFC_DesenharTextura
*
*	$ED Descrição da função:
*		Recebe índice de uma textura e a desenha em tela com o vértice inferior esquerdo nas coordenadas (x, y) passadas e com
*		a largura e altura especificados. Essa função deve ser chamada dentro do loop de display.
*
*	$EP Parâmetros:
*		$P textura		-	índice descritor da textura.
*		$P xEsquerda	-	coordenada x do vértice inferior esquerdo
*		$P yBaixo		-	coordenada y do vértice inferior esquerdo
*		$P largura		-	largura da textura em desenho
*		$P altura		-	altura da textura em desenho
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*******************************************************************************************************************************/
static ITFC_tpCondRet ITFC_DesenharTextura(GLuint textura, float xEsquerda, float yBaixo, float largura, float altura) {
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textura);

	glColor3f(1,1,1);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);	glVertex2f(xEsquerda, yBaixo);
		glTexCoord2f(1, 0);	glVertex2f(xEsquerda + largura, yBaixo);
		glTexCoord2f(1, 1);	glVertex2f(xEsquerda + largura, yBaixo + altura);
		glTexCoord2f(0, 1);	glVertex2f(xEsquerda, yBaixo + altura);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	return ITFC_CondRetOK;
}

/*******************************************************************************************************************************
*	$FC Função: ITFC_DesenharCirculo
*
*	$ED Descrição da função:
*		Recebe coordenadas (x, y) do vértice inferior esquerdo de um quadrado imaginário e o tamanho do lado desse quadrado.
*		Desenha um círculo preenchido inscrito nesse quadrado imaginário (mas não desenha quadrado nenhum). Note que o lado
*		do quadrado será o diâmetro do círculo. Essa função deve ser chamada dentro do loop de display.
*
*	$EP Parâmetros:
*		$P xEsquerda	-	coordenada x da extremidade esquerda do círculo
*		$P yBaixo		-	coordenada y da extremidade de baixo do círculo
*		$P diametro		-	diâmetro do círculo (ou lado de um quadrado imaginário)
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*******************************************************************************************************************************/
static ITFC_tpCondRet ITFC_DesenharCirculo(float xEsquerda, float yBaixo, float diametro) {
	float raio = diametro / 2;
	float xCentro = xEsquerda + raio;
	float yCentro = yBaixo + raio;
	int pontosCircunferencia = 50;
	int i;
	float diferencaAngulos = 2*3.1415 / pontosCircunferencia;
	float anguloPonto = 0;
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(xCentro, yCentro);
		for (i = 0; i <= pontosCircunferencia; i++) {
			glVertex2f(xCentro + raio*cos(anguloPonto), yCentro + raio*sin(anguloPonto));
			anguloPonto += diferencaAngulos;
		}
	glEnd();
	return ITFC_CondRetOK;
}

/*******************************************************************************************************************************
*	$FC Função: ITFC_ApresentarErro
*
*	$ED Descrição da função:
*		Imprime mensagem de erro passada como erro do módulo interface e encerra programa.
*
*	$EP Parâmetros:
*		$P msg	-	mensagem a ser impressa antes do encerramento do programa
*******************************************************************************************************************************/
static void ITFC_ApresentarErro(const char* msg) {
	system("cls");
	printf("ITFC (Modulo interface): ");
	printf(msg);
	puts("\n");
	exit(1);
}

/*******************************************************************************************************************************
*	$FC Função: ITFC_DefinirCor
*
*	$ED Descrição da função:
*		Recebe uma cor predefinida e define o padrão de desenho dos objetos posteriores à chamada da função como aquela cor.
*		SEM_COR é desenhado como preto. Essa função deve ser chamada dentro do loop de display.
*
*	$EP Parâmetros:
*		$P cor	-	cor a ser definida
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*******************************************************************************************************************************/
static ITFC_tpCondRet ITFC_DefinirCor(DEF_tpCor cor) {
	switch(cor) {
		case AZUL:
			glColor3f(0,0,1);
			break;
		case AMARELO:
			glColor3f(1,1,0);
			break;
		case VERDE:
			glColor3f(0,1,0);
			break;
		case VERMELHO:
			glColor3f(1,0,0);
			break;
		default:
			glColor3f(0,0,0);
	}
	return ITFC_CondRetOK;
}

/*******************************************************************************************************************************
*	$FC Função: ITFC_DesenharPeoes
*
*	$ED Descrição da função:
*		Desenha os peões de todos os jogadores participantes da partida atual. Essa função deve ser chamada dentro do loop de display.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*******************************************************************************************************************************/
static ITFC_tpCondRet ITFC_DesenharPeoes() {

	PART_tpCondRet debugPartida;
	ITFC_tpCondRet debugInterface;
	int qtdJogadores;
	int qtdPeoes;
	int i, j;

	/* Pegar quantidade de jogadores participantes da partida */
	debugPartida = PART_ObterQtdJogadores(&qtdJogadores);
	/* Se não retornou OK, erro */
	if (debugPartida == PART_CondRetPartidaInexistente) {
		ITFC_ApresentarErro("Erro de desenho da interface: Nenhuma partida foi iniciada.");
	}
	if (debugPartida) {
		ITFC_ApresentarErro("Erro no modulo PART.");
	}

	/* Pegar quantidade de peões por jogador */
	debugPartida = PART_ObterQtdPeoes(&qtdPeoes);
	/* Se não retornou OK, erro */
	if (debugPartida) {
		ITFC_ApresentarErro("Erro no modulo PART.");
	}

	for (i = 1; i <= qtdJogadores; i++) {
		DEF_tpCor corJog;

		/* Pegar cor do jogador */
		debugPartida = PART_ObterCorJogador(i, &corJog);
		/* Se não retornou OK, erro */
		if (debugPartida) {
			ITFC_ApresentarErro("Erro no modulo PART.");
		}

		/* Desenhar peões */
		for(j = 1; j <= qtdPeoes; j++) {
			TAB_tppCasa tempCasa;
			TAB_tpCondRet debugTabuleiro;
			int xTab, yTab;
			float tamanhoLadoCasa;
			float xCasa, yCasa;
			float xAjusteFonte, yAjusteFonte;
			float bordaPeao;

			/* Pegar tamanho do lado de uma casa */
			tamanhoLadoCasa = 2.0 / 15;

			/* Obter casa do peão */
			debugPartida = PART_ObterCasaPeaoJogador(i, j, &tempCasa);
			/* Se não retornou OK, erro */
			if (debugPartida) {
				ITFC_ApresentarErro("Erro no modulo PART.");
			}

			if (tempCasa != NULL) {
					/* Peão não está na base */

				/* Obter posição (x, y) da casa (nos critérios do módulo TAB) */
				debugTabuleiro = TAB_ObterPosicaoCasa(tempCasa, &xTab, &yTab);
				/* Se não retornou OK, erro */
				if (debugTabuleiro) {
					ITFC_ApresentarErro("Erro no modulo TAB.");
				}

				/* Converte posição (x, y) para critérios do openGL */
				xCasa = (xTab * tamanhoLadoCasa) - 1;
				yCasa = (yTab * tamanhoLadoCasa) - 1;

			} else {
					/* Peão está na base */

				int xTabBase, yTabBase;
				float xBase, yBase;

				/* Pegar coordenadas do tabuleiro (em critérios do módulo TAB) */
				switch(corJog) {
					case AZUL:
						xTabBase = 9;
						yTabBase = 0;
						break;
					case AMARELO:
						xTabBase = 0;
						yTabBase = 0;
						break;
					case VERDE:
						xTabBase = 0;
						yTabBase = 9;
						break;
					case VERMELHO:
						xTabBase = 9;
						yTabBase = 9;
						break;
					default:
						ITFC_ApresentarErro("Erro de inconsistencia da cor do jogador.");
				}

				/* Converte posição (x, y) da base para critérios do openGL */
				xBase = (xTabBase * tamanhoLadoCasa) - 1;
				yBase = (yTabBase * tamanhoLadoCasa) - 1;

				/* Obtém posição (x, y) do peão na base */
				switch(j) {
					case 1:
						xCasa = xBase + (2.5 * tamanhoLadoCasa);
						yCasa = yBase + (3.5 * tamanhoLadoCasa);
						break;
					case 2:
						xCasa = xBase + (1.5 * tamanhoLadoCasa);
						yCasa = yBase + (2.5 * tamanhoLadoCasa);
						break;
					case 3:
						xCasa = xBase + (3.5 * tamanhoLadoCasa);
						yCasa = yBase + (2.5 * tamanhoLadoCasa);
						break;
					case 4:
						xCasa = xBase + (2.5 * tamanhoLadoCasa);
						yCasa = yBase + (1.5 * tamanhoLadoCasa);
						break;
					default:
						ITFC_ApresentarErro("Erro de inconsistência do numero do peao.");
				}
			}

			bordaPeao = tamanhoLadoCasa * 1 / 30;

			glColor3f(0, 0, 0);

			/* Desenhar círculo preto um pouco maior para fazer borda no peão */
			debugInterface = ITFC_DesenharCirculo(xCasa - bordaPeao, yCasa - bordaPeao, tamanhoLadoCasa + (2 * bordaPeao));
			/* Se não retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro ao desenhar peao.");
			}

			/* Definir cor de desenho como cor do jogador */
			debugInterface = ITFC_DefinirCor(corJog);
			/* Se não retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro em definicao de cor do jogador.");
			}

			/* Desenhar peão em sua posição */
			debugInterface = ITFC_DesenharCirculo(xCasa, yCasa, tamanhoLadoCasa);
			/* Se não retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro ao desenhar peao.");
			}

			xAjusteFonte = tamanhoLadoCasa * 1 / 3;
			yAjusteFonte = tamanhoLadoCasa * 1 / 3;

			glColor3f(0, 0, 0);
			glRasterPos2f(xCasa + xAjusteFonte, yCasa + yAjusteFonte);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, j + '0');
		}
	}

	return ITFC_CondRetOK;
}

/*******************************************************************************************************************************
*	Fim do módulo de implementação: Módulo Interface
*******************************************************************************************************************************/