# Decisões de Implementação: Paradigma Imperativo

**Tag Obrigatória:** `[P4-ETAPA-03]`
**Linguagem Escolhida:** C

A implementação deste validador de matrículas foi projetada como uma abstração direta da arquitetura de computadores de Von Neumann[cite: 13]. O programa atua com base no conceito de que o mundo computacional é uma série de estados contínuos e bem definidos, onde programar significa emitir comandos sequenciais explícitos para alterar esse estado[cite: 13]. Abaixo, detalho as respostas exigidas pela especificação da Etapa 3.

### 1. Quais estados são mantidos
No paradigma imperativo, o "estado" de um programa é a fotografia de todos os valores em suas variáveis em um dado momento, atuando como abstrações nomeadas para células de memória física[cite: 13]. 
*   **O que é mantido:** O sistema mantém o estado estrutural do **Banco de Disciplinas** (um vetor de `structs` contendo a grade e os pré-requisitos) e do **Banco de Alunos** (um vetor contendo as matrículas e o histórico de cada estudante). 
*   **Alocação:** Esses vetores são alocados estaticamente no escopo da função `main`, garantindo que ocupem blocos definidos na memória Stack[cite: 16]. 
*   **Controle:** Também são mantidos estados de controle temporários que regem o ciclo de vida do programa, como a variável de sessão `indice_aluno_logado`, índices de iteração (`i`, `j`), e sinalizadores booleanos (`cumpre_requisitos`). Manter esses dados explícitos permite rastrear e alterar as informações passo a passo durante a execução[cite: 13].

### 2. Quais operações modificam estado
A base estrutural da programação imperativa é a sentença de atribuição, que é o mecanismo que roteia os resultados calculados pela Unidade Lógica e Aritmética (ALU) de volta para a memória RAM[cite: 13]. A mutabilidade é a regra de ouro: o valor de uma variável é atualizado e sobrescrito continuamente[cite: 13].
*   O estado é ativamente modificado através do **operador de atribuição** `=` (por exemplo, `nova_disciplina.num_pre_requisitos = qtd_pre_requisitos`).
*   **Operações de incremento**, como `(*num_grade)++` e `(*num_alunos)++`, alteram o estado dos contadores de tamanho lógico que guiam os limites das estruturas de dados.
*   **Manipulação de memória:** Funções de cópia (`strcpy`) e captura de entradas (`scanf`) modificam os blocos de *bytes* das matrizes de caracteres (histórico do aluno). O ciclo imperativo clássico é eterno ao ler o estado, calcular um novo valor e atualizar o estado[cite: 13].

### 3. Onde aparecem efeitos colaterais
Um efeito colateral ocorre quando uma função modifica uma variável global ou interage com o ambiente externo, afetando o estado fora de seu escopo local[cite: 14]. Em C, interagir com dispositivos de Entrada e Saída (I/O) é a principal fonte de efeitos colaterais.
*   **Persistência em Disco:** As funções `carregar_grade`, `salvar_alunos` e `salvar_relatorio` geram efeitos colaterais massivos interagindo com o disco rígido, alterando o ambiente externo ao sobrescrever arquivos `.txt` dinamicamente no diretório `output/`.
*   **Sanitização de Buffer (I/O):** A própria interação com o usuário gera efeitos colaterais no buffer do sistema operacional. O subprograma `limpar_buffer_entrada` lida diretamente com o dispositivo *stdin* executando a varredura e o esvaziamento físico do buffer por meio do comando `getchar()`. Isso garante a confiabilidade do programa, prevenindo travamentos indesejados (*crashes*) na leitura de dados caso os tipos sejam inseridos incorretamente[cite: 14].
*   **Interface:** A impressão de menus e relatórios via `printf` altera o estado do dispositivo de saída padrão.

### 4. Quais estruturas de controle foram escolhidas
O controle de fluxo manipula indiretamente o *Program Counter* do processador, roteando a execução instrução por instrução de forma clara e previsível[cite: 13, 15].
*   **Iteração (Laços `for` e `while`):** A iteração é a solução imperativa nativa para processar blocos sequenciais de memória[cite: 13]. O laço `for` varre as matrizes do histórico e disciplinas, enquanto laços infinitos lógicos `while(1)` controlam o ciclo de vida dos menus.
*   **Seleção (`if` e `else`):** Desvios condicionais alteram a sequência de execução avaliando o estado atual do sistema[cite: 13].
*   **Desvios Restritos (`break` e `continue`):** Para fugir das antigas limitações de legibilidade do comando `goto` (o infame código espaguete)[cite: 15], a solução utiliza a instrução de saída `break` para saídas prematuras de laços de pré-requisitos, e o desvio `continue`, que transfere o controle para o mecanismo do laço que o envolve (pulando a iteração em falhas de validação)[cite: 14].

### 5. Como os subprogramas foram organizados
Subprogramas (funções) representam computações parametrizadas e necessitam de espaços estritos para viverem[cite: 14, 16].
*   **Modularidade:** A arquitetura foi fragmentada em funções procedurais autônomas (ex: `processar_matricula_aluno`, `encontrar_disciplina`).
*   **Gestão de Estado Sem Globais:** Para evitar a redução de confiabilidade atrelada ao uso intenso de variáveis globais[cite: 14], o estado fundamental dos bancos de dados foi declarado no `main` e transferido via ponteiros explícitos.
*   **Registro de Ativação:** Toda vez que um subprograma é chamado, uma instância de **Registro de Ativação** é criada no topo da pilha (Stack)[cite: 16]. Este bloco guardará de forma isolada as variáveis locais (como variáveis temporárias), os parâmetros recebidos e o endereço de retorno para orientar o *Program Counter*, sendo destruído no final da execução da função[cite: 13, 16].

### 6. Por que a solução pode ser considerada predominantemente imperativa
A solução é imperativa porque o foco analítico da programação está no **"COMO"** a máquina deve realizar a tarefa, passo a passo[cite: 13]. A execução do programa é inteiramente conduzida pela mudança constante dos valores armazenados[cite: 13].
*   Diferente do paradigma Orientado a Objetos, os tipos de dados e funções estão separados (não existe encapsulamento isolando regras de negócio em instâncias). As linguagens como C tradicional operam baseadas em registros (`structs`) repassados para rotinas independentes[cite: 13].
*   Diferente da programação Funcional e Lógica (focadas no **"O QUÊ"** fazer)[cite: 13], o programa em C não trabalha com regras declarativas, fechamentos abstratos de ambiente ou transparência referencial onde variáveis são imutáveis[cite: 14]. O algoritmo consiste essencialmente em gerenciar laços de índices e navegar em memória RAM arranjo por arranjo de forma explícita e manual para resolver as validações do grafo de dependências das disciplinas, limitando-se ao fluxo de execução da máquina física Von Neumann subjacente[cite: 13].
