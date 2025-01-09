#include<stdio.h>
#include<locale.h>
#include<wchar.h>
#include<string.h>

#include"converter.c"

int isnotvalid(char ch);
int findbound(wchar_t *ch);
int isnotvalidwc(wchar_t ch);
int searchindex(wchar_t ch, wchar_t *itrans);

void mapping(char *map, wchar_t *itrans);
void hinditoeng(char *file1,char *file2, wchar_t *itrans);

void englishtohindi(char *file1, char *file2, wchar_t *itrans)
{
	int errno=0;
	FILE *input = fopen(file1,"r");
	if(input==NULL)
	{
		errno=1;
		fprintf(stderr,"%s file does not exits. Ensure that given input file is made correctly.\n",file1);
	}

	if(file2==NULL)
	{
		file2 = "output2";
	}
	FILE *output2 = fopen(file2,"w");

	char ch;
	int f=0;
	int i=0;
	int c=0;
	char eword[100];
	wchar_t dword[100];

	if(errno==0)
	{
		while((ch=fgetc(input))!=EOF)
		{
			while(isnotvalid(ch) && ch!=EOF)
			{
				fwprintf(output2,L"%c",ch);
				ch=fgetc(input);
			}

			if(!isnotvalid(ch) && ch!=EOF)
			{
				i=0;
				while((!isnotvalid(ch)) && ch!=EOF)
				{
					eword[i]=ch;
					i++;
					ch = fgetc(input);
				}
				eword[i]='\0';
				converter(eword,dword,itrans);
				i=0;
				while(dword[i+1]!=L'\0')
				{
					fwprintf(output2,L"%lc",dword[i]);
					i++;
				}	
				fseek(input,-1,SEEK_CUR);
			}
		}
	}
	
	fclose(input);
	fclose(output2);	
}

int isnotvalidwc(wchar_t ch)
{
	int i=0;
	if(ch == 10 || ch == 9 || ch == 32)
	{
		i=1;
	}

	return i;
}

int anothersearchindex(wchar_t ch,wchar_t *itrans,int i)
{
	int f=-2;
	int count=0;

	while(count!=1 && f==-2)
	{
		if(itrans[i] == L'<')
		{   
			count++;
		}

		if(ch==itrans[i] && count!=3)
		{   
			f=i;
		}
		i++;
	}

	return f+1;
}

int searchindex(wchar_t ch, wchar_t *itrans)
{
	int i = 0;
	int f = -2;
	int count = 0;
	if(ch<2305 || ch>2421)  //unicode range of devnagari characters
	{   
		f=-3;
	}

	while(count!=5 && f==-2)
	{
		if(itrans[i] == L'<')
		{   
			count++;
		}

		if(ch==itrans[i] && count!=3)
		{	
			f=i;
		}
		i++;
	}

	return f+1;
}



void hinditoeng(char *file1,char *file2, wchar_t *itrans)
{
	int errno=0;

	FILE *input = fopen(file1,"r");

	if(file2 == NULL)
	{
		file2="output";
	}

	FILE *output = fopen(file2,"w");

	if(input==NULL)
	{
		errno=1;
		fprintf(stderr,"%s file does not exits. Ensure that given input file is made correctly.\n",file1);
	}

	wchar_t ch;
	int i;

	int bound = findbound(itrans); 	//@vowels

	if(errno==0)
	{	
		while((ch=fgetwc(input))!=WEOF)
		{

			if(isnotvalidwc(ch))
			{
				fprintf(output,"%c",ch);
			}

			if(!isnotvalidwc(ch) && errno==0)
			{

				i=searchindex(ch,itrans);
				
				if(i==-1)
				{
					i=anothersearchindex(ch,itrans,bound);
				}

				if(i>bound && i!=-1 && ch != 2306)
				{
					//push the pointer back by 1 character
					fseek(output,-1,SEEK_CUR);
				}

				while(itrans[i]!=';'&& itrans[i]!='/' && i!=-1)
				{
					fprintf(output,"%c",itrans[i]);
					i++;
				}	

			}
		}

		fclose(input);
		fclose(output);
	}
}


int isnotvalid(char ch)
{
	int i=0;
	if(ch == 10 || ch == 9 || ch == 32)
	{
		i=1;
	}
	return i;
}


void mapping(char *map, wchar_t *itrans)
{
	FILE *file = fopen(map,"r");
	int errno=0;

	if(file==NULL)
	{	
		errno=1;
		fprintf(stderr,"%s file does not exits. Ensure that given mapping file is made correctly.\n",map);
	}

	wchar_t ch;
	int i=0;

	while((ch = fgetwc(file))!=WEOF && errno==0)
	{
		if(!isnotvalid(ch))
		{
			itrans[i]=ch;
			i++;
		}

	}

	itrans[i]=L'\0';
	fclose(file);
}

int validate_deva(char *file1)
{
	FILE *input = fopen(file1,"r");

	if(input == NULL)
	{
		fprintf(stderr,"Error While Opening File : %s\n",file1);
		return 1;
	}

	wint_t ch;

	while((ch=fgetwc(input))!= WEOF)
	{
		if((ch < 2305 || ch > 2421) && (ch != 32 && ch!= 44 && ch != 10 && ch!= 9 && ch != 124))
		{
			fprintf(stderr,"Error : Input file must contain devanagri characters.\n");
			fclose(input);
			return 1;
		}
	}

	fclose(input);
	return 0;
}

int validate_eng(char *file1)
{
	FILE *input = fopen(file1,"r");

	if(input == NULL)
	{
		fprintf(stderr,"Error While Opening File : %s\n",file1);
		return 1;
	}

	int ch;

	while((ch=fgetc(input))!= EOF)
	{
		if(ch > 127)
		{
			fprintf(stderr,"Input file must contain english characters.\n");
			fclose(input);
			return 1;
		}
	}

	fclose(input);

	return 0;
}

int main(int arg, char *argv[])
{
	setlocale(LC_ALL,"");
	int cmp;
	if(arg<4)
	{
		fprintf(stderr,"Format is : ./a.out <flag> <mapping_file> <input_file> <output_file>\n");
		return 1;
	}

	cmp = strcmp("-eh",argv[1]);
	if(cmp==0)
	{
		cmp = validate_eng(argv[3]);
		if(cmp==1)
		{
			return 1;
		}
		wchar_t itrans[800];
		mapping(argv[2],itrans);
		englishtohindi(argv[3],argv[4],itrans);
	}		

	cmp = strcmp("-he",argv[1]);	
	if(cmp==0)
	{
		cmp = validate_deva(argv[3]);
		if(cmp==1)
		{
			return 1;
		}
		wchar_t itrans[800];
		mapping(argv[2],itrans);
		hinditoeng(argv[3],argv[4],itrans);
	}
	return 0;
}
