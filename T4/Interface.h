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

} ITFC_tpCondRet;

/*******************************************************************************************************************************
*	$FC Fun��o: PART_CriarPartida
*
*	$ED Descri��o da fun��o:
*		Recebe quantos jogadores participar�o da partida e a cria. O m�nimo de jogadores � 2 e o m�ximo � 4. A cor de cada
*		jogador obedece sempre a seguinte ordem: 1�- AZUL, 2�- VERMELHO, 3�- VERDE, 4�- AMARELO.
*
*	$EP Par�metros:
*		$P iNumJogadores	-	n�mero de jogadores participantes
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
*	Fim do m�dulo de defini��o: M�dulo Interface
*******************************************************************************************************************************/