#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

#include <iostream>
#include <cstring>
#include <string>

const char *SHM_NAME = "MinhaMemoriaCompartilhada";
const size_t SHM_SIZE = 1024;

int main()
{
    std::cout << "[Consumidor - Linux/macOS] Conectando a memoria compartilhada..." << std::endl;

    std::string linux_shm_name = "/" + std::string(SHM_NAME);

    sem_t *mutex = sem_open("/shm_mutex", 0);
    if (mutex == SEM_FAILED) {
        std::perror("Erro ao abrir semáforo (Produtor esta rodando?)");
        return 1;
    }

    int shm_fd = shm_open(linux_shm_name.c_str(), O_RDONLY, 00666);
    if (shm_fd == -1)
    {
        std::perror("Erro ao executar shm_open (O Produtor esta rodando?)");
        sem_close(mutex);
        return 1;
    }

    char *pBuf = (char *)mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (pBuf == MAP_FAILED)
    {
        std::perror("Erro ao executar mmap no Consumidor");
        close(shm_fd);
        sem_close(mutex);
        return 1;
    }

    std::cout << "[Consumidor] Lendo a memoria em tempo real (Pressione CTRL+C para sair)..." << std::endl;
    std::string ultima_mensagem = "";

    while (true)
    {
        sem_wait(mutex);
        
        std::string conteudo_atual = pBuf;
        
        sem_post(mutex);
        
        if (conteudo_atual != ultima_mensagem)
        {
            std::cout << "--> Nova mensagem detectada: " << conteudo_atual << std::endl;
            ultima_mensagem = conteudo_atual;
        }
        usleep(500000); 
    }

    munmap(pBuf, SHM_SIZE);
    close(shm_fd);
    sem_close(mutex);
    return 0;
}