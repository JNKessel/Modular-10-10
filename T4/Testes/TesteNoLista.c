/***************************************************************************
*  $MCI Módulo de implementação: TNOLST Teste lista de símbolos
*
*  Arquivo gerado:              TesteNoLista.c
*  Letras identificadoras:      TNOLST
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

#include    "NoLista.h"


static const char RESET_LISTA_CMD		 [ ] = "=resetteste"		;
static const char CRIAR_NO_CMD		 [ ] = "=cria"					;
static const char DESTRUIR_NO_CMD	 [ ] = "=destroi"				;
static const char COLOCAR_NO_FRENTE_CMD	 [ ] = "=colocafrente"		;
static const char COLOCAR_NO_ATRAS_CMD			 [ ] = "=colocaatras";
static const char OBTER_PROX_NO_CMD         [ ] = "=obterprox"		;
static const char OBTER_NO_PREVIO_CMD		 [ ] = "=obterprevio"	;
static const char ATRIBUIR_INFO_CMD		 [ ] = "=atribinfo"			;
static const char OBTER_INFO_CMD			 [ ] = "=obterinfo"		;


#define TRUE  1
#define FALSE 0

#define VAZIO     0
#define NAO_VAZIO 1
#define IRRELEVANTE 2

#define DIM_VT_LISTA   10
#define DIM_VALOR     100

NOLST_tppNoLista	vtNos[ DIM_VT_LISTA ] ;

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
static char abc[] = "abcdefghi";


   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int inxLista  = -1 ,
          numLidos   = -1 ,
          debugEsp = -1  ;

	  TST_tpCondRet CondRet;
      NOLST_tpCondRet debug;

	  int parmInt[10];
	  char parmChar[10];

      int i ;

      /* Efetuar reset de teste de lista */

         if ( strcmp( ComandoTeste , RESET_LISTA_CMD ) == 0 )
         {

            for( i = 0 ; i < DIM_VT_LISTA ; i++ )
            {
               vtNos[ i ] = NULL ;
            } /* for */

            return TST_CondRetOK ;

         } /* fim ativa: Efetuar reset de teste de lista */

      /* Testar CriarLista */

         else if ( strcmp( ComandoTeste , CRIAR_NO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iiiii" ,
                       &inxLista, &parmInt[0], &parmInt[1], &parmInt[2], &debugEsp) ;

            if ( ( numLidos != 5 )
              || ( ! ValidarInxLista( inxLista , VAZIO )))
            {
               return TST_CondRetParm ;
            } /* if */

            NOLST_CriarNoh(&vtNos[inxLista], &abc[parmInt[0]], vtNos[parmInt[1]], vtNos[parmInt[2]]) ;

            return TST_CompararPonteiroNulo( 1 , vtNos[ inxLista ] ,
               "Erro em ponteiro de novo noLista."  );

         } /* fim ativa: Testar CriarLista */

      /* Testar Esvaziar lista lista */

         else if ( strcmp( ComandoTeste , DESTRUIR_NO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxLista, &debugEsp);

            if ( ( numLidos != 2)
              || ( ! ValidarInxLista( inxLista , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */

            debug = NOLST_DestruirNoh(vtNos[inxLista]);
			vtNos[inxLista] = NULL;

			return TST_CompararInt(debugEsp, debug, "Condicao de Retorno inesperado em teste DestruirNoh");

         } /* fim ativa: Testar Esvaziar lista lista */

      /* Testar Destruir lista */

         else if ( strcmp( ComandoTeste , COLOCAR_NO_FRENTE_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                               &inxLista, &parmInt[0], &debugEsp);

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( inxLista , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */


            debug = NOLST_ColocarNohEmFrente(vtNos[inxLista], vtNos[parmInt[0]]);
			

            return TST_CompararInt(debugEsp, debug, "Condicao de Retorno inesperado em teste ColocarEmFrente");

         } /* fim ativa: Testar Destruir lista */

      /* Testar inserir elemento antes */

         else if ( strcmp( ComandoTeste , COLOCAR_NO_ATRAS_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                       &inxLista , &parmInt[0], &debugEsp) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( inxLista , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debug = NOLST_ColocarNohAtras( vtNos[ inxLista ] , vtNos[parmInt[0]]);

            return TST_CompararInt( debugEsp , debug ,
                     "Condicao de retorno errada ao em teste ColocarAtras.") ;

         } /* fim ativa: Testar inserir elemento antes */

      /* Testar inserir elemento apos */

         else if ( strcmp( ComandoTeste , OBTER_PROX_NO_CMD ) == 0 )
         {
			 NOLST_tppNoLista retorno;

            numLidos = LER_LerParametros( "iii" ,
                       &inxLista , &parmInt[0] , &debugEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( inxLista , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debug = NOLST_ObterProxNoh( vtNos[ inxLista ] , &retorno);	/* if */

			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em teste ObterProxNoh");
			if (CondRet)	return CondRet;

			return TST_CompararPonteiro( vtNos[parmInt[0]] , retorno ,
                     "Retorno inesperado de ponteiro para no"                   ) ;

         } /* fim ativa: Testar inserir elemento apos */

      /* Testar excluir simbolo */

         else if ( strcmp( ComandoTeste , OBTER_NO_PREVIO_CMD ) == 0 )
         {
			 NOLST_tppNoLista retorno;

            numLidos = LER_LerParametros( "iii" ,
                  &inxLista , &parmInt[0], &debugEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( inxLista , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

			debug = NOLST_ObterNohPrevio(vtNos[inxLista], &retorno);

			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em teste ObterNohPrevio");
			if (CondRet)	return CondRet;

			return TST_CompararPonteiro( vtNos[parmInt[0]] , retorno,
                     "Retorno inesperado de ponteiro para no"   ) ;

         } /* fim ativa: Testar excluir simbolo */

      /* Testar obter valor do elemento corrente */

         else if ( strcmp( ComandoTeste , ATRIBUIR_INFO_CMD ) == 0 )
         {
			void* retorno;

            numLidos = LER_LerParametros( "iii" ,
                       &inxLista , &parmInt[0] , &debugEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( inxLista , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

			debug = NOLST_AtribuirInfoNoh(vtNos[inxLista], &abc[parmInt[0]]);

            return TST_CompararInt( debugEsp , debug ,
                         "Condicao de retono inesperado de AtribuirInfoNoh" ) ;

         } /* fim ativa: Testar obter valor do elemento corrente */

      /* Testar ir para o elemento inicial */

         else if ( strcmp( ComandoTeste , OBTER_INFO_CMD ) == 0 )
         {
			 char* obtido;

            numLidos = LER_LerParametros( "ici" , &inxLista, &parmChar[0], &debugEsp) ;

            if ( ( numLidos != 3)
              || ( ! ValidarInxLista( inxLista , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

			debug = NOLST_ObterInfoNoh(vtNos[inxLista], (void**)&obtido) ;
			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em ObterInfoNoh");
			if (CondRet)	return CondRet;

            return TST_CompararChar(parmChar[0], *obtido, "Retorno inesperado de caracter por ObterInfoNoh");

         } /* fim ativa: Testar ir para o elemento inicial */

      /* LIS  &Ir para o elemento final */

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
         if ( vtNos[ inxLista ] != 0 )
         {
            return FALSE ;
         } /* if */
      } else
      {
         if ( vtNos[ inxLista ] == 0 )
         {
            return FALSE ;
         } /* if */
      } /* if */
         
      return TRUE ;

   } /* Fim função: TLIS -Validar indice de lista */

/********** Fim do módulo de implementação: TLIS Teste lista de símbolos **********/

