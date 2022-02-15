#include <iostream>
#include <random>
#include <fstream> 
#include <string>
#include <time.h>

#define WIELKOSC_TABLICY 7
#define MAX_LITER 2
#define WIELKOSC_BLOKU_DYSKOWEGO 140
#define WSPOLCZYNNIK_BLOKOWANIA 20

using namespace std;

int operacje = 0;
int fazy = 0;
int liczbaSerii = 1;

struct bufor {
	int numerStrony = 0;
	int wielkoscBufora = WIELKOSC_BLOKU_DYSKOWEGO;
	char zawartosc[WIELKOSC_BLOKU_DYSKOWEGO];
	int maxStron = (WIELKOSC_BLOKU_DYSKOWEGO / WSPOLCZYNNIK_BLOKOWANIA);
	int liczbaBajtow = 0;
};

bufor Bufor;

struct tasma {
	char** zawartosc;
	int ileRekordow;

	tasma(int ileRekordow)
	{
		this->ileRekordow = ileRekordow;
		zarezerwujTablice();
	};


	void zarezerwujTablice() {
		this->zawartosc = new char*[this->ileRekordow];
		for (int liczbaTablic = 0; liczbaTablic < this->ileRekordow; liczbaTablic++) {
			zawartosc[liczbaTablic] = new char[WIELKOSC_TABLICY];
			zawartosc[liczbaTablic][0] = 0;

		}
	}
};

bool wczytajDoBufora(tasma, int);
tasma losujTablice(tasma);
tasma generujTabliceLosowo(int);
void zapiszDoPliku(tasma, string);
long liczbaBajtow(string);
tasma wczytajZPlikuDat(string);
tasma wczytajZPliku(string);
tasma przygotujTablice();
void wygenerujLosowo();
void wpiszZKlawiatury();
bool posortujDwa(char*, char*);
void wyswietlTasme(tasma);
void rozdziel();
tasma wrzucDoT3(tasma, int);
bool sklej();
void wczytajTestowy();
void menu();
void posortuj();
int main() {

	menu();
	posortuj();
	cout << "Liczba Serii: " << liczbaSerii << endl;
	cout << "Liczba Faz: " << fazy << endl;
	cout << "Liczba Operacji: " << operacje << endl;
	system("pause");
	return 0;

}
bool wczytajDoBufora(tasma T, int indeksT) {
	operacje++;
	if (Bufor.liczbaBajtow % (WSPOLCZYNNIK_BLOKOWANIA) <= 13)
	{
		for (int i = 0; i < WIELKOSC_TABLICY; i++) {
			Bufor.zawartosc[Bufor.liczbaBajtow + i] = T.zawartosc[indeksT][i];
			//		cout << Bufor.zawartosc[Bufor.liczbaBajtow + i] << endl;
		}
		Bufor.liczbaBajtow += WIELKOSC_TABLICY;
	}
	else {
		Bufor.numerStrony++;
		Bufor.liczbaBajtow += (WIELKOSC_TABLICY - 1);
		for (int i = 0; i < WIELKOSC_TABLICY; i++) {
			Bufor.zawartosc[Bufor.liczbaBajtow + i] = T.zawartosc[indeksT][i];
			//	cout << Bufor.zawartosc[Bufor.liczbaBajtow + i] << endl;
		}
		Bufor.liczbaBajtow += WIELKOSC_TABLICY;
	}


	if (Bufor.wielkoscBufora - Bufor.liczbaBajtow < WIELKOSC_TABLICY)
		return true;
	else
		return false;
}

tasma losujTablice(tasma tablice) {

	srand(time(NULL));
	tasma tab = tablice;


	for (int numerTablicy = 0; numerTablicy < tab.ileRekordow; numerTablicy++) {
		int indeks = 0;
		int liczbaLiter = rand() % MAX_LITER + 1;
		for (int numerIndeksu = 0; numerIndeksu < 3; numerIndeksu++, indeks++)
			tab.zawartosc[numerTablicy][indeks] = rand() % 26 + 65;

		for (int numerIndeksu = 3; numerIndeksu < WIELKOSC_TABLICY; numerIndeksu++, indeks++) {
			tab.zawartosc[numerTablicy][indeks] = rand() % 43 + 48;
			while ((tab.zawartosc[numerTablicy][indeks] > 57 && tab.zawartosc[numerTablicy][indeks] < 65) || (tab.zawartosc[numerTablicy][indeks] >= 65 && liczbaLiter < 1))
				tab.zawartosc[numerTablicy][indeks] = rand() % 43 + 48;
			if (tab.zawartosc[numerTablicy][indeks] >= 65)
				liczbaLiter--;
		}
	}


	return tab;
}

tasma generujTabliceLosowo(int liczbaRekordow) {
	tasma tablice(liczbaRekordow);
	tablice = losujTablice(tablice);

	return tablice;
}

void zapiszDoPliku(tasma listaRekordow, string sciezka)
{
	ofstream plik(sciezka);

	ofstream write_file;

	write_file = ofstream(sciezka, ios::out | ios::binary);

	for (int i = 0; i < listaRekordow.ileRekordow; i++)
	{
		write_file.write(listaRekordow.zawartosc[i], 7);
	}
	write_file.close();

}

long liczbaBajtow(string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

tasma wczytajZPlikuDat(string sciezka) {

	string linia;
	ifstream write_file;
	write_file = ifstream(sciezka, ios::out | ios::binary);


	tasma T3(liczbaBajtow(sciezka) / WIELKOSC_TABLICY);

	char znak;
	for (int i = 0; i < liczbaBajtow(sciezka) / WIELKOSC_TABLICY; i++) {
		for (int j = 0; j < WIELKOSC_TABLICY; j++) {
			write_file >> znak;
			T3.zawartosc[i][j] = znak;
		}
	}
	ofstream file;
	file.open(sciezka, ios::out | ios::binary);
	file.close();

	return T3;
}

tasma wczytajZPliku(string sciezka) {
	string linia;
	int licznik = 0;
	fstream plik(sciezka);

	if (plik.good()) {
		while (!plik.eof())
		{
			getline(plik, linia);
			licznik++;
		}
		plik.close();
	}
	tasma T3(licznik);
	ifstream plikk(sciezka);

	if (plikk.good()) {
		while (!plikk.eof())
		{
			getline(plikk, linia);

			for (int i = 0; i < WIELKOSC_TABLICY; i++) {
				T3.zawartosc[(T3.ileRekordow) - (licznik)][i] = linia[i];
			}
			licznik--;
			if (licznik == 0)
				break;
		}
		plikk.close();
	}

	return T3;
}

tasma przygotujTablice() {
	cout << "Podaj liczbe rekordow" << endl;
	int liczbaRekordow;
	cin >> liczbaRekordow;
	tasma tablice(liczbaRekordow); ;
	return tablice;
}

void wygenerujLosowo() {
	tasma tablice = przygotujTablice();
	tablice = generujTabliceLosowo(tablice.ileRekordow);
	cout << "Wpisz sciezke do pliku" << endl;
	string sciezka;
	cin >> sciezka;
	zapiszDoPliku(tablice, sciezka);
}

void wpiszZKlawiatury() {
	tasma tablice = przygotujTablice();
	for (int numerRekordu = 0; numerRekordu < tablice.ileRekordow; numerRekordu++) {
		for (int indeksTablicy = 0; indeksTablicy < WIELKOSC_TABLICY; indeksTablicy++)
			cin >> tablice.zawartosc[numerRekordu][indeksTablicy];
	}
	cout << "Wpisz sciezke do pliku" << endl;
	string sciezka;
	cin >> sciezka;
	zapiszDoPliku(tablice, sciezka);
}

bool posortujDwa(char* pierwsza, char* druga) {
	bool ktoraMniejsza = 0;
	for (int numerLitery = 0; numerLitery < WIELKOSC_TABLICY; numerLitery++) {

		if (pierwsza[numerLitery] - druga[numerLitery] < 0) {
			ktoraMniejsza = 0;
			break;
		}
		else if (pierwsza[numerLitery] - druga[numerLitery] > 0) {
			ktoraMniejsza = 1;
			break;
		}
	}
	return ktoraMniejsza;
}

void wyswietlTasme(tasma T) {
	for (int i = 0; i < T.ileRekordow; i++) {
		for (int j = 0; j < WIELKOSC_TABLICY; j++)
			cout << T.zawartosc[i][j];
		cout << endl;
	}


}

void rozdziel() {
	tasma T3 = wczytajZPlikuDat("t3.dat");
	//wyswietlTasme(T3);
	tasma T1(T3.ileRekordow);
	tasma T2(T3.ileRekordow);

	bool ktoraTasma = 0;
	int licznikGlobalny = 0;
	bool koniec = 0;




	int licznik = 0;
	int licznikPierwszejTasmy = 0;
	int licznikDrugiejTasmy = 0;
	while (true) {

		if (posortujDwa(T3.zawartosc[licznikGlobalny], T3.zawartosc[licznikGlobalny + 1]) == 1) {
			if (fazy == 1)
				liczbaSerii++;
			for (int i = 0; i <= licznik; i++) {
				if (ktoraTasma == 0)
					T1.zawartosc[licznikPierwszejTasmy + i] = T3.zawartosc[licznikGlobalny - licznik + i];
				else if (ktoraTasma == 1)
					T2.zawartosc[licznikDrugiejTasmy + i] = T3.zawartosc[licznikGlobalny - licznik + i];

			}
			if (ktoraTasma == 0)
				licznikPierwszejTasmy += licznik + 1;
			else if (ktoraTasma == 1)
				licznikDrugiejTasmy += licznik + 1;

			licznik = 0;
			ktoraTasma = !ktoraTasma;
		}
		else
			licznik++;

		licznikGlobalny++;
		if (licznikGlobalny + 1 >= T3.ileRekordow) {
			for (int i = 0; i <= licznikGlobalny - licznikPierwszejTasmy - licznikDrugiejTasmy; i++) {
				if (ktoraTasma == 0) {
					T1.zawartosc[licznikPierwszejTasmy + i] = T3.zawartosc[licznikGlobalny - licznik + i];
				}
				else if (ktoraTasma == 1) {
					T2.zawartosc[licznikDrugiejTasmy + i] = T3.zawartosc[licznikGlobalny - licznik + i];
				}
			}
			if (ktoraTasma == 0)
				licznikPierwszejTasmy += T3.ileRekordow - licznikPierwszejTasmy - licznikDrugiejTasmy;
			else if (ktoraTasma == 1)
				licznikDrugiejTasmy += T3.ileRekordow - licznikPierwszejTasmy - licznikDrugiejTasmy;
			koniec = true;
			break;
		}
	}
	T1.ileRekordow = licznikPierwszejTasmy;
	T2.ileRekordow = licznikDrugiejTasmy;
	zapiszDoPliku(T1, "t1.dat");
	zapiszDoPliku(T2, "t2.dat");
}

tasma wrzucDoT3(tasma T3, int indeksGlownyT3) {
	operacje++;
	for (int i = 0; i < Bufor.liczbaBajtow; i++) {
		//	cout << Bufor.zawartosc[i] << endl;
		if (Bufor.zawartosc[i] >= 48 && Bufor.zawartosc[i] <= 90) {

			for (int j = 0; j < WIELKOSC_TABLICY; j++) {
				T3.zawartosc[indeksGlownyT3][j] = Bufor.zawartosc[i + j];
				Bufor.zawartosc[i + j] = 0;
			}
			i += 6;
			indeksGlownyT3++;
		}
	}
	Bufor.liczbaBajtow = 0;
	Bufor.numerStrony = 0;
	for (int i = 0; i < 140; i++) {
		Bufor.zawartosc[i] = 0;
	}
	return T3;
}

bool sklej() {
	tasma T1(wczytajZPlikuDat("t1.dat"));
	tasma T2(wczytajZPlikuDat("t2.dat"));
	tasma T3(T1.ileRekordow + T2.ileRekordow);

	/*cout << "Tasma 1:" << endl;
	wyswietlTasme(T1);
	cout << endl << "Tasma 2:" << endl;;
	wyswietlTasme(T2);
	cout << endl;*/

	bool czyKoniecBufora = false;
	bool ostatniT1 = false;
	bool ostatniT2 = false;
	bool koniecT1 = false;
	bool koniecT2 = false;
	int indeksGlownyT1 = 0;
	int indeksGlownyT2 = 0;
	int indeksGlownyT3 = 0;
	bool czyOstatniaFaza = true;
	while (true) {

		if (T1.ileRekordow == 0 || T2.ileRekordow == 0) {
			T3 = T1;
			/*	cout << "Tasma glowna:" << endl;
				wyswietlTasme(T3);
				cout << endl;*/
			break;
		}
		if (posortujDwa(T1.zawartosc[indeksGlownyT1], T2.zawartosc[indeksGlownyT2]) == 0) {
			czyKoniecBufora = wczytajDoBufora(T1, indeksGlownyT1);
			if (czyKoniecBufora == true)
			{
				T3 = wrzucDoT3(T3, indeksGlownyT3);
				indeksGlownyT3 += 14;
				czyKoniecBufora = false;
			}

			indeksGlownyT1++;
			if (indeksGlownyT1 != T1.ileRekordow) {
				if (posortujDwa(T1.zawartosc[indeksGlownyT1 - 1], T1.zawartosc[indeksGlownyT1]) == 1) {
					ostatniT1 = true;
					czyOstatniaFaza = false;
				}
			}
			else
				koniecT1 = true;
		}
		else if (posortujDwa(T2.zawartosc[indeksGlownyT2], T1.zawartosc[indeksGlownyT1]) == 0) {
			czyKoniecBufora = wczytajDoBufora(T2, indeksGlownyT2);
			if (czyKoniecBufora == true)
			{
				T3 = wrzucDoT3(T3, indeksGlownyT3);
				indeksGlownyT3 += 14;
				czyKoniecBufora = false;
			}

			indeksGlownyT2++;
			if (indeksGlownyT2 != T2.ileRekordow) {
				if (posortujDwa(T2.zawartosc[indeksGlownyT2 - 1], T2.zawartosc[indeksGlownyT2]) == 1) {

					ostatniT2 = true;
					czyOstatniaFaza = false;
				}
			}
			else
				koniecT2 = true;
		}


		if (ostatniT1 == true) {
			czyKoniecBufora = wczytajDoBufora(T2, indeksGlownyT2);
			if (czyKoniecBufora == true)
			{
				T3 = wrzucDoT3(T3, indeksGlownyT3);
				indeksGlownyT3 += 14;
				czyKoniecBufora = false;
			}
			indeksGlownyT2++;
			if (indeksGlownyT2 != T2.ileRekordow) {

				while (posortujDwa(T2.zawartosc[indeksGlownyT2 - 1], T2.zawartosc[indeksGlownyT2]) == 0) {

					czyKoniecBufora = wczytajDoBufora(T2, indeksGlownyT2);
					if (czyKoniecBufora == true)
					{
						T3 = wrzucDoT3(T3, indeksGlownyT3);
						indeksGlownyT3 += 14;
						czyKoniecBufora = false;
					}
					indeksGlownyT2++;
					if (indeksGlownyT2 == T2.ileRekordow) {
						koniecT2 = true;
						break;
					}
				}
				ostatniT1 = false;
			}
			else {
				koniecT2 = true;
				ostatniT1 = false;

			}
		}
		else if (ostatniT2 == true) {
			czyKoniecBufora = wczytajDoBufora(T1, indeksGlownyT1);
			if (czyKoniecBufora == true)
			{
				T3 = wrzucDoT3(T3, indeksGlownyT3);
				indeksGlownyT3 += 14;
				czyKoniecBufora = false;
			}
			indeksGlownyT1++;
			if (indeksGlownyT1 != T1.ileRekordow) {
				while (posortujDwa(T1.zawartosc[indeksGlownyT1 - 1], T1.zawartosc[indeksGlownyT1]) == 0) {
					czyKoniecBufora = wczytajDoBufora(T1, indeksGlownyT1);
					if (czyKoniecBufora == true)
					{
						T3 = wrzucDoT3(T3, indeksGlownyT3);
						indeksGlownyT3 += 14;
						czyKoniecBufora = false;
					}
					indeksGlownyT1++;
					if (indeksGlownyT1 == T1.ileRekordow) {
						koniecT1 = true;
						break;
					}
				}
				ostatniT2 = false;
			}
			else {
				koniecT1 = true;
				ostatniT2 = false;
			}
		}

		if (koniecT1 == true) {
			while (indeksGlownyT2 != T2.ileRekordow) {
				czyKoniecBufora = wczytajDoBufora(T2, indeksGlownyT2);
				if (czyKoniecBufora == true)
				{
					T3 = wrzucDoT3(T3, indeksGlownyT3);
					indeksGlownyT3 += 14;
					czyKoniecBufora = false;
				}
				if (posortujDwa(T3.zawartosc[indeksGlownyT1 + indeksGlownyT2 - 1], T3.zawartosc[indeksGlownyT1 + indeksGlownyT2]) == 1)
					czyOstatniaFaza = false;
				indeksGlownyT2++;
			}
			T3 = wrzucDoT3(T3, indeksGlownyT3);

		}
		else if (koniecT2 == true) {
			while (indeksGlownyT1 != T1.ileRekordow) {
				czyKoniecBufora = wczytajDoBufora(T1, indeksGlownyT1);
				if (czyKoniecBufora == true)
				{
					T3 = wrzucDoT3(T3, indeksGlownyT3);
					indeksGlownyT3 += 14;
					czyKoniecBufora = false;
				}
				if (posortujDwa(T3.zawartosc[indeksGlownyT1 + indeksGlownyT2 - 1], T3.zawartosc[indeksGlownyT1 + indeksGlownyT2]) == 1)
					czyOstatniaFaza = false;

				indeksGlownyT1++;
			}
			T3 = wrzucDoT3(T3, indeksGlownyT3);
		}

		if (T3.ileRekordow == indeksGlownyT1 + indeksGlownyT2) {
			/*cout << "Tasma glowna:" << endl;
			wyswietlTasme(T3);
			cout << endl;*/
			break;
		}
	}
	/*cout << "Tasma glowna:" << endl;
	wyswietlTasme(T3);
	cout << endl;*/
	zapiszDoPliku(T3, "t3.dat");
	return czyOstatniaFaza;
}

void wczytajTestowy() {
	tasma T3 = wczytajZPliku("plik.txt");
	zapiszDoPliku(T3, "t3.dat");
}

void menu()
{
	cout << "1. Losowa generacja rekordow\n";
	cout << "2. Wpisanie rekordow z klawiatury\n";
	cout << "3. Plik testowy\n";
	int wybor;
	cin >> wybor;

	switch (wybor)
	{
	case 1:
		wygenerujLosowo();
		break;
	case 2:
		wpiszZKlawiatury();
		break;
	case 3:
		wczytajTestowy();
		break;

	}
}

void posortuj() {
	bool czyOstatniaFaza = false;
	int liczbaFaz = 0;
	while (czyOstatniaFaza == false) {

		fazy++;
		liczbaFaz++;
		rozdziel();
		czyOstatniaFaza = sklej();
	}
}