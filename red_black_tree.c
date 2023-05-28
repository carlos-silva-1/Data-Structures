/*
* Foi assumido que todos os nós terão chave maior ou igual a 1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* representa um no da arvore */
typedef struct _ARV_NO
{
	int chave;
	char cor;
	struct _ARV_NO* esq;
	struct _ARV_NO* dir;
} ARV_NO;

ARV_NO *arvore; // árvore rubro negra montada pelo algoritmo
ARV_NO **caminho; // vetor que armazena todos os nós percorridos até encontrar a posição de inserção do novo nó
int indiceNoPercorrido = 0; // usado para indicar a posição de inserção dos nós percorridos (durante a insercao) no vetor 'caminho'

/* executa uma rotação esquerda 
   'no' é o nó no menor nível dentre os nós nos quais a rotação é aplicada
   'indice' é o índice deste nó 'no' */
void rotacaoEsq( ARV_NO *no, int indice )
{
	ARV_NO *pt = no->dir;
	no->dir = pt->esq;
	pt->esq = no;

	/* nesse ponto o antigo pai de 'no' ainda aponta para 'no', não para pt (que substitui 'no' após a rotação)
	   se 'no' era a raiz da árvore, 'pt' vira a nova raiz */
	if( indice == 0 )
		arvore = pt;

	/* porem se o pai de 'no' não era a raiz, aponta para 'pt' como filho */
	if( indice != 0 )
	{
		/* descobre se 'no' é filho esquerdo ou direito de seu pai, 
		   e então aponta para 'pt' como filho correto */
		if( no == caminho[ indice-1 ]->esq )
			caminho[ indice-1 ]->esq = pt;
		else
			caminho[ indice-1 ]->dir = pt;
	}
}

/* executa uma rotação direita
   'no' é o nó no menor nível dentre os nós nos quais a rotação é aplicada
   'indice' é o índice deste nó 'no' */
void rotacaoDir( ARV_NO *no, int indice )
{
	ARV_NO *pt = no->esq;
	no->esq = pt->dir;
	pt->dir = no;

	/* nesse ponto o antigo pai de 'no' ainda aponta para 'no', não para pt (que substitui 'no' após a rotação)
	   se 'no' era a raiz da árvore, 'pt' vira a nova raiz */
	if( indice == 0 )
		arvore = pt;

	/* porem se o pai de 'no' não era a raiz, aponta para 'pt' como filho */
	if( indice != 0 )
	{
		/* descobre se 'no' é filho esquerdo ou direito de seu pai, 
		   e então aponta para 'pt' como filho correto */
		if( no == caminho[ indice-1 ]->esq )
			caminho[ indice-1 ]->esq = pt;
		else
			caminho[ indice-1 ]->dir = pt;
	}
}

void recolore( ARV_NO *no )
{
	if( no->cor == 'N' )
		no->cor = 'R';
	else
		no->cor = 'N';
}

/* a função checa se o nó inserido e seu pai formam dois nós rubros adjacentes, se sim faz 
   os ajustes necessários para manter os requerimentos de uma árvore RN atendidos.

   'indice' é o índice do nó em 'caminho' sendo checado por irregularidades. 
   'pt' aponta para o filho presente em 'caminho' do nó mencionado acima */
void concerta( int indice, ARV_NO *pt )
{
	if( indice >= 0 ) // índices negativos não correspondem a nós
	{
		ARV_NO *pai = caminho[ indice ];
		ARV_NO *tio = NULL;
		ARV_NO *avo = NULL;

		if( pai->cor == 'R' )
		{
			/* se pai é a raiz */
			if( indice == 0 )
			{
				if( pt->cor == 'R' )
					recolore( pai );
			}
			else
			{
				/* encontra avo e tio */
				avo = caminho[ indice-1 ];
				if( pai == avo->esq )
					tio = avo->dir;
				else
					tio = avo->esq;

				/* se pai e tio são rubros, recolore pai, tio e avo e chama conserta um nível acima */
				if( tio != NULL && tio->cor == 'R' )
				{
					pai->cor = 'N';
					tio->cor = 'N';
					avo->cor = 'R';
					concerta( indice-2, avo );
				}
				/* se pai é rubro e tio é negro, temos 4 cenários, 
				   que dependem das últimas duas inserções: */
				else if( tio == NULL || tio->cor == 'N' )
				{
					/* caso LL: rotacaoDir com avo e recolore pai e avo */
					if( pt == pai->esq && pai == avo->esq )
					{
						rotacaoDir( avo, indice-1 );
						recolore( pai );
						recolore( avo );
					}
					/* caso LR: rotacaoDirEsq com pai e aplica caso LL */
					else if( pt == pai->dir && pai == avo->esq )
					{
						rotacaoEsq( pai, indice );
						rotacaoDir( avo, indice-1 );
						recolore( pt );
						recolore( avo );
					}
					/* caso RR: rotacaoDirEsq com avo e recolore pai e avo */
					else if( pt == pai->dir && pai == avo->dir )
					{
						rotacaoEsq( avo, indice-1 );
						recolore( pai );
						recolore( avo );
					}
					/* caso RL: rotacaoDirDir com pai e aplica caso RR */
					else if( pt == pai->esq && pai == avo->dir )
					{
						rotacaoDir( pai, indice );
						rotacaoEsq( avo, indice-1 );
						recolore( pt );
						recolore( avo );
					}
				}
			}
		}
	}
}

/* insere nó com chave igual ao argumento 'chave' na árvore 'arvore'
   os nós percorridos até alcançar a posição correta são armazenados 
   no vetor 'caminho' */
void insere( ARV_NO *arvore, int chave )
{
	indiceNoPercorrido = 0;

	ARV_NO *novo = ( ARV_NO* ) calloc( sizeof( ARV_NO ), 1 );
	ARV_NO *folha = NULL;
	ARV_NO *pt = arvore;

	novo->chave = chave;
	novo->cor = 'R';
	
	/* 'pt' percorre a arvore comparando a chave a ser inserida. ao final desse loop 
	   'folha' aponta para o nó que será pai de 'novo' quando este for inserido.
	   todos os nós no caminho da raiz até a folha são armazenados no vetor 'caminho' */
	while( pt != NULL && pt->chave != 0 )
	{
		caminho[ indiceNoPercorrido++ ] = pt;
		folha = pt;

		if( novo->chave < pt->chave )
			pt = pt->esq;
		else
			pt = pt->dir;
	}

	/* insere 'novo' como filho de 'folha' na posição correta */
	if( folha == NULL ) //se param raiz é NULL
		*arvore = *novo;
	else if( novo->chave < folha->chave )
		folha->esq = novo;
	else
		folha->dir = novo;

	concerta( indiceNoPercorrido-1, novo );
}

void printArvore( ARV_NO *raiz )
{
	if( raiz != NULL )
	{
		printf( "%d%c ", raiz->chave, raiz->cor );
		printArvore( raiz->esq );
		printArvore( raiz->dir );
	}
}

int main( int argc, char* argv[] )
{
	/* armazena os nós dados como entrada na linha de comando */
	int *entrada = (int *) calloc( sizeof( int ), 10 );

	size_t bufsize = 256;
	/* usado para armazenar uma linha da entrada antes de converte-la para int e 
	   armazena-lo no vetor 'entrada' */
	char *buffer = (char *) calloc( sizeof( char ), bufsize ); 

	arvore = (ARV_NO *) calloc( sizeof( ARV_NO ), 1 );
	caminho = (ARV_NO **) calloc( sizeof( ARV_NO* ), 10 );

	/* recebe os nós que devem ser inseridos da linha de comando, um nó por linha, 
	   e os insere no vetor 'entrada'. */
	int numElementos = 0;
	while( getline( &buffer, &bufsize, stdin ) > 0 )
	{
		if( strcmp( buffer, "\n" ) == 0 )
			break;
		sscanf( buffer, "%d", &*entrada + numElementos );
		numElementos++;
	}

	/* insere todos os elementos do vetor 'entrada' na árvore */
	for( int i = 0; i < numElementos; i++ )
		insere( arvore, entrada[ i ] );

	printArvore( arvore );

	return 0;
}
