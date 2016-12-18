#include "Partida.h"
#include "Definicoes.h"
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include "interface.h"
#include "glut.h"

static DEF_tpCor corVencedor;

static int pegarSN();

static void apresentarFalha();

static void antesPartida();

static void loopPartida();

static int testeJogadorVenceu();

static void depoisPartida();

static int testeRecomecarPartida();

/*******************************************************************************************************************************
*	$FC Função: main
*
*	$ED Descrição da função:
*		Executa o jogo. Podem ser jogadas quantas partidas forem desejadas.
*
*	$FV Valor retornado:
*		0	-	jogo encerrou corretamente
*		1	-	houve um erro de execução que abortou o programa
*******************************************************************************************************************************/
int main(int argc, char**argv) {

	ITFC_tpCondRet debugInterface;

	printf("\tBem vindo ao LUDO 10/10!\n\nPressione qualquer tecla para iniciar uma partida...");

	getch();

	ITFC_ConfigurarInterface(&argc, argv);
	ITFC_ConfigurarJanela(500, 500);
	ITFC_ConfigurarLoopJanela(antesPartida, depoisPartida, testeRecomecarPartida);
	ITFC_ConfigurarLoopInterface(loopPartida, testeJogadorVenceu);
	debugInterface = ITFC_CarregarImagens();
	if (debugInterface)
		apresentarFalha();
	ITFC_IniciarJanela();

	return 0;
}

/*******************************************************************************************************************************
*	$FC Função: pegarSN
*
*	$ED Descrição da função:
*		Pausa o jogo até o usuário digitar S ou N.
*
*	$FV Valor retornado:
*		1	-	caso o usuário tenha digitado S
*		0	-	caso o usuário tenha digitado N
*******************************************************************************************************************************/
static int pegarSN() {
	int resultado;
	do {
		char resposta = getch();
		switch(resposta) {
			case 's': case 'S':
				resultado = 1;
				break;
			case 'n': case 'N':
				resultado = 0;
				break;
			default:
				resultado = -1;
		}	/* switch */
	} while(resultado == -1);
	return resultado;
}

/*******************************************************************************************************************************
*	$FC Função: apresentarFalha
*
*	$ED Descrição da função:
*		Apresenta mensagem de erro e aborta programa
*******************************************************************************************************************************/
static void apresentarFalha() {
	system("cls");
	printf("Erro grave durante execucao do jogo!\n\nABORTAR...\n");
	exit(1);
}

/*******************************************************************************************************************************
*	$FC Função: antesPartida
*
*	$ED Descrição da função:
*		Pedaço de código que será executado com a criação da janela da interface e sempre que o jogo for recomeçado.
*******************************************************************************************************************************/
static void antesPartida() {

	PART_tpCondRet debugPartida;
	int numJogadores;

	/* Pede quantidade de jogadores que participarão da partida */
	do {
		system("cls");
		printf("Escolha quantos jogadores participarao da partida (2~4):\t");
		numJogadores = getch() - '0';

		if (numJogadores >= 2 && numJogadores <= 4) {
			printf("\n\nA partida tera %d jogadores. Tem certeza? (s/n)\t", numJogadores);

			/* Se o usuário digitou N, pede de novo para digitar a quantidade de jogadores */
			if (!pegarSN())
				numJogadores = 0;
		}

	} while(numJogadores < 2 || numJogadores > 4);

	puts("\n\n");

	/* Criar partida */
	debugPartida = PART_CriarPartida(numJogadores);
	/* Se não retornou OK, erro */
	if (debugPartida)
		apresentarFalha();
}

static void loopPartida() {
	PART_tpCondRet debugPartida;

	printf("Pressione qualquer coisa para continuar...");
	getch();

	system("cls");

	/* Jogar turno */
	debugPartida = PART_Jogar();
	/* Se não retornou OK, erro */
	if (debugPartida)
		apresentarFalha();
}

static int testeJogadorVenceu() {
	DEF_tpBool venceu;
	PART_tpCondRet debugPartida;

	/* Testar se em último turno, jogador venceu */
	debugPartida = PART_ChecarVitoria(&venceu, &corVencedor);
	/* Se não retornou OK, erro */
	if (debugPartida)
		apresentarFalha();

	/*printf("\n\nPressione qualquer coisa para continuar...");*/
	/*getch();*/

	if (venceu == True)
		return 1;
	else
		return 0;
}

static void depoisPartida() {
	PART_tpCondRet debugPartida;

	/* Anunciar vencedor */
	system("cls");
	debugPartida = PART_AnunciarVencedor(corVencedor);
	/* Se não retornou OK, erro */
	if (debugPartida)
		apresentarFalha();

	/* Destruir partida encerrada */
	debugPartida = PART_DestruirPartida();
	/* Se não retornou OK, erro */
	if (debugPartida)
		apresentarFalha();
}

static int testeRecomecarPartida() {
	int jogarNovamente;

	printf("\n\nDeseja jogar novamente? (s/n)\t");

	/* Pegar desejo do usuário de jogar novamente */
	jogarNovamente = pegarSN();

	if (!jogarNovamente) {
			/* O jogador não deseja jogar novamente, encerrar o programa */

		system("cls");
		printf("Obrigado por jogar!\n\n");
		return 1;
	} else {
		return 0;
	}
}