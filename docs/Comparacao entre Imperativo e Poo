# ETAPA 05 — COMPARAÇÃO ENTRE IMPERATIVO E POO

## TAG OBRIGATÓRIA

`[P4-ETAPA-05]`

# Análise Comparativa: Imperativo vs Orientado a Objetos

## 1. Análise dos Aspectos de Implementação

### Representação do estado

Na implementação imperativa em C, o estado do sistema é mantido principalmente em estruturas `struct` armazenadas em vetores, como o vetor da grade curricular e o banco de alunos. Essas estruturas são manipuladas por funções externas, que recebem os dados e realizam as alterações necessárias.

Na implementação orientada a objetos em C#, o estado foi distribuído entre objetos que representam as partes do problema. A classe `Aluno` mantém seu próprio histórico e suas disciplinas cursadas, enquanto `Disciplina` mantém seus dados e pré-requisitos. A `GradeCurricular` mantém as disciplinas disponíveis e o `BancoAlunos` mantém os alunos cadastrados.

Assim, a principal diferença está na forma como o estado é organizado: no C ele fica concentrado em estruturas manipuladas externamente, enquanto no C# ele fica associado aos objetos que representam esses dados.

### Mutabilidade

A versão imperativa trabalha diretamente com a alteração das estruturas armazenadas. Variáveis, contadores, vetores e campos das `structs` são modificados por atribuições, incrementos e funções como `strcpy`.

Na versão orientada a objetos, a mutabilidade continua existindo, pois o sistema precisa cadastrar alunos, adicionar disciplinas e alterar os dados durante sua execução. A diferença é que essas alterações são controladas pelos próprios objetos.

Por exemplo, a coleção interna do `Aluno` é privada e a inclusão de uma disciplina ocorre através do método `AdicionarDisciplina`, que realiza verificações antes de modificar o estado. As coleções também são disponibilizadas externamente principalmente através de interfaces de leitura, como `IReadOnlyList`.

Portanto, a orientação a objetos não elimina a mutabilidade, mas permite controlar melhor onde e como ela ocorre.

### Fluxo de controle

Na implementação imperativa, o fluxo é controlado diretamente pelo programa através de `if`, `else`, `for`, `while`, `break` e `continue`. As funções executam uma sequência explícita de operações e retornam seus resultados para quem as chamou.

Na implementação orientada a objetos, o fluxo principal continua utilizando estruturas condicionais e de repetição, principalmente na interação com o usuário. A diferença aparece principalmente na distribuição das responsabilidades.

Durante uma validação, por exemplo, o `ValidadorMatricula` percorre uma coleção de objetos `IRegraValidacao` e chama `Validar()` em cada regra. Dessa forma, ele não precisa possuir um `if` específico para cada tipo de regra.

### Decomposição do problema

No paradigma imperativo, o problema foi dividido principalmente em funções. Exemplos são funções responsáveis por localizar alunos ou disciplinas, carregar dados, salvar informações e processar uma matrícula.

Na orientação a objetos, a decomposição foi feita identificando entidades e responsabilidades do domínio. Foram criadas classes como `Aluno`, `Disciplina`, `GradeCurricular`, `BancoAlunos`, `ValidadorMatricula` e `RepositorioArquivos`.

Além disso, as regras de validação foram separadas através da interface `IRegraValidacao`, permitindo que cada regra tenha sua própria implementação.

Assim, enquanto a versão imperativa organiza principalmente **ações**, a versão OO organiza principalmente **objetos, dados e responsabilidades**.

### Reutilização

A implementação imperativa possui funções que podem ser reutilizadas, mas elas dependem mais diretamente das estruturas de dados utilizadas pelo programa, como vetores, índices e ponteiros.

Na versão orientada a objetos, a reutilização aparece principalmente na arquitetura das regras de validação. O `ValidadorMatricula` trabalha com `IRegraValidacao`, podendo receber diferentes implementações dessa interface.

Por exemplo, `RegraJaCursada` e `RegraPreRequisitos` podem ser utilizadas pelo mesmo mecanismo de validação. Uma nova regra pode seguir o mesmo contrato sem precisar alterar a estrutura básica do validador.

### Manutenção

Na versão imperativa, uma mudança em determinada regra pode exigir alterações nas funções responsáveis pelo processamento da matrícula. Conforme novas verificações são adicionadas, essas funções podem acumular mais condições e responsabilidades.

Na versão OO, as responsabilidades estão mais distribuídas. Uma alteração relacionada ao histórico do aluno pode ser feita na classe `Aluno`, enquanto uma alteração relacionada a uma regra de matrícula pode ser feita na implementação correspondente de `IRegraValidacao`.

Isso facilita a localização do código que precisa ser alterado, embora também aumente a quantidade de classes que precisam ser compreendidas.

### Facilidade de extensão

A orientação a objetos trouxe uma vantagem principalmente na extensão das regras de validação.

Atualmente, o `ValidadorMatricula` utiliza uma coleção de `IRegraValidacao`. Dessa forma, uma nova regra pode ser criada como uma nova classe que implemente essa interface e adicionada ao conjunto de regras.

Por exemplo, seria possível criar uma regra para verificar um limite de disciplinas por matrícula sem precisar transformar o `ValidadorMatricula` em uma sequência extensa de condições específicas.

Na implementação imperativa, uma mudança desse tipo provavelmente seria incorporada às funções existentes de processamento da matrícula.

### Tratamento de erros

Na versão em C, várias situações de erro são representadas por valores de retorno e sinalizadores. Por exemplo, uma função de busca pode retornar `-1` quando uma disciplina não é encontrada. O código que chamou a função precisa então interpretar esse resultado e tomar uma decisão.

Na versão C#, os resultados da validação são retornados de forma estruturada através de tuplas nomeadas, contendo informações como `deferida`, `motivo` e `disciplina`.

Além disso, as próprias classes responsáveis pelos dados realizam algumas validações. Por exemplo, `Aluno.AdicionarDisciplina` verifica situações como limite do histórico e disciplina duplicada, enquanto `GradeCurricular` e `BancoAlunos` verificam limites e duplicidades durante os cadastros.

Assim, parte do tratamento de erros fica associada à responsabilidade de cada objeto.

### Efeitos colaterais

Nas duas implementações existem efeitos colaterais porque o sistema precisa interagir com o usuário e com arquivos.

Na implementação imperativa, operações de entrada, saída e persistência estão presentes nas funções do programa. A execução de funções como `scanf`, `printf` e operações de arquivo altera o ambiente externo ao programa.

Na versão OO, a classe `RepositorioArquivos` foi criada para concentrar as operações de leitura e gravação. Dessa forma, as classes responsáveis pelas regras de negócio não precisam lidar diretamente com os arquivos.

Isso não elimina os efeitos colaterais, mas melhora sua organização e separação em relação à lógica de validação.

### Facilidade para testar

A lógica da versão OO pode ser testada de maneira mais isolada porque algumas responsabilidades estão representadas em classes independentes.

Por exemplo, uma instância de `Aluno` pode ser criada e utilizada para verificar o funcionamento de `JaCursou` ou `AdicionarDisciplina`. Da mesma forma, uma `RegraPreRequisitos` pode receber um aluno e uma disciplina para verificar uma situação específica.

Na versão imperativa, as funções dependem mais diretamente das estruturas e dos parâmetros que representam os bancos de dados, sendo necessário preparar esses dados manualmente para realizar testes.

Portanto, a separação das responsabilidades na versão OO facilita testes isolados de partes específicas do sistema. Isso não significa que a implementação atual possua automaticamente uma suíte de testes unitários, mas sua estrutura é mais adequada para criá-los.

### Organização do código

A versão imperativa concentra sua organização em funções e estruturas de dados. A lógica de manipulação dos alunos e disciplinas fica distribuída entre essas funções, que recebem os dados necessários por parâmetros.

Na versão OO, o código é distribuído entre classes com responsabilidades específicas. O `Aluno` concentra comportamentos relacionados ao aluno, `Disciplina` representa a disciplina, `GradeCurricular` administra as disciplinas, `BancoAlunos` administra os alunos, `ValidadorMatricula` coordena a validação e `RepositorioArquivos` cuida da persistência.

Essa divisão torna a estrutura do sistema mais próxima dos elementos que estão sendo representados.

### Complexidade

A complexidade das duas versões aparece de maneiras diferentes.

Na implementação imperativa, existe uma quantidade maior de manipulação direta de estruturas, índices, ponteiros e condições. O programador precisa controlar explicitamente como os dados são localizados e modificados.

Na versão OO, parte dessa complexidade é transferida para as próprias classes. Isso pode deixar determinadas operações mais simples de entender, mas aumenta a quantidade de abstrações do programa.

Portanto, orientação a objetos não necessariamente reduz a complexidade total. Ela reorganiza a complexidade, tornando algumas partes mais estruturadas e introduzindo outras abstrações.

---

# 2. Perguntas

## 1. Qual problema ficou mais fácil de expressar de forma imperativa?

As operações mais diretas de entrada, saída e manipulação sequencial de dados ficaram mais naturais na implementação imperativa.

Ler dados, percorrer vetores, procurar uma disciplina através de índices e executar uma sequência de condições pode ser feito diretamente com `scanf`, `printf`, `for`, `while` e `if`.

Para operações simples e muito próximas da estrutura dos dados, o modelo imperativo exige menos abstrações.

## 2. Qual problema ficou mais fácil de expressar utilizando orientação a objetos?

A organização das regras de matrícula e dos dados relacionados aos alunos e disciplinas ficou mais fácil de expressar utilizando orientação a objetos.

Na versão OO, o `Aluno` possui o próprio histórico, a `Disciplina` possui seus pré-requisitos e o `ValidadorMatricula` utiliza objetos que representam regras específicas.

Isso evita que toda a lógica precise conhecer diretamente os índices e estruturas internas utilizadas para armazenar os dados.

## 3. Onde a orientação a objetos realmente trouxe vantagem?

A principal vantagem apareceu na **separação de responsabilidades e na extensão das regras de validação**.

A utilização de `IRegraValidacao` permitiu separar diferentes regras, como `RegraJaCursada` e `RegraPreRequisitos`, mantendo o `ValidadorMatricula` independente dos detalhes internos de cada uma.

Outra vantagem foi o isolamento da persistência através do `RepositorioArquivos`. As regras de negócio não precisam conhecer diretamente a forma como os dados são salvos nos arquivos.

Essas duas características tornam a estrutura mais organizada quando o sistema precisa crescer.

## 4. Em quais situações a utilização de objetos acrescentou complexidade desnecessária?

A orientação a objetos acrescentou algumas camadas para operações que poderiam ser realizadas diretamente com estruturas simples.

Por exemplo, manter classes como `BancoAlunos` e `GradeCurricular` cria uma camada adicional em comparação com simplesmente utilizar uma lista ou um dicionário diretamente no fluxo principal.

Da mesma forma, criar uma interface para as regras de validação representa uma estrutura adicional que não seria necessária se o sistema tivesse apenas uma ou duas regras fixas.

Portanto, o benefício dessas abstrações aparece principalmente quando existe necessidade de organização, reutilização ou extensão. Para operações muito pequenas e estáveis, a abordagem imperativa pode ser mais simples.

## 5. Que partes do problema praticamente não mudaram entre as duas implementações?

A regra fundamental do problema permaneceu praticamente a mesma.

Nas duas implementações, o sistema precisa:

1. localizar um aluno;
2. localizar a disciplina solicitada;
3. verificar se a disciplina já foi cursada;
4. verificar seus pré-requisitos;
5. deferir ou indeferir a solicitação;
6. informar o motivo da decisão;
7. armazenar os dados necessários para o funcionamento do sistema.

Também não mudou a necessidade de interação com o usuário através de menus e entradas de dados. O que mudou principalmente foi a maneira como essas operações foram organizadas no código.

## 6. Que partes precisaram ser completamente remodeladas?

A principal remodelação ocorreu na **representação dos dados e na distribuição das responsabilidades**.

Na implementação em C, alunos e disciplinas eram representados por `structs` e armazenados em estruturas que eram manipuladas por funções externas. A função responsável pelo processamento precisava trabalhar diretamente com esses dados.

Na versão C#, esses elementos foram transformados em objetos. O `Aluno` passou a controlar seu próprio histórico, a `Disciplina` passou a representar seus próprios pré-requisitos e a `GradeCurricular` e o `BancoAlunos` passaram a administrar suas respectivas coleções.

Outra remodelação importante ocorreu na validação. Em vez de concentrar as verificações em uma única sequência de condições, as regras foram separadas através da interface `IRegraValidacao` e de suas implementações.

A persistência também foi reorganizada com a criação do `RepositorioArquivos`, isolando as operações de arquivo das demais responsabilidades do sistema.

---

# 3. Conclusão

A comparação entre as duas implementações mostra que o problema em si permaneceu praticamente o mesmo, mas a forma de representá-lo mudou.

A implementação imperativa se mostrou mais direta para operações sequenciais, manipulação de estruturas e controle explícito do fluxo. Já a implementação orientada a objetos permitiu organizar o sistema em entidades e responsabilidades, trazendo vantagens principalmente para encapsulamento, separação de responsabilidades, reutilização e extensão das regras de validação.

Ao mesmo tempo, a versão OO também introduziu mais classes, interfaces e abstrações. Isso mostra que orientação a objetos não é automaticamente melhor para qualquer situação. Seu benefício aparece principalmente quando a divisão de responsabilidades e a possibilidade de evolução do sistema justificam essas abstrações.

Portanto, a principal diferença entre as duas versões não está nas regras do problema, que permaneceram praticamente as mesmas, mas na maneira como essas regras e os dados foram organizados e relacionados dentro do programa.
