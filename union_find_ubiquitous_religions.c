#include <stdio.h>
#include <stdlib.h>

typedef struct _VERTICE
{
	struct _VERTICE *pai; //quando criado o pai do no é o proprio no
	int tamanho; //numero de vértices que tem este nó como raiz
} VERTICE;

const int MAX_NUM_ESTUDANTES = 50000; 
int NUM_ESTUDANTES; //'N' no enunciado do problema
int NUM_PARES; //'M' no enunciado do problema
int numConjuntos;

size_t TAMBUFFER = 25; //tamanho maximo de chars da linha que recebe 'N' e 'M'

int caso = 1; //numero do caso sendo tratado

void armazenaEntradaStrings( char **entradaStrings );
void armazenaEntradaInts( int **entradaInts, char **entradaStrings );
VERTICE *find( VERTICE *vert );
void unify( VERTICE *vert1, VERTICE *vert2 );

int main( int argc, char *argv[] )
{
	VERTICE **conjuntos;
	char *linhaNM; //armazena os parametros N e M
	char **entradaStrings; //armazena a entrada, um vetor de chars para cada linha
	int **entradaInts; //passa a entrada de chars para inteiros

	/* le primeira linha da entrada e armazena em 'linhaNM'
	   depois le N e M dessa linha */
	linhaNM = (char *)calloc( sizeof( char ), TAMBUFFER );
	getline( &linhaNM, &TAMBUFFER, stdin ); 
	sscanf( linhaNM, "%d %d", &NUM_ESTUDANTES, &NUM_PARES );

	/* valida o numero de estudantes */
	if( NUM_ESTUDANTES <= 0 || NUM_ESTUDANTES > MAX_NUM_ESTUDANTES )
	{
		puts( "Numero de estudantes deve ser um inteiro positivo menor que 50000." );
		return 1;
	}

	/* um loop deste while trata de um caso na entrada */
	while( NUM_ESTUDANTES != 0 && NUM_PARES != 0 )
	{
		numConjuntos = NUM_ESTUDANTES;

		/* aloca e inicializa matrizes que armazenam a entrada, e os conjuntos */
		entradaStrings  = (char **) calloc( sizeof(char *), NUM_PARES+1 );
		for( int i = 1; i < NUM_PARES+1; i++ )
        	entradaStrings[ i ] = calloc( sizeof(char *), TAMBUFFER );

		entradaInts = (int **) calloc( sizeof(int *), NUM_ESTUDANTES+1 );
		for( int i = 0; i < NUM_ESTUDANTES+1; i++ )
			entradaInts[i] = (int *) calloc( sizeof(int), 2 );

		conjuntos = (VERTICE **) calloc( sizeof(VERTICE *), NUM_ESTUDANTES+1 );
		for( int i = 0; i < NUM_ESTUDANTES+1; i++ )
			conjuntos[i] = (VERTICE *) calloc( sizeof(VERTICE), 1 );
	
		/* armazena a entrada em strings e passa os valores para uma matriz de ints */
		armazenaEntradaStrings( entradaStrings );
		armazenaEntradaInts( entradaInts, entradaStrings );

		/* inicializa os vertices, cada vertice tem como raiz si proprio, e forma seu proprio conjunto */
		for( int i = 0; i < NUM_ESTUDANTES+1; i++ )
		{
			conjuntos[i]->pai = conjuntos[i];
			conjuntos[i]->tamanho = 1;
		}

		/* une os conjuntos seguindo as conexoes listadas na matriz da entrada */
		for( int i = 1; i < NUM_ESTUDANTES+1; i++ )
			unify( conjuntos[ entradaInts[i][0] ], conjuntos[ entradaInts[i][1] ] );

		printf( "Case %d: %d\n", caso++, numConjuntos );

		free( entradaStrings );
		free( entradaInts );
		free( conjuntos );

		/* le 'N' e 'M' para o próximo caso, se forem 0 e 0 termina a execução */
		getline( &linhaNM, &TAMBUFFER, stdin ); 
		sscanf( linhaNM, "%d %d", &NUM_ESTUDANTES, &NUM_PARES );
	}

	return 0;
}

/* le a linha de comando, uma linha de cada vez, e armazena cada linha em um vetor de chars
   o conjunto de linhas armazena toda a entrada para um caso */
void armazenaEntradaStrings( char **entradaStrings )
{
	for( int i = 1; i < NUM_PARES+1; i++ )
		getline( &*entradaStrings + i, &TAMBUFFER, stdin );
}

/* le dois inteiros das linhas armazenadas na matriz de chars e armazena na matrix de ints */
void armazenaEntradaInts( int **entradaInts, char **entradaStrings )
{
	for( int i = 1; i < NUM_PARES+1; i++ ) 
		sscanf( entradaStrings[i], "%d %d", &entradaInts[i][0], &entradaInts[i][1] );
}

/* encontra qual é a raiz desse vertice, indicando a qual conjunto pertence
   tambem faz a compressão de caminho */
VERTICE *find( VERTICE *vert )
{
	VERTICE *raiz = vert->pai;
	VERTICE *prox;

	/* navega vertices pelo ponteiro 'pai' ate encontrar a raiz */
	while( raiz->pai != raiz )
		raiz = raiz->pai;

	/* compressao de caminho */
	while( vert->pai != raiz )
	{
		prox = vert->pai;
		vert->pai = raiz;
		vert = prox;
		vert->tamanho--;
	}

	return raiz;
}

/* une os conjuntos aos quais 'vert1' e 'vert2' pertencem */
void unify( VERTICE *vert1, VERTICE *vert2 )
{
	VERTICE *raiz1 = find( vert1 );
	VERTICE *raiz2 = find( vert2 );

	if( raiz1 == raiz2 )
		return;

	numConjuntos--;

	/* anexa o conjunto menor ao maior */
	if( raiz1->tamanho > raiz2->tamanho )
	{
		raiz2->pai = raiz1;
		raiz1->tamanho += raiz2->tamanho;
	}
	else
	{
		raiz1->pai = raiz2;
		raiz2->tamanho += raiz1->tamanho;
	}
}
