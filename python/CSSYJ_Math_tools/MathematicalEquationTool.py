'''数学方程工具'''

from GeneralMathematicalTool import *


def SimpleExpressionCalculation_Str(expression: list):
    '''简单数学表达式运算'''
    if IsStrTypeNumber(expression[0]):
        result = int(expression[0])
    else:
        result = 0
    OmitCharacterCount = 0
    ReadCharacter_Position = -1
    
    for ReadCharacter in expression:
        ReadCharacter_Position += 1
        if OmitCharacterCount > 0:
            OmitCharacterCount -= 1
            continue
        
        if IsStrTypeNumber(ReadCharacter):
            continue
        elif ReadCharacter in NormalExpressionCharacters:
            if ReadCharacter == '+':
                OmitCharacterCount += 1
                result = result + int(expression[ReadCharacter_Position + OmitCharacterCount])
            if ReadCharacter == '-':
                OmitCharacterCount += 1
                result = result - int(expression[ReadCharacter_Position + OmitCharacterCount])
    
    print(fr'SimpleResult:{result}')
    return str(result)



def ExpressionParsing(expression: str):
    '''数学表达式解析'''

    ParsedExpression: list = []
    OmitCharacterCount: int = 0
    ReadCharacter_Position: int = -1
    print(fr'expression:{expression}')

    for ReadCharacter in expression:
        ReadCharacter_Position += 1
        if OmitCharacterCount > 0:
            OmitCharacterCount -= 1
            continue
        if ReadCharacter == ' ':
            continue

        if ReadCharacter in NormalCharacters:
            if ReadCharacter in NormalNumbers:
                ReadNumbers = ''
                ReadNumbersCharacter = ''
                BeyondList = False
                while not BeyondList:
                    try:
                        ReadNumbersCharacter = expression[ReadCharacter_Position + OmitCharacterCount]
                        ReadNumbers += ReadNumbersCharacter
                    except IndexError:
                        BeyondList = True
                    try:
                        if not(expression[ReadCharacter_Position + OmitCharacterCount + 1] in NormalNumbers):
                            break
                    except IndexError:
                        pass
                    OmitCharacterCount += 1
                ParsedExpression.append(ReadNumbers)
            else:
                ParsedExpression.append(ReadCharacter)

        elif ReadCharacter == '(' or ReadCharacter == '[' or ReadCharacter == '{':
            if ReadCharacter == '(':
                OmitCharacterCount = 1
                ExpressionInsideBrackets = []
                ReadCharacterInsideBrackets = ''

                while ReadCharacterInsideBrackets != ')':
                    ReadCharacterInsideBrackets = expression[ReadCharacter_Position + OmitCharacterCount]
                    ExpressionInsideBrackets.append(ReadCharacterInsideBrackets)
                    OmitCharacterCount += 1
                del ExpressionInsideBrackets[-1]
                OmitCharacterCount -= 1
                
                SimpleResult = SimpleExpressionCalculation_Str(ExpressionParsing(ExpressionInsideBrackets))
                ParsedExpression.append(SimpleResult)
            
            elif ReadCharacter == '[':
                OmitCharacterCount = 1
                ExpressionInsideBrackets = []
                ReadCharacterInsideBrackets = ''

                while ReadCharacterInsideBrackets != ']':
                    ReadCharacterInsideBrackets = expression[ReadCharacter_Position + OmitCharacterCount]
                    ExpressionInsideBrackets.append(ReadCharacterInsideBrackets)
                    OmitCharacterCount += 1
                del ExpressionInsideBrackets[-1]
                OmitCharacterCount -= 1

                SimpleResult = SimpleExpressionCalculation_Str(ExpressionParsing(ExpressionInsideBrackets))
                ParsedExpression.append(SimpleResult)
                
            elif ReadCharacter == '{':
                OmitCharacterCount = 1
                ExpressionInsideBrackets = []
                ReadCharacterInsideBrackets = ''

                while ReadCharacterInsideBrackets != '}':
                    ReadCharacterInsideBrackets = expression[ReadCharacter_Position + OmitCharacterCount]
                    ExpressionInsideBrackets.append(ReadCharacterInsideBrackets)
                    OmitCharacterCount += 1
                OmitCharacterCount -= 1
                del ExpressionInsideBrackets[-1]
                
                SimpleResult = SimpleExpressionCalculation_Str(ExpressionParsing(ExpressionInsideBrackets))
                ParsedExpression.append(SimpleResult)

    print(fr'ParsedExpression:{ParsedExpression}')
    return ParsedExpression


def ExpressionCalculation(expression):
    '''数学表达式运算'''
    ParsedExpression = ExpressionParsing(expression)
    result = SimpleExpressionCalculation_Str(ParsedExpression)
    return result
    

# 
# print(fr'result:{ExpressionCalculation('111+11+22-(-11-66+43)')}')
print(fr'result:{ExpressionCalculation('[(11-9)+7-(11+1)]-{[111-(11+77)]+1}')}')
# print(fr'result:{ExpressionCalculation('23+22+12')}')