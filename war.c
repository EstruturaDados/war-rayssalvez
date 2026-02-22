#include <stdio.h>
#include <stdlib.h> // Para malloc, calloc, free.
#include <string.h> // Para manipulação de textos (strcpy).
#include <time.h>   // Para srand e rand.

// 1. (Struct) com os tamanhos exatos exigidos:
typedef struct {
    char nome[6];
    char cor[7];
    int tropas;
} Territorio;

// --- MÓDULOS (A Divisão de Tarefas) ---

// Módulo 1: Exibir Mapa (Holograma / Passagem por valor):
void exibirMapa(Territorio mapa[], int tamanho) {
    printf("\n=== PANORAMA DO JOGO ===\n");
    for (int i = 0; i < tamanho; i++) {
        printf("ID: %d | Territorio: %s | Cor: %s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("========================\n");
}

// Módulo 2: (Ataque com Ponteiros / Passagem por Referência):
void atacar(Territorio* atacante, Territorio* defensor) {
    // Validação de segurança: Não pode atacar aliado.
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\nMovimento Invalido! Voce nao pode atacar um territorio da sua propria cor.\n");
        return;
    }

    // Simulando os dados de ataque e defesa (1 a 6).
    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;
    
    printf("\n--- BATALHA: %s ataca %s ---\n", atacante->nome, defensor->nome);
    printf("Dado Atacante: %d | Dado Defensor: %d\n", dado_ataque, dado_defesa);

    // Regras de combate:
    if (dado_ataque > dado_defesa) {
        printf("-> Vitoria do atacante! O territorio mudou de dono.\n");
        // O defensor perde o controle, ganha a cor do atacante e metade das tropas dele.
        strcpy(defensor->cor, atacante->cor);
        int tropas_transferidas = atacante->tropas / 2;
        defensor->tropas = tropas_transferidas;
        atacante->tropas -= tropas_transferidas;
    } else {
        printf("-> O ataque falhou. Uma tropa atacante foi destruida.\n");
        atacante->tropas -= 1;
    }
}

// Módulo 3: Atribuir Missão (Ponteiros e Sorteio).
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    // Copia o texto sorteado direto para o endereço de memória do jogador.
    strcpy(destino, missoes[sorteio]); 
}

// Módulo 4: Verificar Missão (Lógica inicial simples de verificação).
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    printf("\n[Sistema avaliando a missao: '%s'...]\n", missao);
    // Retorna 0 (falso) como estado padrão por enquanto.
    return 0; 
}

// Módulo 5: (Liberação de Memória):
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    free(mapa);
    free(missaoJogador);
}

// --- ARENA PRINCIPAL ---
int main() {
    // Garantindo que os números de sorteio sejam sempre diferentes usando o tempo real.
    srand(time(NULL));

    printf("--- WAR: NIVEL MESTRE ---\n");

    // Alocação Dinâmica:
    int qtd_territorios;
    printf("Quantos territorios deseja criar para esta partida? ");
    scanf("%d", &qtd_territorios);

    // Alocando o mapa usando calloc para que já comece limpo (zerado).
    Territorio* mapa = (Territorio*) calloc(qtd_territorios, sizeof(Territorio));

    // Lendo os dados de cada território:
    for(int i = 0; i < qtd_territorios; i++) {
        printf("\n--- Territorio %d ---\n", i);
        printf("Nome do territorio: ");
        scanf("%s", mapa[i].nome);
        printf("Cor do exercito dominante: ");
        scanf("%s", mapa[i].cor);
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }

    // O Vetor de Missões (As 5 opções).
    char* banco_de_missoes[8] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Conquistar a Asia",
        "Conquistar a America do Sul",
        "Dominar 5 territorios"
    };

    // Alocando espaço para o texto da missão do jogador:
    char* minha_missao = (char*) malloc(100 * sizeof(char));

    // Missão:
    atribuirMissao(minha_missao, banco_de_missoes, 5);
    
    // Passando o texto por valor (apenas para exibição limpa):
    printf("\n*** SUA MISSAO ESTRATEGICA ***\n%s\n", minha_missao);

    // Exibindo o estado do mundo:
    exibirMapa(mapa, qtd_territorios);

    // Simulando interação de combate se houver espaço.
    if (qtd_territorios >= 2) {
        int id_ataque, id_defesa;
        printf("\nFase de Ataque!\n");
        printf("Digite o ID do territorio ATACANTE: ");
        scanf("%d", &id_ataque);
        printf("Digite o ID do territorio DEFENSOR: ");
        scanf("%d", &id_defesa);

        // Ataques usando endereços de memória (&).
        atacar(&mapa[id_ataque], &mapa[id_defesa]);
        
        // Verificando o estrago da batalha.
        exibirMapa(mapa, qtd_territorios);
    } else {
        printf("\nO mapa e pequeno demais para gerar combates.\n");
    }

    // Verificação silenciosa de condição de vitória no fim do turno.
    int venceu = verificarMissao(minha_missao, mapa, qtd_territorios);
    if(venceu == 1) {
        printf("\nPARABENS! Voce completou sua missao e venceu o jogo!\n");
    }

    // (evitando memory leaks).
    liberarMemoria(mapa, minha_missao);
    printf("\nSistema: Memoria liberada com sucesso. Fim de jogo!\n");

    return 0;
}