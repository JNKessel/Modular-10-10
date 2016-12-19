/*******************************************************************************************************************************
*	$MCD Módulo de definição: Módulo No Lista
*	Arquivo:	NoLista.c
*	Letras identificadoras:		NOLST
*
*	Autores:	rrc
*			phf
*			jnk
*
*	$HA Histórico de evolução:
*		Versâo	Autor	Data		Observações
*		1.00	rrc		17/10/2016	Começo de implementação, maioria das funções ainda não implementadas
*		1.10	rrc		09/12/2016	Documentação de funções
*		2.00	rrc		13/12/2016	Implementação de outras funções
*		3.00	rrc		14/12/2016	Fim das implementações
*		4.00	rrc		15/12/2016	Adições ao módulo, correção de bugs
*		5.00	rrc		18/12/2016	Várias adições de funções a serem usadas pelo módulo ITFC
*******************************************************************************************************************************/

#include <stdlib.h>

#define NOLISTA_OWN
	#include "NoLista.h"
#undef NOLISTA_OWN

/***********************************************************************
*
*  $TC Tipo de dados: NOLST descritor da estrutura do No de lista duplamente encadeada
*
*
***********************************************************************/

typedef struct NOLST_tgNoLista {
	NOLST_tppNoLista pAnt;
	NOLST_tppNoLista pProx;
	void* pInfo;
} NOLST_tpNoLista;


/***************************************************************************
*
*  Função: NOLST  &Criar Noh
*  ****/

NOLST_tpCondRet NOLST_CriarNoh(NOLST_tppNoLista* pNohRet, void* pInfo, NOLST_tppNoLista pNohAnterior,NOLST_tppNoLista pNoProximo) {
	NOLST_tpCondRet debug;
	*pNohRet = (NOLST_tppNoLista)malloc(sizeof(NOLST_tpNoLista));
    if (pNohRet == NULL){
        return NOLST_CondRetSemMemoria;
    }//if
	debug = NOLST_LigarNos(pNohAnterior, *pNohRet);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	debug = NOLST_LigarNos(*pNohRet, pNoProximo);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	(*pNohRet)->pInfo = pInfo;
	return NOLST_CondRetOK;
} /* Fim Funcao: NOLST &Criar Noh */


/***************************************************************************
*
*  Função: NOLST  &Destruir Noh
*  ****/

NOLST_tpCondRet  NOLST_DestruirNoh(NOLST_tppNoLista pNoLst) {
	NOLST_tpCondRet debug;
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	debug = NOLST_LigarNos(pNoLst->pAnt, pNoLst->pProx);
    if (debug){
        return debug;
    }//if
	free(pNoLst); // Implementacao nao assume erro em free
	return NOLST_CondRetOK;
} /* Fim Funcao: NOLST &Destruir Noh  */


/***************************************************************************
*
*  Função: NOLST  &Colocar Noh Em Frente
*  ****/

NOLST_tpCondRet  NOLST_ColocarNohEmFrente(NOLST_tppNoLista pNoLst, NOLST_tppNoLista pNovoNo) {
	NOLST_tpCondRet debug;
	NOLST_tppNoLista velhoProx;
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	velhoProx = pNoLst->pProx;
	if (pNovoNo->pAnt
        || pNovoNo->pProx){
        return NOLST_CondRetNoOcupado;
    }//if
	debug = NOLST_LigarNos(pNoLst, pNovoNo);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	debug = NOLST_LigarNos(pNovoNo, velhoProx);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	return NOLST_CondRetOK;
} /* Fim Funcao: NOLST &Colocar Noh Em Frente */


/***************************************************************************
*
*  Função: NOLST  &Colocar Noh Atras
*  ****/

NOLST_tpCondRet  NOLST_ColocarNohAtras(NOLST_tppNoLista pNoLst, NOLST_tppNoLista pNovoNo) {
	NOLST_tpCondRet debug;
	NOLST_tppNoLista velhoAnt;
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	velhoAnt = pNoLst->pAnt;
	if (pNovoNo->pAnt
        || pNovoNo->pProx){
        return NOLST_CondRetNoOcupado;
    }//if
	debug = NOLST_LigarNos(pNovoNo, pNoLst);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	debug = NOLST_LigarNos(velhoAnt, pNovoNo);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	return NOLST_CondRetOK;
} /* Fim funcao: NOLST &Colocar Noh Atras */


/***************************************************************************
*
*  Função: NOLST  &Obter Prox Noh
*  ****/

NOLST_tpCondRet  NOLST_ObterProxNoh(NOLST_tppNoLista pNoLst, NOLST_tppNoLista* pNoProxRet) {
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	*pNoProxRet = pNoLst->pProx;
	return NOLST_CondRetOK;
} /* Fim Funcao: NOLST &Obter Prox Noh */


/***************************************************************************
*
*  Função: NOLST  &Obter Noh Previo
*  ****/

NOLST_tpCondRet  NOLST_ObterNohPrevio(NOLST_tppNoLista pNoLst, NOLST_tppNoLista* pNoPrevioRet) {
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	*pNoPrevioRet = pNoLst->pAnt;
	return NOLST_CondRetOK;
} /* Fim Funcao: NOLST &Obter Noh Previo */


/***************************************************************************
*
*  Função: NOLST  &Atribuir Info Noh
*  ****/

NOLST_tpCondRet  NOLST_AtribuirInfoNoh(NOLST_tppNoLista pNoLst, void* pInfo) {
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	pNoLst->pInfo = pInfo;
	return NOLST_CondRetOK;
} /* Fim Funcao: NOLST &Atribuir Info Noh */


/***************************************************************************
*
*  Função: NOLST  &Obter Info Noh
*  ****/

NOLST_tpCondRet  NOLST_ObterInfoNoh(NOLST_tppNoLista pNoLst, void** pInfoRet) {
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	*pInfoRet = pNoLst->pInfo;
	return NOLST_CondRetOK;
}/* Fim funcao: NOLST &Obter Info Noh */


/***************************************************************************
*
*  Função: NOLST  &Ligar Nos
*  ****/

NOLST_tpCondRet NOLST_LigarNos(NOLST_tppNoLista pEsq, NOLST_tppNoLista pDir) {
    if (pEsq){
		pEsq->pProx = pDir;
    }//if
    if (pDir){
		pDir->pAnt = pEsq;
    }//if
	return NOLST_CondRetOK;
} /* Fim funcao: NOLST &Ligar Nos */

/********** Fim do módulo de implementação: NOLST No de lista duplamente encadeada **********/

