/***************************************************************************
*  $MCI Módulo de implementação: LSTC  Lista Circular duplamente encadeada
*
*  Arquivo gerado:              ListaC.c
*  Letras identificadoras:      LSTC
*
*
*  Projeto: INF 1301 / Jogo de Ludo
*  Autores: phf
*	    rrc
*	    jnk
*
*
***************************************************************************/
#include <stdlib.h>

#include "NoLista.h"

#define LISTAC_OWN
	#include "ListaC.h"
#undef LISTAC_OWN

/* Modulo do arcabouco de teste */
/*#ifdef _DEBUG
	#include "Generico.h"
	#include "Conta.h"
	#include "cespdin.h"
	#include "IdTiposEspaco.def"
#endif*/

/***********************************************************************
*	$TC Tipo de dados: LSTC descritor da cabeca da lista circular
*
*	$ED Descrição do tipo
*		Representa a cabeça da lista
***********************************************************************/

typedef struct LSTC_tgListaC {
	NOLST_tppNoLista pNoCorr1;
		/* Ponteiro 1 para nó corrente */

	NOLST_tppNoLista pNoCorr2;
		/* Ponteiro 2 para nó corrente */

	NOLST_tppNoLista pNoCorr3;
		/* Ponteiro 3 para nó corrente */

		/*	$EED Assertivas estruturais
		*		Todos os ponteiros para nós corrente devem apontar 
		*		para o mesmo nó (lista só possui um nó corrente)
		*/

	int NumElem;
		/* Número de elementos da lista circular */

		/*	$EED Assertivas estruturais
		*		Deve equivaler à quantidade de nós diferente que se
		*		se apontam na lista.
		*/

	void (* ExcluirElem)(void* pInfo);
		/* Função de exclusão dos elementos da lista */
} LSTC_tpListaC;

/***** Protótipos das funções encapuladas no módulo *****/

static LSTC_tpCondRet JumpToPos(LSTC_tppListaC pLstC, int iPos, NOLST_tppNoLista* pNoRet);

static int CriterioPadrao(void* pElemBuscado, void* pElemLista);

static void TratadorParaCorrentePerdida(LSTC_tppListaC pLstC);

static void TratadorParaPonteiroPerdido(LSTC_tppListaC pLstC, int direcao);

static int AssertivaParaPonteiroPerdido(LSTC_tppListaC pLstC);

static int TratadorParaObterTamanhoInconsistente(int Tam1, int Tam2, int Tam3);

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: LSTC  &Criar listaC
*  ****/

LSTC_tpCondRet LSTC_CriarListaC(LSTC_tppListaC* pLstCRet, void (* ExcluirElem)(void* pInfo)) {
	*pLstCRet = (LSTC_tppListaC)malloc(sizeof(LSTC_tpListaC));
	if (!pLstCRet){
		// ////CNT_CONTAR("LSTC_CriarFaltouMemoria");
		return LSTC_CondRetSemMemoria;
	} /* if */
	
	#ifdef _DEBUG
		//CNT_CONTAR("LSTC_CriarSucesso");
	#endif
	
	(*pLstCRet)->pNoCorr1 = NULL;
	(*pLstCRet)->pNoCorr2 = NULL;
	(*pLstCRet)->pNoCorr3 = NULL;
	(*pLstCRet)->NumElem = 0;
	(*pLstCRet)->ExcluirElem = ExcluirElem;
	
	return LSTC_CondRetOK;
} /* Fim função: LIS  &Criar lista */

/***************************************************************************
*
*  Função: LIS  &Destruir lista
*  ****/

LSTC_tpCondRet LSTC_DestruirListaC(LSTC_tppListaC pLstC) {
	NOLST_tpCondRet debugNo;
	if (!pLstC){
		
		////CNT_CONTAR("LSTC_ListaInexistente");
		
		return LSTC_CondRetListaInexistente;
	} /* if */
	
	#ifdef _DEBUG
		//CNT_CONTAR("LSTC_DestruirListaExistente");
	#endif

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_DestruirTratadorCorrente");
		#endif
		
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));
	
	#ifdef _DEBUG
		//CNT_CONTAR("LSTC_DestruirLoop");
	#endif
	
	while (pLstC->pNoCorr1) {
		
		void* tempInfo;
		NOLST_tppNoLista temp = pLstC->pNoCorr1;
		debugNo = NOLST_ObterProxNoh(pLstC->pNoCorr1, &pLstC->pNoCorr1);

		/* Se o próximo nó é ele mesmo, então só restou um nó na lista circular. */
		if (pLstC->pNoCorr1 == temp)
			pLstC->pNoCorr1 = NULL;

		if (debugNo){
			
			////CNT_CONTAR("LSTC_DestuicaoErroNo1");
			
			return LSTC_CondRetErroNo;
		}
		debugNo = NOLST_ObterInfoNoh(temp, &tempInfo);
		
		if (debugNo){
			
			////CNT_CONTAR("LSTC_DestruicaoErroNo2");
			
			return LSTC_CondRetErroNo;
		}
		
		if (pLstC->ExcluirElem && tempInfo){
			pLstC->ExcluirElem(tempInfo);
		}
		
		debugNo = NOLST_DestruirNoh(temp);
		if (debugNo){
			
			////CNT_CONTAR("LSTC_DestruicaoErroNo3");
			
			return LSTC_CondRetErroNo;
		}
	}
	free(pLstC);
	return LSTC_CondRetOK;
} /* Fim função: LIS  &Destruir lista */

/***************************************************************************
*
*  Função: LSTC  &Eh ListaC Vazia
*  ****/

LSTC_tpCondRet LSTC_EhListaCVazia(LSTC_tppListaC pLstC, int* pEhVaziaRet) {
	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}

	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_EhVaziaTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));


	if (!pLstC->pNoCorr1){		// if(pLstC->pNoCorr == NULL)
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_EhVaziaListaVazia");
		#endif
		*pEhVaziaRet = 1;
	}
	else{
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_EhVaziaListaNaoVazia");
		#endif
		*pEhVaziaRet = 0;
	}
	return LSTC_CondRetOK;
} /* Fim função: LSTC  &Eh ListaC Vazia */


/***************************************************************************
*
*  Função: LSTC  &Eh ListaC Vazia Alternativa
*  ****/

LSTC_tpCondRet LSTC_EhListaCVaziaAlt(LSTC_tppListaC pLstC, int* pEhVazia) {
	if(pLstC->NumElem){
		#ifdef _DEBUG
	    		//CNT_CONTAR("LSTC_EhVaziaAltListaVazia");
		#endif
	    	*pEhVazia = 0;
    	}
	else{
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_EhVaziaAltListaNaoVazia");
		#endif
		*pEhVazia = 1;
	}
     
    return LSTC_CondRetOK;
} /* Fim Funcao: LSTC &Eh ListaC Vazia Alternativa */

/***************************************************************************
*
*  Função: LSTC  &Inserir Elemento
*  ****/

LSTC_tpCondRet LSTC_InserirElemento(LSTC_tppListaC pLstC, int iPos, void* pInfo) {

	NOLST_tppNoLista novo = NULL, temp;
	NOLST_tpCondRet debugNo;
	LSTC_tpCondRet debug;
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2)
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_InserirTratadorCorrente");
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
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 || (pLstC->NumElem != 0 && iPos >= pLstC->NumElem) || (pLstC->NumElem == 0 && iPos > 0)){
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_InserirPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */

		/* Se a lista tem 5 elementos, inserir no índice 5 equivale a inserir no índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, inserir no índice -1 equivale a inserir no índice 4 (já que a lista é circular). */
		if (iPos < 0)
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia */

		/* Independente do índice passado, a inserção ocorrerá na posição 0. Esse elemento será o novo corrente. */
		iPos = 0;
	}

	if (iPos == 0) {
		if (pLstC->NumElem == 0) {
			#ifdef _DEBUG
				//CNT_CONTAR("LSTC_InserirPrimeiroElemento");
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
				//CNT_CONTAR("LSTC_InserirPosZero");
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
		//CNT_CONTAR("LSTC_InserirPosValida");
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
} /* Fim Funcao: LSTC &Inserir Elemento */

/***************************************************************************
*
*  Função: LSTC  &Inserir Elemento Alternativa
*  ****/

LSTC_tpCondRet LSTC_InserirElementoAlt(LSTC_tppListaC pLstC, int iPos, void* pInfo){
   	int i;
	NOLST_tpCondRet debugNo;
    	NOLST_tppNoLista No = pLstC->pNoCorr1, prox;

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3)
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_InserirAltTratadorCorrente");
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
    
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
    if(iPos < 0 
       || iPos > pLstC->NumElem){
	    #ifdef _DEBUG
	    	//CNT_CONTAR("LSTC_InserirAltPosInvalida");
	    #endif
	    return LSTC_CondRetPosInvalida;
    }*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */

		/* Se a lista tem 5 elementos, inserir no índice 5 equivale a inserir no índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, inserir no índice -1 equivale a inserir no índice 4 (já que a lista é circular). */
		if (iPos < 0)
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia */

		/* Independente do índice passado, a inserção ocorrerá na posição 0. Esse elemento será o novo corrente. */
		iPos = 0;
	}




	#ifdef _DEBUG
    		//CNT_CONTAR("LSTC_InserirAltPosValida");
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
} /* Fim Funcao: LSTC &Inserir Elemento Alternativa */

/***************************************************************************
*
*  Função: LSTC  &Retirar Elemento
*  ****/

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

	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_RetirarPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0)
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia, erro de remoção */

		return LSTC_CondRetListaVazia;
	}

	#ifdef _DEBUG
		//CNT_CONTAR("LSTC_RetirarPosValida");
	#endif
	if (iPos % pLstC->NumElem == 0) {

		if((pLstC->pNoCorr1 != pLstC->pNoCorr2)
		   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
		   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
			//Tratador
			#ifdef _DEBUG
				//CNT_CONTAR("LSTC_RetirarTratadorCorrente");
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
} /* Fim funcao: LSTC &Retirar Elemento */

/***************************************************************************
*
*  Função: LSTC  &Retirar Elemento Alternativa
*  ****/

LSTC_tpCondRet LSTC_RetirarElementoAlt(LSTC_tppListaC pLstC, int iPos){
	int i;
	NOLST_tppNoLista No = pLstC->pNoCorr1, prox;
	NOLST_tpCondRet debugNo;

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_RetirarAltTratadorCorrente");
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
    
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
    if(iPos < 0 
       || iPos >= pLstC->NumElem){
	    #ifdef _DEBUG
	    	//CNT_CONTAR("LSTC_RetirarAltPosInvalida");
	    #endif
	    return LSTC_CondRetPosInvalida;
    }*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0)
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia, erro de remoção */

		return LSTC_CondRetListaVazia;
	}
     
	#ifdef _DEBUG
	    	//CNT_CONTAR("LSTC_RetirarAltPosValida"); 
	#endif
	
    if(iPos == 0){
	    #ifdef _DEBUG
	    	//CNT_CONTAR("LSTC_RetirarAltPosCabeca");
	    #endif
	    NOLST_ObterProxNoh(No, &prox);
    }
     
    else{ 
	    #ifdef _DEBUG
	    	//CNT_CONTAR("LSTC_RetirarAltPosNaoCabeca");
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
} /* Fim funcao: LSTC &Retirar Elemento Alternativa */

/***************************************************************************
*
*  Função: LSTC  &Atribuir Elemento
*  ****/

LSTC_tpCondRet LSTC_AtribuirElemento(LSTC_tppListaC pLstC, int iPos, void* pInfo) {

	NOLST_tppNoLista no;
	LSTC_tpCondRet debug;
	NOLST_tpCondRet debugNo;
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_AtribuirTratadorCorrente");
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
	
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_AtribuirPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0)
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia, erro de atribuição */

		return LSTC_CondRetListaVazia;
	}


	#ifdef _DEBUG
		//CNT_CONTAR("LSTC_AtribuirPosValida");
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
} /* Fim funcao: LSTC &Atribuir Elemento */

/***************************************************************************
*
*  Função: LSTC  &Obter Elemento
*  ****/

LSTC_tpCondRet LSTC_ObterElemento(LSTC_tppListaC pLstC, int iPos, void** pInfoRet) {

	NOLST_tppNoLista no;
	LSTC_tpCondRet debug;
	NOLST_tpCondRet debugNo;
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_ObterTratadorCorrente");
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
	
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_ObterPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0)
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia, erro de obtenção */

		return LSTC_CondRetListaVazia;
	}

	#ifdef _DEBUG
		//CNT_CONTAR("LSTC_ObterPosValida");
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
} /* Fim funcao: LSTC &Obter Elemento */

/***************************************************************************
*
*  Função: LSTC  &Mover Corrente
*  ****/

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
} /* Fim funcao: LSTC &Mover Corrente */

/***************************************************************************
*
*  Função: LSTC  &Procurar Elemento
*  ****/

LSTC_tpCondRet LSTC_ProcurarElemento(LSTC_tppListaC pLstC, void* pElemBuscado, int* pIndiceRet, int (*Criterio)(void* pElemBuscado, void* pElemLst)) {

	int i, flag = 0;
	NOLST_tpCondRet debugNo;
	LSTC_tpCondRet debug;
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	if (!pLstC) {
		return LSTC_CondRetListaInexistente;
	}

	if (!Criterio) {
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
				//CNT_CONTAR("LSTC_ProcurarAchouElem");
			#endif
			*pIndiceRet = i;
			flag = 1;
			break;
		}
	}
	if (!flag) {
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_ProcurarNaoAchouElem");
		#endif
		*pIndiceRet = -1;
		return LSTC_CondRetElemInexistente;
	}

	return LSTC_CondRetOK; 
} /* Fim funcao: LSTC &Procurar Elemento */

/***************************************************************************
*
*  Função: LSTC  &Obter Tamanho ListaC
*  ****/

LSTC_tpCondRet LSTC_ObterTamanhoListaC(LSTC_tppListaC pLstC, int* pTamanhoRet) {

	int tamanho1, tamanho2, tamanho3;

	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_TamanhoTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	*pTamanhoRet = pLstC->NumElem;
	
	//Assertiva
	tamanho1 = *pTamanhoRet;
	LSTC_ObterTamanhoListaCAlt(pLstC, &tamanho2);
	LSTC_ObterTamanhoListaCAlt2(pLstC, &tamanho3);
	if((tamanho1 != tamanho2) ||(tamanho1 != tamanho3)){
	
		//tratador
		*pTamanhoRet = TratadorParaObterTamanhoInconsistente(tamanho1, tamanho2, tamanho2);
	
	}
	
	return LSTC_CondRetOK;
} /* Fim funcao: LSTC &Obter Tamanho ListaC */

/***************************************************************************
*
*  Função: LSTC  &Obter Tamanho ListaC Alternativa
*  ****/

LSTC_tpCondRet LSTC_ObterTamanhoListaCAlt(LSTC_tppListaC pLstC, int* pTamanhoRet) {
	NOLST_tppNoLista noTemp;

	*pTamanhoRet = 1;
	if (!pLstC) {
		return LSTC_CondRetListaInexistente;
	}

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_TamanhoAltTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));


	if (pLstC->pNoCorr1 == NULL) {
		*pTamanhoRet = 0;
	} else {
		NOLST_ObterProxNoh(pLstC->pNoCorr1, &noTemp);
		while(noTemp != pLstC->pNoCorr1) {
	
			NOLST_ObterProxNoh(noTemp, &noTemp);
			(*pTamanhoRet) ++;
		}
	}

	return LSTC_CondRetOK;
} /* Fim funcao: LSTC &Obter Tamanho ListaC Alternativa */

/***************************************************************************
*
*  Função: LSTC  &Obter Tamanho ListaC Alternativa 2
*  ****/

LSTC_tpCondRet LSTC_ObterTamanhoListaCAlt2(LSTC_tppListaC pLstC, int* pTamanhoRet) {
	NOLST_tppNoLista noTemp;

	*pTamanhoRet = 1;
	if (!pLstC){
		return LSTC_CondRetListaInexistente;
	}

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_TamanhoAltTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um inteiro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	/*tratador*/TratadorParaPonteiroPerdido(pLstC, /*assertiva*/AssertivaParaPonteiroPerdido(pLstC));

	if (pLstC->pNoCorr1 == NULL) {
		*pTamanhoRet = 0;
	} else {
		NOLST_ObterNohPrevio(pLstC->pNoCorr1, &noTemp);
		while(noTemp != pLstC->pNoCorr1){
	
			NOLST_ObterNohPrevio(noTemp, &noTemp);
			(*pTamanhoRet) ++;
		}
	}

	return LSTC_CondRetOK;
} /* Fim funcao: LSTC &Obter Tamanho ListaC Alternativa 2 */

/*******************************************************************************************************************************
*	$FC Função: JumpToPos
*
*	$ED Descrição da função:
*		Recebe um ponteiro para lista circular não vazia e a posicao para a qual desejamos executar o salto. Retorna um 
* 		ponteiro para o no da posicao desejada.
*
*	$EP Parâmetros:
*		$P pLstC		-	ponteiro para lista circular nao vazia
*		$P iPos			-	posição para a qual desejamos fazer o pulo
*		$P pNoRet		-	parâmetro que retornara um ponteiro para o no na posicao escolhida.
*								Este parâmetro é passado como referência.
*
*	$FV Valor retornado:
*		LSTC_CondRetOK
*		LSTC_CondRetListaInexistente
*		LSTC_CondRetListaVazia
*		LSTC_CondRetErroNo
*******************************************************************************************************************************/

static LSTC_tpCondRet JumpToPos(LSTC_tppListaC pLstC, int iPos, NOLST_tppNoLista* pNoRet) {
	
	NOLST_tppNoLista aux = pLstC->pNoCorr1;
	NOLST_tpCondRet debugNo;

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2)
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_JumpTratadorCorrente");
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
	
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		#ifdef _DEBUG
			//CNT_CONTAR("LSTC_JumpPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/


	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0)
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia, erro de pulo */

		return LSTC_CondRetListaVazia;
	}

	#ifdef _DEBUG
		//CNT_CONTAR("LSTC_JumpPosValida");
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
} /* Fim funcao: LSTC &Jump To Pos */

/*******************************************************************************************************************************
*	$FC Função: CriterioPadrao
*
*	$ED Descrição da função:
*		Recebe um ponteiro para um elemento buscado e um ponteiro para elemento sob analise. Compara ambos, retornando 1
*		se iguais ou 0 se diferentes.
*
*	$EP Parâmetros:
*		$P pElemBuscado		-	ponteiro para o elemento buscado
*		$P pElemLista		-	ponteiro para o elemento sob analise
*
*	$FV Valor retornado:
*		1 se igual
*		0 se diferente
*******************************************************************************************************************************/

int CriterioPadrao(void* pElemBuscado, void* pElemLista) {
	if (pElemBuscado == pElemLista)
		return 1;
	else
		return 0;
} /* Fim funcao: &Criterio Padrao */

/*******************************************************************************************************************************
*	$FC Função: TratadorParaCorrentePerdida
*
*	$ED Descrição da função:
*		Recebe um ponteiro para lista circular. Trata o caso em que perdemos o ponteiro para o no corrente da lista
*
*	$EP Parâmetros:
*		$P pLstC	-	ponteiro para a lista circular a ser tratada
*
*	$FV Valor retornado:
*		
*******************************************************************************************************************************/

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
} /* Fim funcao: &TratadorParaCorrentePerdida */

/*******************************************************************************************************************************
*	$FC Função: TratadorParaObterTamanhoInconsistente
*
*	$ED Descrição da função:
*		Recebe tres inteiros referentes ao tamanho da lista circular. Retorna a moda do conjunto
*
*	$EP Parâmetros:
*		$P Tam1		-	inteiro que indica o tamanho da lista obtido por um metodo
*		$P Tam2  	-	inteiro que indica o tamanho da lista obtido por um segundo metodo
*		$P Tam3		-	inteiro que indica o tamanho da lista obtido por um terceiro metodo
*
*	$FV Valor retornado:
*		
*******************************************************************************************************************************/

int TratadorParaObterTamanhoInconsistente(int Tam1, int Tam2, int Tam3){

	if(Tam1 != Tam2) {
	
		if(Tam2 == Tam3){

			return Tam2;
		
		}else if(Tam1 == Tam3){

			return Tam1;
		
		}
	} else if(Tam1 != Tam3) {

		if(Tam1 == Tam2){

			return Tam1;
		}
	}
	return Tam1;
} /* Fim funcao: &TratadorParaObterTamanhoInconsistente */

/*******************************************************************************************************************************
*	$FC Função: AssertivaParaPonteiroPerdido
*
*	$ED Descrição da função:
*		Recebe um ponteiro para lista circular. Verifica se alguma ligacao entre os nos foi perdida. Retorna 1 se um 
*		ponteiro prox foi perdido, -1 se um ponteiro ant foi perdido ou 0 se todos os ponteiros estao corretos
*
*	$EP Parâmetros:
*		$P pLstC	-	ponteiro para a lista circular a ser verificada
*
*	$FV Valor retornado:
*		1, caso ponteiro prox tenha sido perdido
*		-1, caso ponteiro ant tenha sido perdido
*		0, caso tudo esteja em ordem
*		
*******************************************************************************************************************************/

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
	
} /* Fim funcao: AssertivaParaPonteiroPerdido */

/*******************************************************************************************************************************
*	$FC Função: TratadorParaPonteiroPerdido
*
*	$ED Descrição da função:
*		Recebe um ponteiro para lista circular e a direcao do ponteiro perdido obtida pela Assertiva acima. A funcao
*		conserta os ponteiros perdidos.
*
*	$EP Parâmetros:
*		$P pLstC	-	ponteiro para a lista circular a ser tratada
*		$P direcao	-	inteiro que indica a direcao do ponteiro perdido
*
*	$FV Valor retornado:
*		
*******************************************************************************************************************************/

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
}/* Fim funcao: &TratadorParaPonteiroPerdido */

/*******************************************************************************************************************************
*	$FC Função: LSTC_Deturpadora
*
*	$ED Descrição da função:
*		Recebe um ponteiro para lista circular e um codigo referente a uma classe enumerada de deturpacao. A funcao abre
*		um switch relativo ao codigo de deturpacao e estraga aquele aspecto da lista circular.
*
*	$EP Parâmetros:
*		$P pLstC	-	ponteiro para a lista circular a ser tratada
*		$P cod		- 	inteiro referente ao aspecto que desejamos deturpar
*
*	$FV Valor retornado:
*		LSTC_CondRetOK
*******************************************************************************************************************************/

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
} /* Fim Funcao: LSTC &Deturpadora */

/********** Fim do módulo de implementação: LSTC  Lista circular duplamente encadeada **********/
