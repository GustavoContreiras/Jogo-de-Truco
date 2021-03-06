/***************************************************************************
*  $MCI M�dulo de implementa��o: TCED Teste controle de espa�o din�mico
*
*  Arquivo gerado:              TESTCED.c
*  Letras identificadoras:      TCED
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: C:\AUTOTEST\PROJETOS\ARCABOUC.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     4       avs   01/fev/2006 criar linguagem script simb�lica
*     3       avs   08/dez/2004 uniformiza��o dos exemplos
*     2       avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1       avs   16/abr/2003 in�cio desenvolvimento
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>

#include    "TST_Espc.h"

#include    "generico.h"
#include    "CEspDin.h"
#include    "LerParm.h"


typedef     int      BOOL ;

/* Tabela dos nomes dos comandos de teste espec�ficos */

static const char ResetEspacosCmd[ ]            = "=resetespaco" ;
static const char AlocarEspacoCmd[ ]            = "=alocarespaco" ;
static const char DesalocarEspacoCmd[ ]         = "=desalocarespaco" ;
static const char ExibirEspacoBrutoCmd[ ]       = "=exibirbruto" ;
static const char ExibirEspacoCmd[ ]            = "=exibirespaco" ;
static const char InserirStringEspacoCmd[ ]     = "=inserirstringespaco" ;
static const char DefinirTipoEspacoCmd[ ]       = "=definirtipoespaco" ;
static const char ObterTipoEspacoCmd[ ]         = "=obtertipoespaco" ;
static const char ObterStringEspacoCmd[ ]       = "=obterstringespaco" ;
static const char ObterTamanhoEspacoCmd[ ]      = "=obtertamanhoespaco" ;
static const char VerificarEspacoCmd[ ]         = "=verificarespaco" ;
static const char DeturparEspacoCmd[ ]          = "=deturparespaco" ;
static const char MarcarEspacoAtivoCmd[ ]       = "=marcarespacoativo" ;
static const char MarcarEspacoNaoAtivoCmd[ ]    = "=marcarespaconaoativo" ;
static const char MarcarTodosNaoAtivosCmd[ ]    = "=marcartodosnaoativos" ;
static const char VerificarEspacoAtivoCmd[ ]    = "=verificarespacoativo" ;
static const char IniciarIteradorEspacosCmd[ ]  = "=iniciariteradorespacos" ;
static const char AvancarProximoEspacoCmd[ ]    = "=avancarproximoespaco" ;
static const char ObterEspacoCorrenteCmd[ ]     = "=obterespacocorrente" ;
static const char ExisteEspacoCorrenteCmd[ ]    = "=existeespacocorrente" ;
static const char TerminarIteradorEspacoCmd[ ]  = "=terminariteradorespaco" ;
static const char ExcluirEspacoCorrenteCmd[ ]   = "=excluirespacocorrente" ;
static const char VerificarFrequenciaCmd[ ]     = "=verificarfrequencia" ;

#define     TRUE     1
#define     FALSE    0
#define     MSG_ERRO    ">>>"

#define     DIM_VT_ESPACO  5

static void * vtEspaco[ DIM_VT_ESPACO ] ;
       
static int EhPrimeiraVez = TRUE ;

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static void InstrumentarString( char * pString , int dimString ) ;

   static BOOL VerificarInxEspaco( int inxEspacoParm , BOOL EhNaoNulo ) ;

   static int VerificarValor( void * pValor ) ;

   static BOOL VerificarString( char * String , int dimString ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TCED &Efetuar comando de teste do controle de espa�o din�mico
*
*  $ED Descri��o da fun��o
*     Esta fun��o testa as fun��es do m�dulo de controle
*     de espa�o din�mico que n�o s�o controladas pelo m�dulo
*     INTESPAC ICED Interpretador de comandos de controle de espa�o din�mico
*
*     O m�dulo TCNTESP interpreta os comandos de teste utilizados para
*     controlar os acesso a espa�os de dados din�micos.
*     O contexto � formado por um vetor de 4 espa�os que podem ser
*     endere�ados pelo par�metro <inxEspaco> dom comando.
*     S�o seguintes os comandos interpretados:
*
*     =resetespaco
*         esvazia todos os registros de refer�ncia a espa�os alocados
*
*     =alocarespaco               <inxEspaco>  <tam>  <lin>  <NomeArquivo>
*         efetua um malloc
*
*     =desalocarespaco            <inxEspaco>
*         efetua um free
*
*     =exibirespaco               <inxEspaco>
*         exibe o conte�do de um espa�o em formato hexadecimal
*
*     =inserirstringespaco        <inxEspaco>  string terminado em \n
*         atribui um valor ao espa�o. Pode ser utilizado para corromper
*         as estruturas de dados armazenadas em mem�ria din�mica
*
*     =deturparespaco             <inxEspaco>  <deslocamento>  <charHexa>
*         atribui um valor em mem�ria relativa a um dado espa�o.
*         Pode ser utilizado para corromper a estrutura de controle
*         de espa�os din�micos
*
*     =verificarespaco            <inxEspaco>  <numFalhasEsperadas>
*         examina a validade do registro de determinado espa�o. O espa�o
*         pode conter falhas em virtude de poss�veis corrup��es com
*         o comando =deturparespaco
*
*     =definirtipoespaco          <inxEspaco>  <idTipo>  <condEsperada>
*         define o tipo associado ao espa�o
*
*     =obtertipoespaco            <inxEspaco>  <idTipoEsperado>
*         obt�m o tipo do espa�o
*
*     =obterstringespaco          <inxEspaco>  stringEsperado terminado em \n
*         obt�m o string contido no espa�o
*
*     =obtertamanhoespaco         <inxEspaco>  <TamanhoEsperado>
*         obt�m o tamanho do valor contido no espa�o
*
*     =marcarespacoativo          <inxEspaco>
*         marca o espa�o ativo
*
*     =marcarespaconaoativo       <inxEspaco>
*         marca o espa�o inativo
*
*     =marcartodosnaoativos
*         torna inativos todos os espa�os
*
*     =verificarespacoativo       <inxEspaco>
*         verifica se determinado espa�o � ativo
*
*     =iniciariteradorespacos
*         inicia ou restaura o iterador que percorre o conjunto de todos os
*         espa�os din�micos alocados
*
*     =avancarproximoespaco       <condEsperada>
*         caminha para o pr�ximo espa�o na lista
*
*     =obterespacocorrente        <inxEspaco>
*         obt�m a refer�ncia para o espa�o corrente do iterador atribuindo-a
*         ao elemento <inxEspaco>
*
*     =existeespacocorrente       <condEsperada>
*         verifica se o iterador est� ou n�o ativo
*
*     =terminariteradorespaco
*         termina aexecu��o do iterador
*
*     =excluirespacocorrente
*         elimina o espa�o corrente da lista de espa�os controlados
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int  i ;
         
      int  numLidos    = -1 ;
      int  inxEspaco   = -1 ;
      int  valEsperado = -1 ;
      int  valObtido   = -1 ;
         
      #define  DIM_STRING_PARM   100
      char StringParm[ DIM_STRING_PARM ] ;
         
      int       Deslocamento ;
      int       Valor ;

      int   Tamanho ;
      int   numLinhaFonte ;

      int idTipo ;

      int numAllocs  = -1 ,
          Frequencia =  0 ,
          ContaNULL  =  0 ;

      double Tolerancia      ,
             PercentEsperado ,
             PercentObtido   ;

      void * pEspaco = NULL ;

      InstrumentarString( StringParm , DIM_STRING_PARM ) ;

      /* Tratar: Reinicializar m�dulo de teste espe�ifico */

         if ( strcmp( ComandoTeste , ResetEspacosCmd ) == 0 )
         {

            if ( EhPrimeiraVez )
            {
               EhPrimeiraVez = FALSE ;

               for( i = 0 ; i < DIM_VT_ESPACO ; i ++ )
               {
                  vtEspaco[ i ] = NULL ;
               } /* for */

               return TST_CondRetOK ;
            } /* if */

            for( i = 0 ; i < DIM_VT_ESPACO ; i++ )
            {
               if ( vtEspaco[ i ] != NULL )
               {
                  CED_Free( vtEspaco[ i ] ) ;
                  vtEspaco[ i ] = NULL ;
               } /* if */
            } /* for */

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: Reinicializar m�dulo de teste espe�ifico */

      /* Tratar: Deturpar espa�o */

         else if ( strcmp( ComandoTeste , DeturparEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                               &inxEspaco , &Deslocamento , &Valor ) ;

            if ( ( numLidos != 3 )
              || !VerificarInxEspaco( inxEspaco , TRUE ))
            {
               return TST_CondRetParm ;
            } /* if */

            (( char * ) vtEspaco[ inxEspaco ] )[ Deslocamento ] = ( char ) Valor ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: Deturpar espa�o */

      /* Tratar: Atribuir string ao espa�o */

         else if ( strcmp( ComandoTeste , InserirStringEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "is" ,
                               &inxEspaco , StringParm ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , TRUE )
              || !VerificarString( StringParm , DIM_STRING_PARM ))
            {
               return TST_CondRetParm ;
            } /* if */

            strcpy( vtEspaco[ inxEspaco ] , StringParm ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: Atribuir string ao espa�o */

      /* Testar: Obter string contido no espa�o */

         else if ( strcmp( ComandoTeste , ObterStringEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "is" ,
                               &inxEspaco , StringParm ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , TRUE )
              || !VerificarString( StringParm , DIM_STRING_PARM ))
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararEspaco( StringParm , vtEspaco[ inxEspaco ] ,
                      strlen( StringParm ) , "Conte�do do valor errado." ) ;

         } /* fim ativa: Testar: Obter string contido no espa�o */

      /* Testar: CED &Alocar espa�o */

         else if ( strcmp( ComandoTeste , AlocarEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "iiis" ,
                               &inxEspaco , &Tamanho , &numLinhaFonte , StringParm ) ;

            if ( ( numLidos != 4 )
              || ( Tamanho  <= 0 )
              || ( numLinhaFonte < 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE )
              || !VerificarString( StringParm , DIM_STRING_PARM ))
            {
               return TST_CondRetParm ;
            } /* if */

            vtEspaco[ inxEspaco ] =
                      CED_Malloc( Tamanho , numLinhaFonte , StringParm ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Alocar espa�o */

      /* Testar: CED &Desalocar espa�o */

         else if ( strcmp( ComandoTeste , DesalocarEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_Free( vtEspaco[ inxEspaco ] ) ;

            vtEspaco[ inxEspaco ] = NULL ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Desalocar espa�o */

      /* Testar: CED &Exibir conte�do bruto do espa�o */

         else if ( strcmp( ComandoTeste , ExibirEspacoBrutoCmd ) == 0 )
         {

            #define  DIM_ESPACO  70

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            TST_ExibirEspacoHexa( DIM_ESPACO + 4 , (( char * ) vtEspaco[ inxEspaco ] ) - DIM_ESPACO ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Exibir conte�do bruto do espa�o */

      /* Testar: CED &Exibir conte�do �til do espa�o */

         else if ( strcmp( ComandoTeste , ExibirEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_ExibirEspaco( vtEspaco[ inxEspaco ] ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Exibir conte�do �til do espa�o */

      /* Testar: CED &Verificar a integridade de um espa�o de dados */

         else if ( strcmp( ComandoTeste , VerificarEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxEspaco , &valEsperado ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_VerificarEspaco( vtEspaco[ inxEspaco ] ,
                                             VerificarValor ) ;

            return TST_CompararBool( valEsperado , valObtido ,
                   "Verifica��o resultou errado." ) ;

         } /* fim ativa: Testar: CED &Verificar a integridade de um espa�o de dados */

      /* Testar: CED &Definir o tipo do espa�o */

         else if ( strcmp( ComandoTeste , DefinirTipoEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                               &inxEspaco , &idTipo , &valEsperado ) ;

            if ( ( numLidos != 3 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_DefinirTipoEspaco( vtEspaco[ inxEspaco ] , idTipo );

            return TST_CompararInt( valEsperado , valObtido ,
                   "Condi��o de retorno errada." ) ;

         } /* fim ativa: Testar: CED &Definir o tipo do espa�o */

      /* Testar: CED &Obter o tipo do espa�o */

         else if ( strcmp( ComandoTeste , ObterTipoEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxEspaco , &valEsperado ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_ObterTipoEspaco( vtEspaco[ inxEspaco ] ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Tipo do espa�o errado." ) ;

         } /* fim ativa: Testar: CED &Obter o tipo do espa�o */

      /* Testar: CED &Obter tamanho do valor contido no espa�o */

         else if ( strcmp( ComandoTeste , ObterTamanhoEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxEspaco , &valEsperado ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_ObterTamanhoValor( vtEspaco[ inxEspaco ] ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Tamanho do valor errado." ) ;

         } /* fim ativa: Testar: CED &Obter tamanho do valor contido no espa�o */

      /* Testar: CED &Marcar ativo o espa�o */

         else if ( strcmp( ComandoTeste , MarcarEspacoAtivoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , TRUE ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_MarcarEspacoAtivo( vtEspaco[ inxEspaco ] ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Marcar ativo o espa�o */

      /* Testar: CED &Marcar n�o ativo o espa�o */

         else if ( strcmp( ComandoTeste , MarcarEspacoNaoAtivoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , TRUE ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_MarcarEspacoNaoAtivo( vtEspaco[ inxEspaco ] ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Marcar n�o ativo o espa�o */

      /* Testar: CED &Marcar n�o ativos todos os espa�os */

         else if ( strcmp( ComandoTeste , MarcarTodosNaoAtivosCmd ) == 0 )
         {

            CED_MarcarTodosEspacosInativos( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Marcar n�o ativos todos os espa�os */

      /* Testar: CED &Verificar se espa�o � ativo */

         else if ( strcmp( ComandoTeste , VerificarEspacoAtivoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxEspaco , &valEsperado ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , TRUE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_EhEspacoAtivo( vtEspaco[ inxEspaco ] ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Marca de espa�o ativo errada." ) ;

         } /* fim ativa: Testar: CED &Verificar se espa�o � ativo */

      /* Testar CED  &Limitar com freq��ncia de gera��o de NULL */

         else if ( strcmp( ComandoTeste , VerificarFrequenciaCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "iif" ,
                               &numAllocs , &Frequencia , &Tolerancia ) ;

            if ( ( numLidos != 3   )
              || ( numAllocs < 100 ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_LimitarFrequencia( Frequencia ) ;
            if ( !valObtido )
            {
               return TST_NotificarFalha( "Freq��ncia limite de malloc em erro." ) ;
            } /* if */

            for( i = 0 ; i < numAllocs ; i++ )
            {
               pEspaco = CED_Malloc( 100 , i + 1 , "TesteFrequencia" ) ;
               if ( pEspaco == NULL )
               {
                  ContaNULL ++ ;
               } else
               {
                  CED_Free( pEspaco ) ;
               } /* if */
            } /* for */

            PercentEsperado = ( 1000. - Frequencia ) / 1000. ;

            PercentObtido   = ContaNULL ;
            PercentObtido   = PercentObtido / numAllocs ;

            return TST_CompararFloat( PercentEsperado , PercentObtido , Tolerancia ,
                        "N�mero de NULLs gerados est� fora da toler�ncia." ) ;

         } /* fim ativa: Testar CED  &Limitar com freq��ncia de gera��o de NULL */

      /* Testar: CED &Iterador: iniciar iterador de espa�os */

         else if ( strcmp( ComandoTeste , IniciarIteradorEspacosCmd ) == 0 )
         {

            CED_InicializarIteradorEspacos(  ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Iterador: iniciar iterador de espa�os */

      /* Testar: CED &Iterador: avan�ar para o pr�ximo espa�o */

         else if ( strcmp( ComandoTeste , AvancarProximoEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_AvancarProximoEspaco( ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Condi��o de avan�o errada." ) ;

         } /* fim ativa: Testar: CED &Iterador: avan�ar para o pr�ximo espa�o */

      /* Testar: CED &Iterador: obter refer�ncia para o espa�o corrente */

         else if ( strcmp( ComandoTeste , ObterEspacoCorrenteCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            vtEspaco[ inxEspaco ] = CED_ObterPonteiroEspacoCorrente( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Iterador: obter refer�ncia para o espa�o corrente */

      /* Testar: CED &Iterador: existe espa�o corrente */

         else if ( strcmp( ComandoTeste , ExisteEspacoCorrenteCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_ExisteEspacoCorrente( ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Condi��o de exist�ncia de iterador errada." ) ; {

            } /* fim repete: Testar: CED &Iterador: existe espa�o corrente */

         } /* fim ativa: Testar: CED &Iterador: existe espa�o corrente */

      /* Testar: CED &Iterador: terminar iterador */

         else if ( strcmp( ComandoTeste , TerminarIteradorEspacoCmd ) == 0 )
         {

            CED_TerminarIteradorEspacos(  ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Iterador: terminar iterador */

      /* Testar: CED &Iterador: excluir espa�o corrente */

         else if ( strcmp( ComandoTeste , ExcluirEspacoCorrenteCmd ) == 0 )
         {

            CED_ExcluirEspacoCorrente( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Iterador: excluir espa�o corrente */

      return TST_CondRetNaoConhec ;

   } /* Fim fun��o: TCED &Efetuar comando de teste do controle de espa�o din�mico */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TCED -Instrumentar string local
*
*  $ED Descri��o da fun��o
*     Adiciona controle de extravas�o a um espa�o destinado para string
*
***********************************************************************/

   void InstrumentarString( char * pString , int dimString )
   {

      TST_ASSERT( dimString >= 2 ) ;

      pString[ dimString - 1 ] = 0 ;
      pString[ dimString - 2 ] = '\xFC' ;
      memset( pString , '?' , dimString - 2 ) ;

   } /* Fim fun��o: TCED -Instrumentar string local */


/***********************************************************************
*
*  $FC Fun��o: TCED -Verificar �ndice de espa�o
*
*  $ED Descri��o da fun��o
*     Verifica se o �ndice do registro de espa�o de dados contido no comando
*     de teste � v�lido.
*
***********************************************************************/

   BOOL VerificarInxEspaco( int inxEspacoParm , BOOL EhNaoNulo )
   {

      if ( ( inxEspacoParm <  0 )
        || ( inxEspacoParm >= DIM_VT_ESPACO ))
      {
         return FALSE ;
      } /* if */

      if ( EhNaoNulo )
      {
         if ( vtEspaco[ inxEspacoParm ] == NULL )
         {
            return FALSE ;
         } /* if */
      } /* if */

      return TRUE ;

   } /* Fim fun��o: TCED -Verificar �ndice de espa�o */


/***********************************************************************
*
*  $FC Fun��o: TCED -Verificar valor contido no espa�o
*
*  $ED Descri��o da fun��o
*     Esta fun��o ilustra a interface de verifica��o
*     Se o espa�o inicia com caractere "vazio (rec�m alocado)"
*     todo ele deve estar vazio.
*
*     A implementa��o da presente fun��o n�o pode ser utilizada
*     quando a aloca��o de espa�os for realizada por malloc,
*     uma vez que depende da fun��o CED_ObterEspaco que n�o
*     existe para espa�os alocados com malloc.
*
*     Para poder ser utilizada com espa�os alocados com malloc,
*     a verifica��o deve ser poss�vel conhecendo-se apenas o
*     ponteiro para o valor.
*
***********************************************************************/

   int VerificarValor( void * pValor )
   {

      int Tamanho ;
      int i ;

      Tamanho = CED_ObterTamanhoValor( pValor ) ;

      if ( (( char * ) pValor )[ 0 ] == '|' )
      {
         for( i = 0 ; i < Tamanho ; i++ )
         {
            if ( (( char * ) pValor )[ i ] != '|' )
            {
               return 0 ;
            } /* if */
         } /* for */
      } /* if */

      return 1 ;

   } /* Fim fun��o: TCED -Verificar valor contido no espa�o */


/***********************************************************************
*
*  $FC Fun��o: TCED -Verificar extravas�o de string
*
*  $ED Descri��o da fun��o
*     Verifica se ocorreu extravas�o em string local controlado
*
***********************************************************************/

   BOOL VerificarString( char * String , int dimString )
   {

      if ( ( String[ dimString - 1 ] != 0      )
        || ( String[ dimString - 2 ] != '\xFC' ))
      {
         return FALSE ;
      } /* if */
      return TRUE ;

   } /* Fim fun��o: TCED -Verificar extravas�o de string */

/********** Fim do m�dulo de implementa��o: TCED Teste controle de espa�o din�mico **********/

