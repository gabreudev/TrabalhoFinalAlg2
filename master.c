#include "enum.h"

int main() {
    

    int opcao;

    do {
        printf("Menu:\n");
        printf("1 - Opcoes de Quarto\n");
        printf("2 - LOpcoes do Cliente\n");
        printf("3 - Opcoes de reserva\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: 
               menuQuarto();
               break;
            case 2:
               menuCliente();
                break;
            case 3: 
                menuReserva();
                break;
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);
 
    return 0;
}