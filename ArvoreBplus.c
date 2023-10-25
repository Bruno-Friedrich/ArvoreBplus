//--------------------------------------------------------------
// NOMES DO RESPONSÁVEL: Bruno Friedrich Raquel (13672450)
//--------------------------------------------------------------

// Bibliotecas utilizadas no programa
#include <locale.h> // Biblioteca para utilizar os caracteres especiais
#include <stdio.h>  // Biblioteca padrão
#include <stdlib.h> // Biblioteca para utilização do malloc

// Define o grau base da árvore
#define T 3   

// Como o grau base da árvore é 3
// 2*T-1 -> CHEIO = 5
#define CHEIO 5 

#define TRUE 1  // Utilizado para operações booleanas
#define FALSE 0 // Utilizado para operações booleanas

typedef int bool;     // "comando"
typedef int ValueKey; // Valor da chave 

int num_nodes; // Quantidade de nós da árvore
 
typedef struct NODE {
  bool is_leaf; // Indica se é folha ou não
  bool is_root; // Indica se é a raiz ou não
  int num_keys; // Número de chaves no nó

  struct NODE *children[2 * T]; // Array de filhos
  ValueKey keys[2 * T - 1];          // Valor das chaves
  int *record[2 * T - 1];       // Armazena os registros

  struct NODE *next; // Endereço do próximo nó
} NODE;

typedef struct {
  NODE *Root; // Raiz da Árvore B+
} TreeBP;


/* FUNÇÕES UTILIZADAS PARA AS OPERAÇÕES */
void initBP(TreeBP * treeBP);                                                  // Inicializa a árvore B+
NODE * newNODE();                                                              // Inicializa um novo NÓ
void split(NODE *parent, NODE *child, int indice);                             // Separa um nó para a inserção
void insertNODE(ValueKey key, TreeBP * treeBP);                                // Insere um NÓ na árvore B+
void insertWithoutSplit(ValueKey key, NODE * tmp);                             // Inserção para os casos que não precisa do split
void removeNODE(ValueKey key, TreeBP * treeBP);                                // Remove um NÓ e analisa seus respectivos casos
NODE * locateNODE(ValueKey key, NODE *root, int * key_index);                  // Localiza um NÓ e o retorna se for interno
NODE * searchRecord(NODE * tmp, int key, int * recordIndice, NODE **parent);   // Procura um registro nos nós folha
void printBP(FILE * arquivo_saida, NODE * tmp);                                // Imprime a árvore B+ de forma parentesada

  
/****************************************/
/****************** MAIN ****************/
/****************************************/


int main(int argc, char **argv) {
  // Seta caracteres para o padrão do sistema operacional
  setlocale(LC_ALL, "");

  // Imprime caso a quantidade de argumentos tenha sido passada de forma errada
  // Forma correta: arvoreBmais.exe <arquivo_entrada.txt> <arquivo_saida.txt>
  if (argc < 3 || argc > 3) {
    printf("Argumentos passados de forma errada.\n");
    printf("Forma correta: %s <arquivo_entrada.txt> <arquivo_saida.txt>\n", argv[0]);
    return -1;
  }

  // Inicialização do programa
  printf("/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/\n");
  printf("/*Esse EP consiste em implementar operações básicas em uma árvore B+.*/\n");
  printf("/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/\n\n");

  printf("Abrindo os arquivos...\n");

  // Abre o arquivo para leitura
  FILE *arquivo_entrada = fopen(argv[1], "r");
  if (arquivo_entrada == NULL) {
    printf("Erro ao abrir o arquivo de entrada.\n");
    return 1;
  }

  // Abre o arquivo para escrita
  FILE *arquivo_saida = fopen(argv[2], "w");
  if (arquivo_saida == NULL) {
    printf("Erro ao abrir o arquivo de saída.\n");
    fclose(arquivo_entrada); // Fecha o arquivo de entrada, já que não será usado
    return 1;
  }
  
  // Mensagem que os arquivos de texto foram abertos com sucesso
  printf("Os arquivos foram abertos!\n\n");

  // Apresenta ao usuário como chamar as operações
  printf("Código para chamar as operações do trabalho.\n");

  printf("Inserção: i <chave>\n");
  printf("Remoção: r <chave>\n");
  printf("Impressão: p\n");
  printf("Finalização: f\n\n");

  // Utilizado para salvar as respostas do usuário
  char instrucao;
  int numero;

  // ALoca memória para a raiz e a inicializa 
  TreeBP treeBP;
  initBP(&treeBP);
  
  while (fscanf(arquivo_entrada, " %c", &instrucao) == 1) {
    switch (instrucao) {
    // Inserção
    case 'i': {
      if (fscanf(arquivo_entrada, " %i", &numero) == 1)
        printf("Inserção: %i\n", numero);
      insertNODE(numero, &treeBP);
      printf("Valor inserido com sucesso\n");
    } break;

    // Remoção
    case 'r': {
      if (fscanf(arquivo_entrada, " %i", &numero) == 1)
        printf("Remoção: %i\n", numero);
      removeNODE(numero, &treeBP);
    } break;

    // Impressão
    case 'p': {
      printf ("Iniciando o processo de impressão..\n");
      printBP(arquivo_saida, treeBP.Root);
      printf("A impressão foi realizada\n");
      fprintf(arquivo_saida, "\n");
    } break;

    // Finalização
    case 'f': {
      printf("Programa executado com sucesso!\n");
      printf("Terminando a execução do programa.\n");
      return 0;
    } break;

    // Comando errado
    default:
      printf("Comando inválido.\n");
      break;
    }
  }
  return 0;
}


// Inicialia a Árvore B+
void initBP(TreeBP * treeBP) {
  // Aloca memória para o NÓ da raiz
  NODE * newNODE = (NODE*) malloc (sizeof(NODE));
  
  // A árvore recebe o novo NÓ que representa a raiz
  treeBP->Root = newNODE;
  
  // Com nenhum elemento, a raiz é uma folha
  treeBP->Root->is_leaf = TRUE;
  
  // Como acabou de inicializar, não possui elementos
  treeBP->Root->num_keys = 0;
}


// Inicializa um novo NÓ 
NODE * newNODE()
{
  NODE * newNODE = (NODE*) malloc(sizeof(NODE));
  for (int i = 0; i < 2 * T; i++)
      newNODE->record[i] = NULL;
  newNODE->num_keys = 0;
  return newNODE;
}


// Separa o filho de um dado pai
void split(NODE *parent, NODE *child, int indice)
{
  // Variável utilizada para facilitar as operações
  int k = T-1;
  
  // Novo nó que será utilizado para adicionar os elementos novos do split
  NODE * newNode = newNODE();

  // Vai ser folha se o filho for folha
  newNode->is_leaf = child->is_leaf;

  // Se o nó NÃO for folha
  if (!child->is_leaf)
  {
    newNode->num_keys = k;
    for (int j = 0; j < k; j++) newNode->keys[j] = child->keys[j + T];

    for (int j = 0; j < T; j++) newNode->children[j] = child->children[j + T];

    child->num_keys = k;

    for (int j = parent->num_keys; j > indice; j--) parent->children[j + 1] = parent->children[j];

    parent->children[indice + 1] = newNode;

    for (int j = parent->num_keys; j > indice - 1; j--) parent->keys[j] = parent->keys[j - 1];

    parent->keys[indice] = child->keys[k];
    parent->num_keys++;
  }

  // Se o nó É folha
  else
  {
    newNode->num_keys = T;
    
    for (int j = 0; j <= k; j++) newNode->keys[j] = child->keys[j + k];

    child->num_keys = k;
    child->next = newNode;

    for (int j = parent->num_keys; j > indice; j--) parent->children[j + 1] = parent->children[j];
    
    parent->children[indice + 1] = newNode;
    
    for (int j = parent->num_keys; j > indice - 1; j--) parent->keys[j] = parent->keys[j - 1];
    
    parent->keys[indice] = child->keys[k];
    parent->num_keys++;
  }  
}


// Insere um NÓ na árvore B+
void insertNODE(ValueKey key, TreeBP * treeBP)
{
  if (treeBP->Root->num_keys < CHEIO)
  {
    insertWithoutSplit(key, treeBP->Root);  
  }
  else
  {
    // Novo nó para o split
    NODE *newNode = newNODE();

    // Troca da raiz
    NODE * tmp = treeBP->Root; 
    treeBP->Root = newNode;
    newNode->children[0] = tmp;

    // Atribuições restantes
    newNode->is_leaf = FALSE;
    newNode->num_keys = 0;

    // Separa o filho
    split(newNode, tmp, 0);
    insertWithoutSplit(key, newNode);  
  }
}


// Inserção para os casos que não precisa do split
void insertWithoutSplit(ValueKey key, NODE * tmp)
{
  // Armazena o nó passado como parâmetro para auxiliar na inserção
  NODE * currentNode = tmp;

  // Variável utilizada para facilitar o cálculo da posição
  int j = tmp->num_keys - 1;
  
  // Se o nó NÃO for folha
  if (!currentNode->is_leaf) 
  {
    // Encontra a posição se tiver pelo menos 2 elementos
    while (j >= 1 && key < currentNode->keys[j]) j--;
    j++;

    if (key < currentNode->keys[0]) j--;

    // O filho está cheio
    if (currentNode->children[j]->num_keys == CHEIO) 
    {
        split(currentNode, currentNode->children[j], j);
        if (key > currentNode->keys[j]) j++;
    }
    insertWithoutSplit(key, currentNode->children[j]);
  }
  
  // O nó É folha
  else 
  {
    // Move o arranjo de keys para conseguir colocar a chave
    while (j >= 0 && key < currentNode->keys[j])
    {
        currentNode->keys[j + 1] = currentNode->keys[j];
        j--;
    }
    j++;

    // Atualiza o valor da chave/nó
    currentNode->keys[j] = key;
    currentNode->record[j] = NULL;
    currentNode->num_keys++;
  }
}


void removeNODE(ValueKey key, TreeBP * treeBP)
{
  // Verificação se é interno ou externo
  int keyIndice;
  NODE * position = locateNODE(key, treeBP->Root, &keyIndice);

  if (position == NULL)
  {
    // Chave só está na folha
    int recordIndice;
    NODE *parent = NULL;
    NODE *currentNode = searchRecord(treeBP->Root, key, &recordIndice, &parent);
    currentNode->record[recordIndice] = NULL;

    // Remove a chave do nó folha
    for (int i = recordIndice; i < currentNode->num_keys; i++)
        currentNode->keys[i] = currentNode->keys[i + 1];
    currentNode->num_keys--;

    if (parent)
    {
      // Encontra o índice do nó atual no nó pai
      int index_of_current = 0;
      while (parent->children[index_of_current] != currentNode)
          index_of_current++;

      NODE *left_brother = parent->children[index_of_current - 1];
      NODE *right_brother = parent->children[index_of_current];

      if (currentNode->num_keys <= T - 1)
      {
        // 3 casos para reequilibrar a árvore após a exclusão

        // Caso 1: O irmão à esquerda tem mais chaves que o mínimo
        if (left_brother->num_keys >= T)
        {
          // Move a maior chave do irmão à esquerda para o nó atual
          int receiving_key = left_brother->keys[left_brother->num_keys - 1];
          int i = currentNode->num_keys - 1;
          while (i >= 0)
          {
              currentNode->keys[i + 1] = currentNode->keys[i];
              i--;
          }
          currentNode->keys[i + 1] = receiving_key;
          currentNode->num_keys++;
          left_brother->num_keys--;
          parent->keys[index_of_current - 1] = receiving_key;
        }
          
        // Caso 2: O irmão à direita tem mais chaves que o mínimo
        else if (right_brother && right_brother->num_keys >= T)
        {
          // Move a menor chave do irmão à direita para o nó atual
          int receiving_key = right_brother->keys[0];
          int i = currentNode->num_keys - 1;
          while (receiving_key < currentNode->keys[i])
          {
              currentNode->keys[i + 1] = currentNode->keys[i];
              i--;
          }
          currentNode->keys[i + 1] = receiving_key;
          right_brother->num_keys--;
          parent->keys[index_of_current] = receiving_key;
        }
          
        // Caso 3: Ambos os irmãos têm o mínimo de chaves, então é necessária a mesclagem
        else
        {
          // Move todas as chaves do irmão à direita para o irmão à esquerda
          for (int i = left_brother->num_keys; i < 2 * T - 1; i++) left_brother->keys[i] = right_brother->keys[i - left_brother->num_keys];
          left_brother->num_keys++;

          // Remove a chave do nó pai e o ponteiro para o irmão à direita
          for (int i = index_of_current; i < parent->num_keys - 1; i++) parent->keys[i] = parent->keys[i + 1];
          for (int i = index_of_current + 1; i < parent->num_keys; i++) parent->children[i] = parent->children[i + 1];
          parent->num_keys--;
        }
      }
    }
  }

  
  else // chave está em um nó interno
  {
      // Obtém os filhos à esquerda e à direita da chave a ser removida
      NODE *childLeft = position->children[keyIndice];
      NODE *childRight = position->children[keyIndice + 1];
  
      // Caso 1: O filho à esquerda tem chaves suficientes para emprestar uma chave
      if (childLeft->num_keys >= T) 
      {
          // Substitui a chave a ser removida pela maior chave no filho à esquerda
          position->keys[keyIndice] = childLeft->keys[childLeft->num_keys - 1];
          // Move a menor chave do próximo nó para o filho à esquerda
          childLeft->next->keys[0] = childLeft->keys[childLeft->num_keys - 1];
          // Atualiza o número de chaves no filho à esquerda, pois emprestou uma chave
          childLeft->num_keys--;
      }
        
      // Caso 2: O filho à direita tem chaves suficientes para emprestar uma chave
      else if (childRight->num_keys >= T) 
      {
          // Substitui a chave a ser removida pela menor chave no filho à direita
          position->keys[keyIndice] = childRight->keys[1];
          // Desloca todas as chaves no filho à direita para a esquerda
          for (int i = 0; i < childRight->num_keys - 1; i++)
              childRight->keys[i] = childRight->keys[i + 1];
          // Atualiza o número de chaves no filho à direita, pois emprestou uma chave
          childRight->num_keys--;
      }
        
      // Caso 3: Ambos os filhos têm apenas T-1 chaves, é necessário mesclar
      else 
      {
          // Move todas as chaves do filho à direita para o final do filho à esquerda
          for (int i = childLeft->num_keys; i < 2 * T - 1; i++)
              childLeft->keys[i] = childRight->keys[i - childLeft->num_keys];
          
          // Atualiza o número de chaves no filho à esquerda após a mesclagem
          childLeft->num_keys = 2 * T - 2;
  
          // Remove a chave e o ponteiro para o filho à direita da posição atual
          for (int i = keyIndice; i < position->num_keys - 1; i++)
              position->keys[i] = position->keys[i + 1];
          for (int i = keyIndice + 1; i < position->num_keys; i++)
          {
              position->children[i] = position->children[i + 1];
          }
          position->num_keys--;
      }
  }
}


// Localiza um NÓ e o retorna se for interno
NODE * locateNODE(ValueKey key, NODE *tmp, int * keyIndice)
{
  NODE *currentNode = tmp;
  // Se for folha, não retorna o NÓ
  if (currentNode->is_leaf)
      return NULL;
  // Senão, busca o nó interno e retorna ele e passa sua posição a partir do ponteiro
  else
  {
    for (int i = 0; i < currentNode->num_keys; i++)
    {
      if (key < currentNode->keys[i])
      {
        locateNODE(key, currentNode->children[i], keyIndice);
      }
      else if (key == currentNode->keys[i])
      {
        *keyIndice = i;
        return currentNode;
      }
    }
    return NULL;
  }
}


NODE * searchRecord(NODE * tmp, int key, int * recordIndice, NODE **parent)
{
  NODE *current_node = tmp;
  int k = 0;

  // Se o nó atual é uma folha (nó sem filhos)
  if (current_node->is_leaf)
  {
    // Percorre as chaves do nó atual
    for (int i = 0; i < current_node->num_keys; i++)
    {
      // Se a chave buscada for maior que a chave atual, incrementa o índice 'k'
      if (key > current_node->keys[k]) k++;
        
      // Se encontrou a chave buscada, atualiza o 'recordIndice' e retorna o nó atual
      else if (key == current_node->keys[k])
      {
          *recordIndice = k;
          return current_node;
      }
        
      // Caso contrário, retorna NULL, pois a chave não foi encontrada no nó atual
      else
      {
          return NULL;
      }
    }
  }
    
  // Se o nó atual não é uma folha (nó com filhos)
  else
  {
    // Define o pai do nó atual no ponteiro 'parent'
    *parent = current_node;

    // Percorre as chaves do nó atual
    for (int i = 0; i < current_node->num_keys; i++)
    {
      // Se a chave buscada for maior que a chave atual, incrementa o índice 'k'
      if (key > current_node->keys[k]) k++;
    }

    // Chama recursivamente a função para continuar a busca no filho correto do nó atual
    return searchRecord(current_node->children[k], key, recordIndice, parent);
  }
}


// Imprime a árvore B+ de forma parentesada
void printBP(FILE * arquivo_saida, NODE * tmp)
{
  // Verifica se o primeiro tmp (raiz) do tipo NÓ é zero
  // Se for, a árvore é vazia
  if (tmp->num_keys == 0)
  {
      fprintf(arquivo_saida, "vazia");
      return;
  }

  // Inicia  impressão com um parêntese
  fprintf(arquivo_saida, "(");

  // Se o filho possui ao menos 1 elemento não nulo
  // Chama a função para imprimir o filho e por fim imprime a chave
  if (tmp->children[0] != NULL)
  {

      printBP(arquivo_saida, tmp->children[0]);
      fprintf(arquivo_saida, " %i ", tmp->keys[0]);
  }
  // Se for vazio, apenas imprime a chave
  else
      fprintf(arquivo_saida, "%i", tmp->keys[0]);
  
  int i = 1;
  // Loop para imprimir todos os filhos de um NÓ
  // Para se chegar no final do NÓ ou verificar todos os filhos
  for (; i < tmp->num_keys || tmp->children[i] != NULL; i++)
  {
      if (tmp->children[i] != NULL && i <= tmp->num_keys)
          printBP(arquivo_saida, tmp->children[i]);

      if (tmp->children[i] != NULL && i <= tmp->num_keys - 1)
          fprintf(arquivo_saida, " %i ", tmp->keys[i]);
      else if (i <= tmp->num_keys - 1)
          fprintf(arquivo_saida, " %i", tmp->keys[i]);
  }
  
  // Finaliza a execução com a impressão do parêntese
  fprintf(arquivo_saida, ")");
}
