/*******************************************************************************************************************************
*	$MCD Módulo de definição: Módulo Interface
*	Arquivo:	INTERFACE.H
*	Letras identificadoras:		ITFC
*
*	Autores:	rrc - Rafael Rubim Cabral
*
*	$HA Histórico de evolução:
*		Versâo	Autor	Data		Observações
*		1.00	rrc		16/12/2016	Começo de implementação
*
*	$ED Descrição do módulo:
*		Este módulo permite o desenho na tela através de uma interface gráfica
*******************************************************************************************************************************/

#pragma once

/*******************************************************************************************************************************
*	$TC Tipo de dados: ITFC Condições de retorno
*
*	$ED Descrição do tipo:
*		Condições de retorno das funções da interface	
*******************************************************************************************************************************/
typedef enum {
	ITFC_CondRetOK,
		/* Concluiu corretamente */

	ITFC_CondRetInterfaceJaConfigurada,
		/* Interface já foi configurada */

	ITFC_CondRetInterfaceNaoConfigurada,
		/* Interface ainda não foi configurada */

	ITFC_CondRetJanelaJaConfigurada,
		/* Janela já foi configurada */

	ITFC_CondRetJanelaNaoConfigurada,
		/* Janela ainda não foi configurada */

	ITFC_CondRetJanelaNaoIniciada,
		/* Janela ainda não foi iniciada */

	ITFC_CondRetInterfaceJaIniciada
		/* Interface já foi iniciada */
} ITFC_tpCondRet;

/*******************************************************************************************************************************
*	$FC Função: ITFC_ConfigurarInterface
*
*	$ED Descrição da função:
*		Configura interface gráfica. Deve ser chamada antes das outras funções do módulo.
*
*	$EP Parâmetros:
*		$P pArgc	-	este argumento deve receber um ponteiro para "argc", primeiro argumento que a função main recebe.
*		$P argv		-	este argumento deve receber "argv", segundo argumento que a função main recebe.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceJaConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarInterface(int* pArgc, char** argv);

/*******************************************************************************************************************************
*	$FC Função: ITFC_ConfigurarJanela
*
*	$ED Descrição da função:
*		Recebe dimensões (largura e altura) em pixels e configura uma janela de interface gráfica. Só funciona se a interface
*		já tiver sido configurada.
*
*	$EP Parâmetros:
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
*	$FC Função: ITFC_ConfigurarLoopJanela
*
*	$ED Descrição da função:
*		Recebe uma função que recebe e retorna void. Essa função será executada em loop durante a existência da janela da
*		interface gráfica. Chamar esta função (ITFC_ConfigurarLoopjanela) uma segunda vez com outro parâmetro substituirá a
*		função de loop da janela. Só funciona se a interface já tiver sido configurada.
*
*	$EP Parâmetros:
*		$P funcaoLoopPrincipal	-	função que será executada em loop. Esta função é chamada de "loop da janela" e representa o
*									loop do jogo. No final dela, o usuário deve ser questionado se deseja jogar novamente. Em
*									caso negativo, esta função deve encerrar o programa através da função "exit", da biblioteca
*									"stdlib".
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarLoopJanela(void (*funcaoLoopJanela)());

/*******************************************************************************************************************************
*	$FC Função: ITFC_ConfigurarLoopInterface
*
*	$ED Descrição da função:
*		Recebe uma função que recebe e retorna void. Essa função começará a ser executada em loop sempre que a função
*		ITFC_IniciarPartida for chamada. No fim de cada execução desse loop, a interface gráfica é atualizada, mostrando as novas
*		condições do tabuleiro e disposição de peões. Recebe também a condição de fim de loop, função que cuidará de quando a 
*		interface gráfica da janela deve parar de ser atualizada e o loop parar de ser executado. Quando a interface não está em
*		execução (antes de chamar esta função ou depois da condição de fim de loop ser satisfeita), a janela exibe uma imagem em
*		branco. Chamar esta função (ITFC_ConfigurarLoopPrincipal) uma segunda vez com outro parâmetro substituirá a função de loop.
*		Só funciona se a interface já tiver sido configurada. 
*
*	$EP Parâmetros:
*		$P funcaoLoopInterface	-	função que será executada em loop quando se desejar exibir a interface (não uma imagem em 
*									branco).
*		$P condicaoFimLoop		-	função que deve retornar 1 quando se deseja encerrar o loop da interface e 0 caso contrário.
*									Essa função será chamada ao fim de cada loop.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarLoopInterface(void (*funcaoLoopInterface)(), int (*condicaoFimLoop)());

/*******************************************************************************************************************************
*	$FC Função: ITFC_IniciarJanela
*
*	$ED Descrição da função:
*		Abre uma janela que mostrará a interface gráfica, segundo configurações prévias especificadas em ITFC_ConfigurarJanela. 
*		A partir de então, a janela desviará o controle do programa e terá um loop próprio que não pode ser cancelado sem o
*		encerramento do programa, descrito em ITFC_ConfigurarLoopJanela. É por essa função que o usuário tem controle
*		sobre o fluxo de execução e sobre o encerramento do programa. O programa poderá ser encerrado através de chamadas à função
*		exit (da biblioteca "stdlib.h") ou com o fechamento manual da janela. ATENÇÃO: caso não tenha ficado claro: após esta
*		função (ITFC_IniciarJanela) ser chamada, nenhum comando após ela será executado. Iniciar a janela criará uma janela toda
*		em branco. A interface do jogo em si só será desenhada após cada chamamento da função ITFC_IniciarInterface. Só funciona
*		caso a interface e a janela já tenham sido configuradas.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK	-	Na prática OK nunca é retornado, pois se a janela é executada com sucesso, o fluxo de execução se altera
*		ITFC_CondRetInterfaceNaoConfigurada
*		ITFC_CondRetJanelaNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_IniciarJanela();

/*******************************************************************************************************************************
*	$FC Função: ITFC_IniciarInterface
*
*	$ED Descrição da função:
*		Esta função deve ser chamada após o início da janela da interface. Isso significa que só funcionará se for chamada dentro
*		do loop da janela (função especificada através do chamamento de ITFC_ConfigurarLoopJanela). Esta função
*		(ITFC_IniciarPartida) inicia um subloop que ocorre no loop da janela. Esse subloop é o loop da função especificada no
*		chamamento de ITFC_ConfigurarLoopInterface, que se encerra sozinho quando atende à condição também especificada no
*		chamamento de ITFC_ConfigurarLoopInterface. Após casa execução do loop da interface, a janela redesenha o tabuleiro do
*		jogo. Esta função (ITFC_IniciarInterface) não deve ser chamada sea interface ainda está em loop, mas pode ser chamado
*		mais de uma vez durante a execução do programa (uma vez para cada partida), após cada encerramento.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*		ITFC_CondRetJanelaNaoIniciada
*		ITFC_CondRetInterfaceJaIniciada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_IniciarInterface();

/*******************************************************************************************************************************
*	Fim do módulo de definição: Módulo Interface
*******************************************************************************************************************************/