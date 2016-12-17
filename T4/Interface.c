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

static ITFC_tpFluxoExecucao fluxoExecucao = INICIO;
	/* Estado do fluxo de execução da janela */

/*******************************************************************************************************************************
*	Protótipos de funções encapsuladas no módulo:
*******************************************************************************************************************************/

static void ITFC_LoopPrincipal();

static void ITFC_LoopDisplay();

static void ITFC_Display();

static void ITFC_DisplayBranco();

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
}	/* Fim Função ITFC_LoopDisplay */

/*******************************************************************************************************************************
*	$FC Função: ITFC_LoopDisplay
*
*	$ED Descrição da função:
*		Desenha tabuleiro e peões na tela
*******************************************************************************************************************************/
static void ITFC_Display() {

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	/* glutGet(GLUT_WINDOW_WIDTH); */
	/* glutGet(GLUT_WINDOW_HEIGHT); */

	glutSwapBuffers();
}	/* Fim Função ITFC_Display */

/*******************************************************************************************************************************
*	$FC Função: ITFC_DisplayBranco
*
*	$ED Descrição da função:
*		Desenha imagem em branco
*******************************************************************************************************************************/
static void ITFC_DisplayBranco() {

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	glutSwapBuffers();
}	/* Fim Função ITFC_DisplayBranco */