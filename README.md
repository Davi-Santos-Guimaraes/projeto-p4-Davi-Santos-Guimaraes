# Projeto P4 — Um Problema, Quatro Paradigmas

**Aluno:** Davi Santos Guimarães  
**Disciplina:** Paradigmas de Linguagens de Programação  
**Tag Atual:** `[P4-ETAPA-01]`

---

## Visão Geral do Projeto
Este repositório é dedicado ao desenvolvimento de soluções para um mesmo problema computacional explorando quatro paradigmas de programação distintos. O objetivo principal é analisar na prática como a mudança de paradigma impacta a modelagem, a representação de dados, o gerenciamento de estado e o fluxo de controle da aplicação.

---

## 1. O Problema: Validador de Matrícula Curricular e Dependências

### Contexto e Motivação
Nas instituições de ensino superior, a estrutura acadêmica é organizada em uma malha de disciplinas interligadas por requisitos de precedência. Para que um estudante possa se matricular em disciplinas avançadas, a instituição exige a comprovação de domínio nos conteúdos fundamentais ministrados em etapas anteriores (os chamados pré-requisitos). 

Atualmente, o processo manual ou semi-automatizado de análise de históricos pode gerar erros de validação, permitindo matrículas indevidas ou bloqueando a progressão de alunos elegíveis.

### Objetivo Computacional
O problema central consiste em processar um grafo direcionado acíclico (DAG) implícito — representado pelas dependências entre disciplinas — e confrontá-lo com o histórico de aprovações do aluno. O sistema deve automatizar a validação de pedidos de matrícula, avaliando o histórico escolar e determinando com precisão quais disciplinas solicitadas podem ser cursadas (deferidas) e quais devem ser bloqueadas (indeferidas) por falta de pré-requisitos.

---

## 2. Especificação Técnica

### 2.1 Entradas
O sistema recebe três conjuntos de dados:
*   **Grade Curricular:** Um dicionário ou lista estruturada contendo os códigos de todas as disciplinas ofertadas e seus respectivos pré-requisitos.
*   **Histórico do Aluno:** Uma lista contendo os códigos das disciplinas que o aluno já concluiu com aprovação.
*   **Solicitação de Matrícula:** Uma lista contendo os códigos das disciplinas que o aluno deseja cursar no semestre atual.

### 2.2 Saídas
O sistema deverá produzir dois relatórios ou listas resultantes:
*   **Disciplinas Deferidas:** Códigos das disciplinas aprovadas para matrícula.
*   **Disciplinas Indeferidas:** Códigos das disciplinas rejeitadas, acompanhados do motivo exato (ex: "Falta o pré-requisito X" ou "Disciplina já cursada").

### 2.3 Regras do problema
1. Para uma disciplina ser deferida, o aluno deve possuir em seu histórico de aprovação todos os pré-requisitos exigidos por ela na grade curricular.
2. Disciplinas que não possuem pré-requisitos na grade curricular são deferidas automaticamente, desde que não tenham sido cursadas.
3. Se uma disciplina solicitada já constar no histórico de disciplinas concluídas do aluno, ela deve ser obrigatoriamente indeferida.

### 2.4 Restrições
*   O sistema não fará validação de conflitos de horários entre as turmas/disciplinas solicitadas.
*   Não haverá limite máximo ou mínimo de créditos por solicitação de matrícula.
*   O sistema não terá interface gráfica (GUI), operando estritamente através da entrada e processamento lógico dos dados.

### 2.5 Casos de Exemplo
*   **Entrada:** Histórico vazio; Solicita: "Algoritmos 1" (sem pré-requisito) → **Saída esperada:** "Algoritmos 1" Deferida.
*   **Entrada:** Histórico ("Algoritmos 1"); Solicita: "Algoritmos 2" (Pré-requisito: "Algoritmos 1") → **Saída esperada:** "Algoritmos 2" Deferida.
*   **Entrada:** Histórico vazio; Solicita: "Algoritmos 2" (Pré-requisito: "Algoritmos 1") → **Saída esperada:** "Algoritmos 2" Indeferida (Falta: "Algoritmos 1").
*   **Entrada:** Histórico ("Cálculo 1"); Solicita: "Cálculo 1" → **Saída esperada:** "Cálculo 1" Indeferida (Já cursada).
*   **Entrada:** Histórico ("Matemática Discreta"); Solicita: "Estruturas de Dados" (Pré-requisitos: "Algoritmos 1" e "Matemática Discreta") → **Saída esperada:** "Estruturas de Dados" Indeferida (Falta: "Algoritmos 1").

### 2.6 Casos-Limite
*   O aluno insere o código da mesma disciplina múltiplas vezes na solicitação de matrícula do mesmo semestre.
*   O aluno solicita uma disciplina com múltiplos pré-requisitos possuindo apenas uma parte deles no histórico.
*   O aluno solicita uma disciplina com um código inexistente na Grade Curricular oficial fornecida.

---

## 3. Adequação e Modelagem nos Quatro Paradigmas
Este problema foi escolhido por possuir uma lógica de negócio clara que permite ser decomposta de formas completamente distintas em cada modelo de programação:

*   **Paradigma Imperativo:** Focará no controle de fluxo explícito e na alteração do estado de variáveis e arrays. Permitirá explorar a varredura sequencial de coleções através de laços de repetição (`for`/`while`), atualizando contadores e vetores de resultado passo a passo para checar o histórico contra a grade.
*   **Paradigma Orientado a Objetos:** Permitirá abstrair os conceitos do domínio em entidades bem delimitadas. Um objeto `Controlador` ou `Validador` receberá instâncias de `Aluno` e solicitará validações às instâncias de `Disciplina`, priorizando o encapsulamento, responsabilidades bem distribuídas e a troca de mensagens entre objetos.
*   **Paradigma Funcional:** Garantirá a ausência de efeitos colaterais tratando o histórico e a solicitação como listas estritamente imutáveis. O cruzamento das aprovações com os pré-requisitos será resolvido puramente através de funções de alta ordem (`map`, `filter`), transparência referencial e operações de teoria dos conjuntos.
*   **Paradigma Lógico:** É o cenário ideal para o modelo declarativo. A grade de disciplinas e o histórico do aluno atuarão como a base de dados em forma de fatos. A liberação de matrícula será uma regra declarativa processada pelo mecanismo de inferência e unificação nativo do paradigma lógico, dispensando a escrita de rotinas manuais de busca.

---

## 4. Linguagens Escolhidas

*   **Imperativo (C):** Linguagem com que estou mais confortável em trabalhar com loops
  
*   **Orientado a Objetos (C#):** Aprendi a usar C# em POO e acredito que pode ser uma boa linguagem para resolver esse problema.
  
*   **Funcional (Python):** Python provavelmente vai ser a que irei usar para esse.
  
*   **Lógico (Prolog):** Não sei do porque ainda, mas é a mais recomendada para esse tipo de paradigma.

---

## 5. Estrutura do Repositório

```text
projeto-p4-Davi-Santos-Guimaraes/
│
├── README.md
│
├── docs/
│   ├── problema.md
│   ├── especificacao.md
│   ├── decisoes.md
│   └── comparacao-final.md
│
├── testes/
│   └── casos.md
│
├── imperativo/
├── poo/
├── funcional/
├── logico/
└── integrado/
