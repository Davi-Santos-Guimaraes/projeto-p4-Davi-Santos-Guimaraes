# Casos de Teste — Validador de Matrícula

**Descrição:** Conjunto de testes independentes de paradigma para avaliar o deferimento ou indeferimento de solicitações de matrícula.
**Entradas Base:** Grade Curricular (dicionário/lista de disciplinas e pré-requisitos), Histórico do Aluno (lista de disciplinas concluídas) e Solicitação de Matrícula (lista de disciplinas desejadas).
**Saídas Base:** Lista de Disciplinas Deferidas e Lista de Disciplinas Indeferidas com o motivo exato.

## Casos Normais (10 casos)

*   **Identificador:** NORM-01
    *   **Entrada:** Histórico vazio; Solicita: "Algoritmos 1" (sem pré-requisito).
    *   **Saída esperada:** "Algoritmos 1" Deferida.
    *   **Descrição:** Disciplinas que não possuem pré-requisitos na grade curricular são deferidas automaticamente se não cursadas.

*   **Identificador:** NORM-02
    *   **Entrada:** Histórico ("Algoritmos 1"); Solicita: "Algoritmos 2" (Pré-requisito: "Algoritmos 1").
    *   **Saída esperada:** "Algoritmos 2" Deferida.
    *   **Descrição:** Aluno possui em seu histórico de aprovação todos os pré-requisitos exigidos pela disciplina.

*   **Identificador:** NORM-03
    *   **Entrada:** Histórico vazio; Solicita: "Algoritmos 2" (Pré-requisito: "Algoritmos 1").
    *   **Saída esperada:** "Algoritmos 2" Indeferida (Falta: "Algoritmos 1").
    *   **Descrição:** Bloqueio de matrícula por falta do pré-requisito exigido.

*   **Identificador:** NORM-04
    *   **Entrada:** Histórico ("Cálculo 1"); Solicita: "Cálculo 1".
    *   **Saída esperada:** "Cálculo 1" Indeferida (Já cursada).
    *   **Descrição:** Se uma disciplina solicitada já constar no histórico de concluídas, ela deve ser obrigatoriamente indeferida.

*   **Identificador:** NORM-05
    *   **Entrada:** Histórico ("Algoritmos 1", "Matemática Discreta"); Solicita: "Estruturas de Dados" (Pré-requisitos: "Algoritmos 1" e "Matemática Discreta").
    *   **Saída esperada:** "Estruturas de Dados" Deferida.
    *   **Descrição:** Solicitação de disciplina com múltiplos pré-requisitos onde todos foram concluídos com sucesso.

*   **Identificador:** NORM-06
    *   **Entrada:** Histórico vazio; Solicita: "Sociologia", "Filosofia" (Ambas sem pré-requisitos).
    *   **Saída esperada:** "Sociologia" Deferida, "Filosofia" Deferida.
    *   **Descrição:** Múltiplas solicitações simultâneas de matérias independentes sem dependências.

*   **Identificador:** NORM-07
    *   **Entrada:** Histórico ("Física 1"); Solicita: "Física 2" (Pré-req: "Física 1"), "Física 1".
    *   **Saída esperada:** "Física 2" Deferida; "Física 1" Indeferida (Já cursada).
    *   **Descrição:** Lote de solicitações mistas, gerando tanto relatórios de deferimento quanto de indeferimento simultaneamente.

*   **Identificador:** NORM-08
    *   **Entrada:** Histórico ("Cálculo 1"); Solicita: "Física 3" (Pré-req: "Física 2").
    *   **Saída esperada:** "Física 3" Indeferida (Falta: "Física 2").
    *   **Descrição:** Tentativa de matrícula em disciplina avançada possuindo um histórico válido, porém totalmente não relacionado aos pré-requisitos exigidos.

*   **Identificador:** NORM-09
    *   **Entrada:** Histórico ("Banco de Dados 1"); Solicita: "Banco de Dados 2" (Pré-req: "Banco de Dados 1"), "Engenharia de Software" (sem pré-req).
    *   **Saída esperada:** Ambas Deferidas.
    *   **Descrição:** Matrícula em múltiplas disciplinas mistas, sendo uma dependente do histórico e a outra livre.

*   **Identificador:** NORM-10
    *   **Entrada:** Histórico ("Prog 1"); Solicita: "Prog 3" (Pré-req: "Prog 2").
    *   **Saída esperada:** "Prog 3" Indeferida (Falta: "Prog 2").
    *   **Descrição:** Pulo de cadeia de pré-requisitos; o sistema deve validar a dependência direta, não permitindo avançar sem o pré-requisito imediato.

## Casos-limite (3 casos)

*   **Identificador:** LIM-01
    *   **Entrada:** Solicitação de matrícula contém a mesma disciplina múltiplas vezes no mesmo semestre.
    *   **Saída esperada:** Deferimento único, ou a segunda requisição deve ser Indeferida (Solicitação duplicada).
    *   **Descrição:** Tratamento de entradas repetidas inseridas pelo aluno.

*   **Identificador:** LIM-02
    *   **Entrada:** Histórico ("Matemática Discreta"); Solicita: "Estruturas de Dados" (Pré-requisitos: "Algoritmos 1" e "Matemática Discreta").
    *   **Saída esperada:** "Estruturas de Dados" Indeferida (Falta: "Algoritmos 1").
    *   **Descrição:** O aluno solicita uma disciplina com múltiplos pré-requisitos possuindo apenas uma parte deles no histórico.

*   **Identificador:** LIM-03
    *   **Entrada:** O aluno solicita uma disciplina com um código inexistente na Grade Curricular oficial fornecida (Ex: código "MAT999").
    *   **Saída esperada:** "MAT999" Indeferida (Disciplina inexistente).
    *   **Descrição:** Tratamento de falhas de consistência entre a solicitação e a grade vigente.

## Casos de Entrada Inválida (2 casos)

*   **Identificador:** INV-01
    *   **Entrada:** Grade Curricular nula ou estrutura vazia passada ao sistema.
    *   **Saída esperada:** Erro na execução ou interrupção informando "Base de dados curricular indisponível".
    *   **Descrição:** O sistema não pode processar o motor de decisão sem a base de conhecimento/fatos estabelecida (a grade curricular).

*   **Identificador:** INV-02
    *   **Entrada:** Histórico do Aluno passado como um tipo de dado escalar (ex: um número inteiro `123`) em vez de uma lista/array estruturada.
    *   **Saída esperada:** Exceção tratada indicando formato inválido de entrada.
    *   **Descrição:** Validação da integridade e tipagem dos dados de entrada (essencial para linguagens como C ou para prever crashes no sistema lógico/funcional).
