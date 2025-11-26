#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5
#define MAX_PILHA 5

typedef struct {
    char nome;
    int id ;
}Peca;

typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

Peca gerarPeca() {
    // 1. Vetor com as opções de peças permitidas
    char tipos[] = {'I', 'O', 'T', 'L'};

    // 2. Sorteia um índice entre 0 e 3
    int indiceSorteado = rand() % 4;

    // 3. O SEGREDO DO ID: Variável static
    // Ela é inicializada com 0 apenas na PRIMEIRA vez que o programa roda.
    // Nas próximas chamadas, ela mantém o valor anterior.
    static int contadorId = 0;

    // 4. Cria a peça temporária para retorno
    Peca novaPeca;
    novaPeca.nome = tipos[indiceSorteado]; // Pega a letra sorteada
    novaPeca.id = contadorId;              // Pega o ID atual

    // 5. Incrementa o contador para a PRÓXIMA vez que a função for chamada
    contadorId++;

    return novaPeca;
}

int filaCheia(Fila *f) {
    return f->total == MAX;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

void inserir(Fila *f) {
    if (f->total == MAX) {
        printf("Fila cheia. Não é possível inserir.\n");
        return;
    }
    // Chamo a função e GUARDO o resultado numa variável 'nova'
    Peca nova = gerarPeca(); 

    // Agora uso a variável 'nova' para salvar na fila
    f->itens[f->fim] = nova; 

    // 4. Atualizo os índices 
    f->fim = (f->fim + 1) % MAX;
    f->total++;
    
    printf("Peça '%c' (ID: %d) inserida com sucesso!\n", nova.nome, nova.id);
}


void jogar(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("Fila vazia. Não é possível jogar, adicione uma peça.\n");
        return;    
    }

    *p = f->itens[f->inicio];             
    f->inicio = (f->inicio + 1) % MAX;     
    f->total--;                        
}

void exibirFila(Fila f) {
    if (f.total == 0) {
        printf("Fila vazia!\n");
        return;
    }

    printf("Fila atual: ");
    
    // Loop corre apenas a quantidade de itens que existem
    for (int i = 0; i < f.total; i++) {
        
        // Calcula o índice real girando a roleta
        // Ex: Se inicio é 4 e i é 1 (tamanho 5), (4+1)%5 vira índice 0.
        int indiceReal = (f.inicio + i) % MAX;
        
        printf("[%c %d] ", f.itens[indiceReal].nome, f.itens[indiceReal].id);
    }
    printf("\n");
}

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

void push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia. Não é possível inserir.\n");
        return;
    }
 
    p->topo++;
    p->itens[p->topo] = nova;
}

void pop(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Não é possível remover.\n");
        return;
    }

    *removida = p->itens[p->topo];
    p->topo--;
}

void peek(Pilha *p, Peca *visualizada) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Nada para espiar.\n");
        return;
    }

    *visualizada = p->itens[p->topo];
}

void mostrarPilha(Pilha *p) {
    printf("Pilha (topo -> base):\n");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c, %d]\n", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

int main() {
    srand(time(NULL));

    Fila f;
    inicializarFila(&f);  // Inicializa a fila

    Pilha p;
    inicializarPilha(&p);

    // Vamos encher a fila antes de começar o jogo
    printf("--- Inicializando o sistema ---\n");
    for(int i = 0; i < MAX; i++) {
        inserir(&f); // Chama a função que criei lá fora
    }
    //menu do jogo
    int opcao;
    Peca pecaRemovida; // Variável auxiliar para guardar quem saiu

    do {
        printf("\n=== TETRIS STACK (Fila: %d/%d) ===\n", f.total, MAX);
        exibirFila(f); //chama função de exibir aqui
        mostrarPilha(&p); //Mostra a pilha em baixo da fila

        printf("1. Jogar peca (Remover)\n");
        printf("2. Reservar peça\n");
        printf("3. Usar peça reservada\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                // Passa o endereço da fila (&f) e o endereço da peca temporaria (&pecaRemovida)
                jogar(&f, &pecaRemovida); 
                printf(">>> Voce jogou a peca: [%c %d] <<<\n", pecaRemovida.nome, pecaRemovida.id);
                inserir(&f);
                break;
            case 2:
                // Verifica se a pilha cabe mais
                if (!pilhaCheia(&p)) {
                // 1. Tira da fila 
                jogar(&f, &pecaRemovida); 
        
                // 2. Coloca na pilha
                push(&p, pecaRemovida); 
        
                printf(">>> Voce reservou a peca: [%c %d] <<<\n", pecaRemovida.nome, pecaRemovida.id);
        
                // 3. Importante: Repor a fila!
                inserir(&f); 
                } else {
                    printf("Pilha cheia!\n");
                }
            break;
            case 3 :
                if (!pilhaVazia(&p)) {
                    pop(&p, &pecaRemovida);
                    printf(">>> Você usou a peça reservada: [%c %d] <<<\n", pecaRemovida.nome, pecaRemovida.id);      
                }
                else {
                    printf("Não ha peças na reserva!\n");
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);

    return 0;
}
    