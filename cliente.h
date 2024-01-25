#include <stdio.h>
#include "enum.h"

void atualizarCliente(FILE *file);

int apagarClientePorCpf(FILE *file, char cpf[]);

Cliente obterNovoCliente();

void cadastrarCliente(FILE *file, Cliente cliente);

void listarClientes(FILE *file);
