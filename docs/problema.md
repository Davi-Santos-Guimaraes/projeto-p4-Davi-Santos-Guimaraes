# Visão Geral do Problema: Validador de Matrícula Curricular e Dependências

**Tag:** `[P4-ETAPA-01]`

---

## 1. Contexto e Motivação
Nas instituições de ensino superior, a estrutura acadêmica é organizada em uma malha de disciplinas interligadas por requisitos de precedência. Para que um estudante possa se matricular em disciplinas avançadas, a instituição exige a comprovação de domínio nos conteúdos fundamentais ministrados em etapas anteriores (os chamados pré-requisitos).

Atualmente, o processo manual ou semi-automatizado de análise de históricos pode gerar gargalos operacionais e erros de validação, permitindo matrículas indevidas ou bloqueando indevidamente a progressão de alunos elegíveis. 

O foco deste projeto é construir um motor de decisão capaz de analisar o estado acadêmico de um estudante e responder com precisão se um conjunto de disciplinas solicitadas pode ou não ser deferido.

---

## 2. O Problema Computacional
O problema central consiste em processar um grafo direcionado acíclico (DAG) implícito — representado pelas dependências entre disciplinas — e confrontá-lo com um conjunto de fatos consumados (o histórico de aprovações do aluno).

A cada solicitação de matrícula no semestre corrente, o sistema deve responder a três perguntas fundamentais para cada matéria solicitada:
1. O aluno já concluiu esta disciplina anteriormente?
2. O aluno cumpre integralmente todos os pré-requisitos exigidos por esta disciplina?
3. A solicitação apresenta inconsistências (como duplicidades ou matérias inexistentes na grade)?

---

## 3. Justificativa de Adequação aos 4 Paradigmas
Este problema foi escolhido por possuir uma lógica de negócio clara que permite ser modelada e decomposta de formas completamente distintas em cada um dos quatro paradigmas estudados na disciplina:

* **Paradigma Imperativo:** O problema permite explorar o controle explícito de estado (variáveis mutáveis e sinalizadores) e a varredura sequencial de coleções através de laços de repetição (`for`/`while`), atualizando contadores e vetores de resultado passo a passo.
* **Paradigma Orientado a Objetos:** Permite abstrair os conceitos do domínio em entidades bem delimitadas (`Aluno`, `Disciplina`, `GradeCurricular`, `Validador`). A solução focará em encapsulamento, responsabilidades bem distribuídas e troca de mensagens entre objetos.
* **Paradigma Funcional:** O problema se encaixa perfeitamente em transformações sobre dados imutáveis. O histórico e as solicitações são tratados como listas puras, e a validação é resolvida com funções de alta ordem (`map`, `filter`), transparência referencial e operações de teoria dos conjuntos sem efeitos colaterais.
* **Paradigma Lógico:** É o cenário ideal para o modelo declarativo. A grade de disciplinas e o histórico do aluno são declarados diretamente como uma base de conhecimento (fatos), enquanto as regras de pré-requisito são expressas como predicados lógicos. O motor de inferência da linguagem resolve o problema por unificação e retrocesso nativos.
