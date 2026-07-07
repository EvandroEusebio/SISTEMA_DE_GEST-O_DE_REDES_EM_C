# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Output (fica na mesma pasta que o codigo, para os caminhos relativos
# como "usuarios.txt" e "data/..." funcionarem sem problemas)
TARGET = main

# Ficheiros fonte (tudo na mesma pasta)
SRCS = main.c grafo.c dispositivo.c login.c persistencia.c
OBJS = $(SRCS:.c=.o)

# Regra por omissao
all: $(TARGET)

# Ligacao
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilar .c para .o
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Limpar
clean:
	rm -f *.o $(TARGET)

# Executar
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
