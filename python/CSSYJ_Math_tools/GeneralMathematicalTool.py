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
