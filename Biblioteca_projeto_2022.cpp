#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

typedef struct info_livro{
	char sigla;
	int reg;
}info_livro;

typedef struct aluno{
	char RA[7];
	char nome[80];
	int emprestado;
	int reservado;
	info_livro tabela[4];
}aluno;

//livro
typedef struct info_aluno{
    char sigla;
    char RA[7];
    int dia_ret;                                // dia – máx=31
    int mes_ret;                              // mes: fev=28 dias
    int dia_dev;                               // mes: abril=jun=set=nov=30 dias
    int mes_dev;                             // os demais = 31 dias
}info_aluno;

typedef struct livro{
    int reg_livro;
    char titulo[80];
    char autor[80];
    info_aluno status[2];
}livro;



//livro
void aloca_livro(livro **c,int j);
void cadastro_livro(livro *c);
int busca_vlivro(livro *c,int j);
int verifica_livro();
void grava_livro(livro *c);
void mostra_ltotal(livro *c);
void mostra_lparcial(livro *c);
void mostra_ltparcial(livro *c);

//aluno
 void aloca_aluno(aluno **p,int q);
 void cadastro_aluno(aluno *p);
 void grava_aluno(aluno *p);
 void mostra_total(aluno *p);
 int verifica();
 void mostra_parcial(aluno *p);
 
 //opção
 void maiuscula(char *aux,int tam);
 void emprestimo_reserva(aluno *p,livro *q);
 int busca_ra(aluno *p,char *aux);
 int busca_livro(livro *q,char *aux);
 int busca_livro_reserva(livro *q,char *aux );
 void atualiza_aluno(aluno *palu,int pos);
 void atualiza_livro(livro *pliv,int pos);
 void devolucao(aluno *palu,livro *pliv);
int devolve_livro(livro *pliv,char *aux);
void troca_reserva(aluno *palu,int reg_livro);
 
  
 int  main()
 {
 	setlocale(LC_ALL,"Portuguese");
 	//aluno
 	aluno *al = NULL;
 	aloca_aluno(&al,1);
 	
 	//livro
 	livro *lv = NULL;
 	aloca_livro(&lv,1);
 	
	int op;
 	do{ 
 		system("cls");
 		printf("\n\tMenu");
 		printf("\n\n[1] Cadastro Aluno \n[2] Mostra Aluno Total \n[3] Mostra Aluno Parcial \n[4] Cadastro Livro \n[5] Mostra Livro Total \n[6] Mostra Livro Status \n[7] Mostra Livro Titulo \n[8] Emprestimo \n[9] Devolução \n[10] Fim \n ");
 		scanf("%i",&op);
 		fflush(stdin);

 		switch(op){
 			case 1:
 					cadastro_aluno(al);
 					break;
 				
 			case 2:
 					mostra_total(al);
 					break;
 				
 			case 3:
 					mostra_parcial(al);
 					break;
  			case 4:
 					cadastro_livro(lv);
 					break;
 			case 5:
 					mostra_ltotal(lv);
 					break;					 
 			case 6:
 					mostra_lparcial(lv);
 					break;					 
 			case 7:
 					mostra_ltparcial(lv);
 					break;				
 			case 8:
 					emprestimo_reserva(al,lv);
 					break;	
 			case 9:
 					devolucao(al,lv);
 					break;				 	  			
 		}
	 }while(op!=10);
 }
 
 void troca_reserva(aluno *palu,int reg_livro)
{
	FILE *falu=NULL;
	int i,j,qalu,pos;
	
	qalu=verifica();
	if((falu=fopen("aluno.bin","rb+"))==NULL) //atualiza o arquivo aluno
	    printf("\nErro ao abrir o arquivo\n");
	else
	  {
	  	for(i=0;i<qalu;i++)
	  	  {
	  	    fseek(falu,i*sizeof(aluno),0);
	  	    fread(palu,sizeof(aluno),1,falu);
	  	    for (j=0;j<4;j++)     //varre as tabelas
	  	    	if((palu->tabela+j)->reg==reg_livro)
	  	      	  {
	  	      		(palu->tabela+j)->sigla='E';     //troca a reserva por empréstimo
	  	      		(palu->reservado)--;			//decrementa a qtde de reserva
	  	      		(palu->emprestado)++;			//incrementa a qtde de emprestados
	  	      		j=4;			//força a saída do for interno
	  	      		pos=i;			//guarda a posição do aluno
	  	      	    i=qalu;			//força a saída do for externo
	  	          }//if
	      }//for
	  	fseek(falu,pos*sizeof(aluno),0);
	  	fwrite(palu,sizeof(aluno),1,falu); //atualiza os dados no arquivo aluno
	  }//else
	fclose(falu);	
}//troca_reserva
 
 
 
 
 int devolve_livro(livro *pliv,char *aux) 
{
	FILE *fliv=NULL;
	int i,qliv,achou=-1;
	
	qliv=verifica_livro();
	if((fliv=fopen("livro.bin","rb"))==NULL) 
	    printf("\nErro ao abrir o arquivo\n");
	else
	  {
	  	for(i=0;i<qliv;i++)
	  	  {
	  	  	fseek(fliv,i*sizeof(livro),0);
	    	fread(pliv,sizeof(livro),1,fliv);
	  	    if(strcmp(pliv->titulo,aux)==0 && (pliv->status+0)->sigla=='E')    // compara título e status
	    	 {
	       		achou=i;
	       		i=qliv;
	    	 }//if
	      }//for
	    fclose(fliv);    
	  }//else
	return achou;
}//devolve_livro
 
 
 
 
 
 void devolucao(aluno *palu,livro *pliv)
{
	char aux_RA[7],aux_tit[80];
	int i,ind,dia, mes,achou=-1,multa=0,pos,tam,tab;
	
	printf("\nRA: ");
	gets(aux_RA);
	fflush(stdin);
	achou=busca_ra(palu,aux_RA);
	if(achou==-1)
	  printf("\nRA invalido\n\n");
	else 
	   {
	    printf("\nTitulo: ");
		gets(aux_tit);
		fflush(stdin);
		tam=strlen(aux_tit);
		maiuscula(aux_tit,tam);
		printf("\nDia atual: ");
		scanf("%i",&dia);
		fflush(stdin);
		printf("\nMes atual: ");
		scanf("%i",&mes);
		fflush(stdin);
		ind=devolve_livro(pliv,aux_tit);  //acha o livro a ser devolvido e atualiza
		if(ind==-1)
		  printf("\nTitulo invalido ou nao esta emprestado");
		else
		  {
		  	if(strcmp((pliv->status+0)->RA,aux_RA)==0)    //confere o RA 
		  	  {
		  	  	for(i=0;i<4;i++)    //varrer as tabelas do aluno p/atualização
				  if((palu->tabela+i)->reg==pliv->reg_livro)
				    {
				     (palu->tabela+i)->sigla='L';
				     (palu->tabela+i)->reg=0;
				     (palu->emprestado)--;
				     tab=i+1;     //indice do próximo elemento da tabela
				     if((palu->tabela+tab)->sigla=='E' || (palu->tabela+tab)->sigla=='R')   //troca na tabela para a posição anterior, que ficou livre
				       {
				       	(palu->tabela+i)->sigla=(palu->tabela+tab)->sigla;  //coloca na posicao que ficou livre
				        (palu->tabela+i)->reg=(palu->tabela+tab)->reg;
				        (palu->tabela+tab)->sigla='L';						//libera a posição atual
				        (palu->tabela+tab)->reg=0;
				       }//if
				      atualiza_aluno(palu,achou);	
				  	}//if
				
		  	  	if(mes<=(pliv->status+0)->mes_dev)      //está dentro do prazo de devolução
		  	      if(dia>((pliv->status+0)->dia_dev))		//passou o dia
		  	        {
		  	          multa=(dia-((pliv->status+0)->dia_dev))*3;
		  	          printf("\nMulta de: %i reais\n\n",multa);
		  	        }//if
		  	    
		  		if((pliv->status+1)->sigla=='R')     //verifica se tem reserva 
				  {
				  	strcpy((pliv->status+0)->RA,(pliv->status+1)->RA);   //as infos da reserva passam para emprestimo
				  	(pliv->status+0)->dia_ret=dia;
				  	(pliv->status+0)->mes_ret=mes;
				  	printf("\nO aluno RA: %s - trocou este livro de Reserva para Emprestimo\n", (pliv->status+0)->RA);
				  	printf("\nNOVO Dia Devolucao: ");
	  				scanf("%i",&(pliv->status+0)->dia_dev);				
	  			    fflush(stdin);
	  			    printf("\nNOVO Mes Devolucao: ");
	  			    scanf("%i",&(pliv->status+0)->mes_dev);
	  			    fflush(stdin);
				  	pos=1;
				  	troca_reserva(palu,pliv->reg_livro);//passar a reserva do NOVO aluno para emprestado
				  }//if
				else			//não tem reserva
				  {
				  	pos=0;
				  }//else
				(pliv->status+pos)->sigla='L';	//limpa as infos do emprestimo (pos=0) ou da reserva (pos=1)
				strcpy((pliv->status+pos)->RA,"      ");   
				(pliv->status+pos)->dia_ret=0;
				(pliv->status+pos)->mes_ret=0;
				(pliv->status+pos)->dia_dev=0;
				(pliv->status+pos)->mes_dev=0;  
				atualiza_livro(pliv,ind);
	
	  	      }//if
	      }//else
	  }//else
	printf("\n\n");
	system("pause");
}//devolucao

 
 
 
 
 
 
 void atualiza_aluno(aluno *palu,int pos)
{
	FILE *falu=NULL;
	
	if((falu=fopen("aluno.bin","rb+"))==NULL) //atualiza o arquivo aluno
	    printf("\nErro ao abrir o arquivo\n");
	else
	  {
	  	fseek(falu,pos*sizeof(aluno),0);
	  	fwrite(palu,sizeof(aluno),1,falu); //grava os dados no arquivo aluno
	  }//else
	fclose(falu);	
}//atualiza_aluno

void atualiza_livro(livro *pliv,int pos)
{
	FILE *fliv=NULL;
	
	if((fliv=fopen("livro.bin","rb+"))==NULL) //atualiza o arquivo livro
	    printf("\nErro ao abrir o arquivo\n");
	else
	  {
	  	fseek(fliv,pos*sizeof(livro),0);
	  	fwrite(pliv,sizeof(livro),1,fliv); //grava os dados no arquivo livro
	  }//else
	fclose(fliv);	
}//atualiza_livro

 
int busca_livro_reserva(livro *q,char *aux) 
{
	FILE *fliv=NULL;
	int i,qliv,achou=-1;
	
	qliv=verifica_livro();
	if((fliv=fopen("livro.bin","rb"))==NULL) 
	{
	    printf("\nErro ao abrir o arquivo\n");
	}
	else
	  {
	  	for(i=0;i<qliv;i++)
	  	  {
	  	  	fseek(fliv,i*sizeof(livro),0);
	    	fread(q,sizeof(livro),1,fliv);
	    	
	  	    if(strcmp(q->titulo,aux)== 0 && (q->status+1)->sigla=='L')			
	  	     {
	       		achou=i;
	       		printf("\nAchei");
	       		system("pause");
	       		i=qliv;
	    	 }
	      }
	    fclose(fliv);    
	  }//else
	  
	return achou;
}

 
 
 
 
  int busca_livro(livro *q,char *aux)
  {
	FILE *fliv=NULL;
	int i,qliv,achou=-1;
	
	qliv=verifica_livro();
	if((fliv=fopen("livro.bin","rb"))==NULL) 
	    printf("\nErro ao abrir o arquivo\n");
	else
	  {
	  	for(i=0;i<qliv;i++)
	  	  {
	  	  	fseek(fliv,i*sizeof(livro),0);
	    	fread(q,sizeof(livro),1,fliv);
	  	    if(strcmp(q->titulo,aux)<1 && (q->status+0)->sigla=='L')    // compara título e status
	    	 {
	       		achou=i;
	       		printf("\nAchei[3] ");
	       		system("pause");
	       		i=qliv;
	    	 }//if
	      }
	    fclose(fliv);    
	  }
	return achou;
  }
 
 
 
  int busca_ra(aluno *p,char *aux)
  {
	FILE *fra=NULL;
	int i,qalu,achou=-1;
	
	qalu=verifica();
	if((fra=fopen("aluno.bin","rb"))==NULL)
	  printf("\nArquivo Inexistente");
	else
	  {
	  	for(i=0;i<qalu;i++)
	  	  {
	  	    fseek(fra,i*sizeof(aluno),0);
	  	    fread(p,sizeof(aluno),1,fra);
	  	    if(strcmp(p->RA,aux)==0)
	  	      {
	  	      	achou=i;
	  	      	i=qalu;
	  	      }
	      }
	    fclose(fra);
	  }
	return achou;
  }
  
  
 void emprestimo_reserva(aluno *p,livro *q)
 {
 	char aux_ra[7],aux_tit[80];
	int ind,ind1,achou=-1,pos,tab,tam;
	
	printf("\nDigite o RA:");
	gets(aux_ra);
	fflush(stdin);
	achou=busca_ra(p,aux_ra);
	
	if(achou==1)
		printf("\nRA invalido!");
		
	else
	{
		printf("\n\nRA: %s\nNome: %s\nQtde livros emprestados: %i\nQtde de livros reservados: %i\n",p->RA,p->nome,p->emprestado,p->reservado);
		
		if(p->emprestado>3 && p->reservado>1)
	      printf("\nRA não permitido - atingiu o numero de emprestimos/reserva\n\n");
	    else
	    {
		    printf("\nTitulo do Livro: ");
			gets(aux_tit);
			fflush(stdin);
			tam=strlen(aux_tit);
			maiuscula(aux_tit,tam);
			ind=busca_livro(q,aux_tit);
			if(ind==-1)
			{
				ind1=busca_livro_reserva(q,aux_tit);
				
				printf("\nAchei %i",ind1);
	       		system("pause");
	       		
				if(ind1==-1)
					printf("\nLivro nao Encontrado!!");
				else
					ind=ind1;
			}
			
			if(ind!=-1)
	  		  {
	  			tab=(p->emprestado + p->reservado);   
				printf("\nRA: %s",p->RA);
	    		printf("\nNome: %s",p->nome);
	    		if((q->status+0)->sigla=='L')  //livro live
	  			  {
	  			  	printf("\nLivro sera emprestado - registro %i\n",q->reg_livro);
	  			  	pos=0;   //emprestimo
	  			  	(q->status+pos)->sigla= 'E';
	  			  	(p->tabela+tab)->sigla= 'E';
	  			  	(p->tabela+tab)->reg= q->reg_livro;
	  			  	(p->emprestado)++;  
	  			  	printf("\nDia Retirada: ");
	  				scanf("%i",&(q->status+pos)->dia_ret);				
	  				fflush(stdin);
	  				printf("\nMes Retirada: ");
	  				scanf("%i",&(q->status+pos)->mes_ret);				
	  				fflush(stdin);
	  			  }//if
	  			else
	  			  {
	  			  	printf("\nLivro sera reservado - registro %i\n",q->reg_livro);
	  			  	pos=1;   //reserva
	  			  	(q->status+pos)->sigla= 'R';	
	  			  	(p->tabela+tab)->sigla= 'R';
	  			  	(p->tabela+tab)->reg= q->reg_livro;
	  			  	(p->reservado)++;  
	  			  	(q->status+pos)->dia_ret=((q->status+0)->dia_dev)+1;  
	  			  	(q->status+pos)->mes_ret=(q->status+0)->mes_dev;
					printf("\nDia Retirada: %i", (q->status+pos)->dia_ret);				
	  				printf("\nMes Retirada: %i", (q->status+pos)->mes_ret);				
	  			  }//else
				strcpy((q->status+pos)->RA,aux_ra);	
				printf("\nDia Devolucao: ");
	  			scanf("%i",&(q->status+pos)->dia_dev);				
	  			fflush(stdin);
	  			printf("\nMes Devolucao: ");
	  			scanf("%i",&(q->status+pos)->mes_dev);				
	  			fflush(stdin);
	  			atualiza_livro(q,ind);
				atualiza_aluno(p,achou);
	  	      }//if
		}
	}
	printf("\n\n");
	system("pause");
 }
 
 
 
 void maiuscula(char *aux,int tam)
{
int i;
for(i=0;i<tam;i++)
  if(*(aux+i)>='a'&&*(aux+i)<='z')
    *(aux+i)-=32;
}//maiuscula
 
 void mostra_ltparcial(livro *c)
 {
 	FILE *ftll=NULL;
  	int aux,i,j,x;
  	char nome[100];
	aux=verifica_livro();
	
  	if((ftll=fopen("livro.bin","rb"))==NULL)
		{
  		printf("\nErro ao abrir o arquivo\n\n");
  		system("pause");
 		}
	else
	{
		printf("\nDigite o Titulo do livro:");
		gets(nome);
		fflush(stdin);
		
		for(i = 0; i < aux; i++)
		{
			fseek(ftll, i*sizeof(livro),0);
			fread(c,sizeof(livro), 1, ftll);

				if(strcmp(c->titulo,nome)==0)
				{
					printf("\tLivro Encontrado!!!\n Titulo: %s\n Autor: %s\n Registro do Livro: %i\n\n\n", c->titulo, c->autor, c->reg_livro);
						for(j = 0; j < 2; j++)
							{
								printf("RA do Aluno: %s \t Sigla: %c \t Retirado: %i / %i \t Devolucao: %i / %i \n\n\n\n",(c->status+j)->RA, (c->status+j)->sigla, (c->status+j)->dia_ret, (c->status+j)->mes_ret, (c->status+j)->dia_dev, (c->status+j)->mes_dev);	
							}
				}		
		}
		fclose(ftll);
		system("pause");
	 }
 }
 
 
 void mostra_lparcial(livro *c)
 {
 	FILE *fpl=NULL;
  	int aux,i,j,x,y;
  	char r;
	aux=verifica_livro();
  	if((fpl=fopen("livro.bin","rb"))==NULL)
		{
  		printf("\nErro ao abrir o arquivo\n\n");
  		system("pause");
 		}
	else
	{
		printf("\nDigite o Status do livro:");
		scanf("%c",&r);
		fflush(stdin);
		for(i = 0; i < aux; i++)
		{
			fseek(fpl, i*sizeof(livro),0);
			fread(c,sizeof(livro), 1, fpl);
			
			
			for(x = 0; x < 2; x++)
			{
				if((c->status+x)->sigla==r)
				{
					if(c->reg_livro!=y)
					{
						printf("\tLivro Encontrado!!!\n Titulo: %s\n Autor: %s\n Registro do Livro: %i\n\n\n", c->titulo, c->autor, c->reg_livro);
						y=c->reg_livro;
						if((c->status+x)->sigla==r)
							for(j = 0; j < 2; j++)
								{
									if((c->status+j)->sigla==r)
									{
										printf("RA do Aluno: %s \t Sigla: %c \t Retirado: %i / %i \t Devolucao: %i / %i \n\n\n\n",(c->status+j)->RA, (c->status+j)->sigla, (c->status+j)->dia_ret, (c->status+j)->mes_ret, (c->status+j)->dia_dev, (c->status+j)->mes_dev);	
									}
								}
					}
				}
				
				
			}
				
		}
		fclose(fpl);
		system("pause");
	 }
}
 
 
 void mostra_ltotal(livro *c)
 {
 	FILE *fml=NULL;
  	int aux,i,j;
	aux=verifica_livro();
  	if((fml=fopen("livro.bin","rb"))==NULL)
		{
  		printf("\nErro ao abrir o arquivo\n\n");
  		system("pause");
 		}
	else
	{
		for(i = 0; i < aux; i++)
		{
			fseek(fml, i*sizeof(livro),0);
			fread(c,sizeof(livro), 1, fml);
			printf("\tLivro Encontrado!!!\n Titulo: %s\n Autor: %s\n Registro do Livro: %i\n\n\n", c->titulo, c->autor, c->reg_livro);
				for(j = 0; j < 2; j++)
				{
					printf("RA do Aluno: %s \t Sigla: %c \t Retirado: %i / %i \t Devolucao: %i / %i \n\n\n\n",(c->status+j)->RA, (c->status+j)->sigla, (c->status+j)->dia_ret, (c->status+j)->mes_ret, (c->status+j)->dia_dev, (c->status+j)->mes_dev);
					
				}
				
		}
		fclose(fml);
		system("pause");
	}
 }
 
 
void grava_livro(livro *c)
{
	FILE *fgl=NULL;
 	if((fgl = fopen("livro.bin","ab"))==NULL)
 		printf("\nErro\n");
 	else
 		fwrite(c,sizeof(livro),1,fgl);
 	fclose(fgl);
 	
}
 
 
int verifica_livro()
{
  	FILE *fvl = NULL;
	int cont = 0;
	if((fvl = fopen("livro.bin","rb"))==NULL)
	{
		return cont;
	}
	else{
		fseek(fvl, 0, 2);
		cont = ftell(fvl)/sizeof(livro);
		fclose(fvl);
		return cont;
	}
  }
 
 
 int busca_vlivro(livro *c,int j)
 {
 	FILE *fbv=NULL;
	int aux=-1,i;
	system("cls");
	if((fbv=fopen("livro.bin","rb"))==NULL)
	  return aux;
	else
	  {
	  	for(i=0;i<j;i++)
	  	  {
	  	  	fseek(fbv,i*sizeof(livro),0);
	  	  	fread(c,sizeof(livro),1,fbv);
	  	  	if(c->reg_livro==-1)   
	  	  	  {
	  	  	  	aux=i;  
	  	  	  	i=j;  
			  }
		  }
		  fclose(fbv); 
		  return aux;  	  
 		}
 }
 
 
 
 void cadastro_livro(livro *c)
 {
  	int j,i,aux,ind;
  	char r;
	aux=verifica_livro();   
	ind=busca_vlivro(c,aux);
	if(ind!=-1)  
 		aux=ind;
 		
 	c->reg_livro=aux+1;
 	
 	printf("\nDigite o Titulo do livro: ");
 	gets(c->titulo);
 	fflush(stdin);
 	printf("\nDigite o Autor do livro: ");
 	gets(c->autor);
 	fflush(stdin);
 	
 	
 	(c->status+0)->sigla='L';   	
	(c->status+1)->sigla='L';		
 
	grava_livro(c);
 }
 
 
 
 
 void aloca_livro(livro **c,int j)
 {
	if((*c=(livro*)realloc(*c,j*sizeof(livro)))==NULL)
  		exit(1);	
 }
 
 
 
 void mostra_parcial(aluno *p)
 {
 	FILE *fmp=NULL;
  	int aux,i,j;
  	char opc[10];
	aux=verifica();
  	if((fmp=fopen("aluno.bin","rb"))==NULL)
		{
  		printf("\nErro ao abrir o arquivo");
 		}
	else
	{
		printf("\nDigite o RA do aluno:");
		gets(opc);
		fflush(stdin);
		
		for(i = 0; i < aux; i++){
			fseek(fmp, i*sizeof(aluno),0);
			fread(p,sizeof(aluno), 1, fmp);
			if(strcmp(p->RA,opc)==0){
				printf("\n\tAluno Encontrado!!!\n RA: %s\n Nome: %s\n Livros Emprestados: %i\n Livros Reservados: %i\n", p->RA, p->nome, p->emprestado, p->reservado);
				system("pause");
				break;
			}
			
			if(p->emprestado !=0 || p->reservado !=0)
				for(j = 0; j < (p->emprestado + p->reservado); j++)
					printf("Livro: %i - %c - %i\n",j+1,(p->tabela+j)->sigla,(p->tabela+j)->reg);
		}
		fclose(fmp);
	}
 }
 
 
 
 
 //aluno
  int verifica()
  {
  	FILE *fva = NULL;
	int cont = 0;
	if((fva = fopen("aluno.bin","rb"))==NULL)
	{
		return cont;
	}
	else{
		fseek(fva, 0, 2);
		cont = ftell(fva)/sizeof(aluno);
		fclose(fva);
		return cont;
	}
  }
 
  void mostra_total(aluno *p)
  {
  	FILE *fop=NULL;
  	int aux,i,j;
	aux=verifica();
  	if((fop=fopen("aluno.bin","rb"))==NULL)
		{
  		printf("\nErro ao abrir o arquivo");
 		}
	else
	{
		for(i = 0; i < aux; i++){
			fseek(fop, i*sizeof(aluno),0);
			fread(p,sizeof(aluno), 1, fop);
			printf("\tAluno Encontrado!!!\n RA: %s\n Nome: %s\n Livros Emprestados: %i\n Livros Reservados: %i\n\n\n", p->RA, p->nome, p->emprestado, p->reservado);
			
			
			if(p->emprestado !=0 || p->reservado !=0)
				for(j = 0; j < (p->emprestado + p->reservado); j++)
					printf("Livro: %i - %c - %i\n",j+1,(p->tabela+j)->sigla,(p->tabela+j)->reg);
		}
		fclose(fop);
		system("pause");
	}
  }
 
 
 void grava_aluno(aluno *p)
 {
 	FILE *fal=NULL;
 	if((fal = fopen("aluno.bin","ab"))==NULL)
 		printf("\nErro\n");
 	else
 		fwrite(p,sizeof(aluno),1,fal);
 	fclose(fal);
 	
 }
 
 
  void cadastro_aluno(aluno *p)
  {
  	int i;
  	
  	printf("\n\tMenu");
  	printf("\nDigite o nome do Aluno:");
  	gets(p->nome);
  	fflush(stdin);
  	printf("\nDigite o RA do Aluno:");
  	gets(p->RA);
  	fflush(stdin);
  	p->reservado = 0;
  	p->emprestado = 0;
  	
  	for(i=0;i<4;i++);
  	{
  		(p->tabela+i)->sigla = 'L';
	}
	
	grava_aluno(p);
  }
 
 
 
  void aloca_aluno(aluno **p,int q)
  {
  	if((*p=(aluno*)realloc(*p,q*sizeof(aluno)))==NULL)
  		exit(1);
  	
  }
