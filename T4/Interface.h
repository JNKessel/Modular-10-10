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

} ITFC_tpCondRet;

/*******************************************************************************************************************************
*	$FC Função: PART_CriarPartida
*
*	$ED Descrição da função:
*		Recebe quantos jogadores participarão da partida e a cria. O mínimo de jogadores é 2 e o máximo é 4. A cor de cada
*		jogador obedece sempre a seguinte ordem: 1º- AZUL, 2º- VERMELHO, 3º- VERDE, 4º- AMARELO.
*
*	$EP Parâmetros:
*		$P iNumJogadores	-	número de jogadores participantes
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetSemMemoria
*		PART_CondRetNumInvalido
*		PART_CondRetErroListaC
*		PART_CondRetErroLista
*		PART_CondRetErroPeao
*******************************************************************************************************************************/
ITFC_tpCondRet ITFC_CriarJanela();

/*******************************************************************************************************************************
*	Fim do módulo de definição: Módulo Interface
*******************************************************************************************************************************/