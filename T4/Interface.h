/*******************************************************************************************************************************
*	$MCD M�dulo de defini��o: M�dulo Interface
*	Arquivo:	INTERFACE.H
*	Letras identificadoras:		ITFC
*
*	Autores:	rrc - Rafael Rubim Cabral
*
*	$HA Hist�rico de evolu��o:
*		Vers�o	Autor	Data		Observa��es
*		1.00	rrc		16/12/2016	Come�o de implementa��o
*
*	$ED Descri��o do m�dulo:
*		Este m�dulo permite o desenho na tela atrav�s de uma interface gr�fica
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

	ITFC_CondRetJanelaNaoIniciada,
		/* Janela ainda n�o foi iniciada */

	ITFC_CondRetInterfaceJaIniciada
		/* Interface j� foi iniciada */
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
*	$FC Fun��o: ITFC_ConfigurarLoopJanela
*
*	$ED Descri��o da fun��o:
*		Recebe uma fun��o que recebe e retorna void. Essa fun��o ser� executada em loop durante a exist�ncia da janela da
*		interface gr�fica. Chamar esta fun��o (ITFC_ConfigurarLoopjanela) uma segunda vez com outro par�metro substituir� a
*		fun��o de loop da janela. S� funciona se a interface j� tiver sido configurada.
*
*	$EP Par�metros:
*		$P funcaoLoopPrincipal	-	fun��o que ser� executada em loop. Esta fun��o � chamada de "loop da janela" e representa o
*									loop do jogo. No final dela, o usu�rio deve ser questionado se deseja jogar novamente. Em
*									caso negativo, esta fun��o deve encerrar o programa atrav�s da fun��o "exit", da biblioteca
*									"stdlib".
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarLoopJanela(void (*funcaoLoopJanela)());

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_ConfigurarLoopInterface
*
*	$ED Descri��o da fun��o:
*		Recebe uma fun��o que recebe e retorna void. Essa fun��o come�ar� a ser executada em loop sempre que a fun��o
*		ITFC_IniciarPartida for chamada. No fim de cada execu��o desse loop, a interface gr�fica � atualizada, mostrando as novas
*		condi��es do tabuleiro e disposi��o de pe�es. Recebe tamb�m a condi��o de fim de loop, fun��o que cuidar� de quando a 
*		interface gr�fica da janela deve parar de ser atualizada e o loop parar de ser executado. Quando a interface n�o est� em
*		execu��o (antes de chamar esta fun��o ou depois da condi��o de fim de loop ser satisfeita), a janela exibe uma imagem em
*		branco. Chamar esta fun��o (ITFC_ConfigurarLoopPrincipal) uma segunda vez com outro par�metro substituir� a fun��o de loop.
*		S� funciona se a interface j� tiver sido configurada. 
*
*	$EP Par�metros:
*		$P funcaoLoopInterface	-	fun��o que ser� executada em loop quando se desejar exibir a interface (n�o uma imagem em 
*									branco).
*		$P condicaoFimLoop		-	fun��o que deve retornar 1 quando se deseja encerrar o loop da interface e 0 caso contr�rio.
*									Essa fun��o ser� chamada ao fim de cada loop.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarLoopInterface(void (*funcaoLoopInterface)(), int (*condicaoFimLoop)());

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_IniciarJanela
*
*	$ED Descri��o da fun��o:
*		Abre uma janela que mostrar� a interface gr�fica, segundo configura��es pr�vias especificadas em ITFC_ConfigurarJanela. 
*		A partir de ent�o, a janela desviar� o controle do programa e ter� um loop pr�prio que n�o pode ser cancelado sem o
*		encerramento do programa, descrito em ITFC_ConfigurarLoopJanela. � por essa fun��o que o usu�rio tem controle
*		sobre o fluxo de execu��o e sobre o encerramento do programa. O programa poder� ser encerrado atrav�s de chamadas � fun��o
*		exit (da biblioteca "stdlib.h") ou com o fechamento manual da janela. ATEN��O: caso n�o tenha ficado claro: ap�s esta
*		fun��o (ITFC_IniciarJanela) ser chamada, nenhum comando ap�s ela ser� executado. Iniciar a janela criar� uma janela toda
*		em branco. A interface do jogo em si s� ser� desenhada ap�s cada chamamento da fun��o ITFC_IniciarInterface. S� funciona
*		caso a interface e a janela j� tenham sido configuradas.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK	-	Na pr�tica OK nunca � retornado, pois se a janela � executada com sucesso, o fluxo de execu��o se altera
*		ITFC_CondRetInterfaceNaoConfigurada
*		ITFC_CondRetJanelaNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_IniciarJanela();

/*******************************************************************************************************************************
*	$FC Fun��o: ITFC_IniciarInterface
*
*	$ED Descri��o da fun��o:
*		Esta fun��o deve ser chamada ap�s o in�cio da janela da interface. Isso significa que s� funcionar� se for chamada dentro
*		do loop da janela (fun��o especificada atrav�s do chamamento de ITFC_ConfigurarLoopJanela). Esta fun��o
*		(ITFC_IniciarPartida) inicia um subloop que ocorre no loop da janela. Esse subloop � o loop da fun��o especificada no
*		chamamento de ITFC_ConfigurarLoopInterface, que se encerra sozinho quando atende � condi��o tamb�m especificada no
*		chamamento de ITFC_ConfigurarLoopInterface. Ap�s casa execu��o do loop da interface, a janela redesenha o tabuleiro do
*		jogo. Esta fun��o (ITFC_IniciarInterface) n�o deve ser chamada sea interface ainda est� em loop, mas pode ser chamado
*		mais de uma vez durante a execu��o do programa (uma vez para cada partida), ap�s cada encerramento.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*		ITFC_CondRetJanelaNaoIniciada
*		ITFC_CondRetInterfaceJaIniciada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_IniciarInterface();

/*******************************************************************************************************************************
*	Fim do m�dulo de defini��o: M�dulo Interface
*******************************************************************************************************************************/