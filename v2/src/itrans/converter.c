#include<stdio.h>
#include<locale.h>
#include<wchar.h>
#include<string.h>

int findbound(wchar_t *ch);
int searchindex(wchar_t ch, wchar_t *itrans);

int mystrcmp(char *a,char *b)
{
	int i=0;
	int f=0;
	while(a[i]!= '\0' && f==0)
	{
		if(a[i]!=b[i])
		{
			f=1;
		}
		i++;
	}
	return f;
}

int isValidIrregularConstant(char *ca, wchar_t *itrans,int i)
{
	int j=0;
	int f=0;
	int cmp=-1;
	int count=0;
	char charr[20];

	while(itrans[i]!=L'>')
	{
		i++;
	}
	
	i++;
	
	while(count!=1 && f==0)
	{
		if(itrans[i]==L'<')
		{
			count++;
		}
				
		if(itrans[i]<128 && count!=1)
		{
			j=0;
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j]=itrans[i];
				i++;
				j++;
			}	
			
			charr[j]='\0';
			cmp=mystrcmp(ca,charr);
			if(cmp==0)
			{
				f=2;
			}
		}
		i++;
	}
	return f;
}

int isValidConsonantWithNuqta(char *ca, wchar_t *itrans,int i)
{
	int j=0;
	int f=0;
	int cmp=-1;
	int count=0;
	char charr[20];

	while(itrans[i]==L'>')
	{
		i++;
	}
	
	i++;	
	
	while(count!=1 && f==0)
	{
		if(itrans[i]==L'<')
		{
			count++;
		}
				
		if(itrans[i]<128 && count!=1)
		{
			j=0;
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j]=itrans[i];
				i++;
				j++;
			}	
			
			charr[j]='\0';
			cmp=mystrcmp(ca,charr);
			if(cmp==0)
			{
				f=3;
			}
		}
		i++;
	}

	if(f==0)
	{
		f=isValidIrregularConstant(ca,itrans,i);
	}	

	return f;
}
	

int isValidAtVowel(char *ca, wchar_t *itrans)
{
	int bound=findbound(itrans);
	int i=bound+1;
	int j=0;
	int f=0;
	int cmp=-1;
	int count=0;
	char charr[20];
	
	while(count!=1 && f==0)
	{
		if(itrans[i]==L'<')
		{
			count++;
		}

		if(itrans[i]<128 && count!=1)
		{
			j=0;
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j] = itrans[i];
				i++;
				j++;
			}
		
			charr[j]='\0';
			cmp=mystrcmp(ca,charr);
			if(cmp==0)
			{
				f=2;
			}
		}
		i++;
	}

	return f;
}

int isValidVowel(char *ca, wchar_t *itrans)
{	
	int i=0;
	int j=0;
	int count=0;
	char charr[20];
	int f=0;
	int cmp=-1;
	while(count!=2 && f==0)
	{
		if(itrans[i]==L'<')
		{
			count++;
			while(itrans[i]!=L'>')
			{
				i++;
			}
			i++;
		}
		
		if(itrans[i]<128 && count !=2)
		{
			j=0;
			
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j]=itrans[i];
				j++;
				i++;
			}
			
			charr[j]='\0';
			cmp=mystrcmp(ca,charr);
			if(cmp == 0)
			{
				f=1;
			}
			
		} 
		i++;
	}

	if(f==0)
	{
		f=isValidAtVowel(ca,itrans);
	}
	
	return f;
}

int isValidConsonant(char *ca, wchar_t *itrans)
{
	int i=0;
	int j=0;
	int count=0;
	int f=0;
	int cmp=-1;
	char charr[20];
	
	while(count!=2)
	{	
		if(itrans[i]==L'<')
		{
			count++;
		}
		i++;
	}
	
	while(itrans[i]!=L'>')
	{
		i++;
	}
	
	i++;	
	
	while(count!=3 && f==0)
	{
		if(itrans[i]==L'<')
		{
			count++;
		}
		
		if(itrans[i]<128 && count!=3)
		{
			j=0;
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j]=itrans[i];
				j++;
				i++;
			}	
			
			charr[j]='\0';
			cmp=mystrcmp(ca,charr);
			
			if(cmp ==0)
			{
				f=1;
			}
		}
		i++; 
	}
	
//	printf("i inc consonat is %d\n",i);	
	if(f==0)
	{
		f=isValidConsonantWithNuqta(ca,itrans,i);
	}

	return f;
}

int returnindex(wchar_t *itrans,int i)
{
	while(itrans[i]!=L';' && itrans[i]!=L'>')
	{
		i--;
	}
	
	return i+1;
}

int searchVowel(char *ca, wchar_t *itrans)
{
	int i=0;
	int j=0;
	int count=0;
	char charr[20];
	int f=0;
	int cmp=-1;
	while(count!=2 && f==0)
	{
		if(itrans[i]==L'<')
		{
			count++;
			while(itrans[i]!=L'>')
			{
				i++;
			}
			i++;
		}
		
		if(itrans[i]<128 && count !=2)
		{
			j=0;
			
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j]=itrans[i];
				j++;
				i++;
			}
			
			charr[j]='\0';
			cmp=strcmp(ca,charr);
			if(cmp == 0)
			{
				f=1;
				i--;
				cmp = returnindex(itrans,i);
			}
		} 
		i++;
	}


	return cmp;
}

int searchConsonantWithNuqta(char *ca,wchar_t *itrans)
{
	int i=0;
	int count=0;
	int f=0;
	int j=0;
	int cmp=-1;
	char charr[20];
	
	while(count!=3)
	{
		if(itrans[i]==L'>')
		{
			count++;
		}
		i++;
	}
	
	while(count!=4 && f==0)
	{
		if(itrans[i]==L'<')
		{
			count++;
		}

		if(itrans[i]<128 && count !=4)
		{
			j=0;
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j]=itrans[i];
				j++;
				i++;
			}	
			
			charr[j]='\0';
			cmp=strcmp(ca,charr);
			
			if(cmp == 0)
			{
				f=1;
				i--;
				cmp = returnindex(itrans,i);
			}

		}
		i++;
	}
	return cmp;

}

int searchIrregularConsonant(char *ca, wchar_t *itrans)	
{
	int i=0;
	int count=0;
	int f=0;
	int j=0;
	int cmp=-1;
	char charr[20];
	
 
	while(count!=4)
	{
		if(itrans[i]==L'>')
		{
			count++;
		}
		i++;
	}
	
	while(count!=5 && f==0)
	{
		if(itrans[i]==L'<')
		{
			count++;
		}

		if(itrans[i]<128 && count !=5)
		{
			j=0;
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j]=itrans[i];
				j++;
				i++;
			}	
			
			charr[j]='\0';
			cmp=strcmp(ca,charr);
			
			if(cmp == 0)
			{
				f=1;
				i--;
				cmp = returnindex(itrans,i);
			}

		}
		i++;
	}
	return cmp;
}

int searchConsonant(char *ca, wchar_t *itrans)
{
	int i=0;
	int j=0;
	int count=0;
	int f=0;
	int cmp=-1;
	char charr[20];
	
	while(count!=2)
	{	
		if(itrans[i]==L'<')
		{
			count++;
		}
		i++;
	}
	
	while(itrans[i]!=L'>')
	{
		i++;
	}
	
	i++;	
	
	while(count!=3 && f==0)
	{
		if(itrans[i]==L'<')
		{
			count++;
		}
		
		if(itrans[i]<128 && count!=3)
		{
			j=0;
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j]=itrans[i];
				j++;
				i++;
			}	
			
			charr[j]='\0';
			cmp=strcmp(ca,charr);
			
			if(cmp == 0)
			{
				f=1;
				i--;
				cmp = returnindex(itrans,i);
			}
		}
		i++; 
	}
	return cmp;
}

int searchAtVowel(char *ca, wchar_t *itrans)
{
	int bound = findbound(itrans);
	int i=bound+1;
	int j=0;
	int count=0;
	int f=0;
	int cmp=-1;
	char charr[20];
		
	while(count!=1 && f==0)
	{
	
		if(itrans[i]==L'<')
		{
			count++;
		}
		
		if(itrans[i]<128 && count!=1)
		{
			j=0;
			while(itrans[i]!=L'/' && itrans[i]!=L';')
			{
				charr[j]=itrans[i];
				j++;
				i++;
			}	
			charr[j]='\0';
			cmp=strcmp(ca,charr);
			
			if(cmp == 0)
			{
				f=1;
				i--;
				cmp = returnindex(itrans,i);
			}
		}
		i++; 
	}
	return cmp;
}

int addConsonant(char *maxdword,wchar_t *itrans,int d_index,wchar_t *dword,int last,int pch,int currc)
{
	int index;
	if(currc==2 && last==1)
	{
		if(pch!=0)
		{
		dword[d_index]=L'्';
		d_index++;
		}
	
		index = searchIrregularConsonant(maxdword,itrans);
		dword[d_index]=itrans[index];
		dword[d_index+1]=itrans[index+1];
		dword[d_index+2]=itrans[index+2];
		d_index=d_index+3;
	}


	if(currc==2 && last!=1 )
	{
		index = searchIrregularConsonant(maxdword,itrans);
		dword[d_index]=itrans[index];
		dword[d_index+1]=itrans[index+1];
		dword[d_index+2]=itrans[index+2];
		d_index=d_index+3;
		last=1;
	}

	if(currc==3 && last==1)
	{
		if(pch!=0)
		{
		dword[d_index]=L'्';
		d_index++;
		}
	
		index = searchConsonantWithNuqta(maxdword,itrans);
		dword[d_index]=itrans[index];
		dword[d_index+1]=itrans[index+1];
		d_index=d_index+2;
	}

	if(currc==3 && last!=1 )
	{
		index = searchConsonantWithNuqta(maxdword,itrans);
		dword[d_index]=itrans[index];
		dword[d_index+1]=itrans[index+1];
		d_index=d_index+2;
	}
	return d_index;	
}

void converter(char eword[],wchar_t dword[],wchar_t *itrans)
{
	int i=0;
	int j=0;
	char maxdword[20];
	int index;
	int d_index = 0;
	int flag=0;					//0 means half and 1 means full 
	int c=0;
	int v=0,pch=0;
	int last=0;					//consonant:1/vowel:2/@vowel:3
	int currv=-1,currc=-1;
	
	while(eword[i]!='\0')
	{
	
	j=0;	
	maxdword[j]=eword[i];
	i++;
	j++;
	maxdword[j]='\0';
	while(( ((v=isValidVowel(maxdword,itrans))) || (c = isValidConsonant(maxdword,itrans)))   && flag !=-1)  
	{
		if(eword[i]!=L'\0')
		{
		maxdword[j]=eword[i];
		i++;
		j++;
		maxdword[j]='\0';
		}
		
		if(eword[i] == L'\0')
		{
			flag= -1;
		}
		currv=v;
		currc=c;

	}
	
	if(eword[i]!='\0' || (c==0 & v==0))
	{

		j--;
		i--;
		maxdword[j]='\0';
	}
	j--;
	
	flag=0;	
	
	if(currc != 0 && maxdword[j] == 'a')
	{
		flag=1;
	}
	
	if(currc==1 && last==1)
	{
		if(pch!=0)
		{
		dword[d_index]=L'्';
		d_index++;
		}
	
		index = searchConsonant(maxdword,itrans);
		dword[d_index]=itrans[index];
		d_index++;
		pch=0;
		last=1;
	}


	if(currc==1 && last!=1 )
	{
		index = searchConsonant(maxdword,itrans);
		dword[d_index]=itrans[index];
		d_index++;
		last=1;
	}
	
	if(currc==2 || currc==3)
	{
		d_index= addConsonant(maxdword,itrans,d_index,dword,last,pch,currc);
		last=1;
	}

	if(currv==1)
	{
		if(last!=1)
		{
			index=searchVowel(maxdword,itrans);
			dword[d_index]=itrans[index];
			d_index++;
			last=2;
		}

		if(last==1)
		{
			index=searchAtVowel(maxdword,itrans);
			dword[d_index]=itrans[index];
			d_index++;
			last=3;
		}	
	}
	
	if(currv==2)
	{
			index=searchAtVowel(maxdword,itrans);
			dword[d_index]=itrans[index];
			d_index++;
			last=3;
	}

	if(flag==1 && (eword[i] == 'a' || eword[i] == 'u' || eword[i] == 'i'))
	{
		j=0;
		
		maxdword[j]='a';
		maxdword[j+1]=eword[i];
		maxdword[j+2]='\0';
		index = searchAtVowel(maxdword,itrans);
		dword[d_index]=itrans[index];
		d_index++;
		i++;
		last=3;
	}
	
	pch=0;
	if(flag!=1 && last==1)
	{
		pch=1;
	}
	}
	dword[d_index]=L' ';
	d_index++;
	dword[d_index]=L'\0';
}

int findbound(wchar_t *ch)
{
	int i=0;
	
	while(ch[i]!='@')
	{
		i++;
	}

	while(ch[i]!='>')
	{
		i++;
	}

	return i;
}

