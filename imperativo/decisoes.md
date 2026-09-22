# Decisões de Implementação: Paradigma Imperativo

**Tag Obrigatória:** `[P4-ETAPA-03]`
**Linguagem Escolhida:** C

A implementação deste validador de matrículas foi projetada como uma abstração direta da arquitetura de computadores de Von Neumann, focando em comandos sequenciais explícitos para alterar o estado do programa. Abaixo, detalhamos as respostas exigidas pela especificação da Etapa 3.

### 1. Quais estados são mantidos
No paradigma imperativo, o "estado" de um programa é a fotografia de todos os valores em suas variáveis em um dado momento. As variáveis atuam como abstrações nomeadas para células de memória física.
*   Mantemos o estado estrutural do **Banco de Disciplinas** (um vetor de `structs` contendo a grade e os pré-requisitos) e do **Banco de Alunos** (um vetor contendo as matrículas e o histórico de cada estudante).
*   Esses vetores são alocados estaticamente no escopo da função `main`, garantindo que ocupem blocos definidos de memória (Stack).
*   Também são mantidos estados de controle temporários, como a variável indicadora do `indice_aluno_logado`, índices de iteração (`i`, `j`), e sinalizadores booleanos (`cumpre_requisitos`).

### 2. Quais operações modificam estado
O coração do paradigma imperativo é a atribuição, que é o mecanismo que roteia os resultados calculados de volta para a memória RAM. A mutabilidade é a regra de ouro.
*   O estado é continuamente modificado através do operador de atribuição `=` (por exemplo, `nova_disciplina.num_pre_requisitos = qtd_pre_requisitos`).
*   Operações de incremento, como `(*num_grade)++` e `(*num_alunos)++`, modificam ativamente os contadores de tamanho lógico dos bancos de dados.
*   Funções de cópia de memória, como `strcpy` (ou o uso do `scanf`), modificam o conteúdo das matrizes de caracteres que compõem os códigos e nomes das disciplinas no histórico do aluno.

### 3. Onde aparecem efeitos colaterais
Um efeito colateral ocorre quando um subprograma modifica uma variável global ou interage com o ambiente externo, afetando o estado fora de seu escopo local.
*   Os efeitos colaterais aparecem predominantemente nas **operações de I/O (Entrada e Saída)**. As funções `carregar_grade`, `salvar_alunos` e `salvar_relatorio` geram efeitos colaterais massivos ao interagirem com o disco rígido, lendo e sobrescrevendo arquivos `.txt` de forma dinâmica no diretório `output/`.
*   A própria interação com o usuário gera efeitos colaterais no buffer do sistema operacional. O subprograma `limpar_buffer_entrada` lida diretamente com o *stdin* executando a varredura e o esvaziamento físico do buffer por meio do comando `getchar()`, prevenindo travamentos indesejados (*crashes*) na leitura de dados.
*   A impressão de relatórios na tela utilizando a função `printf` também é considerada um efeito colateral, alterando o estado do dispositivo de saída padrão.

### 4. Quais estruturas de controle foram escolhidas
O controle de fluxo manipula indiretamente o *Program Counter* do processador, roteando a execução instrução por instrução.
*   **Laços de Repetição (`for` e `while`):** Escolhidos para a varredura sequencial de blocos de memória (matrizes de disciplinas e históricos). O `for` lida com as repetições onde o limite é previamente conhecido, enquanto laços infinitos lógicos `while(1)` controlam os menus de interação do usuário até encontrarem uma condição de saída.
*   **Desvios Condicionais (`if` e `else`):** Utilizados para avaliar o estado atual das variáveis e alterar a sequência de execução.
*   **Saídas Prematuras (`break` e `continue`, `return`):** O comando `break` foi usado para interrupções imediatas de laços (ex: ao identificar que um pré-requisito falhou), e o `continue` para pular iterações onde entradas inválidas ocorreram. Essa abordagem supre a necessidade de desvios incondicionais perigosos (como o `goto`), resultando em um controle explícito sem perder a legibilidade.

### 5. Como os subprogramas foram organizados
Subprogramas (funções e procedimentos) representam computações parametrizadas.
*   A solução foi fragmentada em subprogramas procedurais modulares (ex: `processar_matricula_aluno`, `encontrar_disciplina`).
*   Para minimizar o acesso a variáveis globais (que reduz a confiabilidade), o estado principal foi armazenado no `main` e passado por referência explícita (ponteiros) para os subprogramas.
*   Toda vez que uma dessas funções é chamada, uma instância de registro de ativação é empilhada na *Stack* da memória, isolando parâmetros e variáveis temporárias. Como a linguagem C usa passagem por valor, enviar os arrays de bancos de dados via ponteiros permite que a função atue diretamente no endereço de memória original, implementando assim os modos de entrada e saída esperados.

### 6. Por que a solução pode ser considerada predominantemente imperativa
O programa é considerado imperativo por focar analiticamente no **"COMO"** a máquina deve realizar a tarefa, instrução por instrução.
*   Diferente do paradigma Orientado a Objetos, não há encapsulamento em objetos escondendo a mutação de estado; as variáveis e funções estão separadas, operando de forma procedimental clássica (estruturas `struct` processadas por subprogramas autônomos).
*   Diferente dos paradigmas Funcional ou Lógico (onde foca-se no **"O QUÊ"** fazer), este programa não utiliza transparência referencial e regras declarativas imutáveis. O algoritmo depende inteiramente de ditar a ordem temporal de execução: ler o estado, calcular as condições na Unidade Lógica e Aritmética (ALU) e atualizar o estado constantemente na memória.