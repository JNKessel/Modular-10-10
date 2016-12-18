#include "interface.h"
#include "definicoes.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "partida.h"
#include "tabuleiro.h"

#include "glut.h"

typedef enum {
	INICIO,
	INTERFACE,
	FIM
} ITFC_tpFluxoExecucao;

/*******************************************************************************************************************************
*	Dados encapsulados no m�dulo:
*******************************************************************************************************************************/

static DEF_tpBool interfaceConfigurada = False;
	/* Para saber se a interface j� foi configurada ou n�o */

static DEF_tpBool janelaConfigurada = False;
	/* Para saber se a janela j� foi configurada ou n�o */

static int larguraTela;
	/* Largura da tela do computador do usu�rio */

static int alturaTela;
	/* Altura da tela do computador do usu�rio */

static void (*FuncaoInicioLoopJanela)() = NULL;
	/* In�cio do loop de janela do usu�rio */

static void (*FuncaoFimLoopJanela)() = NULL;
	/* Fim do loop de janela do usu�rio */

static int (*FuncaoCondicaoFimLoopJanela)() = NULL;
	/* Condi��o do fim do loop de janela do usu�rio */

static void (*FuncaoLoopInterface)() = NULL;
	/* Loop da interface do usu�rio */

static int (*FuncaoCondicaoFimLoopInterface)() = NULL;
	/* Condi��o do fim do loop de interface do usu�rio */

static DEF_tpBool janelaIniciada = False;
	/* Para saber se o loop principal iniciou ou n�o  */

static ITFC_tpFluxoExecucao fluxoExecucao = INICIO;
	/* Estado do fluxo de execu��o da janela */

static GLuint texturaTabuleiro;
	/* Textura do tabuleiro */

static GLuint texturaPeaoAzul;
	/* Textura do pe�o azul */

static GLuint texturaPeaoAmarelo;
	/* Textura do pe�o amarelo */

static GLuint texturaPeaoVerde;
	/* Textura do pe�o verde */

static GLuint texturaPeaoVermelho;
	/* Textura do pe�o vermelho */

/*******************************************************************************************************************************
*	Prot�tipos de fun��es encapsuladas no m�dulo:
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
*	C�digo de fun��es exportadas pelo m�dulo:
*******************************************************************************************************************************/


/*******************************************************************************************************************************
*	Fun��o: ITFC_ConfigurarInterface
*/
ITFC_tpCondRet ITFC_ConfigurarInterface(int* pArgc, char** argv) {

	if (interfaceConfigurada == True) {
		return ITFC_CondRetInterfaceJaConfigurada;
	}

	glutInit(pArgc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	gluOrtho2D(-1, 1, -1, 1);

	larguraTela = glutGet(GLUT_SCREEN_WIDTH);
	alturaTela = glutGet(GLUT_SCREEN_HEIGHT);

	interfaceConfigurada = True;

	return ITFC_CondRetOK;
}	/* Fim Fun��o ITFC_ConfigurarInterface */

/*******************************************************************************************************************************
*	Fun��o: ITFC_ConfigurarJanela
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
}	/* Fim Fun��o ITFC_ConfigurarJanela */

/*******************************************************************************************************************************
*	Fun��o: ITFC_ConfigurarLoopJanela
*/
ITFC_tpCondRet ITFC_ConfigurarLoopJanela(void (*inicioLoopJanela)(), void (*fimLoopJanela)(), int (*condicaoFimLoop)()) {

	if (!interfaceConfigurada) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}

	FuncaoInicioLoopJanela = inicioLoopJanela;
	FuncaoFimLoopJanela = fimLoopJanela;
	FuncaoCondicaoFimLoopJanela = condicaoFimLoop;

	return ITFC_CondRetOK;
}	/* Fim Fun��o ITFC_ConfigurarLoopJanela */

/*******************************************************************************************************************************
*	Fun��o: ITFC_CarregarImagens
*/
ITFC_tpCondRet ITFC_CarregarImagens() {
	ITFC_tpCondRet debugInterface;
	debugInterface = ITFC_CarregarTexturaBMP("tabuleiro.bmp", &texturaTabuleiro);
	if (debugInterface) {
		return debugInterface;
	}
	//debugInterface = ITFC_CarregarTexturaBMP("peaoAzul.bmp", &texturaPeaoAzul);
	//if (debugInterface) {
	//	return debugInterface;
	//}
	//debugInterface = ITFC_CarregarTexturaBMP("peaoAmarelo.bmp", &texturaPeaoAmarelo);
	//if (debugInterface) {
	//	return debugInterface;
	//}
	//debugInterface = ITFC_CarregarTexturaBMP("peaoVerde.bmp", &texturaPeaoVerde);
	//if (debugInterface) {
	//	return debugInterface;
	//}
	//debugInterface = ITFC_CarregarTexturaBMP("peaoVermelho.bmp", &texturaPeaoVermelho);
	//if (debugInterface) {
	//	return debugInterface;
	//}
	return ITFC_CondRetOK;
}	/* Fim Fun��o ITFC_CarregarImagens */

/*******************************************************************************************************************************
*	Fun��o: ITFC_ConfigurarLoopInterface
*/
ITFC_tpCondRet ITFC_ConfigurarLoopInterface(void (*loopInterface)(), int (*condicaoFimLoop)()) {

	if (!interfaceConfigurada) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}

	FuncaoLoopInterface = loopInterface;
	FuncaoCondicaoFimLoopInterface = condicaoFimLoop;

	return ITFC_CondRetOK;
}	/* Fim Fun��o ITFC_ConfigurarLoopInterface */

/*******************************************************************************************************************************
*	Fun��o: ITFC_IniciarJanela
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
}	/* Fim Fun��o ITFC_IniciarJanela */

/*******************************************************************************************************************************
*	C�digo de fun��es encapsuladas no m�dulo:
*******************************************************************************************************************************/

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_LoopDisplay
*
*	$ED Descri��o da fun��o:
*		Loop do glutDisplayFunc
*******************************************************************************************************************************/
static void ITFC_LoopDisplay() {

	int retorno;
	ITFC_tpCondRet debugInterface;

	/* Testar onde est� fluxo de execu��o do programa */
	switch(fluxoExecucao) {
		case INICIO:

			/* Criar partida */
			FuncaoInicioLoopJanela();

			/* Mudar fluxo de execu��o */
			fluxoExecucao = INTERFACE;

			/* Come�ar a desenhar partida */
			debugInterface = ITFC_Display();
			/* Se n�o retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro em display da interface.");
			}

			break;
		case INTERFACE:

			/* Executar turno da partida */
			FuncaoLoopInterface();

			/* Testar se partida acabou */
			retorno = FuncaoCondicaoFimLoopInterface();

			/* Se partida acabar, mudar fluxo de execu��o */
			if (retorno) {
				fluxoExecucao = FIM;
			}

			/* Desenhar partida */
			debugInterface = ITFC_Display();
			/* Se n�o retornou OK, erro */
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
					/* Em caso positivo, mudar fluxo de execu��o */

				fluxoExecucao = INICIO;
			}

			/* Desenhar tela em branco */
			debugInterface = ITFC_DisplayBranco();
			/* Se n�o retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro em display da interface.");
			}

			break;
		}	/* switch */

	glutPostRedisplay();
}	/* Fim Fun��o ITFC_LoopDisplay */

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_LoopDisplay
*
*	$ED Descri��o da fun��o:
*		Desenha tabuleiro e pe�es na tela
*******************************************************************************************************************************/
static ITFC_tpCondRet ITFC_Display() {

	PART_tpCondRet debugPartida;
	TAB_tpCondRet debugTabuleiro;
	ITFC_tpCondRet debugInterface;
	int qtdJogadores;
	int qtdPeoes;
	int i, j;

	/* Limpar buffer secund�rio */
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Desenhar tabuleiro */
	debugInterface = ITFC_DesenharTextura(texturaTabuleiro, -1, -1, 2, 2);
	/* Se n�o retornou OK, erro */
	if (debugInterface) {
		ITFC_ApresentarErro("Erro em desenho de tabuleiro.");
	}

	/* Desenhar todos os pe�es de cada jogador */
	debugInterface = ITFC_DesenharPeoes();
	/* Se n�o retornou OK, erro */
	if (debugInterface) {
		ITFC_ApresentarErro("Erro em desenho de peoes.");
	}
	
	/* Imprimir buffer secund�rio */
	glutSwapBuffers();

	return ITFC_CondRetOK;
}	/* Fim Fun��o ITFC_Display */

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_DisplayBranco
*
*	$ED Descri��o da fun��o:
*		Desenha imagem em branco
*******************************************************************************************************************************/
static ITFC_tpCondRet ITFC_DisplayBranco() {

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	glutSwapBuffers();

	return ITFC_CondRetOK;
}	/* Fim Fun��o ITFC_DisplayBranco */

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_CarregarTexturaBMP
*
*	$ED Descri��o da fun��o:
*		Recebe o caminho de uma imagem no formato .bmp, carrega-a como textura no openGL e retorna �ndice da textura.
*
*	$EP Par�metros:
*		$P pathArquivo		-	caminho para arquivo em formato .bmp da imagem.
*		$P indiceTexturaRet	-	par�metro que receber� �ndice da textura carregada. Este par�metro � passado como refer�ncia.
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

	/* Abrir arquivo da imagem como bin�rio para leitura */
	imagem = fopen( pathArquivo, "rb" );
	/* Se n�o carregar, erro */
	if (imagem == NULL) {
		return ITFC_CondRetErroCarregamentoImagem;
	}

	/* Segundo a internet, headers de imagens .bmp t�m 54 bytes (se est� na internet s� pode ser verdade) */
	/* Guardar header em buffer */
	fread(header, 54, 1, imagem);

	/* Carregar largura da imagem a partir do header (valor inteiro de 4 bytes que come�a no 19� byte, segundo a internet) */
	/* Copiar de header para vari�vel, byte a byte */
	ponteiro = (unsigned char*) &larguraImagem;
	for (i = 0; i < 4; i++) {
		*ponteiro = header[18+i];
		ponteiro++;
	}
	/* Carregar altura da imagem a partir do header (valor inteiro de 4 bytes que come�a no 23� byte, segundo a internet) */
	/* Copiar de header para vari�vel, byte a byte */
	ponteiro = (unsigned char*) &alturaImagem;
	for (i = 0; i < 4; i++) {
		*ponteiro = header[22+i];
		ponteiro++;
	}

	/* Carregar buffer pra guardar imagem */
	vetorImagem = (unsigned char*) malloc(larguraImagem*alturaImagem*3);
	/* Se n�o houve espa�o, erro */
	if (vetorImagem == NULL) {
		return ITFC_CondRetSemMemoria;
	}
	
	/* Carregar imagem em buffer */
	fread(vetorImagem, larguraImagem*alturaImagem*3, 1, imagem);

	/* Fechar arquivo bin�rio */
	fclose(imagem);

	/* Inverte R e B porque imagens .bmp s�o guardadas na mem�ria em formato BGR ao inv�s de RGB */
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

	/* Carregar buffer da imagem em textura (� tudo copiado da internet) */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, larguraImagem, alturaImagem, GL_RGB, GL_UNSIGNED_BYTE, vetorImagem);

	/* Liberar buffer */
	free(vetorImagem);

	/* Retornar �ndice da textura */
	*indiceTexturaRet = textura;

	return ITFC_CondRetOK;
}	/* Fim Fun��o ITFC_CarregarTexturaBMP */

static ITFC_tpCondRet ITFC_DesenharTextura(GLuint textura, float xEsquerda, float yBaixo, float largura, float altura) {
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textura);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);	glVertex2f(xEsquerda, yBaixo);
		glTexCoord2f(1, 0);	glVertex2f(xEsquerda + largura, yBaixo);
		glTexCoord2f(1, 1);	glVertex2f(xEsquerda + largura, yBaixo + altura);
		glTexCoord2f(0, 1);	glVertex2f(xEsquerda, yBaixo + altura);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	return ITFC_CondRetOK;
}

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

static void ITFC_ApresentarErro(const char* msg) {
	system("cls");
	printf("ITFC (Modulo interface): ");
	printf(msg);
	puts("\n");
	exit(1);
}

static ITFC_tpCondRet ITFC_DefinirCor(DEF_tpCor cor) {
	switch(cor) {
		case AZUL:
			glColor3f(0,0,1);
			break;
		case AMARELO:
			glColor3f(0,0.8,0.8);
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

static ITFC_tpCondRet ITFC_DesenharPeoes() {

	PART_tpCondRet debugPartida;
	TAB_tpCondRet debugTabuleiro;
	ITFC_tpCondRet debugInterface;
	int qtdJogadores;
	int qtdPeoes;
	int i, j;

	/* Pegar quantidade de jogadores participantes da partida */
	debugPartida = PART_ObterQtdJogadores(&qtdJogadores);
	/* Se n�o retornou OK, erro */
	if (debugPartida == PART_CondRetPartidaInexistente) {
		ITFC_ApresentarErro("Erro de desenho da interface: Nenhuma partida foi iniciada.");
	}
	if (debugPartida) {
		ITFC_ApresentarErro("Erro no m�dulo PART.");
	}

	/* Pegar quantidade de pe�es por jogador */
	debugPartida = PART_ObterQtdPeoes(&qtdPeoes);
	/* Se n�o retornou OK, erro */
	if (debugPartida) {
		ITFC_ApresentarErro("Erro no m�dulo PART.");
	}

	for (i = 1; i <= qtdJogadores; i++) {
		DEF_tpCor corJog;

		/* Pegar cor do jogador */
		debugPartida = PART_ObterCorJogador(i, &corJog);
		/* Se n�o retornou OK, erro */
		if (debugPartida) {
			ITFC_ApresentarErro("Erro no m�dulo PART.");
		}

		/* Desenhar pe�es */
		for(j = 1; j <= qtdPeoes; j++) {
			TAB_tppCasa tempCasa;
			TAB_tpCondRet debugTabuleiro;
			int xTab, yTab;
			float tamanhoLadoCasa;
			float xCasa, yCasa;
			float xAjusteFonte, yAjusteFonte;

			/* Pegar tamanho do lado de uma casa */
			tamanhoLadoCasa = 2.0 / 15;

			/* Obter casa do pe�o */
			debugPartida = PART_ObterCasaPeaoJogador(i, j, &tempCasa);
			/* Se n�o retornou OK, erro */
			if (debugPartida) {
				ITFC_ApresentarErro("Erro no m�dulo PART.");
			}

			if (tempCasa != NULL) {
					/* Pe�o n�o est� na base */

				/* Obter posi��o (x, y) da casa (nos crit�rios do m�dulo TAB) */
				debugTabuleiro = TAB_ObterPosicaoCasa(tempCasa, &xTab, &yTab);
				/* Se n�o retornou OK, erro */
				if (debugTabuleiro) {
					ITFC_ApresentarErro("Erro no m�dulo TAB.");
				}

				/* Converte posi��o (x, y) para crit�rios do openGL */
				xCasa = (xTab * tamanhoLadoCasa) - 1;
				yCasa = (yTab * tamanhoLadoCasa) - 1;

			} else {
					/* Pe�o est� na base */

				int xTabBase, yTabBase;
				float xBase, yBase;

				/* Pegar coordenadas do tabuleiro (em crit�rios do m�dulo TAB) */
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
						ITFC_ApresentarErro("Erro de inconsist�ncia da cor do jogador.");
				}

				/* Converte posi��o (x, y) da base para crit�rios do openGL */
				xBase = (xTabBase * tamanhoLadoCasa) - 1;
				yBase = (yTabBase * tamanhoLadoCasa) - 1;

				/* Obt�m posi��o (x, y) do pe�o na base */
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
						ITFC_ApresentarErro("Erro de inconsist�ncia do n�mero do pe�o.");
				}
			}

			/* Definir cor de desenho como cor do jogador */
			debugInterface = ITFC_DefinirCor(corJog);
			/* Se n�o retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro em definicao de cor do jogador.");
			}

			/* Desenhar pe�o em sua posi��o */
			debugInterface = ITFC_DesenharCirculo(xCasa, yCasa, tamanhoLadoCasa);
			/* Se n�o retornou OK, erro */
			if (debugInterface) {
				ITFC_ApresentarErro("Erro ao desenhar pe�o.");
			}

			xAjusteFonte = tamanhoLadoCasa * 1 / 3;
			yAjusteFonte = tamanhoLadoCasa * 1 / 3;

			glColor3f(0, 0, 0);
			glRasterPos2f(xCasa + xAjusteFonte, yCasa + yAjusteFonte);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, j + '0');
		}
	}

	return ITFC_CondRetOK;
}