'''数学通用工具'''

NormalCharacters = ['+','-','*','/','0','1','2','3','4','5','6','7','8','9']
NormalNumbers = ['0','1','2','3','4','5','6','7','8','9']
NormalExpressionCharacters = ['+','-','*','/']

def IsStrTypeNumber(text):
    try:
        int(text)
        return True
    except ValueError:
        return False

class fraction:
    '''分数类'''
    def __init__(self, numerator: int, denominator: int):
        self.numerator = numerator
        self.denominator = denominator

    def __str__(self):
        return f'{self.numerator}/{self.denominator}'

    def __add__(self, other):
        if isinstance(other, fraction):
            new_numerator = self.numerator * other.denominator + other.numerator * self.denominator
            new_denominator = self.denominator * other.denominator
            return fraction(new_numerator, new_denominator)
        return NotImplemented

    def __sub__(self, other):
        if isinstance(other, fraction):
            new_numerator = self.numerator * other.denominator - other.numerator * self.denominator
            new_denominator = self.denominator * other.denominator
            return fraction(new_numerator, new_denominator)
        return NotImplemented

    def __mul__(self, other):
        if isinstance(other, fraction):
            new_numerator = self.numerator * other.numerator
            new_denominator = self.denominator * other.denominator
            return fraction(new_numerator, new_denominator)
        return NotImplemented

    def __truediv__(self, other):
        if isinstance(other, fraction):
            new_numerator = self.numerator * other.denominator
            new_denominator = self.denominator * other.numerator
            return fraction(new_numerator, new_denominator)
        return NotImplemented
