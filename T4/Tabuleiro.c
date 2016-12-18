/*****************************************************************
 /*
 /* Módulo de implementação: Módulo Tabuleiro
 /*
 /* Nome do arquivo: Tabuleiro.c
 /*
 /****************************************************************/
/* Inclusões do compilador */
 
#include <stdlib.h>

/* Inclusão do respectivo módulo de definição */
#include "Tabuleiro.h"

/* Inclusão de módulos de definição de servidores e de tabelas de definição */
#include "Definicoes.h"
#include "ListaC.h"
#include "Lista.h"

typedef struct TAB_tgTabuleiro {
    LSTC_tppListaC tabuleiro;
    LIS_tppLista azul;
    LIS_tppLista amarelo;
    LIS_tppLista verde;
    LIS_tppLista vermelho;
} TAB_tpTabuleiro;

typedef struct TAB_tgCasa {
    DEF_tpCor scorPeao; /*vai de 0 a 4, 0 significa que nao tem peao na casa*/
    
    DEF_tpCor corDeSaida; /* pra ver se ela é uma casa de
                     saida de base e de qual cor ela é */
    
    DEF_tpBool fim; /*talvez preciso disso pra saber se a casa é
             o final do oscar(linha de chegada) ou nao*/
    
    LIS_tppLista oscar;

	int x; /* Posição x da casa no tabuleiro. Esta posição é relativa ao canto esquerdo da casa. 
		   O canto esquerdo do tabuleiro tem posição 0 e cresce 1 para a direita, para cada casa. */
	int y; /* Posição y da casa no tabuleiro. Esta posição é relativa ao lado inferior da casa. 
		   O lado inferior do tabuleiro tem posição 0 e cresce 1 para cima, para cada casa. */

} TAB_tpCasa;

static void ExcluirCasa (void* casa);

static TAB_tpCondRet PreencheListaRetaFinal(LIS_tppLista lst);

static TAB_tpCondRet TAB_CriarCasa(TAB_tppCasa casa);

static TAB_tppTabuleiro tab;

static void PosicionarCasas();

/* Código do módulo de implementação */

TAB_tpCondRet TAB_CriarTabuleiro() {
    int pos=0, i;
    LSTC_tpCondRet debugLSTC;
    LIS_tpCondRet debugLIS;
    TAB_tpCondRet debugTAB;
    TAB_tpCasa *casa;
    
    if(tab!=NULL) {
        return TAB_CondRetTabJaExiste;
    }//if
    
    tab = (TAB_tppTabuleiro) malloc (sizeof(TAB_tpTabuleiro));
    if (tab==NULL) {
        return TAB_CondRetSemMemoria;
    }//if
    
    debugLSTC = LSTC_CriarListaC(&tab->tabuleiro, ExcluirCasa);
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;          //Checa se houve erro na funcao da lista circular
    }//if
    
    debugLIS = LIS_CriarLista(&tab->azul, ExcluirCasa);
    if(debugLIS){
        return TAB_CondRetErroLista;                   //Checa se houve erro na funcao da lista
    }//if
    
    debugLIS = LIS_CriarLista(&tab->amarelo, ExcluirCasa);
    if(debugLIS){
        return TAB_CondRetErroLista;                   //Checa se houve erro na funcao da lista
    }//if
    
    debugLIS = LIS_CriarLista(&tab->vermelho, ExcluirCasa);
    if(debugLIS){
        return TAB_CondRetErroLista;                   //Checa se houve erro na funcao da lista
    }//if
    
    debugLIS = LIS_CriarLista(&tab->verde, ExcluirCasa);
    if(debugLIS){
        return TAB_CondRetErroLista;                   //Checa se houve erro na funcao da lista
    }//if
        
    //Insere todas as casas
    debugTAB = TAB_CriarCasa(&casa);
    if(debugTAB){
        return debugTAB;
    }//if
    
    debugLSTC = LSTC_InserirElemento(tab->tabuleiro, pos, casa);
    if(debugLSTC) {
        return TAB_CondRetErroListaCircular;
    }//if
    
    while(pos<51){
        debugTAB = TAB_CriarCasa(&casa);
        if(debugTAB){
            return debugTAB;
        }//if
		
        debugLSTC = LSTC_InserirElemento(tab->tabuleiro, pos, casa);
        if(debugLSTC){
            return TAB_CondRetErroListaCircular;
        }//if
        pos++;
    }
    
    //Posiciona as entradas no tabuleiro
    for(i=0;i<4;i++){
        pos=i*13;                   // Determina posicao de entrada no tabuleiro
        
        debugTAB = TAB_CriarCasa(&casa);
        if(debugTAB){
            return debugTAB;
        } //if
        
        casa->corDeSaida = (DEF_tpCor) (i+1);       // Determina cor de saida
        
        debugLSTC = LSTC_AtribuirElemento(tab->tabuleiro, pos, casa);
        if(debugLSTC){
            return TAB_CondRetErroListaCircular;
        }//if
    }//for
    
    //Posiciona as entradas dos oscars
    debugTAB = TAB_CriarCasa(&casa);
    if(debugTAB){
        return debugTAB;
    }//if
	
    casa->oscar = tab->azul;
    debugLSTC = LSTC_AtribuirElemento(tab->tabuleiro, 50, casa);        //Posiciona entrada do oscar azul
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
    debugTAB = TAB_CriarCasa(&casa);
    if(debugTAB){
        return debugTAB;
    }//if
    
    casa->oscar = tab->amarelo;
    debugLSTC = LSTC_AtribuirElemento(tab->tabuleiro, 11, casa);        //Posiciona entrada do oscar amarelo
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
    debugTAB = TAB_CriarCasa(&casa);
    if(debugTAB){
        return debugTAB;
    }//if
    
    casa->oscar = tab->verde;
    debugLSTC = LSTC_AtribuirElemento(tab->tabuleiro, 24, casa);        //Posiciona entrada do oscar vermelho
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
    debugTAB = TAB_CriarCasa(&casa);
    if(debugTAB){
        return debugTAB;
    }//if
    
    casa->oscar = tab->vermelho;
    debugLSTC = LSTC_AtribuirElemento(tab->tabuleiro, 37, casa);        //Posiciona entrada do oscar verde
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
    //Prepara Listas Lineares das Retas Finais
    debugTAB = PreencheListaRetaFinal(tab->azul);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if
    
    debugTAB = PreencheListaRetaFinal(tab->amarelo);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if
    
    debugTAB = PreencheListaRetaFinal(tab->verde);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if
    
    debugTAB = PreencheListaRetaFinal(tab->vermelho);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if

    PosicionarCasas();
	    
    return TAB_CondRetOK;

}

static int Criterio(void* pElemBuscado, void* pElemLista){
	TAB_tppCasa casaLista = (TAB_tppCasa) pElemLista;
	TAB_tppCasa casaBuscada = (TAB_tppCasa) pElemBuscado;

	if(casaLista == casaBuscada){
		return 1;
	}
	else{
		return 0;
	}
}

// Se retornar casa inexistente, ultrapassou o final. Se retornar erro, nao encontrou o peao.

TAB_tpCondRet TAB_RetornarCasa(TAB_tppCasa casa, DEF_tpCor corPeao, int n, TAB_tppCasa* casaRetorno) {
	int i;
	TAB_tppCasa casa_temp = NULL;
	DEF_tpBool booleano;
	int indice;
	LIS_tppLista oscar;
	LIS_tpCondRet debugLis;
	LSTC_tpCondRet debugListaC;

	/* Procurar casa passada na lista circular */
	debugListaC = LSTC_ProcurarElemento(tab->tabuleiro, casa, &indice, Criterio);
	/* Se não retornou OK, erro */
	if(debugListaC && debugListaC != LSTC_CondRetElemInexistente) {
		return TAB_CondRetErroListaCircular;
	}

	/* Testar se a casa passada estava na lista circular ou em um dos oscars */
	if (debugListaC == LSTC_CondRetOK) {
			/* Casa passada está em lista circular */

		int entreiNoOscar = 0;
			
		/* Testar, a cada passo, se está na entrada do oscar */
		for(i = 0; i < n ; i++) {

			/* Pegar casa i casa à frente da inicial */
			debugListaC = LSTC_ObterElemento(tab->tabuleiro, indice + i, (void**)&casa_temp);
			/* Se não retornou OK, erro */
			if(debugListaC)
				return TAB_CondRetErroListaCircular;
			
			/* Testar se esta casa é uma entrada para oscar */
			if(casa_temp->oscar != NULL) {

				if	((casa_temp->oscar == tab->azul && corPeao == AZUL) ||
					(casa_temp->oscar == tab->verde && corPeao == VERDE) ||
					(casa_temp->oscar == tab->amarelo && corPeao == AMARELO) ||
					(casa_temp->oscar == tab->vermelho && corPeao == VERMELHO)) {
						/* Se esta casa for uma entrada para o oscar da cor do peão */

					oscar = casa_temp->oscar;

					/* Começar no início do oscar */
					debugLis = LIS_IrInicioLista(oscar);
					/* Se não retornou OK, erro */
					if (debugLis)	return TAB_CondRetErroLista;

					/* Andar casas restantes */
					debugLis = LIS_AvancarElementoCorrente(casa_temp->oscar, n-i-1);
					/* Se não retornou OK, erro */
					if (debugLis)	return TAB_CondRetErroLista;

					/* Pegar casa final */
					debugLis = LIS_ObterValor(oscar,  (void**)&casa_temp);
					/* Se não retornou OK, erro */
					if (debugLis)	return TAB_CondRetErroLista;

					entreiNoOscar = 1;

					break;
				}	/* if */
			}	/* if */
		}	/* for */

		if (!entreiNoOscar) {
			/* Pegar casa n casas à frente da inicial */
			debugListaC = LSTC_ObterElemento(tab->tabuleiro, indice + n, (void**)&casa_temp);
			/* Se não retornou OK, erro */
			if(debugListaC)
				return TAB_CondRetErroListaCircular;
		}
	}
	else if(debugListaC == LSTC_CondRetElemInexistente) {
			/* Casa passada não está em lista circular (pode estar em um dos oscars) */

		/* Pegar oscar que casa passada pode estar */
		if(corPeao == AZUL) {
			oscar = tab->azul;
		}
		else if(corPeao == AMARELO) {
			oscar = tab->amarelo;
		}
		else if(corPeao == VERDE) {
			oscar = tab->verde;
		}
		else if(corPeao == VERMELHO) {
			oscar = tab->vermelho;
		}

		/* Ir pro início do oscar */
		debugLis = LIS_IrInicioLista(oscar);
		/* Se não retornou OK, erro */
		if (debugLis)
			return TAB_CondRetErroLista;

		/* Procurar casa no oscar */
		debugLis = LIS_ProcurarValor(oscar , casa);
		
		/* Testar se casa estava ou não no oscar */
		if(debugLis == LIS_CondRetOK) {
				/* Se casa foi encontrada no oscar */

			/* Tentar andar n casas à frente */
			debugLis = LIS_AvancarElementoCorrente(oscar, n);
			
			/* Testar se haviam n casa à frente da casa passada */
			switch(debugLis) {
				case LIS_CondRetOK:
						/* Conseguiu andar n casas à frente */

					/* Pega a casa final */
					debugLis = LIS_ObterValor(oscar, (void**)&casa_temp);
					/* Se não retornou OK, erro */
					if(debugLis) return TAB_CondRetErroLista;
					break;
				case LIS_CondRetFimLista:
					/* Não conseguiu andar n casas à frente */

					/* Retorna fim da lista (casa final inexistente) */
					return TAB_CondRetUltrapassouFinal;
					break;
				default:
					/* Se houve qualquer outro retorno, erro */

					return TAB_CondRetErroLista;
			}	/* switch */
		} else if (debugLis == LIS_CondRetNaoAchou) {
				/* Se a casa passada não foi encontrada nem no oscar, retornar erro de inconsistência */

			return TAB_CondRetCasaInexistente;
		} else {
				/* Se houve qualquer outro retorno, erro */

			return TAB_CondRetErroLista;
		}	/* if */
	}

	/* Independente da casa final estar na lista circular ou num oscar, a variável casa_temp contém a casa final */
	*casaRetorno = casa_temp;

    return TAB_CondRetOK;
}

TAB_tpCondRet TAB_ChecarCor(TAB_tppCasa casa, int n, DEF_tpCor corPeao, DEF_tpCor* cRetorno) { 
	TAB_tppCasa casaRetorno;
	TAB_tpCondRet debugTab;

	/* Pegar casa n casa à frente da inicial */
	debugTab = TAB_RetornarCasa(casa, corPeao, n, &casaRetorno);
	/* Se não retornou OK, erro */
	if(debugTab)
		return debugTab;

	/* Cor na casa desejada é a cor da casa pega */
	*cRetorno = casaRetorno->scorPeao;

	return debugTab;
}

TAB_tpCondRet TAB_RetornarCasaDeSaida(DEF_tpCor cor, TAB_tppCasa* casaRetorno) {

	int pos;
	LSTC_tpCondRet debugLSTC;
    
    if(tab==NULL){
        return TAB_CondRetTabInexistente;
    }//if
    
    if (cor==AZUL){
        pos=0;
    }//if
    else if(cor == AMARELO){
        pos=13;
    }//if
    else if(cor == VERDE){
        pos=26;
    }//if
    else if(cor == VERMELHO){
        pos=39;
    }
    
    debugLSTC = LSTC_ObterElemento(tab->tabuleiro, pos, (void**)casaRetorno);
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
	return TAB_CondRetOK;
}

TAB_tpCondRet TAB_EhCasaFinal(TAB_tppCasa casa, DEF_tpBool* finalRetorno){
    if(casa==NULL){
        return TAB_CondRetCasaInexistente;
    }//if
    
    *finalRetorno = casa->fim;
    return TAB_CondRetOK;
}

TAB_tpCondRet TAB_ObterPosicaoCasa(TAB_tppCasa casa, int* x, int* y) {
	if (casa == NULL) {
		return TAB_CondRetCasaInexistente;
	}
	*x = casa->x;
	*y = casa->y;
	return TAB_CondRetOK;
}

TAB_tpCondRet TAB_MudarCorPeaoNaCasa(TAB_tppCasa casa, DEF_tpCor cor) {
	if (casa == NULL) {
		return TAB_CondRetCasaInexistente;
	}
	casa->scorPeao = cor;
	return TAB_CondRetOK;
}

TAB_tpCondRet TAB_DestruirTabuleiro(){
	LIS_tpCondRet debugLIS;
	LSTC_tpCondRet debugLSTC;
    
    if(tab == NULL){
        return TAB_CondRetTabInexistente;
    }//if
    
	debugLIS = LIS_DestruirLista(tab->azul);
    if(debugLIS){
        return TAB_CondRetErroLista;
    }//if
    
	debugLIS = LIS_DestruirLista(tab->amarelo);
    if(debugLIS){
        return TAB_CondRetErroLista;
    }//if
    
	debugLIS = LIS_DestruirLista(tab->vermelho);
    if(debugLIS){
        return TAB_CondRetErroLista;
    }//if
    
	debugLIS = LIS_DestruirLista(tab->verde);
    if(debugLIS){
        return TAB_CondRetErroLista;
    }//if
    
	debugLSTC = LSTC_DestruirListaC(tab->tabuleiro);
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
	free(tab);
	tab = NULL;
	return TAB_CondRetOK;
}

TAB_tpCondRet PreencheListaRetaFinal(LIS_tppLista lst) {
    int i;
    LIS_tpCondRet debugLIS;
    TAB_tppCasa casa;
    for(i=0;i<4;i++){
        TAB_CriarCasa(&casa);
        debugLIS = LIS_InserirElementoApos(lst, casa);
        if(debugLIS){
            return TAB_CondRetErroLista;
        }//if
    }//for
    TAB_CriarCasa(&casa);
    casa->fim=True;
    debugLIS = LIS_InserirElementoApos(lst, casa);
    if(debugLIS){
        return TAB_CondRetErroLista;
    }//if
    return TAB_CondRetOK;
}


void ExcluirCasa(void* casa){
    TAB_tppCasa aux;
    aux = (TAB_tppCasa) casa;
    free(aux);
}

TAB_tpCondRet TAB_CriarCasa(TAB_tppCasa* casa){
    *casa = (TAB_tppCasa) malloc(sizeof(TAB_tpCasa));
    if(*casa==NULL){
        return TAB_CondRetSemMemoria;
    }
    else{
        (*casa)->scorPeao = SEM_COR;
        (*casa)->corDeSaida = SEM_COR;
        (*casa)->fim = False;
        (*casa)->oscar = NULL;
        return TAB_CondRetOK;
    }
}

static void PosicionarCasas(){

	TAB_tppCasa casa_temp;
	int i;

	for(i = -1; i <= 4; i++) {
		LSTC_ObterElemento(tab->tabuleiro, i, (void**)&casa_temp);
		casa_temp->x = (13 - i);
		casa_temp->y = 6 ;
		LSTC_ObterElemento(tab->tabuleiro, i+6 , (void**)&casa_temp);
		casa_temp->x = 8;
		casa_temp->y = (4 - i);
		LSTC_ObterElemento(tab->tabuleiro, i+13 , (void**)&casa_temp);
		casa_temp->x = 6;
		casa_temp->y = (i + 1);
		LSTC_ObterElemento(tab->tabuleiro, i+19 , (void**)&casa_temp);
		casa_temp->x = (4 - i);
		casa_temp->y = 6;
		LSTC_ObterElemento(tab->tabuleiro, i+26 , (void**)&casa_temp);
		casa_temp->x = (i + 1);
		casa_temp->y = 8;
		LSTC_ObterElemento(tab->tabuleiro, i+32 , (void**)&casa_temp);
		casa_temp->x = 6;
		casa_temp->y = (10 + i);
		LSTC_ObterElemento(tab->tabuleiro, i+39 , (void**)&casa_temp);
		casa_temp->x = 8;
		casa_temp->y = (13 - i);
		LSTC_ObterElemento(tab->tabuleiro, i+45 , (void**)&casa_temp);
		casa_temp->x = (10 + i);
		casa_temp->y = 8;
	}
	LSTC_ObterElemento(tab->tabuleiro, 11, (void**)&casa_temp);
	casa_temp->x = 7;
	casa_temp->y = 0;
	LSTC_ObterElemento(tab->tabuleiro, 24, (void**)&casa_temp);
	casa_temp->x = 0;
	casa_temp->y = 7;
	LSTC_ObterElemento(tab->tabuleiro, 37, (void**)&casa_temp);
	casa_temp->x = 7;
	casa_temp->y = 14;
	LSTC_ObterElemento(tab->tabuleiro, 50, (void**)&casa_temp);
	casa_temp->x = 14;
	casa_temp->y = 7;
	
}



/********** Fim da implementação: modulo **********/
