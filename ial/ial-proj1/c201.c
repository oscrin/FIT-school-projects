
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
** 
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
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

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L -> Act = NULL;
	L -> First = NULL;
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
	tElemPtr hPtr; // pomocna promenna pro ulozeni pointeru
	while (L -> First != NULL) { // az po posledni prvek
		hPtr = L -> First;
		L -> First = L -> First -> ptr; // nastaveni pointeru na dalsi prvek
		free(hPtr); // uvolneni presleho prvku
	}
	L -> Act = NULL;
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
    tElemPtr nPtr; // pomocna promenna pro vytvoreni prvku
    if ((nPtr = malloc(sizeof(struct tElem))) == NULL) {
    	Error(); // error pri neuspechu alokace pameti pro prvek
    }
    else {
    	nPtr -> data = val; // ulozeni hodnoty do prvku
    	nPtr -> ptr = L -> First; // ulozeni aktualne prvniho prvku seznamu na ukazatel vkladaneho prvku
    	L -> First = nPtr; // nastaveni vkladaneho prvku jako prvni prvek
    }
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
	L -> Act = L -> First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	if (L -> First == NULL) { // pri prazdnem seznamu nastava error
		Error();
	}
	else {
		*val = L -> First -> data; // hodnota prvniho prvku seznamu
	}
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
	if (L -> Act == L -> First) { // pokud je aktivni prvek prvni, je nastaven na NULL
		L -> Act = NULL;
	}
	if (L -> First != NULL) { // pokud seznam neni prazdny
		tElemPtr hPtr = L -> First; // pomocna promenna ukazujici na aktualne prvni prvek
		L -> First = L -> First -> ptr; // nastaveni druheho prvku seznamu jako prvni prvek
		free(hPtr); // uvolneni puvodne prvniho prvku
	}
}	

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
	tElemPtr hPtr; // pomocna promenna ukazujici na nasledujici prvek
	if (L -> Act != NULL && (hPtr = L -> Act -> ptr) != NULL) { // pokud je seznam aktivní a pokud existuje nasledujici prvek
		L -> Act -> ptr = hPtr -> ptr; // vypusteni prvku k odstraneni a svazani aktivniho s dalsim nasledujicim
		free(hPtr); // odstraneni vypusteneho prvku
	}
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
	if (L -> Act != NULL) { // pokud je list aktivni
		tElemPtr nPtr;
		if ((nPtr = malloc(sizeof(struct tElem))) == NULL) { // alokace noveho prvku
			Error();
		}
		else {
			nPtr -> data = val; // ulozeni dat do noveho prvku
			nPtr -> ptr = L -> Act -> ptr; // navazani post-aktivniho prvku na novy prvek
			L -> Act -> ptr = nPtr; // nastaveni post-aktivniho prvku na novy prvek
		}
	}
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
	if (L -> Act == NULL) { // pokud neni seznam aktivni
		Error();
	}
	else {
		*val = L -> Act -> data; // nakopirovani dat do promenne val
	}
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
	if (L -> Act != NULL) { // pokud je seznam aktivni
		L -> Act -> data = val; // prepis dat promennou val
	}
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
	if (L -> Act != NULL) { // pokud je seznam aktivni
		L -> Act = L -> Act -> ptr; // nasledujici prvek prirad aktivnimu
	}

}

int Active (tList *L) {		
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
	return (L -> Act != NULL) ? 1 : 0;
}

/* Autor řešení: Jan Kubica (xkubic39) */
/* Konec c201.c */
