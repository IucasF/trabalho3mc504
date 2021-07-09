/*
 * Problema do barbeiro dorminhoco com vários barbeiros.
 */

/*****************************************************************
 *  Cineminha desenvolvido por:                                  *
 *     Danilo Silva Marshall (EC05 - UNICAMP)           RA042865 *
 *     Erika Regina Campos de Almeida (EC05 - UNICAMP)  RA043217 *
 ****************************************************************/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CLIENTES 10
#define N_BARBEIROS 2
#define N_CADEIRAS 5

/*
 * Variaveis para guardar estado dos clientes/barbeiros.
 * A - arrived           (cliente);
 * C (C ou B) - cutting  (cliente/barbeiro);
 * E - exit  		 (cliente);
 * I - idle              (cliente);
 * L - left    		 (cliente);
 * S - sleeping 	 (barbeiro);
 * W - waiting		 (cliente).
 */
typedef enum {A, CC, E, I, L, W} estado_c;
typedef enum {CB, S} estado_b;

int cad_disponiveis = 5;
int cad_barbeiro_disponiveis = 2;

estado_c estadoC[N_CLIENTES];
estado_b estadoB[N_BARBEIROS];

/*
 * Variaveis para guardar estado das cadeiras.
 * B - busy
 * F - free
 */
typedef enum {B, F} estado_cadeira;

estado_cadeira estadoCadeiraCliente[N_CADEIRAS];

int clientesCadeira[N_CADEIRAS];
int clientesBarbeiro[N_BARBEIROS];

/**
sem_t sem_cadeiras;
sem_t sem_cad_barbeiro[N_BARBEIROS];
sem_t sem_cabelo_cortado[N_BARBEIROS];
sem_t sem_cliente_cadeira[N_BARBEIROS];
**/
//sem_t  sem_cadeiras
pthread_mutex_t lock_cadeiras;
pthread_mutex_t lock_cad_barbeiro;
pthread_mutex_t lock_cabelo_cortado;
pthread_mutex_t lock_cliente_cadeira;

pthread_mutex_t lock_escreve_visor;
pthread_mutex_t lock_le_visor;
pthread_mutex_t lock_estados;

/**
sem_t sem_escreve_visor;
sem_t sem_le_visor;
sem_t sem_estados; /* Semaforo para troca de estados. */

int visor;

/* Funcao responsavel pela impressao do cineminha. */
void imprimeCineminha() {
  int i, j, qtosClientesArrived = 0, qtosClientesExit = 0, titulo = 0;

  int clientesArrived[N_CLIENTES], clientesExit[N_CLIENTES];

  /* Verifica quais clientes estao no estado "Arrived". */
  for (i = 0; i < N_CLIENTES; i++) {
    if (estadoC[i] == A) {
      clientesArrived[qtosClientesArrived++] = i;
    }
  }

  /* Verifica quais clientes estao no estado "Exit". */
  for (i = 0; i < N_CLIENTES; i++) {
    if (estadoC[i] == E) {
      clientesExit[qtosClientesExit++] = i;
    }
  }

  /*****************************************************************************
   **************************** IMPRESSAO DO TITULO ****************************
   ****************************************************************************/

  /* Imprime os espacos em branco da parte referente aos clientes Arrived. */
  for (i = 0; i < qtosClientesArrived; i++) {
    printf("     ");
  }

  /* Faz a conta para imprimir o título 'BARBEARIA' no meio. */

  if (qtosClientesArrived == 0) {
    printf("     ");
  }

  titulo = (4 + 5 * N_CADEIRAS + 5 + 5 * N_BARBEIROS) / 2;

  titulo -= 5;

  for (i = 0; i < titulo; i++) {
    printf(" ");
  }

  printf("BARBEARIA\n");

  /*****************************************************************************
   ************************** FIM IMPRESSAO DO TITULO **************************
   ****************************************************************************/

  /*****************************************************************************
   ************************** IMPRESSAO PRIMEIRA LINHA *************************
   ****************************************************************************/

  for (i = 0; i < qtosClientesArrived; i++) {
    printf(" ... ");
  }

  /* Se nao chegou clientes, imprimimos espacamento para ficar alinhado com
   * outros momentos do cineminha.
   */
  if (qtosClientesArrived == 0) {
    printf("     ");
  }

  printf ("/ ");

  /* Imprimi as cadeiras dos clientes. */
  for (i = 0; i < N_CADEIRAS; i++) {
    if (estadoCadeiraCliente[i] == B) {
      printf(" ... ");
    } else {
      printf("     ");
    }
  }

  /* Espaço entre as cadeiras dos clientes e as dos barbeiros. */
  printf("    ");

  for (i = 0; i < N_BARBEIROS; i++) {
    switch (estadoB[i]) {
    case CB:
      printf(" ... ");
      break;
    case S:
      printf(" zzz ");
      break;
    }
  }
  printf("\\ \n");

  /*****************************************************************************
   ********************** FIM DA IMPRESSAO PRIMEIRA LINHA **********************
   ****************************************************************************/

  /*****************************************************************************
   ************************** IMPRESSAO SEGUNDA LINHA **************************
   ****************************************************************************/

  for (i = 0; i < qtosClientesArrived; i++) {
    printf(" %2d  ", clientesArrived[i]);
  }

  /* Se nao chegou clientes, imprimimos espacamento para ficar alinhado com
   * outros momentos do cineminha.
   */
  if (qtosClientesArrived == 0) {
    printf("     ");
  }

  printf ("| ");

  for (i = 0; i < N_CADEIRAS; i++) {
    if (estadoCadeiraCliente[i] == B) {
      printf(" %2d  ", clientesCadeira[i]);
    } else {
      printf("     ");
    }
  }

  /* Espaço entre as cadeiras dos clientes e dos barbeiros. */
  printf("    ");

  j = 0;
  for (i = 0; i < N_BARBEIROS; i++) {
    switch (estadoB[i]) {
    case CB:
      printf(" %2d  ", clientesBarbeiro[i]);
      break;
    case S:
      printf("     ");
      break;
    }
  }

  printf("| ");

  for (i = 0; i < qtosClientesExit; i++) {
    printf(" %2d  ", clientesExit[i]);
  }

  printf("\n");

  /*****************************************************************************
   ********************** FIM DA IMPRESSAO SEGUNDA LINHA ***********************
   ****************************************************************************/

  /*****************************************************************************
   ************************* IMPRESSAO TERCEIRA LINHA **************************
   ****************************************************************************/

  for (i = 0; i < qtosClientesArrived; i++) {
    printf("     ");
  }

  /* Se nao chegou clientes, imprimimos espacamento para ficar alinhado com
   * outros momentos do cineminha.
   */
  if (qtosClientesArrived == 0) {
    printf("     ");
  }

  printf("\\ ");

  for (i = 0; i < N_CADEIRAS; i++) {
    printf(" C%02d ", i);
  }

  printf("    ");

  for (i = 0; i < N_BARBEIROS; i++) {
    printf(" B%02d ", i);
  }

  printf("/ \n\n");

  /*****************************************************************************
   ********************** FIM DA IMPRESSAO TERCEIRA LINHA **********************
   ****************************************************************************/

  return;
}

/* Thread dos barbeiros. */
void* f_barbeiro(void *v) {
  int id = *(int*)v;

  while(1) {
    pthread_mutex_lock(&lock_escreve_visor);
    visor = id;
    pthread_mutex_unlock(&lock_le_visor);

    /* Para o corte nao ser imediato.*/
    sleep(1);

    pthread_mutex_lock(&lock_cliente_cadeira);
    cad_barbeiro_disponiveis++;
    pthread_mutex_unlock(&lock_cabelo_cortado);

    sleep(random()%3);
  }
  return NULL;
}

/* Thread dos clientes. */
void* f_cliente(void* v) {
  int id = *(int*) v;
  int i;
  int minha_cadeira, minhaCadeiraCliente;

  sleep(random()%3);

  pthread_mutex_lock(&lock_estados);
  /* Cliente chegou na barbearia. */
  estadoC[id] = A;

  /* Imprime que o cliente chegou na barbearia. */
  imprimeCineminha();
  pthread_mutex_unlock(&lock_estados);

  if (cad_disponiveis > 0) {
    pthread_mutex_lock(&lock_cadeiras);
    /* Cliente entrou na barbearia e sentou em uma cadeira. */
    pthread_mutex_lock(&lock_estados);
    estadoC[id] = W;

    /* Aloca uma cadeira para o cliente. */
    for (i = 0; i < N_CADEIRAS; i++) {
      if (estadoCadeiraCliente[i] == F) {
	estadoCadeiraCliente[i] = B;
	clientesCadeira[i] = id;
  cad_disponiveis--;
	minhaCadeiraCliente = i;

	break;
      }
    }
    pthread_mutex_unlock(&lock_cadeiras);

    /* Imprime que o cliente esta esperando na barbearia. */
    imprimeCineminha();
    pthread_mutex_unlock(&lock_estados);

    /* Cliente espera o visor mostrar um barbeiro livre. */
    pthread_mutex_lock(&lock_le_visor);
    minha_cadeira = visor;
    /* Permite que um outro barbeiro escreva no visor. */
    pthread_mutex_unlock(&lock_escreve_visor);
    /* Espera cadeira do barbeiro ficar livre para sentar. */
    if(cad_barbeiro_disponiveis > 0){
      pthread_mutex_lock(&lock_cad_barbeiro);
      cad_barbeiro_disponiveis--;
    }


    pthread_mutex_unlock(&lock_cliente_cadeira);

    pthread_mutex_lock(&lock_estados);

    /* Libera a cadeira que estava sentado para um novo cliente que chegar. */
    pthread_mutex_unlock(&lock_cadeiras);
    cad_disponiveis++;

    /* Altera os estados para cortar o cabelo. */
    estadoC[id] = CC;
    estadoB[minha_cadeira] = CB;

    /* Guarda que o cliente esta na cadeira do barbeiro indicada por
     * minha_cadeira.
     */
    clientesBarbeiro[minha_cadeira] = id;

    estadoCadeiraCliente[minhaCadeiraCliente] = F;

    /* Imprime que o barbeiro vai cortar o cabelo. */
    imprimeCineminha();
    pthread_mutex_unlock(&lock_estados);

    /* Termino de alterar e imprimir o cineminha. */

    pthread_mutex_lock(&lock_cabelo_cortado);

    pthread_mutex_lock(&lock_estados);
    /* Cliente sai da barbearia. */
    estadoC[id] = E;
    estadoB[minha_cadeira] = S;

    /* Imprime que o cliente saiu da barbearia. */
    imprimeCineminha();

    pthread_mutex_unlock(&lock_cad_barbeiro);
    cad_barbeiro_disponiveis++;




    /* Altera o estado pois o cliente vai embora. */
    estadoC[id] = L;
    pthread_mutex_unlock(&lock_estados);

  } else{
    /* Cliente desistiu de cortar o cabelo. */
    pthread_mutex_lock(&lock_estados);
    estadoC[id] = L;
    pthread_mutex_unlock(&lock_estados);
  }


  return NULL;
}

int main() {
  pthread_t thr_clientes[N_CLIENTES], thr_barbeiros[N_BARBEIROS];
  int i, id_cl[N_CLIENTES], id_bar[N_BARBEIROS];

/**  sem_init(&sem_cadeiras, 0, N_CADEIRAS);
  sem_init(&sem_escreve_visor, 0, 1);
  sem_init(&sem_le_visor, 0, 0);
  sem_init(&sem_estados, 0, 1);
  **/


  //sem_init(&sem_cadeiras, 0, N_CADEIRAS);
  pthread_mutex_init(&lock_cadeiras,NULL);
  pthread_mutex_init(&lock_cad_barbeiro, NULL);
  pthread_mutex_init(&lock_cabelo_cortado, NULL);
  pthread_mutex_init(&lock_cliente_cadeira, NULL);
  pthread_mutex_init(&lock_escreve_visor, NULL);
  pthread_mutex_init(&lock_le_visor, NULL);
  pthread_mutex_init(&lock_estados, NULL);



  for (i = 0; i < N_BARBEIROS; i++) {
    pthread_mutex_lock(&lock_estados);
    estadoB[i] = S;
    pthread_mutex_unlock(&lock_estados);
  }

  for (i = 0; i < N_CLIENTES; i++) {
    pthread_mutex_lock(&lock_estados);
    estadoC[i] = I;
    pthread_mutex_unlock(&lock_estados);
  }

  for (i = 0; i < N_CADEIRAS; i++) {
    pthread_mutex_lock(&lock_estados);
    estadoCadeiraCliente[i] = F;
    pthread_mutex_unlock(&lock_estados);
  }

  for (i = 0; i < N_CLIENTES; i++) {
    id_cl[i] = i;
    pthread_create(&thr_clientes[i], NULL, f_cliente, (void*) &id_cl[i]);
  }

  for (i = 0; i < N_BARBEIROS; i++) {
    id_bar[i] = i;
    pthread_create(&thr_barbeiros[i], NULL, f_barbeiro, (void*) &id_bar[i]);
  }

  for (i = 0; i < N_CLIENTES; i++)
    pthread_join(thr_clientes[i], NULL);

  /* Barbeiros são assassinados. */

  return 0;
}
