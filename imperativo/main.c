#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_DISCIPLINAS 100
#define MAX_ALUNOS 50
#define MAX_NOME 50
#define MAX_PRE_REQ 5
#define MAX_HISTORICO 20

// 1. Estruturas de Dados
typedef struct {
    char codigo[MAX_NOME];
    char nome[MAX_NOME];
    char pre_requisitos[MAX_PRE_REQ][MAX_NOME];
    int num_pre_requisitos;
} Disciplina;

typedef struct {
    int matricula;
    char nome[MAX_NOME];
    char historico[MAX_HISTORICO][MAX_NOME];
    int num_historico;
} Aluno;

static FILE *abrir_arquivo(const char *nome_arquivo, const char *modo) {
    char caminho_local[512];
    char caminho_saida[512];
    char caminho_pai[512];
    size_t tamanho_nome = strlen(nome_arquivo);

    if (tamanho_nome >= sizeof(caminho_local) - 8) {
        return NULL;
    }

    memcpy(caminho_local, nome_arquivo, tamanho_nome + 1);
    memcpy(caminho_saida, "output/", 7);
    memcpy(caminho_saida + 7, nome_arquivo, tamanho_nome + 1);

    if (3 + tamanho_nome >= sizeof(caminho_pai)) {
        caminho_pai[0] = '\0';
    } else {
        memcpy(caminho_pai, "../", 3);
        memcpy(caminho_pai + 3, nome_arquivo, tamanho_nome + 1);
    }

    FILE *arquivo = fopen(caminho_saida, modo);
    if (arquivo != NULL) return arquivo;

    arquivo = fopen(caminho_local, modo);
    if (arquivo != NULL) return arquivo;

    arquivo = fopen(caminho_pai, modo);
    if (arquivo != NULL) return arquivo;

    return NULL;
}

void limpar_buffer_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

bool ler_inte_seguro(int *valor) {
    if (scanf("%d", valor) != 1) {
        limpar_buffer_entrada();
        return false;
    }
    limpar_buffer_entrada();
    return true;
}

bool ler_texto_seguro(char *buffer, size_t tamanho) {
    if (tamanho == 0) {
        return false;
    }

    int resultado = scanf("%49s", buffer);
    if (resultado != 1) {
        limpar_buffer_entrada();
        return false;
    }

    limpar_buffer_entrada();
    return true;
}

// --- SUBPROGRAMAS DE ACESSO A ARQUIVOS (Efeitos Colaterais) ---

// Lê o arquivo grade.txt e carrega na memória
void carregar_grade(Disciplina grade[], int *num_grade) {
    FILE *arquivo = abrir_arquivo("grade.txt", "r");
    if (arquivo == NULL) {
        printf("Aviso: Nao foi possivel abrir grade.txt\n");
        *num_grade = 0;
        return;
    }
    
    *num_grade = 0;
    // Lê sequencialmente até encontrar o fim do arquivo (EOF)
    while (fscanf(arquivo, "%s %s %d", grade[*num_grade].codigo, grade[*num_grade].nome, &grade[*num_grade].num_pre_requisitos) != EOF) {
        for (int i = 0; i < grade[*num_grade].num_pre_requisitos; i++) {
            fscanf(arquivo, "%s", grade[*num_grade].pre_requisitos[i]);
        }
        (*num_grade)++;
    }
    fclose(arquivo);
}

// Lê o arquivo alunos.txt e carrega na memória
void carregar_alunos(Aluno banco_alunos[], int *num_alunos) {
    FILE *arquivo = abrir_arquivo("alunos.txt", "r");
    if (arquivo == NULL) {
        printf("Aviso: Nao foi possivel abrir alunos.txt\n");
        *num_alunos = 0;
        return;
    }
    
    *num_alunos = 0;
    while (fscanf(arquivo, "%d %s %d", &banco_alunos[*num_alunos].matricula, banco_alunos[*num_alunos].nome, &banco_alunos[*num_alunos].num_historico) != EOF) {
        for (int i = 0; i < banco_alunos[*num_alunos].num_historico; i++) {
            fscanf(arquivo, "%s", banco_alunos[*num_alunos].historico[i]);
        }
        (*num_alunos)++;
    }
    fclose(arquivo);
}

// Grava o resultado em um arquivo de texto formatado
void salvar_relatorio(char* aluno_nome, char* materia, char* status, char* motivo) {
    FILE *arquivo = abrir_arquivo("relatorio_matriculas.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "Aluno: %-15s | Materia: %-20s | Status: %-10s | Motivo: %s\n", 
                aluno_nome, materia, status, motivo);
        fclose(arquivo);
    }
}


// --- SUBPROGRAMAS DE LÓGICA IMPERATIVA ---

int encontrar_aluno(int matricula, Aluno banco_alunos[], int num_alunos) {
    for (int i = 0; i < num_alunos; i++) {
        if (banco_alunos[i].matricula == matricula) return i;
    }
    return -1;
}

bool ja_cursada(char* codigo, Aluno aluno) {
    for (int i = 0; i < aluno.num_historico; i++) {
        if (strcmp(aluno.historico[i], codigo) == 0) return true;
    }
    return false;
}

int encontrar_disciplina(char* codigo, Disciplina grade[], int num_grade) {
    for (int i = 0; i < num_grade; i++) {
        if (strcmp(grade[i].codigo, codigo) == 0) return i;
    }
    return -1;
}

bool disciplina_ja_existe_por_nome(const char *nome, Disciplina grade[], int num_grade) {
    for (int i = 0; i < num_grade; i++) {
        if (strcmp(grade[i].nome, nome) == 0) return true;
    }
    return false;
}

bool aluno_ja_existe_por_nome(const char *nome, Aluno banco_alunos[], int num_alunos) {
    for (int i = 0; i < num_alunos; i++) {
        if (strcmp(banco_alunos[i].nome, nome) == 0) return true;
    }
    return false;
}

void listar_materias(Disciplina grade[], int num_grade) {
    printf("\n=== MATERIAS DISPONIVEIS PARA MATRICULA ===\n");
    for (int i = 0; i < num_grade; i++) {
        printf("Codigo: %-6s | Nome: %s\n", grade[i].codigo, grade[i].nome);
    }
    printf("===========================================\n\n");
}

void exibir_historico_aluno(Aluno aluno) {
    printf("\n=== HISTORICO DE %s ===\n", aluno.nome);
    if (aluno.num_historico == 0) {
        printf("Nenhuma disciplina cursada ate o momento.\n");
    } else {
        for (int i = 0; i < aluno.num_historico; i++) {
            printf("%d. %s\n", i + 1, aluno.historico[i]);
        }
    }
    printf("========================\n");
}

void salvar_grade(Disciplina grade[], int num_grade) {
    FILE *arquivo = abrir_arquivo("grade.txt", "w");
    if (arquivo == NULL) {
        printf("Aviso: Nao foi possivel salvar grade.txt\n");
        return;
    }

    for (int i = 0; i < num_grade; i++) {
        fprintf(arquivo, "%s %s %d", grade[i].codigo, grade[i].nome, grade[i].num_pre_requisitos);
        for (int j = 0; j < grade[i].num_pre_requisitos; j++) {
            fprintf(arquivo, " %s", grade[i].pre_requisitos[j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}

void salvar_alunos(Aluno banco_alunos[], int num_alunos) {
    FILE *arquivo = abrir_arquivo("alunos.txt", "w");
    if (arquivo == NULL) {
        printf("Aviso: Nao foi possivel salvar alunos.txt\n");
        return;
    }

    for (int i = 0; i < num_alunos; i++) {
        fprintf(arquivo, "%d %s %d", banco_alunos[i].matricula, banco_alunos[i].nome, banco_alunos[i].num_historico);
        for (int j = 0; j < banco_alunos[i].num_historico; j++) {
            fprintf(arquivo, " %s", banco_alunos[i].historico[j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}

void cadastrar_nova_disciplina(Disciplina grade[], int *num_grade) {
    Disciplina nova_disciplina;

    if (*num_grade >= MAX_DISCIPLINAS) {
        printf("Erro: limite de disciplinas atingido.\n");
        return;
    }

    printf("Codigo da disciplina: ");
    if (!ler_texto_seguro(nova_disciplina.codigo, sizeof(nova_disciplina.codigo))) {
        printf("Entrada invalida para codigo da disciplina.\n");
        return;
    }

    if (encontrar_disciplina(nova_disciplina.codigo, grade, *num_grade) != -1) {
        printf("Erro: disciplina ja cadastrada.\n");
        return;
    }

    printf("Nome da disciplina: ");
    if (!ler_texto_seguro(nova_disciplina.nome, sizeof(nova_disciplina.nome))) {
        printf("Entrada invalida para nome da disciplina.\n");
        return;
    }

    if (disciplina_ja_existe_por_nome(nova_disciplina.nome, grade, *num_grade)) {
        printf("Erro: ja existe uma disciplina com esse nome.\n");
        return;
    }

    printf("Quantidade de pre-requisitos: ");
    int qtd_pre_requisitos = 0;
    if (!ler_inte_seguro(&qtd_pre_requisitos)) {
        printf("Entrada invalida para quantidade de pre-requisitos.\n");
        return;
    }
    nova_disciplina.num_pre_requisitos = qtd_pre_requisitos;

    if (nova_disciplina.num_pre_requisitos < 0 || nova_disciplina.num_pre_requisitos > MAX_PRE_REQ) {
        printf("Erro: numero de pre-requisitos invalido. Use entre 0 e %d.\n", MAX_PRE_REQ);
        nova_disciplina.num_pre_requisitos = 0;
    }

    for (int i = 0; i < nova_disciplina.num_pre_requisitos; i++) {
        printf("Pre-requisito %d: ", i + 1);
        if (!ler_texto_seguro(nova_disciplina.pre_requisitos[i], sizeof(nova_disciplina.pre_requisitos[i]))) {
            printf("Entrada invalida para pre-requisito.\n");
            nova_disciplina.pre_requisitos[i][0] = '\0';
        }
    }

    grade[*num_grade] = nova_disciplina;
    (*num_grade)++;
    salvar_grade(grade, *num_grade);

    printf("Disciplina cadastrada com sucesso!\n");
    listar_materias(grade, *num_grade);
}

void cadastrar_novo_aluno(Aluno banco_alunos[], int *num_alunos) {
    Aluno novo_aluno;

    if (*num_alunos >= MAX_ALUNOS) {
        printf("Erro: limite de alunos atingido.\n");
        return;
    }

    printf("Matricula do aluno: ");
    if (!ler_inte_seguro(&novo_aluno.matricula)) {
        printf("Entrada invalida para matricula.\n");
        return;
    }

    if (encontrar_aluno(novo_aluno.matricula, banco_alunos, *num_alunos) != -1) {
        printf("Erro: ja existe um aluno com essa matricula.\n");
        return;
    }

    printf("Nome do aluno: ");
    if (!ler_texto_seguro(novo_aluno.nome, sizeof(novo_aluno.nome))) {
        printf("Entrada invalida para nome do aluno.\n");
        return;
    }

    if (aluno_ja_existe_por_nome(novo_aluno.nome, banco_alunos, *num_alunos)) {
        printf("Erro: ja existe um aluno com esse nome.\n");
        return;
    }

    novo_aluno.num_historico = 0;

    banco_alunos[*num_alunos] = novo_aluno;
    (*num_alunos)++;
    salvar_alunos(banco_alunos, *num_alunos);

    printf("Aluno cadastrado com sucesso!\n");
}

int fazer_login_aluno(Aluno banco_alunos[], int num_alunos) {
    int matricula_busca;

    while (1) {
        printf("Digite o numero de matricula do aluno: ");
        if (!ler_inte_seguro(&matricula_busca)) {
            printf("Entrada invalida. Digite apenas a matricula numerica.\n");
            continue;
        }

        int indice_aluno = encontrar_aluno(matricula_busca, banco_alunos, num_alunos);
        if (indice_aluno == -1) {
            printf("Erro: Aluno nao encontrado.\n");
            return -1;
        }

        printf("Bem-vindo(a), %s!\n", banco_alunos[indice_aluno].nome);
        exibir_historico_aluno(banco_alunos[indice_aluno]);
        return indice_aluno;
    }
}

void processar_matricula_aluno(Disciplina grade[], int num_grade, Aluno banco_alunos[], int num_alunos, int indice_aluno) {
    Aluno *aluno_atual = &banco_alunos[indice_aluno];
    char codigo_solicitado[MAX_NOME];

    listar_materias(grade, num_grade);
    printf("Digite o CODIGO da materia: ");
    if (!ler_texto_seguro(codigo_solicitado, sizeof(codigo_solicitado))) {
        printf("Entrada invalida. Digite um codigo de disciplina.\n");
        return;
    }

    printf("\n--- PROCESSANDO SOLICITACAO ---\n");

    if (ja_cursada(codigo_solicitado, *aluno_atual)) {
        printf("[INDEFERIDA] Motivo: Disciplina ja cursada.\n");
        salvar_relatorio(aluno_atual->nome, codigo_solicitado, "INDEFERIDA", "Ja cursada");
        return;
    }

    int indice_grade = encontrar_disciplina(codigo_solicitado, grade, num_grade);
    if (indice_grade == -1) {
        printf("[INDEFERIDA] Motivo: Disciplina inexistente na grade.\n");
        salvar_relatorio(aluno_atual->nome, codigo_solicitado, "INDEFERIDA", "Inexistente");
        return;
    }

    bool cumpre_requisitos = true;
    Disciplina disc = grade[indice_grade];

    for (int j = 0; j < disc.num_pre_requisitos; j++) {
        if (!ja_cursada(disc.pre_requisitos[j], *aluno_atual)) {
            char motivo[100];
            sprintf(motivo, "Falta o pre-requisito '%s'", disc.pre_requisitos[j]);

            printf("[INDEFERIDA] Motivo: %s.\n", motivo);
            salvar_relatorio(aluno_atual->nome, disc.nome, "INDEFERIDA", motivo);

            cumpre_requisitos = false;
            break;
        }
    }

    if (cumpre_requisitos) {
        strcpy(aluno_atual->historico[aluno_atual->num_historico], disc.codigo);
        aluno_atual->num_historico++;
        salvar_alunos(banco_alunos, num_alunos);

        printf("[DEFERIDA] Matricula liberada!\n");
        salvar_relatorio(aluno_atual->nome, disc.nome, "DEFERIDA", "Requisitos cumpridos");
    }
}

void menu_sessao_aluno(Disciplina grade[], int num_grade, Aluno banco_alunos[], int num_alunos, int indice_aluno) {
    int opcao_sessao = -1;

    while (opcao_sessao != 0 && opcao_sessao != 2) {
        printf("\n=== OPCOES DO ALUNO %s ===\n", banco_alunos[indice_aluno].nome);
        printf("1 - Registrar outra materia\n");
        printf("2 - Trocar de conta\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (!ler_inte_seguro(&opcao_sessao)) {
            printf("Opcao invalida. Digite apenas um numero.\n");
            opcao_sessao = -1;
            continue;
        }

        if (opcao_sessao == 1) {
            processar_matricula_aluno(grade, num_grade, banco_alunos, num_alunos, indice_aluno);
            exibir_historico_aluno(banco_alunos[indice_aluno]);
        } else if (opcao_sessao == 2) {
            printf("Conta trocada com sucesso.\n");
            return;
        } else if (opcao_sessao == 0) {
            printf("Sistema encerrado. Verifique o arquivo 'relatorio_matriculas.txt'.\n");
            exit(0);
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

int main(int argc, char *argv[]) {
    Disciplina grade[MAX_DISCIPLINAS];
    Aluno banco_alunos[MAX_ALUNOS];
    int total_disciplinas = 0;
    int total_alunos = 0;
    int indice_aluno_logado = -1;

    (void)argc;
    (void)argv;

    carregar_grade(grade, &total_disciplinas);
    carregar_alunos(banco_alunos, &total_alunos);

    int opcao = -1;
    while (opcao != 0) {
        if (indice_aluno_logado != -1) {
            menu_sessao_aluno(grade, total_disciplinas, banco_alunos, total_alunos, indice_aluno_logado);
            indice_aluno_logado = -1;
            continue;
        }

        printf("\n--- SISTEMA DE MATRICULAS ---\n");
        printf("1 - Login de aluno\n");
        printf("2 - Cadastrar novo aluno\n");
        printf("3 - Cadastrar nova disciplina\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (!ler_inte_seguro(&opcao)) {
            printf("Opcao invalida. Digite apenas um numero.\n");
            opcao = -1;
            continue;
        }

        if (opcao == 1) {
            indice_aluno_logado = fazer_login_aluno(banco_alunos, total_alunos);
            if (indice_aluno_logado == -1) {
                indice_aluno_logado = -1;
            }
        } else if (opcao == 2) {
            cadastrar_novo_aluno(banco_alunos, &total_alunos);
        } else if (opcao == 3) {
            cadastrar_nova_disciplina(grade, &total_disciplinas);
        } else if (opcao == 0) {
            printf("Sistema encerrado. Verifique o arquivo 'relatorio_matriculas.txt'.\n");
            return 0;
        } else {
            printf("Opcao invalida.\n");
        }
    }

    printf("Sistema encerrado. Verifique o arquivo 'relatorio_matriculas.txt'.\n");
    return 0;
}
