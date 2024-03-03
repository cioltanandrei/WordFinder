#include <pthread.h>
#include <stdio.h>
#include <string.h>

char word[30];
int no_ap = 0;

void *search_word(void *vargp) {

    int *myid = (int *) vargp;
    char path[100] = "";
    char file_text[1024];
    int no_ap_local = 0;
    FILE *fp;


    printf("Enter path of file: ", *myid);
    scanf("%s", path);

    fp = fopen(path, "r");

    if (fp == NULL) {
        printf("Error opening file\n");
    }

    while (fgets(file_text, sizeof(file_text), fp)) {
        char *p = file_text;
        while ((p = strstr(p, word)) != NULL) {
            no_ap_local++;
            p += strlen(word);
        }
    }

    no_ap += no_ap_local;
    pthread_exit(NULL);
}

int main() {
    int no_files;
    printf("No of files: ");
    scanf("%i", &no_files);

    printf("\n");
    printf("Word: ");
    scanf("%s", word);

    pthread_t tid[no_files];

    for (int i = 0; i < no_files; i++)
        pthread_create(&tid[i], NULL, &search_word, &i);

    for (int i = 0; i < no_files; i++)
        pthread_join(tid[i], NULL);

    printf("The word '%s' appears %d times in %d files.\n", word, no_ap, no_files);

    return 0;
}
