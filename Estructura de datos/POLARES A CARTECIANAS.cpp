#include <stdio.h>
#include <math.h>

struct polar {
    float a, b;
};

struct rectangulo {
    float x, y;
};

int main() {
    struct polar P;
    struct rectangulo R;

    printf("Ingresa una magnitud : ");
    scanf("%f",&P.a);

    printf("Ingresa su angulo en grados : ");
    scanf("%f",&P.b);

    float radianes = P.b * 3.1416 / 180.0;
    R.x = P.a * cos(radianes);
    R.y = P.a * sin(radianes);

    printf("Los resultados son %.2f, %.2f\n", R.x, R.y);

    return 0;
}
