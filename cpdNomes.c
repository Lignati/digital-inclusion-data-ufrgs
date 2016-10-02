/*EXPLICAÇÃO DOS ALGORITMOS IMPLEMENTADOS PELO
GRUPO CLEITONANIQUILACAOTOTALDAEXISTENCIA E AMIGOS


Todos os algoritmos exceto o radix foram implementados da mesma maneira
para as chaves categóricas e numéricas


INSERTION SORT


O insertion sort utilizado foi o tradicional que considera o primeiro elemento
ordenado e assim vai inserindo os subsequentes nesse array.


INSERTION BINÁRIO

O insertion sort binário utilizado foi o tradicional que seleciona o elemento mediano do vetor e o separa em vetores
menores até encontrar a posição de inserção correta.


SHELL

O shell sort utilizado parte do princípio da subdivisão do vetor em partes menores e a ordenação desses arranjos
até que não haja mais subdivisões a serem feitas.



BUBBLE OTIMIZADO

O bubble sort implementado foi o tradicional, em que a bolha vai envolvendo os elementos e comparanado 2 a 2 dois
até que eles estejam ordenados, com a otimização ad-hoc de quando nao houver trocas em uma passagem do array ele 
o considera ordenado


HEAP SORT


Heap sort tradicional utilizando de funções auxiliares build heap e heapfy para ordenar o array a partir de um ABP Heap.


Merge Sort
Define arrays de tamanho minimo e os classifica por intercalação recursivamente. Utiliza um array auxiliar para classificar as chaves.

QUICK SORT RANDOMIZADO


Função de quick sort tradicional com o diferencial de selecionar o pivo aleatoriamente a cada partição




RADIX CATEGORICO

Utiliza o algoritmo bubble tradicional sort para cada digito das palavras.


RADIX NUMEROS


Utiliza bubble sort otimizado para cada digito dos numeros. Criando os buckets no abstratamente por meio de chamadas recursivas.*/

#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <string.h>
#define NUM_DADOS 100000
#define MAX_AUX_VALOR 10

struct s_dado {
    int ano;
    int mes;
    int codigo;
    char nome[250];
    char UF[2];
    int T_prest_int_f; //Nº total de prestadores (autorizadas) de acessos de internet fixa (SCM)
    int T_acesso_int_f; //Nº total de acessos de internet fixa (SCM) em serviço
    int Ass_TV_ass;
    int Cobert_3g;
};

void salto(FILE* entrada, int n_saltos)
{
    char c;
    int i;

    for (i=0; i<= n_saltos; i++)
        while (c != ';')
            c = fgetc(entrada);
}

void inicializa_dado(struct s_dado* dado)
{
    int i;
    dado->ano = 0;
    dado->mes = 0;
    dado->codigo = 0;
    for (i=0;i<250;i++)
        dado->nome[i] = 0;
    dado->UF[0] = dado->UF[1] = 0;
    dado->T_prest_int_f = 0;
    dado->T_acesso_int_f = 0;
    dado->Ass_TV_ass = 0;
    dado->Cobert_3g = 0;
}
int escreveArq(char* nArqSaida, struct s_dado* dado){
    FILE* saida;
    int i;
    saida = fopen(nArqSaida,"w+t");
    if(!saida){

        return 1;

    }
    for(i = 0;i< NUM_DADOS;i++){

        fprintf(saida,
               "Ano %d\nMes %d\ncodigo %d\nNome %s\nUF %s\nNumero de fornecedores de internet fixa %d\nNumero de acesso a internet fixa %d\n "
                ,dado[i].ano,dado[i].mes,dado[i].codigo,dado[i].nome,dado[i].UF,dado[i].T_prest_int_f, dado[i].T_acesso_int_f);


        if (dado[i].Cobert_3g)
            fprintf(saida,"3G: sim\n");
        else
            fprintf(saida,"3G: nao\n");
        fprintf(saida,"\n\n\n\n");
    }
    fclose(saida);
    return 0;


}

int main(int argc, char* argv[])
{
    struct s_dado* dado;
    dado = malloc(sizeof(struct s_dado)*NUM_DADOS);
    char carac;

    FILE *entrada;

    char nArqEntrada[16];
    int i=0;
    int j=0;
    char aux_mes[2]= {};
    char ano_aux[5]={0};
    char aux_cod[7] = {0};
    char aux_valor[MAX_AUX_VALOR] = {0};
    int c_max, k;
    strcpy(nArqEntrada,argv[1]);
    if ((entrada = fopen(nArqEntrada, "r")) == NULL)
        printf("Erro ao abrir arquivo");
    printf("ASSEMBLYING DATA\n");
    while (!feof(entrada) && i<NUM_DADOS)
    {
        //inicializando struct
        inicializa_dado(&dado[i]);

//==================ANO===================
        j=0;
        carac = fgetc(entrada);
        while (carac != ';')
        {
            ano_aux[j] = carac;
            j++;
            carac = fgetc(entrada);
        }
        ano_aux[4] = '\0';
        dado[i].ano = atoi(ano_aux);
        //printf("Ano %d\n", dado[i].ano);
//==================MES===================
        aux_mes[0] = fgetc(entrada);
        if( (carac = fgetc(entrada)) == ';')
            dado[i].mes = aux_mes[0] - 48;
        else
        {
            aux_mes[1] = carac;
            dado[i].mes = atoi(aux_mes);
        }
        //printf("Mes %d\n", dado[i].mes);

//==================CODIGO===================
        j=0;
        carac = fgetc(entrada);
        while (carac != ';') //
        {
            aux_cod[j] = carac;
            j++;
            carac = fgetc(entrada);
        }
        aux_cod[7] = '\0';
        dado[i].codigo = atoi(aux_cod);
        //printf("codigo %d\n", dado[i].codigo);

//==================NOME===================
        j=0;
        carac = fgetc(entrada);
        while (carac != ';') //
        {
            dado[i].nome[j] = carac;
            j++;
            carac = fgetc(entrada);
        }
        dado[i].nome[j] = '\0';
        //printf("Nome %s\n", dado[i].nome);

//==================salto codigo UF===================
        salto(entrada,1);


//==================UF===================
        j=0;
        carac = fgetc(entrada);
        while (carac != ';')
        {
            dado[i].UF[j] = carac;
            j++;
            carac = fgetc(entrada);
        }

        //printf("UF %s\n", dado[i].UF);

//==============================================================
//===Nº total de prestadores (autorizadas) de acessos de internet fixa (SCM)===
//==============================================================
        j=0; c_max =1;
        for (k=0; k<MAX_AUX_VALOR;k++)
            aux_valor[k]=0;

        carac = fgetc(entrada);
        while (carac != ';')
        {
            aux_valor[j] = carac;
            j++;
            carac = fgetc(entrada);
        }
        j--;
        while(j+1) //ajustar vetor para conversao
        {
            aux_valor[MAX_AUX_VALOR-c_max] = aux_valor[j];
            aux_valor[j] =0;
            j--; c_max++;
        }
        dado[i].T_prest_int_f = atoi(aux_valor); //transferir valor para o campo
       // printf("Numero de fornecedores de internet fixa %d\n", dado[i].T_prest_int_f);

//==============================================================
//===Nº total de acessos de internet fixa (SCM) em serviço======
//==============================================================
        j=0; c_max =1;
        for (k=0; k<MAX_AUX_VALOR;k++)
            aux_valor[k]=0;

        carac = fgetc(entrada);
        while (carac != ';')
        {
            aux_valor[j] = carac;
            j++;
            carac = fgetc(entrada);
        }
        j--;
        while(j+1) //ajustar vetor para conversao
        {
            aux_valor[MAX_AUX_VALOR-c_max] = aux_valor[j];
            aux_valor[j] =0;
            j--; c_max++;
        }
        dado[i].T_acesso_int_f = atoi(aux_valor); //transferir valor para o campo
        //printf("Numero de acesso a internet fixa %d\n", dado[i].T_acesso_int_f);

//==============================================================
//==============================================================
        //salto Município já atendido pelo PNBL;
        //salto Nº de telefones de uso público (TUP) em serviço;
        //salto  Nº de assinantes de TV por assinatura (TVC, MMDS e TVA)
        salto(entrada,3);
//==============================================================
//==============================================================


//==================================================================================
        //Nº total de assinantes de TV por assinatura (TVC, MMDS, TVA e DTH)
//==================================================================================
        j=0; c_max =1;
        for (k=0; k<MAX_AUX_VALOR;k++)
            aux_valor[k]=0;

        carac = fgetc(entrada);
        while (carac != ';')
        {
            aux_valor[j] = carac;
            j++;
            carac = fgetc(entrada);
        }
        j--;
        while(j+1) //ajustar vetor para conversao
        {
            aux_valor[MAX_AUX_VALOR-c_max] = aux_valor[j];
            aux_valor[j] =0;
            j--; c_max++;
        }
        dado[i].Ass_TV_ass = atoi(aux_valor); //transferir valor para o campo
        //printf("Numero total de assinantes de TV por assinatura %d\n", dado[i].Ass_TV_ass);

//==============================================================
//==============================================================
        //salto Nº de assinantes de TV por assinatura via satélite (DTH);
        //salto Nº de prestadores de serviço de TV por assinatura (TVC, MMDS e TVA);
        salto(entrada,2);
//==============================================================
//==============================================================

//==============================================================
//Município coberto por internet banda larga móvel (SMP tecnologia 3G)
//==============================================================
        carac = fgetc(entrada);

        if (carac == '1' )
            dado[i].Cobert_3g = 1;
        else
            dado[i].Cobert_3g = 0;

       // if (dado[i].Cobert_3g)
           // printf("3G: sim\n");
        //else
            //printf("3G: nao\n");


        //SALTAR TODOS OS OUTROS52
        //salto(entrada,51);
        //ultimo registro
        do
        {
            carac = fgetc(entrada);
            //printf("%c", carac);
        }while (carac != '\n');


        i++;
        //printf("%d", i);

    }



    //escreveArq("cleitao.txt", dado); //verificaORD(dado, NUM_DADOS);
    insertion(dado,NUM_DADOS);   verificaORD(dado, NUM_DADOS);
    //insertionBin(dado,NUM_DADOS);  verificaORD(dado, NUM_DADOS);
    //shell(dado, NUM_DADOS);     verificaORD(dado, NUM_DADOS);
    //bubble(dado, NUM_DADOS);    verificaORD(dado, NUM_DADOS);
    //heap(dado,NUM_DADOS);   verificaORD(dado, NUM_DADOS);
    //preMerge(dado,0,NUM_DADOS); verificaORD(dado, NUM_DADOS);
    //preQuick(dado,0,NUM_DADOS); verificaORD(dado, NUM_DADOS);
    //radix(dado, NUM_DADOS);  verificaORD(dado, NUM_DADOS);
    return 0;
}
void verificaORD (struct s_dado* dado, int tamanho){
    int i; int erro=0;

    for (i=0; i<NUM_DADOS-1; i++)
    {
        if (strcmp(dado[i].UF, dado[i+1].UF) < 0)
        {       printf("\nerro %s", dado[i].UF);
                printf("\nerro %s\n", dado[i].nome);
                erro = 1;

        }
    }
    if (!erro)
        printf("  Verificacao ok\n\n");

}


void insertion(struct s_dado* dado , int tamanho){
	int i, j;
	struct s_dado aux;
	clock_t start, end, elapsed;
	
	start=0;end=0;
	start=clock();

	for (i = 1; i < tamanho; i++)
	{
		aux = dado[i];




		j = i - 1;

		while ((j >= 0) && ((strcmp(aux.UF,dado[j].UF)>0)))
		{
			dado[j + 1] = dado[j];



            j = j - 1;
		}

		dado[j + 1] = aux;

	}

    end=clock(); //le o tempo final
    elapsed = 1000 * (end - start) / (CLOCKS_PER_SEC);
    printf("\nO tempo gasto na ISERTION SORT COM BUSCA LINEAR foi de %ld ms\n",elapsed);

    if(escreveArq("saida/string_inserLin.txt",dado) == 1){

        printf("ERRO NO INSERTION SORT!!!!!!!!!!!\n");

    }
    else
        printf("INSERTION SORT DONE\n");


}
void insertionBin(struct s_dado* dados, int tamanho){
    int i,j;
    struct s_dado aux;
    clock_t start, end, elapsed;
	
    start=0;end=0;
    start=clock();

    for(i = 1;i<NUM_DADOS;i++){

        aux = dados[i];
        j = i-1;



        while((j>=0)&& (strcmp(aux.UF, dados[j].UF) >0)){
            dados[j+1] = dados[j];
            j --;


        }
        dados[j+1] = aux;
    }

    end=clock(); //le o tempo final
    elapsed = 1000 * (end - start) / (CLOCKS_PER_SEC);
    printf("\nO tempo gasto na ISERTION SORT COM BUSCA BINARIA foi de %ld ms\n",elapsed);

    if(escreveArq("saida/string_insertBin.txt",dados) == 1){

        printf("ERRO NO INSERTION SORT BINARIO!!!!!!!!!!!\n");

    }
    else
        printf("BINARY INSERTION SORT DONE\n");
}
void shell(struct s_dado* dados, int tamanho){
    int i,j;
    int intervalo, largura;
    struct s_dado valor;
    clock_t start, end, elapsed;
	
    start=0;end=0;
    start=clock();


    while(intervalo < NUM_DADOS/3)
        intervalo = intervalo*3 +1;

    while(intervalo> 0)
    {
        for (i = intervalo; i<NUM_DADOS; i++)
        {
            valor = dados[i];
            j=i;

            while(j> intervalo-1 && (strcmp(dados[j-intervalo].UF, valor.UF)<=0))
            {
                dados[j]= dados[j-intervalo];
                j=j-intervalo;
            }
            dados[j] = valor;
        }
        intervalo = (intervalo-1)/3;
    }
    end=clock(); //le o tempo final
    elapsed = 1000 * (end - start) / (CLOCKS_PER_SEC);
    printf("\nO tempo gasto no SHELL SORT foi de %ld ms\n",elapsed);

    if(escreveArq("saida/string_shell.txt",dados) == 1){

        printf("ERRO NO SHELL!!!!!!!!!!!\n");

    }
    else
        printf("SHELL SORT DONE\n");



}
void bubble(struct s_dado* dados, int tamanho){
    int i,j;
    struct s_dado aux;
    clock_t start, end, elapsed;
	
    start=0;end=0;
    start=clock();

    for(i = 1; i<NUM_DADOS;i++){
        for(j=0;(j<NUM_DADOS-1);j++){

            if((strcmp(dados[j].UF, dados[j+1].UF)) <0){

                    aux = dados[j];
                    dados[j]= dados [j+1];
                    dados[j+1] = aux;

            }

        }
    }
    end=clock(); //le o tempo final
    elapsed = 1000 * (end - start) / (CLOCKS_PER_SEC);
    printf("\nO tempo gasto no BUBBLE SORT foi de %ld ms\n",elapsed);

    if(escreveArq("saida/string_bubble.txt",dados)== 1){

        printf("ERRO NO BUBBLE!!!!!!!!!!!\n");

    }
    else
        printf("BUBBLE SORT DONE\n");
}
int filhoEsq(int nodo){
    return 2*nodo + 1;
}
int filhoDir(int nodo){
    return 2*nodo + 2;
}
int menorElemento(struct s_dado* dado, int i, int j){
    if(strcmp(dado[i].UF, dado[j].UF) <=0) return i;

    return j;
}
void heapFy(struct s_dado *dado, int nodo, int len){
    if(filhoDir(nodo) < len && filhoEsq(nodo) < len){
        int maiorFilho = menorElemento(dado, filhoDir(nodo), filhoEsq(nodo));

        if((strcmp(dado[maiorFilho].UF, dado[nodo].UF))<0){
            troca(&dado[maiorFilho], &dado[nodo]);
            heapFy(dado, maiorFilho, len);
        }

    }else if(filhoEsq(nodo) < len){
        if((strcmp(dado[filhoEsq(nodo)].UF, dado[nodo].UF) <0)){
            troca(&dado[filhoEsq(nodo)], &dado[nodo]);
            return;
        }
    }

    return;
}
void buildHeap(struct s_dado* dado, int len){
    int i;
    for( i=len/2; i>= 0; i--){
        heapFy(dado, i, len);
    }
}
void heap(struct s_dado * dado, int len ){
    int i;
    clock_t start, end, elapsed;
	
    start=0;end=0;
    start=clock();

    buildHeap(dado, len);

    int lenAtual = len;
    for(i=0; i < len; i++){
        troca(&dado[0], &dado[lenAtual-1]);
        lenAtual--;

        heapFy(dado, 0, lenAtual);

    }
    end=clock(); //le o tempo final
    elapsed = 1000 * (end - start) / (CLOCKS_PER_SEC);
    printf("\nO tempo gasto no HEAP SORT foi de %ld ms\n",elapsed);

    if(escreveArq("saida/string_heap.txt",dado)== 1){

        printf("ERRO NO HEAP!!!!!!!!!!!\n");

    }
    else
        printf("HEAP SORT DONE\n");



}
void troca(struct s_dado *a, struct s_dado *b){
    struct s_dado  aux;
    aux = * a;
    *a = *b;
    *b = aux;

}
void mergeIt(struct s_dado* dados, int ini, int m, int fim){
    int i = ini;
    int j = m;
    int k = 0;
    struct s_dado* aux = malloc(sizeof(struct s_dado) * (fim-ini));


    while(i < m && j < fim){
        if((strcmp(dados[i].UF, dados[j].UF)) >= 0){
            aux[k++] = dados[i++];
        }else{
            aux[k++] = dados[j++];
        }
    }

    while(i < m){
        aux[k++] = dados[i++];
    }
    while(j < fim){
        aux[k++] = dados[j++];
    }

    memcpy(dados+ini, aux, sizeof(struct s_dado)*(fim-ini));
    free(aux);
}
void mergeSort(struct s_dado* dados, int ini, int fim){
    if(fim-ini <= 1) return;

    int m = (fim+ini)/2;
    mergeSort(dados, ini, m);
    mergeSort(dados, m, fim);
    mergeIt(dados, ini, m, fim);



}
void preMerge(struct s_dado* dados, int ini, int fim){
    clock_t start, end, elapsed;
	
    start=0;end=0;
    start=clock();

    mergeSort(dados,ini,fim);

   end=clock(); //le o tempo final
   elapsed = 1000 * (end - start) / (CLOCKS_PER_SEC);
   printf("\nO tempo gasto no MERGE SORT foi de %ld ms\n",elapsed); 
	
   if(escreveArq("saida/string_merge.txt",dados) == 1){

        printf("ERRO NO MERGE!!!!!!!!!!!\n");

    }
    else
        printf("MERGE SORT DONE\n");
}
int particione(struct s_dado *dados, int ini, int fim){
    int m = (rand()%(fim-ini))+ini;
    troca(&dados[ini], &dados[m]);
    int i;
    struct s_dado pivo = dados[ini];
    int j = ini;
    for( i=ini; i < fim; i++){

        if((strcmp(dados[i].UF, pivo.UF) >=0)){
            troca(&dados[i], &dados[j]);
            j++;
        }
    }
    troca(&dados[ini], &dados[j-1]);
    return j-1;
}
void quickSort(struct s_dado* dados, int ini, int fim){

    if(fim - ini <= 1) return;

    int p = particione(dados,ini,fim);

    if(p - ini <= fim- p - 1){
        quickSort(dados, ini, p);
        quickSort(dados, p+1, fim);
    }else{
        quickSort(dados, p+1, fim);
        quickSort(dados, ini, p);
    }
}
void preQuick(struct s_dado* dados, int ini, int fim){

    clock_t start, end, elapsed;
	
    start=0;end=0;
    start=clock();

    quickSort(dados,ini,fim);


   end=clock(); //le o tempo final
   elapsed = 1000 * (end - start) / (CLOCKS_PER_SEC);
   printf("\nO tempo gasto no QUICK SORT foi de %ld ms\n",elapsed);
  
   if(escreveArq("saida/string_quick.txt",dados) == 1){

        printf("ERRO NO QUICK!!!!!!!!!!!\n");

    }
    else
        printf("QUICK SORT DONE\n");
}


void radix(struct s_dado* dados,int tamanho){
    int i,j,k;
    struct s_dado aux;

  
    clock_t start, end, elapsed;
	
    start=0;end=0;
    start=clock();

    for(k =0;k<2;k++){
        for(i = 1; i<tamanho;i++){


            for(j=0;(j<tamanho-1);j++){

                if(dados[j].UF[k] == '\0')
                    dados[j].UF[k+1] = '\0';

                if(dados[j].UF[k]<dados[j+1].UF[k] && (verificaAnterior (dados,j,j+1,k-1) == 0 || k == 0)){
                        aux = dados[j];
                        dados[j] = dados[j+1];
                        dados[j+1] = aux;



                }

            }
        }
    }
   
	
	 end=clock(); //le o tempo final
         elapsed = 1000 * (end - start) / (CLOCKS_PER_SEC);
	 printf("\nO tempo gasto no RADIX SORT MSD foi de %ld ms\n",elapsed);

	 if(escreveArq("saida/string_radix.txt",dados) == 1){

        printf("ERRO NO RADIX!!!!!!!!!!!\n");

    }
    else
        printf("RADIX SORT DONE\n");
}

int verificaAnterior(struct s_dado* dados,int i,int j,int k){

    if(k >=0 ){
        if(dados[i].UF[k] == dados[j].UF[k])
            return (verificaAnterior(dados,i,j,k-1));

        else

            return 1;



    }
    else{return 0;

    }



}
