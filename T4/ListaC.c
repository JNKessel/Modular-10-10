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

/* Incluir stdio para uso no módulo "Generico" */
#include <stdio.h>

#define LISTAC_OWN
	#include "ListaC.h"
#undef LISTAC_OWN

/* Modulo do arcabouco de teste */
#ifdef _DEBUG
	#include "Generico.h"
	#include "Conta.h"
	#include "cespdin.h"
	#include "IdTiposEspaco.def"
#endif

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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_CriarFaltouMemoria");
		#endif
		return LSTC_CondRetSemMemoria;
	} /* if */
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_CriarSucesso");
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
	LSTC_tpCondRet assertiva;
	
	if (!pLstC){
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	} /* if */
	
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
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_DestruirTratadorPonteiro");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_DestruirLoop");
	#endif
	
	while (pLstC->pNoCorr1) {
		
		void* tempInfo;
		NOLST_tppNoLista temp = pLstC->pNoCorr1;
		debugNo = NOLST_ObterProxNoh(pLstC->pNoCorr1, &pLstC->pNoCorr1);

		/* Se o próximo nó é ele mesmo, então só restou um nó na lista circular. */
		if (pLstC->pNoCorr1 == temp){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_DestruirNoUnico");
			#endif
			pLstC->pNoCorr1 = NULL;
		}

		if (debugNo){
			
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_DestuicaoErroNo1");
			#endif

			return LSTC_CondRetErroNo;
		}
		debugNo = NOLST_ObterInfoNoh(temp, &tempInfo);
		
		if (debugNo){
			
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_DestruicaoErroNo2");
			#endif

			return LSTC_CondRetErroNo;
		}
		
		if (pLstC->ExcluirElem && tempInfo){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_DestruirExcluirElem");
			#endif
			pLstC->ExcluirElem(tempInfo);
		}
		
		debugNo = NOLST_DestruirNoh(temp);
		if (debugNo){
			
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_DestruicaoErroNo3");
			#endif

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
	LSTC_tpCondRet assertiva;
	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_EhVaziaListaInexistente");
		#endif
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
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_EhVaziaTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}


	if (!pLstC->pNoCorr1){		// if(pLstC->pNoCorr == NULL)
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
} /* Fim função: LSTC  &Eh ListaC Vazia */


/***************************************************************************
*
*  Função: LSTC  &Eh ListaC Vazia Alternativa
*  ****/

LSTC_tpCondRet LSTC_EhListaCVaziaAlt(LSTC_tppListaC pLstC, int* pEhVazia) {
	LSTC_tpCondRet assertiva;
	
	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_EhVaziaAltListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	}
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_EhVaziaAltTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
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
} /* Fim Funcao: LSTC &Eh ListaC Vazia Alternativa */

/***************************************************************************
*
*  Função: LSTC  &Inserir Elemento
*  ****/

LSTC_tpCondRet LSTC_InserirElemento(LSTC_tppListaC pLstC, int iPos, void* pInfo) {

	NOLST_tppNoLista novo = NULL, temp;
	NOLST_tpCondRet debugNo;
	LSTC_tpCondRet debug;
	LSTC_tpCondRet assertiva;
	
	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	}
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2)
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}

	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 || (pLstC->NumElem != 0 && iPos >= pLstC->NumElem) || (pLstC->NumElem == 0 && iPos > 0)){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirListaNaoVazia");
		#endif

		/* Se a lista tem 5 elementos, inserir no índice 5 equivale a inserir no índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, inserir no índice -1 equivale a inserir no índice 4 (já que a lista é circular). */
		if (iPos < 0){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_InserirPosMenorQue0");
			#endif
			iPos += pLstC->NumElem;
		}
	} else {
			/* Se a lista está vazia */
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirListaVazia");

		/* Independente do índice passado, a inserção ocorrerá na posição 0. Esse elemento será o novo corrente. */
		iPos = 0;
	}

	if (iPos == 0) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirPos0");
		#endif
				   
		if (pLstC->NumElem == 0) {
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_InserirPrimeiroElemento");
			#endif
			debugNo = NOLST_CriarNoh(&novo, pInfo, NULL, NULL);
			if (debugNo){
				#ifdef _DEBUG
					CNT_CONTAR("LSTC_InserirErroNo1");
				#endif
				return LSTC_CondRetNoNaoCriado;
			}

			debugNo = NOLST_LigarNos(novo, novo);
			if (debugNo){
				#ifdef _DEBUG
					CNT_CONTAR("LSTC_InserirErroNo2");
				#endif
				return LSTC_CondRetErroNo;
			}
			
			pLstC->pNoCorr1 = novo;
			pLstC->pNoCorr2 = novo;
			pLstC->pNoCorr3 = novo;
		} else {
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_InserirAposCorrente");
			#endif
			debugNo = NOLST_CriarNoh(&novo, pInfo, NULL, NULL);
			if (debugNo){
				#ifdef _DEBUG
					CNT_CONTAR("LSTC_InserirErroNo3");
				#endif
				return LSTC_CondRetNoNaoCriado;
			}
			
			debugNo = NOLST_ColocarNohEmFrente(pLstC->pNoCorr1, novo);
			if (debugNo){
				#ifdef _DEBUG
					CNT_CONTAR("LSTC_InserirErroNo4");
				#endif
				return LSTC_CondRetErroNo;
			}
		}
		
		debugNo = NOLST_AtribuirInfoNoh(novo, pInfo);
		if (debugNo){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_InserirErroNo5");
			#endif
			return LSTC_CondRetErroNo;
		}
		
		pLstC->NumElem++;
		
		return LSTC_CondRetOK;
	}
	
	debug = JumpToPos(pLstC, iPos, &temp);
	if (debug){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirErroJump");
		#endif
		return debug;
	}
	
	debugNo = NOLST_CriarNoh(&novo, pInfo, NULL, NULL);
	if (debugNo){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirErroNo4");
		#endif
		return LSTC_CondRetNoNaoCriado;
	}
	
	debugNo = NOLST_AtribuirInfoNoh(novo, pInfo);
	if (debugNo){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirErroNo5");
		#endif
		return LSTC_CondRetErroNo;
	}
	
	debugNo = NOLST_ColocarNohEmFrente(temp, novo);
	if (debugNo){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirErroNo6");
		#endif
		return LSTC_CondRetErroNo;
	}
	
	(pLstC->NumElem)++;
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_InserirOK");
	#endif
	
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
	LSTC_tpCondRet assertiva;
	
	if(pLstC == NULL){
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirAltListaInexistente");
		#endif
		
       		return LSTC_CondRetListaInexistente;
    	}

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3)
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirAltTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirAltTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}
    
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
    if(iPos < 0 
       || iPos > pLstC->NumElem){
	    #ifdef _DEBUG
	    	CNT_CONTAR("LSTC_InserirAltPosInvalida");
	    #endif
	    return LSTC_CondRetPosInvalida;
    }*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirAltListaNaoVazia");
		#endif

		/* Se a lista tem 5 elementos, inserir no índice 5 equivale a inserir no índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, inserir no índice -1 equivale a inserir no índice 4 (já que a lista é circular). */
		if (iPos < 0)
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_InserirAltPosMenorQue0");
			#endif
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia */
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirAltListaVazia");
		#endif

		/* Independente do índice passado, a inserção ocorrerá na posição 0. Esse elemento será o novo corrente. */
		iPos = 0;
	}
	
	#ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirAltLoopObterProxNo");
	#endif
	
    for(i=0 ; i<iPos ; i++){	    
       	    debugNo = NOLST_ObterProxNoh(No, &No);
            if(debugNo){
		    #ifdef _DEBUG
			CNT_CONTAR("LSTC_InserirAltErroNo1");
		    #endif
		    return LSTC_CondRetErroNo;
	    }
    }
     
    debugNo = NOLST_ObterProxNoh(No, &prox);
    if(debugNo){
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_InserirAltErroNo2");
	#endif
	return LSTC_CondRetErroNo;
    }
     
    debugNo = NOLST_CriarNoh(&No, pInfo, No, prox);
    if(debugNo){
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_InserirAltErroNo3");
	#endif
        return LSTC_CondRetErroNo;
    }
     
    (pLstC->NumElem)++;
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_InserirAltOK");
	#endif
     
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
	LSTC_tpCondRet assertiva;
	
	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	}
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}


	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/

	if (pLstC->NumElem != 0) {
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarListaNaoVazia");
		#endif
			/* Se a lista não está vazia */

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_RetirarPosMenosQue0");
			#endif
			iPos += pLstC->NumElem;
		}
	} else {
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarListaVazia");
		#endif
			/* Se a lista está vazia, erro de remoção */

		return LSTC_CondRetListaVazia;
	}

	
	if (iPos % pLstC->NumElem == 0) {
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarPosMultiploNumElem");
		#endif

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
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_RetirarErroNo1");
			#endif
			return LSTC_CondRetErroNo;
		}
		
	} else {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarNaoMultiploNumElem");
		#endif
		
		debug = JumpToPos(pLstC, iPos, &temp);
		if (debug){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_RetirarErroJump");
			#endif
			return debug;
		}
		
	}

	debugNo = NOLST_ObterInfoNoh(temp, &tempInfo);
	if (debugNo) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarErroNo2");
		#endif
		return LSTC_CondRetErroNo;
	}
	
	if (pLstC->ExcluirElem && tempInfo){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarExcluirElem");
		#endif
		pLstC->ExcluirElem(tempInfo);
	}
	
	debugNo = NOLST_DestruirNoh(temp);
	if (debugNo) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarErroNo3");
		#endif
		return LSTC_CondRetErroNo;
	}
	
	(pLstC->NumElem)--;
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_RetirarOK");
	#endif
	
	return LSTC_CondRetOK;
} /* Fim funcao: LSTC &Retirar Elemento */

/***************************************************************************
*
*  Função: LSTC  &Retirar Elemento Alternativa
*  ****/

LSTC_tpCondRet LSTC_RetirarElementoAlt(LSTC_tppListaC pLstC, int iPos){
	int i;
	NOLST_tppNoLista No, prox;
	NOLST_tpCondRet debugNo;
	LSTC_tpCondRet assertiva;
	
	if(pLstC == NULL){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarAltListaInexistente");
		#endif
        	return LSTC_CondRetListaInexistente;
    	}

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarAltTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarAltTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}
    
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
    if(iPos < 0 
       || iPos >= pLstC->NumElem){
	    #ifdef _DEBUG
	    	CNT_CONTAR("LSTC_RetirarAltPosInvalida");
	    #endif
	    return LSTC_CondRetPosInvalida;
    }*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarAltListaNaoVazia");
		#endif

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0)
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_RetirarAltPosMenorQue0");
			#endif
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia, erro de remoção */
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarAltListaVazia");
		#endif

		return LSTC_CondRetListaVazia;
	}

	No = pLstC->pNoCorr1;
	
	prox = pLstC->pNoCorr1;
		/* Para não dar warning */

    if(iPos == 0) {
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_RetirarAltErroNo1");
		#endif
                return LSTC_CondRetErroNo;
            }
        }
    }
     
    debugNo = NOLST_DestruirNoh(No);
    if(debugNo){
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_RetirarAltErroNo2");
	#endif
        return LSTC_CondRetErroNo;
    }
    
    if(iPos == 0) {
	#ifdef _DEBUG
	  	CNT_CONTAR("LSTC_RetirarAltPosCabeca2");
	#endif
	pLstC->pNoCorr1 = prox;
	pLstC->pNoCorr2 = prox;
	pLstC->pNoCorr3 = prox;
    }
    
	(pLstC->NumElem)--;
	
	#ifdef _DEBUG
	  	CNT_CONTAR("LSTC_RetirarAltOK");
	#endif
     
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
        LSTC_tpCondRet assertiva;
	
	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	}
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}

	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirListaNaoVazia");
		#endif

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_AtribuirPosMenorQue0");
			#endif
			iPos += pLstC->NumElem;
		}
	} else {
			/* Se a lista está vazia, erro de atribuição */
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirListaVazia");
		#endif

		return LSTC_CondRetListaVazia;
	}


	debug = JumpToPos(pLstC, iPos, &no);
	if (debug){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirErroJump");
		#endif
		return debug;
	}

	debugNo = NOLST_AtribuirInfoNoh(no, pInfo);
	if (debugNo){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AtribuirErroNo1");
		#endif
		return LSTC_CondRetErroNo;
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_AtribuirOK");
	#endif

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
	LSTC_tpCondRet assertiva;
	
	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	}
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}
	
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/

	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterListaNaoVazia");
		#endif

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_ObterPosMenorQue0");
			#endif
			iPos += pLstC->NumElem;
		}
	} else {
			/* Se a lista está vazia, erro de obtenção */
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterListaVazia");
		#endif

		return LSTC_CondRetListaVazia;
	}
	
	debug = JumpToPos(pLstC, iPos, &no);
	if (debug){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterErroJump");
		#endif
		return debug;
	}

	debugNo = NOLST_ObterInfoNoh(no, pInfoRet);
	if (debugNo){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ObterErroNo1");
		#endif
		return LSTC_CondRetErroNo;
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_ObterOK");
	#endif

	return LSTC_CondRetOK;
} /* Fim funcao: LSTC &Obter Elemento */

/***************************************************************************
*
*  Função: LSTC  &Mover Corrente
*  ****/

LSTC_tpCondRet LSTC_MoverCorrente(LSTC_tppListaC pLstC, int iN) {
	LSTC_tpCondRet debug;
	NOLST_tppNoLista no;
	LSTC_tpCondRet assertiva;
	
	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_MoverCorrenteListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	}
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_MoverCorrenteTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}
	
	debug = JumpToPos(pLstC, iN, &no);
	if (debug){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_MoverCorrenteErroJump");
		#endif
		return debug;
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_MoverCorrenteOK");
	#endif
	
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
	LSTC_tpCondRet assertiva;
	
	if (!pLstC) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ProcurarListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	}
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_ProcurarTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}

	if (!Criterio) {
		Criterio = CriterioPadrao;
	}

	for (i = 0; i < pLstC->NumElem; i++) {
		void* tempInfo;
		NOLST_tppNoLista tempNo;
		
		debug = JumpToPos(pLstC, i, &tempNo);
		if (debug){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_ProcurarErroJump");
			#endif
			return debug;
		}
		
		debugNo = NOLST_ObterInfoNoh(tempNo, &tempInfo);
		if (debugNo){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_ProcurarErroNo1");
			#endif
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
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_ProcurarOK");
	#endif

	return LSTC_CondRetOK; 
} /* Fim funcao: LSTC &Procurar Elemento */

/***************************************************************************
*
*  Função: LSTC  &Obter Tamanho ListaC
*  ****/

LSTC_tpCondRet LSTC_ObterTamanhoListaC(LSTC_tppListaC pLstC, int* pTamanhoRet) {

	int tamanho1, tamanho2, tamanho3;
	LSTC_tpCondRet debugListaC;

	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoListaInexistente");
		#endif
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
	
	//Assertiva
	tamanho1 = *pTamanhoRet;
	debugListaC = LSTC_ObterTamanhoListaCAlt(pLstC, &tamanho2);
	/* Se não retornou OK, erro */
	if (debugListaC) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoErroTamanhoAlt1");
		#endif
		return debugListaC;
	}
	debugListaC = LSTC_ObterTamanhoListaCAlt2(pLstC, &tamanho3);
	/* Se não retornou OK, erro */
	if (debugListaC) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoErroTamanhoAlt2");
		#endif
		return debugListaC;
	}
	if((tamanho1 != tamanho2) ||(tamanho1 != tamanho3)){
	
		//tratador
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoTratadorTamanhoInconsistente");
		#endif
		*pTamanhoRet = TratadorParaObterTamanhoInconsistente(tamanho1, tamanho2, tamanho2);
	
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_TamanhoOK");
	#endif
	
	return LSTC_CondRetOK;
} /* Fim funcao: LSTC &Obter Tamanho ListaC */

/***************************************************************************
*
*  Função: LSTC  &Obter Tamanho ListaC Alternativa
*  ****/

LSTC_tpCondRet LSTC_ObterTamanhoListaCAlt(LSTC_tppListaC pLstC, int* pTamanhoRet) {
	NOLST_tppNoLista noTemp;
	LSTC_tpCondRet assertiva;

	*pTamanhoRet = 1;
	if (!pLstC) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAltListaInexistente");
		#endif
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
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAltTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}


	if (pLstC->pNoCorr1 == NULL) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAltListaVazia");
		#endif
		*pTamanhoRet = 0;
	} else {
		NOLST_ObterProxNoh(pLstC->pNoCorr1, &noTemp);
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAltLoop");
		#endif
		while(noTemp != pLstC->pNoCorr1) {
	
			NOLST_ObterProxNoh(noTemp, &noTemp);
			(*pTamanhoRet) ++;
		}
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_TamanhoAltOK");
	#endif

	return LSTC_CondRetOK;
} /* Fim funcao: LSTC &Obter Tamanho ListaC Alternativa */

/***************************************************************************
*
*  Função: LSTC  &Obter Tamanho ListaC Alternativa 2
*  ****/

LSTC_tpCondRet LSTC_ObterTamanhoListaCAlt2(LSTC_tppListaC pLstC, int* pTamanhoRet) {
	NOLST_tppNoLista noTemp;
	LSTC_tpCondRet assertiva;

	*pTamanhoRet = 1;
	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAlt2ListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	}

	if((pLstC->pNoCorr1 != pLstC->pNoCorr2) 
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAlt2TratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAlt2TratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}

	if (pLstC->pNoCorr1 == NULL) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAlt2ListaVazia");
		#endif
		*pTamanhoRet = 0;
	} else {
		NOLST_ObterNohPrevio(pLstC->pNoCorr1, &noTemp);
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TamanhoAlt2Loop");
		#endif
		while(noTemp != pLstC->pNoCorr1){
	
			NOLST_ObterNohPrevio(noTemp, &noTemp);
			(*pTamanhoRet) ++;
		}
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_TamanhoAlt2OK");
	#endif

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
	
	LSTC_tpCondRet assertiva;
	
	NOLST_tppNoLista aux = pLstC->pNoCorr1;
	NOLST_tpCondRet debugNo;

	if (!pLstC){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_JumpListaInexistente");
		#endif
		return LSTC_CondRetListaInexistente;
	}
	
	if((pLstC->pNoCorr1 != pLstC->pNoCorr2)
	   || (pLstC->pNoCorr1 != pLstC->pNoCorr3) 
	   || (pLstC->pNoCorr2 != pLstC->pNoCorr3)){ //Assertiva
		//Tratador
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_JumpTratadorCorrente");
		#endif
		TratadorParaCorrentePerdida(pLstC);
	}
	/*A assertiva retorna um tipo de erro informando qual a direçao do ponteiro na lista(duplamente incadeada) que tem erro.
	  O Tratador vai entao(sabendo a direçao do ponteiro com problema) consertar o problema e se a assertiva nao detectar
	  nenhum erro o tratador nao faz nada*/
	assertiva = AssertivaParaPonteiroPerdido(pLstC);
	if(assertiva != LSTC_CondRetOK){
		/*tratador*/
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_JumpTratadorPonteiroPerdido");
		#endif
		TratadorParaPonteiroPerdido(pLstC, assertiva);
	}
	
	/*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> RETIREI POSSIBILIDADE DE POS_INVALIDA
	if (iPos < 0 
	    || iPos >= pLstC->NumElem){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_JumpPosInvalida");
		#endif
		return LSTC_CondRetPosInvalida;
	}*/


	if (pLstC->NumElem != 0) {
			/* Se a lista não está vazia */
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_JumpListaNaoVazia");
		#endif

		/* Se a lista tem 5 elementos, passar o índice 5 equivale a passar o índice 0 (já que a lista é circular). */
		iPos = iPos % pLstC->NumElem;

		/* Se a lista tem 5 elementos, passar o índice -1 equivale a passar o índice 4 (já que a lista é circular). */
		if (iPos < 0)
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_JumpPosMenorQue0");
			#endif
			iPos += pLstC->NumElem;
	} else {
			/* Se a lista está vazia, erro de pulo */
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_JumpListaVazia");
		#endif

		return LSTC_CondRetListaVazia;
	}

	#ifdef _DEBUG
		CNT_CONTAR("LSTC_JumpLoop");
	#endif
	
	while(iPos--) {
		debugNo = NOLST_ObterProxNoh(aux, &aux);
		if (debugNo){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_JumpErroNo1");
			#endif
			return LSTC_CondRetErroNo;
		}
	}

	*pNoRet = aux;
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_JumpOK");
	#endif

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
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TratadorCorrenteCorr1DiferenteCorr2");
		#endif
	
			if(pLstC->pNoCorr2 == pLstC->pNoCorr3){
				#ifdef _DEBUG
					CNT_CONTAR("LSTC_TratadorCorrenteCorr2IgualCorr3");
				#endif
				pLstC->pNoCorr1 = pLstC->pNoCorr2;
			}
			else if(pLstC->pNoCorr1 == pLstC->pNoCorr3){
				#ifdef _DEBUG
					CNT_CONTAR("LSTC_TratadorCorrenteCorr1IgualCorr3");
				#endif
				pLstC->pNoCorr2 = pLstC->pNoCorr1;
		
			}
	}
	else if(pLstC->pNoCorr1 != pLstC->pNoCorr3){
		
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TratadorCorrenteCorr1DiferenteCorr3");
		#endif

		if(pLstC->pNoCorr1 == pLstC->pNoCorr2){
			
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_TratadorCorrenteCorr1IgualCorr2");
			#endif

			pLstC->pNoCorr3 = pLstC->pNoCorr1;
		}
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_TratadorCorrenteOK");
	#endif
	
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TratadorTamanhoTam1DiferenteTam2");
		#endif
	
		if(Tam2 == Tam3){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_TratadorTamanhoTam2IgualTam3");
			#endif
			return Tam2;
		
		}
		else if(Tam1 == Tam3){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_TratadorTamanhoTam1IgualTam3");
			#endif
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
*		LSTC_CondRetPonteiroPerdidoProx, caso ponteiro prox tenha sido perdido
*		LSTC_CondRetPonteiroPerdidoAnt, caso ponteiro ant tenha sido perdido
*		LSTC_CondRetOK, caso tudo esteja em ordem
*		
*******************************************************************************************************************************/

//checa se algum ponteiro da lista foi perdido em qualquer direçao da lista
LSTC_tpCondRet AssertivaParaPonteiroPerdido(LSTC_tppListaC pLstC){

	NOLST_tppNoLista noAnt = pLstC->pNoCorr1;
	NOLST_tppNoLista noProx = pLstC->pNoCorr1;
	NOLST_tppNoLista noTemp;

	// Lista vazia
	if (pLstC->pNoCorr1 == NULL) {
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_AssertivaListaVazia");
		#endif
		return LSTC_CondRetOK;
	}
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_AssertivaListaNaoVazia");
	#endif

	do{
		NOLST_ObterProxNoh(noProx, &noProx);
		if(noProx == NULL){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_AssertivaPerdidoProx");
			#endif
			return LSTC_CondRetPonteiroPerdidoProx;
		}
	
		NOLST_ObterNohPrevio(noProx, &noTemp);
		if(noTemp != noAnt){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_AssertivaPerdidoAnt");
			#endif
			return LSTC_CondRetPonteiroPerdidoAnt;
		}
		
		NOLST_ObterProxNoh(noAnt, &noAnt);
	
	}while(noProx != pLstC->pNoCorr1);
	
	noAnt = pLstC->pNoCorr1;
	noProx = pLstC->pNoCorr1;
	
	do{
		NOLST_ObterNohPrevio(noAnt, &noAnt);
		if(noAnt == NULL){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_AssertivaPerdidoAnt");
			#endif
			return LSTC_CondRetPonteiroPerdidoAnt;
		}
	
		NOLST_ObterProxNoh(noAnt, &noTemp);
		if(noTemp != noProx){
			#ifdef _DEBUG
				CNT_CONTAR("LSTC_AssertivaPerdidoProx");
			#endif
			return LSTC_CondRetPonteiroPerdidoProx;
		}
		
		NOLST_ObterNohPrevio(noProx, &noProx);
	
	}while(noAnt != pLstC->pNoCorr1);
	
	#ifdef _DEBUG
		CNT_CONTAR("LSTC_AssertivaOK");
	#endif
	return LSTC_CondRetOK;
	
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
void TratadorParaPonteiroPerdido(LSTC_tppListaC pLstC, LSTC_tpCondRet direcao){

	NOLST_tppNoLista noAnt = pLstC->pNoCorr1;
	NOLST_tppNoLista noProx = pLstC->pNoCorr1;
	NOLST_tppNoLista noTemp;
	
	if(direcao == LSTC_CondRetPonteiroPerdidoProx){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TratadorPonteiroProx");
		#endif
		do{
			NOLST_ObterNohPrevio(noAnt, &noAnt);
			NOLST_ObterProxNoh(noAnt, &noTemp);
			if(noTemp != noProx){
				
				NOLST_LigarNos(noAnt, noTemp);
				
			}
		
			NOLST_ObterNohPrevio(noProx, &noProx);
	
		}while(noAnt != pLstC->pNoCorr1);
	
	}else if(direcao == LSTC_CondRetPonteiroPerdidoAnt){
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_TratadorPonteiroAnt");
		#endif
		do{
			NOLST_ObterProxNoh(noProx, &noProx);
			NOLST_ObterNohPrevio(noProx, &noTemp);
			if(noTemp != noAnt){
				NOLST_LigarNos(noAnt, noTemp);
			}
		
			NOLST_ObterProxNoh(noAnt, &noAnt);
	
		}while(noProx != pLstC->pNoCorr1);
		
	}
	#ifdef _DEBUG
			CNT_CONTAR("LSTC_TratadorPonteiroOK");
	#endif
	
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
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_DeturpadoraCorrente");
		#endif
            pLstC->pNoCorr1 = NULL;
            break;
         
        case(Cabeca):
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_DeturpadoraCabeca");
		#endif
            pLstC = NULL;
            break;
             
        case(NumElem):
		#ifdef _DEBUG
			CNT_CONTAR("LSTC_DeturpadoraNumElem");
		#endif
            pLstC->NumElem = -1;
            break;
             
        default:
			;
    }

	#ifdef _DEBUG
			CNT_CONTAR("LSTC_DeturpadoraOK");
	#endif
    return LSTC_CondRetOK;
} /* Fim Funcao: LSTC &Deturpadora */

/********** Fim do módulo de implementação: LSTC  Lista circular duplamente encadeada **********/
