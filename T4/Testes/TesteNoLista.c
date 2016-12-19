/***************************************************************************
*  $MCI Módulo de implementação: TNOLST Teste lista de símbolos
*
*  Arquivo gerado:              TesteNoLista.c
*  Letras identificadoras:      TNOLST
*
*  Autores: rrc
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     1       rrc   19/out/2016 desenvolvimento
*	  2		  rrc	19/dez/2016 Adaptei para nova interface de NoLista
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#include    "NoLista.h"


static const char RESET_LISTA_CMD[]				= "=resetteste"		;
static const char CRIAR_NO_CMD[]				= "=cria"			;
static const char DESTRUIR_NO_CMD[]				= "=destroi"		;
static const char COLOCAR_NO_FRENTE_CMD[]		= "=colocafrente"	;
static const char COLOCAR_NO_ATRAS_CMD[]		= "=colocaatras"	;
static const char OBTER_PROX_NO_CMD[]			= "=obterprox"		;
static const char OBTER_NO_PREVIO_CMD[]			= "=obterprevio"	;
static const char ATRIBUIR_INFO_CMD[]			= "=atribinfo"		;
static const char OBTER_INFO_CMD[]				= "=obterinfo"		;
static const char LIGAR_NOS_CMD[]				= "=ligarnos"		;


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

   static int ValidarInxVecNos( int inxNo , int Modo ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TNOLST &Testar NoLista
*
*  $ED Descrição da função
*     Podem ser criados até 10 nós, identificadas pelos índices 0 a 10
*
*     Comandos disponíveis:
*
*     =resetteste
*           - anula o vetor de nós. Provoca vazamento de memória
*     =criarlistac       inxNo
*     =destruirlistac    inxNo
*     =ehlistacvazia	 inxNo  inxRet	
*     =inselem	         inxNo  iPos		inxChar		CondRetEsp
*     =retirelem		 inxNo  iPos		CondRetEsp
*     =atribuirelem		 inxNo  iPos		inxChar	
*     =obterelem		 inxNo  iPos  		inxCharRet  CondRetEsp
*     =movcorr			 inxNo  iN
*     =procurarelem      inxNo  iPos		pCharBuscado	
*     =obtertamlista     inxNo  pNumElem
*     =ligarNos			 inxNo1 inxNo2		CondRetEsp
***********************************************************************/
static char abc[] = "abcdefghi";


   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int inxNo  = -1 ,
          numLidos   = -1 ,
          debugEsp = -1  ;

	  TST_tpCondRet CondRet;
      NOLST_tpCondRet debug;

	  int parmInt[10];
	  char parmChar[10];

      int i ;

      /* Efetuar reset de teste de nós */

         if ( strcmp( ComandoTeste , RESET_LISTA_CMD ) == 0 )
         {

            for( i = 0 ; i < DIM_VT_LISTA ; i++ )
            {
               vtNos[ i ] = NULL ;
            } /* for */

            return TST_CondRetOK ;

         } /* fim ativa: Efetuar reset de teste de nós */

      /* Testar  NOLST_CriarNoh */

         else if ( strcmp( ComandoTeste , CRIAR_NO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iiiii" ,
                       &inxNo, &parmInt[0], &parmInt[1], &parmInt[2], &debugEsp) ;

            if ( ( numLidos != 5 )
              || ( ! ValidarInxVecNos( inxNo , VAZIO )))
            {
               return TST_CondRetParm ;
            } /* if */

            NOLST_CriarNoh(&vtNos[inxNo], &abc[parmInt[0]], vtNos[parmInt[1]], vtNos[parmInt[2]]) ;

            return TST_CompararPonteiroNulo( 1 , vtNos[ inxNo ] ,
               "Erro em ponteiro de novo noLista."  );

         } /* fim ativa: Testar  NOLST_CriarNoh */

      /* Testar NOLST_DestruirNoh */

         else if ( strcmp( ComandoTeste , DESTRUIR_NO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxNo, &debugEsp);

            if ( ( numLidos != 2)
              || ( ! ValidarInxVecNos( inxNo , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */

            debug = NOLST_DestruirNoh(vtNos[inxNo]);
			vtNos[inxNo] = NULL;

			return TST_CompararInt(debugEsp, debug, "Condicao de Retorno inesperado em teste DestruirNoh");

         } /* fim ativa: Testar NOLST_DestruirNoh */

      /* Testar NOLST_ColocarNohEmFrente */

         else if ( strcmp( ComandoTeste , COLOCAR_NO_FRENTE_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                               &inxNo, &parmInt[0], &debugEsp);

            if ( ( numLidos != 3 )
              || ( ! ValidarInxVecNos( inxNo , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */


            debug = NOLST_ColocarNohEmFrente(vtNos[inxNo], vtNos[parmInt[0]]);
			

            return TST_CompararInt(debugEsp, debug, "Condicao de Retorno inesperado em teste ColocarEmFrente");

         } /* fim ativa: Testar NOLST_ColocarNohEmFrente */

      /* Testar NOLST_ColocarNohAtras */

         else if ( strcmp( ComandoTeste , COLOCAR_NO_ATRAS_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                       &inxNo , &parmInt[0], &debugEsp) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxVecNos( inxNo , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debug = NOLST_ColocarNohAtras( vtNos[ inxNo ] , vtNos[parmInt[0]]);

            return TST_CompararInt( debugEsp , debug ,
                     "Condicao de retorno errada ao em teste ColocarAtras.") ;

         } /* fim ativa: Testar NOLST_ColocarNohAtras */

      /* Testar NOLST_ObterProxNoh */

         else if ( strcmp( ComandoTeste , OBTER_PROX_NO_CMD ) == 0 )
         {
			 NOLST_tppNoLista retorno;

            numLidos = LER_LerParametros( "iii" ,
                       &inxNo , &parmInt[0] , &debugEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxVecNos( inxNo , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debug = NOLST_ObterProxNoh( vtNos[ inxNo ] , &retorno);	/* if */

			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em teste ObterProxNoh");
			if (CondRet)	return CondRet;

			return TST_CompararPonteiro( vtNos[parmInt[0]] , retorno ,
                     "Retorno inesperado de ponteiro para no"                   ) ;

         } /* fim ativa: Testar NOLST_ObterProxNoh */

      /* Testar NOLST_ObterNohPrevio */

         else if ( strcmp( ComandoTeste , OBTER_NO_PREVIO_CMD ) == 0 )
         {
			 NOLST_tppNoLista retorno;

            numLidos = LER_LerParametros( "iii" ,
                  &inxNo , &parmInt[0], &debugEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxVecNos( inxNo , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

			debug = NOLST_ObterNohPrevio(vtNos[inxNo], &retorno);

			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em teste ObterNohPrevio");
			if (CondRet)	return CondRet;

			return TST_CompararPonteiro( vtNos[parmInt[0]] , retorno,
                     "Retorno inesperado de ponteiro para no"   ) ;

         } /* fim ativa: Testar NOLST_ObterNohPrevio */

      /* Testar NOLST_AtribuirInfoNoh */

         else if ( strcmp( ComandoTeste , ATRIBUIR_INFO_CMD ) == 0 )
         {
            numLidos = LER_LerParametros( "iii" ,
                       &inxNo , &parmInt[0] , &debugEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxVecNos( inxNo , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

			debug = NOLST_AtribuirInfoNoh(vtNos[inxNo], &abc[parmInt[0]]);

            return TST_CompararInt( debugEsp , debug ,
                         "Condicao de retono inesperado de AtribuirInfoNoh" ) ;

         } /* fim ativa: Testar NOLST_AtribuirInfoNoh */

      /* Testar NOLST_ObterInfoNoh */

         else if ( strcmp( ComandoTeste , OBTER_INFO_CMD ) == 0 )
         {
			 char* obtido;

            numLidos = LER_LerParametros( "ici" , &inxNo, &parmChar[0], &debugEsp) ;

            if ( ( numLidos != 3)
              || ( ! ValidarInxVecNos( inxNo , IRRELEVANTE )) )
            {
               return TST_CondRetParm;
            } /* if */

			debug = NOLST_ObterInfoNoh(vtNos[inxNo], (void**)&obtido) ;
			CondRet = TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em ObterInfoNoh");
			if (CondRet)	return CondRet;

            return TST_CompararChar(parmChar[0], *obtido, "Retorno inesperado de caracter por ObterInfoNoh");

         } /* fim ativa: Testar NOLST_ObterInfoNoh */

		/* Testar NOLST_LigarNos */
		 
		 else if ( strcmp( ComandoTeste , LIGAR_NOS_CMD ) == 0 )
         {
            numLidos = LER_LerParametros( "iii" , &inxNo, &parmInt[0], &debugEsp) ;

            if ( ( numLidos != 3)
              || ( ! ValidarInxVecNos( inxNo , IRRELEVANTE )) || ( ! ValidarInxVecNos( parmInt[0] , IRRELEVANTE )) )
            {
               return TST_CondRetParm;
            } /* if */

			debug = NOLST_LigarNos(vtNos[inxNo], vtNos[parmInt[0]]) ;

			return TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em LigarNos");
         } 
		 
		 /* fim ativa: Testar NOLST_ObterInfoNoh */

      /* NOLST  &Ir para o elemento final */

      return TST_CondRetNaoConhec;

   } /* Fim função: TNOLST &Testar lista */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TNOLST -Validar indice de vetor de nós
*
***********************************************************************/

   int ValidarInxVecNos( int inxNo , int Modo )
   {

      if ( ( inxNo <  0 )
        || ( inxNo >= DIM_VT_LISTA ))
      {
         return FALSE ;
      } /* if */
      if ( Modo == IRRELEVANTE )
		return TRUE;
      if ( Modo == VAZIO )
      {
         if ( vtNos[ inxNo ] != 0 )
         {
            return FALSE ;
         } /* if */
      } else
      {
         if ( vtNos[ inxNo ] == 0 )
         {
            return FALSE ;
         } /* if */
      } /* if */
         
      return TRUE ;

   } /* Fim função: TNOLST -Validar indice de vetor de nós */

/********** Fim do módulo de implementação: TNOLST Teste lista de símbolos **********/