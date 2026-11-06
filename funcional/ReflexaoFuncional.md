# Etapa 06 — Implementação Funcional

**Tag:** `[P4-ETAPA-06]`  
**Linguagem Utilizada:** Python 3  
**Paradigma:** Funcional  

---

## 1. Mapeamento de Conceitos Funcionais

### Funções Puras vs. Impuras
* **Funções Puras:** `disciplina_existe`, `ja_cursada`, `obter_requisitos_faltantes`, `avaliar_disciplina`, `processar_solicitacao`, `filtrar_deferidas`, `filtrar_indeferidas` e `gerar_relatorio_final`. Todas elas operam exclusivamente sobre os parâmetros de entrada recebidos, possuem transparência referencial total e não provocam efeitos colaterais.
* **Funções Impuras (Fronteira I/O):** `imprimir_resultado_teste` e `executar_bateria_de_testes`. São responsáveis por interagir com o ambiente externo (terminal) através de impressões de dados na saída padrão.

### Dados Imutáveis
O estado mutável foi eliminado do núcleo da aplicação:
* A grade curricular é estruturada em um dicionário de leitura (tratado de forma imutável) e tuplas.
* O histórico do aluno é encapsulado em um `frozenset[str]`, garantindo a impossibilidade de alteração em tempo de execução.
* As solicitações e os resultados finais utilizam `tuple` e `NamedTuple` (`Resultado`), impedindo mutações pontuais em elementos de listas.

### Transformação de Dados e Composição
Em vez de construir iterativamente vetores através de append ou mutações de memória, o programa constrói um pipeline de transformações:
`Solicitação (Tuple[str])` $\rightarrow$ `Avaliações (Tuple[Resultado])` $\rightarrow$ `Filtragem (Deferidos / Indeferidos)`

### Funções de Alta Ordem e Recursão
* Funções como `filter` e `lambda` são empregadas para seleções e filtragens de listas sem loops explícitos.
* A função `processar_solicitacao` utiliza uma função auxiliar recursiva para iterar sobre a solicitação, acumulando os itens já vistos em um `frozenset` imutável para a detecção de duplicatas, preservando a transparência referencial.

---

## 2. Reflexão Obrigatória: Mudança de Paradigma

### De "Que estado devo modificar?" para "Que transformação deve ser aplicada aos dados?"

Nas abordagens anteriores (Imperativa e Orientada a Objetos):
* **Imperativo:** O foco estava na manipulação explicita de vetores mutáveis (como `ArrayList` ou `list`), iterando com estruturas `for`/`while` e efetuando mutações de variáveis de controle ou listas de resultados (`deferidas.append()`).
* **Orientado a Objetos:** A preocupação residia na encapsulação de estado em instâncias (`Aluno`, `Disciplina`, `Validador`), invocando métodos mutadores que alteravam os atributos internos dos objetos conforme o fluxo avançava.

No **Paradigma Funcional**:
* A pergunta central passa a ser: *Quais funções mapeiam o tipo de dados `Entrada` para o tipo de dados `Saída`?*
* A lista de solicitações não é "processada e alterada", mas sim **mapeada** para uma nova coleção imutável de resultados por meio da aplicação da regra de validação em cada elemento.
* Não existem acumuladores globais mutáveis ou métodos que alteram instâncias; o fluxo de controle é inteiramente guiado por composição de funções puras, onde o estado é apenas o dado imutável sendo repassado ao longo do pipeline de transformação.
