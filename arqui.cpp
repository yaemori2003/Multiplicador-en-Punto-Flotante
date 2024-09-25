#include <iostream>
#include <bitset>
#include <cmath>

using namespace std;

void mostrarIEEE754(float numero);
struct Flotante32;
Flotante32 dividir(float num);
float juntar(const Flotante32& num);

struct Flotante32 {
    int signo;
    int exponente;
    unsigned int mantisa;
};

void imprimirLinea() {
    cout << "+---------------------------------------------+" << endl;
}

int main() {
    float x, y;
    cout << "Ingrese dos numeros en punto flotante: ";
    cin >> x >> y;

    imprimirLinea();
    cout << "|           Proceso de Calculo Iniciado       |" << endl;
    imprimirLinea();

    if (x == 0 || y == 0) {
        imprimirLinea();
        cout << "| Resultado: 0                               |" << endl;
        imprimirLinea();
        return 0;
    }

    Flotante32 numX = dividir(x);
    Flotante32 numY = dividir(y);

    int exponenteResultado = (numX.exponente + numY.exponente - 127);
    unsigned long long mantisaResultado = (unsigned long long)numX.mantisa * numY.mantisa;

    if (mantisaResultado & (1LL << 47)) {
        mantisaResultado >>= 1;
        exponenteResultado++;
    }

    if (exponenteResultado >= 255) {
        imprimirLinea();
        cout << "|            Overflow en exponente            |" << endl;
        imprimirLinea();
        return 0;
    }
    else if (exponenteResultado <= 0) {
        imprimirLinea();
        cout << "|            Underflow en exponente           |" << endl;
        imprimirLinea();
        return 0;
    }

    mantisaResultado >>= 24;

    Flotante32 resultado;
    resultado.signo = numX.signo ^ numY.signo;
    resultado.exponente = exponenteResultado;
    resultado.mantisa = mantisaResultado;

    float finalResult = juntar(resultado);

    imprimirLinea();
    cout << "|              Resultado Final               |" << endl;
    imprimirLinea();

    cout << "| Resultado final: " << finalResult << endl;
    imprimirLinea();

    mostrarIEEE754(finalResult);

    return 0;
}

void mostrarIEEE754(float numero) {
    unsigned int bits = *reinterpret_cast<unsigned int*>(&numero);
    imprimirLinea();
    cout << "| El numero decimal " << numero << " en formato IEEE 754 |" << endl;
    cout << "| de 32 bits es:                                   |" << endl;
    imprimirLinea();
    cout << "| Representacion binaria: " << bitset<32>(bits) << " |" << endl;
    imprimirLinea();
}

Flotante32 dividir(float num) {
    Flotante32 result;
    unsigned int bits = *reinterpret_cast<unsigned int*>(&num);

    result.signo = (bits >> 31) & 1;
    result.exponente = (bits >> 23) & 0xFF;
    result.mantisa = (bits & 0x7FFFFF) | (1 << 23);

    imprimirLinea();
    cout << "|          Descomposicion de " << num << "           |" << endl;
    imprimirLinea();

    cout << "| Numero: " << num << endl;
    cout << "| Signo: " << result.signo << endl;
    cout << "| Exponente: " << result.exponente << endl;
    cout << "| Mantisa: " << bitset<24>(result.mantisa) << " (en binario) |" << endl;
    imprimirLinea();

    return result;
}

float juntar(const Flotante32& num) {
    unsigned int resultado = 0;
    unsigned int mantisa = num.mantisa & 0x7FFFFF;
    resultado |= (num.signo << 31);
    resultado |= (num.exponente << 23);
    resultado |= mantisa;

    return *reinterpret_cast<float*>(&resultado);
}
