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
*		5.00	rrc		18/12/2016	Várias adições de funções a serem usadas pelo módulo ITFC
*******************************************************************************************************************************/

#include "Partida.h"

#include "Peao.h"
#include "ListaC.h"
#include "Lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define NUM_PEOES 4
	/* Número de peões que cada jogador possui */

#define MAX_REPETICOES_TURNO 3
	/* Quantidade máxima de turnos consecutivos que um jogador pode jogar */

/*******************************************************************************************************************************
*	Dados encapsulados no módulo:
*******************************************************************************************************************************/

/*******************************************************************************************************************************
*	$TC Tipo de dados: PART Jpgador
*
*	$ED Descrição do tipo:
*		Representa um jogador, que possui um númerp identificador, uma lista de peões e a cor de seus peões
*******************************************************************************************************************************/
typedef struct PART_tgJogador {
    
	int iNumJogador;
    LIS_tppLista pLstPeoes;
    DEF_tpCor Cor;

} PART_tpJogador;

static LSTC_tppListaC lstJogadores = NULL;
	/* Lista circular de jogadores. O corrente é aquele cujo turno está em andamento. */
	/* Inicialmente NULL para representar nenhuma partida em andamento */

static int iQtdJogadores;
	/* Quantidade de jogadores participando da partida. */

static PART_tpJogador* pUltimoJogador;
	/* Jogador que foi o último a jogar (cujo turno acabou de passar) - NULL se ninguém houver jogado */

static int iNumJogadasRepetidas;
	/* Número de jogadas repetidas que o jogador de turno atual já jogou. Ele jogará outra jogada repetida caso tire 6 ou coma
	outro peão em sua vez. Se esse número passar de MAX_REPETICOES_TURNO-1, a vez é passada para o próximo jogador */

/*******************************************************************************************************************************
*	Protótipos de funções encapsuladas no módulo:
*******************************************************************************************************************************/

static void ExcluirPeao(void* pVoid);

static void ExcluirJogador(void* pVoid);

static PART_tpCondRet PART_ChecarPeoesDisponiveis(PART_tpJogador* jogadorVez, int dado, LIS_tppLista peoesDisponiveis);

static PART_tpCondRet PART_Escolher(LIS_tppLista peoesDisponiveis, PEAO_tppPeao* peaoEscolhidoRet);

static int CriterioProcurarCorJogador(void* pElemBuscado, void* pElemLista);

static int CriterioProcurarNumJogador(void* pElemBuscado, void* pElemLista);

static PART_tpCondRet PART_ComerPeao(PEAO_tppPeao peaoPrincipal, DEF_tpCor corPeaoComido);

static PART_tpCondRet PART_ImprimirCor(DEF_tpCor cor);

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
	TAB_tpCondRet debugTabuleiro;
	int i, j;

	/* Número mínimo e máximo de jogadores de um jogo de ludo */
	if (n < 2 || n > 4)
		return PART_CondRetNumInvalido;
	iQtdJogadores = n;

	/* Se já existe lista de jogadores, partida já existe */
	if (lstJogadores != NULL) {
		return PART_CondRetPartidaJaExiste;
	}

	/* Lista de jogadores é circular */
	debugLstC = LSTC_CriarListaC(&lstJogadores, ExcluirJogador);
	/* Se não retornou OK, erro */
	if (debugLstC)	return PART_CondRetErroListaC;

	/* Nenhum jogador jogou ainda */
	pUltimoJogador = NULL;

	/* Número de jogadas repetidas ainda é zero */
	iNumJogadasRepetidas = 0;

	/* Criar tabuleiro */
	debugTabuleiro = TAB_CriarTabuleiro();
	/* Se não retornou OK, erro */
	if (debugTabuleiro)
		return PART_CondRetErroTabuleiro;

	/* Criar cada jogador e inseri-lo em lista: */
	for (i = 0; i < n; i++) {
		DEF_tpCor cor;
		PART_tpJogador* jog;

		/* Cores dos jogadores estão na ordem */
		switch(i) {
			case 0:	cor = AZUL;		break;
			case 1: cor = AMARELO;	break;
			case 2:	cor = VERDE;	break;
			case 3:	cor = VERMELHO;	break;
		}
		/* Criar jogador */
		jog = (PART_tpJogador*)malloc(sizeof(PART_tpJogador));
		if (!jog)	return PART_CondRetSemMemoria;

		jog->Cor = cor;
		jog->iNumJogador = i + 1;

		/* Criar lista de peões */
		debugLista = LIS_CriarLista(&jog->pLstPeoes, ExcluirPeao);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		/* Criar cada peão do jogador: */
		for (j = 0; j < NUM_PEOES; j++) {
			PEAO_tppPeao tempPeao;

			/* Criar peão */
			debugPeao = PEAO_CriarPeao(&tempPeao, cor, j+1);
			/* Se não retornou OK, erro */
			if (debugPeao)	return PART_CondRetErroPeao;

			/* Inserir peão na lista de peões do jogador */
			debugLista = LIS_InserirElementoApos(jog->pLstPeoes, tempPeao);
			/* Se não retornou OK, erro */
			if (debugLista)	return PART_CondRetErroLista;
		}	/* for */

		if (i == 0)
			j = 0;
		else
			j = i-1;

		/* Inserir jogador na lista de jogadores da partida, na ordem das cores */
		debugLstC = LSTC_InserirElemento(lstJogadores, j, jog);
		/* Se não retornou OK, erro */
		if (debugLstC)	return PART_CondRetErroListaC;
	}	/* for */
	return PART_CondRetOK;
} /* Fim Função PART_CriarPartida */

/*******************************************************************************************************************************
*	Função: PART_Jogar
*/
PART_tpCondRet PART_Jogar() {
	LSTC_tpCondRet debugListaC;
	PART_tpCondRet debugPartida;
	LIS_tpCondRet debugLista;
	PEAO_tpCondRet debugPeao;
	PART_tpJogador* jogAtual;
	LIS_tppLista lstPeoesDisponiveis;
	PEAO_tppPeao peaoEscolhido;
	DEF_tpBool peaoEstaBase;
	DEF_tpCor corAComer;
	DEF_tpBool podeAndar;
	int numDado, qtdPeoesDisponiveis, jogarNovamente = 0, numPeaoEscolhido;

	if (lstJogadores == NULL)	return PART_CondRetPartidaInexistente;

	/* Pegar jogador atual */
	debugListaC = LSTC_ObterElemento(lstJogadores, 0, (void**)&jogAtual);
	/* Se não retornou OK, erro */
	if (debugListaC)	return PART_CondRetErroListaC;

	printf("Jogada do jogador ");
	/* Imprimir cor do jogador */
	debugPartida = PART_ImprimirCor(jogAtual->Cor);
	/* Se não retornou OK, erro */
	if (debugPartida)
		return debugPartida;
	printf(":\n\n");

	printf("Pressione qualquer tecla para jogar o dado...");

	/* LEMBRAR DE TIRAR ISSO E DESCOMENTAR ABAIXO */
	numDado = getch() - '0';

	/*srand(time(NULL));
	numDado = (rand() % 6) + 1*/;

	printf("\nO dado rola... Voce sorteou um %d!\n", numDado);

	if (numDado == 6)
		jogarNovamente = 1;

	/* Criar lista vazia temporária para colocar peões disponíveis (sem função de exclusão pois a lista é de ponteiros temporários) */
	debugLista = LIS_CriarLista(&lstPeoesDisponiveis, NULL);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	/* Colocar peões disponíveis para movimentação na lista */
	debugPartida = PART_ChecarPeoesDisponiveis(jogAtual, numDado, lstPeoesDisponiveis);
	/* Se não retornou OK, erro */
	if (debugPartida)
		return debugPartida;

	/* Pegar quantos peões estão disponíveis */
	debugLista = LIS_ObterTamanhoLista(lstPeoesDisponiveis, &qtdPeoesDisponiveis);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	if (qtdPeoesDisponiveis == 0) {
		printf("Nenhum de seus peoes pode ser movimentado!\nFim de turno...\n\n");

		/* Destruir lista temporária */
		debugLista = LIS_DestruirLista(lstPeoesDisponiveis);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		if (!jogarNovamente || iNumJogadasRepetidas >= MAX_REPETICOES_TURNO-1) {
				/* Se o jogador atual não jogará novamente ou já jogou 3 vezes seguidas (2 repetidas) */

			LSTC_MoverCorrente(lstJogadores, 1);
			iNumJogadasRepetidas = 0;
		} else {
				/* Caso que o jogador atual jogará novamente */

			iNumJogadasRepetidas++;
		}	/* if */

		/* Marca o último a jogar */
		pUltimoJogador = jogAtual;

		/* Encerrar jogada */
		return PART_CondRetOK;
	}	/* if */
		/* Caso em que há peões disponíveis para movimentar, movimentação obrigatória */

	/* Pedir para jogador escolher o peão que deseja movimentar */
	debugPartida = PART_Escolher(lstPeoesDisponiveis, &peaoEscolhido);
	/* Se não retornou OK, erro */
	if (debugPartida)
		return debugPartida;

	/* Ver se peão está na base */
	debugPeao = PEAO_EstaPeaoBase(peaoEscolhido, &peaoEstaBase);
	/* Se não retornou OK, erro */
	if (debugPeao)
		return PART_CondRetErroPeao;

	/* Pegar número do peão escolhido */
	debugPeao = PEAO_ObterNumeroPeao(peaoEscolhido, &numPeaoEscolhido);
	/* Se não retornou OK, erro */
	if (debugPeao)
		return PART_CondRetErroPeao;

	if (peaoEstaBase == True) {
		/* Caso que o peão escolhido para se movimentar está na base */

		/* Se está na base e estava disponível, então o jogador tem que ter tirado 6 */
		if (numDado != 6)
			return PART_CondRetInconsistencia;

		/* Pegar possível cor de um peão que será comido na realização do movimento */
		debugPeao = PEAO_ChecarMovimentoDisponivelPeao(peaoEscolhido, numDado, &podeAndar, &corAComer);
		/* Se não retornou OK, erro */
		if (debugPeao)
			return PART_CondRetErroPeao;

		/* Deve ser possível andar, se peão já estava disponível */
		if (podeAndar == False)
			return PART_CondRetInconsistencia;

		/* Sair da base com peão */
		debugPeao = PEAO_SairBasePeao(peaoEscolhido);
		/* Se não retornou OK, erro */
		if (debugPeao)
			return PART_CondRetErroPeao;

		printf("\n\nO peao %d saiu da base!", numPeaoEscolhido);
	} else {
			/* Caso que o peão escolhido para se movimentar está em uma casa */

			/* Pegar possível cor de um peão que será comido na realização do movimento */
			debugPeao = PEAO_ChecarMovimentoDisponivelPeao(peaoEscolhido, numDado, &podeAndar, &corAComer);
			/* Se não retornou OK, erro */
			if (debugPeao)
				return PART_CondRetErroPeao;

			/* Deve ser possível andar, se peão já estava disponível */
			if (podeAndar == False)
				return PART_CondRetInconsistencia;

			/* Andar com peão */
			debugPeao = PEAO_AndarPeao(peaoEscolhido, numDado);
			/* Se não retornou OK, erro */
			if (debugPeao)
				return PART_CondRetErroPeao;

			printf("\n\nO peao %d andou %d casas!", numPeaoEscolhido, numDado);
	}	/* if */

	/* Se for necessário comer outro peão */
	if (corAComer != SEM_COR) {
		PART_ComerPeao(peaoEscolhido, jogAtual->Cor, corAComer);
		jogarNovamente = 1;
	}	/* if */

	/* Destruir lista temporária */
	debugLista = LIS_DestruirLista(lstPeoesDisponiveis);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	if (!jogarNovamente || iNumJogadasRepetidas >= MAX_REPETICOES_TURNO-1) {
			/* Se o jogador atual não jogará novamente ou já jogou 3 vezes seguidas (2 repetidas) */

		LSTC_MoverCorrente(lstJogadores, 1);
		iNumJogadasRepetidas = 0;
	} else {
			/* Caso que o jogador atual jogará novamente */

		iNumJogadasRepetidas++;
	}	/* if */

	/* Marca o último a jogar */
	pUltimoJogador = jogAtual;

	printf("\nFim de turno...\n\n", numDado);

	/* Encerrar jogada */
	return PART_CondRetOK;
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

	if (lstJogadores == NULL)	return PART_CondRetPartidaInexistente;

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
		if (final == True)
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
}	/* Fim Função PART_ChecarVitoria */

/*******************************************************************************************************************************
*	Função: PART_AnunciarVencedor
*/
PART_tpCondRet PART_AnunciarVencedor(DEF_tpCor CorVencedorRet) {
	PART_tpCondRet debugPartida;

	if (lstJogadores == NULL)	return PART_CondRetPartidaInexistente;

	printf("Parabens jogador ");

	/* Imprimir cor do jogador */
	debugPartida = PART_ImprimirCor(CorVencedorRet);
	/* Se não retornou OK, erro */
	if (debugPartida)	return debugPartida;

	printf(", voce venceu!!!");

	return PART_CondRetOK;
}	/* Fim Função PART_AnunciarVencedor */

/*******************************************************************************************************************************
*	Função: PART_ObterQtdJogadores
*/
PART_tpCondRet PART_ObterQtdJogadores(int* qtdJogadores) {
	LSTC_tpCondRet debugListaC;
	int tamLst;

	if (lstJogadores == NULL) {
		return PART_CondRetPartidaInexistente;
	}

	debugListaC = LSTC_ObterTamanhoListaC(lstJogadores, &tamLst);
	if (debugListaC) {
		return PART_CondRetErroListaC;
	}

	*qtdJogadores = tamLst;

	return PART_CondRetOK;
}	/* Fim Função PART_ObterQtdJogadores */

/*******************************************************************************************************************************
*	Função: PART_ObterQtdPeoes
*/
PART_tpCondRet PART_ObterQtdPeoes(int* qtdPeoes) {

	*qtdPeoes = NUM_PEOES;

	return PART_CondRetOK;
}	/* Fim Função PART_ObterQtdPeoes */

/*******************************************************************************************************************************
*	Função: PART_ObterCorJogador
*/
PART_tpCondRet PART_ObterCorJogador(int iNumJogador, DEF_tpCor* corJogador) {

	PART_tpCondRet debugPartida;
	LSTC_tpCondRet debugListaC;
	PART_tpJogador* jog;
	int qtdMaxJogadores, indice;

	if (lstJogadores == NULL) {
		return PART_CondRetPartidaInexistente;
	}

	/* Pegar quantidade máxima de jogadores para testes */
	debugPartida = PART_ObterQtdJogadores(&qtdMaxJogadores);
	/* Se não retornou OK, erro */
	if (debugPartida) {
		return debugPartida;
	}

	if (iNumJogador < 1 || iNumJogador > qtdMaxJogadores) {
		return PART_CondRetNumInvalido;
	}

	/* Pegar índice de jogador na lista */
	debugListaC = LSTC_ProcurarElemento(lstJogadores, &iNumJogador, &indice, CriterioProcurarNumJogador);
	/* Se não retornou OK, erro */
	if (debugListaC == LSTC_CondRetElemInexistente) {
		return PART_CondRetInconsistencia;
	}
	if (debugListaC) {
		return PART_CondRetErroListaC;
	}

	/* Pegar jogador */
	debugListaC = LSTC_ObterElemento(lstJogadores, indice, (void**)&jog);
	/* Se não retornou OK, erro */
	if (debugListaC) {
		return PART_CondRetErroListaC;
	}

	/* Retornar cor do jogador */
	*corJogador = jog->Cor;

	return PART_CondRetOK;
}	/* Fim Função PART_ObterCorJogador */

/*******************************************************************************************************************************
*	Função: PART_ObterCasaPeaoJogador
*/
PART_tpCondRet PART_ObterCasaPeaoJogador(int iNumJogador, int iNumPeao, TAB_tppCasa* casaPeao) {

	PART_tpCondRet debugPartida;
	LSTC_tpCondRet debugListaC;
	LIS_tpCondRet debugLista;
	LIS_tppLista lstPeoes;
	PART_tpJogador* jog;
	int qtdMaxJogadores, indice, qtdPeoes, i;

	if (lstJogadores == NULL) {
		return PART_CondRetPartidaInexistente;
	}

	/* Pegar quantidade máxima de jogadores para testes */
	debugPartida = PART_ObterQtdJogadores(&qtdMaxJogadores);
	/* Se não retornou OK, erro */
	if (debugPartida) {
		return debugPartida;
	}

	if (iNumJogador < 1 || iNumJogador > qtdMaxJogadores) {
		return PART_CondRetNumInvalido;
	}

	if (iNumPeao < 1 || iNumPeao > NUM_PEOES) {
		return PART_CondRetNumInvalido;
	}

	/* Pegar índice de jogador na lista */
	debugListaC = LSTC_ProcurarElemento(lstJogadores, &iNumJogador, &indice, CriterioProcurarNumJogador);
	/* Se não retornou OK, erro */
	if (debugListaC == LSTC_CondRetElemInexistente) {
		return PART_CondRetInconsistencia;
	}
	if (debugListaC) {
		return PART_CondRetErroListaC;
	}

	/* Pegar jogador */
	debugListaC = LSTC_ObterElemento(lstJogadores, indice, (void**)&jog);
	/* Se não retornou OK, erro */
	if (debugListaC) {
		return PART_CondRetErroListaC;
	}

	lstPeoes = jog->pLstPeoes;

	/* Pegar quantidade de peões na lista */
	debugLista = LIS_ObterTamanhoLista(lstPeoes, &qtdPeoes);
	/* Se não retornou OK, erro */
	if (debugLista) {
		return PART_CondRetErroLista;
	}

	/* Ir para início de lista de peões */
	debugLista = LIS_IrInicioLista(lstPeoes);
	/* Se não retornou OK, erro */
	if (debugLista) {
		return PART_CondRetErroLista;
	}

	/* Índice do elemento atual da lista */
	i = 0;

	/* Percorrer lista procurando por número de peão */
	while(1) {
		PEAO_tppPeao tempPeao;
		PEAO_tpCondRet debugPeao;
		int numTempPeao;

		/* Pegar peão */
		debugLista = LIS_ObterValor(lstPeoes, &tempPeao);
		/* Se não retornou OK, erro */
		if (debugLista) {
			return PART_CondRetErroLista;
		}

		/* Pegar número do peão */
		debugPeao = PEAO_ObterNumeroPeao(tempPeao, &numTempPeao);
		/* Se não retornou OK, erro */
		if (debugPeao) {
			return PART_CondRetErroPeao;
		}

		/* Se peão é o peão procurado */
		if (numTempPeao == iNumPeao) {

			/* Retornar casa do peão */
			debugPeao = PEAO_ObterCasaPeao(tempPeao, casaPeao);
			/* Se não retornou OK, erro */
			if (debugPeao) {
				return PART_CondRetErroPeao;
			}

			break;
		}

		/* Se chegou em último elemento e não encontrou peão, erro */
		if (i >= qtdPeoes - 1) {
			return PART_CondRetInconsistencia;
		}

		/* Avançar em lista de peões */
		debugLista = LIS_AvancarElementoCorrente(lstPeoes, 1);
		/* Se não retornou OK, erro */
		if (debugLista) {
			return PART_CondRetErroLista;
		}
	}	/* while */

	return PART_CondRetOK;
}	/* Fim Função PART_ObterCasaPeaoJogador */

/*******************************************************************************************************************************
*	Função: PART_DestruirPartida
*/
PART_tpCondRet PART_DestruirPartida() {
	LSTC_tpCondRet debugListaC;
	TAB_tpCondRet debugTabuleiro;

	if (lstJogadores == NULL)	return PART_CondRetPartidaInexistente;

	debugTabuleiro = TAB_DestruirTabuleiro();
	/* Se não retornou OK, erro */
	if (debugTabuleiro)	return PART_CondRetErroTabuleiro;

	/* Libera lista de jogadores */
	debugListaC = LSTC_DestruirListaC(lstJogadores);
	/* Se não retornou OK, erro */
	if (debugListaC)	return PART_CondRetErroListaC;

	lstJogadores = NULL;

	return PART_CondRetOK;
}	/* Fim Função PART_DestruirPartida */

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
static PART_tpCondRet PART_ChecarPeoesDisponiveis(PART_tpJogador* jogadorVez, int iNumDado, LIS_tppLista peoesDisponiveis) {
	LIS_tpCondRet debugLista;
	LIS_tppLista lstPeoes;
	int tamLst, i, qtdPeoes;

	/* Pegar tamanho da lista passada: */
	debugLista = LIS_ObterTamanhoLista(peoesDisponiveis, &tamLst);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;
	/* Se a lista passada é não vazia, erro */
	if (tamLst != 0)	return PART_CondRetInconsistencia;

	lstPeoes = jogadorVez->pLstPeoes;
		/* Lista de peões do jogador */

	/* Pegar tamanho da lista de peões: */
	debugLista = LIS_ObterTamanhoLista(lstPeoes, &qtdPeoes);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	/* Vai para o primeiro peão */
	debugLista = LIS_IrInicioLista(lstPeoes);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	i = 0;
		/* Posição na lista */
	/* Percorrer lista de peões: */
	while(1) {
		PEAO_tppPeao tempPeao;
		DEF_tpBool movimentoDisp, estaBase;
		DEF_tpCor corAFrente;
		PEAO_tpCondRet debugPeao;

		/* Pegar peão */
		debugLista = LIS_ObterValor(lstPeoes, (void**)&tempPeao);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		/* Ver se o peao esta na base */
		debugPeao = PEAO_EstaPeaoBase(tempPeao, &estaBase);
		/* Se não retornou OK, erro */
		if (debugPeao)
			return PART_CondRetErroPeao;

		/* Ver se ele pode andar */
		debugPeao = PEAO_ChecarMovimentoDisponivelPeao(tempPeao, iNumDado, &movimentoDisp, &corAFrente);
		/* Se não retornou OK, erro */
		if (debugPeao)
			return PART_CondRetErroPeao;

		/* Se puder andar */
		if (movimentoDisp == True) {
			/* Adicionar peão à lista de peões disponíveis */
			debugLista = LIS_InserirElementoApos(peoesDisponiveis, tempPeao);
			/* Se não retornou OK, erro */
			if (debugLista)	return PART_CondRetErroLista;
		}	/* if */

		/* Se chegou em último elemento da lista, para de percorrer */
		if (i >= qtdPeoes - 1)
			break;

		/* Avança em lista */
		debugLista = LIS_AvancarElementoCorrente(lstPeoes, 1);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		i++;
	}	/* while */

	return PART_CondRetOK;
}	/* Fim Função PART_ChecarPeoesDisponiveis */

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

	/* Ir para início da lista */
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

	printf(".");

	/* Tentar obter um número válido do teclado (se for inválido, repete, pedindo o usuário para digitar outro) */
	while(1) {
		int flag = 0;

		/* scanf("%d", &numEscolhido); */
		numEscolhido = getch() - '0';

		/* Ir para início da lista */
		debugLista = LIS_IrInicioLista(peoesDisponiveis);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

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

		printf("\nCaracter digitado invalido. Escolha um peao para movimentar.");

	}	/* while */

	return PART_CondRetOK;
}	/* Fim Função PART_Escolher */

/*******************************************************************************************************************************
*	$FC Função: CriterioProcurarCorJogador
*
*	$ED Descrição da função:
*		Função de critério de busca em lista circular. Serve para procurar uma cor de um jogador em uma lista de jogadores.
*
*	$EP Parâmetros:
*		$P pElemBuscado	-	será procurada uma cor
*		$P pElemLista	-	elementos da lista são jogadores
*
*	$FV Valor retornado:
*		1	-	caso jogador da cor passada seja encontrado
*		0	-	caso jogador não tenha sido encontrado
*******************************************************************************************************************************/
static int CriterioProcurarCorJogador(void* pElemBuscado, void* pElemLista) {
	DEF_tpCor* pCorBuscada = (DEF_tpCor*) pElemBuscado;
	PART_tpJogador* pJogadorLista = (PART_tpJogador*) pElemLista;
	if (pJogadorLista->Cor == *pCorBuscada)
		return 1;
	else
		return 0;
}

/*******************************************************************************************************************************
*	$FC Função: CriterioProcurarNumJogador
*
*	$ED Descrição da função:
*		Função de critério de busca em lista circular. Serve para procurar um número de um jogador em uma lista de jogadores.
*
*	$EP Parâmetros:
*		$P pElemBuscado	-	será procurado um número
*		$P pElemLista	-	elementos da lista são jogadores
*
*	$FV Valor retornado:
*		1	-	caso jogador do número passado seja encontrado
*		0	-	caso jogador não tenha sido encontrado
*******************************************************************************************************************************/
static int CriterioProcurarNumJogador(void* pElemBuscado, void* pElemLista) {
	int numBuscado = *(int*) pElemBuscado;
	PART_tpJogador* pJogadorLista = (PART_tpJogador*) pElemLista;
	if (pJogadorLista->iNumJogador == numBuscado) {
		return 1;
	} else {
		return 0;
	}
}

/*******************************************************************************************************************************
*	$FC Função: PART_ComerPeao
*
*	$ED Descrição da função:
*		Recebe o peão que comerá o outro e sua cor, cuja posição já deve ser a mesma casa do peão comido. Recebe também a cor do 
*		peão comido. Faz o peão comido voltar para a base e imprime uma mensagem avisando que ele foi comido na interface do console.
*
*	$EP Parâmetros:
*		$P peaoPrincipal	-	peão que comerá o outro
*		$P corPeaoPrincipal	-	cor do peão que comerá o outro
*		$P corPeaoComido	-	cor do peão que será comido
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetInconsistencia
*		PART_CondRetErroPeao
*		PART_CondRetErroListaC
*		PART_CondRetErroLista
*		PART_CondRetErroTabuleiro
*******************************************************************************************************************************/
static PART_tpCondRet PART_ComerPeao(PEAO_tppPeao peaoPrincipal, DEF_tpCor corPeaoPrincipal, DEF_tpCor corPeaoComido) {
	TAB_tppCasa casaAComer;
	LSTC_tpCondRet debugListaC;
	PEAO_tpCondRet debugPeao;
	LIS_tpCondRet debugLista;
	int indiceJogadorCor, iTamLstPeoes, i, numPeaoComido;
	PART_tpJogador* jogComido;
	LIS_tppLista pLstPeoes;

	/* Pegar casa dos peões */
	debugPeao = PEAO_ObterCasaPeao(peaoPrincipal, &casaAComer);
	/* Se não retornou OK, erro */
	if (debugPeao)	return PART_CondRetErroPeao;

	/* Pegar índice do jogador dono do peão comido */
	debugListaC = LSTC_ProcurarElemento(lstJogadores, &corPeaoComido, &indiceJogadorCor, CriterioProcurarCorJogador);
	/* Se não retornou OK, erro */
	if (debugListaC)	return PART_CondRetErroListaC;

	/* Pegar jogador dono do peão comido pelo seu índice */
	debugListaC = LSTC_ObterElemento(lstJogadores, indiceJogadorCor, (void**)&jogComido);
	/* Se não retornou OK, erro */
	if (debugListaC)	return PART_CondRetErroListaC;

	pLstPeoes = jogComido->pLstPeoes;
		/* Lista de peões que contém peão comido */

	/* Ir para início da lista para procurar peão comido */
	debugLista = LIS_IrInicioLista(pLstPeoes);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	/* Pegar tamanho da lista de peões */
	debugLista = LIS_ObterTamanhoLista(pLstPeoes, &iTamLstPeoes);
	/* Se não retornou OK, erro */
	if (debugLista)	return PART_CondRetErroLista;

	i = 0;
		/* Posição atual na lista de peões */

	/* Percorrer lista para encontrar peão comido por sua casa atual */
	while(1) {
		PEAO_tppPeao tempPeao;
		TAB_tppCasa tempCasa;

		/* Pegar peão da lista */
		debugLista = LIS_ObterValor(pLstPeoes, (void**)&tempPeao);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;

		/* Obter casa do peão */
		debugPeao = PEAO_ObterCasaPeao(tempPeao, &tempCasa);
		/* Se não retornou OK, erro */
		if (debugPeao)	return PART_CondRetErroPeao;

		/* Testar se peão está na casa procurada (a que foi comida) */
		if (tempCasa == casaAComer) {
				/* Em caso positivo, retornar peão para base, imprimir mensagem e encerrar função */

			PART_tpCondRet debugPartida;
			TAB_tpCondRet debugTabuleiro;

			/* Voltar para base */
			debugPeao = PEAO_VoltarBasePeao(tempPeao);
			/* Se não retornou OK, erro */
			if (debugPeao)	return PART_CondRetErroPeao;

			/* Agora que o peão foi comido, botar a nova cor naquela casa */
			debugTabuleiro = TAB_MudarCorPeaoNaCasa(casaAComer, corPeaoPrincipal);
			/* Se não retornou OK, erro */
			if (debugTabuleiro) {
				return PART_CondRetErroTabuleiro;
			}

			/* Pegar número do peão comido */
			debugPeao = PEAO_ObterNumeroPeao(tempPeao, &numPeaoComido);
			/* Se não retornou OK, erro */
			if (debugPeao)	return PART_CondRetErroPeao;

			printf("\n\nO peao ");

			/* Imprimir cor do peão comido */
			debugPartida = PART_ImprimirCor(corPeaoComido);
			/* Se não retornou OK, erro */
			if (debugPartida)	return debugPartida;

			printf(" %d foi comido e voltou para sua base!\n", numPeaoComido);

			break;
		}	/* if */

		/* Se chegou no final da lista e não encontrou peão, erro */
		if (i >= iTamLstPeoes - 1)
			return PART_CondRetInconsistencia;

		/* Avançar na lista */
		debugLista = LIS_AvancarElementoCorrente(pLstPeoes, 1);
		/* Se não retornou OK, erro */
		if (debugLista)	return PART_CondRetErroLista;
	}	/* while */

	return PART_CondRetOK;
}	/* Fim Função PART_ComerPeao */

/*******************************************************************************************************************************
*	$FC Função: PART_ImprimirCor
*
*	$ED Descrição da função:
*		Recebe uma cor e imprime seu nome na interface do console. Dá erro caso cor recebida seja SEM_COR.
*
*	$EP Parâmetros:
*		$P cor	-	cor a ser impressa
*
*	$FV Valor retornado:
*		PART_CondRetOK
*		PART_CondRetInconsistencia
*******************************************************************************************************************************/
static PART_tpCondRet PART_ImprimirCor(DEF_tpCor cor) {

	switch(cor) {
		case AZUL:
			printf("azul");
			break;
		case VERMELHO:
			printf("vermelho");
			break;
		case VERDE:
			printf("verde");
			break;
		case AMARELO:
			printf("amarelo");
			break;
		default:
			return PART_CondRetInconsistencia;
	}	/* switch */
	return PART_CondRetOK;
}	/* Fim Função PART_ImprimirCor */

/*******************************************************************************************************************************
*	Fim do módulo de definição: Módulo Partida
*******************************************************************************************************************************/