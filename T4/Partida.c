#include "Partida.h"

#include <stdlib.h>
#include <stdio.h>

#define NUM_PEOES 4
	/* Número de peões que cada jogador possui */

/*******************************************************************************************************************************
*	Dados encapsulados no módulo:
*******************************************************************************************************************************/

typedef struct PART_tgJogador {
    
    LIS_tppLista pLstPeoes;
    DEF_tpCor Cor;

} PART_tpJogador;

static LSTC_tppListaC lstJogadores = NULL;
	/* Lista circular de jogadores. O corrente é aquele cujo turno está em andamento. */

static int iQtdJogadores;
	/* Quantidade de jogadores participando da partida. */

static PART_tpJogador* pUltimoJogador;
	/* Jogador que foi o último a jogar (cujo turno acabou de passar) - NULL se ninguém houver jogado */

/*******************************************************************************************************************************
*	Protótipos de funções encapsuladas no módulo:
*******************************************************************************************************************************/

static void ExcluirPeao(void* pVoid);

static void ExcluirJogador(void* pVoid);

static void ExcluirInt(void* pVoid);

static PART_tpCondRet PART_ChecarPeoesDesponiveis(PART_tpJogador* jogadorVez, int dado, LIS_tppLista peoesDisponiveis);

static PART_tpCondRet PART_Escolher(LIS_tppLista peoesDisponiveis, PEAO_tppPeao* peaoEscolhidoRet);

/*******************************************************************************************************************************
*	Código de funções exportadas pelo módulo:
*******************************************************************************************************************************/

/*******************************************************************************************************************************
*	Função: PART_CriarPartida
*/
PART_tpCondRet PART_CriarPartida(int n) {
	PEAO_tpCondRet debugPeao;
	LSTC_tpCondRet debugLstC;
	LIS_tpCondRet debugLista;
	int i;

	/* Número mínimo e máximo de jogadores de um jogo de ludo */
	if (n < 2 || n > 4)
		return PART_CondRetNumInvalido;
	iQtdJogadores = n;

	/* Lista de jogadores é circular */
	debugLstC = LSTC_CriarListaC(&lstJogadores, ExcluirJogador);
	/* Se não retornou OK, erro */
	if (debugLstC)	return PART_CondRetErroListaC;

	/* Nenhum jogador jogou ainda */
	pUltimoJogador = NULL;

	/* Criar cada jogador e inseri-lo em lista: */
	for (i = 0; i < n; i++) {
		DEF_tpCor cor;
		PART_tpJogador* jog;
		switch(i) {
			case 0:	cor = AZUL;		break;
			case 1: cor = VERMELHO;	break;
			case 2:	cor = VERDE;	break;
			case 3:	cor = AMARELO;	break;
		}
		jog = (PART_tpJogador*)malloc(sizeof(PART_tpJogador));
		if (!jog)	return PART_CondRetSemMemoria;
		jog->Cor = cor;
		debugLista = LIS_CriarLista(&jog->pLstPeoes, ExcluirPeao);
		if (debugLista)	return PART_CondRetErroLista;
		/* Criar cada peão do jogador: */
		for (i = 0; i < NUM_PEOES; i++) {
			PEAO_tppPeao tempPeao;
			debugPeao = PEAO_CriarPeao(&tempPeao, cor, i+1);
			if (debugPeao)	return PART_CondRetErroPeao;
			debugLista = LIS_InserirElementoApos(jog->pLstPeoes, tempPeao);
			if (debugLista)	return PART_CondRetErroListaC;
		}	/* for */
	}	/* for */
	return PART_CondRetOK;
} /* Fim Função PART_CriarPartida */

/*******************************************************************************************************************************
*	Função: PART_Jogar
*/
PART_tpCondRet PART_Jogar() {

}	/* Fim Função PART_Jogar */

/*******************************************************************************************************************************
*	Função: PART_ChecarVitoria
*/
PART_tpCondRet PART_ChecarVitoria(DEF_tpBool* BoolRet, DEF_tpCor* CorVencedorRet) {
	int i, qtdPeoes, peoesFinal = 0, qtdJog;
	PART_tpJogador* jog;
	PEAO_tppPeao peao;
	LIS_tppLista lstPeoes;
	LSTC_tpCondRet debugListaC;
	LIS_tpCondRet debugLista;

	if (lstJogadores == NULL)	PART_CondRetPartidaInexistente;

	///* Pega jogador que jogou anteriormente */
	//debugListaC = LSTC_ObterElemento(lstJogadores, iQtdJogadores-1, (void**)&jog);
	///* Se não retornou OK, erro */
	//if (debugListaC)	return PART_CondRetErroListaC;

	if (!pUltimoJogador)	return PART_CondRetNinguemJogou;

	jog = pUltimoJogador;

	/* Pega quantidade de peões */
	lstPeoes = jog->pLstPeoes;
	debugLista = LIS_ObterTamanhoLista(lstPeoes, &qtdPeoes);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	/* Vai para início da lista de peões do jogador pego */
	debugLista = LIS_IrInicioLista(lstPeoes);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	/* i conta a posição atual na lista de peões */
	i = 0;

	/* Percorre a lista de peões */
	while(1) {
		PEAO_tpCondRet debugPeao;
		PEAO_tppPeao peao;
		DEF_tpBool final;

		/* Obtém peão */
		debugLista = LIS_ObterValor(lstPeoes, (void**)&peao);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		/* Testa se ele está no final */
		debugPeao = PEAO_EstaPeaoFinal(peao, &final);
		/* Se não retornou OK, erro */
		if (debugPeao)	return PART_CondRetErroPeao;

		/* Se o peão estiver no final, conta-o */
		if (final)
			peoesFinal++;

		/* Se está no último peão da lista, não avança a lista e para de percorrê-la */
		if (i >= qtdPeoes-1)
			break;

		/* Vai para o próximo peão */
		debugLista = LIS_AvancarElementoCorrente(jog->pLstPeoes, 1);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		i++;
	}	/* while */

	/* Se todos os peões do jogador estão no final, ele ganhou! */
	if (peoesFinal == qtdPeoes)
		*BoolRet = True;
	else
		*BoolRet = False;
	*CorVencedorRet = jog->Cor;
	return PART_CondRetOK;
} /* Fim Função PART_ChecarVitoria */

/*******************************************************************************************************************************
*	Função: PART_DestruirPartida
*/
PART_tpCondRet PART_DestruirPartida() {
	LSTC_tpCondRet debugListaC;

	if (lstJogadores == NULL)	PART_CondRetPartidaInexistente;

	/* Libera lista de jogadores */
	debugListaC = LSTC_DestruirListaC(lstJogadores);
	/* Se não retornou OK, erro */
	if (debugListaC)	return PART_CondRetErroListaC;

	lstJogadores = NULL;

	return PART_CondRetOK;
} /* Fim Função PART_DestruirPartida */

/*******************************************************************************************************************************
*	Código de funções encapsuladas no módulo:
*******************************************************************************************************************************/

/*******************************************************************************************************************************
*	$FC Função: ExcluirPeao
*
*	$ED Descrição da função:
*		Função de exclusão de peões de uma lista.
*
*	$EP Parâmetros:
*		$P pVoid			-	parâmetro que receberá ponteiro para o peão em formato genérico e o liberará da memória.
*******************************************************************************************************************************/
static void ExcluirPeao(void* pVoid) {
	PEAO_tppPeao pPeao = (PEAO_tppPeao) pVoid;

	/* Libera peão */
	PEAO_DestruirPeao(pPeao);
}	/* Fim Função ExcluirPeao */

/*******************************************************************************************************************************
*	$FC Função: ExcluirPeao
*
*	$ED Descrição da função:
*		Função de exclusão de jogadores de uma lista.
*
*	$EP Parâmetros:
*		$P pVoid			-	parâmetro que receberá ponteiro para o jogador em formato genérico e o liberará da memória.
*******************************************************************************************************************************/
static void ExcluirJogador(void* pVoid) {
	PART_tpJogador* pJog = (PART_tpJogador*) pVoid;

	/* Libera lista de peões */
	LIS_DestruirLista(pJog->pLstPeoes);
	/* Libera jogador */
	free(pJog);
}	/* Fim Função ExcluirJogador */

/*******************************************************************************************************************************
*	$FC Função: ExcluirInt
*
*	$ED Descrição da função:
*		Função de exclusão de inteiros de uma lista.
*
*	$EP Parâmetros:
*		$P pVoid			-	parâmetro que receberá ponteiro para o inteiro em formato genérico e o liberará da memória.
*******************************************************************************************************************************/
static void ExcluirInt(void* pVoid) {
	int* pInt = (int*) pVoid;

	/* Libera inteiro */
	free(pInt);
}	/* Fim Função ExcluirInt */

/*******************************************************************************************************************************
*	$FC Função: PART_ChecarPeoesDisponiveis
*
*	$ED Descrição da função:
*		Recebe um jogador, o número de casas que ele deseja andar com um de seus peões e uma lista vazia. A função encherá a
*		lista com os peões do jogador que podem ser escolhidos para serem movimentados (peões cujo movimento é válido). A
*		função de exclusão de elementos dessa lista NÃO deve excluir os peões, uma vez que a lista é apenas de ponteiros
*		temporários para peões que existem na duração da partida.
*
*	$EP Parâmetros:
*		$P jogadorVez			-	jogador em questão
*		$P iNumDado				-	número tirado no dado (quantidade de casas que se deseja andar)
*		$P peoesDisponiveis		-	lista que deve estar inicialmente vazia
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetInconsistencia
*		PART_CondRetSemMemoria
*		PART_CondRetErroLista
*		PART_CondRetErroPeao
*******************************************************************************************************************************/
static PART_tpCondRet PART_ChecarPeoesDesponiveis(PART_tpJogador* jogadorVez, int iNumDado, LIS_tppLista peoesDisponiveis) {
	LIS_tpCondRet debugLista;
	LIS_tppLista lstPeoes;
	int tamLst, i;

	/* Pegar tamanho da lista passada: */
	debugLista = LIS_ObterTamanhoLista(peoesDisponiveis, &tamLst);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;
	/* Se a lista passada é não vazia, erro */
	if (tamLst != 0)	return PART_CondRetInconsistencia;

	lstPeoes = jogadorVez->pLstPeoes;
		/* Lista de peões do jogador */

	/* Vai para o primeiro peão */
	debugLista = LIS_IrInicioLista(lstPeoes);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	i = 0;
		/* Posição na lista */
	/* Percorrer lista de peões: */
	while(1) {
		PEAO_tppPeao tempPeao;
		DEF_tpBool movimentoDisp;
		DEF_tpCor corAFrente;
		PEAO_tpCondRet debugPeao;

		/* Pegar peão */
		debugLista = LIS_ObterValor(lstPeoes, (void**)&tempPeao);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		/* Ver se ele pode andar */
		debugPeao = PEAO_ChecarMovimentoDisponivelPeao(tempPeao, iNumDado, &movimentoDisp, &corAFrente);
		/* Se não retornou OK, erro */
		if (debugPeao)	return PART_CondRetErroPeao;

		/* Se puder andar */
		if (movimentoDisp) {
			/* Adicionar peão à lista de peões disponíveis */
			debugLista = LIS_InserirElementoApos(peoesDisponiveis, tempPeao);
			/* Se não retornou OK, erro */
			if (debugLista)	return PART_CondRetErroLista;
		}	/* if */

		/* Se chegou em último elemento da lista, para de percorrer */
		if (i >= tamLst - 1)
			break;

		/* Avança em lista */
		debugLista = LIS_AvancarElementoCorrente(lstPeoes, 1);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		i++;
	}	/* while */

	return PART_CondRetOK;
}

/*******************************************************************************************************************************
*	$FC Função: PART_Escolher
*
*	$ED Descrição da função:
*		Recebe uma lista não vazia de peões e apresenta seus números identificadores, através da interface do console, ao
*		usuário. O usuário poderá escolher um deles através do teclado. A função retorna o peão escolhido.
*
*	$EP Parâmetros:
*		$P peoesDisponiveis	-	lista não vazia de peões
*		$P peaoEscolhidoRet	-	parâmetro que receberá peão escolhido.
*								Este parâmetro é passado como referência.
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetInconsistencia
*		PART_CondRetErroLista
*		PART_CondRetErroPeao
*******************************************************************************************************************************/
static PART_tpCondRet PART_Escolher(LIS_tppLista peoesDisponiveis, PEAO_tppPeao* peaoEscolhidoRet) {
	LIS_tpCondRet debugLista;
	int tamLst, i, numEscolhido;

	/* Pegar tamanho da lista */
	debugLista = LIS_ObterTamanhoLista(peoesDisponiveis, &tamLst);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;
	/* Se a lista está vazia, erro */
	if (tamLst == 0)	return PART_CondRetInconsistencia;

	debugLista = LIS_IrInicioLista(peoesDisponiveis);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	printf("Escolha um dos seguintes peoes para movimentar: ");

	i = 0;
		/* Posição atual na lista */

	while(1) {
		PEAO_tpCondRet debugPeao;
		PEAO_tppPeao tempPeao;
		int numPeao;

		/* Pegar peão da lista */
		debugLista = LIS_ObterValor(peoesDisponiveis, (void**)&tempPeao);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		/* Pegar número do peão */
		debugPeao = PEAO_ObterNumeroPeao(tempPeao, &numPeao);
		/* Se não retornou OK, erro */
		if (debugPeao)	return PART_CondRetErroPeao;

		printf("%d", numPeao);

		/* Se chegou em último elemento da lista, para de percorrer */
		if (i >= tamLst - 1)
			break;

		if (i == tamLst - 2)
			printf(" ou ");
		else
			printf(", ");

		/* Avançar em lista */
		debugLista = LIS_AvancarElementoCorrente(peoesDisponiveis, 1);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		i++;
	}	/* while */

	printf(".\n");

	/* Tentar obter um número válido do teclado (se for inválido, repete, pedindo o usuário para digitar outro) */
	while(1) {
		int flag = 0;

		scanf("%d", &numEscolhido);

		i = 0;
			/* Posição atual na lista */

		while(1) {
			PEAO_tpCondRet debugPeao;
			PEAO_tppPeao tempPeao;
			int numPeao;

			/* Pegar peão da lista */
			debugLista = LIS_ObterValor(peoesDisponiveis, (void**)&tempPeao);
			/* Se não retornou OK, erro */
			if (debugLista)	return PART_CondRetErroLista;

			debugPeao = PEAO_ObterNumeroPeao(tempPeao, &numPeao);

			/* Se o número escolhido está na lista, marca uma flag, marca peão retornado e segue em frente */
			if (numEscolhido == numPeao) {
				*peaoEscolhidoRet = tempPeao;
				flag = 1;
				break;
			}

			/* Se chegou em último elemento da lista, para de percorrer */
			if (i >= tamLst - 1)
				break;

			/* Avançar em lista */
			debugLista = LIS_AvancarElementoCorrente(peoesDisponiveis, 1);
			/* Se não retornou OK, erro */
			if (debugLista)	return PART_CondRetErroLista;

			i++;
		}	/* while */

		/* Se a flag está marcada, o peão escolhido foi encontrado na lista e segue-se em frente */
		if (flag)
			break;
		/* Se não, o número escolhido foi inválido e pede-se outro número */

		printf("Número digitado inválido. Escolha um peão para movimentar:\n");

	}	/* while */

	return PART_CondRetOK;
}