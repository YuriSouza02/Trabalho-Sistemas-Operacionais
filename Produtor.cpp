#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>

const char* SHM_NAME = "MinhaMemoriaCompartilhada";
const size_t SHM_SIZE = 1024;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: ./produtor <quantidade_de_mensagens>" << std::endl;
        return 1;
    }

    int quantidadeMensagens = std::atoi(argv[1]);
    if (quantidadeMensagens <= 0) {
        std::cerr << "Erro: informe uma quantidade inteira positiva." << std::endl;
        return 1;
    }

    std::cout << "[Produtor - Linux] Criando memoria compartilhada..." << std::endl;

    std::string linux_shm_name = "/" + std::string(SHM_NAME);
    int shm_fd = shm_open(linux_shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        std::perror("Erro ao executar shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        std::perror("Erro ao executar ftruncate");
        close(shm_fd);
        return 1;
    }

    char* pBuf = (char*) mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (pBuf == MAP_FAILED) {
        std::perror("Erro ao executar mmap");
        close(shm_fd);
        return 1;
    }

    sem_t *mutex = sem_open("/shm_mutex", O_CREAT, 0666, 1);
    if (mutex == SEM_FAILED) {
        std::perror("Erro ao criar semáforo");
        munmap(pBuf, SHM_SIZE);
        close(shm_fd);
        shm_unlink(linux_shm_name.c_str());
        return 1;
    }
    
    std::cout << "Memoria compartilhada pronta!" << std::endl;

    for (int i = 1; i <= quantidadeMensagens; ++i) {
        std::string mensagem = "mensagem " + std::to_string(i);

        sem_wait(mutex);

        std::strncpy(pBuf, mensagem.c_str(), SHM_SIZE - 1);
        pBuf[SHM_SIZE - 1] = '\0';

        sem_post(mutex);

        std::cout << "[Produtor] Enviado: " << mensagem << std::endl;
        usleep(500000);
    }

    sem_close(mutex);
    sem_unlink("/shm_mutex");
    munmap(pBuf, SHM_SIZE);
    close(shm_fd);
    shm_unlink(linux_shm_name.c_str());

    std::cout << "[Produtor] Memoria liberada. Programa encerrado." << std::endl;
    return 0;
}
