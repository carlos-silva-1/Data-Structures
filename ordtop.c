#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
A lista de adjacencias será composta por elementos do tipo VERTICE

Ex:
VERTICE *lista = calloc( sizeof( VERTICE ), 5 );

Com a entrada:
5 6   //num de vertices e num de arestas
2 3   //adj de 1
      //adj de 2
2 4   //adj de 3
5     //adj de 4
2     //adj de 5

Será:
lista[1]-> {chave = 2}-> {chave = 3}-> NULL
lista[2]-> {chave = 0}-> NULL
lista[3]-> {chave = 2}-> {chave = 4}-> NULL
lista[4]-> {chave = 5}-> NULL
lista[5]-> {chave = 2}-> NULL

Foi considerado que o menor vértice será '1', não podendo existir
vértice com chave '0' ou negativa.
*/
typedef struct _VERTICE
{
      int chave;
      struct _VERTICE* prox;
} VERTICE;

void insere( VERTICE *vertice, int chave );
void printVetor( int *vetor, int tamanho );

int main( int argc, char *argv[] )
{
      char *buf; //recebe a linha contendo o numero de vertices e arestas
      size_t TAMBUFFER = 100; //tamanho maximo da linha que recebe N_VERTICES e N_ARESTAS
      size_t tam_linha = 256; //tamanho maximo de cada linha da matriz de adjacencia
      int N_VERTICES, N_ARESTAS;

      buf = ( char* ) calloc( sizeof( char ), TAMBUFFER );
      getline( &buf, &TAMBUFFER, stdin );
      sscanf( buf, "%d %d", &N_VERTICES, &N_ARESTAS ); //recebe num de vertices e arestas da primeira linha

      /* Os vertices são contados de 1 até n. Os vetores e listas utilizados utilizam seus indices para indicar 
      qual é o vertice tratado, e portanto o indice [0] deve sempre ser ignorado. */
      int grau_entrada[ N_VERTICES+1 ];
      int ordem[ N_VERTICES+1 ]; //ordenação final
      int pos = 1; //indice de um vértice na ordenação final
      char **entrada = calloc( sizeof( char* ), N_VERTICES+1 ); //armazena a matriz de adjacencia em forma de strings
      VERTICE *lista = calloc( sizeof( VERTICE ), N_VERTICES+1 ); //lista encadeada para armazenar a matriz de adjacencia

      //inicializa grau_entrada[] e ordem[] para 0
      for( int i = 1; i < N_VERTICES+1; i++ )
      {
            grau_entrada[ i ] = 0;
            ordem[ i ] = 0;
      }

      //recebe a matriz ADJ em forma de strings
      for( int i = 1; i < N_VERTICES+1; i++ )
      {
            entrada[ i ] = calloc( sizeof( char ), tam_linha );
            getline( &*entrada + i, &TAMBUFFER, stdin );
      }

      /*transfere a matriz ADJ de strings 'entrada' para a lista encadeada
      ex: se o vertice 1 tem arestas para os vertices 2, 11, e 6:
      entrada[ 1 ] = "2 11 6"
      resultado:
      lista[ 1 ]-> 2 -> 11 -> 6 -> NULL*/ 
      for( int i = 1; i < N_VERTICES+1; i++ ) 
      {
            int j = 0, temp = 0;
            for( ; entrada[ i ][ j ] != '\n'; j++ )
            {
                  if( entrada[ i ][ j ] == ' ' && temp != 0 ) //se char é espaço, temp armazena um vertice adjacente
                  {
                        insere( lista+i, temp );
                        temp = 0;
                  }
                  else if( entrada[ i ][ j ] >= 48 && entrada[ i ][ j ] <= 57 )
                        temp = temp * 10 + ( entrada[ i ][ j ] - 48 );
            }
            if( entrada[ i ][ j ] == '\n' && temp != 0 ) //se char é '\n', temp armazena um vertice adjacente
                  insere( lista+i, temp );
      }
      
      VERTICE *navegador;

      //conta quantas arestas foram inseridas
      for( int i = 1; i < N_VERTICES+1; i++ )
      {
            navegador = lista+i;
            while( navegador != NULL && navegador->chave != 0 )
            {
                  N_ARESTAS--;
                  navegador = navegador->prox;
            }
      }
      if( N_ARESTAS != 0 )
      {
            puts( "Erro. Numero errado de arestas inserido." );
            return 1;
      }

      //descobre o grau de entrada de cada vertice
      for( int i = 1; i < N_VERTICES+1; i++ )
      {
            navegador = lista + i;
            do
            {
                  grau_entrada[ navegador->chave ]++; //no caso em que nao houver aresta saindo do vertice, grau_entrada[0]++, nao traz problema pois grau_entrada[0] é ignorado
                  navegador = navegador->prox;
            } while( navegador != NULL );
      }

      //põe os vertices fonte como primeiros na ordenação final
      for( int i = 1; i < N_VERTICES+1; i++ )
      {
            if( grau_entrada[ i ] == 0 )
            {
                  ordem[ pos ] = i;
                  pos++;
            }
      }

      //ordena o resto do grafo
      int i = 1;
      while( i < N_VERTICES+1 )
      {
            navegador = lista + ordem[ i ];
            do
            {
                  grau_entrada[ navegador->chave ]--;
                  if( grau_entrada[ navegador->chave ] == 0 && navegador->chave != 0 ) //se encontra um novo vertice fonte o põe na ordenação
                  {
                        ordem[ pos ] = navegador->chave;
                        pos++;
                  }
                  navegador = navegador->prox;
            } while( navegador != NULL );

            i++;
      }

      if( pos != N_VERTICES+1 )
            puts( "Erro. Tem ciclo." );

      printVetor( ordem, N_VERTICES+1 );

      return 0;
}

//FUNÇÕES AUXILIARES

void insere( VERTICE *vertice, int chave )
{
      if( chave > 0 )
      {
            if( vertice->chave == 0 ) //caso de inserção na primeira posição
                  vertice->chave = chave;
            else
            {
                  while( vertice->prox != NULL ) //encontra ultimo vertice na lista
                        vertice = vertice->prox;

                  vertice->prox = ( VERTICE* ) calloc( sizeof( VERTICE ), 1 );
                  vertice = vertice->prox;
                  vertice->chave = chave;
            }
      }
      else
            puts( "Chave invalida. Deve ser um valor maior ou igual a 1." );
}

void printVetor( int *vetor, int tamanho )
{
      for( int i = 1; i < tamanho; i++ )
            printf( "%d ", vetor[ i ] );

      puts("");
}
