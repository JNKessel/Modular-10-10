/***************************************************************************
*  $MCI Módulo de implementação: TTAB Teste lista de símbolos
*
*  Arquivo gerado:              TesteTabuleiro.c
*  Letras identificadoras:      TTAB
*
*  Autores: rrc
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     1       rrc   19/out/2016 desenvolvimento
*	  2		  rrc	19/dez/2016 correção de funções para adequar a mudanças 
*								radicais em módulo tabuleiro
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#include    "Tabuleiro.h"


static const char RESET_VETOR_CMD			 [ ] = "=resetteste"	;
static const char CRIAR_TABULEIRO_CMD		 [ ] = "=cria"			;
static const char DESTRUIR_TABULEIRO_CMD	 [ ] = "=destroi"		;
static const char DISPONIBILIDADE_CASA_CMD	 [ ] = "=disponivel"	;
static const char RETORNAR_CASA_CMD			 [ ] = "=retornacasa"	;
static const char RETORNAR_CASA_SAIDA_CMD	 [ ] = "=casasaida"		;
static const char EH_CASA_FINAL_CMD			 [ ] = "=ehcasafinal"	;

#define TRUE  1
#define FALSE 0

#define VAZIO     0
#define NAO_VAZIO 1
#define IRRELEVANTE 2

#define DIM_VT_LISTA   10
#define DIM_VALOR     100

TAB_tppCasa			vtCasas[ DIM_VT_LISTA ] ;

/***** Protótipos das funções encapuladas no módulo *****/

   static int ValidarInxVecCasas( int inxLista , int Modo ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TTAB &Testar tabuleiro
*
*  $ED Descrição da função
*     Podem ser criados até 10 tabuleiros, identificados pelos índices 0 a 10
*	  Algumas casas serão criadas, também identificadas por índices 0 a 10
*
*     Comandos disponíveis:
*
*     =resetteste
*           - anula o vetor de listas. Provoca vazamento de memória
*     =cria       inxLista
*     =destroi    inxLista
*     =disponivel	 inxLista  inxRet	
*     =retornacasa	         inxLista  iPos		inxChar		CondRetEsp
*     =casasaida		 inxLista  iPos		CondRetEsp
*     =ehcasafinal		 inxLista  iPos		inxChar
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int numLidos   = -1 ,
          debugEsp = -1  ;

	  TST_tpCondRet CondRet;
      TAB_tpCondRet debug;

	  int parmInt[10];
	  char parmChar[10];

	  //vtCasas[0] = {};

      int i ;

      /* Efetuar reset de teste de lista */

         if ( strcmp( ComandoTeste , RESET_VETOR_CMD ) == 0 )
         {

            for( i = 0 ; i < DIM_VT_LISTA ; i++ )
            {
               vtCasas[ i ] = NULL ;
            } /* for */

            return TST_CondRetOK ;

         } /* fim ativa: Efetuar reset de teste de lista */

   

         else if ( strcmp( ComandoTeste , CRIAR_TABULEIRO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
										&debugEsp) ;

            if (( numLidos != 1 ))
            {
               return TST_CondRetParm ;
            } /* if */

            debug = TAB_CriarTabuleiro();

			return TST_CompararInt(debugEsp, debug, "Condicao de retorno inesperada em CriarTabuleiro");

         }

         else if ( strcmp( ComandoTeste , DESTRUIR_TABULEIRO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
										&debugEsp);

            if (( numLidos != 1))
            {
               return TST_CondRetParm ;
            } /* if */

            debug = TAB_DestruirTabuleiro();

			return TST_CompararInt(debugEsp, debug, "Condicao de Retorno inesperado em teste DestruirTabuleiro");

         }

    

         else if ( strcmp( ComandoTeste , DISPONIBILIDADE_CASA_CMD ) == 0 )
         {
			/*FUNÇÃO NÃO É DO T2- NÃO IMPLEMENTADA!!!*/
			 return TST_CondRetParm ;
         }


         else if ( strcmp( ComandoTeste , RETORNAR_CASA_CMD ) == 0 )
         {
			/*FUNÇÃO NÃO É DO T2- NÃO IMPLEMENTADA!!!*/
			 return TST_CondRetParm ;

         } 

         else if ( strcmp( ComandoTeste , RETORNAR_CASA_SAIDA_CMD ) == 0 )
         {
			TAB_tppCasa retorno;
            numLidos = LER_LerParametros( "iii" ,
										&parmInt[1] , &parmInt[2], &debugEsp ) ;

            if (( numLidos != 3 ) || (!ValidarInxVecCasas( parmInt[2] , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */

			debug = TAB_RetornarCasaDeSaida(parmInt[1], &retorno);
			CondRet = TST_CompararInt( debugEsp, debug ,
                     "Condicao de retorno errada ao retornar casa de saida.") ;
			if (CondRet)	return CondRet;

            return TST_CompararPonteiro(vtCasas[parmInt[2]], retorno ,
                     "Retorno inesperado de casa de saida.") ;

         }

         else if ( strcmp( ComandoTeste , EH_CASA_FINAL_CMD ) == 0 )
         {
			DEF_tpBool retorno;
            numLidos = LER_LerParametros( "iii" ,
                  &parmInt[0] , &parmInt[1], &debugEsp) ;

            if ( ( numLidos != 3 ) || (!ValidarInxVecCasas( parmInt[0] , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */

			debug = TAB_EhCasaFinal(vtCasas[parmInt[0]], &retorno);
			CondRet = TST_CompararInt( debugEsp , debug,
				"Condição de retorno errada em EhCasaFinal.") ;
			if (CondRet)	return CondRet;

            return TST_CompararInt( parmInt[1] , retorno,
				"Retorno inesperado de EhCasaFinal.") ;

         }

      return TST_CondRetNaoConhec ;

   }


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TTAB -Validar indice de vetor de casas
*
***********************************************************************/

   int ValidarInxVecCasas( int inxLista , int Modo )
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
         if ( vtCasas[ inxLista ] != 0 )
         {
            return FALSE ;
         } /* if */
      } else
      {
         if ( vtCasas[ inxLista ] == 0 )
         {
            return FALSE ;
         } /* if */
      } /* if */
         
      return TRUE ;

   } /* Fim função: TTAB -Validar indice de vetor de casas */

/********** Fim do módulo de implementação: TLIS Teste lista de símbolos **********/

