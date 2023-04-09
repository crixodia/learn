from fractions import Fraction
from typing import List, Tuple


def ft(tuple_: Tuple[int, int]) -> Tuple[Fraction, Fraction]:
    return Fraction(tuple_[0]), Fraction(tuple_[1])


def fts(tuple_) -> Tuple[str, str]:
    return str(tuple_[0]), str(tuple_[1])


def dot(x: Tuple[Fraction, Fraction], y: Tuple[Fraction, Fraction]) -> Fraction:
    return x[0] * y[0] + x[1] * y[1]


def subt(x: Tuple[Fraction, Fraction], y: Tuple[Fraction, Fraction]) -> Tuple[Fraction, Fraction]:
    def sub(x, y): return x - y
    return tuple(map(sub, x, y))


def areastr(A, B, C, area, name):
    return f"{name} = det|[{B[0]} - {A[0]}, {C[0]} - {A[0]}], [{B[1]} - {A[1]}, {C[1]} - {A[1]}]|/2 = {abs(area)}"

def areastrf(A, B, C, area, name):
    return f"{name} = det|[{float(B[0])} - {float(A[0])}, {float(C[0])} - {float(A[0])}], [{float(B[1])} - {float(A[1])}, {float(C[1])} - {float(A[1])}]|/2 = {float(abs(area))}"

class ParametricLine:  # metaclass=Line
    __module__ == __module__

    def __init__(
            self,
            A: Tuple[Fraction, Fraction],
            B: Tuple[Fraction, Fraction],
            name: str = "L",
            param: str = "t"
    ):
        self.A = ft(A)
        self.B = ft(B)
        self.param = param
        self.name = name

    def __str__(self) -> str:
        _A = fts(self.A)
        _B = fts(self.B)
        _t = self.param
        _n = self.name
        s = f"{_n}: R -> R^2:\n\t{_t} -> {_t}{_A} + (1 - {_t}){_B}\n"
        return s.replace("'", "")

    def get_expression(self) -> str:
        _A = fts(self.A)
        _B = fts(self.B)
        _t = self.param
        return f"{_t}{_A} + (1 - {_t}){_B}"

    def get_type(self) -> str:
        return "parametric"

    def evaluate(self, t: Fraction) -> Tuple[Fraction, Fraction]:
        a, b = ft(tuple(map(lambda x: x * t, self.A))
                  ), ft(tuple(map(lambda x: x * (1 - t), self.B)))
        return a[0] + b[0], a[1] + b[1]

    def get_intersection(self, Line, verbose: bool = False) -> Tuple[Fraction, Fraction]:
        P = None
        if Line.get_type() == "parametric":

            u = subt(Line.A, Line.B)
            v = subt(self.A, self.B)

            xv = list(v[::-1])
            xv[1] = -xv[1]

            h = subt(self.B, Line.B)
            r = dot(h, xv)/dot(u, xv)

            P = Line.evaluate(r)

            output = f"\nIntersection of {self.name} and {Line.name}:\n\n" +\
                f"\tu = {fts(Line.A)} - {fts(Line.B)} = {fts(u)}\n" +\
                f"\tv = {fts(self.A)} - {fts(self.B)} = {fts(v)}\n" +\
                f"\txv = {fts(xv)}\n" +\
                f"\tr = {fts(h)}{fts(xv)}/{fts(u)}{fts(xv)} = {r}\n" +\
                f"\t{self.name}∩{Line.name} = " +\
                self.get_expression().replace(self.param, "(" + str(r) + ")") +\
                f" = {fts(P)}\n"

        elif Line.get_type() == "implicit":
            t = -dot(subt(self.B, Line.S), Line.n) / \
                dot(subt(self.A, self.B), Line.n)
            P = self.evaluate(t)

            output = f"\nIntersection of {self.name} and {Line.name}:\n\n" +\
                f"\t[{fts(self.A)}{fts(self.B)} - {fts(Line.S)}]{fts(Line.n)} = 0\n" +\
                f"\t{self.param} = {fts(subt(self.B, Line.S))}{fts(Line.n)}/" +\
                f"{fts(subt(self.A, self.A))}{fts(Line.n)}\n" +\
                f"\t{self.name}∩{Line.name} = " +\
                self.get_expression().replace(self.param, "(" + str(t) + ")") +\
                f" = {fts(P)}\n"

        if verbose:
            print(output.replace("'", ""))
        return P


class ImplicitLine:

    def __init__(
            self,
            S: Tuple[Fraction, Fraction],
            n: Tuple[Fraction, Fraction],
            name: str = "L",
            param: str = "t"
    ):
        self.S = ft(S)
        self.n = ft(n)
        self.param = param
        self.name = name

    def __str__(self) -> str:
        _S = fts(self.S)
        _N = fts(self.n)
        _n = self.name
        s = f"{_n} = " + "{" + f"[(x, y) - {_S}]{_N} = 0"+"}"
        return s.replace("'", "")

    def get_type(self) -> str:
        return "implicit"


class Barycentric:

    def __init__(
        self,
        A: Tuple[Fraction, Fraction],
        B: Tuple[Fraction, Fraction],
        C: Tuple[Fraction, Fraction],
        D: Tuple[Fraction, Fraction],
        names: List[str] = ["A", "B", "C", "D"],
    ):
        self.A = ft(A)
        self.B = ft(B)
        self.C = ft(C)
        self.D = ft(D)
        self.names = names
        self.cords = self.get_cords()

    def get_area(
            self,
            A: Tuple[Fraction, Fraction],
            B: Tuple[Fraction, Fraction],
            C: Tuple[Fraction, Fraction]
    ) -> Fraction:
        return (A[0] * B[1] + B[0] * C[1] + C[0] * A[1]) - (A[0] * C[1] + B[0] * A[1] + C[0] * B[1])

    def __str__(self) -> str:
        return f"{self.names[3]} = ({(self.cords[2])}) * {self.names[0]} + ({(self.cords[0])}) * {self.names[1]} + ({(self.cords[1])}) * {self.names[2]}"

    def get_cords(self, verbose=False) -> Tuple[Fraction, Fraction, Fraction]:
        area = self.get_area(self.A, self.B, self.C)
        if area == 0:
            return (0, 0, 0)
        else:
            alpha = self.get_area(self.C, self.A, self.D)
            beta = self.get_area(self.A, self.B, self.D)
            gamma = self.get_area(self.B, self.C, self.D)
            if verbose:
                print("Barycentric coordinates:\n")
                print("\t"+areastrf(self.A, self.B, self.C, area/2, "sA"))
                print("\t" + areastrf(self.C, self.A, self.D, alpha/2, "sa"))
                print("\t" + areastrf(self.A, self.B, self.D, beta/2, "sb"))
                print("\t" + areastrf(self.B, self.C, self.D, gamma/2, "sc"))
                print("\n\t" + str(self))
            return (
                float(alpha/area),
                float(beta/area),
                float(gamma/area)
            )


if __name__ == "__main__":
    p = ParametricLine((1, 5), (10, 7), "AB", "t")
    q = ParametricLine((5, 2), (4, 3), "CD", "r")
    p.get_intersection(q, verbose=True)

    p = ParametricLine((1, 5), (10, 7), "p", "r")
    L = ImplicitLine((0, -1), (-3, 2), "L")
    p.get_intersection(L, verbose=True)

    b = Barycentric((-0.55, 0.6), (-0.15, 0.2), (0.25, 0.6), (-0.3, 0.5))
    b.get_cords(verbose=True)
