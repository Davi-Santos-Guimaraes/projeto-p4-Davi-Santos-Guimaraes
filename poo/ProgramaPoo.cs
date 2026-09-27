using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;

namespace SistemaMatriculasPOO
{
    public static class Limites
    {
        public const int Disciplinas = 100;
        public const int Alunos = 50;
        public const int Texto = 49;
        public const int PreRequisitos = 5;
        public const int Historico = 20;
    }

    public class Disciplina
    {
        private readonly List<string> _preRequisitos;

        public string Codigo { get; }
        public string Nome { get; }
        public IReadOnlyList<string> PreRequisitos => _preRequisitos.AsReadOnly();

        public Disciplina(string codigo, string nome, IEnumerable<string> preRequisitos = null)
        {
            Codigo = codigo;
            Nome = nome;
            _preRequisitos = new List<string>(preRequisitos ?? Enumerable.Empty<string>());
        }
    }

    public class Aluno
    {
        private readonly List<string> _historico;
        private readonly HashSet<string> _disciplinasCursadas;

        public int Matricula { get; }
        public string Nome { get; }
        public IReadOnlyList<string> Historico => _historico.AsReadOnly();

        public Aluno(int matricula, string nome, IEnumerable<string> historico = null)
        {
            Matricula = matricula;
            Nome = nome;
            _historico = new List<string>(historico ?? Enumerable.Empty<string>());
            _disciplinasCursadas = new HashSet<string>(_historico, StringComparer.Ordinal);
        }

        public bool JaCursou(string codigoDisciplina) => _disciplinasCursadas.Contains(codigoDisciplina);

        public bool AdicionarDisciplina(string codigoDisciplina)
        {
            if (_historico.Count >= Limites.Historico || !_disciplinasCursadas.Add(codigoDisciplina))
            {
                return false;
            }

            _historico.Add(codigoDisciplina);
            return true;
        }
    }

    public class GradeCurricular
    {
        private readonly Dictionary<string, Disciplina> _disciplinas;

        public GradeCurricular(IEnumerable<Disciplina> disciplinas = null)
        {
            _disciplinas = new Dictionary<string, Disciplina>(StringComparer.Ordinal);
            foreach (var disciplina in disciplinas ?? Enumerable.Empty<Disciplina>())
            {
                AdicionarDisciplina(disciplina);
            }
        }

        public bool AdicionarDisciplina(Disciplina disciplina)
        {
            if (_disciplinas.Count >= Limites.Disciplinas || _disciplinas.ContainsKey(disciplina.Codigo) ||
                _disciplinas.Values.Any(item => item.Nome == disciplina.Nome))
            {
                return false;
            }

            _disciplinas.Add(disciplina.Codigo, disciplina);
            return true;
        }

        public Disciplina ObterDisciplina(string codigo)
        {
            _disciplinas.TryGetValue(codigo, out var disciplina);
            return disciplina;
        }

        public IEnumerable<Disciplina> ListarDisciplinas() => _disciplinas.Values;
    }

    public class BancoAlunos
    {
        private readonly List<Aluno> _alunos;

        public BancoAlunos(IEnumerable<Aluno> alunos = null)
        {
            _alunos = new List<Aluno>(alunos ?? Enumerable.Empty<Aluno>());
        }

        public IReadOnlyList<Aluno> Listar() => _alunos.AsReadOnly();

        public Aluno ObterPorMatricula(int matricula) => _alunos.FirstOrDefault(aluno => aluno.Matricula == matricula);

        public bool Adicionar(Aluno aluno)
        {
            if (_alunos.Count >= Limites.Alunos || ObterPorMatricula(aluno.Matricula) != null ||
                _alunos.Any(item => item.Nome == aluno.Nome))
            {
                return false;
            }

            _alunos.Add(aluno);
            return true;
        }
    }

    public interface IRegraValidacao
    {
        (bool aprovado, string motivo) Validar(Aluno aluno, Disciplina disciplina);
    }

    public class RegraJaCursada : IRegraValidacao
    {
        public (bool, string) Validar(Aluno aluno, Disciplina disciplina)
        {
            return aluno.JaCursou(disciplina.Codigo)
                ? (false, "Ja cursada")
                : (true, string.Empty);
        }
    }

    public class RegraPreRequisitos : IRegraValidacao
    {
        public (bool, string) Validar(Aluno aluno, Disciplina disciplina)
        {
            foreach (var requisito in disciplina.PreRequisitos)
            {
                if (!aluno.JaCursou(requisito))
                {
                    return (false, $"Falta o pre-requisito '{requisito}'");
                }
            }

            return (true, string.Empty);
        }
    }

    public class ValidadorMatricula
    {
        private readonly List<IRegraValidacao> _regras = new List<IRegraValidacao>
        {
            new RegraJaCursada(),
            new RegraPreRequisitos()
        };

        public (bool deferida, string motivo, Disciplina disciplina) Validar(
            Aluno aluno, string codigoDisciplina, GradeCurricular grade)
        {
            var disciplina = grade.ObterDisciplina(codigoDisciplina);
            if (disciplina == null)
            {
                return (false, "Inexistente", null);
            }

            foreach (var regra in _regras)
            {
                var (aprovado, motivo) = regra.Validar(aluno, disciplina);
                if (!aprovado)
                {
                    return (false, motivo, disciplina);
                }
            }

            return (true, "Requisitos cumpridos", disciplina);
        }
    }

    public class RepositorioArquivos
    {
        private static readonly Encoding Codificacao = new UTF8Encoding(false);
        private readonly string _diretorio;

        public RepositorioArquivos()
        {
            _diretorio = EncontrarDiretorioDados();
            Directory.CreateDirectory(_diretorio);
        }

        public List<Disciplina> CarregarGrade()
        {
            var caminho = Caminho("grade.txt");
            if (!File.Exists(caminho))
            {
                Console.WriteLine("Aviso: Nao foi possivel abrir grade.txt");
                return new List<Disciplina>();
            }

            var disciplinas = new List<Disciplina>();
            foreach (var linha in File.ReadLines(caminho, Codificacao))
            {
                var campos = SepararCampos(linha);
                if (campos.Length < 3 || !int.TryParse(campos[2], NumberStyles.Integer,
                        CultureInfo.InvariantCulture, out var quantidade) || quantidade < 0 ||
                    quantidade > Limites.PreRequisitos || campos.Length != quantidade + 3 ||
                    disciplinas.Count >= Limites.Disciplinas)
                {
                    continue;
                }

                disciplinas.Add(new Disciplina(campos[0], campos[1], campos.Skip(3)));
            }

            return disciplinas;
        }

        public List<Aluno> CarregarAlunos()
        {
            var caminho = Caminho("alunos.txt");
            if (!File.Exists(caminho))
            {
                Console.WriteLine("Aviso: Nao foi possivel abrir alunos.txt");
                return new List<Aluno>();
            }

            var alunos = new List<Aluno>();
            foreach (var linha in File.ReadLines(caminho, Codificacao))
            {
                var campos = SepararCampos(linha);
                if (campos.Length < 3 || !int.TryParse(campos[0], NumberStyles.Integer,
                        CultureInfo.InvariantCulture, out var matricula) ||
                    !int.TryParse(campos[2], NumberStyles.Integer, CultureInfo.InvariantCulture, out var quantidade) ||
                    quantidade < 0 || quantidade > Limites.Historico || campos.Length != quantidade + 3 ||
                    alunos.Count >= Limites.Alunos)
                {
                    continue;
                }

                alunos.Add(new Aluno(matricula, campos[1], campos.Skip(3)));
            }

            return alunos;
        }

        public void SalvarGrade(IEnumerable<Disciplina> disciplinas)
        {
            var linhas = disciplinas.Select(disciplina => string.Join(" ", new[]
            {
                disciplina.Codigo,
                disciplina.Nome,
                disciplina.PreRequisitos.Count.ToString(CultureInfo.InvariantCulture)
            }.Concat(disciplina.PreRequisitos)));
            File.WriteAllLines(Caminho("grade.txt"), linhas, Codificacao);
        }

        public void SalvarAlunos(IEnumerable<Aluno> alunos)
        {
            var linhas = alunos.Select(aluno => string.Join(" ", new[]
            {
                aluno.Matricula.ToString(CultureInfo.InvariantCulture),
                aluno.Nome,
                aluno.Historico.Count.ToString(CultureInfo.InvariantCulture)
            }.Concat(aluno.Historico)));
            File.WriteAllLines(Caminho("alunos.txt"), linhas, Codificacao);
        }

        public void RegistrarResultado(string aluno, string materia, string status, string motivo)
        {
            var linha = $"Aluno: {aluno,-15} | Materia: {materia,-20} | Status: {status,-10} | Motivo: {motivo}{Environment.NewLine}";
            File.AppendAllText(Caminho("relatorio_matriculas.txt"), linha, Codificacao);
        }

        private string Caminho(string nome) => Path.Combine(_diretorio, nome);

        private static string[] SepararCampos(string linha)
        {
            return linha.Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
        }

        private static string EncontrarDiretorioDados()
        {
            var atual = new DirectoryInfo(Environment.CurrentDirectory);
            while (atual != null)
            {
                if (string.Equals(atual.Name, "poo", StringComparison.OrdinalIgnoreCase))
                {
                    return Path.Combine(atual.FullName, "output");
                }

                var pastaPoo = Path.Combine(atual.FullName, "poo");
                if (Directory.Exists(pastaPoo))
                {
                    return Path.Combine(pastaPoo, "output");
                }

                atual = atual.Parent;
            }

            atual = new DirectoryInfo(AppContext.BaseDirectory);
            while (atual != null)
            {
                if (string.Equals(atual.Name, "poo", StringComparison.OrdinalIgnoreCase))
                {
                    return Path.Combine(atual.FullName, "output");
                }

                atual = atual.Parent;
            }

            return Path.Combine(Environment.CurrentDirectory, "output");
        }
    }

    internal static class Program
    {
        private static bool _fimEntrada;

        private static int Main(string[] args)
        {
            var repositorio = new RepositorioArquivos();
            var grade = new GradeCurricular(repositorio.CarregarGrade());
            var alunos = new BancoAlunos(repositorio.CarregarAlunos());
            var validador = new ValidadorMatricula();
            Aluno alunoLogado = null;

            while (!_fimEntrada)
            {
                if (alunoLogado != null)
                {
                    MenuSessao(alunoLogado, grade, alunos, repositorio, validador);
                    alunoLogado = null;
                    continue;
                }

                Console.WriteLine("\n--- SISTEMA DE MATRICULAS ---");
                Console.WriteLine("1 - Login de aluno");
                Console.WriteLine("2 - Cadastrar novo aluno");
                Console.WriteLine("3 - Cadastrar nova disciplina");
                Console.WriteLine("0 - Sair");
                Console.Write("Escolha: ");

                if (!LerInteiro(out var opcao))
                {
                    if (!_fimEntrada) Console.WriteLine("Opcao invalida. Digite apenas um numero.");
                    continue;
                }

                switch (opcao)
                {
                    case 1:
                        alunoLogado = FazerLogin(alunos);
                        break;
                    case 2:
                        CadastrarAluno(alunos, repositorio);
                        break;
                    case 3:
                        CadastrarDisciplina(grade, repositorio);
                        break;
                    case 0:
                        Encerrar();
                        break;
                    default:
                        Console.WriteLine("Opcao invalida.");
                        break;
                }
            }

            return 0;
        }

        private static Aluno FazerLogin(BancoAlunos alunos)
        {
            while (!_fimEntrada)
            {
                Console.Write("Digite o numero de matricula do aluno: ");
                if (!LerInteiro(out var matricula))
                {
                    if (!_fimEntrada) Console.WriteLine("Entrada invalida. Digite apenas a matricula numerica.");
                    continue;
                }

                var aluno = alunos.ObterPorMatricula(matricula);
                if (aluno == null)
                {
                    Console.WriteLine("Erro: Aluno nao encontrado.");
                    return null;
                }

                Console.WriteLine($"Bem-vindo(a), {aluno.Nome}!");
                ExibirHistorico(aluno);
                return aluno;
            }

            return null;
        }

        private static void CadastrarAluno(BancoAlunos alunos, RepositorioArquivos repositorio)
        {
            if (alunos.Listar().Count >= Limites.Alunos)
            {
                Console.WriteLine("Erro: limite de alunos atingido.");
                return;
            }

            Console.Write("Matricula do aluno: ");
            if (!LerInteiro(out var matricula))
            {
                if (!_fimEntrada) Console.WriteLine("Entrada invalida para matricula.");
                return;
            }

            if (alunos.ObterPorMatricula(matricula) != null)
            {
                Console.WriteLine("Erro: ja existe um aluno com essa matricula.");
                return;
            }

            Console.Write("Nome do aluno: ");
            if (!LerTexto(out var nome))
            {
                if (!_fimEntrada) Console.WriteLine("Entrada invalida para nome do aluno.");
                return;
            }

            if (alunos.Listar().Any(aluno => aluno.Nome == nome))
            {
                Console.WriteLine("Erro: ja existe um aluno com esse nome.");
                return;
            }

            alunos.Adicionar(new Aluno(matricula, nome));
            repositorio.SalvarAlunos(alunos.Listar());
            Console.WriteLine("Aluno cadastrado com sucesso!");
        }

        private static void CadastrarDisciplina(GradeCurricular grade, RepositorioArquivos repositorio)
        {
            if (grade.ListarDisciplinas().Count() >= Limites.Disciplinas)
            {
                Console.WriteLine("Erro: limite de disciplinas atingido.");
                return;
            }

            Console.Write("Codigo da disciplina: ");
            if (!LerTexto(out var codigo))
            {
                if (!_fimEntrada) Console.WriteLine("Entrada invalida para codigo da disciplina.");
                return;
            }

            if (grade.ObterDisciplina(codigo) != null)
            {
                Console.WriteLine("Erro: disciplina ja cadastrada.");
                return;
            }

            Console.Write("Nome da disciplina: ");
            if (!LerTexto(out var nome))
            {
                if (!_fimEntrada) Console.WriteLine("Entrada invalida para nome da disciplina.");
                return;
            }

            if (grade.ListarDisciplinas().Any(disciplina => disciplina.Nome == nome))
            {
                Console.WriteLine("Erro: ja existe uma disciplina com esse nome.");
                return;
            }

            Console.Write("Quantidade de pre-requisitos: ");
            if (!LerInteiro(out var quantidade))
            {
                if (!_fimEntrada) Console.WriteLine("Entrada invalida para quantidade de pre-requisitos.");
                return;
            }

            if (quantidade < 0 || quantidade > Limites.PreRequisitos)
            {
                Console.WriteLine($"Erro: numero de pre-requisitos invalido. Use entre 0 e {Limites.PreRequisitos}.");
                return;
            }

            var preRequisitos = new List<string>();
            for (var indice = 0; indice < quantidade; indice++)
            {
                Console.Write($"Pre-requisito {indice + 1}: ");
                if (!LerTexto(out var requisito))
                {
                    if (!_fimEntrada) Console.WriteLine("Entrada invalida para pre-requisito.");
                    return;
                }
                preRequisitos.Add(requisito);
            }

            grade.AdicionarDisciplina(new Disciplina(codigo, nome, preRequisitos));
            repositorio.SalvarGrade(grade.ListarDisciplinas());
            Console.WriteLine("Disciplina cadastrada com sucesso!");
            ListarMaterias(grade);
        }

        private static void MenuSessao(
            Aluno aluno, GradeCurricular grade, BancoAlunos alunos,
            RepositorioArquivos repositorio, ValidadorMatricula validador)
        {
            while (!_fimEntrada)
            {
                Console.WriteLine($"\n=== OPCOES DO ALUNO {aluno.Nome} ===");
                Console.WriteLine("1 - Registrar outra materia");
                Console.WriteLine("2 - Trocar de conta");
                Console.WriteLine("0 - Sair");
                Console.Write("Escolha: ");

                if (!LerInteiro(out var opcao))
                {
                    if (!_fimEntrada) Console.WriteLine("Opcao invalida. Digite apenas um numero.");
                    continue;
                }

                if (opcao == 1)
                {
                    ProcessarMatricula(aluno, grade, alunos, repositorio, validador);
                    if (!_fimEntrada) ExibirHistorico(aluno);
                }
                else if (opcao == 2)
                {
                    Console.WriteLine("Conta trocada com sucesso.");
                    return;
                }
                else if (opcao == 0)
                {
                    Encerrar();
                    return;
                }
                else
                {
                    Console.WriteLine("Opcao invalida.");
                }
            }
        }

        private static void ProcessarMatricula(
            Aluno aluno, GradeCurricular grade, BancoAlunos alunos,
            RepositorioArquivos repositorio, ValidadorMatricula validador)
        {
            ListarMaterias(grade);
            Console.Write("Digite o CODIGO da materia: ");
            if (!LerTexto(out var codigo))
            {
                if (!_fimEntrada) Console.WriteLine("Entrada invalida. Digite um codigo de disciplina.");
                return;
            }

            Console.WriteLine("\n--- PROCESSANDO SOLICITACAO ---");
            if (aluno.JaCursou(codigo))
            {
                Console.WriteLine("[INDEFERIDA] Motivo: Disciplina ja cursada.");
                repositorio.RegistrarResultado(aluno.Nome, codigo, "INDEFERIDA", "Ja cursada");
                return;
            }

            var resultado = validador.Validar(aluno, codigo, grade);
            if (!resultado.deferida)
            {
                var textoMotivo = resultado.motivo == "Inexistente"
                    ? "Disciplina inexistente na grade."
                    : resultado.motivo + ".";
                Console.WriteLine($"[INDEFERIDA] Motivo: {textoMotivo}");
                repositorio.RegistrarResultado(aluno.Nome,
                    resultado.disciplina?.Nome ?? codigo, "INDEFERIDA", resultado.motivo);
                return;
            }

            if (!aluno.AdicionarDisciplina(resultado.disciplina.Codigo))
            {
                Console.WriteLine("Erro: limite de disciplinas no historico atingido.");
                return;
            }

            repositorio.SalvarAlunos(alunos.Listar());
            Console.WriteLine("[DEFERIDA] Matricula liberada!");
            repositorio.RegistrarResultado(aluno.Nome, resultado.disciplina.Nome,
                "DEFERIDA", resultado.motivo);
        }

        private static void ListarMaterias(GradeCurricular grade)
        {
            Console.WriteLine("\n=== MATERIAS DISPONIVEIS PARA MATRICULA ===");
            foreach (var disciplina in grade.ListarDisciplinas())
            {
                Console.WriteLine($"Codigo: {disciplina.Codigo,-6} | Nome: {disciplina.Nome}");
            }
            Console.WriteLine("===========================================\n");
        }

        private static void ExibirHistorico(Aluno aluno)
        {
            Console.WriteLine($"\n=== HISTORICO DE {aluno.Nome} ===");
            if (aluno.Historico.Count == 0)
            {
                Console.WriteLine("Nenhuma disciplina cursada ate o momento.");
            }
            else
            {
                for (var indice = 0; indice < aluno.Historico.Count; indice++)
                {
                    Console.WriteLine($"{indice + 1}. {aluno.Historico[indice]}");
                }
            }
            Console.WriteLine("========================");
        }

        private static bool LerInteiro(out int valor)
        {
            valor = 0;
            var entrada = Console.ReadLine();
            if (entrada == null)
            {
                _fimEntrada = true;
                return false;
            }

            var campos = entrada.Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
            return campos.Length > 0 && int.TryParse(campos[0], NumberStyles.Integer,
                CultureInfo.InvariantCulture, out valor);
        }

        private static bool LerTexto(out string texto)
        {
            texto = string.Empty;
            var entrada = Console.ReadLine();
            if (entrada == null)
            {
                _fimEntrada = true;
                return false;
            }

            var campos = entrada.Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
            if (campos.Length == 0 || campos[0].Length > Limites.Texto)
            {
                return false;
            }

            texto = campos[0];
            return true;
        }

        private static void Encerrar()
        {
            Console.WriteLine("Sistema encerrado. Verifique o arquivo 'relatorio_matriculas.txt'.");
            _fimEntrada = true;
        }
    }
}