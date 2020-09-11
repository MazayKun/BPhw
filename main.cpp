#include <iostream>
#include <functional>
#include <cmath>

// Консольные сообщения
const std::string MAIN_MESSAGE = std::string("Choose a config option:\n\t1 - Set objective function\n") +
                                 "\t2 - Set integration method\n\t3 - Set integration step & range\n" +
                                 "\t4 - Calculate result\n\t5 - Quit\n";
const std::string FUNCTION_SELECTION_MESSAGE = std::string("\t1 - sin\n") + "\t2 - cos\n" + "\t3 - x\n" + "\t4 - x^2\n";
const std::string METHOD_SELECTION_MESSAGE = std::string("\t1 - trapezoidal\n\t2 - rectangle\n");
const std::string RANGE_MESSAGE = std::string("Enter the start and the end of the integration interval:\n");
const std::string STEP_MESSAGE = std::string("Enter the step of the integration:\n");
const std::string ANSWER_MESSAGE = std::string("The result of integration with such parameters is\t");
const std::string WRONG_COMMAND_MESSAGE = std::string("Wrong command!\n");
const std::string END_MESSAGE = std::string("Bye!\n");


// Функция интегрирования методом прямоугольников
double rectangleIntegral(double start, double end, double step, std::function<double (double)> f) {
    if (end - start < step) {
        step = end - start;
    }

    double answer = 0;
    double currStart = start;
    while(currStart + step <= end) {
        answer += f(currStart + step/2) * step;
        currStart += step;
    }
    if(currStart < end) {
        answer += f((end - currStart)/2) * (end - currStart);
    }
    return answer;
}

// Функция интегрирования методом трапеции
double trapezoidalIntegral(double start, double end, double step, std::function<double (double)> f) {
    if (end - start < step) {
        step = end - start;
    }

    double answer = 0;
    double currStart = start;
    while(currStart + step <= end) {
        answer += 0.5 * step * (f(currStart) + f(currStart + step));
        currStart += step;
    }
    if(currStart < end) {
        answer += 0.5 * (end - currStart) * (f(currStart) + f(end));
    }
    return answer;
}

// Исключение неправильно введенного кода функции
class ObjFuncException: public std::exception {
private:
    std::string m_error;
public:
    ObjFuncException(){
        m_error = "Bad objective function code!\n";
    }

    const char* what() const noexcept { return m_error.c_str(); }
};

// Исключение неправильно введенного кода метода
class IntegrationMethodException: public std::exception {
private:
    std::string m_error;
public:
    IntegrationMethodException(){
        m_error = "Bad method function code!\n";
    }

    const char* what() const noexcept { return m_error.c_str(); }
};

// Исключение неполного набора параметров
class IncompleteConfigurationException: public std::exception {
private:
    std::string m_error;
public:
    IncompleteConfigurationException(std::string parametrName){
        m_error = std::string("Not all parameters are specified(") + parametrName+  ")!\n";
    }

    const char* what() const noexcept { return m_error.c_str(); }
};

// Класс конфигурации и запуска вычисления интеграла функции
class Integrator {
public:

    // Задает фунцию для интегрирования по ее коду
    void setObjectiveFunc(int type) {
        switch (type) {
            case 1: {
                objectiveFunc = sin;
                break;
            }
            case 2: {
                objectiveFunc = cos;
                break;
            }
            case 3: {
                objectiveFunc = [](double x){return x;};
                break;
            }
            case 4: {
                objectiveFunc = [](double x){return pow(x, 2);};
                break;
            }
            default:{
               throw ObjFuncException();
            }
        }
    }

    // Задает метод интегрирования по его коду
    void setMethod(int method) {
        switch(method) {
            case 1: {
                this->methodFunction = trapezoidalIntegral;
                break;
            }
            case 2: {
                this->methodFunction = rectangleIntegral;
                break;
            }
            default:{
                throw IntegrationMethodException();
            }
        }
    }

    // Задает шаг интегрирования
    void setStep(double step) {
        this->step = step;
    }

    // Задает интервал интегрирования
    void setRange(double start, double end) {
        if(start > end) {
            double tmp = start;
            start = end;
            end = tmp;
        }
        this->start = start;
        this->end = end;
    }

    // Функция вычисления интеграла с заданными конфигурациями
    double calculate() {
        if (objectiveFunc == NULL){
            throw IncompleteConfigurationException("objective function");
        }
        if (methodFunction == NULL){
            throw IncompleteConfigurationException("method");
        }
        if (step == 0){
            throw IncompleteConfigurationException("step");
        }
        if (start == end) {
            return 0;
        }
        if(step > end - start) {
            step = end - start;
        }
        return methodFunction(start, end, step, objectiveFunc);
    }
private:
    std::function<double(double)> objectiveFunc = NULL;
    std::function<double(double, double, double, std::function<double(double)>)> methodFunction = NULL;
    double step = NULL;
    double start = 0;
    double end = 0;
};

int main(int argc, char *argv[]) {
    std::cout << (0.0 == NULL);
    int ch;
    Integrator integrator = Integrator();
    while(true) {
        std::cout << MAIN_MESSAGE;
        std::cin >> ch;
        switch (ch) {
            case 1:{
                std::cout << FUNCTION_SELECTION_MESSAGE ;
                std::cin >> ch;
                try {
                    integrator.setObjectiveFunc(ch);
                }catch(const ObjFuncException e) {
                    std::cerr << e.what();
                }

                break;
            }
            case 2:{
                std::cout << METHOD_SELECTION_MESSAGE ;
                std::cin >> ch;
                try {
                    std::cout << ch;
                    integrator.setMethod(ch);
                }catch(const IntegrationMethodException e) {
                    std::cerr << e.what();
                }
                break;
            }
            case 3:{
                double start, end, step;
                std::cout << RANGE_MESSAGE ;
                std::cin >> start >> end;
                integrator.setRange(start, end);
                std::cout << STEP_MESSAGE ;
                std::cin >> step;
                integrator.setStep(step);
                break;
            }
            case 4:{
                double answer;
                try {
                    answer = integrator.calculate();
                    std::cout << ANSWER_MESSAGE << answer << std::endl;
                }catch(const IncompleteConfigurationException e) {
                    std::cerr << e.what();
                }

                break;
            }
            case 5:{
                std::cout << END_MESSAGE;
                return 0;
            }
            default:{
                std::cout << WRONG_COMMAND_MESSAGE;
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
