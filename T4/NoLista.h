/*******************************************************
* $MCD Modulo de definiçao: Modulo NoLista
* Arquivo:   NoLista.h
*
* Autores: rrc - Rafael Rubim Cabral
*
* Editado: jnk - Julio Neuman Kessel 19/out/2016
*******************************************************/

#pragma once

#if defined( NOLISTA_OWN )
   #define NOLISTA_EXT
#else
   #define NOLISTA_EXT extern
#endif

typedef enum {
	NOLST_CondRetOK = 0,
	NOLST_CondRetErro,
	NOLST_CondRetSemMemoria,
	NOLST_CondRetNoOcupado,
	NOLST_CondRetNoInexistente
} NOLST_tpCondRet;

typedef struct NOLST_tgNoLista* NOLST_tppNoLista;


/***********************************************************************
* $FC Funçao: NOLST_CriarNoh
* 
* $ED Descriçao da funçao:
* Cria um novo nó e o posiciona entre pNohAnterior e pNoProximo. 
* O nó criado é inicializado com
* a informação passada para a função e é retornado em pNohRet
*
* $FV Valores de Retorno:
*     NOLST_CondRetSemMemoria - Falta de Memoria
*     NOLST_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
NOLST_tpCondRet NOLST_CriarNoh(NOLST_tppNoLista* pNohRet, void* pInfo, NOLST_tppNoLista pNohAnterior, NOLST_tppNoLista pNoProximo);


/***********************************************************************
* $FC Funçao: NOLST_DestruirNoh
* 
* $ED Descriçao da funçao:
* Libera um nó da memoria
*
* $FV Valores de Retorno:
*     NOLST_CondRetNoInexistente - Nó nao existe
*     NOLST_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
NOLST_tpCondRet NOLST_DestruirNoh(NOLST_tppNoLista pNoLst);


/***********************************************************************
* $FC Funçao: NOLST_ColocarNohEmFrente
* 
* $ED Descriçao da funçao:
* Recebe dois nós e coloca pNovoNo na posição seguinte de pNoLst
*
* $FV Valores de Retorno:
*     NOLST_CondRetNoInexistente - Nó nao existe
*     NOLST_CondRetNoOcupado - pNovoNo ja faz parte de outra lista
*     NOLST_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
NOLST_tpCondRet NOLST_ColocarNohEmFrente(NOLST_tppNoLista pNoLst, NOLST_tppNoLista pNovoNo);


/***********************************************************************
* $FC Funçao: NOLST_ColocarNohAtras
* 
* $ED Descriçao da funçao:
* Recebe dois nós e coloca pNovoNo na posição anterior a pNoLst
*
* $FV Valores de Retorno:
*     NOLST_CondRetNoInexistente - Nó nao existe
*     NOLST_CondRetNoOcupado - pNovoNo ja faz parte de outra lista
*     NOLST_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
NOLST_tpCondRet NOLST_ColocarNohAtras(NOLST_tppNoLista pNoLst, NOLST_tppNoLista pNovoNo);


/***********************************************************************
* $FC Funçao: NOLST_ObterProxNoh
* 
* $ED Descriçao da funçao:
* Recebe um nó e retorna o nó seguinte a ele em pNoProxRet
*
* $FV Valores de Retorno:
*     NOLST_CondRetNoInexistente - Nó nao existe
*     NOLST_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
NOLST_tpCondRet NOLST_ObterProxNoh(NOLST_tppNoLista pNoLst, NOLST_tppNoLista* pNoProxRet);


/***********************************************************************
* $FC Funçao: NOLST_ObterNohPrevio
* 
* $ED Descriçao da funçao:
* Recebe um nó e retorna o nó antecessor a ele em pNoPrevioRet
*
* $FV Valores de Retorno:
*     NOLST_CondRetNoInexistente - Nó nao existe
*     NOLST_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
NOLST_tpCondRet NOLST_ObterNohPrevio(NOLST_tppNoLista pNoLst, NOLST_tppNoLista* pNoPrevioRet);


/***********************************************************************
* $FC Funçao: NOLST_AtribuirInfoNoh
* 
* $ED Descriçao da funçao:
* Recebe um nó e uma informação e atribui ela ao nó
*
* $FV Valores de Retorno:
*     NOLST_CondRetNoInexistente - Nó nao existe
*     NOLST_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
NOLST_tpCondRet NOLST_AtribuirInfoNoh(NOLST_tppNoLista pNoLst, void* pInfo);


/***********************************************************************
* $FC Funçao: NOLST_ObterInfoNoh
* 
* $ED Descriçao da funçao:
* Recebe um nó e retorna sua informação em pInfoRet
*
* $FV Valores de Retorno:
*     NOLST_CondRetNoInexistente - Nó nao existe
*     NOLST_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
NOLST_tpCondRet NOLST_ObterInfoNoh(NOLST_tppNoLista pNoLst, void** pInfoRet);

NOLST_tpCondRet NOLST_LigarNos(NOLST_tppNoLista pEsq, NOLST_tppNoLista pDir);











