/*******************************************************************************************************************************
*	$MCD Módulo de definição: Módulo Interface
*	Arquivo:	INTERFACE.H
*	Letras identificadoras:		ITFC
*
*	Autores:	rrc - Rafael Rubim Cabral
*
*	$HA Histórico de evolução:
*		Versâo	Autor	Data		Observações
*		1.00	rrc		16/12/2016	Começo de implementação, implementação da maioria das funções do módulo
*		3.00	rrc		18/12/2016	Fim da implementação de módulo
*
*	$ED Descrição do módulo:
*		Este módulo permite o desenho na tela do jogo de ludo através de uma interface gráfica. Iniciar uma janela para imprimir
*		a interface gráfica toma posse do fluxo de execução do programa, o que significa que nenhuma função chamada após
*		a função ITFC_IniciarJanela funcionará, e ela não retornará. Para que o programador ainda tenha controle sobre o que
*		ocorre no fluxo de execução após a janela da interface ser executada, pode-se implementar funções próprias que serão
*		passadas antes para as funções ITFC_ConfigurarLoopJanela e ITFC_ConfigurarLoopInterface. Isso fará com que tais funções
*		próprias sejam executadas em loop (em ordem predeterminada) durante a existência da janela.
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

	ITFC_CondRetSemMemoria,
		/* Falta de memória */

	ITFC_CondRetErroCarregamentoImagem,
		/* Não carregou imagem */
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
*	$FC Função: ITFC_CarregarImagens
*
*	$ED Descrição da função:
*		Carrega imagens utilizadas pela interface gráfica no jogo.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetErroCarregamentoImagem
*		ITFC_CondRetSemMemoria
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_CarregarImagens();

/*******************************************************************************************************************************
*	$FC Função: ITFC_ConfigurarLoopJanela
*
*	$ED Descrição da função:
*		Recebe três funções. Essas funções serão executadas em loop durante a existência da janela da interface gráfica. Chamar
*		esta função (ITFC_ConfigurarLoopjanela) uma segunda vez com outros parâmetros substituirá essas funções de loop da janela.
*		Só funciona se a interface já tiver sido configurada.
*
*	$EP Parâmetros:
*		$P inicioLoopJanela	-	função que será executada primeiro no loop. Contém o código que inicia uma partida do
*								jogo. Recebe e retorna void.
*		$P fimLoopJanela	-	função que será executada por último no loop. Contém o código que finaliza a partida do
*								jogo. Recebe e retorna void.
*		$P condicaoFimLoop	-	função que questiona usuário se deseja jogar novamente. Em caso negativo, esta função
*								deve retornar 1. Caso contrário ela retorna 0. Recebe void.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarLoopJanela(void (*inicioLoopJanela)(), void (*fimLoopJanela)(), int (*condicaoFimLoop)());

/*******************************************************************************************************************************
*	$FC Função: ITFC_ConfigurarLoopInterface
*
*	$ED Descrição da função:
*		Recebe uma função que recebe e retorna void. Essa função começará a ser executada em loop sempre que a função
*		de inicio de loop da janela for chamada. No fim de cada execução desse loop, a interface gráfica é atualizada, mostrando as
*		novas condições do tabuleiro e disposição de peões. Recebe também a condição de fim de loop, função que cuidará de quando a 
*		interface gráfica da janela deve parar de ser atualizada e o loop parar de ser executado. Quando a interface não está em
*		execução (antes de chamar esta função ou depois da condição de fim de loop ser satisfeita), a janela exibe uma imagem em
*		branco e a função de fim de loop da janela é executada. Chamar esta função (ITFC_ConfigurarLoopPrincipal) uma segunda vez
*		com outro parâmetro substituirá a função de loop. Só funciona se a interface já tiver sido configurada. 
*
*	$EP Parâmetros:
*		$P loopInterface	-	função que será executada em loop quando se desejar exibir a interface (não uma imagem em 
*								branco). Essa função estará em loop na duração da partida. Quando ela acaba, a partida acaba.
*								Recebe e retorna void.
*		$P condicaoFimLoop	-	função que deve retornar 1 quando se deseja encerrar o loop da interface e 0 caso contrário.
*								Essa função será chamada ao fim de cada loop. Recebe void.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK
*		ITFC_CondRetInterfaceNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_ConfigurarLoopInterface(void (*loopInterface)(), int (*condicaoFimLoop)());

/*******************************************************************************************************************************
*	$FC Função: ITFC_IniciarJanela
*
*	$ED Descrição da função:
*		Abre uma janela que mostrará a interface gráfica, segundo configurações prévias especificadas em ITFC_ConfigurarJanela. 
*		A partir de então, a janela desviará o controle do programa e terá um loop próprio que não pode ser cancelado sem o
*		encerramento do programa. O loop é descrito em ITFC_ConfigurarLoopJanela e ITFC_ConfigurarLoopInterface e ocorre na seguinte
*		ordem: função de início de loop da janela, função de loop de interface, função de fim de loop de janela. A função de loop
*		de interface roda algumas vezes antes de ser encerrada (enquanto durar uma partida) e desenha o jogo durante sua execução.
*		Ao fim da função de loop da janela, caso o jogador deseje jogar novamente, a função início de loop da janela volta a
*		recomeçar e repete-se todo o ciclo. É por essas funções que o usuário tem controle do fluxo de execução e do encerramento
*		do programa. O programa poderá ser encerrado através de chamadas à função exit (da biblioteca "stdlib.h") ou com o 
*		fechamento manual da janela. ATENÇÃO: caso não tenha ficado claro: após esta função (ITFC_IniciarJanela) ser chamada,
*		nenhum comando após ela será executado. Iniciar a janela criará uma janela toda em branco. A interface do jogo em si só
*		será desenhada durante o loop de interface. Só funciona caso a interface e a janela já tenham sido configuradas.
*
*	$FV Valor retornado:
*		ITFC_CondRetOK	-	Na prática OK nunca é retornado, pois se a janela é executada com sucesso, o fluxo de execução se altera
*		ITFC_CondRetInterfaceNaoConfigurada
*		ITFC_CondRetJanelaNaoConfigurada
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_IniciarJanela();

/*******************************************************************************************************************************
*	Fim do módulo de definição: Módulo Interface
*******************************************************************************************************************************/