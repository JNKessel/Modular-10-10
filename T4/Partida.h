/*******************************************************************************************************************************
*	$MCD Módulo de definição: Módulo Partida
*	Arquivo:	PARTIDA.H
*
*	Autores:	rrc - Rafael Rubim Cabral
*
*	$HA Histórico de evolução:
*		Versâo	Autor	Data		Observações
*		1.00	rrc		17/10/2016	Começo de implementação, maioria das funções ainda não implementadas
*		2.00	rrc		12/09/2016	Dodumentação e continuação em implementação de funções
*
*	$ED Descrição do módulo:
*		Este módulo permite a criação de uma partida a partir de sua quantidade de jogadores e manipulação de cada turno.
*		Não pode ser criada mais de uma partida simultaneamente.
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
PART_tpCondRet PART_CriarPartida(int iNumJogadores);

PART_tpCondRet PART_Jogar();

/*******************************************************************************************************************************
*	$FC Função: PART_ChecarVitoria
*
*	$ED Descrição da função:
*		Retorna se o jogador que jogou o último turno venceu o jogo e em caso positivo, retorna também sua cor.
*
*	$EP Parâmetros:
*		$P boolRet			-	parâmetro que receberá o retorno da função: True caso o jogador tenha vencido, False caso contrário.
*								Este parâmetro é passado como referência.
*		$P CorVencedorRet	-	parâmetro que receberá o a cor do jogador que venceu o jogo.
*								Este parâmetro é passado como referência.
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
