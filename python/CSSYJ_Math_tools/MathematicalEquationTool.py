NormalExpressionCharacters = ['+','-','*','/','0','1','2','3','4','5','6','7','8','9']
NormalNumbers = ['0','1','2','3','4','5','6','7','8','9']
ExpressionCharacters = ['+','-','*','/']


def ExpressionParsing(expression):
    '''数学表达式解析'''

    ParsedExpression = []
    OmitCharacterCount = 0
    ReadCharacter_Position = -1

    for ReadCharacter in expression:
        ReadCharacter_Position += 1
        if OmitCharacterCount > 0:
            OmitCharacterCount -= 1
            continue

        if ReadCharacter in NormalExpressionCharacters:
            if ReadCharacter in NormalNumbers:
                ReadNumbers = ''
                ReadNumbersCharacter = ''
                BeyondList = False
                while not BeyondList:
                    try:
                        ReadNumbersCharacter = expression[ReadCharacter_Position + OmitCharacterCount]
                    except IndexError:
                        BeyondList = True
                    ReadNumbers += ReadNumbersCharacter
                    try:
                        if not(expression[ReadCharacter_Position + OmitCharacterCount + 1] in NormalNumbers):
                            break
                    except IndexError:
                        pass
                    OmitCharacterCount += 1
                    print(ReadNumbers)
                ParsedExpression.append(ReadNumbers)
            else:
                ParsedExpression.append(ReadCharacter)

        elif ReadCharacter == '{':
            OmitCharacterCount = 1
            ExpressionInsideBrackets = []
            ReadCharacterInsideBrackets = ''

            while ReadCharacterInsideBrackets != '}':
                ReadCharacterInsideBrackets = expression[ReadCharacter_Position + OmitCharacterCount]
                ExpressionInsideBrackets.append(ReadCharacterInsideBrackets)
                OmitCharacterCount += 1
            OmitCharacterCount += 1

            if ParsedExpression[-1] == '+':
                for AddedCharacter in ExpressionParsing(ExpressionInsideBrackets):
                    ParsedExpression.append(AddedCharacter)
            elif ParsedExpression[-1] == '-':
                for AddedCharacter in ExpressionParsing(ExpressionInsideBrackets):
                    if AddedCharacter == '+':
                        ParsedExpression.append('-')
                    elif AddedCharacter == '-':
                        ParsedExpression.append('+')
                    else:
                        ParsedExpression.append(AddedCharacter)
        
        elif ReadCharacter == '[':
            print('[]')
            OmitCharacterCount = 1
            ExpressionInsideBrackets = []
            ReadCharacterInsideBrackets = ''

            while ReadCharacterInsideBrackets != ']':
                ReadCharacterInsideBrackets = expression[ReadCharacter_Position + OmitCharacterCount]
                ExpressionInsideBrackets.append(ReadCharacterInsideBrackets)
                OmitCharacterCount += 1
            OmitCharacterCount += 1

            if ParsedExpression[-1] == '+':
                for AddedCharacter in ExpressionParsing(ExpressionInsideBrackets):
                    ParsedExpression.append(AddedCharacter)
            elif ParsedExpression[-1] == '-':
                for AddedCharacter in ExpressionParsing(ExpressionInsideBrackets):
                    if AddedCharacter == '+':
                        ParsedExpression.append('-')
                    elif AddedCharacter == '-':
                        ParsedExpression.append('+')
                    else:
                        ParsedExpression.append(AddedCharacter)
            
        elif ReadCharacter == '(':
            OmitCharacterCount = 1
            ExpressionInsideBrackets = []
            ReadCharacterInsideBrackets = ''

            while ReadCharacterInsideBrackets != ')':
                ReadCharacterInsideBrackets = expression[ReadCharacter_Position + OmitCharacterCount]
                ExpressionInsideBrackets.append(ReadCharacterInsideBrackets)
                OmitCharacterCount += 1
            OmitCharacterCount += 1

            if ParsedExpression[-1] == '+':
                for AddedCharacter in ExpressionParsing(ExpressionInsideBrackets):
                    ParsedExpression.append(AddedCharacter)
            elif ParsedExpression[-1] == '-':
                for AddedCharacter in ExpressionParsing(ExpressionInsideBrackets):
                    if AddedCharacter == '+':
                        ParsedExpression.append('-')
                    elif AddedCharacter == '-':
                        ParsedExpression.append('+')
                    else:
                        ParsedExpression.append(AddedCharacter)
        
    return ParsedExpression


def ExpressionCalculation(expression):
    '''数学表达式运算'''
    ParsedExpression = ExpressionParsing(expression)
    result = 0
    return ParsedExpression
    


print(fr'result:{ExpressionCalculation('111+11+22-(-11-66+43)')}')