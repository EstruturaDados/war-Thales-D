// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================

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


//---------------------Protótipo das funções----------------------------

void atacar(Territorio *atacante, Territorio *defensor); // Função de ataque: simula um combate entre dois territórios
void exibirTerritorios(Territorio *mapa, int n); // Exibe os dados de todos os territórios
void liberarMemoria(Territorio *mapa); // Função para liberar a memória alocada dinamicamente
void atribuirMissao(char *destino, char *missoes[], int totalMissoes); // Atribui uma missão aleatória a um jogador
int verificarMissao(char *missao, Territorio *mapa, int tamanho);
//-----------------------------------------------------------------------


int main() {

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int n;
    printf("Informe o número total de territórios: ");
    scanf("%d", &n);
    getchar(); // limpar buffer

    // Alocação dinâmica com calloc
    Territorio *mapa = (Territorio *) calloc(n, sizeof(Territorio));

     // Cadastro dos territórios
    printf("\n=== Cadastro de Territórios ===\n");

    for (int i = 0; i < n; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        printf("Nome do Território: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);

        printf("Cor do Exército: ");
        fgets(mapa[i].corExercito, sizeof(mapa[i].corExercito), stdin);

        printf("Quantidade de Tropas: ");
        scanf("%d", &mapa[i].quantidadeTropas);
        getchar();
    }
    // === Criação do vetor de missões ===
    char *missoes[] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas vermelhas",
        "Controlar todos os territórios com mais de 5 tropas",
        "Ter o dobro de territórios do inimigo",
        "Dominar o mapa inteiro"
    };
    int totalMissoes = 5;

    // === Sorteio e alocação dinâmica da missão do jogador ===
    char *missaoJogador = (char *) malloc(100 * sizeof(char));

    atribuirMissao(missaoJogador, missoes, totalMissoes);

    printf("\nSua missão é: %s\n", missaoJogador);
    
    int opcao;
    do {
        exibirTerritorios(mapa, n);

        printf("\n--- MENU ---\n");
        printf("1 - Simular ataque\n");
        printf("2 - Verificar missão\n");
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
                printf("Um território não pode atacar a si mesmo!\n");
                continue;
            }
            
            atacar(&mapa[idxAtacante - 1], &mapa[idxDefensor - 1]);

            // Após cada ataque, verifica se a missão foi cumprida
            if (verificarMissao(missaoJogador, mapa, n)) {
                printf("\nMISSÃO CUMPRIDA! Você venceu o jogo!\n");
                break;
            } else {
                printf("\nVocê ainda não cumpriu sua missão! Volte a LUTAR!\n");
            }
        } else {
            printf("\nSua missão é: %s\n", missaoJogador);
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    free(missaoJogador);
    printf("\nPrograma encerrado.\n");
    return 0;
}

//==============================FUNÇÕES========================================

void atacar(Territorio *atacante, Territorio *defensor) {
    printf("\n %s (%s) está atacando %s (%s)!\n",
           atacante->nome, atacante->corExercito,
           defensor->nome, defensor->corExercito);

    // Verifica se atacante tem tropas suficientes
    if (atacante->quantidadeTropas < 1) {
        printf("%s não pode atacar — possui apenas %d tropa(s)!\n",
               atacante->nome, atacante->quantidadeTropas);
        return;
    }

    
    // Geração de valores aleatórios de 1 a 6
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nDado Atacante: %d | Dado Defensor: %d\n",
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

//--------------------------------------------------------------------

void exibirTerritorios(Territorio *mapa, int n) {
    printf("\n=== ESTADO ATUAL DOS TERRITÓRIOS ===\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("  Nome: %s", mapa[i].nome);
        printf("  Cor do Exército: %s", mapa[i].corExercito);
        printf("  Tropas: %d\n", mapa[i].quantidadeTropas);
    }
    printf("=====================================\n");
}


// ----------Função para liberar a memória alocada dinamicamente---------------------------------------
void liberarMemoria(Territorio *mapa) {
    free(mapa);
    printf("\nMemória liberada com sucesso.\n");
}

// ====================Atribui uma missão aleatória a um jogador=======================================
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Verifica se a missão foi cumprida
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {

    const char *corJogador = "Azul\n";
    const char *corInimigo = "Vermelho\n";

    // === Missão 1: Conquistar 3 territórios seguidos ===
    if (strstr(missao, "Conquistar 3 territórios seguidos")==0) {
        int consecutivos  = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) == 0) {
                consecutivos ++;
                if (consecutivos  >= 3)
                    return 1; //missão cumprida
            } else {
                consecutivos = 0; // reinicia contagem
            }
        }
        return 0;
    }

    // === Missão 2: Eliminar todas as tropas vermelhas ===
    else if (strcmp(missao, "Eliminar todas as tropas vermelhas") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].corExercito, corInimigo) == 0) {
                return 0; // ainda existe inimigo vermelho
            }
        }
        return 1; // nenhum território vermelho restante
    }
   
     // === Missão 3: Controlar todos os territórios com mais de 5 tropas ===
    else if (strcmp(missao, "Controlar todos os territórios com mais de 5 tropas") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].quantidadeTropas > 5 &&
                strcmp(mapa[i].corExercito, corJogador) != 0) {
                return 0; // há território com mais de 5 tropas que não é do jogador
            }
        }
        return 1; // todos os territórios fortes são do jogador
    }

    // === Missão 4: Ter o dobro de territórios do inimigo ===
    else if (strcmp(missao, "Ter o dobro de territórios do inimigo") == 0) {
        int totalJogador = 0, totalInimigo = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) == 0)
                totalJogador++;
            else if (strcmp(mapa[i].corExercito, corInimigo) == 0)
                totalInimigo++;
        }

        // Se inimigo tiver 0 territórios, missão também é considerada cumprida
        if (totalInimigo == 0 || totalJogador >= 2 * totalInimigo)
            return 1;
        else
            return 0;
    }

    // === Missão 5: Dominar o mapa inteiro ===
    else if (strcmp(missao, "Dominar o mapa inteiro") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) != 0)
                return 0; // ainda há territórios de outra cor
        }
        return 1; // todos os territórios são do jogador
    }

    // Caso a missão não corresponda a nenhuma das acima
    return 0;
}
