/*******************************************************************************************************************************
*	$MCD M�dulo de defini��o: M�dulo Interface
*	Arquivo:	INTERFACE.H
*	Letras identificadoras:		ITFC
*
*	Autores:	rrc - Rafael Rubim Cabral
*
*	$HA Hist�rico de evolu��o:
*		Vers�o	Autor	Data		Observa��es
*		1.00	rrc		16/12/2016	Come�o de implementa��o, implementa��o da maioria das fun��es do m�dulo
*		3.00	rrc		18/12/2016	Fim da implementa��o de m�dulo
*
*	$ED Descri��o do m�dulo:
*		Este m�dulo permite o desenho na tela do jogo de ludo atrav�s de uma interface gr�fica. Iniciar uma janela para imprimir
*		a interface gr�fica toma posse do fluxo de execu��o do programa, o que significa que nenhuma fun��o chamada ap�s
*		a fun��o ITFC_IniciarJanela funcionar�, e ela n�o retornar�. Para que o programador ainda tenha controle sobre o que
*		ocorre no fluxo de execu��o ap�s a janela da interface ser executada, pode-se implementar fun��es pr�prias que ser�o
*		passadas antes para as fun��es ITFC_ConfigurarLoopJanela e ITFC_ConfigurarLoopInterface. Isso far� com que tais fun��es
*		pr�prias sejam executadas em loop (em ordem predeterminada) durante a exist�ncia da janela.
*******************************************************************************************************************************/

#pragma once

/*******************************************************************************************************************************
*	$TC Tipo de dados: ITFC Condi��es de retorno
*
*	$ED Descri��o do tipo:
*		Condi��es de retorno das fun��es da interface	
*******************************************************************************************************************************/
typedef enum {
	ITFC_CondRetOK,
		/* Concluiu corretamente */

	ITFC_CondRetInterfaceJaConfigurada,
		/* Interface j� foi configurada */

	ITFC_CondRetInterfaceNaoConfigurada,
		/* Interface ainda n�o foi configurada */

	ITFC_CondRetJanelaJaConfigurada,
		/* Janela j� foi configurada */

	ITFC_CondRetJanelaNaoConfigurada,
		/* Janela ainda n�o foi configurada */

	ITFC_CondRetSemMemoria,
		/* Falta de mem�ria */

	ITFC_CondRetErroCarregamentoImagem,
		/* N�o carregou imagem */
} ITFC_tpCondRet;

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_ConfigurarInterface
*
*	$ED Descri��o da fun��o:
*		Configura interface gr�fica. Deve ser chamada antes das outras fun��es do m�dulo.
*
*	$EP Par�metros:
*		$P pArgc	-	este argumento deve receber um ponteiro para "argc", primeiro argumento que a fun��o main recebe.
*		$P argv		-	este argumento deve receber "argv", segundo argumento que a fun��o main recebe.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceJaConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarInterface(int* pArgc, char** argv);

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_ConfigurarJanela
*
*	$ED Descri��o da fun��o:
*		Recebe dimens�es (largura e altura) em pixels e configura uma janela de interface gr�fica. S� funciona se a interface
*		j� tiver sido configurada.
*
*	$EP Par�metros:
*		$P largura	-	largura em pixels da janela.
*		$P altura	-	altura em pixels da janela.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*		ITFC_CondRetJanelaJaConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarJanela(int largura, int altura);


/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_CarregarImagens
*
*	$ED Descri��o da fun��o:
*		Carrega imagens utilizadas pela interface gr�fica no jogo.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetErroCarregamentoImagem
*		ITFC_CondRetSemMemoria
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_CarregarImagens();

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_ConfigurarLoopJanela
*
*	$ED Descri��o da fun��o:
*		Recebe tr�s fun��es. Essas fun��es ser�o executadas em loop durante a exist�ncia da janela da interface gr�fica. Chamar
*		esta fun��o (ITFC_ConfigurarLoopjanela) uma segunda vez com outros par�metros substituir� essas fun��es de loop da janela.
*		S� funciona se a interface j� tiver sido configurada.
*
*	$EP Par�metros:
*		$P inicioLoopJanela	-	fun��o que ser� executada primeiro no loop. Cont�m o c�digo que inicia uma partida do
*								jogo. Recebe e retorna void.
*		$P fimLoopJanela	-	fun��o que ser� executada por �ltimo no loop. Cont�m o c�digo que finaliza a partida do
*								jogo. Recebe e retorna void.
*		$P condicaoFimLoop	-	fun��o que questiona usu�rio se deseja jogar novamente. Em caso negativo, esta fun��o
*								deve retornar 1. Caso contr�rio ela retorna 0. Recebe void.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarLoopJanela(void (*inicioLoopJanela)(), void (*fimLoopJanela)(), int (*condicaoFimLoop)());

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_ConfigurarLoopInterface
*
*	$ED Descri��o da fun��o:
*		Recebe uma fun��o que recebe e retorna void. Essa fun��o come�ar� a ser executada em loop sempre que a fun��o
*		de inicio de loop da janela for chamada. No fim de cada execu��o desse loop, a interface gr�fica � atualizada, mostrando as
*		novas condi��es do tabuleiro e disposi��o de pe�es. Recebe tamb�m a condi��o de fim de loop, fun��o que cuidar� de quando a 
*		interface gr�fica da janela deve parar de ser atualizada e o loop parar de ser executado. Quando a interface n�o est� em
*		execu��o (antes de chamar esta fun��o ou depois da condi��o de fim de loop ser satisfeita), a janela exibe uma imagem em
*		branco e a fun��o de fim de loop da janela � executada. Chamar esta fun��o (ITFC_ConfigurarLoopPrincipal) uma segunda vez
*		com outro par�metro substituir� a fun��o de loop. S� funciona se a interface j� tiver sido configurada. 
*
*	$EP Par�metros:
*		$P loopInterface	-	fun��o que ser� executada em loop quando se desejar exibir a interface (n�o uma imagem em 
*								branco). Essa fun��o estar� em loop na dura��o da partida. Quando ela acaba, a partida acaba.
*								Recebe e retorna void.
*		$P condicaoFimLoop	-	fun��o que deve retornar 1 quando se deseja encerrar o loop da interface e 0 caso contr�rio.
*								Essa fun��o ser� chamada ao fim de cada loop. Recebe void.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarLoopInterface(void (*loopInterface)(), int (*condicaoFimLoop)());

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_IniciarJanela
*
*	$ED Descri��o da fun��o:
*		Abre uma janela que mostrar� a interface gr�fica, segundo configura��es pr�vias especificadas em ITFC_ConfigurarJanela. 
*		A partir de ent�o, a janela desviar� o controle do programa e ter� um loop pr�prio que n�o pode ser cancelado sem o
*		encerramento do programa. O loop � descrito em ITFC_ConfigurarLoopJanela e ITFC_ConfigurarLoopInterface e ocorre na seguinte
*		ordem: fun��o de in�cio de loop da janela, fun��o de loop de interface, fun��o de fim de loop de janela. A fun��o de loop
*		de interface roda algumas vezes antes de ser encerrada (enquanto durar uma partida) e desenha o jogo durante sua execu��o.
*		Ao fim da fun��o de loop da janela, caso o jogador deseje jogar novamente, a fun��o in�cio de loop da janela volta a
*		recome�ar e repete-se todo o ciclo. � por essas fun��es que o usu�rio tem controle do fluxo de execu��o e do encerramento
*		do programa. O programa poder� ser encerrado atrav�s de chamadas � fun��o exit (da biblioteca "stdlib.h") ou com o 
*		fechamento manual da janela. ATEN��O: caso n�o tenha ficado claro: ap�s esta fun��o (ITFC_IniciarJanela) ser chamada,
*		nenhum comando ap�s ela ser� executado. Iniciar a janela criar� uma janela toda em branco. A interface do jogo em si s�
*		ser� desenhada durante o loop de interface. S� funciona caso a interface e a janela j� tenham sido configuradas.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK	-	Na pr�tica OK nunca � retornado, pois se a janela � executada com sucesso, o fluxo de execu��o se altera
*		ITFC_CondRetInterfaceNaoConfigurada
*		ITFC_CondRetJanelaNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_IniciarJanela();

/*******************************************************************************************************************************
*	Fim do m�dulo de defini��o: M�dulo Interface
*******************************************************************************************************************************/