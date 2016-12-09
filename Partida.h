/*******************************************************************************************************************************
*	$MCD M�dulo de defini��o: M�dulo Partida
*	Arquivo:	PARTIDA.H
*
*	Autores:	rrc - Rafael Rubim Cabral
*
*	$HA Hist�rico de evolu��o:
*		Vers�o	Autor	Data		Observa��es
*		1.00	rrc		17/10/2016	Come�o de implementa��o, maioria das fun��es ainda n�o implementadas
*		2.00	rrc		12/09/2016	Dodumenta��o e continua��o em implementa��o de fun��es
*
*	$ED Descri��o do m�dulo:
*		Este m�dulo permite a cria��o de uma partida a partir de sua quantidade de jogadores e manipula��o de cada turno.
*		N�o pode ser criada mais de uma partida simultaneamente.
*******************************************************************************************************************************/

#pragma once

#include "Definicoes.h"
#include "Peao.h"
#include "ListaC.h"
#include "Lista.h"

typedef enum eEstado {
    Base,
    Tabuleiro,
    Final
} Estado;

typedef enum {
	PART_CondRetOK,
	PART_CondRetSemMemoria,
	PART_CondRetErroListaC,
	PART_CondRetErroPeao,
	PART_CondRetErroLista,
	PART_CondRetNumInvalido,
	PART_CondRetPartidaInexistente
} PART_tpCondRet;

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
PART_tpCondRet PART_CriarPartida(int iNumJogadores);

PART_tpCondRet PART_Jogar();

/*******************************************************************************************************************************
*	$FC Fun��o: PART_ChecarVitoria
*
*	$ED Descri��o da fun��o:
*		Retorna se o jogador que jogou o �ltimo turno venceu o jogo e em caso positivo, retorna tamb�m sua cor.
*
*	$EP Par�metros:
*		$P boolRet			-	par�metro que receber� o retorno da fun��o: True caso o jogador tenha vencido, False caso contr�rio.
*								Este par�metro � passado como refer�ncia.
*		$P CorVencedorRet	-	par�metro que receber� o a cor do jogador que venceu o jogo.
*								Este par�metro � passado como refer�ncia.
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetErroLista
*		PART_CondRetErroListaC
*		PART_CondRetErroPeao
*******************************************************************************************************************************/
PART_tpCondRet PART_ChecarVitoria(DEF_tpBool* BoolRet, DEF_tpCor* CorVencedorRet);

PART_tpCondRet PART_ChecarPeoesDesponiveis(PART_tpJogador* jogadorVez, int dado, LIS_tppLista* PeoesDisponiveisRet);

PART_tpCondRet PART_Escolher(LIS_tppLista* PeoesDisponiveis, PEAO_tppPeao PeaoEscolhidoRet);

PART_tpCondRet PART_DestruirPartida();