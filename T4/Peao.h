/*******************************************************
* $MCD Modulo de definiçao: Modulo Peao
* Arquivo:   Peao.h
*
* Autores: jnk - Julio Neuman Kessel
*          rrc - Rafael Rubim Cabral
*          phf - Pedro Henrique Frayha
*******************************************************/

#pragma once

#if defined( PEAO_OWN )
   #define PEAO_EXT
#else
   #define PEAO_EXT extern
#endif

#include "Definicoes.h"
#include "Tabuleiro.h"

typedef struct PEAO_tgPeao* PEAO_tppPeao;

typedef enum {
    
	PEAO_CondRetOK				= 0,
	PEAO_CondRetSemMemoria		= 1,
	PEAO_CondRetNumeroInvalido	= 2,
	PEAO_CondRetPeaoInexistente = 3,
	PEAO_CondRetPeaoEstaBase	= 4,
	PEAO_CondRetPeaoNaoEstaBase = 5,
	PEAO_CondRetErroTabuleiro	= 6,
	PEAO_CondRetInconsistencia	= 7

} PEAO_tpCondRet;

/***********************************************************************
* $FC Funçao: PEAO_CriarPeao
* 
* $ED Descriçao da funçao:
* Cria um novo peao na variavel passada e inicializa seus valores e o
* posiciona na Base de sua cor
*
* $FV Valores de Retorno:
*     PEAO_CondRetSemMemoria - Falta de Memoria
*     PEAO_CondRetNumeroInvalido - Numero de peao invalido
*     PEAO_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
PEAO_tpCondRet PEAO_CriarPeao(PEAO_tppPeao* pPeao, DEF_tpCor Cor, int iNum);


/***********************************************************************
* $FC Funçao: PEAO_AndarPeao
* 
* $ED Descriçao da funçao:
* Avança um numero n de casa ao longo do tabuleiro com o peao pPeao.
* Dá erro se peão estiver na base.
*
* $FV Valores de Retorno:
*     PEAO_CondRetPeaoInexistente - Peao nao existe
*	PEAO_CondRetOK
*	PEAO_CondRetPeaoEstaBase
***********************************************************************/
PEAO_tpCondRet PEAO_AndarPeao(PEAO_tppPeao pPeao, int n);
/*Esta funcao esta associada com a implementacao da dinamica da partida
  Não implementada pois foge ao escopo do T2*/

/***********************************************************************
* $FC Funçao: PEAO_SairBasePeao
* 
* $ED Descriçao da funçao:
* Recebe um peao e muda a posiçao dele para a casa de saida da base dos
* peoes da cor dele. Dá erro se peão não estiver na base.
*
* $FV Valores de Retorno:
*     PEAO_CondRetPeaoInexistente - Peao nao existe
*     PEAO_CondRetOK - Operaçao realizada sem problemas
*     PEAO_CondRetPeaoNaoEstaBase
***********************************************************************/
PEAO_tpCondRet PEAO_SairBasePeao(PEAO_tppPeao pPeao);

/***********************************************************************
* $FC Funçao: PEAO_EstaPeaoFinal
* 
* $ED Descriçao da funçao:
* Recebe um peao e retorna na variavel de retorno se ele esta 
* numa casa de chegada ou não
*
* $FV Valores de Retorno:
*     PEAO_CondRetPeaoInexistente - Peao nao existe
*     PEAO_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
PEAO_tpCondRet PEAO_EstaPeaoFinal(PEAO_tppPeao pPeao, DEF_tpBool* BoolRet);

/***********************************************************************
* $FC Funçao: PEAO_EstaPeaoBase
* 
* $ED Descriçao da funçao:
* Recebe um peao e retorna na variavel de retorno se ele
* esta na base ou não
*
* $FV Valores de Retorno:
*     PEAO_CondRetPeaoInexistente - Peao nao existe
*     PEAO_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
PEAO_tpCondRet PEAO_EstaPeaoBase(PEAO_tppPeao pPeao, DEF_tpBool* BoolRet);
 
/***********************************************************************
* $FC Funçao: PEAO_ObterNumeroPeao
* 
* $ED Descriçao da funçao:
* Recebe um peao e retorna na variavelde retorno o numero dele
*
* $FV Valores de Retorno:
*     PEAO_CondRetPeaoInexistente - Peao nao existe
*     PEAO_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
PEAO_tpCondRet PEAO_ObterNumeroPeao(PEAO_tppPeao pPeao, int* NumRet);
 
/***********************************************************************
* $FC Funçao: PEAO_ObterCasaPeao
* 
* $ED Descriçao da funçao:
* Recebe um peao e retorna na variavelde casaRetorno um ponteiro para a casa na qual o peao se encontra.
* Retorna na variável NULL caso o peão esteja na base
*
* $FV Valores de Retorno:
*     PEAO_CondRetPeaoInexistente - Peao nao existe
*     PEAO_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/

PEAO_tpCondRet PEAO_ObterCasaPeao(PEAO_tppPeao pPeao, TAB_tppCasa* casaRetorno);

/***********************************************************************
* $FC Funçao: PEAO_ChecarMovimentoDisponivelPeao
* 
* $ED Descriçao da funçao:
* Recebe um peao e o numero de casas que ele quer se movimentar,
* se ele puder se movimentar é retornando True em BoolRet, caso contrario
* é retornado False. Se o peao puder avançar o numero de 
* casas recebido mas ela ja estiver ocupada por
* um peao de outra cor, é retornada a cor desse peao em CorRet. A função
* leva em conta se o peão está na base (movimento disponível se o número do
* dado for 6 e não houver um peão da mesma cor na saída).
*
* $FV Valores de Retorno:
*	PEAO_CondRetPeaoInexistente - Peao nao existe
*	PEAO_CondRetOK - Operaçao realizada sem problemas
*	PEAO_CondRetInconsistencia
*	PEAO_CondRetErroTabuleiro
***********************************************************************/
PEAO_tpCondRet PEAO_ChecarMovimentoDisponivelPeao(PEAO_tppPeao pPeao, int dado, DEF_tpBool* BoolRet, DEF_tpCor* CorRet);

/***********************************************************************
* $FC Funçao: PEAO_VoltarBasePeao
* 
* $ED Descriçao da funçao:
* Recebe um peao e faz com que ele volte para a base de sua cor
*
* $FV Valores de Retorno:
*	PEAO_CondRetPeaoInexistente - Peao nao existe
*	PEAO_CondRetOK - Operaçao realizada sem problemas
*	PEAO_CondRetPeaoNaoEstaBase
*	PEAO_CondRetErroTabuleiro
***********************************************************************/
PEAO_tpCondRet PEAO_VoltarBasePeao(PEAO_tppPeao pPeao);

/***********************************************************************
* $FC Funçao: PEAO_DestruirPeao
* 
* $ED Descriçao da funçao:
* Recebe um peao e libera ele da memoria
*
* $FV Valores de Retorno:
*     PEAO_CondRetPeaoInexistente - Peao nao existe
*     PEAO_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
PEAO_tpCondRet PEAO_DestruirPeao(PEAO_tppPeao pPeao);
