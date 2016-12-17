#include "interface.h"
#include "definicoes.h"
#include <stdlib.h>

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

/*******************************************************************************************************************************
*	Prot�tipos de fun��es encapsuladas no m�dulo:
*******************************************************************************************************************************/

static void ITFC_LoopPrincipal();

static void ITFC_LoopDisplay();

static void ITFC_Display();

static void ITFC_DisplayBranco();

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