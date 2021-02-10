#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

//Struktura opisujaca gracza
//name - nazwa gracza
//points - liczba uzyskanych punktow
struct player
{
	char name[100];
	int points;
}; 

//struktura opisujaca haslo
struct word
{
	char kategoria[100];
	char haslo[100];
};

struct player** players(struct player**,int*);
struct word** wczytaj(struct word**,int*);
void play(int,int,struct player**,struct word**);
int zakrecKolem();
int losuj(int);
int sprawdz(char, char[],char**,int*);
int nextPlayer(int, int);
void runda(struct word**, struct player**, int, int, int*,int);
struct player** freePlayers(struct player**,int);
struct word** freeWords(struct word**,int);
void saveScore(struct player**,int);

int main()
{
	int l_graczy=0,l_hasel=0;
	char znak='0';
	struct word **tablica_hasel=NULL;
	struct player **tablica_graczy = NULL;
	printf("------KOLO FORTUNY------\n");
	printf("1.Wczytaj liste hasel z pliku\n2.Wybierz liczbe graczy.\n3.Rozpocznij gre!\nx-  jesli chcesz zakonczyc\n");
	while (znak!='x')
	{
		scanf(" %c", &znak);
		switch (znak)
		{
		case '1':
			tablica_hasel = wczytaj(tablica_hasel,&l_hasel);
			break;
		case '2':
			tablica_graczy = players(tablica_graczy, &l_graczy);
			break;
		case '3':
			if (tablica_hasel == NULL || l_hasel < 3)
			{
				printf("Najpierw wczytaj wystarczajaca ilosc hasel! (min 3)");
				break;
			}
			else if (tablica_graczy == NULL)
			{
				printf("Wybierz najpierw liczbe graczy!");
				break;
			}
			play(l_graczy, l_hasel, tablica_graczy, tablica_hasel);
			printf("------KOLO FORTUNY------\n");
			printf("1.Wczytaj nowa liste hasel z pliku\n2.Wybierz nowa liczbe graczy.\n3.Rozpocznij nowa gre!\nx-  jesli chcesz zakonczyc\n");
			break;  
		case 'x':
			tablica_graczy = freePlayers(tablica_graczy, l_graczy);
			tablica_hasel = freeWords(tablica_hasel, l_hasel);
			break;
		default: printf("Wpisz cos innego\n");
		}
	}
	return 0;
}
//Funkcja pobierajaca od uzytkownika ilosc oraz nazwy graczy.
//Pobiera: **tablica_graczy - tablice struktur graczy oraz wskaznik do liczby graczy.
//Zwraca tablice struktur graczy.
struct player** players(struct player **tablica_graczy,int *liczba)
{
	int ilosc=0,i;
	while (ilosc != 1 && ilosc != 2 && ilosc != 3 && ilosc != 4)
	{
		printf("Wybierz liczbe graczy: 1,2,3 lub 4:\n");
		scanf(" %d", &ilosc);
	}
	tablica_graczy = (struct player**)malloc(ilosc*sizeof(int));
	if (tablica_graczy == NULL)
	{
		printf("Blad podczas alokacji pamieci");
		return NULL;
	}
	for (i = 0; i < ilosc; i++)
	{
		tablica_graczy[i] = (struct player*)malloc(sizeof(struct player));
		if (tablica_graczy[i] == NULL)
		{
			printf("Blad podczas alokacji pamieci");
			return NULL;
		}
		printf("Nazwa gracza nr %d:", i+1);
		scanf("%s", &tablica_graczy[i]->name);
		tablica_graczy[i]->points = 0;
	}
	for (i = 0;i<ilosc; ++i) printf("Gracz %d %s\n", i, tablica_graczy[i]->name);
	*liczba = ilosc;
	return tablica_graczy;
}
//Funkcja wczytuje liste hasel oraz kategori z pliku.
//Pobiera: **tablica_hasel -tablice struktur hasel oraz *liczba - wskaznik do ilosci wczytanych hasel.
//Zwraca: tablice strkutur hasel.
struct word** wczytaj(struct word **tablica_hasel,int *liczba)
{
	FILE *f;
	int i = 0,j;
	char nazwa[100], x[100], y[100];
	struct word **tabhasel=NULL; //tablica pomocnicza do realokacji pamieci
	printf("Podaj nazwe pliku\n");
	scanf("%s", nazwa);
	f = fopen(nazwa, "rt");
	if (f == NULL)
	{
		printf("Nie udalo sie odczytac takiego pliku.");
		return 0;
	}
	while (fscanf(f, "%s %s", &x,&y ) == 2)
	{
		tabhasel = (struct word**)realloc(tabhasel, (i+1)*sizeof(int));
		if (tabhasel == NULL)
		{
			printf("BLAD");
			return 0;
		}
		else
		{
			tablica_hasel = tabhasel;
			tablica_hasel[i]= (struct word*)malloc(sizeof(struct word));
			if (tablica_hasel[i] == NULL)
			{
				printf("Blad podczas alokacji pamieci");
				return NULL;
			}
		}
		strcpy(tablica_hasel[i]->kategoria, x);
		strcpy(tablica_hasel[i]->haslo, y);
		i++;
	}
	fclose(f);
	for (j = 0; j < i; j++)
	{
		printf("%s;%s\n", tablica_hasel[j]->kategoria, tablica_hasel[j]->haslo);
	}
	*liczba = i;
	return tablica_hasel;
}
//Funkcja rozpoczynajaca gre oraz podajaca wyniki.
//Pobiera: l_graczy - liczbe graczy, l_hasel- liczbe hasel, **tab_graczy - tablice struktur graczy, **tab_hasel - tablice struktur hasel.
void play(int l_graczy,int l_hasel, struct player **tab_graczy, struct word **tab_hasel)
{
	int i, gracz,h[3],pts=0;
	char znak;
	h[0] = losuj(l_hasel);
	do
	{
		h[1] = losuj(l_hasel);
		h[2] = losuj(l_hasel);
	} while (h[0] == h[1] || h[1]==h[2]);
	gracz = losuj(l_graczy);
	for (i = 0; i < 3; i++)
	{
		runda(tab_hasel, tab_graczy, i, h[i], &gracz,l_graczy);
	}
	printf("\nKONIEC\nWyniki:\n");
	for (i = 0; i < l_graczy; i++) printf("%s %dpkt\n",tab_graczy[i]->name,tab_graczy[i]->points);
	printf("Czy chcesz zapisaæ wyniki do pliku?(t/n)\n");
	scanf(" %c", &znak);
	if (znak == 't') saveScore(tab_graczy,l_graczy);
}
//Funkcja losujaca liczbe z zakresu 50-500 oraz informujaca o wylosowanej liczbie.
//Zwraca wylosowana liczbe.
int zakrecKolem()
{
	int points;
	srand(time(NULL));
	points = 50 + rand() % 501;
	printf("Kolo wylosowalo %d pkt.\n", points);
	return points;
}
//Funkcja losujaca liczbe z zakresu 0-max.
//Pobiera: max -wartosc maksymalna.
//Zwraca: wylosowana liczbe.
int losuj(int max)
{
	int liczba;
	srand(time(NULL));
	liczba = rand()% (max);
	return liczba;
}
//Funkcja sprawdza czy podana litera jest w hasle.
//Pobiera znak - sprawdzana litere, haslo, **tmp - wskaznik do tablicy z zapisanymi odgadnietymi juz literami, *points- wskaznik do ilosci punktow.
//Zwraca: 0 gdy nie udalo sie odgadnac litery, -1 gdy zostalo odgadniete cale haslo, 1 gdy podana litera jest w hasle.
int sprawdz(char znak,char haslo[100],char **tmp,int *points)
{
	int i = 0,licznik=0,j,mnoznik=0; 
	int dlugosc = strlen(haslo);
	if (*tmp == NULL)
	{
		*tmp = malloc(dlugosc*sizeof(char));
		if (tmp == NULL)
		{
			printf("Blad podczas alokacji pamieci");
			return 0;
		}
		for (j = 0; j < dlugosc; ++j)
		{
			(*tmp)[j] = '_';
		}
	}
	while (haslo[i] != '\0')
	{

		if (haslo[i] == ' ')
		{
			printf(" ");
			(*tmp)[i] = ' ';
		}
		else if (haslo[i] == znak) 
		{
			(*tmp)[i] = znak;
			printf("%c ", (*tmp)[i]);
			mnoznik++;
		}
		else
		{
			printf("%c ", (*tmp)[i]);
			if ((*tmp)[i] == '_') licznik++;
		}
		++i;
	}
	*points = (*points)*mnoznik;
	printf("\n");
	if (licznik == 0)
	{	
		return -1;
	}
	if (mnoznik == 0)
	{
		return 0;
	}
	else return 1;
}
//Funkcja okreslajaca numer nastepnego gracza.
//Pobiera: l_graczy- liczbe graczy, gracz- numer aktualnego gracza.
//Zwraca numer kolejnego gracza.
int nextPlayer(int l_graczy,int gracz)
{
	if (gracz + 1 >= l_graczy) return 0;
	else return gracz + 1;

}
//Funkcja- przebieg jednej rundy.
//Pobiera l_graczy - liczbe graczy, **tab_graczy - tablice struktur graczy, **tab_hasel - tablice struktur hasel, *nr_gracza - wskaznik do aktualnego numeru gracza, nr_hasla -numer hasla do odgadniecia, nr_rundy- numer rundy.
void runda(struct word** tab_hasel,struct player** tab_graczy,int nr_rundy,int nr_hasla,int* nr_gracza,int l_graczy)
{
	int k = 0,parametr=1,*tmp_pts,points,i; //tmp_pts - tablica przechowujaca punkty zdobyte przez graczy w jednej rundzie(na koniec tylko zwycieski gracz otrzymuje pkt na konto)
	char *tmp = NULL,znak; //tmp- tablica przechowuj¹ca odgadniête litery has³a
	tmp_pts = malloc(l_graczy*sizeof(int));
	if (tmp_pts == NULL)
	{
		printf("Blad podczas alokacji pamieci");
		return;
	}
	for (i = 0; i < l_graczy; ++i) tmp_pts[i] = 0;
	printf("\nRUNDA %d\n", nr_rundy + 1);
	printf("Kategoria: %s\n", tab_hasel[nr_hasla]->kategoria);
	while ((tab_hasel[nr_hasla]->haslo[k]) != '\0')
	{
		if (tab_hasel[nr_hasla]->haslo[k] == '    ') printf(" ");
		else printf("_");
		printf(" ");
		k++;
	}
	printf("\nZaczyna %s.\n", tab_graczy[*nr_gracza]->name);
	printf("%s, wpisuj litery. \nJesli sie pomylisz nastepny gracz otrzymuje szanse zdobycia puli punktow.\n", tab_graczy[*nr_gracza]->name);
	parametr = 1;
	while (parametr != -1)
	{
		points = zakrecKolem();
		scanf(" %c", &znak);
		parametr = sprawdz(znak, tab_hasel[nr_hasla]->haslo, &tmp, &points);
		if (parametr == 0 && tmp == NULL)
		{
			free(tmp_pts);
			return;
		}
		else if (parametr ==0)
		{
			*nr_gracza = nextPlayer(l_graczy, *nr_gracza);
			printf("W hasle nie ma takiej literki, szanse otrzymuje teraz %s:\n", tab_graczy[*nr_gracza]->name);
		}
		tmp_pts[*nr_gracza] += points;
	}
	if (parametr == -1)
	{
		printf("%s odgaduje haslo!. Jego konto zasila %d pkt.\n", tab_graczy[*nr_gracza]->name, tmp_pts[*nr_gracza]);
		tab_graczy[*nr_gracza]->points += tmp_pts[*nr_gracza];
	}
	free(tmp_pts);
	free(tmp);
}
//Funkcja zwalniajaca pamiec ze tablicy struktur graczy.
//Pobiera tablice struktur- tab_graczy oraz ilosc zapisanych w niej graczy- l_graczy.
//Zwraca pusta tablice.
struct player** freePlayers(struct player** tab_graczy,int l_graczy)
{
	int i;
	for (i = 0; i < l_graczy; i++) free(tab_graczy[i]);
	free(tab_graczy);
	return tab_graczy;
}
//Funkcja zwalniajaca pamiec ze tablicy struktur hasel.
//Pobiera tablice struktur- tab_hasel oraz ilosc zapisanych w niej hasel- l_hasel.
//Zwraca pusta tablice.
struct word** freeWords(struct word** tab_hasel, int l_hasel)
{
	int i;
	for (i = 0; i < l_hasel; i++) free(tab_hasel[i]);
	free(tab_hasel);
	return tab_hasel;
}
//Funkcja zapisujaca wyniki gry do pliku.
//Pobiera tablice struktur graczy - tab_graczy oraz ich liczbe - l_graczy.
void saveScore(struct player ** tab_graczy,int l_graczy)
{
	int i;
	FILE *f;
	char nazwa[50];
	printf("Podaj nazwe pliku:\n");
	scanf(" %s", &nazwa);
	f = fopen(nazwa, "wt");
	if (f == NULL)
	{
		printf("B³¹d odczytu pliku");
		return;
	}
	else
	{
		for (i = 0; i < l_graczy; i++)
		{
			fprintf(f, "%d. %s : %d pkt.\n", i+1, tab_graczy[i]->name, tab_graczy[i]->points);
		}
		printf("Wyniki zostaly zapisane\n");
		fclose(f);
	}
}