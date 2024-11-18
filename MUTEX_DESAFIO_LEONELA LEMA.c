#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_COPIES 5  // Número máximo de copias que cada niño puede hacer
#define MAX_CONSECUTIVE_COPIES 3 // Número máximo de copias consecutivas por niño

pthread_mutex_t photocopier_mutex; // Mutex para la fotocopiadora
int copies_consecutive[2] = {0, 0}; // Array para contar las copias consecutivas de cada niño (0 -> Juan, 1 -> Ana)

void* child(void* arg) {
    char* child_name = (char*) arg; // Nombre del niño
    int child_index = (child_name[0] == 'J') ? 0 : 1; // 0 para Juan, 1 para Ana
    for (int i = 0; i < MAX_COPIES; i++) {
        printf("%s: esperando para usar la fotocopiadora...\n", child_name);
        
        // Intentar usar la fotocopiadora
        pthread_mutex_lock(&photocopier_mutex); // Toma el candado

        // Si el niño ya hizo 3 copias consecutivas, libera el mutex para que el otro niño pueda usar la fotocopiadora
        if (copies_consecutive[child_index] == MAX_CONSECUTIVE_COPIES) {
            printf("%s: alcanzó el límite de 3 copias consecutivas. Liberando fotocopiadora para el otro niño.\n", child_name);
            pthread_mutex_unlock(&photocopier_mutex); // Libera el candado temporalmente
            sleep(1); // Simula el tiempo de espera para que el otro niño haga sus copias
            pthread_mutex_lock(&photocopier_mutex); // Vuelve a tomar el candado
            copies_consecutive[child_index] = 0; // Reinicia el contador de copias consecutivas
        }

        printf("%s: usando la fotocopiadora para copia %d\n", child_name, i + 1);
        
        // Simula el tiempo que toma hacer copias
        sleep(1);
        
        printf("%s: terminó la copia %d\n", child_name, i + 1);
        copies_consecutive[child_index]++; // Incrementa el contador de copias consecutivas
        pthread_mutex_unlock(&photocopier_mutex); // Libera el candado
        
        // Simula el tiempo antes de hacer la próxima copia
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t juan, ana; // Dos hilos para Juan y Ana

    pthread_mutex_init(&photocopier_mutex, NULL); // Inicializa el mutex

    // Crea dos hilos (Juan y Ana)
    pthread_create(&juan, NULL, child, "Juan");
    pthread_create(&ana, NULL, child, "Ana");

    // Espera a que ambos hilos terminen
    pthread_join(juan, NULL);
    pthread_join(ana, NULL);

    pthread_mutex_destroy(&photocopier_mutex); // Destruye el mutex

    return 0; // Finaliza el programa
}
