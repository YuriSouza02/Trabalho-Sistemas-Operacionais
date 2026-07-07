# Produtor-Consumidor com Memória Compartilhada

Implementação clássica do problema Produtor-Consumidor usando **memória compartilhada** para sincronização entre processos em C++.

## Objetivo

Demonstrar comunicação inter-processos (IPC) com compartilhamento seguro de dados através de memória compartilhada, com suporte para **Windows e Linux**.

## Requisitos

- C++11 ou superior
- GCC/G++ (Linux) ou MSVC (Windows)

## Como Usar

### Compilação
```bash
g++ -o produtor Produtor.cpp
g++ -o consumidor Consumidor.cpp
```

### Execução
```bash
./produtor 100  # Terminal 1
./consumidor  # Terminal 2
```

## Estrutura

| Arquivo | Descrição |
|---------|-----------|
| `Produtor.cpp` | Cria memória compartilhada e escreve dados |
| `Consumidor.cpp` | Lê dados da memória compartilhada |

## Notas

- Execute o **Produtor** antes do Consumidor
- Funciona em Windows e Linux
- Tamanho da memória: 1024 bytes