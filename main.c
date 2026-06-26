#include "Grafo.h"
#include "Dispositivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    Grafo *rede = criarGrafo();
    menu_dispositivo(rede);

    liberarGrafo(rede);
    return 0;
}
