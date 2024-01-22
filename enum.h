enum TipoQuarto {
    Simples,
    Duplo,
    Suite
};

enum Status {
    Livre,
    Ocupado,
    Reservado
};

typedef struct {
    int numero;
    float preco;
    enum TipoQuarto tipoQuarto;
    enum Status status;
} Quarto;
