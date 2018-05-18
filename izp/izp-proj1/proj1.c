/******************************************************************/
/*                          IZP Project 1                         */
/*                                                                */
/*                         proj1.c  (v1.0)                        */
/*                                                                */
/*                 Created by Jan Kubica (xkubic39)               */
/*                            Oct 2015                            */
/******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<limits.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 101
#define STRING_DAY_LENGTH 4


typedef enum ReadType { 
	Ndate,
	Nnumber,
	Nprime,
	Nword,
	Npalindrom
} rType; // vycet moznosti vystupu, ktere budou pouzity ve funkci printOutput

int checkPrime(unsigned int number)
{
	unsigned int divider = 0;
	
	if (number > INT_MAX) // kontrola prvocisla jen po hodnotu INT_MAX
		return FALSE;
	
	if (number == 0 || number == 1) // pocatecni podminka
		return FALSE;

	unsigned int k = 1;

	for (; k <= number/2; k++) // cyklus se provadi jen do poloviny cisla, moznost urychleni vypoctu
		if (number % k == 0) // modulo, ktere dava 0 celociselny zbytek
			divider++;
			
	if (divider > 1) // cislo deitelne vice nez 1 neni prvocislo (zohlednujeme kontrolu jen do poloviny cisla)
		return FALSE;
	return TRUE;
}


unsigned int stringLength(char *buffer) // zjisteni delky retezce
{
	unsigned int num = 0;

	for (; buffer[num] != '\0'; ++num); // cykleni dokud se znak nerovna '\0', do te doby se zvysuje promenna num

	return num;
}

int checkPalindrom(char *buffer)
{
	unsigned int i = 0, j = stringLength(buffer) - 1; // inicializace promennych pro prochazeni polem (prvni a posledni prvek)
	
	while (i < j)
	{
		if (buffer[i] != buffer[j]) 
		{ 
			return FALSE;
		}

		i++; j--; // priblizovani indexu ke stredu retezce
	}
	return TRUE;
}

int checkDate(char *b, char *daybuf)
{
	if (stringLength(b) != 10) return FALSE; // test ochrany preteceni (retezec ma treba 3 znaky)
	if (isdigit(b[0]) && isdigit(b[1]) && isdigit(b[2]) && isdigit(b[3]) && b[4] == '-' && isdigit(b[5]) && isdigit(b[6]) && b[7] == '-' && isdigit(b[8]) && isdigit(b[9]))
	{ // testovani formatu YYYY-MM-DD

		int year = (b[0] - '0') * 1000 + (b[1] - '0') * 100 + (b[2] - '0') * 10 + (b[3] - '0');
		int month = (b[5] - '0') * 10 + (b[6] - '0');
		int day = (b[8] - '0') * 10 + (b[9] - '0');
		
		if (year < 1900) // podminky pro mktime, aby cas odpovidal realnemu casu (2015-99-99)
			return FALSE;
		if (month > 12)
			return FALSE;
		if (day > 31)
			return FALSE;		
			
		if (((month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day == 31)) // podminka pro mesice s 30 dny
			return FALSE;
		if (((year % 4) == 0) && (month == 2) && (day > 29)) 
			return FALSE; // kdyz je prestupny rok, den v unoru nabyva max 29
		if (((year % 4) != 0) && (month == 2) && (day > 28)) 
			return FALSE; // kdyz neni prestupny rok a je unor, nemuzou nastat dny 29, 30, 31
			
		struct tm date; // struktura pro vypocet dne
		date.tm_year = year - 1900; // -1900 viz definice struktury
		date.tm_mon = month - 1; // -1 viz definice struktury
		date.tm_mday = day;
		date.tm_hour = 0;
		date.tm_min = 0;
		date.tm_sec = 0;
		date.tm_isdst = -1;
			
		if (mktime(&date) == -1) // pokud nelze dohledat den, i kdyz by se jednalo o radne datum (napr 1950-09-23), omezeni od pocatku epoch time
			return FALSE;
		else {
			(void) strftime(daybuf, sizeof(daybuf), "%a", &date);
			return TRUE;
		}
	}
	return FALSE;
}

rType checkInput(char *buffer, char *day)
{
	// test, zda-li je na vstupu cislo
	char *endptr; // koncový zbytek
	int ntmp;
	ntmp = strtol(buffer, &endptr, 10); // prevedeni cisla v desitkove soustave
	if (ntmp >= 0 && *endptr == '\0') // cislo ma byt kladne a otestuje se endptr - (pr. 123a x 123)
	{
		if (checkPrime(ntmp) == TRUE)
			return Nprime;
		else
			return Nnumber;
	}

	// test, jestli se jedna o datum
	if (checkDate(buffer, day) == TRUE)
	{
		return Ndate;
	}
	
	// test, jestli se jedna o palindrom
	if (checkPalindrom(buffer) == TRUE)
	{
		return Npalindrom;
	}
	else return Nword;

}

int readInput(char *buffer) // pomocna fce pro sken do bufferu, vracici hodnotu int pro kontrolu s EOF
{
	return scanf("%100s", buffer);
}

void printOutput(char *buffer, rType type, char *day)
{
	switch (type) // vypis podle typu vystupu za pomoci rType
	{
	case Nnumber:
		printf("number: %s\n", buffer);
		break;
	case Nprime:
		printf("number: %s (prime)\n", buffer);
		break;
	case Ndate:
		printf("date: %s %s\n", day, buffer);
		break;
	case Nword:
		printf("word: %s\n", buffer);
		break;
	case Npalindrom:
		printf("word: %s (palindrome)\n", buffer);
		break;
	}
}

int main(int argc, char *argv [])
{
	if (argc > 1 && *argv) // pri spusteni s libovolnymi argumenty program vypise svuj popis a uspesne skonci
	{
		printf("IZP Project 1 (v1.0)\n");
		printf("This is a program for simple text processing. \nIt recognizes words, palindromes, numbers, prime numbers and dates.\n");
		printf("Faculty of Information Technology, Brno University of Technology\n");
		printf("Created by Jan Kubica (xkubic39), Oct 2015\n");
		return 0;
	}

	int code;
	char buffer[BUFFER_SIZE];
	rType type;
	char day[STRING_DAY_LENGTH]; // retezec v pripade data, na vypsani dne

	while((code = readInput(buffer)) != EOF) // cykleni az do nactení posledniho slova
	{
		if (code == 0) // podminka pro neocekavana vstupni data
		{
			printf("INPUT ERROR\n"); 
			return 1; // navratova hodnota pro neocekavany vstup
		}
		type = checkInput(buffer, day); // kontrola vstupu
		printOutput(buffer, type, day); // vypis hodnoty za pomoci rType
	}
}
