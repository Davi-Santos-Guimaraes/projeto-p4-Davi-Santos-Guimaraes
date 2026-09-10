# [P4-ETAPA-01] Especificação Técnica do Problema

## 1. Descrição do Problema
Automação do processo de validação de solicitações de matrícula acadêmica via cruzamento de histórico escolar e grade curricular com pré-requisitos.

## 2. Objetivo
Avaliar a solicitação de matrícula de um aluno e determinar quais disciplinas solicitadas podem ser cursadas e quais devem ser bloqueadas por falta de pré-requisitos ou por duplicidade.

## 3. Entradas
* **Grade Curricular:** Estrutura contendo os códigos de todas as disciplinas ofertadas e seus respectivos pré-requisitos.
* **Histórico do Aluno:** Lista com os códigos das disciplinas que o aluno já concluiu com aprovação.
* **Solicitação de Matrícula:** Lista com os códigos das disciplinas que o aluno deseja cursar no semestre atual.

## 4. Saídas
* **Disciplinas Deferidas:** Códigos das disciplinas aprovadas para matrícula.
* **Disciplinas Indeferidas:** Códigos das disciplinas rejeitadas, acompanhados do motivo exato (ex: "Falta o pré-requisito X" ou "Disciplina já cursada").

## 5. Regras do Problema
1. Para uma disciplina ser deferida, o aluno deve possuir em seu histórico todos os pré-requisitos exigidos por ela na grade curricular.
2. Disciplinas sem pré-requisitos na grade são deferidas automaticamente, desde que não tenham sido cursadas.
3. Se uma disciplina solicitada já constar no histórico de disciplinas concluídas do aluno, ela deve ser obrigatoriamente indeferida.

## 6. Casos de Exemplo
* **Entrada:** Histórico vazio; Solicita: "Algoritmos 1" (sem pré-requisito) → **Saída:** "Algoritmos 1" Deferida.
* **Entrada:** Histórico ("Algoritmos 1"); Solicita: "Algoritmos 2" (Pré-requisito: "Algoritmos 1") → **Saída:** "Algoritmos 2" Deferida.
* **Entrada:** Histórico vazio; Solicita: "Algoritmos 2" (Pré-requisito: "Algoritmos 1") → **Saída:** "Algoritmos 2" Indeferida (Falta: "Algoritmos 1").
* **Entrada:** Histórico ("Cálculo 1"); Solicita: "Cálculo 1" → **Saída:** "Cálculo 1" Indeferida (Já cursada).
* **Entrada:** Histórico ("Matemática Discreta"); Solicita: "Estruturas de Dados" (Pré-requisitos: "Algoritmos 1" e "Matemática Discreta") → **Saída:** "Estruturas de Dados" Indeferida (Falta: "Algoritmos 1").

## 7. Casos-Limite
* O aluno insere o código da mesma disciplina múltiplas vezes na solicitação de matrícula do mesmo semestre.
* O aluno solicita uma disciplina com múltiplos pré-requisitos possuindo apenas uma parte deles no histórico.
* O aluno solicita uma disciplina com um código inexistente na Grade Curricular oficial fornecida.

## 8. Restrições
* Não haverá validação de conflitos de horários entre as turmas/disciplinas solicitadas.
* Não haverá limite máximo ou mínimo de créditos por solicitação de matrícula.
* O sistema não terá interface gráfica (GUI), operando estritamente via terminal/entrada de dados estruturados.

## 9. Principais Conceitos do Domínio
* Disciplina
* Pré-requisito
* Histórico Acadêmico
* Status de Matrícula (Deferido/Indeferido)

## 10. Adequação aos Quatro Paradigmas
* **Imperativo:** Focará no controle de fluxo explícito e na alteração do estado de variáveis/arrays através de laços de repetição (`for`/`while`) para checar o histórico contra a grade.
* **Orientado a Objetos:** Distribuirá as responsabilidades. Um objeto `Controlador` ou `Validador` receberá instâncias de `Aluno` e solicitará validações às instâncias de `Disciplina`, priorizando o encapsulamento e a troca de mensagens.
* **Funcional:** Garantirá a ausência de efeitos colaterais tratando o histórico e a solicitação como listas imutáveis. O cruzamento das aprovações com os pré-requisitos será feito puramente através de funções de alta ordem (`map`, `filter`) e operações de conjuntos matemáticos.
* **Lógico:** O histórico do aluno e a grade atuarão como a base de dados em forma de fatos. A liberação de matrícula será uma regra declarativa processada pelo mecanismo de inferência e unificação nativo do paradigma lógico.

## 11. Linguagens Inicialmente Consideradas
* **Imperativo (C):** Utilizará controle de fluxo explícito, estruturas de repetição e verificação sequencial alterando o estado do sistema.
* **Orientado a Objetos (C#):** Aproveitará o encapsulamento em classes (`Aluno`, `Disciplina`, `Validador`) e a troca de mensagens entre objetos com tipagem estática forte.
* **Funcional (Python):** Tratará o histórico do aluno como uma estrutura de dados imutável, utilizando funções puras de alta ordem e operações de conjuntos matemáticos sem causar efeitos colaterais.
* **Lógico (Prolog):** O histórico e as disciplinas serão declarados como base de conhecimento (fatos), e os pré-requisitos atuarão como as regras. O motor de inferência provará automaticamente se o aluno pode ou não cursar a matéria.
