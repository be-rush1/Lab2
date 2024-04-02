#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define M 1000

typedef struct
{
  float *vetor;
  int inicio;
  int fim;
  float soma;
} args_thread;

float soma(float *vet)
{
  float soma = 0.0;
  for (int i = 0; i < M; i++)
  {
    soma += vet[i];
  }
  return soma;
}

void *soma_vetor(void *arg)
{
  args_thread *args = (args_thread *)arg;
  float soma_parcial = 0.0;
  for (int i = args->inicio; i < args->fim; i++)
  {
    soma_parcial += args->vetor[i];
  }
  args->soma = soma_parcial;
  pthread_exit(NULL);
}

int main(void)
{
  int num_threads;
  printf("Entre com o número de Threads: ");
  scanf("%d", &num_threads);

  args_thread *thread_args = (args_thread *)malloc(sizeof(args_thread) * num_threads);
  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);

  float vetor[M];
  FILE *fp;
  fp = fopen("vet.txt", "r");

  if (fp == NULL)
  {
    printf("erro ao abrir arquivo com vetores!\n");
    fclose(fp);
    exit(1);
  }

  int n = 0;
  while (n < M && fscanf(fp, "%f", &vetor[n]) == 1)
  {
    n++;
  }
  fclose(fp);

  for (int i = 0; i < num_threads; i++)
  {
    thread_args[i].inicio = i * (M / num_threads);
    thread_args[i].fim = (i + 1) * (M / num_threads);
    thread_args[i].vetor = vetor;
    pthread_create(&threads[i], NULL, soma_vetor, (void *)&thread_args[i]);
  }

  float soma_final = 0.0;
  for (int i = 0; i < num_threads; i++)
  {
    pthread_join(threads[i], NULL);
    soma_final += thread_args[i].soma;
  }

  float checa = soma(vetor);
  printf("SOMA FINAL C/ THREADS: %f\n", soma_final);
  printf("RESULTADO DA SOMA DA FUNÇÃO TESTE: %f\n", checa);
  printf("DIFERENÇA DAS DUAS: %f\n", fabs(soma_final - checa));

  free(thread_args);
  free(threads);

  return 0;
}
