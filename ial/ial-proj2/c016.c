
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

	if ((*ptrht) != NULL){ // pokud existuje tabulka
		for(int i = 0; i < HTSIZE; i++)
			(*ptrht)[i] = NULL;	// nastav vsechny polozky na NULL
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	int hCode;
	tHTItem* hItem;

	hCode = hashCode(key);

	// pokud tabulka neni inicializovana nebo pokud tabulka neobsahuje dany prvek
	if ((*ptrht) == NULL || (*ptrht)[hCode] == NULL) { 
		return NULL;
	}
	else if ((*ptrht) != NULL && (*ptrht)[hCode] != NULL) { // tabulka je inicializovana a existuje prvek
		hItem = (*ptrht)[hCode];

		if (strcmp(hItem -> key, key) == 0) { // pokud prvni prvek v seznamu je danym prvkem
			return hItem;
		}
		while (hItem -> ptrnext != NULL) { // hledej v seznamu, dokud nedojdes nakonec
			hItem = hItem -> ptrnext;
			if (strcmp(hItem -> key, key) == 0) // pokud jsi nasel prvek v seznamu
				return hItem; // vrat prvek
		}
	}
	return NULL;
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

	tHTItem* hItem; // help Item
	tHTItem* nItem; // new Item
	int hCode; // hash code

	if ((*ptrht) == NULL) { // pokud neni tabulka inicializovana, vrat se
		return;
	} 
	else {
		hItem = htSearch(ptrht, key); // najdi prvek
		if (hItem != NULL) { // pokud jsi nasel prvek
			hItem -> data = data; // prepis jeho data
		}
		else {
			hCode = hashCode(key); // zhasuj kod
			nItem = malloc(sizeof(struct tHTItem)); // vytvor novy prvek
			if (nItem == NULL) {
				return;
			}

			nItem -> key = malloc(strlen(key) + 1); // vytvor misto pro klic
			if(nItem -> key == NULL) {
				free(nItem);
				return;
			}

			strcpy(nItem -> key, key);	// zkopiruj klic do prvku
			nItem -> data = data;
			nItem -> ptrnext = *ptrht[hCode]; // nastav ukazatel na dalsi prvek na prvni prvek seznamu
			(*ptrht)[hCode] = nItem; // nastav jako prvni prvek seznamu
		}
	}

}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	tHTItem* hItem; // help Item

	hItem = htSearch(ptrht, key); // najdi prvek
	if (hItem == NULL) // pokud neexistuje
		return NULL;
	else
		return (&(hItem -> data)); // jinak vrat jeho data
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

	int hCode = hashCode(key); // zhasuj klic
	tHTItem* hItem = (*ptrht)[hCode]; // najdi misto v tabulce
	tHTItem* fItem;

	if (hItem == NULL) // pokud neexistuje prvek, tak se vrat
		return;

	if ((*ptrht) != NULL) {
		if ((strcmp(hItem -> key, key)) == 0) { // pokud se rovnaji klice, smaz prvni prvek
			(*ptrht)[hCode] = hItem -> ptrnext; // nastav nasledujici prvek jako prvni
			free(hItem -> key);
			free(hItem);
		}

		while (hItem -> ptrnext != NULL) { // prohledej seznam
			fItem = hItem; // uzel otec
			hItem = hItem -> ptrnext; // syn je nasledujici

			if(strcmp(hItem -> key, key) == 0) { // pokud syn je hledanym prvkem
				fItem -> ptrnext = hItem -> ptrnext; // propoj otce s nasledujicim
				free(hItem -> key); // uvolni klic
				free (hItem); // uvolni prvek
			}
		}
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	tHTItem* hItem; // help Item
	tHTItem* dItem; // deleted Item

	if((*ptrht) == NULL) // pokud neni inicializovana tabulka
		return;
	else {
		for (int i = 0; i < HTSIZE; i++) { // cykluj od nulteho radku do konce tabulky
			hItem = (*ptrht)[i];
			if (hItem != NULL) { // pokud radek obsahuje prvek nebo seznam
				while (hItem -> ptrnext != NULL) { // cykluj, dokud existuje alespon jeden prvek
					dItem = hItem; // pomocny prvek nastav jako prvek ke smazani
					hItem = hItem -> ptrnext; // pomocny prvek posun na nasledujici
					free(dItem -> key); // smaze klic prvku ke smazani
					free(dItem); // smaze prvek
				}
				if (hItem -> ptrnext == NULL) { // pokud je prvek poslednim prvkem seznamu
					free(hItem -> key); // uvolni klic prvku
					free(hItem); // uvolni prvek
					(*ptrht)[i] = NULL; // nastav radek na neobsazeny
				}
			}
		}
	}
}
