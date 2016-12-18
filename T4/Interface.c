#include "interface.h"
#include "definicoes.h"
#include <stdlib.h>
#include <stdio.h>

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

static void ITFC_Display();

static void ITFC_DisplayBranco();

static ITFC_tpCondRet ITFC_CarregarTexturaBMP(const char *pathArquivo, GLuint* indiceTexturaRet);

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

	glEnable(GL_TEXTURE_2D);

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

	switch(fluxoExecucao) {
		case INICIO:

			FuncaoInicioLoopJanela();
			fluxoExecucao = INTERFACE;
			ITFC_Display();

			break;
		case INTERFACE:

			FuncaoLoopInterface();
			retorno = FuncaoCondicaoFimLoopInterface();
			if (retorno) {
				fluxoExecucao = FIM;
			}
			ITFC_Display();

			break;
		case FIM:

			FuncaoFimLoopJanela();
			retorno = FuncaoCondicaoFimLoopJanela();
			if (retorno) {
				exit(0);
			} else {
				fluxoExecucao = INICIO;
			}
			ITFC_DisplayBranco();

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
static void ITFC_Display() {

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	/* glutGet(GLUT_WINDOW_WIDTH); */
	/* glutGet(GLUT_WINDOW_HEIGHT); */

	glBindTexture(GL_TEXTURE_2D, texturaTabuleiro);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);	glVertex3f(-1, -1, 0);
		glTexCoord2f(1, 0);	glVertex3f(1, -1, 0);
		glTexCoord2f(1, 1);	glVertex3f(1, 1, 0);
		glTexCoord2f(0, 1);	glVertex3f(-1, 1, 0);
	glEnd();

	glutSwapBuffers();
}	/* Fim Fun��o ITFC_Display */

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_DisplayBranco
*
*	$ED Descri��o da fun��o:
*		Desenha imagem em branco
*******************************************************************************************************************************/
static void ITFC_DisplayBranco() {

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	glutSwapBuffers();
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
	free( vetorImagem );

	/* Retornar �ndice da textura */
	*indiceTexturaRet = textura;

	return ITFC_CondRetOK;
}	/* Fim Fun��o ITFC_CarregarTexturaBMP */