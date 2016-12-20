/*******************************************************
* $MCD Modulo de definiçao: Modulo ListaC
* Arquivo:   ListaC.h
*
* Autores: jnk - Julio Neuman Kessel
*          rrc - Rafael Rubim Cabral
*          phf - Pedro Henrique Frayha
*******************************************************/
#pragma once

#if defined( LISTAC_OWN )
   #define LISTAC_EXT
#else
   #define LISTAC_EXT extern
#endif

typedef struct LSTC_tgListaC* LSTC_tppListaC;

typedef enum {
	LSTC_CondRetOK					=	0,
		/* Nenhum problema */

	LSTC_CondRetSemMemoria			=	1,
		/* Falta de memória */

	LSTC_CondRetElemInexistente		=	2,
		/* Elemento não existe */

	LSTC_CondRetNoNaoCriado			=	3,
		/* Nó não foi criado */

	LSTC_CondRetErroNo				=	4,
		/* Erro no uso do módulo NOLST */

	LSTC_CondRetListaInexistente	=	5,
		/* Lista passada não existe (é NULL) */

	LSTC_CondRetListaVazia			=	6,
		/* Lista está *pasme* vazia */

	LSTC_CondRetPonteiroPerdidoProx	=	7,
		/* Na estrutura da lista, há em algum lugar um ponteiro que não é válido apontando para a direita */

	LSTC_CondRetPonteiroPerdidoAnt	=	8
		/* Na estrutura da lista, há em algum lugar um ponteiro que não é válido apontando para a esquerda */
} LSTC_tpCondRet;

typedef enum {
  	Corrente1,
	Corrente2,
	Corrente3,
   	Cabeca,
 	NumElem,
	pProx,
	pAnt
} LSTC_Deturpacao;

/***********************************************************************
* $FC FunçãoLST_CriarListaC
* 
* $FV Valores de Retorno
*     LSTC_CondRetSemMemoria - Falta de Memoria
*     LSTC_CondRetOK - Criou sem problemas
***********************************************************************/
LSTC_tpCondRet LSTC_CriarListaC(LSTC_tppListaC* pLstCRet, void (* ExcluirElem)(void* pInfo));

/***********************************************************************
* $FC FunçãoLST_DestruirLista
* 
* $FV Valores de Retorno
*     LSTC_CondRetErroNo - Erro na destruição da lista
*     LSTC_CondRetOK - Destruiu sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_DestruirListaC(LSTC_tppListaC pLstC);

/***********************************************************************
* $FC FunçãoLST_EhListaCVazia
* 
* $FV Valores de Retorno
*     LSTC_CondRetOK
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_EhListaCVazia(LSTC_tppListaC pLstC, int* pEhVaziaRet);

/***********************************************************************
* $FC LSTC_EhListaCVaziaAlternativa
* 
*(Função alternativa para a funçao "LSTC_tpCondRet LSTC_EhListaCVazia(LSTC_tppListaC pLstC, int* pEhVaziaRet)")
*
* $FV Valores de Retorno
*     LSTC_CondRetOK
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_EhListaCVaziaAlternativa(LSTC_tppListaC pLstC, int* pEhVazia);

/***********************************************************************
* $FC FunçãoLST_InserirElemento
* 
* $FV Valores de Retorno
*     LSTC_CondRetNoNaoCriado - Erro na criação do nó
*     LSTC_CondRetErroNo - Erro ao manipular nó
*     LSTC_CondRetOK - Elemento inserido sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_InserirElemento(LSTC_tppListaC pLstC, int iPos, void* pInfo);

/***********************************************************************
* $FC LSTC_InserirElementoAlt
* 
*(Função alternativa para a funçao "LSTC_tpCondRet LSTC_InserirElemento(LSTC_tppListaC pLstC, int iPos, void* pInfo)")
*
* $FV Valores de Retorno
*     LSTC_CondRetNoNaoCriado - Erro na criação do nó
*     LSTC_CondRetErroNo - Erro ao manipular nó
*     LSTC_CondRetOK - Elemento inserido sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_InserirElementoAlt(LSTC_tppListaC pLstC, int iPos, void* pInfo);

/***********************************************************************
* $FC FunçãoLST_RetirarElemento
* 
* $FV Valores de Retorno
*     LSTC_CondRetListaVazia - Erro remoção em lista vazia
*     LSTC_CondRetErroNo - Erro ao manipular nó
*     LSTC_CondRetOK - Elemento retirado sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_RetirarElemento(LSTC_tppListaC pLstC, int iPos);

/***********************************************************************
* $FC LSTC_RetirarElementoAlt
* 
*(Função alternativa para a funçao "LSTC_tpCondRet LSTC_RetirarElemento(LSTC_tppListaC pLstC, int iPos)")
*
* $FV Valores de Retorno
*     LSTC_CondRetListaVazia - Erro remoção em lista vazia
*     LSTC_CondRetPosInvalida - Erro posição invalida
*     LSTC_CondRetErroNo - Erro ao manipular nó
*     LSTC_CondRetOK - Elemento retirado sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_RetirarElementoAlt(LSTC_tppListaC pLstC, int iPos);

/***********************************************************************
* $FC FunçãoLST_AtribuirElemento
* 
* $FV Valores de Retorno
*     LSTC_CondRetListaVazia - Erro atribuição em lista vazia
*     LSTC_CondRetErroNo - Erro ao manipular nó
*     LSTC_CondRetOK - Atribuição realizada sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_AtribuirElemento(LSTC_tppListaC pLstC, int iPos, void* pInfo);

/***********************************************************************
* $FC FunçãoLST_ObterElemento
* 
* $FV Valores de Retorno
*     LSTC_CondRetListaVazia - Erro obtenção em lista vazia
*     LSTC_CondRetErroNo - Erro ao manipular nó
*     LSTC_CondRetOK - Elemento obtido sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_ObterElemento(LSTC_tppListaC pLstC, int iPos, void** pInfoRet);

/***********************************************************************
* $FC FunçãoLST_MoverCorrente
* 
* $FV Valores de Retorno
*     LSTC_CondRetOK - Operação realizada sem problemas
*     LSTC_CondRetErroNo - Erro ao manipular nó
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_MoverCorrente(LSTC_tppListaC pLstC, int iN);

/***********************************************************************
* $FC FunçãoLST_ProcurarElemento
* 
* $FV Valores de Retorno
*     LSTC_CondRetErroNo - Erro ao manipular nó
*     LSTC_CondRetElemInexistente - Erro elemento não existe
*     LSTC_CondRetOK - Busca realizada sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
*     LSTC_CondRetListaVazia - Erro de inconsistência em código
***********************************************************************/
// Retorna o indice do elemento procurado ou -1 se inexistente. "criterio" é a função que retorna 1 caso seu parâmetro corresponda ao que se procura (O tipo de elemBuscado pode ser diferente do tipo padrão da lista)
LSTC_tpCondRet LSTC_ProcurarElemento(LSTC_tppListaC pLstC, void* pElemBuscado, int* pIndiceRet, int (*Criterio)(void* pElemBuscado, void* pElemLista));

/***********************************************************************
* $FC FunçãoLST_ObterTamanhoListaC
* 
* $FV Valores de Retorno
*     LSTC_CondRetOK - Operação realizada sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_ObterTamanhoListaC(LSTC_tppListaC pLstC, int* pTamanhoRet);

/***********************************************************************
* $FC FunçãoLST_ObterTamanhoListaCAlt
*
* (Função alternativa para a funçao "LSTC_tpCondRet LSTC_ObterTamanhoListaC(LSTC_tppListaC pLstC, int* pTamanhoRet)")
* 
* $FV Valores de Retorno
*     LSTC_CondRetOK - Operação realizada sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_ObterTamanhoListaCAlt(LSTC_tppListaC pLstC, int* pTamanhoRet);

/***********************************************************************
* $FC FunçãoLST_ObterTamanhoListaCAlt
*
* (Segunda função alternativa para a funçao "LSTC_tpCondRet LSTC_ObterTamanhoListaC(LSTC_tppListaC pLstC, int* pTamanhoRet)")
* 
* $FV Valores de Retorno
*     LSTC_CondRetOK - Operação realizada sem problemas
*	  LSTC_CondRetListaInexistente - Não existe lista
***********************************************************************/
LSTC_tpCondRet LSTC_ObterTamanhoListaCAlt2(LSTC_tppListaC pLstC, int* pTamanhoRet);

/***********************************************************************
* $FC Função: LST_Deturpadora
***********************************************************************/
#ifdef _DEBUG
	LSTC_tpCondRet LSTC_Deturpadora(LSTC_tppListaC pLstC, LSTC_Deturpacao cod);
#endif
