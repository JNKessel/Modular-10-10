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
    TAB_CondRetCasaInexistente
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
TAB_tpCondRet TAB_CriarTabuleiro(TAB_tppTabuleiro* tab);


/***********************************************************************
* $FC Funçao: TAB_ChecarDisponivel
* 
* $ED Descriçao da funçao:
* Retorna se existe um peao, n casas a frente da casa passada para funçao
* e qual a cor desse peao
*
* $FV Valores de Retorno:
*
***********************************************************************/
TAB_tpCondRet TAB_ChecarDisponivel(TAB_tppCasa casa, int n, DEF_tpCor* cRetorno);
/*Esta funcao esta associada com a implementacao da dinamica da partida*/


/***********************************************************************
* $FC Funçao: TAB_RetornarCasa
* 
* $ED Descriçao da funçao:
* Retorna em casaRetorno e casa que um peao da cor c deve ir ao andar n casas a
* partir de casa
*
* $FV Valores de Retorno:
*
***********************************************************************/
TAB_tpCondRet TAB_RetornarCasa(DEF_tpCor c, TAB_tppCasa casa, int n, TAB_tppCasa casaRetorno);
/*Esta funcao esta associada com a implementacao da dinamica da partida*/


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
TAB_tpCondRet TAB_RetornarCasaDeSaida(TAB_tppTabuleiro tab, DEF_tpCor c, TAB_tppCasa* casaRetorno);


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
TAB_tpCondRet TAB_DestruirTabuleiro(TAB_tppTabuleiro tab);

#endif
/********** Fim da definição: modulo **********/









