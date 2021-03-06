/***************************************************************************
*  $MCI M�dulo de implementa��o: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              LISTA.c
*  Letras identificadoras:      LIS
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor      Data         Observa��es
*     6        gsc      26/jun/2018   inclus�o de controles de teste com o m�dulo Conta 
*	    5		   gsc, tgf   11/mai/2018   criadas as fun��es ObterQtdElem e ExcluirPtrParaElem 
*     4        avs      01/fev/2006   criar linguagem script simb�lica
*     3        avs      08/dez/2004   uniformiza��o dos exemplos
*     2        avs      07/jul/2003   unifica��o de todos os m�dulos em um s� projeto
*     1        avs      16/abr/2003   in�cio desenvolvimento
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>

#define LISTA_OWN
#include "LISTA.h"

#ifdef _DEBUG
   #include   "Generico.h"
   #include   "CEspDin.h"
   #include   "Conta.h"
   #include   "..\\tabelas\\IdTiposEspaco.def"
#endif

#undef LISTA_OWN

/***********************************************************************
*  $TC Tipo de dados: LIS Elemento da lista
***********************************************************************/
typedef struct tagElemLista {
	void * pValor ;	//Ponteiro para o valor contido no elemento
  struct tagElemLista * pAnt ; //Ponteiro para o elemento predecessor
  struct tagElemLista * pProx ; //Ponteiro para o elemento sucessor
} tpElemLista ;

/***********************************************************************
*  $TC Tipo de dados: LIS Descritor da cabe�a de lista
***********************************************************************/
typedef struct LIS_tagLista {

	//Ponteiro para a origem da lista
    tpElemLista * pOrigemLista ;

	//Ponteiro para o final da lista
    tpElemLista * pFimLista ; 

	//Ponteiro para o elemento corrente da lista
	tpElemLista * pElemCorr ;

	//N�mero de elementos da lista
	int numElem ;

	///Ponteiro para a fun��o de destrui��o do valor contido em um elemento
	void ( * ExcluirValor ) ( void * pValor ) ;

} LIS_tpLista ;

/************ PROT�TIPOS DAS FUN��ES ENCAPSULADAS NO M�DULO ***************/
static void LiberarElemento( LIS_tppLista   pLista, tpElemLista * pElem ) ;
static tpElemLista * CriarElemento( LIS_tppLista pLista, void * pValor ) ;
static void LimparCabeca( LIS_tppLista pLista ) ;

/***********  C�DIGO DAS FUN��ES EXPORTADAS PELO M�DULO  ******************/

/***************************************************************************
*  Fun��o: LIS  &Criar lista
***************************************************************************/
LIS_tppLista LIS_CriarLista( void   ( * ExcluirValor ) ( void * pDado ) )
{
      LIS_tpLista * pLista = NULL ;

      #ifdef _DEBUG
        CNT_CONTAR("LIS_CriarLista - Entrou e declarou variaveis") ;
      #endif

      pLista = ( LIS_tpLista * ) malloc( sizeof( LIS_tpLista )) ;
      if ( pLista == NULL )
      {
         return NULL ;
      } //fim if

      LimparCabeca( pLista ) ;

      pLista->ExcluirValor = ExcluirValor ;

      #ifdef _DEBUG
         CED_DefinirTipoEspaco( pLista, LIS_TipoEspacoLista ) ;
      #endif

      return pLista ;

} /***************** Fim fun��o: LIS &Criar lista **********************/

/***************************************************************************
*  Fun��o: LIS  &Destruir lista
***************************************************************************/
void LIS_DestruirLista( LIS_tppLista pLista )
{
      #ifdef _DEBUG
        assert( pLista != NULL ) ;
        CNT_CONTAR("LIS_DestruirLista - Entrou e assertou pLista != NULL") ;
      #endif

      LIS_EsvaziarLista( pLista ) ;

      free( pLista ) ;

} /***************** Fim fun��o: LIS &Destruir lista *******************/

/***************************************************************************
*  Fun��o: LIS  &Esvaziar lista
***************************************************************************/
void LIS_EsvaziarLista( LIS_tppLista pLista )
{
      tpElemLista * pElem ;
      tpElemLista * pProx ;

      #ifdef _DEBUG
        assert( pLista != NULL ) ;
        CNT_CONTAR("LIS_EsvaziarLista - Entrou, declarou variaveis e assertou pLista != NULL") ;
      #endif

      pElem = pLista->pOrigemLista ;
      while ( pElem != NULL )
      {
         pProx = pElem->pProx ;
         LiberarElemento( pLista , pElem ) ;
         pElem = pProx ;
      } //fim while

      LimparCabeca( pLista ) ;

} /***************** Fim fun��o: LIS &Esvaziar lista *******************/

/***************************************************************************
*  Fun��o: LIS  &Inserir elemento antes
***************************************************************************/
LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista, void * pValor )
{
      tpElemLista * pElem ;

      #ifdef _DEBUG
        assert( pLista != NULL ) ;
        CNT_CONTAR("LIS_InserirElementoAntes - Entrou, declarou variaveis e assertou pLista != NULL") ;
      #endif

      // Criar elemento a inserir antes

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } //fim if

      // Encadear o elemento antes do elemento corrente

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pAnt != NULL )
            {
               pElem->pAnt  = pLista->pElemCorr->pAnt ;
               pLista->pElemCorr->pAnt->pProx = pElem ;
            } else
            {
               pLista->pOrigemLista = pElem ;
            } //fim if

            pElem->pProx = pLista->pElemCorr ;
            pLista->pElemCorr->pAnt = pElem ;
         } //fim if

         pLista->pElemCorr = pElem ;

         return LIS_CondRetOK ;

} /********* Fim fun��o: LIS &Inserir elemento antes *******************/

/***************************************************************************
*  Fun��o: LIS  &Inserir elemento ap�s
***************************************************************************/
LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista, void * pValor )   
{
      tpElemLista * pElem ;

      #ifdef _DEBUG
        assert( pLista != NULL ) ;
        CNT_CONTAR("LIS_InserirElementoApos - Entrou, declarou variaveis e assertou pLista != NULL") ;
      #endif

      // Criar elemento a inserir ap�s

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } //fim if

      // Encadear o elemento ap�s o elemento

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pProx != NULL )
            {
               pElem->pProx  = pLista->pElemCorr->pProx ;
               pLista->pElemCorr->pProx->pAnt = pElem ;
            } else
            {
               pLista->pFimLista = pElem ;
            } //fim if

            pElem->pAnt = pLista->pElemCorr ;
            pLista->pElemCorr->pProx = pElem ;

         } //fim if
                  
         pLista->pElemCorr = pElem ;
                  
         return LIS_CondRetOK ;

} /********* Fim fun��o: LIS &Inserir elemento ap�s ********************/

/***************************************************************************
*  Fun��o: LIS  &Excluir elemento
***************************************************************************/
LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista )
{
      tpElemLista * pElem ;

      #ifdef _DEBUG
        assert( pLista  != NULL ) ;
        CNT_CONTAR("LIS_ExcluirElemento - Entrou, declarou variaveis e assertou pLista != NULL") ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
         return LIS_CondRetListaVazia ;
      } //fim if

      pElem = pLista->pElemCorr ;

      // Desencadeia � esquerda

         if ( pElem->pAnt != NULL )
         {
            pElem->pAnt->pProx   = pElem->pProx ;
            pLista->pElemCorr    = pElem->pAnt ;
         } else {
            pLista->pElemCorr    = pElem->pProx ;
            pLista->pOrigemLista = pLista->pElemCorr ;
         } //fim if

      // Desencadeia � direita

         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem->pAnt ;
         } else
         {
            pLista->pFimLista = pElem->pAnt ;
         } //fim if

      LiberarElemento( pLista , pElem ) ;

      return LIS_CondRetOK ;

} /************* Fim fun��o: LIS &Excluir elemento *********************/

/***************************************************************************
*  Fun��o: LIS  &Excluir ponteiro para elemento
***************************************************************************/
LIS_tpCondRet LIS_ExcluirPtrParaElemento( LIS_tppLista pLista )
{
  tpElemLista *pElem ;

  #ifdef _DEBUG
    assert( pLista  != NULL ) ;
    CNT_CONTAR("LIS_ExcluirPtrParaElemento - Entrou, declarou variaveis e assertou pLista != NULL") ;
  #endif

  if ( pLista->pElemCorr == NULL )
  {
     return LIS_CondRetListaVazia ;
  } //fim if

  pElem = pLista->pElemCorr ;

  // Desencadeia � esquerda

     if ( pElem->pAnt != NULL )
     {
        pElem->pAnt->pProx   = pElem->pProx ;
        pLista->pElemCorr    = pElem->pAnt ;
     } else {
        pLista->pElemCorr    = pElem->pProx ;
        pLista->pOrigemLista = pLista->pElemCorr ;
     } //fim if

  // Desencadeia � direita

     if ( pElem->pProx != NULL )
     {
        pElem->pProx->pAnt = pElem->pAnt ;
     } else
     {
        pLista->pFimLista = pElem->pAnt ;
     } //fim if

  free(pElem) ;

pLista->numElem-- ;

return LIS_CondRetOK ;

} /**** Fim fun��o: LIS &Excluir ponteiro para elemento ****************/

/***************************************************************************
*  Fun��o: LIS  &Obter quantidade de elementos
***************************************************************************/
int LIS_ObterQtdElem( LIS_tppLista pLista ) {

	int qtd;

  #ifdef _DEBUG
    assert( pLista  != NULL ) ;
    CNT_CONTAR("LIS_ObterQtdElem - Entrou, declarou variaveis e assertou pLista != NULL") ;
  #endif

  qtd = pLista->numElem;
  return qtd;

} /******** Fim fun��o: LIS &Obter quantidade de elementos ****************/

/***************************************************************************
*  Fun��o: LIS  &Obter refer�ncia para o valor contido no elemento
***************************************************************************/
void * LIS_ObterValor( LIS_tppLista pLista ) {

	#ifdef _DEBUG
    assert( pLista != NULL ) ;
    CNT_CONTAR("LIS_ObterValor - Entrou e assertou pLista != NULL") ;
  #endif

    if ( pLista->pElemCorr == NULL )
    {
		return NULL ;
    } //fim if

    return pLista->pElemCorr->pValor ;

} /* Fim fun��o: LIS &Obter referencia para valor contido no elemento */

/***************************************************************************
*  Fun��o: LIS  &Ir para o elemento inicial
***************************************************************************/
void LIS_IrInicioLista( LIS_tppLista pLista ) {

	#ifdef _DEBUG
    assert( pLista != NULL ) ;
    CNT_CONTAR("LIS_IrInicioLista - Entrou e assertou pLista != NULL") ;
  #endif

  pLista->pElemCorr = pLista->pOrigemLista ;

} /*************** Fim fun��o: LIS &Ir inicio lista ********************/

/***************************************************************************
*  Fun��o: LIS  &Ir para o elemento final
***************************************************************************/
void LIS_IrFinalLista( LIS_tppLista pLista ) {

	#ifdef _DEBUG
		assert( pLista != NULL ) ;
    CNT_CONTAR("LIS_IrFinalLista - Entrou e assertou pLista != NULL") ;
  #endif

  pLista->pElemCorr = pLista->pFimLista ;

} /*************** Fim fun��o: LIS &Ir final lista *********************/

/***************************************************************************
*  Fun��o: LIS  &Avan�ar elemento
***************************************************************************/
LIS_tpCondRet LIS_AvancarElementoCorrente(LIS_tppLista pLista, int numElem) {

      int i ;

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
         CNT_CONTAR("LIS_AvancarElementoCorrente - Entrou, declarou variaveis e assertou pLista != NULL") ;
      #endif

      /* Tratar lista vazia */

         if ( pLista->pElemCorr == NULL )
         {

            return LIS_CondRetListaVazia ;

         } /* fim ativa: Tratar lista vazia */

      /* Tratar avan�ar para frente */

         if ( numElem > 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i > 0 ; i-- )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pProx ;
            } /* for */

            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pFimLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avan�ar para frente */

      /* Tratar avan�ar para tr�s */

         else if ( numElem < 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i < 0 ; i++ )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pAnt ;
            } /* for */

            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pOrigemLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avan�ar para tr�s */

      /* Tratar n�o avan�ar */

         return LIS_CondRetOK ;

} /*************** Fim fun��o: LIS &Avancar elemento *******************/

/***************************************************************************
*  Fun��o: LIS  &Procurar elemento contendo valor
***************************************************************************/
LIS_tpCondRet LIS_ProcurarValor( LIS_tppLista pLista, void * pValor ) {

    tpElemLista * pElem ;

    #ifdef _DEBUG
       assert( pLista  != NULL ) ;
       CNT_CONTAR("LIS_ProcurarValor - Entrou, declarou variaveis e assertou pLista != NULL") ;
    #endif

    if ( pLista->pElemCorr == NULL )
    {
       return LIS_CondRetListaVazia ;
    } //fim if

    for ( pElem  = pLista->pElemCorr ;
          pElem != NULL ;
          pElem  = pElem->pProx )
    {
       if ( pElem->pValor == pValor )
       {
          pLista->pElemCorr = pElem ;
          return LIS_CondRetOK ;
       } //fim if
    } //fim if

    return LIS_CondRetNaoAchou ;

} /***** Fim fun��o: LIS &Procurar elemento contendo valor **************/


/***********  C�DIGO DAS FUN��ES ENCAPSULADAS NO M�DULO  *******************/

/***************************************************************************
*  $FC Fun��o: LIS  -Liberar elemento da lista
*
*  $ED Descri��o da fun��o
*     Elimina os espa�os apontados pelo valor do elemento e o
*     pr�prio elemento.
***************************************************************************/
void LiberarElemento( LIS_tppLista pLista, tpElemLista  * pElem   ) {

  #ifdef _DEBUG
     assert( pLista  != NULL ) ;
  #endif

  if ( ( pLista->ExcluirValor != NULL )
      && ( pElem->pValor != NULL        ))
  {
     pLista->ExcluirValor( pElem->pValor ) ;
  } /* if */

  free( pElem ) ;

  pLista->numElem-- ;

} /*********** Fim fun��o: LIS -Liberar elemento da lista *****************/

/***************************************************************************
*  $FC Fun��o: LIS  -Criar o elemento
***************************************************************************/
tpElemLista * CriarElemento( LIS_tppLista pLista, void * pValor  ) {

	tpElemLista * pElem ;

  #ifdef _DEBUG
     assert( pLista  != NULL ) ;
  #endif

  pElem = ( tpElemLista * ) malloc( sizeof( tpElemLista )) ;
  if ( pElem == NULL )
  {
     return NULL ;
  } /* if */

  pElem->pValor = pValor ;
  pElem->pAnt   = NULL  ;
  pElem->pProx  = NULL  ;

  pLista->numElem ++ ;

  #ifdef _DEBUG
 	  CED_DefinirTipoEspaco( pElem, LIS_TipoEspacoElemLista ) ;
  #endif

  return pElem ;

} /*************** Fim fun��o: LIS -Criar o elemento **********************/

/***************************************************************************
*  $FC Fun��o: LIS  -Limpar a cabe�a da lista
***************************************************************************/
void LimparCabeca( LIS_tppLista pLista ) {

  #ifdef _DEBUG
    assert( pLista  != NULL ) ;
  #endif

	pLista->pOrigemLista = NULL ;
  pLista->pFimLista = NULL ;
  pLista->pElemCorr = NULL ;
  pLista->numElem   = 0 ;

} /********* Fim fun��o: LIS -Limpar cabeca da lista **********************/

/***************************************************************************
*  $FC Fun��o: LIS  -Verificar lista
***************************************************************************/
/*#ifdef _DEBUG
LIS_tpCondRet LIS_VerificarLista( void * pListaParm ) {

  LIS_tpLista * pLista = NULL ;

  if ( LIS_VerificarCabeca( pListaParm ) != LIS_CondRetOK ) {
     return LIS_CondRetErroEstrutura ;
  }

  CED_MarcarEspacoAtivo( pListaParm ) ;

  pLista = ( LIS_tpLista * ) ( pListaParm ) ;

  return VerificarElem( pLista->pElemCorr ) ;

} /**************** Fim fun��o: LIS -Verificar lista ***********************/
//#endif

/***************************************************************************
*  $FC Fun��o: LIS  -Verificar cabeca
***************************************************************************/
/*#ifdef _DEBUG
LIS_tpCondRet LIS_VerificarCabeca( void * pCabecaParm ) {

  tpLista * pLista = NULL ;

  	// Verifica o tipo do espa�o
	if ( pCabecaParm == NULL ) {
		TST_NotificarFalha( "Tentou verificar cabe�a inexistente." ) ;
		return LIS_CondRetErroEstrutura ;
	}

	if ( ! CED_VerificarEspaco( pCabecaParm , NULL )) {
		TST_NotificarFalha( "Controle do espa�o acusou erro." ) ;
		return LIS_CondRetErroEstrutura ;
	}

	if (TST_CompararInt(ARV_TipoEspacoCabeca, CED_ObterTipoEspaco(pCabecaParm),
		"Tipo do espa�o de dados n�o � cabe�a de �rvore.") != TST_CondRetOK) {
		return LIS_CondRetErroEstrutura ;
	}

	pArvore = ( tpArvore * )( pCabecaParm ) ;

  	// Verifica raiz da �rvore
	if ( pArvore->pNoRaiz != NULL ) {
		if (TST_CompararPonteiro(pCabecaParm, pArvore->pNoRaiz->pCabeca,
	     	"N� raiz n�o aponta para cabe�a.") != TST_CondRetOK) {
	   		return LIS_CondRetErroEstrutura ;
		}
	} 
	else {
		if ( TST_CompararPonteiro( NULL , pArvore->pNoCorr ,
		     "�rvore vazia tem n� corrente n�o NULL." ) != TST_CondRetOK ) {
	   		return LIS_CondRetErroEstrutura ;
		}
	}

  	// Verifica corrente
     if ( pArvore->pNoCorr != NULL ) {
        if ( TST_CompararPonteiro( pCabecaParm , pArvore->pNoCorr->pCabeca ,
             "N� corrente n�o aponta para cabe�a." ) != TST_CondRetOK ) {
           return LIS_CondRetErroEstrutura ;
        }
     } else {
        if ( TST_CompararPonteiro( NULL , pArvore->pNoRaiz ,
             "�rvore n�o vazia tem n� corrente NULL." ) != TST_CondRetOK ) {
           return LIS_CondRetErroEstrutura ;
        }
     }

  return LIS_CondRetOK ;

} /* Fim fun��o: LIS  &Verificar um n� cabe�a */
//#endif

/***************************************************************************
*  $FC Fun��o: LIS  -Verificar elemento
***************************************************************************/
/*
#ifdef _DEBUG
LIS_tpCondRet LIS_VerificarElem( void * pElemParm ) {

  tpElemLista * pElem = NULL ;
  LIS_tpLista * pLista = NULL ;

  	//Verificar se � n� estrutural
	if ( pElemParm == NULL ) {
		TST_NotificarFalha( "Tentou verificar elemento inexistente." ) ;
		return LIS_CondRetErroEstrutura ;
	}

	if ( ! CED_VerificarEspaco( pElemParm , NULL )) {
		TST_NotificarFalha( "Controle do espa�o acusou erro." ) ;
		return LIS_CondRetErroEstrutura ;
	}

	if ( TST_CompararInt( LIS_TipoEspacoElemLista, CED_ObterTipoEspaco( pElemParm ) ,
	  	 				  "Tipo do espa�o de dados n�o � elemento de lista." ) != TST_CondRetOK ) {
		return LIS_CondRetErroEstrutura ;
	}

	pElem  = ( tpElemLista * )( pElemParm ) ;
	//pLista = pNo->pCabeca ;

  	//Verificar cabe�a
	if ( pArvore->pNoRaiz != NULL  ) {
		if ( TST_CompararPonteiro( pArvore , pArvore->pNoRaiz->pCabeca ,
		     "N� n�o pertence � �rvore." ) != TST_CondRetOK ) {
		   return LIS_CondRetErroEstrutura ;
		}
	} 
	else {
		return TST_NotificarFalha( "N� pertence a �rvore vazia." ) ;
	}

  	//Verificar pai
	if ( pNo->pNoPai != NULL ) {
		if ( ( pNo->pNoPai->pNoEsq != pNo ) && ( pNo->pNoPai->pNoDir != pNo )) {
	   		return LIS_CondRetErroEstrutura ;
		}
	}
	else {
		if ( TST_CompararPonteiro( pNo , pArvore->pNoRaiz ,
		     "N� raiz n�o � apontado por cabe�a da �rvore." ) != TST_CondRetOK ) {
			return LIS_CondRetErroEstrutura ;
		}
	}

  	// Verificar filho � esquerda
	if ( pNo->pNoEsq != NULL ) {
		if ( TST_CompararPonteiro( pNo , pNo->pNoEsq->pNoPai ,
		     "Pai de filho � esquerda n�o � este n�." ) != TST_CondRetOK ) {
	   		return LIS_CondRetErroEstrutura ;
		}
		if ( TST_CompararPonteiro( pArvore , pNo->pNoEsq->pCabeca ,
		     "Filho � esquerda n�o pertence � mesma �rvore." ) != TST_CondRetOK ) {
	   		return LIS_CondRetErroEstrutura ;
		}
	}

  	// Verificar filho � direita
    if ( pNo->pNoDir != NULL )
    {
        if ( TST_CompararPonteiro( pNo , pNo->pNoDir->pNoPai ,
             "Pai de filho � direita n�o � este n�." ) != TST_CondRetOK ) {
           return LIS_CondRetErroEstrutura ;
        }
        if ( TST_CompararPonteiro( pArvore , pNo->pNoDir->pCabeca ,
             "Filho � direita n�o pertence � mesma �rvore." ) != TST_CondRetOK ) {
           return LIS_CondRetErroEstrutura ;
        }
    }


  return LIS_CondRetOK ;

} // Fim fun��o: LIS  &Verificar um elemento de estrutura
#endif
*/

/************* FIM DO M�DULO DE IMPLEMENTA��O: LIS Lista ******************/

