# [P4-ETAPA-01] Especificação do Problema: Validador de Matrícula Curricular e Dependências

## 1. Descrição do problema
Em instituições de ensino superior, o processo de matrícula exige que os estudantes respeitem a hierarquia de dependências das disciplinas. O problema consiste em automatizar a validação de pedidos de matrícula, cruzando o histórico acadêmico do aluno com a grade curricular vigente para garantir que nenhuma etapa obrigatória seja ignorada.

## 2. Objetivo
O sistema deverá ser capaz de processar uma solicitação de matrícula, avaliando o histórico escolar de um aluno e determinando quais disciplinas solicitadas podem ser cursadas e quais devem ser bloqueadas por falta de pré-requisitos.

## 3. Entradas
O sistema recebe três conjuntos de dados:
*   Grade Curricular: Um dicionário ou lista estruturada contendo os códigos de todas as disciplinas ofertadas e seus respectivos pré-requisitos.
*   Histórico do Aluno: Uma lista contendo os códigos das disciplinas que o aluno já concluiu com aprovação.
*   Solicitação de Matrícula: Uma lista contendo os códigos das disciplinas que o aluno deseja cursar no semestre atual.

## 4. Saídas
O sistema deverá produzir dois relatórios ou listas resultantes:
*   Disciplinas Deferidas: Códigos das disciplinas aprovadas para matrícula.
*   Disciplinas Indeferidas: Códigos das disciplinas rejeitadas, acompanhados do motivo exato (ex: "Falta o pré-requisito X" ou "Disciplina já cursada").

## 5. Regras do problema
*   Para uma disciplina ser deferida, o aluno deve possuir em seu histórico de aprovação todos os pré-requisitos exigidos por ela na grade curricular.
*   Disciplinas que não possuem pré-requisitos na grade curricular são deferidas automaticamente, desde que não tenham sido cursadas.
*   Se uma disciplina solicitada já constar no histórico de disciplinas concluídas do aluno, ela deve ser obrigatoriamente indeferida.

## 6. Casos de exemplo
*   **Entrada:** Histórico vazio; Solicita: "Algoritmos 1" (sem pré-requisito) → **Saída esperada:** "Algoritmos 1" Deferida.
*   **Entrada:** Histórico ("Algoritmos 1"); Solicita: "Algoritmos 2" (Pré-requisito: "Algoritmos 1") → **Saída esperada:** "Algoritmos 2" Deferida.
*   **Entrada:** Histórico vazio; Solicita: "Algoritmos 2" (Pré-requisito: "Algoritmos 1") → **Saída esperada:** "Algoritmos 2" Indeferida (Falta: "Algoritmos 1").
*   **Entrada:** Histórico ("Cálculo 1"); Solicita: "Cálculo 1" → **Saída esperada:** "Cálculo 1" Indeferida (Já cursada).
*   **Entrada:** Histórico ("Matemática Discreta"); Solicita: "Estruturas de Dados" (Pré-requisitos: "Algoritmos 1" e "Matemática Discreta") → **Saída esperada:** "Estruturas de Dados" Indeferida (Falta: "Algoritmos 1").

## 7. Casos-limite
*   O aluno insere o código da mesma disciplina múltiplas vezes na solicitação de matrícula do mesmo semestre.
*   O aluno solicita uma disciplina com múltiplos pré-requisitos possuindo apenas uma parte deles no histórico.
*   O aluno solicita uma disciplina com um código inexistente na Grade Curricular oficial fornecida.

## 8. Restrições
*   O sistema não fará validação de conflitos de horários entre as turmas/disciplinas solicitadas.
*   Não haverá limite máximo ou mínimo de créditos por solicitação de matrícula.
*   O sistema não terá interface gráfica (GUI), operando estritamente através da entrada e processamento lógico dos dados.

## 9. Principais conceitos do domínio
*   Disciplina.
*   Pré-requisito.
*   Histórico Acadêmico.
*   Status de Matrícula (Deferido/Indeferido). 

## 10. Adequação aos quatro paradigmas
O problema mantém a mesma estrutura conceitual ao longo de todo o projeto, mas a decomposição da solução e o controle de fluxo mudam em cada modelo de programação:
*   **Imperativo:** Focará no controle de fluxo explícito e na alteração do estado de variáveis e arrays através de laços de repetição tradicionais (como `for` ou `while`) para checar o histórico contra a grade.
*   **Orientado a Objetos:** Distribuirá as responsabilidades. Um objeto `Controlador` ou `Validador` receberá instâncias de `Aluno` e solicitará validações às instâncias de `Disciplina`, priorizando o encapsulamento e a troca de mensagens.
*   **Funcional:** Garantirá a ausência de efeitos colaterais tratando o histórico e a solicitação como listas estritamente imutáveis. O cruzamento das aprovações com os pré-requisitos será feito puramente através de funções de alta ordem (map, filter) e operações de conjunto.
*   **Lógico:** O histórico de aprovação do aluno e a grade de disciplinas atuarão como a base de dados em forma de fatos. A liberação de matrícula será uma regra declarativa processada pelo mecanismo de inferência e unificação nativo do paradigma lógico.

## 11. Linguagens inicialmente consideradas
*   **Imperativo (C ou Python):** A implementação utilizará controle de fluxo explícito, estruturas de repetição (loops) e verificação sequencial alterando o estado do sistema para validar o histórico contra a grade. 
*   **Orientado a Objetos (C#):** A linguagem possui tipagem estática forte e suporte maduro para POO. A solução aproveitará o encapsulamento em classes (`Aluno`, `Disciplina`, `Validador`) e a troca de mensagens entre objetos para ocultar a complexidade da validação.
*   **Funcional (Python):** Python oferece ferramentas nativas para o paradigma funcional. A solução tratará o histórico do aluno como uma estrutura de dados imutável, utilizando funções puras de alta ordem (como `filter` e `map`) e operações de conjuntos matemáticos (Sets) para verificar as dependências sem causar efeitos colaterais.
*   **Lógico (Prolog):** Sendo a principal representante do paradigma lógico, o Prolog é ideal para este problema. O histórico e as disciplinas serão declarados como base de conhecimento (fatos), e os pré-requisitos atuarão como as regras matemáticas. O próprio motor de inferência e unificação da linguagem se encarregará de provar se o aluno pode ou não cursar a matéria.
