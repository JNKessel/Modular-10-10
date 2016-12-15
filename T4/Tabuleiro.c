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

static TAB_tppTabuleiro tab;

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
    
    casa->oscar = tab->vermelho;
    debugLSTC = LSTC_AtribuirElemento(tab->tabuleiro, 24, casa);        //Posiciona entrada do oscar vermelho
    if(debugLSTC){
        return TAB_CondRetErroListaCircular;
    }//if
    
    debugTAB = TAB_CriarCasa(&casa);
    if(debugTAB){
        return debugTAB;
    }//if
    
    casa->oscar = tab->verde;
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
    
    debugTAB = PreencheListaRetaFinal(tab->vermelho);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if
    
    debugTAB = PreencheListaRetaFinal(tab->verde);
    if(debugTAB){                                                      //Verifica se nao houve erro na funcao
        return debugTAB;
    }//if

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
	TAB_tppCasa casa_temp;
	DEF_tpBool booleano;
	int indice;
	LIS_tppLista lis_aux;
	LIS_tpCondRet debugLis;
	LSTC_tpCondRet debugListaC;

	debugListaC = LSTC_ProcurarElemento(tab->tabuleiro, casa, &indice, Criterio);
	
	if(debugListaC == LSTC_CondRetListaInexistente 
	    || debugListaC == LSTC_CondRetErroNo){
		return TAB_CondRetErroListaCircular;
	}

	if(debugListaC == LSTC_CondRetElemInexistente) {
		if(corPeao == AZUL){
			debugLis = LIS_ProcurarValor(tab->azul , casa);
			lis_aux = tab->azul;
		}
		else if(corPeao == AMARELO){
			debugLis = LIS_ProcurarValor(tab->amarelo, casa);
			lis_aux = tab->amarelo;
		}
		else if(corPeao == VERDE){
			debugLis = LIS_ProcurarValor(tab->verde, casa);
			lis_aux = tab->verde;
		}
		else if(corPeao == VERMELHO){
			debugLis = LIS_ProcurarValor(tab->vermelho, casa);
			lis_aux = tab->vermelho;
		}
		
		
		if(!debugLis){
			debugLis = LIS_AvancarElementoCorrente(lis_aux,n);
			switch(debugLis){
				case LIS_CondRetOK:
					debugLis = LIS_ObterValor(lis_aux, (void**) &casa_temp);
					if(debugLis) return TAB_CondRetErroLista;
					else{
						*casaRetorno = casa_temp;
						return TAB_CondRetOK;
					}
					
				case LIS_CondRetFimLista:
					return TAB_CondRetUltrapassouFinal;
					
				case LIS_CondRetListaVazia:
					return TAB_CondRetErroLista;
				default:
					;
			}
		}
		else if (debugLis == LIS_CondRetNaoAchou) TAB_CondRetErro;
		else return TAB_CondRetErroLista;
	}

	for(i=1; i<=n ; i++){
		debugListaC = LSTC_ObterElemento(tab->tabuleiro, indice + i, (void**)&casa_temp);
		
		if(debugListaC) return TAB_CondRetErroListaCircular;
		
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


	*casaRetorno = casa_temp;

    return TAB_CondRetOK;
}

TAB_tpCondRet TAB_ChecarDisponivel(TAB_tppCasa casa, int n, DEF_tpCor corPeao, DEF_tpCor* cRetorno) { 
	TAB_tppCasa casaRetorno;
	TAB_tpCondRet debugTab;
	
	debugTab = TAB_RetornarCasa(casa, corPeao, n, &casaRetorno);
	
	if(!debugTab) *cRetorno = casaRetorno->scorPeao;
	
	else *cRetorno = SEM_COR;

	return debugTab;
}

TAB_tpCondRet TAB_RetornarCasaDeSaida(DEF_tpCor cor, TAB_tppCasa* casaRetorno){

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



/********** Fim da implementação: modulo **********/
