INTEGER, PLUS, MINUS, MULTIPLY, DIV, EOF = 'INTEGER', 'PLUS', 'MINUS', 'MULTIPLY', 'DIV', 'EOF'


class Token():
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return 'Token({type}, {value})'.format(
            type=self.type,
            value=repr(self.value)
        )

    def __repr__(self):
        return self.__str__()


class Interpreter():

    def __init__(self, text):
        self.text = text
        self.position = 0
        self.current_token = None
        self.current_char = self.text[self.position]

    def error(self, error="Error parsing input"):
        raise Exception(error)

    def handlePosition(self):
        self.position += 1
        if self.position > len(self.text) - 1:
            # Indicates end of input
            self.current_char = None
        else:
            self.current_char = self.text[self.position]

    def skip_whitespace(self):
        while self.current_char is not None and self.current_char.isspace():
            self.handlePosition()

    def skip(self):
        self.handlePosition()

    def _alpha(self):
        chars = tuple((chr(i) for i in range(ord('a'), ord('z') + 1))) + tuple((chr(i) for i in range(ord('A'), ord('Z') + 1)))
        if self.current_char in chars:
            return True
        else:
            return False

    def _opr(self):
        if self.current_char in ("+", "-", "/", "*"):
            return True
        return False

    def _num(self):
        result = ""
        while self.current_char is not None and self.current_char.isdigit():
            result += self.current_char
            self.handlePosition()
            # Enable floating point number.
            if self.current_char == '.':
                result += self.current_char
                self.handlePosition()
        return float(result)

    def tokenizer(self):
        while self.current_char is not None:
            if self.position == 0 and self._opr():
                self.skip()
                continue
            if self._alpha():
                print("Ap")
                self.skip()
                continue
            if self.current_char.isspace():
                self.skip_whitespace()
                continue
            if self.current_char.isdigit():
                return Token(INTEGER, self._num())
            if self.current_char == '+':
                self.handlePosition()
                return Token(PLUS, '+')

            if self.current_char == '-':
                self.handlePosition()
                return Token(MINUS, '-')
            if self.current_char == '*':
                self.handlePosition()
                return Token(MULTIPLY, "*")
            if self.current_char == '/':
                self.handlePosition()
                return Token(DIV, "/")
            self.error()
        return Token(EOF, None)

    def compare(self, type):
        if self.current_token.type == type :
            self.current_token = self.tokenizer()
        else:
            self.error("Operator Error")

    def term(self):
        token = self.current_token
        self.compare(INTEGER)
        return token.value

    def expr(self):
        self.current_token = self.tokenizer()
        result = self.term()
        while self.current_token.type in (PLUS, MINUS, MULTIPLY, DIV):
            if self.position == 1:
                if self._opr():
                    continue
            token = self.current_token
            if token.type == PLUS:
                self.compare(PLUS)
                result = result + self.term()
            elif token.type == MINUS:
                self.compare(MINUS)
                result = result - self.term()
            elif token.type == DIV:
                self.compare(DIV)
                result = result / self.term()
            else:
                self.compare(MULTIPLY)
                result = result * self.term()
        return result


def main():
    while True:
        try:
            text = input('cal-> ')
        except EOFError:
            break
        if not text:
            continue
        interpreter = Interpreter(text)
        result = interpreter.expr()
        # print(interpreter.tokenizer())
        print(result)


if __name__ == "__main__":
    main()
