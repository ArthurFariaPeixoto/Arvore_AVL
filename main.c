#include <stdio.h>
#include <malloc.h>
#include <conio.h>

struct No{

    int numero;
    int altura;
    struct No *esquerda;
    struct No *direita;

};
typedef struct No No;


int maior(int a, int b){
    return (a > b) ? a : b;
}

void criar(No **ponteiroRaiz){
    //cria uma arvore vazia
    *ponteiroRaiz = NULL;
}

void destruirAux(No *subRaiz){
    //deleta tudo que estiver contido na árvore
    if (subRaiz){
        destruirAux(subRaiz->esquerda);
        destruirAux(subRaiz->direita);
        free(subRaiz);
    }
}

void destruir(No **raiz){
    //chama a função que deleta tudo que estiver contido na árvore
    destruirAux(*raiz);
    *raiz = NULL;
    printf("\nArvore destruida!");
}

int alturaNo(No *raiz){
    //calcula a "altura" da árvore
    if(raiz == NULL){
        return -1;
    }else{
        return raiz->altura;
    }
}

int fatorBalanceamento(No *raiz){
    //faz o calculo necessário para ver se deve-se aplicar o balanceamento
    if(raiz){
        return (alturaNo(raiz->esquerda) - alturaNo(raiz->direita));
    }else{
      return 0;
    }
}

No *rotacaoEsquerda(No *raiz){
    //Essa função mapeia quais são os nós desbalanceados a direita e rotaciona para que a arvore fique certa
    No *raizAux, *filho;

    raizAux = raiz->direita;
    filho = raizAux->esquerda;

    raizAux->esquerda = raiz;
    raiz->direita = filho;

    raiz->altura = maior(alturaNo(raiz->esquerda), alturaNo(raiz->direita)) +1;
    raizAux->altura = maior(alturaNo(raizAux->esquerda), alturaNo(raizAux->direita))+1;

    return raizAux;
}

No *rotacaoDireita(No *raiz){
    //Essa função faz o mesmo que a rotacaoEsquerda, mas se baseia no balanceamento dos filhos da esquerda
    No *raizAux, *filho;

    raizAux = raiz->esquerda;
    filho = raizAux->direita;

    raizAux->direita = raiz;
    raiz->esquerda = filho;

    raiz->altura = maior(alturaNo(raiz->esquerda), alturaNo(raiz->direita))+1;
    raizAux->altura = maior(alturaNo(raizAux->esquerda), alturaNo(raizAux->direita))+1;

    return raizAux;
}

No *rotacaoDireitaEsquerda(No *raiz){
    raiz->direita = rotacaoDireita(raiz->direita);
    return rotacaoEsquerda(raiz);
}

No *rotacaoEsquerdaDireita(No *raiz){
    raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
    return rotacaoDireita(raiz);
}

No *balancear(No *raiz){
    int fator = fatorBalanceamento(raiz);
    //Se fator for menor que -1, a arvore esta desbalanceada para a direita
    //Se for maior que 1, esta desbalanceada para a esquerda

    if(fator < -1 && fatorBalanceamento(raiz->direita) <= 0){
        raiz = rotacaoEsquerda(raiz);
    }
    else if(fator < -1 && fatorBalanceamento(raiz->direita) > 0){
        raiz = rotacaoDireitaEsquerda(raiz);
    }
    else if(fator > 1 && fatorBalanceamento(raiz->esquerda) >=0){
        raiz = rotacaoDireita(raiz);
    }
    else if(fator > 1 && fatorBalanceamento(raiz->esquerda) < 0){
        raiz = rotacaoEsquerdaDireita(raiz);
    }

    return raiz;
}

No *criaNo(int valor){
    //cria a estrutura Nó que será inserida na árvore
    No *no = malloc(sizeof(No));

    if(no){
        no->numero = valor;
        no->esquerda = NULL;
        no->direita = NULL;
        no->altura = 0;
    }else{
        printf("\n\nERRO AO INSERIR NO");
    }
    return no;
}

No *inserir(No *raiz, int numeroInserido){
    //Insere um Nó na árvore
    if (raiz == NULL){
        return criaNo(numeroInserido);
    }
    else{
        //verifica se o numero é maior ou menor do que os valores já existentes na árvore para decidir de qual lado armazenar o valor
        if (numeroInserido < raiz->numero){
            raiz->esquerda = inserir(raiz->esquerda, numeroInserido);
        }
        else{
            raiz->direita = inserir(raiz->direita, numeroInserido);
        }
    }
    //recalcula a altura dos nós
    raiz->altura = maior(alturaNo(raiz->esquerda), alturaNo(raiz->direita)) + 1;

    //torna a árvore balanceada
    raiz = balancear(raiz);

    return raiz;
}

No* remover(No *pRaiz, int numero){
    if(pRaiz == NULL){
        printf("Numero nao encontrado na arvore!\n");
        return NULL;
    }else{ //procura o nó para remover
        if(pRaiz->numero == numero){
            //remove nós folhas
            if(pRaiz->esquerda == NULL && pRaiz->direita==NULL){
                free(pRaiz);
                printf("Numero %d removido.\n", numero);
                return NULL;
            }else{
                //remove nós com 2 filhos
                if(pRaiz->esquerda != NULL && pRaiz->direita != NULL){
                    No *aux = pRaiz->esquerda;
                    while(aux->direita != NULL){
                        aux= aux->direita;
                    }
                    pRaiz->numero = aux->numero;
                    aux->numero = numero;
                    pRaiz->esquerda = remover(pRaiz->esquerda, numero);
                    return pRaiz;
                }
                else{
                    //remove nós com 1 filho
                    No *aux;
                    if(pRaiz->esquerda != NULL){
                        aux = pRaiz->esquerda;
                    }else{
                        aux = pRaiz->direita;
                    }
                    free(pRaiz);
                    printf("Numero %d removido.\n", numero);
                    return aux;
                }
            }
        }else{
            if(numero < pRaiz->numero){
                pRaiz->esquerda = remover(pRaiz->esquerda, numero);
            }else{
                pRaiz->direita = remover(pRaiz->direita, numero);
            }
        }

        //recalcula a altura dos nós
        pRaiz->altura = maior(alturaNo(pRaiz->esquerda), alturaNo(pRaiz->direita))+1;

        //torna a árvore balanceada
        pRaiz = balancear(pRaiz);
        return pRaiz;
    }
}

void imprimirVisual(No *raiz, int nivel){
    //Imprime a árvore de modo mais fácil para visualizar
    int i;
    if(raiz){
        imprimirVisual(raiz->direita, nivel + 1);
        printf("\n");

        for(i=0; i<nivel; i++){
            printf("\t");
        }
        printf("%d", raiz->numero);
        imprimirVisual(raiz->esquerda, nivel + 1);
    }
}

void exibirInOrder(No *ponteiroRaiz) {
    //Exibe uma lista de valores em ordem
    if (ponteiroRaiz != NULL) {
        exibirInOrder(ponteiroRaiz->esquerda);
        printf(" %d", ponteiroRaiz->numero);
        exibirInOrder(ponteiroRaiz->direita);
    }
}

void exibirPreOrder(No *ponteiroRaiz){
    //Exibe uma lista de valores pré ordem
    if (ponteiroRaiz != NULL) {
        printf(" %d", ponteiroRaiz->numero);
        exibirPreOrder(ponteiroRaiz->esquerda);
        exibirPreOrder(ponteiroRaiz->direita);
    }
}

void exibirPosOrder(No *ponteiroRaiz) {
    //Exibe uma lista de valores pós ordem
    if (ponteiroRaiz != NULL) {
        exibirPosOrder(ponteiroRaiz->esquerda);
        exibirPosOrder(ponteiroRaiz->direita);
        printf(" %d", ponteiroRaiz->numero);
    }
}

void exibirFilhos(No *raiz, int chavePai){
    //Exibe o nó pai e os filhos dele
    if (raiz == NULL) return;
    printf("\nNo pai %i e no filho: %i",chavePai,raiz->numero);
    exibirFilhos(raiz->esquerda,raiz->numero);
    exibirFilhos(raiz->direita,raiz->numero);
}

void exibirRaizFilhos(No *raiz, int chavePai){
    //Exibe a raiz da árvore e chama a função que exibe os nós "pais" e "filhos"
    if (raiz == NULL) return;
    if(chavePai == raiz->numero){
        printf("\nNo raiz: %d", chavePai);
    }
    exibirFilhos(raiz->esquerda, chavePai);
    exibirFilhos(raiz->direita, chavePai);
}

No *consultaNo(No *raiz, int numero){
    //Faz a busca pela árvore para ver se o número desejado está na estrutura
    if (raiz == NULL) {
        return NULL;
    }
    if (raiz->numero == numero) {
        return raiz->numero;
    }
    if (raiz->numero < numero) {
        return consultaNo(raiz->direita, numero);
    }
    return consultaNo(raiz->esquerda, numero);
}

int main() {
    int valor, ordem=0, remove=1, desejaRemover, busca, num=1;
    No *raiz;

    //cria a árvore
    criar(&raiz);

    printf("\nBem-vindo a minha implementacao da arvore AVL.\nDeseja imprimir a lista de valores da arvore de qual maneira?\n");

    while(ordem != 1 && ordem !=2 && ordem!=3) {
        printf("\nDigite 1 para infixa.\nDigite 2 para prefixa.\nDigite 3 para posfixa.\n");
        scanf("%d", &ordem);
        if (ordem != 1 && ordem != 2 && ordem != 3) {
            printf("Valor invalido.\n");
        }
        else if(ordem == 1){
            printf("Apresentacao infixa selecionada.\n");
        }
        else if(ordem == 2){
            printf("Apresentacao prefixa selecionada.\n");
        }
        else{
            printf("Apresentacao posfixa selecionada.\n");
        }
    }
    printf("\nPara encerrar a insercao digite 0\n");
    printf("Digite o valor do no raiz: ");
    scanf("%d", &valor);

    while(valor != 0) {

        raiz = inserir(raiz, valor);

        printf("\nDigite o valor do proximo no:");
        scanf("%d", &valor);
    }

    printf("\nArvore atual:");
    if(ordem == 1){
        exibirInOrder(raiz);
    }
    else if(ordem == 2){
        exibirPreOrder(raiz);
    }
    else if(ordem == 3){
        exibirPosOrder(raiz);
    }

    printf("\nDeseja remover algum valor?\nDigite 1 para sim.\nDigite 0 para nao.\n");
    scanf("%d", &desejaRemover);
    if(desejaRemover == 1){
        printf("\nPara encerrar a operacao de remocao digite 0.\n");
        while(remove != 0){
            printf("\nInforme o valor a ser removido:");
            scanf("%d", &remove);
            if(remove != 0){
                raiz = remover(raiz, remove);
            }
        }
    }

    printf("\nArvore atual:");
    if(ordem == 1){
        exibirInOrder(raiz); //Exibe em ordem
    }
    else if(ordem == 2){
        exibirPreOrder(raiz); //Exibe pré ordem
    }
    else if(ordem == 3){
        exibirPosOrder(raiz); //Exibe pós ordem
    }


    printf("\nDeseja buscar um valor?\nDigite 1 para sim.\nDigite 0 para nao.\n");
    scanf("%d", &busca);
    if(busca==1){
        printf("\nPara encerrar a operacao de busca digite 0.\n");
        while(num != 0) {
            printf("\nDigite o valor que voce quer encontrar:");
            scanf("%d", &num);
            if(num !=0) {
                printf("Procurando o valor: %d", num);
                for(int i=0; i<999999999;i++);//timer para efeito estético

                if (consultaNo(raiz, num) != NULL) {
                    printf("\nO valor %d esta na arvore!\n", num);
                } else {
                    printf("\nO valor %d nao esta na arvore!\n", num);
                }
            }
        }
    }

    printf("\nExibindo a relacao final de nos 'pais' e 'filhos': ");
    exibirRaizFilhos(raiz, raiz->numero);

    printf("\n\nRepresentacao grafica da arvore:\n");
    imprimirVisual(raiz, 0);



    printf("\n\n\nPrograma de arvore AVL se encerrando.");
    printf("\n\nDestruindo a arvore.");
    for(int i=0; i<999999999;i++);//timer para efeito estético
    for(int i=0; i<999999999;i++);//timer para efeito estético
    printf(".");
    for(int i=0; i<999999999;i++);//timer para efeito estético
    for(int i=0; i<999999999;i++);//timer para efeito estético
    printf(".");
    for(int i=0; i<999999999;i++);//timer para efeito estético
    for(int i=0; i<999999999;i++);//timer para efeito estético

    destruir(&raiz); //destrói a árvore e libera a memória
    return 0;
}