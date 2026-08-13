#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

void limpar_buffer()
{
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF);
}

void embaralhar(char auxiliar[], int tamanho)
{
    for (int i = tamanho - 1; i > 0; i--)
    {
        int j = rand() % i; 
        char temp = auxiliar[i];
        auxiliar[i] = auxiliar[j];
        auxiliar[j] = temp;
    }
}

typedef struct
{
    char nome[51];
    int nivel;
    int tentativas;
} Ranking;

typedef struct
{
    int *cores;
    int tam_seq;
    int max_tentativas;
} Infos;

typedef struct
{
    Infos info;
    Ranking ranking;
    int **historico;
    char **historico_res;
    bool existe;
} JogoPausado;

void novoJogo(char opcao, bool jogo_em_andamento, bool *continuar_rodando, JogoPausado *jp);
void ajuda(char opcao, bool jogo_em_andamento, bool *continuar_rodando, JogoPausado *jp);
int sair(char opcao, bool jogo_em_andamento, bool *continuar_rodando, JogoPausado *jp);
void carregarJogo(char opcao, bool jogo_em_andamento, bool *continuar_rodando, JogoPausado *jp);
void salvarJogo(JogoPausado *jp); 
void avaliarTentativa(int tentativas[], int cores[], char resultado[], int tam_seq);
bool jogarPartida(Infos *info, Ranking *ranking, int **historico, char **historico_res);
void guardarJogoPausado(JogoPausado *jp, Infos *info, Ranking *ranking, int **historico, char **historico_res); 
void liberarJogoPausado(JogoPausado *jp); 
void mostrarRanking();
void atualizarRanking(Ranking novo);

int main(void)
{
    printf("\033[32m");
    srand(time(NULL)); 
    char opcao;
    bool jogo_em_andamento = false;
    bool continuar_rodando = true;

    JogoPausado jp;
    jp.existe = false;
    jp.historico = NULL;
    jp.historico_res = NULL;

    do
    {
        printf("\n");
        printf("==============================\n");
        printf("JOGO CÓDIGO SECRETO DE CORES\n");
        printf("==============================\n\n");

        printf("Opções de jogo:\n");
        printf("\nA - Ajuda\nN - Novo Jogo\nC - Carregar Jogo\nS - Salvar Jogo\nR - Ranking\nX - Sair\n");

        do
        {
            printf("\nDigite a opção: ");
            scanf(" %c", &opcao);
            char proximo_caractere = getchar(); 

            if (proximo_caractere != '\n') 
            {
                limpar_buffer();
                opcao = 'J'; 
            }

            opcao = toupper(opcao);

            if(opcao != 'A' && opcao != 'N' && opcao != 'C' && opcao != 'S' && opcao != 'R' && opcao != 'X')
            {
                printf("Opção invalida! Por favor tente novamente");
            }
        }
        while(opcao != 'A' && opcao != 'N' && opcao != 'C' && opcao != 'S' && opcao != 'R' && opcao != 'X'); 
        printf("\n");

        switch(opcao)
        {
            case 'A':
            {
                ajuda(opcao, jogo_em_andamento, &continuar_rodando, &jp);
                break;
            }
            case 'N':
            {
                jogo_em_andamento = true;
                novoJogo(opcao, jogo_em_andamento, &continuar_rodando, &jp);
                break;
            }
            case 'C':
            {
                carregarJogo(opcao, jogo_em_andamento, &continuar_rodando, &jp);
                break;
            }
            case 'S':
            {
                salvarJogo(&jp);
                break;
            }
            case 'R':
            {
                mostrarRanking(); 
                break;
            }
            case 'X':
            {
                sair(opcao, jogo_em_andamento, &continuar_rodando, &jp);
                break;
            }
        }
    }
    while(continuar_rodando);

    liberarJogoPausado(&jp);

    printf("\033[0m");
    return 0;
}

void ajuda(char opcao, bool jogo_em_andamento, bool *continuar_rodando, JogoPausado *jp)
{
    printf("O programa irá gerar aleatoriamente uma sequência de cores. O jogador não verá essa sequência no início da partida.\n");
    printf("A cada tentativa, o jogador deve informar uma sequência de cores com o mesmo tamanho da sequência secreta. Após cada tentativa, o programa irá informar ao jogador o resultado de sua jogada\n");
    printf("No resultado o - significa que o numero escolhido não existe na sequencia gerada, o E significa que o numero escolhido esta na sequencia mas na posição errada, e o C mostra que o numero escolhido esta certo na posição certa\n");

    printf("\nO jogo conta com uma lista fixa de cores, disponivel na tabela abaixo");
    printf("\n______________________\n\n");
    printf("   Código     Cor");
    printf("\n______________________\n");
    printf("\n");
    printf("     1      Vermelho\n     2      Azul\n     3      Verde\n     4      Amarelo\n     5      Roxo\n     6      Laranja\n");
    printf("______________________\n");

    printf("\nAdemais, o jogo terá três níveis de dificuldade, disponiveis na tabela abaixo");
    printf("\n__________________________________________________________________\n\n");
    printf("    Nível    Tamanho da sequência    Número de tentativas");
    printf("\n__________________________________________________________________\n");
    printf("\n");
    printf("    Fácil         4 cores                   10\n");
    printf("    Médio         5 cores                   12\n");
    printf("   Difícil        6 cores                   15\n");
    printf("__________________________________________________________________\n");

    char resposta;
    do
    {
        printf("\nDeseja voltar ao menu principal? (S/N): ");
        scanf(" %c", &resposta);
        char proximo_caractere = getchar(); 

        if (proximo_caractere != '\n') 
        {
            limpar_buffer();
            resposta = 'J'; 
        }

        resposta = toupper(resposta);

        if(resposta != 'S' && resposta != 'N')
        {
            printf("Opção invalida! Por favor tente novamente");
        }
    }
    while(resposta != 'S' && resposta != 'N');

    if(resposta == 'N')
    {
        sair(opcao, jogo_em_andamento, continuar_rodando, jp);
    }
}

int sair(char opcao, bool jogo_em_andamento, bool *continuar_rodando, JogoPausado *jp)
{
    if(jogo_em_andamento)
    {
        char auxiliar;
        do
        {
            printf("Há um jogo em andamento! Deseja salvar antes de sair? (S/N): ");
            scanf(" %c", &auxiliar);

            char proximo_caractere = getchar(); 

            if (proximo_caractere != '\n') 
            {
                limpar_buffer();
                auxiliar = 'J'; 
            }

            auxiliar = toupper(auxiliar);

            if(auxiliar != 'S' && auxiliar != 'N')
            {
                printf("Opção invalida! Por favor tente novamente\n");
            }
        }
        while(auxiliar != 'S' && auxiliar != 'N');

        if (auxiliar == 'S')
        {
            salvarJogo(jp);
            printf("Seu jogo foi salvo!\n");
        }
        
        printf("Jogo encerrado!\n");
        *continuar_rodando = false;
        return (auxiliar == 'S') ? 1 : 0;
    }
    else
    {
        printf("Jogo encerrado!\n");
        *continuar_rodando = false;
        return 0;
    }
}

void novoJogo(char opcao, bool jogo_em_andamento, bool *continuar_rodando, JogoPausado *jp)
{
    Infos info;
    Ranking ranking;
    char nivel;

    printf("Digite seu nome: "); 
    scanf(" %50[^\n]", ranking.nome); 
    limpar_buffer();

    printf("Escolha o nível:\n1 - Fácil (4 cores, 10 tentativas)\n2 - Médio (5 cores, 12 tentativas)\n3 - Difícil (6 cores, 15 tentativas)\n");

    do
    {
        printf("\nOpção: ");
        scanf(" %c", &nivel);
        char proximo_caractere = getchar(); 

        if (proximo_caractere != '\n')
        {
            limpar_buffer();
            nivel = 'J';
        }
        printf("\n");

        if(nivel != '1' && nivel != '2' && nivel != '3')
        {
            printf("Opção indisponivel! Tente novamente\n");
        }
    }
    while(nivel != '1' && nivel != '2' && nivel != '3');

    ranking.nivel = nivel - '0';

    if (nivel == '1') 
    {
        info.tam_seq = 4;
        info.max_tentativas = 10;
    } 
    else if (nivel == '2') 
    {
        info.tam_seq = 5;
        info.max_tentativas = 12;
    } 
    else 
    {
        info.tam_seq = 6;
        info.max_tentativas = 15;
    }

    printf("Cores disponíveis:\n1 - Vermelho\n2 - Azul\n3 - Verde\n4 - Amarelo\n5 - Roxo\n6 - Laranja\n");

    int **historico = (int **)calloc(info.max_tentativas, sizeof(int *));
    char **historico_res = (char **)calloc(info.max_tentativas, sizeof(char *));
    for (int i = 0; i < info.max_tentativas; i++) 
    {
        historico[i] = (int *)calloc(info.tam_seq, sizeof(int));
        historico_res[i] = (char *)calloc(info.tam_seq, sizeof(char));
    }
    info.cores = (int *)malloc(info.tam_seq * sizeof(int));

    for (int i = 0; i < info.tam_seq; i++) 
    {
        info.cores[i] = (rand() % 6) + 1;
    }

    ranking.tentativas = 1;

    bool voltar_menu = jogarPartida(&info, &ranking, historico, historico_res);

    if (voltar_menu)
    {
        guardarJogoPausado(jp, &info, &ranking, historico, historico_res);
        printf("\nVoltando ao menu...\n");
        return;
    }

    for (int i = 0; i < info.max_tentativas; i++)
    {
        free(historico[i]);
        free(historico_res[i]);
    }
    free(historico);
    free(historico_res);
    free(info.cores);
}

void avaliarTentativa(int tentativas[], int cores[], char resultado[], int tam_seq)
{
    bool ocorrencias[6] = { false };

    for (int z = 0; z < tam_seq; z++)
    {
        resultado[z] = '-';
        if (tentativas[z] == cores[z])
        {
            resultado[z] = 'C';
            ocorrencias[z] = true;
        }
    }

    for (int z = 0; z < tam_seq; z++)
    {
        if (resultado[z] == 'C')
        {
            continue;
        }

        for (int k = 0; k < tam_seq; k++)
        {
            if (!ocorrencias[k] && tentativas[z] == cores[k])
            {
                resultado[z] = 'E';
                ocorrencias[k] = true;
                break;
            }
        }
    }

    embaralhar(resultado, tam_seq);
}

bool jogarPartida(Infos *info, Ranking *ranking, int **historico, char **historico_res)
{
    int *tentativas = (int *)malloc(info->tam_seq * sizeof(int));
    char *auxiliar = (char *)malloc(info->tam_seq * sizeof(char));

    char lixo;
    int acertos_exatos = 0;
    int entrada_valida;
    bool voltar_menu = false;

    for ( ; ranking->tentativas <= info->max_tentativas; ranking->tentativas++)
    {
        acertos_exatos = 0;
        printf("\nTentativa %d de %d\n", ranking->tentativas, info->max_tentativas);

        do 
        {
            entrada_valida = 1;

            printf("Digite %d cores (ou 0 para voltar ao menu): ", info->tam_seq);
            for (int e = 0; e < info->tam_seq; e++)
            {
                if (scanf("%d", &tentativas[e]) != 1)
                {
                    limpar_buffer();
                    entrada_valida = 0;
                    break;
                }
                if (tentativas[e] == 0)
                {
                    char resposta_zero;
                    do
                    {
                        printf("Voce digitou 0! Deseja voltar ao menu principal (S/N):");
                        scanf(" %c", &resposta_zero);

                        char proximo_caractere = getchar(); 

                        if (proximo_caractere != '\n') 
                        {
                            limpar_buffer();
                            resposta_zero = 'J'; 
                        }

                        resposta_zero = toupper(resposta_zero);

                        if(resposta_zero != 'S' && resposta_zero != 'N')
                        {
                            printf("Opção invalida! Por favor tente novamente\n");
                        }
                    }
                    while(resposta_zero != 'S' && resposta_zero != 'N');

                    if(resposta_zero == 'S')
                    {
                        voltar_menu = true;
                        break;
                    }
                    else if(resposta_zero == 'N')
                    {
                        entrada_valida = 2;
                        break;
                    }
                }
                if (tentativas[e] < 1 || tentativas[e] > 6)
                {
                    entrada_valida = 0;
                }
            }   

            if (voltar_menu)
            {
                break;
            } 
            if (entrada_valida == 1)
            {
                if(getchar() != '\n' && getchar() != EOF)
                {
                    printf("Voce digitou mais de %d valores!\n",info->tam_seq);
                    entrada_valida = 2;
                }
            }
            if (entrada_valida == 0) 
            {
                printf("Voce digitou um valor invalido! Use apenas os valores da tabela!.\n\n");
            }
            else if(entrada_valida == 2)
            {
                printf("Retornando de onde você parou\n");
            }

        } 
        while (entrada_valida == 0 || entrada_valida == 2);

        if (voltar_menu)
        {
            break;
        }

        avaliarTentativa(tentativas, info->cores, auxiliar, info->tam_seq);

        acertos_exatos = 0;
        for (int z = 0; z < info->tam_seq; z++)
        {
            if (auxiliar[z] == 'C')
            {
                acertos_exatos++;
            }
        }

        for (int e = 0; e < info->tam_seq; e++)
        {
            historico[ranking->tentativas - 1][e] = tentativas[e];
            historico_res[ranking->tentativas - 1][e] = auxiliar[e];
        }

        printf("Resultado:\n");
        for (int r = 0; r < ranking->tentativas; r++)
        {
            printf("Rodada %d: ", r + 1);
            for (int e = 0; e < info->tam_seq; e++)
            {
                printf("%d ", historico[r][e]);
            }
            printf("(");
            for (int d = 0; d < info->tam_seq; d++)
            {
                printf(" %c ", historico_res[r][d]);
            }
            printf(")\n");
        }

        if (acertos_exatos == info->tam_seq)
        { 
            printf("\nParabéns, %s! Você venceu!\n", ranking->nome);
            
            atualizarRanking(*ranking);

            limpar_buffer();
            printf("Pressione uma tecla para o voltar ao menu principal!");
            scanf(" %c", &lixo);
            break;
        }

        if (ranking->tentativas == info->max_tentativas)
        {
            printf("\nQue pena, %s! As tentativas acabaram.\n", ranking->nome);
            printf("A sequencia correta era: ");
            for(int i = 0; i < info->tam_seq; i++)
            {
                printf("%d ", info->cores[i]);
            }
            printf("\n");
            limpar_buffer();
            printf("Pressione uma tecla para o voltar ao menu principal!");
            scanf(" %c", &lixo);
        }
    }

    free(tentativas);
    free(auxiliar);

    return voltar_menu;
}

void guardarJogoPausado(JogoPausado *jp, Infos *info, Ranking *ranking, int **historico, char **historico_res)
{
    liberarJogoPausado(jp);

    jp->info = *info;
    jp->ranking = *ranking;
    jp->historico = historico;
    jp->historico_res = historico_res;
    jp->existe = true;
}

void liberarJogoPausado(JogoPausado *jp)
{
    if (jp->existe)
    {
        for (int i = 0; i < jp->info.max_tentativas; i++)
        {
            free(jp->historico[i]);
            free(jp->historico_res[i]);
        }
        free(jp->historico);
        free(jp->historico_res);
        free(jp->info.cores);

        jp->historico = NULL;
        jp->historico_res = NULL;
        jp->existe = false;
        jp->info.cores = NULL;
    }
}

void salvarJogo(JogoPausado *jp)
{
    if (!jp->existe)
    {
        printf("Não há nenhum jogo em andamento para salvar!\n");
        printf("Inicie um jogo com a opção N e digite 0 durante uma tentativa para pausá-lo antes de salvar.\n");
        return;
    }

    char nome_arquivo[110];
    printf("Digite o nome do arquivo (sem extensão): ");
    scanf(" %99[^\n]", nome_arquivo);
    limpar_buffer();

    strcat(nome_arquivo, ".cor");

    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL)
    {
        printf("Não foi possível criar o arquivo!\n");
        return;
    }

    char nivel_letra = 'F';
    if (jp->ranking.nivel == 2)
    {
        nivel_letra = 'M';
    }
    else if (jp->ranking.nivel == 3)
    {
        nivel_letra = 'D';
    }

    fprintf(arquivo, "%s\n", jp->ranking.nome);
    fprintf(arquivo, "%c\n", nivel_letra);

    for (int i = 0; i < jp->info.tam_seq; i++)
    {
        fprintf(arquivo, "%d", jp->info.cores[i]);
        if (i < jp->info.tam_seq - 1)
        {
            fprintf(arquivo, " ");
        }
    }
    fprintf(arquivo, "\n");

    int tentativas_jogadas = jp->ranking.tentativas - 1;
    fprintf(arquivo, "%d\n", tentativas_jogadas);

    for (int i = 0; i < tentativas_jogadas; i++)
    {
        for (int k = 0; k < jp->info.tam_seq; k++) 
        {
            fprintf(arquivo, "%d", jp->historico[i][k]);
            if (k < jp->info.tam_seq - 1)
            {
                fprintf(arquivo, " ");
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Jogo salvo com sucesso em \"%s\"!\n", nome_arquivo);
}

void carregarJogo(char opcao, bool jogo_em_andamento, bool *continuar_rodando, JogoPausado *jp)
{
    char nome_arquivo[110];
    printf("Digite o nome do arquivo (sem extensão): ");
    scanf(" %99[^\n]", nome_arquivo);
    limpar_buffer();

    strcat(nome_arquivo, ".cor");

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        printf("Arquivo \"%s\" não encontrado!\n", nome_arquivo);
        return;
    }

    Infos info;
    Ranking ranking;
    char nivel_letra;

    fscanf(arquivo, " %50[^\n]", ranking.nome);
    fscanf(arquivo, " %c", &nivel_letra);

    if (nivel_letra == 'F')
    {
        ranking.nivel = 1;
        info.tam_seq = 4;
        info.max_tentativas = 10;
    }
    else if (nivel_letra == 'M')
    {
        ranking.nivel = 2;
        info.tam_seq = 5;
        info.max_tentativas = 12;
    }
    else
    {
        ranking.nivel = 3;
        info.tam_seq = 6;
        info.max_tentativas = 15;
    }
    info.cores = (int *)malloc(info.tam_seq * sizeof(int));

    for (int i = 0; i < info.tam_seq; i++)
    {
        fscanf(arquivo, "%d", &info.cores[i]);
    }

    int tentativas_salvas;
    fscanf(arquivo, "%d", &tentativas_salvas);

    int **historico = (int **)calloc(info.max_tentativas, sizeof(int *));
    char **historico_res = (char **)calloc(info.max_tentativas, sizeof(char *));
    for (int i = 0; i < info.max_tentativas; i++)
    {
        historico[i] = (int *)calloc(info.tam_seq, sizeof(int));
        historico_res[i] = (char *)calloc(info.tam_seq, sizeof(char));
    }

    for (int i = 0; i < tentativas_salvas; i++)
    {
        for (int j = 0; j < info.tam_seq; j++)
        {
            fscanf(arquivo, "%d", &historico[i][j]);
        }
        avaliarTentativa(historico[i], info.cores, historico_res[i], info.tam_seq);
    }

    fclose(arquivo);

    ranking.tentativas = tentativas_salvas + 1;

    printf("\nJogo de %s carregado com sucesso! Continuando de onde parou...\n", ranking.nome);
    printf("Cores disponíveis:\n1 - Vermelho\n2 - Azul\n3 - Verde\n4 - Amarelo\n5 - Roxo\n6 - Laranja\n");

    if (tentativas_salvas > 0)
    {
        printf("\nResultado das tentativas anteriores:\n");
        for (int r = 0; r < tentativas_salvas; r++)
        {
            printf("Rodada %d: ", r + 1);
            for (int e = 0; e < info.tam_seq; e++)
            {
                printf("%d ", historico[r][e]);
            }
            printf("(");
            for (int d = 0; d < info.tam_seq; d++)
            {
                printf(" %c ", historico_res[r][d]);
            }
            printf(")\n");
        }
    }

    bool voltar_menu = jogarPartida(&info, &ranking, historico, historico_res);

    if (voltar_menu)
    {
        guardarJogoPausado(jp, &info, &ranking, historico, historico_res);
        printf("\nVoltando ao menu...\n");
        return;
    }

    for (int i = 0; i < info.max_tentativas; i++)
    {
        free(historico[i]);
        free(historico_res[i]);
    }
    free(historico);
    free(historico_res);
    free(info.cores);
}

void mostrarRanking()
{
    FILE *arquivo = fopen("ranking.rnk", "rb");
    if (arquivo == NULL)
    {
        printf("Nenhum ranking registrado ainda! Seja o primeiro a vencer!\n");
        return;
    }

    Ranking lista[10];
    int qtd = fread(lista, sizeof(Ranking), 10, arquivo);
    fclose(arquivo);

    printf("\nRANKING TOP 10 \n");
    printf("%-5s | %-25s | %-10s | %-10s\n", "Pos", "Nome", "Nível", "Tentativas");
    printf("\n");
    
    for (int i = 0; i < qtd; i++)
    {
        char nivel_str[10];
        if (lista[i].nivel == 1) strcpy(nivel_str, "Fácil");
        else if (lista[i].nivel == 2) strcpy(nivel_str, "Médio");
        else strcpy(nivel_str, "Difícil");

        printf("%02dº   | %-25s | %-10s | %-10d\n", i + 1, lista[i].nome, nivel_str, lista[i].tentativas);
    }
    printf("\n");
}

void atualizarRanking(Ranking novo)
{
    Ranking lista[11];
    int qtd = 0;

    FILE *arquivo = fopen("ranking.rnk", "rb");
    if (arquivo != NULL)
    {
        qtd = fread(lista, sizeof(Ranking), 10, arquivo);
        fclose(arquivo);
    }

    lista[qtd] = novo;
    qtd++;

    for (int i = 0; i < qtd - 1; i++)
    {
        for (int j = 0; j < qtd - i - 1; j++)
        {
            bool trocar = false;
            
            if (lista[j].tentativas > lista[j + 1].tentativas)
            {
                trocar = true;
            }
            else if (lista[j].tentativas == lista[j + 1].tentativas)
            {
                
                if (lista[j].nivel < lista[j + 1].nivel)
                {
                    trocar = true;
                }
            }

            if (trocar)
            {
                Ranking temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }

    if (qtd > 10)
    {
        qtd = 10;
    }

    int posicao = -1;
    for (int i = 0; i < qtd; i++)
    {
        if (strcmp(lista[i].nome, novo.nome) == 0 && 
            lista[i].nivel == novo.nivel && 
            lista[i].tentativas == novo.tentativas)
        {
            posicao = i + 1; 
            break;
        }
    }


    printf("Você terminou a partida em %d tentativas.\n", novo.tentativas);
    
    if (posicao != -1)
    {
        printf("Excepcional! Você conquistou a posição %dº no ranking!\n", posicao);
    }
    else
    {
        printf("Infelizmente você não atingiu pontuação suficiente para entrar no TOP 10.\n");
    }

    arquivo = fopen("ranking.rnk", "wb");
    if (arquivo == NULL)
    {
        printf("Erro ao salvar o arquivo de ranking!\n");
        return;
    }
    fwrite(lista, sizeof(Ranking), qtd, arquivo);
    fclose(arquivo);
}