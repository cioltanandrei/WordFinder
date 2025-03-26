#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LEN 100
#define MAX_TEXT_LEN 1024

char word[30];
int no_ap = 0;
pthread_mutex_t lock;

typedef struct {
    int id;
    char path[MAX_PATH_LEN];
} ThreadArgs;

void *search_word(void *vargp) {
    ThreadArgs *args = (ThreadArgs *)vargp;
    char file_text[MAX_TEXT_LEN];
    int no_ap_local = 0;
    FILE *fp = fopen(args->path, "r");

    if (fp == NULL) {
        printf("Error opening file: %s\n", args->path);
        pthread_exit(NULL);
    }

    while (fgets(file_text, sizeof(file_text), fp)) {
        char *p = file_text;
        while ((p = strstr(p, word)) != NULL) {
            no_ap_local++;
            p += strlen(word);
        }
    }

    fclose(fp);

    pthread_mutex_lock(&lock);
    no_ap += no_ap_local;
    pthread_mutex_unlock(&lock);

    free(args);
    pthread_exit(NULL);
}

int main() {
    int no_files;

    printf("No of files: ");
    scanf("%d", &no_files);

    printf("Word: ");
    scanf("%s", word);

    pthread_t tid[no_files];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < no_files; i++) {
        ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        args->id = i;
        
        printf("Enter path of file %d: ", i + 1);
        scanf("%s", args->path);

        if (pthread_create(&tid[i], NULL, search_word, args) != 0) {
            printf("Error creating thread %d\n", i);
            free(args);
        }
    }

    for (int i = 0; i < no_files; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    printf("The word '%s' appears %d times in %d files.\n", word, no_ap, no_files);

    return 0;
}
