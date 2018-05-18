	
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

/* Autor řešení: Jan Kubica (xkubic39) */
/* Vypracováno dne 29. 10. 2016 */

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L -> Act = NULL;
	L -> First = NULL;
	L -> Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	tDLElemPtr hPtr; // pomocna promenna pro ulozeni pointeru
	while (L -> First != NULL) { // az po posledni prvek
		hPtr = L -> First;
		L -> First = L -> First -> rptr; // nastaveni pointeru na dalsi prvek
		free(hPtr); // uvolneni presleho prvku
	}
	L -> Act = NULL;
	L -> Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr nPtr = malloc(sizeof(struct tDLElem));
	if (nPtr == NULL) {
		DLError();
	}
	else {		
		nPtr -> data = val; // ulozeni hodnoty do noveho prvku
		nPtr -> lptr = NULL; // levy pointer noveho prvku na NULL
		nPtr -> rptr = L -> First; // pravy pointer noveho prvku na prvni prvek
		if (L -> First != NULL) { // jestli existuje prvni prvek
			L -> First -> lptr = nPtr; // levy pointer prvniho prvku na novy prvek
		}
		else {
			L -> Last = nPtr; // jinak nastav posledni prvek jako novy prvek
		}
		L -> First = nPtr; // nastaveni noveho prvku jako prvni prvek
	}
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr nPtr = malloc(sizeof(struct tDLElem));
	if (nPtr == NULL) {
		DLError();
	}
	else {		
		nPtr -> data = val; // ulozeni hodnoty do noveho prvku
		nPtr -> rptr = NULL; // pravy pointer noveho prvku na NULL
		nPtr -> lptr = L -> Last; // levy pointer noveho prvku na posledni prvek
		if (L -> Last != NULL) { // jestli existuje posledni prvek
			L -> Last -> rptr = nPtr; // pravy poiter posledniho prvku na novy prvek
		}
		else {
			L -> First = nPtr; // jinak nastav prvni prvek jako novy prvek
		}
		L -> Last = nPtr; // nastaveni noveho prvku jako prvni prvek
	}
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L -> Act = L -> First; // aktivni je prvni prvek
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
		L -> Act = L -> Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L -> First == NULL) {
		DLError();
	}
	else {
		*val = L -> First -> data;
	}
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L -> Last == NULL) {
		DLError();
	}
	else {
		*val = L -> Last -> data;
	}
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if (L -> First != NULL) {
		tDLElemPtr hPtr = L -> First;
		if (L -> Act == L -> First) {
			L -> Act = NULL;
		}
		if (L -> First == L -> Last) {
			L -> First = NULL;
			L -> Last = NULL;
		}
		else {
			L -> First = L -> First -> rptr;
			L -> First -> lptr = NULL;
		}
		free(hPtr);
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	if (L -> Last != NULL) {
		tDLElemPtr hPtr = L -> Last;
		if (L -> Act == L -> Last) {
			L -> Act = NULL;
		}
		if (L -> Last == L -> First) {
			L -> First = NULL;
			L -> Last = NULL;
		}
		else {
			L -> Last = L -> Last -> lptr;
			L -> Last -> rptr = NULL;
		}
		free(hPtr);
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	if (L -> Act != NULL && L -> Act -> rptr != NULL) {
		tDLElemPtr hPtr = L -> Act -> rptr;
		L -> Act -> rptr = hPtr -> rptr;
		if (hPtr == L -> Last) {
			L -> Last = L -> Act;
		}
		else {
			hPtr -> rptr -> lptr = L -> Act;
		}
		free(hPtr);
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if (L -> Act != NULL && L -> Act -> lptr != NULL) {
		tDLElemPtr hPtr = L -> Act -> lptr;
		L -> Act -> lptr = hPtr -> lptr;
		if (hPtr == L -> First) {
			L -> First = L -> Act;
		}
		else {
			hPtr -> lptr -> rptr = L -> Act;
		}
		free(hPtr);
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L -> Act != NULL) {
		tDLElemPtr hPtr = malloc(sizeof(struct tDLElem));
		if (hPtr == NULL) // pokud není dostatek pameti
			DLError();
		else {
			hPtr -> data = val;	// ulozeni hodnoty val
			hPtr -> rptr = L -> Act -> rptr; // navazani na prvek za vlozenym prvkem
			hPtr -> lptr = L -> Act; // zpetne napojeni noveho prvku na aktivni prvek
			L -> Act -> rptr = hPtr; // prime napojeni noveho prvku na aktivni prvek
			if (L -> Act == L -> Last) { // pokud je pridavany prvek posledni
				L -> Last = hPtr; // nastav jako posledni
			}
			else {
				hPtr -> rptr -> lptr = hPtr; // zpetne navaz prvek za vkladanym prvkem na vkladany prvek
			}
		}
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L -> Act != NULL) {
		tDLElemPtr hPtr = malloc(sizeof(struct tDLElem));
		if (hPtr == NULL) // pokud není dostatek pameti
			DLError();
		else {
			hPtr -> data = val;	// ulozeni hodnoty val
			hPtr -> rptr = L -> Act; // napojeni noveho prvku na aktivni prvek
			hPtr -> lptr = L -> Act -> lptr; // zpetne navazani na prvek pred vlozenym prvkem
			L -> Act -> lptr = hPtr; // prime napojeni noveho prvku na aktivni prvek
			if (L -> Act == L -> First) { // pokud je pridavany prvek posledni
				L -> First = hPtr; // nastav jako posledni
			}
			else {
				hPtr -> lptr -> rptr = hPtr; // zpetne navaz prvek pred vkladanym prvkem na vkladany prvek
			}
		}
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if (L -> Act == NULL) {
		DLError();
	}
	else {
		*val = L -> Act -> data;
	}
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if (L -> Act != NULL) {
		L -> Act -> data = val;
	}
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if (L -> Act != NULL) { // pokud je seznam aktivni
		L -> Act = L -> Act -> rptr; // nasledujici prvek prirad aktivnimu
	}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if (L -> Act != NULL) { // pokud je seznam aktivni
		L -> Act = L -> Act -> lptr; // nasledujici prvek prirad aktivnimu
	}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L -> Act != NULL) ? 1 : 0;
}

/* Autor řešení: Jan Kubica (xkubic39) */
/* Konec c206.c*/
