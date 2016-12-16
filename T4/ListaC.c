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
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_CriarSucesso");
	#endif
	
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
		
		//CNT_CONTAR("LSTC_ListaInexistente");
		
		return LSTC_CondRetListaInexistente;
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_DestruirListaExistente");
	#endif

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_DestruirTratadorCorrente");
		#endif
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_DestruirLoop");
	#endif
	
	while (pLstC->pNoCorr1) {
		
		void* tempInfo;
		NOLST_tppNoLista temp = pLstC->pNoCorr1;
		debugNo = NOLST_ObterProxNoh(pLstC->pNoCorr1, &pLstC->pNoCorr1);

		if (debugNo){
			
			//CNT_CONTAR("LSTC_DestuicaoErroNo1");
			
			return LSTC_CondRetErroNo;
		}
		debugNo = NOLST_ObterInfoNoh(temp, &tempInfo);
		
		if (debugNo){
			
			//CNT_CONTAR("LSTC_DestruicaoErroNo2");
			
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_EhVaziaTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));


	if (!pLstC->pNoCorr1){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_EhVaziaListaVazia");
		#endif
		*pEhVaziaRet = 1;
	}
	else{
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_EhVaziaListaNaoVazia");
		#endif
		*pEhVaziaRet = 0;
	}
	return LSTC_CondRetOK;
}

LSTC_tpCondRet LSTC_EhListaCVaziaAlternativa(LSTC_tppListaC pLstC, int* pEhVazia){
	if(pLstC->NumElem){
		#ifdef _DEBUG
	    		CNT_CONTAR("LSTC_EhVaziaAltListaVazia");
		#endif
	    	*pEhVazia = 0;
    	}
	else{
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_EhVaziaAltListaNaoVazia");
		#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirTratadorCorrente");
		#endif
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
	    || (pLstC->NumElem != 0 && iPos >= pLstC->NumElem) || (pLstC->NumElem == 0 && iPos > 0)){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}

	if (iPos == 0) {
		if (pLstC->NumElem == 0) {
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_InserirPrimeiroElemento");
			#endif
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
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_InserirSubstituiCabeca");
			#endif
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
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_InserirPosValida");
	#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirAltTratadorCorrente");
		#endif
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
       || iPos > pLstC->NumElem){
	    #ifdef _DEBUG
	    	CNT_CONTAR("LSTC_InserirAltPosInvalida");
	    #endif
	    return LSTC_CondRetPosInvalida;
    }
	#ifdef _DEBUG
    		CNT_CONTAR("LSTC_InserirAltPosValida");
	#endif
	
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_RetirarPosValida");
	#endif
	if (iPos % pLstC->NumElem == 0) {

		if((pLstC->pNoCorr1 != pLstC->pNoCorr2)
		   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
		   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
			//Tratador
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_RetirarTratadorCorrente");
			#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarAltTratadorCorrente");
		#endif
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
	    #ifdef _DEBUG
	    	CNT_CONTAR("LSTC_RetirarAltPosInvalida");
	    #endif
	    return LSTC_CondRetPosInvalida;
    }
     
	#ifdef _DEBUG
	    	CNT_CONTAR("LSTC_RetirarAltPosValida"); 
	#endif
	
    if(iPos == 0){
	    #ifdef _DEBUG
	    	CNT_CONTAR("LSTC_RetirarAltPosCabeca");
	    #endif
	    NOLST_ObterProxNoh(No, &prox);
    }
     
    else{ 
	    #ifdef _DEBUG
	    	CNT_CONTAR("LSTC_RetirarAltPosNaoCabeca");
	    #endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirTratadorCorrente");
		#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_AtribuirPosValida");
	#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterTratadorCorrente");
		#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}

	#ifdef _DEBUG
		CNT_CONTAR("LSTC_ObterPosValida");
	#endif
	
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
	
	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}
	
	debug = JumpToPos(pLstC, iN, &no);
	if (debug){
		return debug;
	}
	
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
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_ProcurarAchouElem");
			#endif
			*pIndiceRet = i;
			flag = 1;
			break;
		}
	}
	if (!flag) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ProcurarNaoAchouElem");
		#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoTratadorCorrente");
		#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAltTratadorCorrente");
		#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_JumpTratadorCorrente");
		#endif
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_JumpPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_JumpPosValida");
	#endif

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
