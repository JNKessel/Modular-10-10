#include "Partida.h"
#include "Definicoes.h"
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

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
int pegarSN() {
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
void apresentarFalha() {
	system("cls");
	printf("Erro grave durante execucao do jogo!\n\nABORTAR...\n");
	exit(1);
}

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
int main() {

	int jogarNovamente;

	printf("\tBem vindo ao LUDO 10/10!\n\nPressione qualquer tecla para iniciar uma partida...");

	getch();

	/* Executa um loop para cada partida, enquanto se desejar jogar novamente */
	do {
		PART_tpCondRet debugPartida;
		int numJogadores, trigger;
		DEF_tpBool venceu;
		DEF_tpCor corVencedor;

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

		/* Criar partida */
		debugPartida = PART_CriarPartida(numJogadores);
		/* Se não retornou OK, erro */
		if (debugPartida)
			apresentarFalha();

		/* Executar partida */
		do {
			system("cls");

			/* Jogar turno */
			debugPartida = PART_Jogar();
			/* Se não retornou OK, erro */
			if (debugPartida)
				apresentarFalha();

			printf("Pressione qualquer coisa para continuar...");
			getch();

			/* Testar se em último turno, jogador venceu */
			debugPartida = PART_ChecarVitoria(&venceu, &corVencedor);
			/* Se não retornou OK, erro */
			if (debugPartida)
				apresentarFalha();

		} while(venceu == False);

		/* Anunciar vencedor */
		system("cls");
		debugPartida = PART_AnunciarVencedor(corVencedor);
		/* Se não retornou OK, erro */
		if (debugPartida)	apresentarFalha();

		/* Destruir partida encerrada */
		debugPartida = PART_DestruirPartida();
		/* Se não retornou OK, erro */
		if (debugPartida)	apresentarFalha();

		printf("\n\nDeseja jogar novamente? (s/n)\t");

		/* Pegar desejo do usuário de jogar novamente */
		jogarNovamente = pegarSN();
	} while(jogarNovamente);

	system("cls");
	printf("Obrigado por jogar!\n\n");

	return 0;
}