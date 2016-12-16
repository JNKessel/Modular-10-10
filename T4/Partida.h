/*******************************************************************************************************************************
*	$MCD Módulo de definição: Módulo Partida
*	Arquivo:	PARTIDA.H
*	Letras identificadoras:		PART
*
*	Autores:	rrc - Rafael Rubim Cabral
*
*	$HA Histórico de evolução:
*		Versâo	Autor	Data		Observações
*		1.00	rrc		17/10/2016	Começo de implementação, maioria das funções ainda não implementadas
*		1.10	rrc		09/12/2016	Documentação de funções
*		2.00	rrc		13/12/2016	Implementação de outras funções
*		3.00	rrc		14/12/2016	Fim das implementações
*		4.00	rrc		15/12/2016	Adições ao módulo, correção de bugs
*
*	$ED Descrição do módulo:
*		Este módulo permite a criação de uma partida a partir de sua quantidade de jogadores e manipulação de cada turno através
*		da chamada da função Jogar.
*		Não pode ser criada mais de uma partida simultaneamente.
*******************************************************************************************************************************/

#pragma once

#include "Definicoes.h"

/*******************************************************************************************************************************
*	$TC Tipo de dados: PART Condições de retorno
*
*	$ED Descrição do tipo:
*		Condições de retorno das funções da partida	
*******************************************************************************************************************************/
typedef enum {
	PART_CondRetOK,
		/* Concluiu corretamente */

	PART_CondRetSemMemoria,
		/* Faltou memória em alguma operação de alocação */

	PART_CondRetErroListaC,
		/* Erro em uso de função de módulo LSTC */

	PART_CondRetErroPeao,
		/* Erro em uso de função de módulo PEAO */

	PART_CondRetErroLista,
		/* Erro em uso de função de módulo LIS */

	PART_CondRetErroTabuleiro,
		/* Erro em uso de função de módulo TAB */

	PART_CondRetNumInvalido,
		/* Número de jogadores especificado não é válido */

	PART_CondRetPartidaInexistente,
		/* Tentativa de operação sobre partida que não foi criada ou já foi destruída */

	PART_CondRetNinguemJogou,
		/* Tentativa de operação sobre jogador que jogou último turno, enquanto ninguém ainda jogou */

	PART_CondRetInconsistencia,
		/* Inconsistência interna estrutural ou assertiva */
} PART_tpCondRet;

/*******************************************************************************************************************************
*	$FC Função: PART_CriarPartida
*
*	$ED Descrição da função:
*		Recebe quantos jogadores participarão da partida e a cria. O mínimo de jogadores é 2 e o máximo é 4. A cor de cada
*		jogador obedece sempre a seguinte ordem: 1º- AZUL, 2º- VERMELHO, 3º- VERDE, 4º- AMARELO.
*
*	$EP Parâmetros:
*		$P iNumJogadores	-	número de jogadores participantes
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetSemMemoria
*		PART_CondRetNumInvalido
*		PART_CondRetErroListaC
*		PART_CondRetErroLista
*		PART_CondRetErroPeao
*******************************************************************************************************************************/
PART_tpCondRet PART_CriarPartida(int iNumJogadores);

/*******************************************************************************************************************************
*	$FC Função: PART_Jogar
*
*	$ED Descrição da função:
*		Executa um turno do jogador de vez atual. Isso envolve as seguintes etapas:
*		- Através da interface do console, anuncia a cor do jogador de vez atual
*		- Através da interface do console, pede o jogador para pressionar qualquer tecla para jogar os dados.
*		- Através da interface do console, apresenta o número sorteado nos dados.
*		- Caso nenhum peão do jogador possa ser movimentado, encerra sua jogada (avisando o jogador pela interface).
*		- Apresenta para o jogador o número dos peões que pode movimentar e o pede para escolher um deles pelo teclado.
*		- Torna a pedir um número, caso o digitado seja inválido.
*		- Anda com o peão escolhido, voltando qualquer peão comido para a base.
*		No fim da jogada, a função passa a vez para o próximo jogador (incluindo ele mesmo, caso tenha tirado 6 no dado ou
*		comido outro peão. Isso	não ocorre mais do que 3 vezes consecutivas).
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetPartidaInexistente
*		PART_CondRetInconsistencia
*		PART_CondRetSemMemoria
*		PART_CondRetErroListaC
*		PART_CondRetErroLista
*		PART_CondRetErroPeao
*******************************************************************************************************************************/
PART_tpCondRet PART_Jogar();

/*******************************************************************************************************************************
*	$FC Função: PART_ChecarVitoria
*
*	$ED Descrição da função:
*		Retorna se o jogador que jogou o último turno venceu o jogo e em caso positivo, retorna também sua cor.
*
*	$EP Parâmetros:
*		$P boolRet			-	parâmetro que receberá o retorno da função: True caso o jogador tenha vencido, False caso contrário.
*								Este parâmetro é passado como referência.
*		$P CorVencedorRet	-	parâmetro que receberá o a cor do jogador que venceu o jogo.
*								Este parâmetro é passado como referência.
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetErroLista
*		PART_CondRetErroListaC
*		PART_CondRetErroPeao
*		PART_CondRetNinguemJogou
*		PART_CondRetPartidaInexistente
*******************************************************************************************************************************/
PART_tpCondRet PART_ChecarVitoria(DEF_tpBool* BoolRet, DEF_tpCor* CorVencedorRet);

/*******************************************************************************************************************************
*	$FC Função: PART_AnunciarVencedor
*
*	$ED Descrição da função:
*		Imprime na interface do console uma mensagem de parabenização para o jogador vencedor cuja cor é recebida por parâmetro.
*
*	$EP Parâmetros:
*		$P CorVencedor	-	cor do jogador vencedor
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetInconsistencia
*		PART_CondRetPartidaInexistente
*******************************************************************************************************************************/
PART_tpCondRet PART_AnunciarVencedor(DEF_tpCor CorVencedor);

/*******************************************************************************************************************************
*	$FC Função: PART_DestruirPartida
*
*	$ED Descrição da função:
*		Destroi a partida em andamento, liberando o que for necessário da memória.
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetPartidaInexistente
*		PART_CondRetErroListaC
*		PART_CondRetErroTabuleiro
*******************************************************************************************************************************/
PART_tpCondRet PART_DestruirPartida();

/*******************************************************************************************************************************
*	Fim do módulo de definição: Módulo Partida
*******************************************************************************************************************************/