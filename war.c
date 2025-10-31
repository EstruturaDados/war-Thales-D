// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
//int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

    //return 0;
//}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da struct Territorio
typedef struct {
    char nome[30];
    char corExercito[10];
    int quantidadeTropas;
} Territorio;

// Função de ataque: simula um combate entre dois territórios
void atacar(Territorio *atacante, Territorio *defensor) {
    printf("\n %s (%s) está atacando %s (%s)!\n",
           atacante->nome, atacante->corExercito,
           defensor->nome, defensor->corExercito);

    // Verifica se atacante tem tropas suficientes
    if (atacante->quantidadeTropas <= 1) {
        printf("%s não pode atacar — possui apenas %d tropa(s)!\n",
               atacante->nome, atacante->quantidadeTropas);
        return;
    }

    
    // Geração de valores aleatórios de 1 a 6
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado Atacante: %d | Dado Defensor: %d\n",
           dadoAtacante, dadoDefensor);

    // Determina o vencedor
    if (dadoAtacante > dadoDefensor) {
        printf("%s venceu a batalha!\n", atacante->nome);

        // Reduz uma tropa do defensor
        defensor->quantidadeTropas--;

        // Se o defensor perdeu todas as tropas, muda de dono
        if (defensor->quantidadeTropas <= 0) {
            printf(" %s foi conquistado por %s!\n",
                   defensor->nome, atacante->nome);

            // Atualiza a cor do exército
            strcpy(defensor->corExercito, atacante->corExercito);

            // Transfere metade das tropas
            defensor->quantidadeTropas = atacante->quantidadeTropas / 2;
            atacante->quantidadeTropas /= 2;
        }
    } else {
        printf(" %s defendeu com sucesso!\n", defensor->nome);
        atacante->quantidadeTropas--; // atacante perde uma tropa
    }
}


// Exibe os dados de todos os territórios
void exibirTerritorios(Territorio *mapa, int n) {
    printf("\n=== ESTADO ATUAL DOS TERRITÓRIOS ===\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("  Nome: %s\n", mapa[i].nome);
        printf("  Cor do Exército: %s\n", mapa[i].corExercito);
        printf("  Tropas: %d\n", mapa[i].quantidadeTropas);
    }
    printf("=====================================\n");
}

// Função para liberar a memória alocada dinamicamente
void liberarMemoria(Territorio *mapa) {
    free(mapa);
    printf("\n💾 Memória liberada com sucesso.\n");
}

int main() {

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int n;
    printf("Informe o número total de territórios: ");
    scanf("%d", &n);
    getchar(); // limpar buffer

    // Alocação dinâmica com calloc
    Territorio *mapa = (Territorio *) calloc(n, sizeof(Territorio));

    int i;

    //printf("=== Cadastro de Territórios ===\n\n");

     // Cadastro dos territórios
    printf("\n=== Cadastro de Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        removerNovaLinha(mapa[i].nome);

        printf("Cor do Exército: ");
        fgets(mapa[i].corExercito, sizeof(mapa[i].corExercito), stdin);
        removerNovaLinha(mapa[i].corExercito);

        printf("Quantidade de Tropas: ");
        scanf("%d", &mapa[i].quantidadeTropas);
        getchar();
    }

    int opcao;
    do {
        exibirTerritorios(mapa, n);

        printf("\n--- MENU ---\n");
        printf("1 - Simular ataque\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            int idxAtacante, idxDefensor;

            printf("Número do território atacante (1-%d): ", n);
            scanf("%d", &idxAtacante);
            printf("Número do território defensor (1-%d): ", n);
            scanf("%d", &idxDefensor);
            getchar();

            if (idxAtacante < 1 || idxAtacante > n ||
                idxDefensor < 1 || idxDefensor > n) {
                printf("Índices inválidos!\n");
                continue;
            }

            if (idxAtacante == idxDefensor) {
                printf("❌ Um território não pode atacar a si mesmo!\n");
                continue;
            }
            
            atacar(&mapa[idxAtacante - 1], &mapa[idxDefensor - 1]);
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    printf("\nPrograma encerrado.\n");
    return 0;
}
