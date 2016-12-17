#include "interface.h"
#include "definicoes.h"

#include "glut.h"

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

static void (*FuncaoLoopPrincipal)() = 0;
	/* Loop principal do usu�rio */

static void (*FuncaoLoopJanela)() = 0;
	/* Loop da interface do usu�rio */

static DEF_tpBool janelaIniciada = False;
	/* Para saber se a interface iniciou ou n�o  */

static DEF_tpBool interfaceIniciada = False;
	/* Para saber se a interface iniciou ou n�o  */

static DEF_tpBool interfaceMeioExecu��o = False;
	/* Trigger que indica se interface est� ou n�o no meio do loop */

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
}	/* Fim Fun��o ITFC_ConfigurarJanela */

/*******************************************************************************************************************************
*	Fun��o: ITFC_ConfigurarLoopJanela
*/
ITFC_tpCondRet ITFC_ConfigurarLoopJanela(void (*funcaoLoopJanela)()) {

	if (!interfaceConfigurada) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}

	FuncaoLoopJanela = funcaoLoopJanela;

	return ITFC_CondRetOK;
}	/* Fim Fun��o ITFC_ConfigurarLoopJanela */

/*******************************************************************************************************************************
*	Fun��o: ITFC_ConfigurarLoopInterface
*/
ITFC_tpCondRet ITFC_ConfigurarLoopInterface(void (*funcaoLoopPrincipal)()) {

	if (!interfaceConfigurada) {
		return ITFC_CondRetInterfaceNaoConfigurada;
	}

	FuncaoLoopPrincipal = funcaoLoopPrincipal;

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
*	Fun��o: ITFC_IniciarInterface
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
}	/* Fim Fun��o ITFC_IniciarInterface */

/*******************************************************************************************************************************
*	C�digo de fun��es encapsuladas no m�dulo:
*******************************************************************************************************************************/

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_LoopPrincipal
*
*	$ED Descri��o da fun��o:
*		Loop do glutIdleFunc
*******************************************************************************************************************************/
static void ITFC_LoopPrincipal() {

	FuncaoLoopPrincipal();
}	/* Fim Fun��o ITFC_LoopPrincipal */

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_LoopDisplay
*
*	$ED Descri��o da fun��o:
*		Loop do glutDisplayFunc
*******************************************************************************************************************************/
static void ITFC_LoopDisplay() {

	if (interfaceIniciada == True) {
			/* Se interface est� em loop */

		/* Desenhar tebuleiro */
		ITFC_Display();

		/* Chama a si mesma (porque est� em loop) */
		/*glutPostRedisplay();*/
	} else {
			/* Se interface n�o est� em loop, desenhar imagem btanca */

		ITFC_DisplayBranco();
	}	/* if */
}	/* Fim Fun��o ITFC_LoopDisplay */

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_LoopDisplay
*
*	$ED Descri��o da fun��o:
*		Desenha tabuleiro e pe�es na tela
*******************************************************************************************************************************/
static void ITFC_Display() {

	glClearColor(1, 1, 1, 0);
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