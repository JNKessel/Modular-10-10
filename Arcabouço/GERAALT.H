#if ! defined( GERAALT_ )
#define GERAALT_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: ALT  Gerador de n�meros aleat�rios
*
*  Arquivo gerado:              GERAALT.h
*  Letras identificadoras:      ALT
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: C:\AUTOTEST\PROJETOS\ARCABOUC.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     4       avs   01/fev/2006 criar linguagem script simb�lica
*     3       avs   08/dez/2004 uniformiza��o dos exemplos
*     2       avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1       avs   16/abr/2003 in�cio desenvolvimento
*
*  $ED Descri��o do m�dulo
*     Implementa um pequeno conjunto de geradores de n�meros aleat�rios.
*
*  $EIM Interface com outros m�dulos
*     Antes de gerar qualquer n�mero deve ser chamada a fun��o
*     ALT_Inicializar.
*
***************************************************************************/
 
#if defined( GERAALT_OWN )
   #define GERAALT_EXT
#else
   #define GERAALT_EXT extern
#endif


/***********************************************************************
*
*  $FC Fun��o: ALT  &Inicializar o gerador de n�meros aleat�rios
*
*  $ED Descri��o da fun��o
*     Inicializa o gerador de n�meros aleat�rios.
*     Esta fun��o deve ser chamada antes de qualquer outra do m�dulo GERAALT
*
*  $EP Par�metros
*     Base - se == 0 utiliza a semente normal (PI 3141592653 )
*                  isto gerar� a mesma seq��ncia de n�meros aleat�rios
*                  a seguir da inicializa��o. Esta semente � tida como
*                  uma boa semente, a distribui��o � virtualmente uniforme
*                  e a cardinalidade � a maior poss�vel.
*            se == 1 utiliza a PI | time( )
*                  isto gera uma seq��ncia de n�meros aleat�rios
*                  diferente a cada inicializa��o
*            outros - utiliza o valor fornecido como semente
*                  deve ser tomado cuidado pois uma semente mal escolhida
*                  pode levar � gera��o de uma seq��ncia n�o aleat�ria,
*                  ou de cardinalidade limitada.
*
*  $ER Requisitos
*     Esta fun��o deve ser chamada uma �nica vez e isso antes de gerar
*     o primeiro n�mero aleat�rio
*
***********************************************************************/

   void ALT_Inicializar( unsigned int Base ) ;


/***********************************************************************
*
*  $FC Fun��o: ALT  &Gerar inteiro em uniformemente distribu�do
*
*  $ED Descri��o da fun��o
*     Gerar um n�mero inteiro uniformemente distribu�do em um
*     intervalo dado
*
*  $EP Par�metros
*     $P Inf - limite inferior, inclusive
*     $P Sup - limite superior, inclusive
*
*  $FV Valor retornado
*     Inf <= x <= Sup
*
*  $EAE Assertivas de entradas esperadas
*     Inf <= Sup
*     Dados errados provocar�o um comportamento err�tico no programa.
*
***********************************************************************/

   int ALT_GerarDistUniforme( int Inf , int Sup ) ;


/***********************************************************************
*
*  $FC Fun��o: ALT  &Gerar distribui��o dada por tabela de freq��ncias
*
*  $ED Descri��o da fun��o
*     Gerar distribui��o segundo tabela de freq��ncias
*
*  $EP Par�metros
*     $P numElem - n�mero de elementos (intervalos) da tabela
*                  o n�mero de intervalos � na realidade igual a numElem + 1
*     $P vetElem - vetor de elementos da tabela
*                  Os valores dos elementos s�o n�meros entre 0 e Limite - 1
*                  Os elementos devem estar em ordem crescente.
*                  Os elementos correspondem a uma fun��o cumulativa
*                  Exemplo, se forem desejados as freq��ncias
*                     10% 30% e 60%
*                  devem ser fornecidos os dados 2 {10 , 40} 100
*     $P Limite  - � o limite superior da distribui��o de freq��ncias
*                  Permite chegar a freq��ncias muito pequenas, por exemplo
*                  para uma distribui��o de freq��ncias 9999/10000 e
*                  1/10000 forne�a os par�metros
*                  1 9999 10000
*
*  $FV Valor retornado
*     O �ndice da freq��ncia 0 <= indice <= numElem, com uma probabilidade
*     igual ao correspondente intervalo.
*
*     A probabilidade x � dada por
*         x = ( vetElem[ i ] - vetElem[ i - 1 ] ) / Limite
*         para todos i , 0 <= i <= numElem
*            sendo que vetElem[ -1      ] � assumido igual a 0
*            sendo que vetElem[ numElem ] � assumido igual a Limite
*
*  $EAE Assertivas de entradas esperadas
*     numElem >= 1
*     vetElem[ numElem - 1 ] < Limite
*     para todos i, 0 <= i < numElem : vetElem[ i - 1 ] < vetElem[ i ]
*             sendo que vetElem[ -1 ] � considerado igual a 0
*     Dados errados provocar�o um comportamento err�tico no programa.
*
***********************************************************************/

   int ALT_GerarFrequencia( int numElem , int * vetElem , int Limite ) ;


/***********************************************************************
*
*  $FC Fun��o: ALT  &Gerar permuta��o rand�mica uniformemente distribu�da
*
*  $ED Descri��o da fun��o
*     Gera uma permuta��o de n elementos uniformemente distribu�da.
*     O algoritmo utilizado � o "Knuth shuffle" descrito na Wikipedia.
*
*  $EP Par�metros
*     $P tamVetor - n�mero de elementos a serem embaralhados
*     $P    Vetor - elementos a serem embaralhados. O vetor deve conter
*                   pelo menos tamVetor elementos. Caso n�o contenha, os
*                   resultados ser�o impredit�veis, possivelmente
*                   desastrosos.
*                   O vetor deve estar inicializado com os elementos a
*                   serem permutados. Por exemplo, no caso de um baralho,
*                   o vetor deve ser inicializado com a seq��ncia de
*                   �ndices das cartas (0 a 51).
*
***********************************************************************/

   void ALT_GerarPermutacao( int   tamVetor ,
                             int * Vetor     ) ;

#undef GERAALT_EXT

/********** Fim do m�dulo de defini��o: ALT  Gerador de n�meros aleat�rios **********/

#else
#endif
