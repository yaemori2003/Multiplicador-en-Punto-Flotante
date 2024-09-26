#include <iostream>
#include <iomanip>
#include <bitset>
#include <cmath>

using namespace std;

void extraerComponentes(float valor, int& signo, int& exponente, int& fraccion) {
    unsigned int bits = *(unsigned int*)&valor;
    signo = (bits >> 31) & 1;
    exponente = (bits >> 23) & 0xFF;
    fraccion = bits & 0x7FFFFF;
}

float combinarComponentes(int signo, int exponente, int fraccion) {
    unsigned int bits = (signo << 31) | (exponente << 23) | (fraccion & 0x7FFFFF);
    return *(float*)&bits;
}

unsigned long long ajustarRedondeo(unsigned long long fraccion) {
    if ((fraccion & 1) && ((fraccion >> 1) & 1)) {
        fraccion++;
    }
    return fraccion;
}

bool esExponenteDemasiadoGrande(int exponenteRes) {
    return exponenteRes >= 255;
}

bool esExponenteDemasiadoPequeno(int exponenteRes) {
    return exponenteRes <= 0;
}

float multiplicarFlotantes(float x, float y) {
    int signoX, expX, fracX;
    int signoY, expY, fracY;

    extraerComponentes(x, signoX, expX, fracX);
    extraerComponentes(y, signoY, expY, fracY);

    if (x == 0.0f || y == 0.0f) {
        return 0.0f;
    }

    int expRes = expX + expY - 127;

    if (esExponenteDemasiadoGrande(expRes)) {
        return INFINITY;
    }

    if (esExponenteDemasiadoPequeno(expRes)) {
        return 0.0f;
    }

    unsigned long long fracRes = (static_cast<unsigned long long>(fracX | 0x800000)) * (fracY | 0x800000);

    if (fracRes & (1ULL << 47)) {
        fracRes >>= 24;
        expRes++;
    }
    else {
        fracRes >>= 23;
    }

    fracRes = ajustarRedondeo(fracRes);
    fracRes &= 0x7FFFFF;

    int signoRes = signoX ^ signoY;
    return combinarComponentes(signoRes, expRes, fracRes);
}

void imprimirBits(float valor) {
    unsigned int bits = *(unsigned int*)&valor;
    cout << bitset<32>(bits) << endl;
}

int main() {
    float num1, num2;
    cout << "Introduce dos numeros en punto flotante: ";
    cin >> num1 >> num2;

    cout << "Bits de num1: ";
    imprimirBits(num1);
    cout << "Bits de num2: ";
    imprimirBits(num2);

    float resultadoManual = multiplicarFlotantes(num1, num2);
    cout << "Resultado (Implementacion personalizada) en base 10: " << resultadoManual << endl;
    cout << "Bits del resultado (Implementacion personalizada): ";
    imprimirBits(resultadoManual);

    float resultadoNativo = num1 * num2;
    cout << "Resultado (C++ nativo) en base 10: " << resultadoNativo << endl;
    cout << "Bits del resultado (C++ nativo): ";
    imprimirBits(resultadoNativo);

    float diferencia = fabs(resultadoManual - resultadoNativo);
    cout << fixed << setprecision(10);
    cout << "Diferencia entre resultados: " << diferencia << endl;

    return 0;
}
