# Árvore B+ em C

Este é um programa em C que implementa as operações básicas em uma árvore B+ para inserção, remoção e impressão. O programa lê os comandos de um arquivo de entrada e executa as operações correspondentes, e, em seguida, escreve os resultados em um arquivo de saída.

## Estrutura do Projeto

O projeto consiste nos seguintes arquivos:

- **arvoreBmais.c**: Implementação da lógica da árvore B+ em linguagem C.
- **arquivo_entrada.txt**: Arquivo de entrada contendo os comandos a serem executados na árvore B+.
- **arquivo_saida.txt**: Arquivo de saída onde os resultados das operações são escritos.

## Execução do Programa

> Para executar o programa, siga as instruções abaixo:

1. Compile o programa usando um compilador C. Por exemplo, utilize o GCC:

```bash
gcc arvoreBmais.c -o arvoreBmais
```

2. Execute o programa com os argumentos apropriados:
```bash
./arvoreBmais arquivo_entrada.txt arquivo_saida.txt
```

**Observação:** _certifique-se de substituir "arquivo_entrada.txt" pelo nome do arquivo que contém os comandos desejados e "arquivo_saida.txt" pelo nome do arquivo onde os resultados serão escritos._

## Comandos Disponíveis
- `i` <chave>: Inserção de uma chave na árvore B+.
- `r` <chave>: Remoção de uma chave da árvore B+.
- `p`: Impressão da árvore B+.
- `f`: Finalização do programa.

Lembre-se de seguir o formato correto ao fornecer os comandos no arquivo de entrada:
```bash
i <chave>
r <chave>
p
f
```
## 🤝 Colaborador

A pessoa que contribuiu para o projeto

<table>
  <tr>
    <td align="center">
      <a href="#">
        <img src="https://avatars.githubusercontent.com/u/81971651?s=400&u=548b7cc3deb1bd124ba02dbc2acc865b97138ce3&v=4" width="100px;" alt="Foto do Bruno Friedrich no Github"/><br>
        <sub>
          <b>Bruno Friedrich</b>
        </sub>
      </a>
    </td>
  </tr>
</table>
