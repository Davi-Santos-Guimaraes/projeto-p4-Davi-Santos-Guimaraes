# Reflexão: Paradigma Orientado a Objetos
**Tag:** `[P4-ETAPA-04]`

### Como meu modelo mudou ao passar do paradigma imperativo para o orientado a objetos?
A forma de pensar mudou bastante nessa transição. No modelo imperativo, o programa era basicamente uma sequência de passos, funções e laços que manipulavam os dados diretamente. Agora, no Orientado a Objetos (OO), a lógica foi dividida em objetos que representam partes do problema, como `Aluno`, `Disciplina`, `GradeCurricular` e `ValidadorMatricula`.

Em vez de concentrar a maior parte da lógica em funções que recebem e modificam estruturas, cada classe passou a ter responsabilidades mais específicas. Dessa forma, os dados e as operações relacionadas a eles ficam mais próximos uns dos outros. Abaixo explico de forma simplificada como cada ponto foi resolvido no código:

**1. Representação do estado**
Antes, os dados dos alunos e das disciplinas ficavam principalmente em vetores de structs e eram manipulados por funções externas. Agora, o estado fica guardado dentro de cada objeto. 
Por exemplo, a classe `Aluno` mantém internamente seu histórico e as disciplinas cursadas por meio das coleções privadas `_historico` e `_disciplinasCursadas`. A classe `Disciplina` também mantém seus próprios dados e seus pré-requisitos. Além disso, o `BancoAlunos` mantém os alunos cadastrados e a `GradeCurricular` mantém as disciplinas disponíveis. Assim, o estado do sistema fica dividido entre objetos que representam cada parte do domínio.

**2. Responsabilidades**
Cada classe possui uma responsabilidade mais específica, deixando o sistema mais organizado:
* O `Aluno` mantém seus dados e possui operações relacionadas ao seu histórico, como verificar se já cursou uma disciplina através do método `JaCursou`.
* A `Disciplina` representa uma disciplina da grade, armazenando seu código, nome e seus pré-requisitos.
* A `GradeCurricular` é responsável por armazenar e localizar as disciplinas disponíveis.
* O `BancoAlunos` é responsável por armazenar e localizar os alunos cadastrados.
* O `ValidadorMatricula` coordena a validação de uma solicitação, aplicando as regras cadastradas.
* O `RepositorioArquivos` é responsável pela leitura e gravação dos arquivos .txt, separando a persistência dos dados da lógica principal do sistema.

Dessa forma, uma alteração em uma parte específica do sistema tende a ficar concentrada na classe responsável por ela.

**3. Encapsulamento**
Usei o encapsulamento para proteger os dados internos dos objetos. As coleções utilizadas pelas classes são mantidas como `private`, impedindo que qualquer parte do programa possa alterá-las diretamente. 
Por exemplo, o histórico do `Aluno` é mantido internamente e disponibilizado para leitura através de uma propriedade somente leitura. Para adicionar uma disciplina ao aluno, deve ser utilizado o método `AdicionarDisciplina`, que também realiza as verificações necessárias, como limite do histórico e duplicidade. A mesma ideia é utilizada na `GradeCurricular` e no `BancoAlunos`, que controlam internamente como seus dados são adicionados e consultados.

**4. Relacionamento entre componentes (Composição e Agregação)**
As classes trabalham em conjunto para formar o sistema. A `GradeCurricular`, por exemplo, mantém várias `Disciplina` em um dicionário. Da mesma forma, o `BancoAlunos` mantém vários objetos `Aluno`. 
O `ValidadorMatricula` também possui uma coleção de objetos que implementam `IRegraValidacao`. Assim, ele não precisa conhecer detalhadamente cada regra, apenas precisa solicitar que cada uma realize sua validação. Esses relacionamentos permitem dividir o problema em partes menores sem deixar toda a lógica concentrada em uma única classe.

**5. Reutilização e Extensão do Sistema (Polimorfismo)**
Uma das principais vantagens dessa versão está na forma como as regras de validação foram organizadas. Foi criada a interface `IRegraValidacao`, que define o comportamento esperado para uma regra de validação. As classes `RegraJaCursada` e `RegraPreRequisitos` implementam essa interface. 
O `ValidadorMatricula` trabalha com uma coleção de `IRegraValidacao`, podendo executar as regras sem precisar saber qual classe específica está sendo utilizada. Isso caracteriza o uso de polimorfismo. Por exemplo, se futuramente fosse necessário criar uma regra para limitar a quantidade de disciplinas que um aluno pode solicitar, seria possível criar uma nova classe que implementasse `IRegraValidacao` e adicioná-la ao conjunto de regras do validador. Assim, o sistema pode receber novas regras sem que seja necessário concentrar todas elas em uma única função cheia de `if` e `else`.

**6. Por que não usei Herança de Classes?**
A especificação da etapa não exige que a herança seja utilizada de forma artificial. Analisando o problema, não encontrei uma relação de especialização que justificasse criar uma hierarquia de classes. 
Por exemplo, criar uma classe `DisciplinaComRequisito` herdando de `Disciplina` não seria uma boa solução apenas para demonstrar herança. Os pré-requisitos já são uma característica que pode ser representada dentro da própria classe `Disciplina`. Para as regras de validação, a utilização de uma interface foi mais adequada. `RegraJaCursada` e `RegraPreRequisitos` possuem comportamentos diferentes, mas seguem o mesmo contrato definido por `IRegraValidacao`. Por isso, neste problema, preferi utilizar interfaces e composição em vez de criar uma hierarquia de herança sem uma necessidade real.

**7. Reutilização e separação da persistência**
Outro ponto importante da implementação foi separar o acesso aos arquivos através da classe `RepositorioArquivos`. Na versão imperativa, as operações de leitura e gravação estavam mais diretamente ligadas às funções do programa. Na versão orientada a objetos, essas operações foram reunidas em uma classe específica. Com isso, as classes responsáveis pelas regras de negócio não precisam saber como os dados são armazenados nos arquivos. Se futuramente o sistema passasse a utilizar outro tipo de armazenamento, como um banco de dados, seria possível modificar essa parte sem precisar alterar toda a lógica de validação das matrículas.

**Nível de Dificuldade**
O principal desafio não foi apenas escrever o código, mas mudar a forma de pensar sobre o problema. Na implementação imperativa, era mais natural pensar em quais funções deveriam ser executadas e quais variáveis precisavam ser modificadas. Na implementação orientada a objetos, foi necessário pensar primeiro em quais objetos existem no problema, quais dados pertencem a cada um e quais responsabilidades cada objeto deveria possuir. 
Perguntas como "quem é o dono dessa informação?" e "quem deve realizar essa validação?" ajudaram a definir a divisão das classes. Depois que essa estrutura foi definida, o código ficou mais organizado e as responsabilidades ficaram mais fáceis de identificar. A utilização de encapsulamento, composição e polimorfismo também tornou a solução mais preparada para receber novas regras sem precisar modificar toda a estrutura do programa. Dessa forma, a principal mudança entre as duas etapas foi a maneira de modelar o problema: na solução imperativa, o foco estava na sequência de operações que modificavam os dados; na solução orientada a objetos, o foco passou a ser a organização dos dados e comportamentos em objetos com responsabilidades bem definidas.