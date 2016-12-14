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
    LIS_tppLista vermelho;
    LIS_tppLista verde;
} TAB_tpTabuleiro;

typedef struct TAB_tgCasa {
    DEF_tpCor scorPeao; /*vai de 0 a 4, 0 significa que nao tem peao na casa*/
    
    DEF_tpCor corDeSaida; /* pra ver se ela é uma casa de
                     saida de base e de qual cor ela é */
    
    DEF_tpBool fim; /*talvez preciso disso pra saber se a casa é
             o final do oscar(linha de chegada) ou nao*/
    
    LIS_tppLista oscar;
} TAB_tpCasa;

static void ExcluirCasa (void* casa);

static TAB_tpCondRet PreencheListaRetaFinal(LIS_tppLista lst);

static TAB_tpCondRet TAB_CriarCasa(TAB_tppCasa casa);

/* Código do módulo de implementação */

TAB_tpCondRet TAB_CriarTabuleiro(TAB_tppTabuleiro* tab) {
    int pos=0, i;
    LSTC_tpCondRet debugLSTC;
    LIS_tpCondRet debugLIS;
    TAB_tpCondRet debugTAB;
    TAB_tpCasa *casa;
    
    if(*tab!=NULL) {
        return TAB_CondRetTabJaExiste;
    }//if
    
    *tab = (TAB_tppTabuleiro) malloc (sizeof(TAB_tpTabuleiro));
    if (*tab==NULL) {
        return TAB_CondRetSemMemoria;
    }//if
    
    debugLSTC = LSTC_CriarListaC(&(*tab)->tabuleiro, ExcluirCasa);
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;          //Checa se houve erro na funcao da lista circular
    }//if
    
    debugLIS = LIS_CriarLista(&(*tab)->azul, ExcluirCasa);
    if(debugLIS){
        return TAB_CondRetErroLista;                   //Checa se houve erro na funcao da lista
    }//if
    
    debugLIS = LIS_CriarLista(&(*tab)->amarelo, ExcluirCasa);
    if(debugLIS){
        return TAB_CondRetErroLista;                   //Checa se houve erro na funcao da lista
    }//if
    
    debugLIS = LIS_CriarLista(&(*tab)->vermelho, ExcluirCasa);
    if(debugLIS){
        return TAB_CondRetErroLista;                   //Checa se houve erro na funcao da lista
    }//if
    
    debugLIS = LIS_CriarLista(&(*tab)->verde, ExcluirCasa);
    if(debugLIS){
        return TAB_CondRetErroLista;                   //Checa se houve erro na funcao da lista
    }//if
        
    //Insere todas as casas
    debugTAB = TAB_CriarCasa(casa);
    if(debugTAB){
        return debugTAB;
    }//if
    
    debugLSTC = LSTC_InserirElemento((*tab)->tabuleiro, pos, casa);
    if(debugLSTC) {
        return TAB_CondRetErroListaCircular;
    }//if
    
    while(pos<51){
        debugTAB = TAB_CriarCasa(casa);
        if(debugTAB){
            return debugTAB;
        }//if
		
        debugLSTC = LSTC_InserirElemento((*tab)->tabuleiro, pos, casa);
        if(debugLSTC){
            return TAB_CondRetErroListaCircular;
        }//if
        pos++;
    }
    
    //Posiciona as entradas no tabuleiro
    for(i=0;i<4;i++){
        pos=i*13;                   // Determina posicao de entrada no tabuleiro
        
        debugTAB = TAB_CriarCasa(casa);
        if(debugTAB){
            return debugTAB;
        } //if
        
        casa->corDeSaida = (DEF_tpCor) (i+1);       // Determina cor de saida
        
        debugLSTC = LSTC_AtribuirElemento((*tab)->tabuleiro, pos, casa);
        if(debugLSTC){
            return TAB_CondRetErroListaCircular;
        }//if
    }//for
    
    //Posiciona as entradas dos oscars
    debugTAB = TAB_CriarCasa(casa);
    if(debugTAB){
        return debugTAB;
    }//if
	
    casa->oscar = (*tab)->azul;
    debugLSTC = LSTC_AtribuirElemento((*tab)->tabuleiro, 50, casa);        //Posiciona entrada do oscar azul
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
    debugTAB = TAB_CriarCasa(casa);
    if(debugTAB){
        return debugTAB;
    }//if
    
    casa->oscar = (*tab)->amarelo;
    debugLSTC = LSTC_AtribuirElemento((*tab)->tabuleiro, 11, casa);        //Posiciona entrada do oscar amarelo
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
    debugTAB = TAB_CriarCasa(casa);
    if(debugTAB){
        return debugTAB;
    }//if
    
    casa->oscar = (*tab)->vermelho;
    debugLSTC = LSTC_AtribuirElemento((*tab)->tabuleiro, 24, casa);        //Posiciona entrada do oscar vermelho
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
    debugTAB = TAB_CriarCasa(casa);
    if(debugTAB){
        return debugTAB;
    }//if
    
    casa->oscar = (*tab)->verde;
    debugLSTC = LSTC_AtribuirElemento((*tab)->tabuleiro, 37, casa);        //Posiciona entrada do oscar verde
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
    //Prepara Listas Lineares das Retas Finais
    debugTAB = PreencheListaRetaFinal((*tab)->azul);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if
    
    debugTAB = PreencheListaRetaFinal((*tab)->amarelo);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if
    
    debugTAB = PreencheListaRetaFinal((*tab)->vermelho);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if
    
    debugTAB = PreencheListaRetaFinal((*tab)->verde);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if

    return TAB_CondRetOK;

}

int Criterio(void* pElemBuscado, void* pElemLista){
	TAB_tppCasa casaLista = (TAB_tppCasa) pElemLista;
	TAB_tppCasa casaBuscada = (TAB_tppCasa) pElemBuscado;

	if(casaLista == casaBuscada){
		return 1;
	}
	else{
		return 0;
	}
}

TAB_tpCondRet TAB_ChecarDisponivel(TAB_tppTabuleiro tab, TAB_tppCasa c, int n, DEF_tpCor corPeao, DEF_tpCor* cRetorno) { /* Esta funcao esta associada com a implementacao da dinamica da partida */
	int i;
	TAB_tppCasa casa_temp;
	DEF_tpBool booleano;
	int indice;
	LIS_tppLista lis_aux;

	LSTC_ProcurarElemento(tab->tabuleiro, c, &indice, Criterio);

	
	if(indice == -1) {
		if(corPeao == AZUL){
			LIS_ProcurarValor(tab->azul , c);
		}
		else if(corPeao == AMARELO){
			LIS_ProcurarValor(tab->amarelo, c);
		}
		else if(corPeao == VERDE){
			LIS_ProcurarValor(tab->amarelo, c);
		}
		else if(corPeao == VERMELHO){
			LIS_ProcurarValor(tab->amarelo, c);
		}
	}

	for(i=1; i<=n ; i++){
		LSTC_ObterElemento(tab->tabuleiro, indice + i, (void**)&casa_temp);
		if(casa_temp->oscar != NULL){
			if(casa_temp->oscar == tab->azul 
				&& corPeao == AZUL){
					LIS_AvancarElementoCorrente(casa_temp->oscar, n-i);
					lis_aux = casa_temp->oscar;
					LIS_ObterValor(casa_temp->oscar ,  (void**)&casa_temp);
					LIS_IrInicioLista(lis_aux);
					break;
			}

			else if(casa_temp->oscar == tab->verde 
				&& corPeao == VERDE){
					LIS_AvancarElementoCorrente(casa_temp->oscar, n-i);
					lis_aux = casa_temp->oscar;
					LIS_ObterValor(casa_temp->oscar ,  (void**)&casa_temp);
					LIS_IrInicioLista(lis_aux);
					break;
			}

			else if(casa_temp->oscar == tab->amarelo 
				&& corPeao == AMARELO){
					LIS_AvancarElementoCorrente(casa_temp->oscar, n-i);
					lis_aux = casa_temp->oscar;
					LIS_ObterValor(casa_temp->oscar ,  (void**)&casa_temp);
					LIS_IrInicioLista(lis_aux);
					break;
			}

			else if(casa_temp->oscar == tab->vermelho 
				&& corPeao == VERMELHO){
					LIS_AvancarElementoCorrente(casa_temp->oscar, n-i);
					lis_aux = casa_temp->oscar;
					LIS_ObterValor(casa_temp->oscar ,  (void**)&casa_temp);
					LIS_IrInicioLista(lis_aux);
					break;
			}
		}
	}


	*cRetorno = casa_temp->scorPeao;

    return TAB_CondRetOK;
}

TAB_tpCondRet TAB_RetornarCasa(DEF_tpCor c, TAB_tppCasa casa, int n, TAB_tppCasa casaRetorno) {
	/* Esta funcao esta associada com a dinamica do jogo */

	return TAB_CondRetOK;
}

TAB_tpCondRet TAB_RetornarCasaDeSaida(/*TAB_tppTabuleiro tab,*/ DEF_tpCor cor, TAB_tppCasa* casaRetorno){// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TIRAR COMENTÁRIO PEDRO FRAYHA GRRRRRRRRRRRRRR
    TAB_tppTabuleiro tab; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TIRAR ISSO DAQUI PEDRO FRAYHA SEU FEIO
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
    else if(cor == VERMELHO){
        pos=26;
    }//if
    else if(cor == VERDE){
        pos=39;
    }
    
    debugLSTC = LSTC_ObterElemento(tab->tabuleiro, pos, (void**)casaRetorno);
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
	return TAB_CondRetOK;
}

TAB_tpCondRet TAB_EhCasaFinal(TAB_tppCasa c, DEF_tpBool* finalRetorno){
    if(c==NULL){
        return TAB_CondRetCasaInexistente;
    }//if
    
    *finalRetorno = c->fim;
    return TAB_CondRetOK;
}

TAB_tpCondRet TAB_DestruirTabuleiro(TAB_tppTabuleiro tab){
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
	return TAB_CondRetOK;
}

TAB_tpCondRet PreencheListaRetaFinal(LIS_tppLista lst) {
    int i;
    LIS_tpCondRet debugLIS;
    TAB_tppCasa c;
    for(i=0;i<4;i++){
        TAB_CriarCasa(c);
        debugLIS = LIS_InserirElementoApos(lst, c);
        if(debugLIS){
            return TAB_CondRetErroLista;
        }//if
    }//for
    TAB_CriarCasa(c);
    c->fim=True;
    debugLIS = LIS_InserirElementoApos(lst, c);
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

TAB_tpCondRet TAB_CriarCasa(TAB_tppCasa casa){
    casa = (TAB_tppCasa) malloc(sizeof(TAB_tpCasa));
    if(casa==NULL){
        return TAB_CondRetSemMemoria;
    }
    else{
        casa->scorPeao = SEM_COR;
        casa->corDeSaida = SEM_COR;
        casa->fim = False;
        casa->oscar = NULL;
        return TAB_CondRetOK;
    }
}



/********** Fim da implementação: modulo **********/
