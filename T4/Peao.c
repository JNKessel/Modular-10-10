/*******************************************************
* $MCD Modulo de implementação: Modulo Peao
* Arquivo:   Peao.c
*
* Autores: jnk - Julio Neuman Kessel
*******************************************************/

#include <stdlib.h>
#include "Peao.h"
#include "Definicoes.h"

typedef enum PEAO_tgEstado {
    Base,
    Tabuleiro,
    Final
} PEAO_tpEstado;

typedef struct PEAO_tgPeao {
    TAB_tppCasa pos;
    PEAO_tpEstado estado;
    DEF_tpCor cor;
    int numero;
} PEAO_tpPeao;

PEAO_tpCondRet PEAO_CriarPeao(PEAO_tppPeao* pPeao, DEF_tpCor Cor, int iNum) {

    *pPeao = (PEAO_tppPeao)malloc(sizeof(PEAO_tpPeao));

    if(*pPeao == NULL) {
        return PEAO_CondRetSemMemoria;
    }

	if(iNum < 1 || iNum > 4) {
		return PEAO_CondRetNumeroInvalido;
	}

	(*pPeao)->pos = NULL;
	(*pPeao)->estado = Base;
	(*pPeao)->cor = Cor;
	(*pPeao)->numero = iNum;

    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_AndarPeao(PEAO_tppPeao pPeao, int n) {

	TAB_tppCasa casa;
	TAB_tpCondRet debugTabuleiro;
	DEF_tpBool ehFinal;

	if(pPeao == NULL) {
		return PEAO_CondRetPeaoInexistente;
	}

	if (pPeao->estado == Base) {
		return PEAO_CondRetPeaoEstaBase;
	}

	/* Pegar casa destino */
	debugTabuleiro = TAB_RetornarCasa(pPeao->pos, pPeao->cor, n, &casa);
	/* Se não retornou OK, erro */
	if (debugTabuleiro)	return PEAO_CondRetErroTabuleiro;

	/* Avisar para casa antiga que não há mais peão lá */
    debugTabuleiro = TAB_MudarCorPeaoNaCasa(pPeao -> pos, SEM_COR);
	/* Se não retornou OK, erro */
	if (debugTabuleiro)	return PEAO_CondRetErroTabuleiro;

	pPeao->pos = casa;

	/* Avisar para nova casa que agora há um peão lá */
    debugTabuleiro = TAB_MudarCorPeaoNaCasa(pPeao -> pos, pPeao -> cor);
	/* Se não retornou OK, erro */
	if (debugTabuleiro)	return PEAO_CondRetErroTabuleiro;

	/* Ver se é casa final */
	debugTabuleiro = TAB_EhCasaFinal(casa, &ehFinal);
	/* Se não retornou OK, erro */
	if (debugTabuleiro)	return PEAO_CondRetErroTabuleiro;

	if (ehFinal == True) {
		pPeao->estado = Final;
	}
	
	return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_SairBasePeao(PEAO_tppPeao pPeao) {
  
	TAB_tppCasa casa;
	TAB_tpCondRet debugTabuleiro;

	if(pPeao == NULL) {
		return PEAO_CondRetPeaoInexistente;
	}

	if (pPeao->estado != Base) {
		return PEAO_CondRetPeaoNaoEstaBase;
	}

	/* Pegar casa de saída */
    debugTabuleiro = TAB_RetornarCasaDeSaida(pPeao -> cor, &casa);
	/* Se não retornou OK, erro */
	if (debugTabuleiro)	return PEAO_CondRetErroTabuleiro;
  
    pPeao -> pos = casa;
    pPeao -> estado = Tabuleiro;

	/* Avisar para nova casa que agora há um peão lá */
    debugTabuleiro = TAB_MudarCorPeaoNaCasa(pPeao -> pos, pPeao -> cor);
	/* Se não retornou OK, erro */
	if (debugTabuleiro)	return PEAO_CondRetErroTabuleiro;

    return PEAO_CondRetOK;
} 

PEAO_tpCondRet PEAO_EstaPeaoFinal(PEAO_tppPeao pPeao, DEF_tpBool* BoolRet) {

	if(pPeao == NULL) {
		return PEAO_CondRetPeaoInexistente;
	}

	if (pPeao->estado == Final)
		*BoolRet = True;
	else
		*BoolRet = False;

    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_EstaPeaoBase(PEAO_tppPeao pPeao, DEF_tpBool* BoolRet) {

	if(pPeao == NULL) {
		return PEAO_CondRetPeaoInexistente;
	}

	if (pPeao->estado == Base)
		*BoolRet = True;
	else
		*BoolRet = False;

    return PEAO_CondRetOK;
}
 
PEAO_tpCondRet PEAO_ObterNumeroPeao(PEAO_tppPeao pPeao, int* NumRet) {

	if(pPeao == NULL) {
		return PEAO_CondRetPeaoInexistente;
	}

	*NumRet = pPeao -> numero;

    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_ObterCasaPeao(PEAO_tppPeao pPeao, TAB_tppCasa * casaRetorno) {

	if(pPeao == NULL) {
		return PEAO_CondRetPeaoInexistente;
	}
	
	*casaRetorno = pPeao->pos;
	
	return PEAO_CondRetOK;
}
 
PEAO_tpCondRet PEAO_ChecarMovimentoDisponivelPeao(PEAO_tppPeao pPeao, int dado, DEF_tpBool* BoolRet, DEF_tpCor* CorRet) {

	DEF_tpCor corNaCasa;
	TAB_tpCondRet debugTabuleiro;

	if (pPeao == NULL) {
		return PEAO_CondRetPeaoInexistente;
	}

	if (pPeao->estado == Base) {
			/* Peão está na base */

		/* Se peão está na base sua posição deve ser NULL */
		if (pPeao->pos != NULL) {
			return PEAO_CondRetInconsistencia;
		}

		if (dado != 6) {
				/* Se jogador não tirou 6 no dado, não pode sair da base */

			*BoolRet = False;
		} else {
				/* Se tirou 6 no dado, pode sair da base se não tiver um peão da mesma cor na casa de saída */

			TAB_tppCasa casaSaida;

			/* Pegar casa de saída */
			debugTabuleiro = TAB_RetornarCasaDeSaida(pPeao->cor, &casaSaida);
			/* Se não retornou OK, erro */
			if (debugTabuleiro)
				return PEAO_CondRetErroTabuleiro;

			/* Checar qual cor está na casa de saída do peão */
			debugTabuleiro = TAB_ChecarCor(casaSaida, 0, pPeao->cor, &corNaCasa);
			/* Se não retornou OK, erro */
			if (debugTabuleiro)
				return PEAO_CondRetErroTabuleiro;

			if (corNaCasa == pPeao->cor) {
				*BoolRet = False;
			} else {
				*BoolRet = True;
				*CorRet = corNaCasa;
			}	/* if */
		}	/* if */
	} else {
			/* Se o peão não está na base: */
	
		/* Pegar cor do peão que está na casa destino */
		debugTabuleiro = TAB_ChecarCor(pPeao->pos, dado, pPeao ->cor, &corNaCasa);
		/* Se a casa n casa a frente do peão não existe, então o movimento não está disponível */
		if (debugTabuleiro == TAB_CondRetUltrapassouFinal) {
			*BoolRet = False;
			return PEAO_CondRetOK;
		}
		/* Se não retornou OK, erro */
		if (debugTabuleiro)
			return PEAO_CondRetErroTabuleiro;
  
		if (corNaCasa == pPeao->cor) {
				/* Se a cor for igual à do peão, movimento indisponível */

			*BoolRet = False;
		} else {
				/* Se a cor for diferente da do peão, movimento disponível */
  
			*CorRet = corNaCasa;
			*BoolRet = True;
		}
	}
    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_VoltarBasePeao(PEAO_tppPeao pPeao) {

	TAB_tpCondRet debugTabuleiro;
  
	if(pPeao == NULL) {
		return PEAO_CondRetPeaoInexistente;
	}

	if (pPeao->estado == Base) {
		return PEAO_CondRetPeaoEstaBase;
	}

	/* Avisar para casa antiga que não há mais peão lá */
    debugTabuleiro = TAB_MudarCorPeaoNaCasa(pPeao -> pos, SEM_COR);
	/* Se não retornou OK, erro */
	if (debugTabuleiro)	return PEAO_CondRetErroTabuleiro;

    pPeao -> pos = NULL;
    pPeao -> estado = Base;

    return PEAO_CondRetOK;
}

PEAO_tpCondRet PEAO_DestruirPeao(PEAO_tppPeao pPeao) {

	if(pPeao == NULL){
	
		return PEAO_CondRetPeaoInexistente;
	
	}

    free(pPeao);

    return PEAO_CondRetOK;
}






