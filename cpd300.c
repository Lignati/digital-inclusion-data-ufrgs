#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <string.h>
#include "arvore_b.h"
#define NUM_DADOS 1000000
#define MAX_AUX_VALOR 10

struct s_dado {
    int ano;
    int mes;
    int codigo;
    char nome[250];
    char UF[3];
    int T_prest_int_f; //Nº total de prestadores (autorizadas) de acessos de internet fixa (SCM)
    int T_acesso_int_f; //Nº total de acessos de internet fixa (SCM) em serviço
    int Ass_TV_ass;
    int Cobert_3g;
    int posicao;
};
struct s_indice {
    int codigo;
    char nome[250];
    char UF[3];
    int posicao;
};
struct s_info {
    int existe;
    int T_prest_int_f; //Nº total de prestadores (autorizadas) de acessos de internet fixa (SCM)
    int T_acesso_int_f; //Nº total de acessos de internet fixa (SCM) em serviço
    int Ass_TV_ass;
    int Cobert_3g;

};
struct s_infoDB{
    int existeRegistro;
    int posicao;
    struct s_info mes[12];
};
//estrutura com os arquivos de ano
typedef struct arqAno{


    char NAno[5];
    int ano;

};
void anoEmString(int ano ,char* anoS){
    anoS[0] = '0'+( ano / 1000);
    ano %= 1000;
    anoS[1] = '0' + (ano/100);
    ano %= 100;
    anoS[2] = '0' + (ano/10);
    anoS[3] = '0'+  (ano % 10);
    anoS[4] = '\0';
}
struct s_infoDB criaRegistroBranco(){
    struct s_infoDB branco;
    int i;
    branco.posicao = 0;
    branco.existeRegistro =0;
    for(i=0;i<12;i++){
        branco.mes[i].existe = 0;
    }
    return branco;

}
struct arqAno achaAno(FILE* anosIndice,struct s_dado dado){
    struct arqAno aux;
    fseek(anosIndice, 0, SEEK_END);
    if (ftell(anosIndice) == 0)
    {
        aux.ano = 0;
        return aux;
    }
    fseek(anosIndice,0,SEEK_SET);
    while(fread(&aux, sizeof(struct arqAno), 1, anosIndice)){

        if(aux.ano == dado.ano)return aux;
    }
    aux.ano = 0;

    return aux;
}
void salvaDadosDB(FILE* anosIndice,struct s_dado dado){
    struct s_dado auxDado;
    struct arqAno busca;
    struct s_infoDB auxInfo;
    struct s_infoDB novoRegistro;
    struct s_info novaInfo;
    struct s_infoDB infoBranco;
    int diff, pos_aux;
    busca = achaAno(anosIndice,dado);
    FILE * nova;
    FILE* arquivo;
    int i;
    int iMes;
    iMes = dado.mes -1;///indice Mes



    ///seta as informacaoes para serem gravadas
     novaInfo.existe = 1;
     novaInfo.T_prest_int_f = dado.T_prest_int_f; ///Nº total de prestadores (autorizadas) de acessos de internet fixa (SCM)
     novaInfo.T_acesso_int_f = dado.T_acesso_int_f; ///Nº total de acessos de internet fixa (SCM) em serviço
     novaInfo.Cobert_3g = dado.Cobert_3g;
     novaInfo.Ass_TV_ass = dado.Ass_TV_ass;


     novaInfo.existe = 1;
    busca = achaAno(anosIndice,dado);
    infoBranco = criaRegistroBranco();
    ///faz tratamento para ver se ja existe o ano se nao existe cria arquivo
    if(busca.ano == 0){

        fseek(anosIndice, sizeof(struct arqAno), SEEK_END);
        busca.ano = dado.ano;

        anoEmString(busca.ano,busca.NAno);
        fwrite(&busca,sizeof(struct arqAno),1,anosIndice);
        printf("Busca %d\n",busca.ano);
        nova = fopen(busca.NAno,"w+");
        if(!nova)
            printf("ERRO FATAL\n");
        fclose(nova);
    }

    //printf("reg atual:%d ano %d -- pos %d -- ass%d\n", iMes, busca.ano, dado.posicao, dado.Ass_TV_ass );

    //fseek(anosIndice, sizeof(struct arqAno)*(-1), SEEK_END);
    //fread(&teste, sizeof(struct arqAno), 1, anosIndice);
     //printf("ano: %d\nanoS:%s\n", teste.ano, teste.NAno);

    ///pega o arquivo ja criado de ano e insere
    arquivo = fopen(busca.NAno,"r+b");
    if(!arquivo)
        printf("ERRO FATAL\n");



    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0) //se o arquivo estiver em branco, eu escrevo todas as infobranco antes da posicao
    {
        for(pos_aux=0; pos_aux <= dado.posicao; pos_aux++)
        {
            fwrite(&infoBranco, sizeof(struct s_infoDB), 1, arquivo);
        }

        fseek(arquivo,sizeof(struct s_infoDB)*(-1),SEEK_END);
        fread(&novoRegistro, sizeof(struct s_infoDB), 1, arquivo);
        fseek(arquivo,sizeof(struct s_infoDB)*(-1),SEEK_CUR);
        novoRegistro.mes[iMes] = novaInfo;
        novoRegistro.existeRegistro = 1;
        novoRegistro.posicao = dado.posicao;
        fwrite(&novoRegistro, sizeof(struct s_infoDB), 1, arquivo);

    }
    else
    {
        fseek(arquivo, sizeof(struct s_infoDB)*(-1), SEEK_END);
        fread(&auxInfo, sizeof(struct s_infoDB), 1, arquivo);
        if (auxInfo.posicao < dado.posicao){ //se a posicao nova for maior que a posicao do ultimo, tenho que escrever os arquivos brancos entre elas
            fseek(arquivo, 0, SEEK_END);
            for(pos_aux = auxInfo.posicao+1; pos_aux <= dado.posicao; pos_aux++)
            {
                fwrite(&infoBranco, sizeof(struct s_infoDB), 1, arquivo);
            }
            fseek(arquivo,sizeof(struct s_infoDB)*(-1),SEEK_END);
            fread(&novoRegistro, sizeof(struct s_infoDB), 1, arquivo);
            fseek(arquivo,sizeof(struct s_infoDB)*(-1),SEEK_CUR);
            novoRegistro.mes[iMes] = novaInfo;
            novoRegistro.existeRegistro = 1;
            novoRegistro.posicao = dado.posicao;
            fwrite(&novoRegistro, sizeof(struct s_infoDB), 1, arquivo);
        }
        else if (auxInfo.posicao >= dado.posicao) //Se ela for menor, já possui um arquivo branco salvo na posicao referente, logo, é apenas escrever por cima
        {
            fseek(arquivo, sizeof(struct s_infoDB)*dado.posicao, SEEK_SET);
            fread(&novoRegistro, sizeof(struct s_infoDB), 1, arquivo);
        //printf("---%d--\n", novoRegistro.mes[0].Ass_TV_ass);
            fseek(arquivo,sizeof(struct s_infoDB)*dado.posicao,SEEK_SET);
            novoRegistro.mes[iMes] = novaInfo;
            novoRegistro.existeRegistro = 1;
        //printf("%d--%d---%d\n", iMes, novaInfo.Ass_TV_ass, dado.mes);
        //printf("---%d----%d----%d\n", novoRegistro.mes[0].Ass_TV_ass, novoRegistro.mes[1].Ass_TV_ass, novoRegistro.mes[2].Ass_TV_ass);
            fwrite(&novoRegistro, sizeof(struct s_infoDB), 1, arquivo);
        }
    }



    fclose(arquivo);

}



void salto(FILE* entrada, int n_saltos)
{
    char c;
    int i;

    for (i=0; i<= n_saltos; i++)
        while (c != ';')
            c = fgetc(entrada);
}

void inicializa_dado(struct s_dado* dado){
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

struct arqAnos* conv_txt_bin (FILE *database, TipoApontador *raiz,FILE* anosIndice){  //recebe um arquivo txt e transforma em binario
    struct s_dado dado;
    char carac;
    FILE *entrada;
    struct TipoRegistro reg;
    struct s_indice indice;
    char nArqEntrada[16];
    int cont =0;
    int j=0;
    char aux_mes[2]= {};
    char ano_aux[5]={0};
    char aux_cod[7] = {0};
    char aux_valor[MAX_AUX_VALOR] = {0};
    int c_max, k;
    int pos;
    pos = 0;


    printf("\nConversão de Arquivo texto para arquivo binario\nInsira o nome do arquivo texto: ");
    scanf("%s",nArqEntrada);

    if ((entrada = fopen(nArqEntrada, "r")) == NULL)
        printf("Erro ao abrir arquivo");
    printf("ASSEMBLYING DATA\n");

    if (fgetc(entrada) == EOF)
    {
        printf("\nArquivo Vazio\n");
        return;
    }
    fseek(entrada, (-1)*sizeof(char),SEEK_CUR);


    while ((fgetc(entrada)!=EOF) && cont<NUM_DADOS)
    {

        fseek(entrada, (-1)*sizeof(char),SEEK_CUR);
        //inicializando struct
        inicializa_dado(&dado);

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
        dado.ano = atoi(ano_aux);
//==================MES===================
        aux_mes[0] = fgetc(entrada);
        if( (carac = fgetc(entrada)) == ';')
            dado.mes = aux_mes[0] - 48;
        else
        {
            aux_mes[1] = carac;
            dado.mes = atoi(aux_mes);
        }


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
        dado.codigo = atoi(aux_cod);

//==================NOME===================
        j=0;
        carac = fgetc(entrada);
        while (carac != ';') //
        {
            dado.nome[j] = carac;
            j++;
            carac = fgetc(entrada);
        }
        dado.nome[j] = '\0';

//==================salto codigo UF===================
        salto(entrada,1);


//==================UF===================
        j=0;
        carac = fgetc(entrada);
        while (carac != ';')
        {
            dado.UF[j] = carac;
            j++;
            carac = fgetc(entrada);
        }
        dado.UF[j] = '\0';

        //printf("UF %s\n", dado.UF);

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
        aux_valor[j] = '\0';
        dado.T_prest_int_f = atoi(aux_valor); //transferir valor para o campo




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
        aux_valor[j] = '\0';
        dado.T_prest_int_f = atoi(aux_valor); //transferir valor para o campo

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
        aux_valor[j] = '\0';
        dado.Ass_TV_ass = atoi(aux_valor); //transferir valor para o campo
//==============================================================
//==============================================================
        salto(entrada,2);
//==============================================================
//==============================================================

//==============================================================
//Município coberto por internet banda larga móvel (SMP tecnologia 3G)
//==============================================================
        carac = fgetc(entrada);

        if (carac == '1' )
            dado.Cobert_3g = 1;
        else
            dado.Cobert_3g = 0;


        //SALTAR TODOS OS OUTROS 52

        do
        {
            carac = fgetc(entrada);
        }while (carac != '\n' && carac != EOF);



       cont++;
        if ((cont%100000)==0 )
            printf("Numero de registros convertidos: %d\n", cont);


        //insere as informações na arvore B

        reg.Chave = dado.codigo;
        //
        int retorno = Pesquisa(&reg, *raiz);

        if ( retorno == -1)
        {
            //printf("dado nao esta: %d\n", retorno);
            dado.posicao = busca_posicao(database,dado.codigo);
        }
        else
        {
            //printf("dado ja esta na avr %d\n,", retorno);
            dado.posicao = retorno;
        }

        //printf("ano %d\n", dado.ano);


        indice.codigo = dado.codigo;
        reg.posicao = dado.posicao;
        indice.posicao = dado.posicao;

        //printf("\ndado posicao %d - dado codigo%d \n", dado.posicao, dado.codigo);
        //printf("\nindice posicao %d - indice codigo%d \n", indice.posicao, indice.codigo);
        //printf("Arquivo txt: %d --- %d\n",dado.codigo, indice.posicao );

        //printf("chave: %d, posicao: %d\n",reg.Chave,reg.posicao  );
        Insere(reg,raiz);

        //copia as informacoes da estrutura dado para a estrutura indice da DB

    	strcpy(indice.nome,dado.nome);
    	strcpy(indice.UF,dado.UF);


        fseek(database, sizeof(struct s_indice)*dado.posicao, SEEK_SET);
        fwrite(&indice, sizeof(struct s_indice),1, database);


        //dados no arquivo de anos

        salvaDadosDB(anosIndice,dado);

    }
    printf("\nConversao Finalizada\nTotal de Registros convertidos: %d\n", cont);



}

int busca_posicao (FILE* database,int codigo)
{
    struct s_indice dado_temp;
    fseek(database, 0, SEEK_END);
    if (ftell(database) == 0)
    {
        return 0;
    }

    fseek (database, sizeof(struct s_indice)*(-1), SEEK_END );
    fread(&dado_temp, sizeof(struct s_indice), 1, database);
    if(codigo == dado_temp.codigo)
        return dado_temp.posicao;
    else
        return ((dado_temp.posicao)+ 1);
}

int busca(int posicao, FILE* database,FILE* indiceAnos){ //printa na tela o registro da posicao recebida
    struct s_indice indice;
    struct s_infoDB info;
    struct arqAno anoAtual;
    FILE* arqAnoAtual;
    int i;


    fseek(database,sizeof(struct s_indice)*posicao, SEEK_SET);
    fread(&indice,sizeof(struct s_indice),1,database);

    rewind(indiceAnos);
    while(fread(&anoAtual,sizeof(struct arqAno),1,indiceAnos)){
        if(anoAtual.ano != 0){
            arqAnoAtual = fopen(anoAtual.NAno,"rb");
            fseek(arqAnoAtual,sizeof(struct s_infoDB)*posicao, SEEK_SET);
            fread(&info,sizeof(struct s_infoDB),1,arqAnoAtual);

            if(info.existeRegistro == 1){

                for(i=0;i<12;i++){
                    if(info.mes[i].existe == 1){


            printf(
            "Ano %d\nMes %d\ncodigo %d\nNome %s\nUF %s\nNumero de fornecedores de internet fixa %d\nNumero de acesso a internet fixa %d\nNumero de assinantes de TV por assinatura: %d\n"
            ,anoAtual.ano,i+1,indice.codigo,indice.nome,indice.UF,info.mes[i].T_prest_int_f, info.mes[i].T_acesso_int_f, info.mes[i].Ass_TV_ass);


        if (info.mes[i].Cobert_3g)
            printf("3G: sim\n");
        else
            printf("3G: nao\n");
        printf("\n");

                    }

                }
                system("pause");

        }

    fclose(arqAnoAtual);

        }
    }


return 0;


}

int buscaNome(FILE *database)
{
    char cidade[250];
    struct s_indice buffer;

    printf("Digite o nome da cidade que deseja pesquisar: \n");
    fflush(stdin);
    gets(cidade);
    fseek(database,0,SEEK_SET);
    while (fread(&buffer, sizeof(struct s_indice), 1, database)){
        if(strcmp(buffer.nome, cidade)==0 && buffer.posicao >0){
            return buffer.posicao;
            system("pause");
        }

    }
    return -1;
}

void montaArvore(FILE *database, TipoApontador *raiz){
	struct s_indice dado;
	struct TipoRegistro reg;
	fseek(database,0,SEEK_SET);
	while(fread (&dado,1,sizeof(struct s_indice),database)){

		reg.Chave = dado.codigo;
		reg.posicao = dado.posicao;
		Insere(reg, raiz);


	}

}

int buscaUF (FILE *database, FILE *indiceAnos){

    char UF[3];
    int salvaPos;
    struct s_indice buffer;
    int aux = -1;

    printf("Digite o nome da Unidade Federativa que deseja pesquisar: \n");
    scanf("%s", UF);

    fseek(database,0,SEEK_SET);

    while (fread(&buffer, sizeof(struct s_indice), 1, database)){
        //printf("---------------------------------------\n");
        if(strcmp(buffer.UF, UF)==0){

            salvaPos = buffer.posicao;
            busca(buffer.posicao, database, indiceAnos);
            fseek(indiceAnos,salvaPos*sizeof(indiceAnos),SEEK_SET);
            aux = 1;
            //system("pause");
        }

    }
    return aux;
}

void retiraBase(int posicao, FILE *database, FILE *anosIndice)
{
    struct s_indice indice;
    struct s_infoDB info, branco;
    struct arqAno anoAtual;
    FILE* arqAnoAtual;


    branco = criaRegistroBranco();

    rewind(anosIndice);
    while(fread(&anoAtual, sizeof(struct arqAno), 1, anosIndice)){

            arqAnoAtual = fopen(anoAtual.NAno,"rb");
            fseek(arqAnoAtual,sizeof(struct s_infoDB)*posicao, SEEK_SET);
            fwrite(&branco, sizeof(struct s_infoDB), 1, arqAnoAtual);
            fclose(arqAnoAtual);

    }

    fseek(database,sizeof(struct s_indice)*posicao, SEEK_SET);
    indice.posicao = -1;
    fwrite(&indice, sizeof(struct s_indice), 1, database);


}

void insereNovo(FILE *database, TipoApontador *raiz, FILE *anosIndice){ //insere novo registro manualmente

    struct TipoRegistro reg;
    struct s_dado new_dado;
    struct s_indice indice;

    inicializa_dado(&new_dado);
    printf("Insira os dados: \n");
    printf("Ano: ");
    scanf("%d", &new_dado.ano);
    printf("Mes(codigo): ");
    scanf("%d", &new_dado.mes);
    printf("Codigo da cidade: ");
    scanf("%d", &new_dado.codigo);
    printf("Nome da Cidade: ");
    fflush(stdin);
    gets(new_dado.nome);
    printf("UF: ");
    fflush(stdin);
    gets(new_dado.UF);
    printf("N de prestadores de acessos de internet fixa: ");
    scanf("%d", &new_dado.T_prest_int_f);
    printf("N de acessos de internet: ");
    scanf("%d", &new_dado.T_acesso_int_f);
    printf("N de assinantes de TV por assinatura: ");
    scanf("%d", &new_dado.Ass_TV_ass);
    printf("Cobertura 3g (0/1): ");
    scanf("%d", &new_dado.Cobert_3g);

    if ((database = fopen("database.bin", "a+b")) == NULL)
       printf("Erro ao abrir arquivo: database");

    reg.Chave = new_dado.codigo;
    int retorno = Pesquisa(&reg, *raiz);
    if ( retorno == -1)
    {
        new_dado.posicao = busca_posicao(database,new_dado.codigo);
    }
    else
    {
        new_dado.posicao = retorno;
    }

    indice.codigo = new_dado.codigo;
    reg.posicao = new_dado.posicao;
    indice.posicao = new_dado.posicao;
    strcpy(indice.nome,new_dado.nome);
    strcpy(indice.UF,new_dado.UF);
    Insere(reg, raiz);


    fseek(database, sizeof(struct s_indice)*indice.posicao, SEEK_SET);
    fwrite(&indice, sizeof(struct s_indice),1, database);

    salvaDadosDB(anosIndice,new_dado);
    printf("Novo registro inserido\n");

    fclose(database);

}

 void busca_mult(FILE *database, FILE *anosIndice, TipoApontador *raiz)
 {
     struct TipoRegistro reg;
     printf("1\n");
     int cod[100], cont;

     for(cont = 0; cont<100; cont++)
        cod[cont] = 0;

    printf("2\n");

     cont=0;
     printf("Digite o codigo para realizar a busca + enter (sair = 0)\n");
     scanf( "%d", &cod[cont]);
     while(cod[cont])
     {
         cont++;
         scanf( "%d", &cod[cont]);
     }
     cont = 0;
     int pos;
     while(cod[cont])
     {
         reg.Chave = cod[cont];
         pos = Pesquisa(&reg, *raiz);
         printf("%d -- %d\n", pos, cod[cont]);
         busca( pos, database, anosIndice);
         cont++;
     }



 }


void ordenaCodigos(FILE *database, FILE *anosIndice, TipoApontador *raiz){


      struct TipoRegistro reg;
     printf("1\n");
     int cod[100], cont;

     for(cont = 0; cont<100; cont++)
        cod[cont] = 0;

    printf("2\n");

     cont=0;
     printf("Digite o codigo para realizar a busca + enter (sair = 0)\n");
     scanf( "%d", &cod[cont]);
     while(cod[cont])
     {
         cont++;
         scanf( "%d", &cod[cont]);
     }
     int resp;
     printf("Ordenar registros de forma (1)CRESCENTE OU (2)DECRESCENTE?\n");
     scanf("%d", &resp);
     shellSort(cod, cont);

    if(resp == 1)
     {
        cont = 0;
        int pos;
        while(cod[cont])
        {
            reg.Chave = cod[cont];
            pos = Pesquisa(&reg, *raiz);
            busca( pos, database, anosIndice);
            cont++;
        }
     }
    else if(resp == 2)
    {
        cont = cont -1;
        int pos;
        while(cont+1)
        {
            reg.Chave = cod[cont];
            pos = Pesquisa(&reg, *raiz);
            busca( pos, database, anosIndice);
            cont--;
        }


    }

}
void shellSort(int vet[], int size) {
    int i , j , value;
    int gap = 1;
    while(gap < size) {
        gap = 3*gap+1;
    }
    while ( gap > 1) {
        gap /= 3;
        for(i = gap; i < size; i++) {
            value = vet[i];
            j = i - gap;
            while (j >= 0 && value < vet[j]) {
                vet [j + gap] = vet[j];
                j -= gap;
            }
            vet [j + gap] = value;
        }
    }
}

int main()
{
    struct arqAnos *listaAnos;
    int valor_requerido, aux;
    int resposta;
    TipoRegistro reg;
    FILE* database;
    FILE* anosIndice;
    TipoPagina *raiz;
    inicializa_raiz(&raiz);
    listaAnos = NULL;
    database = fopen("database.bin", "r+b");
    anosIndice = fopen("anosIndice.bin","r+b");
    if (!database || !anosIndice){
        database = fopen("database.bin","w+b");
        anosIndice  = fopen("anosIndice.bin","w+b");
        conv_txt_bin (database,&raiz,anosIndice);
        if(!database || !anosIndice){
            printf("ERRO FATAL TERMINANDO PROGRAMA\n");
            return 2;
        }
    }
    else{
            montaArvore(database,&raiz);
    }
    do{
        printf("\n\nSelecione uma opcao:\n\nRealizar busca por Codigo: \t1\nRealizar busca por nome: \t2\nRealizar busca por UF: \t\t3\nRemover registro: \t\t4\nInserir registro: \t\t5\nBuscar mais de um codigo: \t6\nOrdenar Codigo\t\t\t7\nImprimir Arvore\t\t\t8\nSair: \t\t\t\t0\n");
        scanf("%d", &resposta);

        switch (resposta)
        {
        case 1:
            printf("Digite o codigo do registro: ");
            scanf("%d", &valor_requerido);
            reg.Chave = valor_requerido;
            aux = Pesquisa(&reg, raiz);
            if (aux>=0)
                busca(aux,database,anosIndice);
            else
                printf("Dado nao encontrado");
            break;

        case 2:
            aux = buscaNome(database);
            if (aux >= 0)
                busca(aux, database, anosIndice);
            else
                printf("Dado nao encontrado!\n");

            break;

        case 3:
            aux = buscaUF (database, anosIndice);
            if (aux < 0)
                printf("Dado nao encontrado!\n");

            break;

        case 4:
            printf("Digite o codigo a ser removido: ");
            scanf("%d", &valor_requerido);
            retiraBase(valor_requerido, database, anosIndice);
            Retira(valor_requerido, &raiz);
            break;

        case 5:
            insereNovo(database, &raiz, anosIndice);
            break;

        case 6:
            busca_mult(database, anosIndice, &raiz);
            break;
        case 7:
             ordenaCodigos(database, anosIndice, &raiz);

        case 8:
            Imprime(raiz);
            break;

        }
    }while(resposta);



    fclose(database);
    return 0;
}


