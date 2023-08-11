# Vina++
Trabalho realizado na Disciplina de Programação 2 - UFPR;


O objetivo deste trabalho é implementar o programa vina++, que consiste de um arquivador básico (archiver), isto é, um programa que salva em sequência uma coleção de arquivos (denominados membros) dentro de outro arquivo (denominado archive) cuja estrutura permite recuperar os arquivos originais individualmente.


### Execução do Programa
O pacote de software a ser construído deve gerar o executável chamado vina++, que deve ser executado da seguinte forma:
```bash
vina++ <opção> <archive> [membro1 membro2 ...]
```
Onde a opção pode ser:
- -i : insere/acrescenta um ou mais membros ao archive. Caso o membro já exista no archive, ele deve ser substituído. Novos membros são inseridos respeitando a ordem da linha de comando, ao final do archive;
- -a : mesmo comportamento da opção -i, mas a substituição de um membro existente ocorre APENAS caso o parâmetro seja mais recente que o arquivado;
- -m target : move o membro indicado na linha de comando para imediatamente depois do membro target existente em archive. A movimentação deve ocorrer na seção de dados do archive;
- -x : extrai os membros indicados de archive. Se os membros não forem indicados, todos devem ser extraídos. A extração consiste em ler o membro de archive e criar um arquivo correspondente, com conteúdo idêntico, em disco;
- -r : remove os membros indicados de archive;
- -c : lista o conteúdo de archive em ordem, incluindo as propriedades de cada membro (nome, UID, permissões, tamanho e data de modificação) e sua ordem no arquivo. A saída esperada é igual ao do comando tar com as opções tvf
- -h : imprime uma pequena mensagem de ajuda com as opções disponíveis e encerra.

#### Exemplos:
```bash
  // inclui os arquivos "arq.txt", "foto.jpg" e "despesas.ods" em backup.vpp 
vina++ -i backup.vpp arq.txt foto.jpg despesas.ods

// atualiza "despesas.ods" em backup.vpp, se o arquivo externo for mais recente
vina++ -a backup.vpp despesas.ods

// extrai o arquivo arq.txt de backup.vpp
vina++ -x backup.vpp arq.txt

// extrai todos os arquivos de backup.vpp
vina++ -x backup.vpp

// move o arquivo "arq.txt" para depois de "despesas.ods" em backup.vpp
vina++ -m despesas.ods backup.vpp arq.txt
```

#### Execução:
- Clone o repositorio;
```bash
cd Vina-PlusPlus && make
```

#### OBS:
Programa sujeito a muitos bugs :)
