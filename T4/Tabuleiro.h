/*****************************************************************
 *
 * Módulo de definição: Módulo Tabuleiro
 *
 * Nome do arquivo: Tabuleiro.h
 *
 *
 *	rrc - 19/10/2016 - Uniformização de nomes e retornos
 *
 *  jnk - 19/10/2016 - Adição de comentarios e descrição de funções
 /****************************************************************/

#if !defined TAB_MOD
#define TAB_MOD

#include "Definicoes.h"
#include "ListaC.h"
#include "Lista.h"

/***** Tipo de dados exportado pelo módulo *****/

typedef struct TAB_tgTabuleiro* TAB_tppTabuleiro;

typedef enum{
    TAB_CondRetOK,
    TAB_CondRetErro,
    TAB_CondRetSemMemoria,
    TAB_CondRetErroListaCircular,
    TAB_CondRetErroLista,
    TAB_CondRetTabJaExiste,
    TAB_CondRetTabInexistente,
    TAB_CondRetCasaInexistente,
    TAB_CondRetUltrapassouFinal
	} TAB_tpCondRet;

typedef struct TAB_tgCasa* TAB_tppCasa;


/***********************************************************************
* $FC Funçao: TAB_CriarTabuleiro
* 
* $ED Descriçao da funçao:
* Cria um novo tabuleiro de ludo e suas casas
*
* $FV Valores de Retorno:
*     TAB_CondRetSemMemoria - Falta de Memoria
*     TAB_CondRetErroListaCircular - Erro no uso de função do modulo ListaC
*     TAB_CondRetErroLista - Erro no uso de função do modulo Lista
*     TAB_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
TAB_tpCondRet TAB_CriarTabuleiro();

/***********************************************************************
* $FC Funçao: TAB_RetornarCasa
* 
* $ED Descriçao da funçao:
* Retorna em casaRetorno a casa que um peao da cor c deve ir ao andar n casas a
* partir de casa (Pode-se considerar que o peão nao vai estar na bae)
*
* $FV Valores de Retorno:
*	TAB_CondRetOK
*	TAB_CondRetUltrapassouFinal	-	casa à frente não existe no tabuleiro (passou o final de um dos oscars)
*	TAB_CondRetErroLista
*	TAB_CondRetErroListaCircular
*	TAB_CondRetCasaInexistente	-	casa passada não está no tabuleiro
***********************************************************************/
TAB_tpCondRet TAB_RetornarCasa(TAB_tppCasa casa, DEF_tpCor corPeao, int n, TAB_tppCasa *casaRetorno);

/***********************************************************************
* $FC Funçao: TAB_ChecarCor
* 
* $ED Descriçao da funçao:
* Retorna a cor de um peao que está n casas a frente da casa passada para 
* função. Retorna SEM_COR caso não haja peão nessa casa.
*
* $FV Valores de Retorno:
*	TAB_CondRetOK
*	TAB_CondRetUltrapassouFinal
*	TAB_CondRetErroLista
*	TAB_CondRetErroListaCircular
***********************************************************************/
TAB_tpCondRet TAB_ChecarCor(TAB_tppCasa casa, int n, DEF_tpCor corPeao, DEF_tpCor* cRetorno);

/***********************************************************************
* $FC Funçao: TAB_RetornarCasaDeSaida
* 
* $ED Descriçao da funçao:
* Retorna em casaRetorno a casa de saida de base dos peoes da cor c
*
* $FV Valores de Retorno:
*     TAB_CondRetErroListaCircular - Erro no uso de função do modulo ListaC
*     TAB_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
TAB_tpCondRet TAB_RetornarCasaDeSaida(DEF_tpCor c, TAB_tppCasa* casaRetorno); 


/***********************************************************************
* $FC Funçao: TAB_EhCasaFinal
* 
* $ED Descriçao da funçao:
* Recebe uma casa e retorna na variavel finalRetorno se essa casa é
* uma casa final ou nao
*
* $FV Valores de Retorno:
*     TAB_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
TAB_tpCondRet TAB_EhCasaFinal(TAB_tppCasa c, DEF_tpBool* finalRetorno);


/***********************************************************************
* $FC Funçao: TAB_DestruirTabuleiro
* 
* $ED Descriçao da funçao:
* Destroi da memoria um tabuleiro e suas casas
*
* $FV Valores de Retorno:
*     TAB_CondRetErroListaCircular - Erro no uso de função do modulo ListaC
*     TAB_CondRetErroLista - Erro no uso de função do modulo Lista
*     TAB_CondRetOK - Operaçao realizada sem problemas
***********************************************************************/
TAB_tpCondRet TAB_DestruirTabuleiro();

#endif
/********** Fim da definição: modulo **********/









