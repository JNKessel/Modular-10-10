/*******************************************************
* $MCD Modulo de implementação: Modulo Peao
* Arquivo:   Peao.c
*
* Autores: jnk - Julio Neuman Kessel
*******************************************************/

#include <stdlib.h>
#include "Peao.h"
#include "Definicoes.h"

typedef enum PEAO_tgEstado {
    Base,
    Tabuleiro,
    Final
} PEAO_tpEstado;

typedef struct PEAO_tgPeao {
    TAB_tppCasa pos;
    PEAO_tpEstado estado;
    DEF_tpCor cor;
    int numero;
} PEAO_tpPeao;

PEAO_tpCondRet PEAO_CriarPeao(PEAO_tppPeao* pPeao, DEF_tpCor Cor, int iNum) {

    *pPeao = (PEAO_tppPeao)malloc(sizeof(PEAO_tpPeao));
    if(*pPeao == NULL){

        return PEAO_CondRetSemMemoria;
    }

	if(iNum < 1 || iNum > 4){
	
		return PEAO_CondRetNumeroInvalido;
	
	}
	(*pPeao)->pos = NULL;
	(*pPeao)->estado = Base;
	(*pPeao)->cor = Cor;
	(*pPeao)->numero = iNum;

    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_AndarPeao(PEAO_tppPeao pPeao, int n) {

	TAB_tppCasa casa;

	TAB_RetornarCasa(pPeao->pos, pPeao->cor, n, &casa);

	pPeao->pos = casa;
	
	return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_SairBasePeao(PEAO_tppPeao pPeao) {
  
	TAB_tppCasa casa;

	if(pPeao == NULL) {
		return PEAO_CondRetPeaoInexistente;
	}

    TAB_RetornarCasaDeSaida(pPeao -> cor, &casa);
  
    pPeao -> pos = casa;
    pPeao -> estado = Tabuleiro;

    return PEAO_CondRetOK;
} 

PEAO_tpCondRet PEAO_EstaPeaoFinal(PEAO_tppPeao pPeao, DEF_tpBool* BoolRet) {

	if(pPeao == NULL){
	
		return PEAO_CondRetPeaoInexistente;
	
	}

	if (pPeao->estado == Final)
		*BoolRet = True;
	else
		*BoolRet = False;

    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_EstaPeaoBase(PEAO_tppPeao pPeao, DEF_tpBool* BoolRet) {

	if(pPeao == NULL){
	
		return PEAO_CondRetPeaoInexistente;
	
	}

	if (pPeao->estado == Base)
		*BoolRet = True;
	else
		*BoolRet = False;

    return PEAO_CondRetOK;
}
 
PEAO_tpCondRet PEAO_ObterNumeroPeao(PEAO_tppPeao pPeao, int* NumRet) {

	if(pPeao == NULL){
	
		return PEAO_CondRetPeaoInexistente;
	
	}

	*NumRet = pPeao -> numero;

    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_ObterCasaPeao(PEAO_tppPeao pPeao, TAB_tppCasa * casaRetorno){
	if(pPeao == NULL){
		return PEAO_CondRetPeaoInexistente;
	}
	
	*casaRetorno = pPeao->pos;
	
	return PEAO_CondRetOK;
}
 
PEAO_tpCondRet PEAO_ChecarMovimentoDisponivelPeao(PEAO_tppPeao pPeao, int dado, DEF_tpBool* BoolRet, DEF_tpCor* CorRet) {

	DEF_tpCor corNaCasa;

	if(pPeao == NULL){
	
		return PEAO_CondRetPeaoInexistente;
	
	}

    *CorRet = SEM_COR;
  
    if(dado != 6 && pPeao -> estado == Base) {
        *BoolRet = False;
        return PEAO_CondRetOK;
    }
  
    if(pPeao -> estado == Final){
        *BoolRet = False;
        return PEAO_CondRetOK;
    }
  
	TAB_ChecarDisponivel(pPeao -> pos, dado, pPeao ->cor, &corNaCasa);
  
    if (corNaCasa == NULL || corNaCasa == pPeao -> cor) {
        *BoolRet = False;
        return PEAO_CondRetOK;
    }

    if(corNaCasa == SEM_COR) {
        *BoolRet = True;
        return PEAO_CondRetOK;
    }
  
    *CorRet = corNaCasa;
    *BoolRet = True;

    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_VoltarBasePeao(PEAO_tppPeao pPeao) {
  
	if(pPeao == NULL){
	
		return PEAO_CondRetPeaoInexistente;
	
	}

    pPeao -> pos = NULL;
    pPeao -> estado = Base;

    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_DestruirPeao(PEAO_tppPeao pPeao) {

	if(pPeao == NULL){
	
		return PEAO_CondRetPeaoInexistente;
	
	}

    free(pPeao);

    return PEAO_CondRetOK;
}






