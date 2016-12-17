#include "interface.h"
#include "definicoes.h"

#include "glut.h"

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

static void (*FuncaoLoopPrincipal)() = 0;
	/* Loop principal do usuário */

static void (*FuncaoLoopJanela)() = 0;
	/* Loop da interface do usuário */

static DEF_tpBool janelaIniciada = False;
	/* Para saber se a interface iniciou ou não  */

static DEF_tpBool interfaceIniciada = False;
	/* Para saber se a interface iniciou ou não  */

static DEF_tpBool interfaceMeioExecução = False;
	/* Trigger que indica se interface está ou não no meio do loop */

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

	int janelaXInicial = (larguraTela - larguraJanela) / 2;
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
	glutIdleFunc(ITFC_Display);

	janelaConfigurada = True;

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_ConfigurarJanela */

/*******************************************************************************************************************************
*	Função: ITFC_ConfigurarLoopJanela
*/
ITFC_tpCondRet ITFC_ConfigurarLoopJanela(void (*funcaoLoopJanela)()) {

	if (!interfaceConfigurada) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}

	FuncaoLoopJanela = funcaoLoopJanela;

	return ITFC_CondRetOK;
}	/* Fim Função ITFC_ConfigurarLoopJanela */

/*******************************************************************************************************************************
*	Função: ITFC_ConfigurarLoopInterface
*/
ITFC_tpCondRet ITFC_ConfigurarLoopInterface(void (*funcaoLoopPrincipal)()) {

	if (!interfaceConfigurada) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}

	FuncaoLoopPrincipal = funcaoLoopPrincipal;

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
*	Função: ITFC_IniciarInterface
*/
ITFC_tpCondRet ITFC_IniciarInterface() {

	if (interfaceConfigurada == False) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}
	if (janelaIniciada == False) {
		return ITFC_CondRetJanelaNaoIniciada;
	}
	if (interfaceIniciada == True) {
		return ITFC_CondRetInterfaceJaIniciada;
	}

	interfaceIniciada = True;

	glutPostRedisplay();
}	/* Fim Função ITFC_IniciarInterface */

/*******************************************************************************************************************************
*	Código de funções encapsuladas no módulo:
*******************************************************************************************************************************/

/*******************************************************************************************************************************
*	$FC Função: ITFC_LoopPrincipal
*
*	$ED Descrição da função:
*		Loop do glutIdleFunc
*******************************************************************************************************************************/
static void ITFC_LoopPrincipal() {

	FuncaoLoopPrincipal();
}	/* Fim Função ITFC_LoopPrincipal */

/*******************************************************************************************************************************
*	$FC Função: ITFC_LoopDisplay
*
*	$ED Descrição da função:
*		Loop do glutDisplayFunc
*******************************************************************************************************************************/
static void ITFC_LoopDisplay() {

	if (interfaceIniciada == True) {
			/* Se interface está em loop */

		/* Desenhar tebuleiro */
		ITFC_Display();

		/* Chama a si mesma (porque está em loop) */
		/*glutPostRedisplay();*/
	} else {
			/* Se interface não está em loop, desenhar imagem btanca */

		ITFC_DisplayBranco();
	}	/* if */
}	/* Fim Função ITFC_LoopDisplay */

/*******************************************************************************************************************************
*	$FC Função: ITFC_LoopDisplay
*
*	$ED Descrição da função:
*		Desenha tabuleiro e peões na tela
*******************************************************************************************************************************/
static void ITFC_Display() {

	glClearColor(1, 1, 1, 0);
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