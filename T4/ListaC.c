#include "ListaC.h"
#include "CONTA.h"		//Modulo do arcabouco de teste
#include <stdlib.h>
#include "NoLista.h"

typedef struct LSTC_tgListaC {
	NOLST_tppNoLista pNoCorr1;
	NOLST_tppNoLista pNoCorr2;
	NOLST_tppNoLista pNoCorr3;
	int NumElem;
	void (* ExcluirElem)(void* pInfo);
} LSTC_tpListaC;

static LSTC_tpCondRet JumpToPos(LSTC_tppListaC pLstC, int iPos, NOLST_tppNoLista* pNoRet);

static int CriterioPadrao(void* pElemBuscado, void* pElemLista);

static void TratadorParaCorrentePerdida(LSTC_tppListaC pLstC);

static void TratadorParaPonteiroPerdido(LSTC_tppListaC pLstC, int direcao);

static int AssertivaParaPonteiroPerdido(LSTC_tppListaC pLstC);

LSTC_tpCondRet LSTC_CriarListaC(LSTC_tppListaC* pLstCRet, void (* ExcluirElem)(void* pInfo)) {
	*pLstCRet = (LSTC_tppListaC)malloc(sizeof(LSTC_tpListaC));
	if (!pLstCRet){
		// //CNT_CONTAR("LSTC_CriarFaltouMemoria");
		return LSTC_CondRetSemMemoria;
	}
	
	//CNT_CONTAR("LSTC_CriarSucesso");
	
	(*pLstCRet)->pNoCorr1 = NULL;
	(*pLstCRet)->pNoCorr2 = NULL;
	(*pLstCRet)->pNoCorr3 = NULL;
	(*pLstCRet)->NumElem = 0;
	(*pLstCRet)->ExcluirElem = ExcluirElem;
	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_DestruirListaC(LSTC_tppListaC pLstC) {
	NOLST_tpCondRet debugNo;
	if (!pLstC){
		
		////CNT_CONTAR("LSTC_ListaInexistente");
		
		return LSTC_CondRetListaInexistente;
	}
	
	//CNT_CONTAR("LSTC_DestruirListaExistente");

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		//CNT_CONTAR("LSTC_DestruirTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	//CNT_CONTAR("LSTC_DestruirLoop");
	
	while (pLstC->pNoCorr1) {
		
		void* tempInfo;
		NOLST_tppNoLista temp = pLstC->pNoCorr1;
		debugNo = NOLST_ObterProxNoh(pLstC->pNoCorr1, &pLstC->pNoCorr1);

		if (debugNo){
			
			// //CNT_CONTAR("LSTC_DestuicaoErroNo1");
			
			return LSTC_CondRetErroNo;
		}
		debugNo = NOLST_ObterInfoNoh(temp, &tempInfo);
		
		if (debugNo){
			
			// //CNT_CONTAR("LSTC_DestruicaoErroNo2");
			
			return LSTC_CondRetErroNo;
		}
		
		if (pLstC->ExcluirElem && tempInfo){
			pLstC->ExcluirElem(tempInfo);
		}
		
		free(temp); // Implementação não assume erro em free
	}
	free(pLstC);
	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_EhListaCVazia(LSTC_tppListaC pLstC, int* pEhVaziaRet) {
	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}

	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		//CNT_CONTAR("LSTC_EhVaziaTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));


	if (!pLstC->pNoCorr1){
		
		//CNT_CONTAR("LSTC_EhVaziaListaVazia");
		
		*pEhVaziaRet = 1;
	}
	else{
		
		//CNT_CONTAR("LSTC_EhVaziaListaNaoVazia");
		
		*pEhVaziaRet = 0;
	}
	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_EhListaCVaziaAlternativa(LSTC_tppListaC pLstC, int* pEhVazia){
	if(pLstC->NumElem){
		
	    	//CNT_CONTAR("LSTC_EhVaziaAltListaVazia");
		
	    	*pEhVazia = 0;
    	}
	else{
		
		//CNT_CONTAR("LSTC_EhVaziaAltListaNaoVazia");
		
		*pEhVazia = 1;
	}
     
    return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_InserirElemento(LSTC_tppListaC pLstC, int iPos, void* pInfo) {

	NOLST_tppNoLista novo = NULL, temp;
	NOLST_tpCondRet debugNo;
	LSTC_tpCondRet debug;
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2)
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		//CNT_CONTAR("LSTC_InserirTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}
	if (iPos < 0 
	    || iPos > pLstC->NumElem){
		
		//CNT_CONTAR("LSTC_InserirPosInvalida");
		
		return LSTC_CondRetPosInvalida;
	}

	if (iPos == 0) {
		
		//CNT_CONTAR("LSTC_InserirNovaCabeca");
		
		if (pLstC->NumElem == 0) {
			
			//CNT_CONTAR("LSTC_InserirPrimeiroElemento");
			
			debugNo = NOLST_CriarNoh(&novo, pInfo, NULL, NULL);
			if (debugNo){
				return LSTC_CondRetNoNaoCriado;
			}

			debugNo = NOLST_LigarNos(novo, novo);
			if (debugNo){
				return LSTC_CondRetErroNo;
			}
			
			pLstC->pNoCorr1 = novo;
			pLstC->pNoCorr2 = novo;
			pLstC->pNoCorr3 = novo;
		} else {
			
			//CNT_CONTAR("LSTC_InserirSubstituiCabeca");
			
			debugNo = NOLST_CriarNoh(&novo, pInfo, NULL, NULL);
			if (debugNo){
				return LSTC_CondRetNoNaoCriado;
			}
			
			debugNo = NOLST_ColocarNohEmFrente(pLstC->pNoCorr1, novo);
			if (debugNo){
				return LSTC_CondRetErroNo;
			}
		}
		
		debugNo = NOLST_AtribuirInfoNoh(novo, pInfo);
		if (debugNo){
			return LSTC_CondRetErroNo;
		}
		
		pLstC->NumElem++;
		
		return LSTC_CondRetOK;
	}

	//CNT_CONTAR("LSTC_InserirPosValida");
	
	debug = JumpToPos(pLstC, iPos, &temp);
	if (debug){
		return debug;
	}
	
	debugNo = NOLST_CriarNoh(&novo, pInfo, NULL, NULL);
	if (debugNo){
		return LSTC_CondRetNoNaoCriado;
	}
	
	debugNo = NOLST_AtribuirInfoNoh(novo, pInfo);
	if (debugNo){
		return LSTC_CondRetErroNo;
	}
	
	debugNo = NOLST_ColocarNohEmFrente(temp, novo);
	if (debugNo){
		return LSTC_CondRetErroNo;
	}
	
	pLstC->NumElem++;
	
	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_InserirElementoAlt(LSTC_tppListaC pLstC, int iPos, void* pInfo){
   	int i;
	NOLST_tpCondRet debugNo;
    	NOLST_tppNoLista No = pLstC->pNoCorr1, prox;

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3)
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		//CNT_CONTAR("LSTC_InserirAltTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));
     
    if(pLstC == NULL){
        return LSTC_CondRetListaInexistente;
    }
     
    if(iPos < 0 || iPos > pLstC->NumElem){
	    
	    //CNT_CONTAR("LSTC_InserirAltPosInvalida");
	    
	    return LSTC_CondRetPosInvalida;
    }
   
    //CNT_CONTAR("LSTC_InserirAltPosValida");
	
    for(i=0 ; i<iPos ; i++){	    
       	    debugNo = NOLST_ObterProxNoh(No, &No);
            if(debugNo){
		    return LSTC_CondRetErroNo;
	    }
    }
     
    debugNo = NOLST_ObterProxNoh(No, &prox);
    if(debugNo){
        return LSTC_CondRetErroNo;
    }
     
    debugNo = NOLST_CriarNoh(&No, pInfo, No, prox);
    if(debugNo){
        return LSTC_CondRetErroNo;
    }
     
    pLstC->NumElem;
     
    return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_RetirarElemento(LSTC_tppListaC pLstC, int iPos) {
	NOLST_tppNoLista temp;
	NOLST_tpCondRet debugNo;
	LSTC_tpCondRet debug;
	void* tempInfo;
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	if (!pLstC)	return LSTC_CondRetListaInexistente;
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		//CNT_CONTAR("LSTC_RetirarPosInvalida");
		return LSTC_CondRetPosInvalida;
	}

	//CNT_CONTAR("LSTC_RetirarPosValida");
	
	if (iPos % pLstC->NumElem == 0) {

		if((pLstC->pNoCorr1 != pLstC->pNoCorr2)
		   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
		   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
			//Tratador
			
			//CNT_CONTAR("LSTC_RetirarTratadorCorrente");
			
			TratadorParaCorrentePerdida(pLstC);
	    	}

		temp = pLstC->pNoCorr1;
		debugNo = NOLST_ObterProxNoh(pLstC->pNoCorr1, &pLstC->pNoCorr1);
		pLstC->pNoCorr2 = pLstC->pNoCorr1;
		pLstC->pNoCorr3 = pLstC->pNoCorr1;

		if (debugNo){
			return LSTC_CondRetErroNo;
		}
		
	} else {
		
		debug = JumpToPos(pLstC, iPos, &temp);
		if (debug){
			return debug;
		}
		
	}

	debugNo = NOLST_ObterInfoNoh(temp, &tempInfo);
	if (debugNo){
		return LSTC_CondRetErroNo;
	}
	
	if (pLstC->ExcluirElem && tempInfo){
		pLstC->ExcluirElem(tempInfo);
	}
	
	free(temp);
	
	pLstC->NumElem--;
	
	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_RetirarElementoAlt(LSTC_tppListaC pLstC, int iPos){
	int i;
	NOLST_tppNoLista No = pLstC->pNoCorr1, prox;
	NOLST_tpCondRet debugNo;

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		//CNT_CONTAR("LSTC_RetirarAltTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));
     
    if(pLstC == NULL){
        return LSTC_CondRetListaInexistente;
    }
     
    if(iPos < 0 
       || iPos >= pLstC->NumElem){
	    //CNT_CONTAR("LSTC_RetirarAltPosInvalida");
	    return LSTC_CondRetPosInvalida;
    }
     
    if(iPos == 0){
	    //CNT_CONTAR("LSTC_RetirarAltPos0");
	    NOLST_ObterProxNoh(No, &prox);
    }
     
    else{ 
        for(i=0;i<iPos;i++){
            debugNo = NOLST_ObterProxNoh(No, &No);
            if(debugNo){
                return LSTC_CondRetErroNo;
            }
        }
    }
     
    debugNo = NOLST_DestruirNoh(No);
    if(debugNo){
        return LSTC_CondRetErroNo;
    }
     
	pLstC->pNoCorr1 = prox;
	pLstC->pNoCorr2 = prox;
	pLstC->pNoCorr3 = prox;
	
	pLstC->NumElem--;
     
	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_AtribuirElemento(LSTC_tppListaC pLstC, int iPos, void* pInfo) {

	NOLST_tppNoLista no;
	LSTC_tpCondRet debug;
	NOLST_tpCondRet debugNo;
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		//CNT_CONTAR("LSTC_AtribuirTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}
	
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		
		//CNT_CONTAR("LSTC_AtribuirPosInvalida");
		
		return LSTC_CondRetPosInvalida;
	}
	
	//CNT_CONTAR("LSTC_AtribuirPosValida");

	debug = JumpToPos(pLstC, iPos, &no);
	if (debug){
		return debug;
	}

	debugNo = NOLST_AtribuirInfoNoh(no, pInfo);
	if (debugNo){
		return LSTC_CondRetErroNo;
	}

	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_ObterElemento(LSTC_tppListaC pLstC, int iPos, void** pInfoRet) {

	NOLST_tppNoLista no;
	LSTC_tpCondRet debug;
	NOLST_tpCondRet debugNo;
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		//CNT_CONTAR("LSTC_ObterTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}
	
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		
		//CNT_CONTAR("LSTC_ObterPosInvalida");
		
		return LSTC_CondRetPosInvalida;
	}

	//CNT_CONTAR("LSTC_ObterPosValida");
	
	debug = JumpToPos(pLstC, iPos, &no);
	if (debug){
		return debug;
	}

	debugNo = NOLST_ObterInfoNoh(no, pInfoRet);
	if (debugNo){
		return LSTC_CondRetErroNo;
	}

	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_MoverCorrente(LSTC_tppListaC pLstC, int iN) {
	LSTC_tpCondRet debug;
	NOLST_tppNoLista no;
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));
	
	if (!pLstC)	return LSTC_CondRetListaInexistente;
	debug = JumpToPos(pLstC, iN, &no);
	if (debug)	return debug;
	pLstC->pNoCorr1 = no;
	pLstC->pNoCorr2 = no;
	pLstC->pNoCorr3 = no;
	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_ProcurarElemento(LSTC_tppListaC pLstC, void* pElemBuscado, int* pIndiceRet, int (*Criterio)(void* pElemBuscado, void* pElemLst)) {

	int i, flag = 0;
	NOLST_tpCondRet debugNo;
	LSTC_tpCondRet debug;
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}

	if (!Criterio){
		Criterio = CriterioPadrao;
	}

	for (i = 0; i < pLstC->NumElem; i++) {
		void* tempInfo;
		NOLST_tppNoLista tempNo;
		
		debug = JumpToPos(pLstC, i, &tempNo);
		if (debug){
			return debug;
		}
		
		debugNo = NOLST_ObterInfoNoh(tempNo, &tempInfo);
		if (debugNo){
			return LSTC_CondRetErroNo;
		}
		
		if (Criterio(pElemBuscado, tempInfo)) {
			*pIndiceRet = i;
			flag = 1;
			break;
		}
	}
	if (!flag) {
		*pIndiceRet = -1;
		return LSTC_CondRetElemInexistente;
	}

	return LSTC_CondRetOK; 
}

LSTC_tpCondRet LSTC_ObterTamanhoListaC(LSTC_tppListaC pLstC, int* pTamanhoRet) {

	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		//CNT_CONTAR("LSTC_TamanhoTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	*pTamanhoRet = pLstC->NumElem;
	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_ObterTamanhoListaCAlt(LSTC_tppListaC pLstC, int* pTamanhoRet) {
	NOLST_tppNoLista noTemp;

	*pTamanhoRet = 2;
	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		//CNT_CONTAR("LSTC_TamanhoAltTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	NOLST_ObterProxNoh(pLstC->pNoCorr1, &noTemp);
	while(noTemp != pLstC->pNoCorr1){
	
		NOLST_ObterProxNoh(noTemp, &noTemp);
		*pTamanhoRet ++;
	}

	return LSTC_CondRetOK;
}

LSTC_tpCondRet JumpToPos(LSTC_tppListaC pLstC, int iPos, NOLST_tppNoLista* pNoRet) {
	
	NOLST_tppNoLista aux = pLstC->pNoCorr1;
	NOLST_tpCondRet debugNo;

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2)
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		//CNT_CONTAR("LSTC_JumpTratadorCorrente");
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}
	
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		//CNT_CONTAR("LSTC_JumpPosInvalida");
		return LSTC_CondRetPosInvalida;
	}
	
	//CNT_CONTAR("LSTC_JumpPosValida");

	if (iPos >= 0)
		while(iPos--) {
			debugNo = NOLST_ObterProxNoh(aux, &aux);
			if (debugNo)	return LSTC_CondRetErroNo;
		}
	else
		while(iPos++) {
			debugNo = NOLST_ObterNohPrevio(aux, &aux);
			if (debugNo)	return LSTC_CondRetErroNo;
		}

	*pNoRet = aux;
	return LSTC_CondRetOK;
}

int CriterioPadrao(void* pElemBuscado, void* pElemLista) {
	if (pElemBuscado == pElemLista)
		return 1;
	else
		return 0;
}

void TratadorParaCorrentePerdida(LSTC_tppListaC pLstC){

	if(pLstC->pNoCorr1 != pLstC->pNoCorr2){
	
			if(pLstC->pNoCorr2 == pLstC->pNoCorr3){

				pLstC->pNoCorr1 = pLstC->pNoCorr2;
		
			}else if(pLstC->pNoCorr1 == pLstC->pNoCorr3){

				pLstC->pNoCorr2 = pLstC->pNoCorr1;
		
			}
		}else if(pLstC->pNoCorr1 != pLstC->pNoCorr3){

			if(pLstC->pNoCorr1 == pLstC->pNoCorr2){

				pLstC->pNoCorr3 = pLstC->pNoCorr1;
			}
		}
}

//checa se algum ponteiro da lista foi perdido em qualquer direçao da lista
int AssertivaParaPonteiroPerdido(LSTC_tppListaC pLstC){

	NOLST_tppNoLista noAnt = pLstC->pNoCorr1;
	NOLST_tppNoLista noProx = pLstC->pNoCorr1;
	NOLST_tppNoLista noTemp;

	// Lista vazia
	if (pLstC->pNoCorr1 == NULL) {
		return 0;
	}

	do{
		NOLST_ObterProxNoh(noProx, &noProx);
		if(noProx == NULL){
			return 1;
		}
	
		NOLST_ObterNohPrevio(noProx, &noTemp);
		if(noTemp != noAnt){
			return -1;
		}
		
		NOLST_ObterProxNoh(noAnt, &noAnt);
	
	}while(noProx != pLstC->pNoCorr1);
	
	noAnt = pLstC->pNoCorr1;
	noProx = pLstC->pNoCorr1;
	
	do{
		NOLST_ObterNohPrevio(noAnt, &noAnt);
		if(noAnt == NULL){
			return -1;
		}
	
		NOLST_ObterProxNoh(noAnt, &noTemp);
		if(noTemp != noProx){
			return 1;
		}
		
		NOLST_ObterNohPrevio(noProx, &noProx);
	
	}while(noAnt != pLstC->pNoCorr1);
	return 0;
	
}

//resolve o erro se algum ponteiro for perdido
void TratadorParaPonteiroPerdido(LSTC_tppListaC pLstC, int direcao){

	NOLST_tppNoLista noAnt = pLstC->pNoCorr1;
	NOLST_tppNoLista noProx = pLstC->pNoCorr1;
	NOLST_tppNoLista noTemp;
	
	if(direcao == 1){
	
		do{
			NOLST_ObterNohPrevio(noAnt, &noAnt);
			NOLST_ObterProxNoh(noAnt, &noTemp);
			if(noTemp != noProx){
				
				NOLST_LigarNos(noAnt, noTemp);
				
			}
		
			NOLST_ObterNohPrevio(noProx, &noProx);
	
		}while(noAnt != pLstC->pNoCorr1);
	
	}else if(direcao == -1){
	
		do{
			NOLST_ObterProxNoh(noProx, &noProx);
			NOLST_ObterNohPrevio(noProx, &noTemp);
			if(noTemp != noAnt){
				NOLST_LigarNos(noAnt, noTemp);
			}
		
			NOLST_ObterProxNoh(noAnt, &noAnt);
	
		}while(noProx != pLstC->pNoCorr1);
		
	}
}

LSTC_tpCondRet LSTC_Deturpadora(LSTC_tppListaC pLstC, LSTC_Deturpacao cod){
    switch(cod){
        case(Corrente):
            pLstC->pNoCorr1 = NULL;
            break;
         
        case(Cabeca):
            pLstC = NULL;
            break;
             
        case(NumElem):
            pLstC->NumElem = -1;
            break;
             
        default:
			;
    }

    return LSTC_CondRetOK;
}
