#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BACKPACK_CAP 10
#define MAX_NAME_LEN 30
#define MAX_TYPE_LEN 20

typedef struct Componente {
  char name[MAX_NAME_LEN];
  char type[MAX_TYPE_LEN];
  int prio;
  int quant;
} Componente;

Componente findB(char* search, Componente arr[], int arrSize);
bool rmComponent(Componente backpack[], int* size, char* componentName);
int sortArray(Componente backapack[], int size, int sortingMode, int* isSortedMode);
int sortByName(Componente backpack[], int size);
int sortByPriority(Componente backpack[], int size);
int sortByType(Componente backpack[], int size);
void cleanInputBuffer();
void printActionMenu(int* option, int size, int isSortedMode);
void printFindComponentMenu(char* componentName);
void printInsertComponentMenu(Componente* temp);
void printComponent(Componente backpack[], int size, char* componentName);
void printList(Componente backpack[], int size);
void printRemoveComponentMenu(char* componentName);
void printSortComponentMenu(int* sortingMode);
void push(Componente backpack[], int* size, Componente newComponent);
void swap(Componente *a, Componente *b);

int main() {
  // Criamos o array
  Componente backpack[MAX_BACKPACK_CAP];
  int componentCount = 0;
  int isSortedMode;
  int sortingMode;
  
  Componente temp; // Usado para capturar dados do menu
  int option;
  char tempName[MAX_NAME_LEN];

  do {
    printActionMenu(&option, componentCount, isSortedMode); 
    
    switch(option) {
      case 1: 
        if (componentCount == MAX_BACKPACK_CAP) {
          printf("\nMochila lotada!\n");
          break;
        }
        
        printInsertComponentMenu(&temp);

        if (temp.prio > 5 || temp.prio < 1) {
          printf("\nPor favor, digite um numero para a prioridade entre 1 e 5!\n");
          break;
        }

        push(backpack, &componentCount, temp);
        printf("\nComponente '%s' adicionado!\n", backpack[componentCount - 1].name);
        isSortedMode = 0;

        printList(backpack, componentCount);
        break;
      case 2:
        if (componentCount == 0) {
          printf("\nMochila vazia!\n");
          break;
        }
        
        printRemoveComponentMenu(tempName);
        if (rmComponent(backpack, &componentCount, tempName)) {
          printf("\nComponente '%s' descartado!\n", tempName);
          printList(backpack, componentCount);
        } else {          
          printf("\nComponente nao encontrado!\n");
        }
        break;
      case 3:
        printList(backpack, componentCount);
        break;
      case 4: 
        if (componentCount == 0) {
          printf("\nMochila vazia!\n");
          break;
        }
        printSortComponentMenu(&sortingMode);
        int comparisionsCount = sortArray(backpack, componentCount, sortingMode, &isSortedMode);
        char* sortedModeStr;

        switch (isSortedMode) {
        case 1:
          sortedModeStr = "NOME";
          break;
        case 2:
          sortedModeStr = "TIPO";
          break;
        case 3:
          sortedModeStr = "PRIORIDADE";
          break;
        }

        printf("\nMochila organizada por %s\n", sortedModeStr);
        printf("Analise de Desempenho: Foram necessarias %d comparacoes.\n", comparisionsCount);
        break;
      case 5:
        if (componentCount == 0) {
          printf("\nMochila vazia!\n");
          break;
        }
        if (isSortedMode != 1) {
          printf("\nO array precisa estar organizado em ordem alfabetica para utilizar esta opcao.\n");
          printf("Organize ele antes!\n");
          break;
        }

        printFindComponentMenu(tempName);
        printComponent(backpack, componentCount, tempName);
        break;
      case 0:
        printf("\nSaindo...\n");
        break;
      default:
        if (option < 0 || option > 4) {
          printf("\nPor favor, escolha uma opcao valida!\n");
        }
    }
    if (option != 0) system("pause");

  } while (option != 0);

  return 0;
}

// Adiciona ao final do array
void push(Componente backpack[], int* size, Componente newComponent) {
  if (*size >= MAX_BACKPACK_CAP) return;

  backpack[*size] = newComponent;
  (*size)++;
}

// Remove e "puxa" os componentes da frente para não deixar buracos
bool rmComponent(Componente backpack[], int* size, char* componentName) {
  for (int i = 0; i < *size; i++) {
    if (strcmp(backpack[i].name, componentName) == 0) {
      
      // Move todos os componentes a direita uma posição para a esquerda
      for (int j = i; j < (*size) - 1; j++) {
        backpack[j] = backpack[j + 1];
      }
      
      (*size)--;
      return true;
    }
  }
  return false;
}

void printList(Componente backpack[], int size) {
  printf("\n--- COMPONENTES NA MOCHILA (%d/%d) ---\n", size, MAX_BACKPACK_CAP);
  
  if (size == 0) {
    printf("Mochila vazia.\n");
    return;
  }

  printf("\n-----------------------------------------------------------------------------\n");
  printf("%-29s | %-19s | %-10s | %-10s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
  printf("-----------------------------------------------------------------------------\n");
  
  for (int i = 0; i < size; i++) {
    printf("%-29s | %-19s | %-10d | %-10d\n", backpack[i].name, backpack[i].type, backpack[i].quant, backpack[i].prio);
  }
  printf("-----------------------------------------------------------------------------\n");
}

void printInsertComponentMenu(Componente* temp) {
  printf("\n--- Adicionar Novo Componente ---\n");
  printf("Nome: ");
  fgets(temp->name, MAX_NAME_LEN, stdin);
  temp->name[strcspn(temp->name, "\n")] = 0;

  printf("Tipo: ");
  fgets(temp->type, MAX_TYPE_LEN, stdin);
  temp->type[strcspn(temp->type, "\n")] = 0;
  
  printf("Quantidade: ");
  scanf("%d", &temp->quant);

  printf("Prioridade de Montagem (1-5): ");
  scanf("%d", &temp->prio);
  cleanInputBuffer();
}

void printActionMenu(int* option, int size, int isSortedMode) {
  char* sortedByNameStatus;
  
  if (isSortedMode == 1) {
    sortedByNameStatus = "ORDENADO";
  } else {
    sortedByNameStatus = "NAO ORDENADO";
  }

  printf("\n==================================\n");
  printf("      PLANO DE FUGA\n");
  printf("==================================\n");
  printf("Capacidade: %d/%d\n", size, MAX_BACKPACK_CAP);
  printf("Status de Ordenacao por Nome: %s\n\n", sortedByNameStatus);
  printf("1 - Adicionar Componente\n");
  printf("2 - Remover Componente\n");
  printf("3 - Listar Componentes\n");
  printf("4 - Organizar Mochila\n");
  printf("5 - Busca Binaria por Componente-Chave (por nome)\n");
  printf("0 - ATIVAR TORRE DE FUGA (Sair)\n");  
  printf("----------------------------------\n");
  printf("Escolha uma opcao: ");
  scanf("%d", option);
  cleanInputBuffer();
}

void printRemoveComponentMenu(char* componentName) {
  printf("\n--- Remover Componente ---\n");
  printf("Nome do Componente: ");
  fgets(componentName, MAX_NAME_LEN, stdin);
  componentName[strcspn(componentName, "\n")] = 0;
}

void printSortComponentMenu(int* sortingMode) {
  printf("\n--- Estrategia de Organizacao ---\n");
  printf("Como desejar ordenar os componentes?\n\n");
  printf("1 - Por Nome (Ordem Alfabetica)\n");
  printf("2 - Por Tipo\n");
  printf("3 - Por Prioridade de Montagem\n");
  printf("0 - Cancelar\n");
  printf("------------------------------------\n");
  printf("Escolha o criterio: ");
  scanf("%d", sortingMode);
  cleanInputBuffer();
}

void printFindComponentMenu(char* componentName) {
  printf("\n--- Busca Binaria por Componente-Chave ---\n");
  printf("Nome do Componente: ");
  fgets(componentName, MAX_NAME_LEN, stdin);
  componentName[strcspn(componentName, "\n")] = 0;
}

void cleanInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void printComponent(Componente backpack[], int size, char* componentName) {
  Componente component = findB(componentName, backpack, size);

  
  if (strcmp(component.name, "") == 0) {
    printf("\nComponente nao encontrado!\n");
    return;
  }

  printf("\n-----------------------------------------------------------------------------\n");
  printf("%-29s | %-19s | %-10s | %-10s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
  printf("-----------------------------------------------------------------------------\n");
  printf("%-29s | %-19s | %-10d | %-10d\n", component.name, component.type, component.quant, component.prio);
  printf("-----------------------------------------------------------------------------\n");
}

int sortByName(Componente arr[], int size) {  
  int indexOfLowest;
  int comparasionsCount = 0;
 
  for (int i = 0; i < size; i++) {
    indexOfLowest = i;

    for (int j = i; j < size; j++) {
      if (strcmp(arr[j].name, arr[indexOfLowest].name) < 0) {
        indexOfLowest = j;
      }
      comparasionsCount++;
    }

    swap(&arr[indexOfLowest], &arr[i]);
  }

  return comparasionsCount;
}
int sortByType(Componente arr[], int size) {  
  int indexOfLowest;
  int comparasionsCount = 0;

  for (int i = 0; i < size; i++) {
    indexOfLowest = i;

    for (int j = i; j < size; j++) {
      if (strcmp(arr[j].type, arr[indexOfLowest].type) < 0) {
        indexOfLowest = j;
      }
      comparasionsCount++;
    }

    swap(&arr[indexOfLowest], &arr[i]);
  }

  return comparasionsCount;
}
int sortByPriority(Componente arr[], int size) {  
  int indexOfLowest;
  int comparasionsCount = 0;

  for (int i = 0; i < size; i++) {
    indexOfLowest = i;

    for (int j = i; j < size; j++) {
      if (arr[j].prio < arr[indexOfLowest].prio) {
        indexOfLowest = j;
      }
      comparasionsCount++;
    }

    swap(&arr[indexOfLowest], &arr[i]);
  }

  return comparasionsCount;
}

int sortArray(Componente backpack[], int size, int sortingMode, int* isSortedMode) {
  switch (sortingMode) {
    case 1:
      *isSortedMode = 1;
      return sortByName(backpack, size);
    case 2:
      *isSortedMode = 2;    
      return sortByType(backpack, size);
    case 3:
      *isSortedMode = 3;
      return sortByPriority(backpack, size);
    default:
      if (sortingMode == 0) {
        printf("Cancelando...\n");
      } else {
        printf("Por favor, digite uma opcao valida!\n");
      }
  }

  return -1;
}
void swap(Componente *a, Componente *b) {
  Componente temp = *a;
  *a = *b;
  *b = temp;
}
Componente findB(char* search, Componente arr[], int arrSize) {
  int max = arrSize - 1;
  int min = 0;
  int half = max / 2;
  Componente curr = arr[half];

  while (min <= max) {
    if (strcmp(curr.name, search) > 0) {
      max = half - 1;
      half = (max + min) / 2;
      curr = arr[half];
    } else if (strcmp(curr.name, search) < 0) {
      min = half+1;
      half = (max + min) / 2;
      curr = arr[half];
    } else {
      return curr;      
    }
  }
  Componente emptyComponent = {"", "", 0, 0};

  return emptyComponent;
}