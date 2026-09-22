# Decisões de Implementação: Paradigma Imperativo

**Tag Obrigatória:** `[P4-ETAPA-03]`
**Linguagem Escolhida:** C

A implementação deste validador de matrículas foi projetada como uma abstração direta da arquitetura de computadores de Von Neumann. O programa atua com base no conceito de que o mundo computacional é uma série de estados contínuos e bem definidos, onde programar significa emitir comandos sequenciais explícitos para alterar esse estado. Abaixo, detalho as respostas exigidas pela especificação da Etapa 3.

### 1. Quais estados são mantidos
No paradigma imperativo, o "estado" de um programa é a fotografia de todos os valores em suas variáveis em um dado momento, atuando como abstrações nomeadas para células de memória física. 
*   **O que é mantido:** O sistema mantém o estado estrutural do **Banco de Disciplinas** (um vetor de `structs` contendo a grade e os pré-requisitos) e do **Banco de Alunos** (um vetor contendo as matrículas e o histórico de cada estudante). 
*   **Alocação:** Esses vetores são alocados estaticamente no escopo da função `main`, garantindo que ocupem blocos definidos na memória Stack. 
*   **Controle:** Também são mantidos estados de controle temporários que regem o ciclo de vida do programa, como a variável de sessão `indice_aluno_logado`, índices de iteração (`i`, `j`), e sinalizadores booleanos (`cumpre_requisitos`). Manter esses dados explícitos permite rastrear e alterar as informações passo a passo durante a execução.

### 2. Quais operações modificam estado
A base estrutural da programação imperativa é a sentença de atribuição, que é o mecanismo que roteia os resultados calculados pela Unidade Lógica e Aritmética (ALU) de volta para a memória RAM. A mutabilidade é a regra de ouro: o valor de uma variável é atualizado e sobrescrito continuamente.
*   O estado é ativamente modificado através do **operador de atribuição** `=` (por exemplo, `nova_disciplina.num_pre_requisitos = qtd_pre_requisitos`).
*   **Operações de incremento**, como `(*num_grade)++` e `(*num_alunos)++`, alteram o estado dos contadores de tamanho lógico que guiam os limites das estruturas de dados.
*   **Manipulação de memória:** Funções de cópia (`strcpy`) e captura de entradas (`scanf`) modificam os blocos de *bytes* das matrizes de caracteres (histórico do aluno). O ciclo imperativo clássico é eterno ao ler o estado, calcular um novo valor e atualizar o estado.

### 3. Onde aparecem efeitos colaterais
Um efeito colateral ocorre quando uma função modifica uma variável global ou interage com o ambiente externo, afetando o estado fora de seu escopo local. Em C, interagir com dispositivos de Entrada e Saída (I/O) é a principal fonte de efeitos colaterais.
*   **Persistência em Disco:** As funções `carregar_grade`, `salvar_alunos` e `salvar_relatorio` geram efeitos colaterais massivos interagindo com o disco rígido, alterando o ambiente externo ao sobrescrever arquivos `.txt` dinamicamente no diretório `output/`.
*   **Sanitização de Buffer (I/O):** A própria interação com o usuário gera efeitos colaterais no buffer do sistema operacional. O subprograma `limpar_buffer_entrada` lida diretamente com o dispositivo *stdin* executando a varredura e o esvaziamento físico do buffer por meio do comando `getchar()`. Isso garante a confiabilidade do programa, prevenindo travamentos indesejados (*crashes*) na leitura de dados caso os tipos sejam inseridos incorretamente.
*   **Interface:** A impressão de menus e relatórios via `printf` altera o estado do dispositivo de saída padrão.

### 4. Quais estruturas de controle foram escolhidas
O controle de fluxo manipula indiretamente o *Program Counter* do processador, roteando a execução instrução por instrução de forma clara e previsível.
*   **Iteração (Laços `for` e `while`):** A iteração é a solução imperativa nativa para processar blocos sequenciais de memória. O laço `for` varre as matrizes do histórico e disciplinas, enquanto laços infinitos lógicos `while(1)` controlam o ciclo de vida dos menus.
*   **Seleção (`if` e `else`):** Desvios condicionais alteram a sequência de execução avaliando o estado atual do sistema.
*   **Desvios Restritos (`break` e `continue`):** Para fugir das antigas limitações de legibilidade do comando `goto` (o infame código espaguete), a solução utiliza a instrução de saída `break` para saídas prematuras de laços de pré-requisitos, e o desvio `continue`, que transfere o controle para o mecanismo do laço que o envolve (pulando a iteração em falhas de validação).

### 5. Como os subprogramas foram organizados
Subprogramas (funções) representam computações parametrizadas e necessitam de espaços estritos para viverem.
*   **Modularidade:** A arquitetura foi fragmentada em funções procedurais autônomas (ex: `processar_matricula_aluno`, `encontrar_disciplina`).
*   **Gestão de Estado Sem Globais:** Para evitar a redução de confiabilidade atrelada ao uso intenso de variáveis globais, o estado fundamental dos bancos de dados foi declarado no `main` e transferido via ponteiros explícitos.
*   **Registro de Ativação:** Toda vez que um subprograma é chamado, uma instância de **Registro de Ativação** é criada no topo da pilha (Stack). Este bloco guardará de forma isolada as variáveis locais (como variáveis temporárias), os parâmetros recebidos e o endereço de retorno para orientar o *Program Counter*, sendo destruído no final da execução da função.

### 6. Por que a solução pode ser considerada predominantemente imperativa
A solução é imperativa porque o foco analítico da programação está no **"COMO"** a máquina deve realizar a tarefa, passo a passo. A execução do programa é inteiramente conduzida pela mudança constante dos valores armazenados.
*   Diferente do paradigma Orientado a Objetos, os tipos de dados e funções estão separados (não existe encapsulamento isolando regras de negócio em instâncias). As linguagens como C tradicional operam baseadas em registros (`structs`) repassados para rotinas independentes.
*   Diferente da programação Funcional e Lógica (focadas no **"O QUÊ"** fazer), o programa em C não trabalha com regras declarativas, fechamentos abstratos de ambiente ou transparência referencial onde variáveis são imutáveis. O algoritmo consiste essencialmente em gerenciar laços de índices e navegar em memória RAM arranjo por arranjo de forma explícita e manual para resolver as validações do grafo de dependências das disciplinas, limitando-se ao fluxo de execução da máquina física Von Neumann subjacente.

### 7. Validação dos Casos da Etapa 2
Os casos de teste definidos na Etapa 2 serviram como referência analítica para validar o comportamento computacional do validador de matrícula implementado.

#### 7.1 Casos Normais
*   **NORM-01 (Disciplina sem pré-requisito):** Atendido. A função `processar_matricula_aluno` localiza a disciplina na grade e valida seus pré-requisitos. Como a quantidade de pré-requisitos é zero, o sinalizador `cumpre_requisitos` permanece verdadeiro e a matrícula é deferida com sucesso.
*   **NORM-02 (Pré-requisito cumprido):** Atendido. Para cada dependência cadastrada, o programa utiliza `ja_cursada()` iterativamente na matriz do histórico do aluno, confirmando a presença e liberando a matrícula.
*   **NORM-03 (Pré-requisito não cumprido):** Atendido. O sistema interrompe o fluxo por meio do comando `break` ao não encontrar o requisito, disparando a rotina de indeferimento e gravando a ocorrência no relatório em disco.
*   **NORM-04 (Disciplina já cursada):** Atendido. A validação de `ja_cursada()` executa prioritariamente logo na entrada da rotina, bloqueando requisições duplicadas de histórico.
*   **NORM-05 (Múltiplos pré-requisitos cumpridos):** Atendido. O laço `for` varre todas as dependências da grade; se qualquer uma falhar, o processo é abortado imediatamente.
*   **NORM-06, NORM-07 e NORM-09 (Solicitações múltiplas/mistas):** Parcialmente atendidos de forma conceitual. O motor do programa valida cada disciplina com total exatidão, mas a interface imperativa de console foi estruturada para processar uma solicitação por vez em ciclos iterativos de menu, e não listas em lote simultâneo.
*   **NORM-08 e NORM-10 (Histórico não relacionado e salto de cadeia):** Atendidos. A busca mapeia estritamente os pré-requisitos diretos da disciplina corrente, rejeitando tentativas de burlar a hierarquia do grafo.

#### 7.2 Casos-Limite
*   **LIM-01 (Solicitação duplicada):** Parcialmente atendido. O programa barra se a matéria já estiver consolidada no histórico, mas não gerencia uma lista temporária isolada de requisições do semestre corrente antes da efetivação.
*   **LIM-02 e LIM-03 (Parcialidade de requisitos e Código inexistente):** Atendidos. A lógica booleana interrompe o fluxo na ausência de parte dos requisitos, e a função `encontrar_disciplina()` retorna `-1` de forma segura ao não casar o código digitado com a base de dados.

#### 7.3 Casos de Entrada Inválida
*   **INV-01 (Grade indisponível):** Parcialmente atendido. A função `carregar_grade()` testa o ponteiro do arquivo e trata a falha definindo o total de disciplinas como zero, embora não exiba exatamente a string textual especificada no contrato semântico.
*   **INV-02 (Histórico em formato inválido):** Não se aplica diretamente à execução via terminal, pois a robustez da leitura estruturada via `fscanf` aliada à sanitização do *buffer* impede a injeção de tipos escalares incorretos em tempo de execução.
