// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================

// Inclusão das bibliotecas.
#include <stdio.h>
#include <stdlib.h> // Malloc, Calloc, Free, Rand, Srand.
#include <string.h> // Strcpy, Strcmp.
#include <time.h>   // Time.
#include <locale.h> // Acentos.

// --- Constantes Globais ---
#define TOTAL_TERRITORIOS 5
#define TOTAL_MISSOES 3
#define TAM_NOME 30

// --- Estrutura de Dados ---
struct Territorio {
    char nome[TAM_NOME];
    char dono[TAM_NOME]; // Quem domina (Jogador ou Inimigo).
    int tropas;
};

// --- Protótipos das Funções ---
struct Territorio* alocarMapa();
void inicializarTerritorios(struct Territorio *mapa);
void liberarMemoria(struct Territorio *mapa);
void exibirMenuPrincipal();
void exibirMapa(const struct Territorio *mapa); // Apenas lê (const).
void exibirMissao(int idMissao);
void faseDeAtaque(struct Territorio *mapa);
void simularAtaque(struct Territorio *origem, struct Territorio *destino);
int sortearMissao();
int verificarVitoria(const struct Territorio *mapa, int idMissao);
void limparBufferEntrada();

// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL)); // Semente do Caos.

    // Aloca a memória
    struct Territorio *mapa = alocarMapa();
    
    // Verifica se a alocação foi bem-sucedida.
    if (mapa == NULL) {
        printf("ERRO CRÍTICO: Falha na alocação de memória (RAM insuficiente)!\n");
        return 1;
    }

    // Preenche os dados.
    inicializarTerritorios(mapa);

    // Sorteia missão.
    int missaoAtual = sortearMissao();
    int opcao = 0;

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        // Limpa a tela (truque simples).
        printf("\n\n\n"); 
        
        exibirMissao(missaoAtual);
        exibirMapa(mapa);
        exibirMenuPrincipal();

        printf("Comando: ");
        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpa o "Enter" que sobra.

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case 2:
                if (verificarVitoria(mapa, missaoAtual)) {
                    printf("\n🎉 PARABÉNS! MISSÃO CUMPRIDA! O MUNDO É SEU! 🎉\n");
                    opcao = 0; // Encerra o jogo.
                } else {
                    printf("\n⚠️ Objetivo ainda não alcançado. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\nEncerrando simulação...\n");
                break;
            default:
                printf("\nComando inválido! Tente de novo.\n");
        }
        
        // Pausa para leitura.
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);

    // 3. Limpeza:
    liberarMemoria(mapa);
    printf("Memória liberada. Fim de jogo.\n");

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
struct Territorio* alocarMapa() {
    // Calloc é melhor que Malloc aqui porque já limpa a memória (zera tudo).
    return (struct Territorio*) calloc(TOTAL_TERRITORIOS, sizeof(struct Territorio));
}

// inicializarTerritorios():
void inicializarTerritorios(struct Territorio *mapa) {
    // Configurando dados iniciais (Hardcoded para teste).
    strcpy(mapa[0].nome, "Brasil");      strcpy(mapa[0].dono, "Jogador"); mapa[0].tropas = 5;
    strcpy(mapa[1].nome, "Argentina");   strcpy(mapa[1].dono, "Inimigo"); mapa[1].tropas = 2;
    strcpy(mapa[2].nome, "EUA");         strcpy(mapa[2].dono, "Inimigo"); mapa[2].tropas = 3;
    strcpy(mapa[3].nome, "China");       strcpy(mapa[3].dono, "Inimigo"); mapa[3].tropas = 4;
    strcpy(mapa[4].nome, "Australia");   strcpy(mapa[4].dono, "Inimigo"); mapa[4].tropas = 2;
}

// liberarMemoria():
void liberarMemoria(struct Territorio *mapa) {
    free(mapa); // Devolve a memória pro sistema.
}

// exibirMenuPrincipal():
void exibirMenuPrincipal() {
    printf("\n=== PAINEL DE CONTROLE ===\n");
    printf("1. Atacar Território\n");
    printf("2. Verificar Vitória\n");
    printf("0. Sair do Jogo\n");
    printf("=========================\n");
}

// exibirMapa():
void exibirMapa(const struct Territorio *mapa) {
    printf("\n--- MAPA MUNDIAL ---\n");
    for(int i = 0; i < TOTAL_TERRITORIOS; i++) {
        printf("[%d] %-10s | Dono: %-7s | Tropas: %d\n", 
               i, mapa[i].nome, mapa[i].dono, mapa[i].tropas);
    }
    printf("--------------------\n");
}

// exibirMissao():
void exibirMissao(int idMissao) {
    printf("\n>>> SUA MISSÃO SECRETA (ID: %d) <<<\n", idMissao);
    if (idMissao == 1) printf("Objetivo: Conquistar a 'China'.\n");
    else if (idMissao == 2) printf("Objetivo: Ter mais de 10 tropas no 'Brasil'.\n");
    else printf("Objetivo: Eliminar a 'Argentina'.\n");
}

// faseDeAtaque():
void faseDeAtaque(struct Territorio *mapa) {
    int origem, destino;
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha o ID do seu território (Origem): ");
    scanf("%d", &origem);
    printf("Escolha o ID do alvo (Destino): ");
    scanf("%d", &destino);
    limparBufferEntrada();

    // Validação básica
    if (origem < 0 || origem >= TOTAL_TERRITORIOS || destino < 0 || destino >= TOTAL_TERRITORIOS) {
        printf("Erro: Coordenadas inválidas!\n");
        return;
    }

    simularAtaque(&mapa[origem], &mapa[destino]); // Passa o ENDEREÇO específico.
}

// simularAtaque():
void simularAtaque(struct Territorio *origem, struct Territorio *destino) {
    // Regra 1: Só pode atacar se for seu.
    if (strcmp(origem->dono, "Jogador") != 0) {
        printf("General, você não comanda esse território!\n");
        return;
    }
    // Regra 2: Precisa de tropas.
    if (origem->tropas <= 1) {
        printf("Tropas insuficientes para ataque (Mínimo 2)!\n");
        return;
    }

    printf("\nBatalha iniciada: %s vs %s!\n", origem->nome, destino->nome);
    
    // Rola os dados (0 a 9).
    int dadoAtaque = rand() % 10;
    int dadoDefesa = rand() % 10;

    printf("Ataque rolou: %d | Defesa rolou: %d\n", dadoAtaque, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("VITÓRIA! O inimigo perdeu 1 tropa.\n");
        destino->tropas--;
        
        // Conquista.
        if (destino->tropas <= 0) {
            printf("TERRITÓRIO CONQUISTADO! %s agora é seu!\n", destino->nome);
            strcpy(destino->dono, "Jogador");
            destino->tropas = 1; // Ocupação.
            origem->tropas--;    // Deslocamento.
        }
    } else {
        printf("DERROTA! Você perdeu 1 tropa.\n");
        origem->tropas--;
    }
}

// sortearMissao():
int sortearMissao() {
    return (rand() % TOTAL_MISSOES) + 1;
}

// verificarVitoria():
int verificarVitoria(const struct Territorio *mapa, int idMissao) {
    if (idMissao == 1) { // Conquistar China (Índice 3).
        return (strcmp(mapa[3].dono, "Jogador") == 0);
    }
    if (idMissao == 2) { // Brasil (Índice 0) ter > 10 tropas.
        return (mapa[0].tropas > 10);
    }
    if (idMissao == 3) { // Argentina (Índice 1) ser nossa.
        return (strcmp(mapa[1].dono, "Jogador") == 0);
    }
    return 0;
}

// limparBufferEntrada():
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}