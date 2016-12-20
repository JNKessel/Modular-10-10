/***************************************************************************
*  $MCI Módulo de implementação: TPEAO Teste lista de símbolos
*
*  Arquivo gerado:              TestPEAO.c
*  Letras identificadoras:      TPEAO
*
*  Autores: rrc
*
*  $HA Histórico de evolução:
*		Versão		Autor		Data		Observações
*		1			rrc			28/11		Implementação geral
*		2			rrc			20/12		acrescentei função faltante
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#include    "Peao.h"
#include	"Definicoes.h"


static const char RESET_VETOR_CMD		[ ] = "=resetteste"		;
static const char CRIAR_PEAO_CMD		[ ] = "=criarpeao"		;
static const char DESTRUIR_PEAO_CMD		[ ] = "=destruirpeao"	;
static const char ANDAR_PEAO_CMD		[ ] = "=andarpeao"		;
static const char SAIR_BASE_CMD			[ ] = "=sairbase"		;
static const char ESTA_FINAL_CMD		[ ] = "=estafinal"		;
static const char ESTA_BASE_CMD         [ ] = "=estabase"		;
static const char OBTER_NUM_CMD         [ ] = "=obternum"		;
static const char OBTER_CASA_CMD        [ ] = "=obtercasa"		;
static const char CHECAR_DIPON_CMD		[ ] = "=checardispon"	;
static const char VOLTAR_BASE_CMD		[ ] = "=voltarbase"		;


#define TRUE  1
#define FALSE 0

#define VAZIO     0
#define NAO_VAZIO 1
#define IRRELEVANTE 2

#define DIM_VT_PEAO   10
#define DIM_VALOR     100

static PEAO_tppPeao	vtPeoes[ DIM_VT_PEAO ] ;
static DEF_tpCor	vtCores[4] = {AZUL,AMARELO,VERDE,VERMELHO};
static DEF_tpBool	vtBools[2] = {False, True};

/***** Protótipos das funções encapuladas no módulo *****/

   static int ValidarInxPeao( int inxPeao , int Modo ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***********************************************************************
*
*  $FC Função: TPEAO &Testar peao
*
*  $ED Descrição da função
*     Podem ser criados até 10 peões, identificados pelos índices 0 a 10
*
*     Comandos disponíveis:
*
*     =resetteste
*           - anula o vetor de listas. Provoca vazamento de memória
*     =criarpeao					inxPeao	inxCor	num  CondRetEsp
*     =destruirpeao					inxPeao CondRetEsp
*     =andarpeao					inxPeao	num		CondRetEsp
*     =sairbase						inxPeao	CondRetEsp
*     =estafinal					inxPeao	boolRetEsp	CondRetEsp
*     =estabase						inxPeao	boolRetEsp	CondRetEsp
*     =obternum						inxPeao	numRetEsp	CondRetEsp
*     =checardispon					inxPeao	numDado	boolRetEsp	CorRetEsp	CondRetEsp
*     =voltarbase					inxPeao	CondRetEsp
*	  =obtercasa					inxPeao	inxCasa	CondRetEsp
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int inxPeao  = -1 ,
		  inxCor = -1,
		  inxBool = -1,
          numLidos   = -1 ,
          CondRetEsp = -1  ;

	  PEAO_tpCondRet debugRet;

      TST_tpCondRet CondRet ;

	  int parmInt[10];

      int i ;

      /* Efetuar reset de teste de vetor de peoes */

         if ( strcmp( ComandoTeste , RESET_VETOR_CMD ) == 0 )
         {
            for( i = 0 ; i < DIM_VT_PEAO ; i++ )
            {
               vtPeoes[ i ] = NULL ;
            } /* for */

            return TST_CondRetOK ;

         } /* fim ativa: Efetuar reset de teste de vetor de peoes */

      /* Testar CriarPeao */

         else if ( strcmp( ComandoTeste , CRIAR_PEAO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iiii",
                       &inxPeao, &inxCor, &parmInt[0], &CondRetEsp ) ;

            if ( ( numLidos != 4 )
              || ( ! ValidarInxPeao( inxPeao , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */

            debugRet = PEAO_CriarPeao( &vtPeoes[ inxPeao ], vtCores[inxCor], parmInt[0] ) ;
			
			CondRet = TST_CompararInt(CondRetEsp, debugRet, "Erro retorno inesperado ao criar peao");
			if (CondRet) {
				return CondRet;
			}
			
			return TST_CompararPonteiroNulo( 1 , vtPeoes[ inxPeao ] ,
               "Erro em ponteiro de nova lista."  ) ;

         } /* fim ativa: Testar CriarPeao */

      /* Testar DestruirPeao */

         else if ( strcmp( ComandoTeste , DESTRUIR_PEAO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxPeao, &CondRetEsp ) ;

            if ( ( numLidos != 2 )
              || ( ! ValidarInxPeao( inxPeao , NAO_VAZIO )))
            {
               return TST_CondRetParm ;
            } /* if */

            debugRet = PEAO_DestruirPeao( vtPeoes[inxPeao] ) ;

            return TST_CompararInt( CondRetEsp , debugRet ,
                     "Condicao de retorno diferente do esperado.") ;

         } /* fim ativa: Testar DestruirPeao */

      /* Testar AndarPeao */

         else if ( strcmp( ComandoTeste , ANDAR_PEAO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                               &inxPeao, &parmInt[0], &CondRetEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxPeao( inxPeao , IRRELEVANTE )))
            {
               return TST_CondRetParm ;
            } /* if */

            debugRet = PEAO_AndarPeao( vtPeoes[ inxPeao ], parmInt[0] ) ;

            return TST_CompararInt( CondRetEsp , debugRet ,
                     "Condicao de retorno diferente do esperado.") ;

         } /* fim ativa: Testar AndarPeao */

      /* Testar SairBasePeao */

         else if ( strcmp( ComandoTeste , SAIR_BASE_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                       &inxPeao , &CondRetEsp ) ;

            if ( ( numLidos != 2 )
              || ( ! ValidarInxPeao( inxPeao , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debugRet = PEAO_SairBasePeao( vtPeoes[ inxPeao ] ) ;

            return TST_CompararInt( CondRetEsp , debugRet ,
                     "Condicao de retorno inesperada.") ;

         } /* fim ativa: Testar SairBasePeao */

      /* Testar EstaPeaoFinal */

         else if ( strcmp( ComandoTeste , ESTA_FINAL_CMD ) == 0 )
         {
			 DEF_tpBool boolRet;

            numLidos = LER_LerParametros( "iii" ,
                       &inxPeao , &inxBool , &CondRetEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxPeao( inxPeao , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debugRet = PEAO_EstaPeaoFinal( vtPeoes[ inxPeao ] , &boolRet ) ;

			CondRet = TST_CompararInt( vtBools[inxBool] , boolRet ,
                     "Retorno de booleano inesperado.") ;

			if (CondRet != TST_CondRetOK)
				return CondRet;

            return TST_CompararInt( CondRetEsp , debugRet ,
                     "Condicao de retorno inesperada.") ;

         } /* fim ativa: Testar EstaPeaoFinal */

      /* Testar EstaPeaoBase */

         else if ( strcmp( ComandoTeste , ESTA_BASE_CMD ) == 0 )
         {
			DEF_tpBool boolRet;

            numLidos = LER_LerParametros( "iii" ,
					 &inxPeao, &inxBool, &CondRetEsp);

            if ( ( numLidos != 3 )
              || ( ! ValidarInxPeao( inxPeao , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

			debugRet = PEAO_EstaPeaoBase( vtPeoes[ inxPeao ], &boolRet );

			CondRet = TST_CompararInt( boolRet, vtBools[inxBool],
                     "Retorno booleano de funcao errado."   ) ;

			if (CondRet != TST_CondRetOK)
				return CondRet;

            return TST_CompararInt( CondRetEsp , debugRet,
							"Condição de retorno errada.");

         } /* fim ativa: Testar EstaPeaoBase */

      /* Testar ObterNumeroPeao */

         else if ( strcmp( ComandoTeste , OBTER_NUM_CMD ) == 0 )
         {
			int numRet;

            numLidos = LER_LerParametros( "iii" ,
                       &inxPeao , &parmInt[0] , &CondRetEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxPeao( inxPeao , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debugRet = PEAO_ObterNumeroPeao( vtPeoes[ inxPeao ], &numRet) ;

            CondRet = TST_CompararInt( parmInt[0], numRet,
                     "Retorno de numero de funcao errado."   ) ;

			if (CondRet != TST_CondRetOK)
				return CondRet;

            return TST_CompararInt( CondRetEsp , debugRet,
							"Condição de retorno errada.");

         } /* fim ativa: Testar ObterNumeroPeao */

		 /* Testar ObterCasaPeao */

         else if ( strcmp( ComandoTeste , OBTER_CASA_CMD ) == 0 )
         {
			 TAB_tppCasa casa;

            numLidos = LER_LerParametros( "iii" ,
                       &inxPeao , &parmInt[0] , &CondRetEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxPeao( inxPeao , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debugRet = PEAO_ObterCasaPeao( vtPeoes[ inxPeao ], &casa) ;

            return TST_CompararInt( CondRetEsp , debugRet,
							"Condição de retorno errada.");

         } /* fim ativa: Testar ObterCasaPeao */

      /* Testar ChecarMovimentoDisponivelPeao */

         else if ( strcmp( ComandoTeste , CHECAR_DIPON_CMD ) == 0 )
         {
			 DEF_tpBool boolRet;
			 DEF_tpCor corRet;

            numLidos = LER_LerParametros( "iiiii" ,
					&inxPeao, &parmInt[0], &inxBool, &inxCor, &CondRetEsp ) ;

            if ( ( numLidos != 5 )
              || ( ! ValidarInxPeao( inxPeao , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debugRet = PEAO_ChecarMovimentoDisponivelPeao( vtPeoes[ inxPeao ], parmInt[0], &boolRet, &corRet ) ;

			CondRet = TST_CompararInt( vtBools[inxBool] , boolRet,
                     "Retorno de booleano de funcao errado."   ) ;

			if (CondRet != TST_CondRetOK)
				return CondRet;

			CondRet = TST_CompararInt( vtCores[inxCor] , corRet,
                     "Retorno de cor de funcao errado."   ) ;

			if (CondRet != TST_CondRetOK)
				return CondRet;

            return TST_CompararInt( CondRetEsp , debugRet,
							"Condição de retorno errada.");

         } /* fim ativa: Testar ChecarMovimentoDisponivelPeao */

      /* Testar VoltarBasePeao */

         else if ( strcmp( ComandoTeste , VOLTAR_BASE_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" , &inxPeao, &CondRetEsp ) ;

            if ( ( numLidos != 2 )
              || ( ! ValidarInxPeao( inxPeao , IRRELEVANTE )) )
            {
               return TST_CondRetParm ;
            } /* if */

            debugRet = PEAO_VoltarBasePeao( vtPeoes[ inxPeao ] ) ;

            return TST_CompararInt( CondRetEsp , debugRet,
							"Condição de retorno errada.");

         } /* fim ativa: VoltarBasePeao */

      return TST_CondRetNaoConhec ;

   } /* Fim função: PEAO &Testar Peao */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TPEAO -Validar indice de peao
*
***********************************************************************/

   int ValidarInxPeao( int inxPeao , int Modo )
   {

      if ( ( inxPeao <  0 )
        || ( inxPeao >= DIM_VT_PEAO ))
      {
         return FALSE ;
      } /* if */
      if ( Modo == IRRELEVANTE )
		return TRUE;
      if ( Modo == VAZIO )
      {
         if ( vtPeoes[ inxPeao ] != NULL )
         {
            return FALSE ;
         } /* if */
      } else
      {
         if ( vtPeoes[ inxPeao ] == NULL )
         {
            return FALSE ;
         } /* if */
      } /* if */
         
      return TRUE ;

   } /* Fim função: TPEAO -Validar indice de peao */

/********** Fim do módulo de implementação: TPEAO Teste lista de símbolos **********/