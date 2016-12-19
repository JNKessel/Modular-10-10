/***************************************************************************
*  $MCI Módulo de implementação: TLSTC Teste lista de símbolos
*
*  Arquivo gerado:              TesteListaC.c
*  Letras identificadoras:      TLSTC
*
*  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: rrc
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     1       rrc   19/out/2016 desenvolvimento
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#include    "ListaC.h"


static const char RESET_LISTA_CMD		 [ ] = "=resetteste"	;
static const char CRIAR_LISTA_CMD		 [ ] = "=cria"			;
static const char DESTRUIR_LISTA_CMD	 [ ] = "=destroi"		;
static const char EH_LISTA_VAZIA_CMD	 [ ] = "=ehvazia"		;
static const char INS_ELEM_CMD			 [ ] = "=ins"			;
static const char RETIR_ELEM_CMD         [ ] = "=retir"			;
static const char ATRIBUIR_ELEM_CMD		 [ ] = "=atrib"			;
static const char OBTER_ELEM_CMD		 [ ] = "=obter"			;
static const char MOVE_CORR_CMD			 [ ] = "=move"			;
static const char PROCURAR_ELEM_CMD		 [ ] = "=busca"			;
static const char OBTER_TAM_CMD			 [ ] = "=obtertam"		;


#define TRUE  1
#define FALSE 0

#define VAZIO     0
#define NAO_VAZIO 1
#define IRRELEVANTE 2

#define DIM_VT_LISTA   10
#define DIM_VALOR     100

LSTC_tppListaC	vtListas[ DIM_VT_LISTA ] ;
static char abc[] = "abcdefghij";

/***** Protótipos das funções encapuladas no módulo *****/

   static void DestruirValor( void * pValor ) ;

   static int ValidarInxLista( int inxLista , int Modo ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TLIS &Testar lista
*
*  $ED Descrição da função
*     Podem ser criadas até 10 listas, identificadas pelos índices 0 a 10
*
*     Comandos disponíveis:
*
*     =resetteste
*           - anula o vetor de listas. Provoca vazamento de memória
*     =criarlistac       inxLista
*     =destruirlistac    inxLista
*     =ehlistacvazia	 inxLista  inxRet	
*     =inselem	         inxLista  iPos		inxChar		CondRetEsp
*     =retirelem		 inxLista  iPos		CondRetEsp
*     =atribuirelem		 inxLista  iPos		inxChar	
*     =obterelem		 inxLista  iPos  	inxCharRet  CondRetEsp
*     =movcorr			 inxLista  iN
*     =procurarelem      inxLista  iPos		pCharBuscado	
*     =obtertamlista     inxLista  pNumElem
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int numLidos   = -1 ,
          debugEsp = -1  ;

	  TST_tpCondRet CondRet;
      LSTC_tpCondRet debug;

	  int parmInt[10];
	  char parmChar[10];

      int i ;

      /* Efetuar reset de teste de lista */

         if ( strcmp( ComandoTeste , RESET_LISTA_CMD ) == 0 )
         {

            for( i = 0 ; i < DIM_VT_LISTA ; i++ )
            {
               vtListas[ i ] = NULL ;
            } /* for */

            return TST_CondRetOK ;

         } /* fim ativa: Efetuar reset de teste de lista */

      /* Testar CriarLista */

         else if ( strcmp( ComandoTeste , CRIAR_LISTA_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                       &parmInt[0], &debugEsp) ;

            if ( ( numLidos != 2 )
              || ( ! ValidarInxLista( parmInt[0] , VAZIO )))
            {
               return TST_CondRetParm ;
            } /* if */

            debug = LSTC_CriarListaC(&vtListas[parmInt[0]], NULL);

			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em CriarListaC");
			if (CondRet)	return (CondRet);

            return TST_CompararPonteiroNulo( 1 , vtListas[parmInt[0]] ,
               "Erro em ponteiro de nova listaC.");

         } /* fim ativa: Testar CriarLista */

      /* Testar Esvaziar lista lista */

         else if ( strcmp( ComandoTeste , DESTRUIR_LISTA_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &parmInt[0], &debugEsp);

            if ( ( numLidos != 2)
              || ( ! ValidarInxLista( parmInt[0] , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */

            debug = LSTC_DestruirListaC(vtListas[parmInt[0]]);
			vtListas[parmInt[0]] = NULL;

			return TST_CompararInt(debugEsp, debug, "Condicao de Retorno inesperado em teste DestruirLista");

         } /* fim ativa: Testar Esvaziar lista lista */

      /* Testar Destruir lista */

         else if ( strcmp( ComandoTeste , EH_LISTA_VAZIA_CMD ) == 0 )
         {
			int retorno;
            numLidos = LER_LerParametros( "iii" ,
                               &parmInt[0], &parmInt[1], &debugEsp);

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( parmInt[0] , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */


            debug = LSTC_EhListaCVazia(vtListas[parmInt[0]], &retorno);
			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de Retorno inesperado em teste EhListaCVazia");
			if (CondRet)	return CondRet;
            return TST_CompararInt(parmInt[1], retorno, "Retorno inesperado em teste EhListaCVazia");

         } /* fim ativa: Testar Destruir lista */

      /* Testar inserir elemento antes */

         else if ( strcmp( ComandoTeste , INS_ELEM_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iiii" ,
                       &parmInt[0] , &parmInt[1] , &parmInt[2], &debugEsp) ;

            if ( ( numLidos != 4 )
              || ( ! ValidarInxLista( parmInt[0] , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debug = LSTC_InserirElemento(vtListas[parmInt[0]], parmInt[1], &abc[parmInt[2]]);

            return TST_CompararInt( debugEsp , debug ,
                     "Condicao de retorno errada ao inserir elemento.");

         } /* fim ativa: Testar inserir elemento antes */

      /* Testar inserir elemento apos */

         else if ( strcmp( ComandoTeste , RETIR_ELEM_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                       &parmInt[0] , &parmInt[1] , &debugEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( parmInt[0] , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debug = LSTC_RetirarElemento( vtListas[parmInt[0]] , parmInt[1]);

            return TST_CompararInt( debugEsp , debug ,
                     "Condicao de retorno errada ao retirar elemento.") ;

         } /* fim ativa: Testar inserir elemento apos */

      /* Testar excluir simbolo */

         else if ( strcmp( ComandoTeste , ATRIBUIR_ELEM_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iiii" ,
                  &parmInt[0] , &parmInt[1], &parmInt[2], &debugEsp) ;

            if ( ( numLidos != 4 )
              || ( ! ValidarInxLista( parmInt[0] , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

			debug = LSTC_AtribuirElemento(vtListas[parmInt[0]], parmInt[1], &abc[parmInt[2]]);

            return TST_CompararInt( debugEsp , debug,
				"Condição de retorno errada em AtribuirElemento.") ;

         } /* fim ativa: Testar excluir simbolo */

      /* Testar obter valor do elemento corrente */

         else if ( strcmp( ComandoTeste , OBTER_ELEM_CMD ) == 0 )
         {
			char* obtido;
            numLidos = LER_LerParametros( "iici" ,
                       &parmInt[0] , &parmInt[1] , &parmChar[0], &debugEsp ) ;

            if ( ( numLidos != 4 )
              || ( ! ValidarInxLista( parmInt[0] , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debug = LSTC_ObterElemento(vtListas[parmInt[0]], parmInt[1], (void**)&obtido);
			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em ObterElemento");
			if (CondRet)	return CondRet;

            return TST_CompararChar( parmChar[0] , *obtido ,
                         "Valor do elemento retornado nao corresponde ao esperado." );

         } /* fim ativa: Testar obter valor do elemento corrente */

      /* Testar ir para o elemento inicial */

         else if ( strcmp( ComandoTeste , MOVE_CORR_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" , &parmInt[0], &parmInt[1], &debugEsp );

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( parmInt[0] , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debug = LSTC_MoverCorrente(vtListas[parmInt[0]], parmInt[1]);

            return TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em MoverCorrente") ;

         } /* fim ativa: Testar ir para o elemento inicial */

      /* LIS  &Ir para o elemento final */

         else if ( strcmp( ComandoTeste ,PROCURAR_ELEM_CMD ) == 0 )
         {
			int retorno;
            numLidos = LER_LerParametros( "iiii" , &parmInt[0], &parmInt[1], &parmInt[2], &debugEsp ) ;

            if ( ( numLidos != 4 )
              || ( ! ValidarInxLista( parmInt[0] , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debug = LSTC_ProcurarElemento(vtListas[parmInt[0]], &abc[parmInt[1]], &retorno, NULL);
			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em ProcurarElemento");
			if (CondRet)	return CondRet;

            return TST_CompararInt(parmInt[2], retorno, "Indice encontrado em busca nao corresponde ao esperado");

         } /* fim ativa: LIS  &Ir para o elemento final */

      /* LIS  &Avançar elemento */

         else if ( strcmp( ComandoTeste , OBTER_TAM_CMD ) == 0 )
         {
			int retorno;
            numLidos = LER_LerParametros( "iii" , &parmInt[0] , &parmInt[1] ,
                                &debugEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( parmInt[0] , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

			debug = LSTC_ObterTamanhoListaC(vtListas[parmInt[0]], &retorno);
			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperade em ObterTamanhoListaC");
			if (CondRet)	return CondRet;

            return TST_CompararInt( parmInt[1] , retorno,
                      "Tamanho de lista obtido inesperado" ) ;

         } /* fim ativa: LIS  &Avançar elemento */

      return TST_CondRetNaoConhec ;

   } /* Fim função: TLIS &Testar lista */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TLIS -Destruir valor
*
***********************************************************************/

   void DestruirValor( void * pValor )
   {

      free( pValor ) ;

   } /* Fim função: TLIS -Destruir valor */


/***********************************************************************
*
*  $FC Função: TLIS -Validar indice de lista
*
***********************************************************************/

   int ValidarInxLista( int inxLista , int Modo )
   {

      if ( ( inxLista <  0 )
        || ( inxLista >= DIM_VT_LISTA ))
      {
         return FALSE ;
      } /* if */
      if ( Modo == IRRELEVANTE )
		return TRUE;
      if ( Modo == VAZIO )
      {
         if ( vtListas[ inxLista ] != 0 )
         {
            return FALSE ;
         } /* if */
      } else
      {
         if ( vtListas[ inxLista ] == 0 )
         {
            return FALSE ;
         } /* if */
      } /* if */
         
      return TRUE ;

   } /* Fim função: TLIS -Validar indice de lista */

/********** Fim do módulo de implementação: TLIS Teste lista de símbolos **********/

