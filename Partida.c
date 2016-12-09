#include "Partida.h"

#include <stdlib.h>

#define NUM_PEOES 4

typedef struct PART_tgJogador {
    
    LIS_tppLista pLstPeoes;
    DEF_tpCor Cor;

} PART_tpJogador;

static LSTC_tppListaC lstJogadores;

static int iQtdJogadores;

static PART_tpJogador* pUltimoJogador;

static void ExcluirPeao(void* pVoid);

static void ExcluirJogador(void* pVoid);

static PART_tpCondRet PART_ChecarPeoesDesponiveis(PART_tpJogador* jogadorVez, int dado, LIS_tppLista* PeoesDisponiveisRet) {} /* N�o implementada pois foge ao escopo do t2 */

static PART_tpCondRet PART_Escolher(LIS_tppLista* PeoesDisponiveis, PEAO_tppPeao PeaoEscolhidoRet); /* N�o implementada pois foge ao escopo do T2 */

/*******************************************************************************************************************************
*	Fun��o: PART_CriarPartida
*/
PART_tpCondRet PART_CriarPartida(int n) {
	PEAO_tpCondRet debugPeao;
	LSTC_tpCondRet debugLstC;
	LIS_tpCondRet debugLista;
	int i;

	/* N�mero m�nimo e m�ximo de jogadores de um jogo de ludo */
	if (n < 2 || n > 4)
		return PART_CondRetNumInvalido;
	iQtdJogadores = n;

	/* Lista de jogadores � circular */
	debugLstC = LSTC_CriarListaC(&lstJogadores, ExcluirJogador);
	/* Se n�o retornou OK, erro */
	if (debugLstC) return PART_CondRetErroListaC;

	/* Nenhum jogador jogou ainda */
	pUltimoJog = NULL;

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
		/* Criar cada pe�o do jogador: */
		for (i = 0; i < NUM_PEOES; i++) {
			PEAO_tppPeao tempPeao;
			debugPeao = PEAO_CriarPeao(&tempPeao, cor, i+1);
			if (debugPeao)	return PART_CondRetErroPeao;
			debugLista = LIS_InserirElementoApos(jog->pLstPeoes, tempPeao);
			if (debugLista)	return PART_CondRetErroListaC;
		}
	}
	return PART_CondRetOK;
} /* Fim Fun��o PART_CriarPartida */

/*******************************************************************************************************************************
*	Fun��o: PART_Jogar
*/
PART_tpCondRet PART_Jogar(){}	/*N�o implementada pois foge ao escopo do T2*/
/* Fim Fun��o PART_Jogar */

/*******************************************************************************************************************************
*	Fun��o: PART_ChecarVitoria
*/
PART_tpCondRet PART_ChecarVitoria(DEF_tpBool* BoolRet, DEF_tpCor* CorVencedorRet) {
	int i, qtdPeoes, peoesFinal = 0, qtdJog;
	DEF_tpBool final;
	PART_tpJogador* jog;
	PEAO_tppPeao peao;
	LIS_tppLista lstPeoes;
	LSTC_tpCondRet debugListaC;
	LIS_tpCondRet debugLista;

	if (lstJogadores == NULL)	PART_CondRetPartidaInexistente;

	/* Pega jogador que jogou anteriormente */
	debugListaC = LSTC_ObterElemento(lstJogadores, iQtdJogadores-1, (void**)&jog);
	/* Se n�o retornou OK, erro */
	if (debugListaC)	return PART_CondRetErroListaC;

	/* Pega quantidade de pe�es */
	lstPeoes = jog->pLstPeoes;
	debugLista = LIS_ObterTamanhoLista(lstPeoes, &qtdPeoes);
	/* Se n�o retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	/* Vai para in�cio da lista de pe�es do jogador pego */
	debugLista = LIS_IrInicioLista(lstPeoes);
	/* Se n�o retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	/* i conta a posi��o atual na lista de pe�es */
	i = 0;

	/* Percorre a lista de pe�es */
	while(1) {
		PEAO_tpCondRet debugPeao;
		PEAO_tppPeao peao;
		Estado estado;

		/* Obt�m pe�o */
		debugLista = LIS_ObterValor(lstPeoes, (void**)&peao);
		/* Se n�o retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		/* Testa se ele est� no final */
		debugPeao = PEAO_EstaPeaoFinal(peao, &final);
		/* Se n�o retornou OK, erro */
		if (debugPeao)	return PART_CondRetErroPeao;

		/* Se o pe�o estiver no final, conta-o */
		if (final)
			peoesFinal++;

		/* Se est� no �ltimo pe�o da lista, n�o avan�a a lista e para de percorr�-la */
		if (i >= qtdPeoes-1)
			break;

		/* Vai para o pr�ximo pe�o */
		debugLista = LIS_AvancarElementoCorrente(jog->pLstPeoes, 1);
		/* Se n�o retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		i++;
	}

	/* Se todos os pe�es do jogador est�o no final, ele ganhou! */
	if (peoesFinal == qtdPeoes)
		*BoolRet = True;
	else
		*BoolRet = False;
	*CorVencedorRet = jog->Cor;
	return PART_CondRetOK;
} /* Fim Fun��o PART_ChecarVitoria */

/*******************************************************************************************************************************
*	Fun��o: PART_DestruirPartida
*/
PART_tpCondRet PART_DestruirPartida() {
	LSTC_tpCondRet debugListaC;
	debugListaC = LSTC_DestruirListaC(lstJogadores);
} /* Fim Fun��o PART_DestruirPartida */


/*******************************************************************************************************************************
*	Fun��o: PART_DestruirPartida
*/
void ExcluirPeao(void* pVoid) {
	PEAO_tppPeao pPeao = (PEAO_tppPeao) pVoid;
	PEAO_DestruirPeao(pPeao);
}

void ExcluirJogador(void* pVoid) {
	PART_tpJogador* pJog = (PART_tpJogador*) pVoid;
	LIS_DestruirLista(pJog->pLstPeoes);
	free(pJog);
}