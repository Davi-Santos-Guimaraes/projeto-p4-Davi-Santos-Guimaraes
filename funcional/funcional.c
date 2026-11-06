
"""
Projeto P4 — Validador de Matrícula Curricular e Dependências
Paradigma Funcional (Python 3.10+)
Tag: [P4-ETAPA-06]
"""

from typing import NamedTuple, FrozenSet, Dict, Tuple, Callable
from functools import reduce

# ==============================================================================
# ESTRUTURAS DE DADOS IMUTÁVEIS
# ==============================================================================

class Resultado(NamedTuple):
    codigo: str
    deferido: bool
    motivo: str

GradeCurricular = Dict[str, Tuple[str, ...]]
HistoricoAluno = FrozenSet[str]
SolicitacaoMatricula = Tuple[str, ...]

# ==============================================================================
# NÚCLEO DA LÓGICA (FUNÇÕES PURAS E TRANSPARENTES REFERENCIALMENTE)
# ==============================================================================

def disciplina_existe(codigo: str, grade: GradeCurricular) -> bool:
    """Verifica se a disciplina pertence à grade curricular oficial."""
    return codigo in grade

def ja_cursada(codigo: str, historico: HistoricoAluno) -> bool:
    """Verifica se a disciplina já foi concluída pelo aluno."""
    return codigo in historico

def obter_requisitos_faltantes(
    codigo: str, 
    grade: GradeCurricular, 
    historico: HistoricoAluno
) -> Tuple[str, ...]:
    """Retorna os pré-requisitos da disciplina que não constam no histórico."""
    requisitos = grade.get(codigo, ())
    return tuple(filter(lambda req: req not in historico, requisitos))

def avaliar_disciplina(
    codigo: str, 
    grade: GradeCurricular, 
    historico: HistoricoAluno, 
    processadas_anteriormente: FrozenSet[str]
) -> Resultado:
    """
    Avalia individualmente a solicitação de uma disciplina aplicando as regras
    de negócio de forma puramente declarativa.
    """
    if not disciplina_existe(codigo, grade):
        return Resultado(codigo, False, "Disciplina inexistente na Grade Curricular")
    
    if ja_cursada(codigo, historico):
        return Resultado(codigo, False, "Disciplina já cursada")
    
    if codigo in processadas_anteriormente:
        return Resultado(codigo, False, "Solicitação duplicada no mesmo semestre")
    
    faltantes = obter_requisitos_faltantes(codigo, grade, historico)
    if faltantes:
        motivo_str = f"Falta o(s) pré-requisito(s): {', '.join(faltantes)}"
        return Resultado(codigo, False, motivo_str)
    
    return Resultado(codigo, True, "Pré-requisitos satisfeitos")

def processar_solicitacao(
    solicitacao: SolicitacaoMatricula, 
    grade: GradeCurricular, 
    historico: HistoricoAluno
) -> Tuple[Resultado, ...]:
    """
    Transforma uma solicitação de matrícula em uma tupla de Resultados.
    Utiliza uma função auxiliar interna recursiva para manter a imutabilidade
    e controlar a identificação de itens duplicados na mesma solicitação.
    """
    def transformar(
        restantes: Tuple[str, ...], 
        vistos: FrozenSet[str], 
        acumulador: Tuple[Resultado, ...]
    ) -> Tuple[Resultado, ...]:
        if not restantes:
            return acumulador
        atual = restantes[0]
        res = avaliar_disciplina(atual, grade, historico, vistos)
        return transformar(restantes[1:], vistos.union({atual}), acumulador + (res,))

    return transformar(solicitacao, frozenset(), ())

def filtrar_deferidas(resultados: Tuple[Resultado, ...]) -> Tuple[Resultado, ...]:
    """Filtra puramente as solicitações deferidas."""
    return tuple(filter(lambda r: r.deferido, resultados))

def filtrar_indeferidas(resultados: Tuple[Resultado, ...]) -> Tuple[Resultado, ...]:
    """Filtra puramente as solicitações indeferidas."""
    return tuple(filter(lambda r: not r.deferido, resultados))

def gerar_relatorio_final(
    solicitacao: SolicitacaoMatricula,
    grade: GradeCurricular,
    historico: HistoricoAluno
) -> Tuple[Tuple[Resultado, ...], Tuple[Resultado, ...]]:
    """
    Composição de transformações de dados para gerar o pacote de deferimento/indeferimento.
    """
    resultados = processar_solicitacao(solicitacao, grade, historico)
    return filtrar_deferidas(resultados), filtrar_indeferidas(resultados)

# ==============================================================================
# FRONTEIRA DE EFEITOS DE CONTORNO (I/O - IMPURO)
# ==============================================================================

def formatar_linha_resultado(res: Resultado) -> str:
    """Formata uma instância de Resultado em texto."""
    if res.deferido:
        return f"  [DEFERIDO]   {res.codigo}"
    return f"  [INDEFERIDO] {res.codigo} -> Motivo: {res.motivo}"

def imprimir_resultado_teste(
    titulo: str, 
    deferidas: Tuple[Resultado, ...], 
    indeferidas: Tuple[Resultado, ...]
) -> None:
    """Função impura de I/O encarregada da exibição no terminal."""
    print(f"=== {titulo} ===")
    print("Disciplinas Deferidas:")
    if not deferidas:
        print("  (Nenhuma)")
    else:
        for d in deferidas:
            print(formatar_linha_resultado(d))
            
    print("Disciplinas Indeferidas:")
    if not indeferidas:
        print("  (Nenhuma)")
    else:
        for i in indeferidas:
            print(formatar_linha_resultado(i))
    print("-" * 60)

def executar_bateria_de_testes() -> None:
    """Executa a suite de testes validando os cenários e casos-limite do projeto."""
    grade_oficial: GradeCurricular = {
        "Algoritmos 1": (),
        "Algoritmos 2": ("Algoritmos 1",),
        "Cálculo 1": (),
        "Matemática Discreta": (),
        "Estruturas de Dados": ("Algoritmos 1", "Matemática Discreta")
    }

    # Caso 1
    d1, i1 = gerar_relatorio_final(("Algoritmos 1",), grade_oficial, frozenset())
    imprimir_resultado_teste("Caso 1: Histórico vazio, solicita Algoritmos 1", d1, i1)

    # Caso 2
    d2, i2 = gerar_relatorio_final(("Algoritmos 2",), grade_oficial, frozenset({"Algoritmos 1"}))
    imprimir_resultado_teste("Caso 2: Histórico (Algoritmos 1), solicita Algoritmos 2", d2, i2)

    # Caso 3
    d3, i3 = gerar_relatorio_final(("Algoritmos 2",), grade_oficial, frozenset())
    imprimir_resultado_teste("Caso 3: Histórico vazio, solicita Algoritmos 2", d3, i3)

    # Caso 4
    d4, i4 = gerar_relatorio_final(("Cálculo 1",), grade_oficial, frozenset({"Cálculo 1"}))
    imprimir_resultado_teste("Caso 4: Histórico (Cálculo 1), solicita Cálculo 1", d4, i4)

    # Caso 5
    d5, i5 = gerar_relatorio_final(("Estruturas de Dados",), grade_oficial, frozenset({"Matemática Discreta"}))
    imprimir_resultado_teste("Caso 5: Histórico (Matemática Discreta), solicita Estruturas de Dados", d5, i5)

    # Caso Limite: Duplicatas e Código Inexistente
    d6, i6 = gerar_relatorio_final(("Algoritmos 1", "Algoritmos 1", "Física 1"), grade_oficial, frozenset())
    imprimir_resultado_teste("Caso Limite: Duplicata na solicitação + Código Inexistente", d6, i6)

if __name__ == "__main__":
    executar_bateria_de_testes()
