// Inclusão das bibliotecas exigidas pelo Nível Novato:
#include <stdio.h>
#include <string.h>

// 1. Criação da Struct:
typedef struct {
    char nome[5]; // Espaço para nomes com até 29 letras.
    char cor[6];  // Espaço para cores com até 9 letras.
    int tropas;    // Número de tropas.
} Territorio;

int main() {
    // 2. Declaração do vetor de structs para 5 territórios:
    Territorio mapa[7];
    
    printf("--- BEM-VINDO AO CADASTRO DO WAR ---\n");
    printf("solicitando os dados de 5 territorios.\n\n");

    // 3. Entrada dos dados: Laço for para cadastrar os 5 territórios:
    for(int i = 0; i < 5; i++) {
        printf("--- Territorio %d ---\n", i + 1);
        
        // Mensagens claras orientando o usuário:
        printf("Digite o nome do territorio: ");
        // Usando scanf("%s", ...) conforme sugerido pela matéria para strings simples.
        scanf("%s", mapa[i].nome); 
        
        printf("Digite a cor do exercito (ex: Vermelho, Azul): ");
        scanf("%s", mapa[i].cor);
        
        printf("Digite a quantidade inicial de tropas: ");
        scanf("%d", &mapa[i].tropas);
        printf("\n");
    }

    // 4. Exibição dos dados: Percorrendo o vetor para mostrar tudo formatado.
    printf("--- RELATORIO FINAL DOS TERRITORIOS ---\n");
    for(int i = 0; i < 5; i++) {
        // Exibe os dados com tempo de resposta imediato, cumprindo o requisito de desempenho.
        printf("Territorio: %s | Cor: %s | Tropas: %d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }

    return 0;
}
