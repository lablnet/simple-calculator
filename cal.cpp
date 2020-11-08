#include <iostream>
#include <string>
#include <cctype>

// Enums for labels.
enum Tokens {INTEGER, PLUS, MINUS, MULTIPLY, DIV,POWER, MODULE, END};
// Python None type.
const char None = '\0';

class Token {
    public:
        Tokens type;
        std::string value;

        Token(){};
        Token(Tokens type, const std::string &value) : type(type), value(value) {}

        // this function behave like magic method in php, python like __str__ , _tostring etc.
        friend std::ostream &operator<<(std::ostream &os, const Token &token) {
            os << "type: " << token.type << " value: " << token.value;
            return os;
        }
};


class Interpreter {
    private:
        Token current_token;
        char current_char;
        int position = 0;
        std::string text;
        long double  result;
    public:
        Interpreter(const std::string &text) {
            this->text = text;
            this->current_char = this->text[this->position];
        }

        // Evaluate the expression.
        Interpreter expr() {
            this->current_token = this->tokenizer();
            float result = this->terms();
            while (this->current_token.type < END) {
                if (this->current_token.type == DIV) {
                    this->compare(DIV);
                    float num = this->terms();
                    if (num == 0) Interpreter::error("Number can not divided by zero");
                    result = result / this->terms();
                }
                if (this->current_token.type == MULTIPLY) {
                    this->compare(MULTIPLY);
                    result = result * this->terms();
                }
                if (this->current_token.type == PLUS) {
                    this->compare(PLUS);
                    result = result + this->terms();
                }
                if (this->current_token.type == MINUS) {
                    this->compare(MINUS);
                    result = result - this->terms();
                }
                if (this->current_token.type == POWER) {
                    this->compare(POWER);
                    double exp = this->terms();
                    while (exp != 1) {
                        result *= result;
                        exp -= 1;
                    }
                }
                if (this->current_token.type == MODULE) {
                    this->compare(MODULE);
                    result = (int) result % (int) this->terms();
                }
            }
            this->result = result;
            return *this;
        }

        // display the output result.
        void print() {
            std::cout << this->result << std::endl;
        }
    private:

        // fire an error.
        static void error(const std::string& error="Error parsing input", bool quit = false) {
            std::cout << error;
            if (quit) exit(1);
        }

        // increment the position to get more characters.
        void handlePosition() {
            this->position += 1;
            if (this->position > (this->text.length() - 1) ) {
                // End of input
                this->current_char = None;
            } else
                this->current_char = this->text[this->position];
        }

        // skip the whitespaces.
        void skipWhitespace() {
            while (this->current_char != None && this->current_char == ' ') {
                this->handlePosition();
            }
        }

        // match the valid operator.
        bool opr() {
            char operators[4] = {'+', '-', '*', '/'};
            for (char opr : operators) {
                if (this->current_char == opr)
                    return true;
            }
            return false;
        }

        //get the number and floating point number.
        float num() {
            std::string result;
            while (this->current_char != None && isdigit(this->current_char)) {
                result += this->current_char;
                this->handlePosition();
                // floating point number
                if (this->current_char == '.') {
                    result += this->current_char;
                    this->handlePosition();
                }
            }
            return std::stof(result);
        }

        //tokenizer
        Token tokenizer() {
            while (this->current_char != None) {
                this->skipWhitespace(); // skip whitespaces.

                if (this->position == 0 && this->opr()) {
                    this->handlePosition(); //skip
                    continue;
                }
                if (this->current_char == '+') {
                    this->handlePosition();
                    return Token(PLUS, "+");
                }
                if (this->current_char == '-') {
                    this->handlePosition();
                    return Token(MINUS, "/");
                }
                if (this->current_char == '*') {
                    this->handlePosition();
                    return Token(MULTIPLY, "*");
                }
                if (this->current_char == '/') {
                    this->handlePosition();
                    return Token(DIV, "/");
                }
                if (this->current_char == '^') {
                    this->handlePosition();
                    return Token(POWER, "^");
                }
                if (this->current_char == '%') {
                    this->handlePosition();
                    return Token(MODULE, "%");
                }
                if (std::isalpha(this->current_char) || (!std::isdigit(this->current_char) && !this->opr())) {
                    this->handlePosition(); // skip
                    Interpreter::error("Alphabets or Special characters are not allowed", true);
                }
                if (isdigit(this->current_char)) {
                    return Token(INTEGER, std::to_string(this->num()));
                }
            }
            return Token(END, "");
        }

        //Compare the token.
        void compare(Tokens T) {
            if (this->current_token.type == T) {
                this->current_token = this->tokenizer();
            } else {
                Interpreter::error("Operator error", true);
            }
        }

        // get the number.
        float terms() {
            Token token = this->current_token;
            this->compare(INTEGER);
            return std::stof(token.value);
        }
};
int main()
{
    std::string input;
    while(true) {
        std::cout << "cal-> ";
        std::getline(std::cin, input);
        Interpreter inpr(input);
        inpr.expr().print();
    }
}
