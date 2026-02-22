// ============================================================================
//                            PROJETO WAR
// ============================================================================
// Funcionalidades:
// - Alocação Dinâmica (calloc/malloc)
// - Sistema de Ataque com Dados (rand)
// - Sistema de Missões Estratégicas
// - Modularização Completa
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// Estrutura do Território.
struct Territorio {
    char nome[30];
    char cor[10]; // Quem é o dono (Ex: "Azul", "Vermelho").
    int tropas;
};

// Protótipos (O Menu de Ferramentas).
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho);
void exibirMapa(struct Territorio* mapa, int tamanho);
void liberarMemoria(struct Territorio* mapa, char* missao);

// CÉREBRO PRINCIPAL.
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL)); // Semente do Caos para os dados.

    // 1. BANCO DE MISSÕES (As opções possíveis).
    char *listaMissoes[] = {
        "Dominar o Mundo (Ter todos os territórios)",
        "Exército Poderoso (Ter um território com 10+ tropas)",
        "Estrategista (Conquistar 3 territórios quaisquer)"
    };
    int totalMissoes = 3;

    // 2. PREPARAÇÃO DO MAPA (Alocação Dinâmica).
    int qtdTerritorios;
    printf("Quantos territórios no jogo? ");
    scanf("%d", &qtdTerritorios);

    // Usa CALLOC como pedido (limpa a memória com zeros).
    struct Territorio *mapa = (struct Territorio*) calloc(qtdTerritorios, sizeof(struct Territorio));
    
    // Aloca memória para a missão do jogador.
    char *missaoJogador = (char*) malloc(100 * sizeof(char));

    // 3. CADASTRO INICIAL.
    printf("\n--- SETUP DO MAPA ---\n");
    for(int i = 0; i < qtdTerritorios; i++) {
        printf("Território %d - Nome: ", i);
        scanf("%s", mapa[i].nome);
        printf("Dono (Cor): ");
        scanf("%s", mapa[i].cor);
        printf("Tropas iniciais: ");
        scanf("%d", &mapa[i].tropas);
    }

    // 4. SORTEIO DA MISSÃO.
    atribuirMissao(missaoJogador, listaMissoes, totalMissoes);
    printf("\n>>> SUA MISSÃO SECRETA: %s <<<\n", missaoJogador);

    // 5. LOOP DO JOGO (A Guerra).
    int opcao = 0;
    do {
        printf("\n=== COMANDO DE GUERRA ===\n");
        printf("1. Ver Mapa\n2. Atacar\n3. Verificar Vitória\n0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            exibirMapa(mapa, qtdTerritorios);
            printf("Missão Atual: %s\n", missaoJogador);
        }
        else if (opcao == 2) {
            int idAtq, idDef;
            printf("ID do Atacante (0 a %d): ", qtdTerritorios-1);
            scanf("%d", &idAtq);
            printf("ID do Defensor (0 a %d): ", qtdTerritorios-1);
            scanf("%d", &idDef);

            // Validação simples para não travar.
            if (idAtq >= 0 && idAtq < qtdTerritorios && idDef >= 0 && idDef < qtdTerritorios) {
                // Passando endereços de memória (&) para a função atacar
                atacar(&mapa[idAtq], &mapa[idDef]);
            } else {
                printf("Coordenadas inválidas!\n");
            }
        }
        else if (opcao == 3) {
            if (verificarMissao(missaoJogador, mapa, qtdTerritorios)) {
                printf("\n🎉 VITÓRIA! MISSÃO CUMPRIDA! 🎉\n");
                opcao = 0; // Fim de jogo
            } else {
                printf("\nMissão ainda não cumprida. Continue lutando!\n");
            }
        }

    } while (opcao != 0);

    // 6. LIMPEZA FINAL. (Free).
    liberarMemoria(mapa, missaoJogador);
    printf("Memória liberada. Sistema Offline.\n");

    return 0;
}

// ============================================================================
//         A OFICINA (IMPLEMENTAÇÃO DAS FUNÇÕES)
// ============================================================================

// Sorteia uma missão da lista e copia para a variável do jogador.
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]); // Copia o texto sorteado para o destino
}

// A lógica de batalha exigida nas imagens.
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    printf("\n--- BATALHA: %s vs %s ---\n", atacante->nome, defensor->nome);

    // Validação: Não pode atacar a si mesmo.
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: Fogo amigo! Você não pode atacar seu próprio time.\n");
        return;
    }
    if (atacante->tropas <= 1) {
        printf("Erro: Tropas insuficientes para atacar.\n");
        return;
    }

    // Rolagem de dados (1 a 6).
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;
    printf("Dados: Atacante [%d] x Defensor [%d]\n", dadoAtk, dadoDef);

    if (dadoAtk > dadoDef) {
        printf("Vitória do Ataque!\n");
        // Regra da imagem: Transfere a cor e metade das tropas.
        strcpy(defensor->cor, atacante->cor); // Troca o dono.
        defensor->tropas = atacante->tropas / 2; // Move metade
        atacante->tropas = atacante->tropas - defensor->tropas; // O que sobrou fica.
        printf("Território conquistado! Tropas movidas.\n");
    } else {
        printf("Defesa segurou!\n");
        // Regra da imagem: Atacante perde uma tropa.
        atacante->tropas--;
    }
}

// Verifica se a missão específica foi cumprida.
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho) {
    // Lógica para: "Exército Poderoso".
    if (strstr(missao, "Exército Poderoso") != NULL) {
        for(int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas >= 10) return 1; // Verdadeiro.
        }
    }
    // Lógica para: "Dominar o Mundo" (Simplificada: Verifica se todos tem a mesma cor do primeiro).
    else if (strstr(missao, "Dominar o Mundo") != NULL) {
        char* corDono = mapa[0].cor;
        for(int i = 1; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corDono) != 0) return 0; // Falso se achar cor diferente.
        }
        return 1; // Verdadeiro se passou por todos.
    }
    
    // Se for outra missão (ou lógica complexa), retorna 0 por enquanto.

    return 0;
}

void exibirMapa(struct Territorio* mapa, int tamanho) {
    printf("\nSTATUS GLOBAL:\n");
    for(int i = 0; i < tamanho; i++) {
        printf("[%d] %s | Dono: %s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void liberarMemoria(struct Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
}