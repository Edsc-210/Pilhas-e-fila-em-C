#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX 5
#define MAX_PILHA 3

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
    Fila invertida;
    inicializarFila(&f);  // Inicializa a fila
    inicializarFila(&invertida);

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
        printf("3. Trocar peça atual\n");
        printf("4. Troca multipla (trocar toda a reserva)\n");
        printf("5. Usar peça reservada\n");
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
                //Tira da fila 
                jogar(&f, &pecaRemovida); 
        
                // Coloca na pilha
                push(&p, pecaRemovida); 
        
                printf(">>> Voce reservou a peca: [%c %d] <<<\n", pecaRemovida.nome, pecaRemovida.id);
        
                // Importante: Repor a fila!
                inserir(&f); 
                } else {
                    printf("Pilha cheia!\n");
                }
            break;
            case 3:
                // Só dá pra trocar se tiver gente na fila E na pilha
                 if (!filaVazia(&f) && !pilhaVazia(&p)) {
        
                    // Guarda a peça da fila numa variável auxiliar (Temp)
                    Peca temp = f.itens[f.inicio];

                    // Coloca a peça da pilha na frente da fila
                    f.itens[f.inicio] = p.itens[p.topo];

                    // Coloca a peça que estava na temp (antiga fila) no topo da pilha
                    p.itens[p.topo] = temp;

                    printf(">>> Troca realizada com sucesso! <<<\n");
                } else {
                    printf("Erro: Fila ou Pilha vazia, impossivel trocar.\n");
                    }
            break;
            case 4:
                // Verifica se tem peças suficientes (3 na fila e 3 na pilha)
                // (Assumindo que MAX_PILHA é 3, ou verificando se p.topo == 2)
                if (f.total >= 3 && p.topo >= 2) { 
        
                    // Vamos trocar as 3 primeiras posições
                    for (int i = 0; i < 3; i++) {
            
                    // --- Lógica Matemática ---
                    // Na Fila: Precisamos andar circularmente -> (inicio + i) % MAX
                    // Na Pilha: Precisamos descer do topo -> (topo - i)
            
                    int indiceFila = (f.inicio + i) % MAX;
                    int indicePilha = p.topo - i;

                    // O Swap (A mesma lógica do case 3)
                    Peca temp = f.itens[indiceFila];
                    f.itens[indiceFila] = p.itens[indicePilha];
                    p.itens[indicePilha] = temp;
                }
                printf(">>> Troca MULTIPLA realizada! <<<\n");

                } else {
                    printf("Erro: Precisa de pelo menos 3 pecas na fila e na pilha cheia.\n");
            }
             break;

            case 5 :
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
    