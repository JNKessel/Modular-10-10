#include "NoLista.h"

#include <stdlib.h>

typedef struct NOLST_tgNoLista {
	NOLST_tppNoLista pAnt;
	NOLST_tppNoLista pProx;
	void* pInfo;
} NOLST_tpNoLista;

static NOLST_tpCondRet Ligar(NOLST_tppNoLista pEsq, NOLST_tppNoLista pDir);

NOLST_tpCondRet NOLST_CriarNoh(NOLST_tppNoLista* pNohRet, void* pInfo, NOLST_tppNoLista pNohAnterior,NOLST_tppNoLista pNoProximo) {
	NOLST_tpCondRet debug;
	*pNohRet = (NOLST_tppNoLista)malloc(sizeof(NOLST_tpNoLista));
    if (pNohRet == NULL){
        return NOLST_CondRetSemMemoria;
    }//if
	debug = Ligar(pNohAnterior, *pNohRet);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	debug = Ligar(*pNohRet, pNoProximo);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	(*pNohRet)->pInfo = pInfo;
	return NOLST_CondRetOK;
}

NOLST_tpCondRet  NOLST_DestruirNoh(NOLST_tppNoLista pNoLst) {
	NOLST_tpCondRet debug;
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	debug = Ligar(pNoLst->pAnt, pNoLst->pProx);
    if (debug){
        return debug;
    }//if
	free(pNoLst); // Implementacao nao assume erro em free
	return NOLST_CondRetOK;
}

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
	debug = Ligar(pNoLst, pNovoNo);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	debug = Ligar(pNovoNo, velhoProx);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	return NOLST_CondRetOK;
}

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
	debug = Ligar(pNovoNo, pNoLst);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	debug = Ligar(velhoAnt, pNovoNo);
    if (debug){
        return debug; // Se Ligar nao retornou OK
    }//if
	return NOLST_CondRetOK;
}

NOLST_tpCondRet  NOLST_ObterProxNoh(NOLST_tppNoLista pNoLst, NOLST_tppNoLista* pNoProxRet) {
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	*pNoProxRet = pNoLst->pProx;
	return NOLST_CondRetOK;
}

NOLST_tpCondRet  NOLST_ObterNohPrevio(NOLST_tppNoLista pNoLst, NOLST_tppNoLista* pNoPrevioRet) {
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	*pNoPrevioRet = pNoLst->pAnt;
	return NOLST_CondRetOK;
}

NOLST_tpCondRet  NOLST_AtribuirInfoNoh(NOLST_tppNoLista pNoLst, void* pInfo) {
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	pNoLst->pInfo = pInfo;
	return NOLST_CondRetOK;
}

NOLST_tpCondRet  NOLST_ObterInfoNoh(NOLST_tppNoLista pNoLst, void** pInfoRet) {
    if (pNoLst == NULL){
        return NOLST_CondRetNoInexistente;
    }//if
	*pInfoRet = pNoLst->pInfo;
	return NOLST_CondRetOK;
}

NOLST_tpCondRet Ligar(NOLST_tppNoLista pEsq, NOLST_tppNoLista pDir) {
    if (pEsq){
		pEsq->pProx = pDir;
    }//if
    if (pDir){
		pDir->pAnt = pEsq;
    }//if
	return NOLST_CondRetOK;
}
