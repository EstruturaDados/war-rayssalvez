#include <stdio.h>
#include <stdlib.h> // Necessário para calloc, malloc e free.
#include <string.h>
#include <time.h>   // Necessário para srand e rand (dados do ataque).

// 1. (Nossa Struct).
typedef struct {
    char nome[8];
    char cor[9];
    int tropas;
} Territorio;

// --- MÓDULOS ---

// Módulo para exibir o estado atual do mapa:
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n=== MAPA ATUALIZADO ===\n");
    for (int i = 0; i < tamanho; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=======================\n");
}

// 2. Função de Ataque usando Ponteiros.
void atacar(Territorio* atacante, Territorio* defensor) {
    // Validação: Não pode atacar um território aliado.
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n=> Movimento Invalido: Voce nao pode atacar seu proprio territorio!\n");
        return;
    }

    // Simulando dados de 1 a 6:
    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;
    
    printf("\n--- BATALHA: %s vs %s ---\n", atacante->nome, defensor->nome);
    printf("Ataque rolou: %d | Defesa rolou: %d\n", dado_ataque, dado_defesa);

    // Regras de vitória e derrota:
    if (dado_ataque > dado_defesa) {
        printf("RESULTADO: O atacante VENCEU a batalha!\n");
        
        // Transfere a cor (muda de dono).
        strcpy(defensor->cor, atacante->cor);
        
        // Transfere metade das tropas do atacante para o novo território.
        int tropas_transferidas = atacante->tropas / 2;
        defensor->tropas = tropas_transferidas;
        atacante->tropas -= tropas_transferidas;
        
    } else {
        printf("RESULTADO: O ataque FALHOU!\n");
        // Atacante perde uma tropa.
        atacante->tropas -= 1;
    }
}

// 4. (Liberar Memória).
void liberarMemoria(Territorio* mapa) {
    free(mapa); // Devolve a memória alocada dinamicamente.
}

// --- ARENA PRINCIPAL ---
int main() {
    // Garante que os dados sejam aleatórios a cada partida usando o tempo atual do PC.
    srand(time(NULL));

    int qtd_territorios;
    printf("--- WAR: NIVEL AVENTUREIRO ---\n");
    printf("Quantos territorios o mapa tera? ");
    scanf("%d", &qtd_territorios);

    // 1. Alocação Dinâmica com calloc.
    // Cria o mapa do tamanho exato que o usuário pediu e zera os valores.
    Territorio* mapa = (Territorio*) calloc(qtd_territorios, sizeof(Territorio));

    // Laço para preencher os territórios:
    for(int i = 0; i < qtd_territorios; i++) {
        printf("\n--- Cadastrando Territorio %d ---\n", i);
        printf("Nome: ");
        scanf("%s", mapa[i].nome);
        printf("Cor (ex: Vermelho, Azul): ");
        scanf("%s", mapa[i].cor);
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }

    exibirMapa(mapa, qtd_territorios);

    // Interface amigável para o usuário escolher o ataque:
    if (qtd_territorios >= 2) {
        int id_ataque, id_defesa;
        printf("\nChegou a hora do ataque!\n");
        printf("Digite o numero (ID) do territorio ATACANTE: ");
        scanf("%d", &id_ataque);
        printf("Digite o numero (ID) do territorio DEFENSOR: ");
        scanf("%d", &id_defesa);

        // Chamamos a função atacar passando os ENDEREÇOS (&) dos territórios escolhidos.
        atacar(&mapa[id_ataque], &mapa[id_defesa]);
        
        // Exibimos o mapa para ver as mudanças causadas pelos ponteiros:
        exibirMapa(mapa, qtd_territorios);
    } else {
        printf("\nSem territorios suficientes para uma batalha.\n");
    }

    // Fim de jogo: libera a memória.
    liberarMemoria(mapa);
    printf("Memoria liberada. Fim de jogo!\n");

    return 0;
}