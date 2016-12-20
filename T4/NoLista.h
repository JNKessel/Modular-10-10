/*******************************************************************************************************************************
*	$MCD Módulo de definição: Módulo No Lista
*	Arquivo:	NoLista.c
*	Letras identificadoras:		NOLST
*
*	Autores:	rrc	-	Rafael Rubim Cabral
*				phf	-	Pedro Henrique Frayha
*				jnk	-	Julio Neuman Kessel
*
*	$HA Histórico de evolução:
*		Versâo	Autor	Data		Observações
*		1.00	rrc		17/10/2016	Começo de implementação, maioria das funções ainda não implementadas
*		1.10	rrc		09/12/2016	Documentação de funções
*		2.00	rrc		13/12/2016	Implementação de outras funções
*		3.00	rrc		14/12/2016	Fim das implementações
*		4.00	rrc		15/12/2016	Adições ao módulo, correção de bugs
*		5.00	rrc		18/12/2016	Várias adições de funções a serem usadas pelo módulo ITFC
*		6.00	jnk		19/12/2016	Editado 
*
*	Esse módulo disponibiliza uma estrutura de dados genérica que serve de nó de uma lista. Cada nó possui duas referências (ponteiros)
*	para outros nós, ou nada (NULL). O módulo permite sua manipulação de forma que sua aplicação possa ser variada segundo o módulo que
*	o utiliza. Cada nó guarda um valor de um elemento de forma genérica (ponteiro).
*******************************************************************************************************************************/

#pragma once

#if defined( NOLISTA_OWN )
   #define NOLISTA_EXT
#else
   #define NOLISTA_EXT extern
#endif

typedef enum {
	NOLST_CondRetOK				= 0,
		/* Nenhum problema. */

	NOLST_CondRetErro			= 1,
		/* Erro de inconsistência */

	NOLST_CondRetSemMemoria		= 2,
		/*  */

	NOLST_CondRetNoOcupado		= 3,
	NOLST_CondRetNoInexistente	= 4
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

/***********************************************************************
* $FC Funçao: NOLST_LigarNos
* 
* $ED Descriçao da funçao:
* Recebe dois nós e os liga, ignorando outros nós a que possam estar
* ligados. Aceita NULL em qualquer parâmetro, o que pode ligar um nó com
* NULL.
*
* $EP Parâmetros:
*	$P pEsq	-	nó que ficará à esquerda (ou NULL). Seu ponteiro para direita
*				agora será pDir. Seu ponteiro esquerdo não se altera.
*	$P pDir	-	nó que ficará à direita (ou NULL). Seu ponteiro para esquerda
*				agora será pEsq. Seu ponteiro direito não se altera.
*
* $FV Valores de Retorno:
*     NOLST_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
NOLST_tpCondRet NOLST_LigarNos(NOLST_tppNoLista pEsq, NOLST_tppNoLista pDir);











