# Produtor-Consumidor em C++ (IPC com Memória Compartilhada e Semáforos)

Este projeto demonstra a implementação do padrão **Produtor-Consumidor** utilizando mecanismos de **Comunicação Inter-Processos (IPC)** em sistemas operacionais compatíveis com POSIX (Linux e macOS). 

O sistema utiliza **Memória Compartilhada POSIX** (`shm_open`, `mmap`) para troca de dados em alta velocidade e **Semáforos Nomeados** (`sem_open`) para garantir exclusão mútua e evitar condições de corrida (*race conditions*).

---

## 🛠️ Tecnologias e Conceitos Utilizados

- **Linguagem:** C++ (C++11 ou superior)
- **Biblioteca POSIX:** `<sys/mman.h>`, `<fcntl.h>`, `<semaphore.h>`, `<unistd.h>`
- **Conceitos de Sistemas Operacionais:**
  - Memória Compartilhada (Shared Memory)
  - Semáforos Nomeados (Mutex Inter-processos)
  - Sincronização de Processos
  - Mapeamento de Memória Virtual (`mmap`)

---

## 📁 Estrutura do Projeto

```text
.
├── Produtor.cpp     # Código-fonte do processo produtor
├── Consumidor.cpp   # Código-fonte do processo consumidor
└── README.md        # Documentação do projeto
```

---

## ⚙️ Como Funciona

1. **Produtor (`Produtor.cpp`):**
   - Cria o segmento de memória compartilhada (`/MinhaMemoriaCompartilhada`) com tamanho de 1024 bytes.
   - Cria e inicializa um semáforo nomeado (`/shm_mutex`) com valor 1.
   - Escreve sequencialmente a quantidade de mensagens especificada via linha de comando no buffer.
   - Utiliza `sem_wait()` e `sem_post()` para proteger a escrita na memória compartilhada.
   - Desaloca e remove os recursos (`shm_unlink`, `sem_unlink`) ao finalizar.

2. **Consumidor (`Consumidor.cpp`):**
   - Conecta-se ao semáforo e à memória compartilhada criados pelo produtor.
   - Monitora ciclicamente (a cada 500ms) a memória compartilhada para verificar se há novas mensagens.
   - Garante a leitura segura bloqueando o semáforo durante o acesso ao buffer.
   - Exibe no terminal sempre que detecta uma alteração no conteúdo.

---

## 🚀 Como Compilar e Executar

### Pré-requisitos
- Compilador C++ (`g++` ou `clang++`).
- Sistema Operacional Linux ou macOS.

### 1. Compilação

No terminal, execute os comandos abaixo para compilar ambos os programas:

**No Linux:**
```bash
g++ Produtor.cpp -o produtor -pthread -lrt
g++ Consumidor.cpp -o consumidor -pthread -lrt
```
*> Nota: As flags `-pthread` e `-lrt` são necessárias no Linux para vincular as bibliotecas de semáforos e memória compartilhada POSIX.*

**No macOS:**
```bash
g++ Produtor.cpp -o produtor
g++ Consumidor.cpp -o consumidor
```

---

### 2. Execução

Para visualizar a comunicação em tempo real, abra **dois terminais distintos**.

#### **Passo 1:** Iniciar o Produtor (Terminal 1)
O produtor exige como parâmetro a quantidade de mensagens que deve gerar.

```bash
./produtor 10
```

#### **Passo 2:** Iniciar o Consumidor (Terminal 2)
Em outro terminal, execute o consumidor para acompanhar as leituras:

```bash
./consumidor
```

---

## 📌 Exemplo de Saída

**Terminal 1 (Produtor):**
```text
[Produtor - Linux] Criando memoria compartilhada...
Memoria compartilhada pronta!
[Produtor] Enviado: mensagem 1
[Produtor] Enviado: mensagem 2
[Produtor] Enviado: mensagem 3
...
[Produtor] Memoria liberada. Programa encerrado.
```

**Terminal 2 (Consumidor):**
```text
[Consumidor - Linux/macOS] Conectando a memoria compartilhada...
[Consumidor] Lendo a memoria em tempo real (Pressione CTRL+C para sair)...
--> Nova mensagem detectada: mensagem 1
--> Nova mensagem detectada: mensagem 2
--> Nova mensagem detectada: mensagem 3
```

---

## ⚠️ Observações Importantes

- O **Produtor** deve ser iniciado **antes** do Consumidor, pois é ele quem aloca a memória compartilhada e inicializa o semáforo.
- Se o produtor for interrompido de forma abrupta (ex: `SIGKILL` ou crash) antes de chamar `shm_unlink` e `sem_unlink`, os recursos podem permanecer no SO. Nesses casos, pode ser necessário removê-los manualmente em `/dev/shm` (no Linux).
