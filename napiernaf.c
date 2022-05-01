/**
 * Trzeci projekt zaliczeniowy: arytmetyka napiernaf.
 * Implementacja podstawowych działań arytmetycznych na liczbach zapisanych
 * w systemie napiernaf.
 * autor: Jan Roguwski
 * Data:
 * 2022-01-11
 */
#include<stdlib.h>
#include<limits.h>

/*Definicja stałej: Jeśli najbardziej znacząca cyfra w zapisie
napiernaf liczby jest na pozycji k>=M, wtedy liczba ta na pewno
nie mieści się w typie int.*/
#define M 32

/*Realokuje wskaźnik A na fragment pamięci rozmiaru bn, i przepisuje do A wartości tablicy B. Zwalnia pamięć zaalokowaną na B.
Aktualizuje an na nowy rozmiar tablicy A.*/
void przepisz(int **A, int *an, int *B, int bn)
{
    /*jJeśli rozmiar tablicy *A jest dodatni, to zwalniamy tę tablicę.*/
    if((*an)>0) free((*A));
    (*A)=NULL;

    /*Realokacja pamięci, przepisanie wartości, zwolnienie tablicy B.*/
    (*A)=realloc((*A), (size_t)bn*sizeof(int));
    *an=bn;
    for(int i=0; i<bn; i++) (*A)[i]=B[i];
    if(B!=NULL) free(B);

    return;
}

/*Zapisuje reprezentację liczby x w dynamicznie utworzonej *n-elementowej tablicy (*a).*/
void iton(int x, int **a, int *n)
{
    /*Zmienne pomocnicze:*/
    int *wynik=NULL, wn=0, pozycja=0, mod=0, liczba=x;

    /*Realizacja algorytmu wyznaczania zapisu BBR-NAF,
    oraz przetłumaczenie zapisu BBR-NAF na zapis napiernaf:*/
    while(liczba!=0)
    {
        /*Wartość liczba modulo 4:*/
        mod=0;

        if(liczba%2!=0)
        {
            /*Jeśli liczba%2==0, to w zapisie BBR-NAF występuje 0, zatem pomijamy je w zapisie Napier-NAF.
            Liczba jest nieparzysta, zatem mamy nierówności INT_MIN < liczba <= INT_MAX.*/
            if((liczba-1)%4==0)//liczba==1 mod 4, liczba-1 >= INT_MIN.
            {
                mod=1;
                /*Zapisujemy liczbę w tablicy, więc rozmiar tablicy się zwiększa.*/
                wn++;
                wynik=realloc(wynik, (size_t)wn*sizeof(int));
                wynik[wn-1]=pozycja;
            }
            else if(liczba==INT_MAX || (liczba+1)%4==0)//x==(-1) mod 4
            {
                mod=(-1);
                /*Zapisujemy liczbę w tablicy, więc rozmiar tablicy się zwiększa.*/
                wn++;
                wynik=realloc(wynik, (size_t)wn*sizeof(int));
                wynik[wn-1]=(-1)-pozycja;
            }
        }
        /*Zwiększamy wskaźnik pozycja – przechodzimy na następną cyfrę w zapisie BBR-NAF.*/
        pozycja++;
        /*Dzielimy x na 2, rozbicie na przypadki w celu uniknięcia przekroczenia wartości maksymalnej typu int.*/
        if(liczba<INT_MAX) liczba=(liczba-mod)/2;
        else liczba=(liczba/2)+1;
    }
    *a=wynik;
    *n=wn;
    return;
}

/*k jest całkowite, nieujemne.
Dla k<M-1=31, zwraca wartość 2^k.
dla k>=M-1=31, zwraca (-1).*/
int pot2(int k)
{
    /*Zmienne pomocnicze:*/
    int w=1, a=2;

    if(k>=M-1) w=(-1);
    else
    {
        /*Algorytm szybkiego potęgowania:*/
        while(k>0)
        {
            if(k%2==1) w*=a;
            if(k>1) a*=a;
            k/=2;
        }
    }
    return w;
}

/*Daje jako wynik wartość liczby reprezentowanej przez n-elementową tablicę a, lub 0 jeśli wartość ta nie mieści się w zakresie typu int.
Niech A oznacza liczbę zapisaną systemem BBR-NAF w tablicy a.
Niech M oznacza pozycję najbardziej znaczącej cyfry w zapisie liczby A.
Proste szacowania pozwalają dojść do wniosku, że gdy cyfrą tą jest 1, to A > 2^(M-1).
Analogicznie, gdy cyfrą jest (-1), mamy A < -2^(M-1).
Stąd wynika wniosek, że dla M >= 32, liczba A na pewno nie mieści się w typie int (bo INT_MAX = 2^31-1 oraz INT_MIN = -2^31).
Dlatego nie ma sensu wywoływać funkcji pot2 z argumentami >= 32, bo możemy od razu stwierdzić, że liczba nie mieści się w typie int.
Natomiast wywołanie pot2(31) zwróci wartość (-1), która oznacza wartość INT_MAX+1=2^31.*/
int ntoi(int *a, int n)
{
    /*Zmienne pomocnicze:*/
    long long wynik=0;
    int potega, wykladnik, cyfra;

    for(int i=0; i<n; i++)
    {
        if(a[i]>=0)
        {
            cyfra=1;
            wykladnik=a[i];
        }
        else
        {
            cyfra=(-1);
            wykladnik=(-1)*(a[i])-1;
        }
        if(wykladnik>=M)
        {
            /*Na mocy obserwacji wiemy, że A nie mieści się w typie int.*/
            i=n;
            wynik=0;
        }
        else
        {
            /*Musimy policzyć 2^wykladnik i dodać cyfra*potega do wyniku, uwzględniając przypadek, gdy potega==-1 (przekracza int).*/
            potega=pot2(wykladnik);
	        if(potega==(-1)) wynik+=cyfra*((long long)INT_MAX+1);
            else wynik+=(long long)cyfra*potega;
        }
    }
    if(wynik<(long long)INT_MIN || wynik>(long long)INT_MAX) wynik=0;
    return (int)wynik;
}

/*Max*/
int max(int a, int b)
{
    if(a>b) return a;
    return b;
}

/*Min*/
int min(int a, int b)
{
    if(a>b) return b;
    return a;
}

/*Dostaje tablicę z zapisem napiernaf, odczytuje liczbę A[wa] (jeśli wa<n) i odczytuje cyfrę i jej pozycję w zapisie bbr-naf.
Jeśli wa>=n, ustawia cyfrę na 0, a pozycję na -1.*/
void pobierz_cyfre(int *A, int n, int wa, int *cyfra, int *poz)
{
    /*Zmienne pomocnicze:*/
    int c=0, p=-1;

    if(wa<n)
    {
        if(A[wa]>=0)
        {
            c=1;
            p=A[wa];
        }
        else
        {
            c=-1;
            p=(-1)*(A[wa])-1;
        }
    }
    *cyfra=c;
    *poz=p;
    return;
}

/*Dostaje pozycje i tablice z zapisem napiernaf, zwraca cyfrę (0,1 lub -1), która wystepuje
na danej pozycji w zapisie bbr-naf tej liczby.*/
void czytaj_nastepne_cyfry(int *cyfraa, int *cyfrab, int pozycja, int *A, int *B, int an, int bn, int wa, int wb)
{
    /*Zmienne pomocnicze:*/
    int poza=-1, pozb=-1;

    *cyfraa=0; *cyfrab=0;
    pobierz_cyfre(A, an, wa, cyfraa, &poza);
    if(poza!=pozycja) *cyfraa=0;
    pobierz_cyfre(B, bn, wb, cyfrab, &pozb);
    if(pozb!=pozycja) *cyfrab=0;
    return;
}

/*Jeśli pom==0: patrzy na tablice A i B pod wskaźnikami wa, wb. wybiera ten z nich, który wskazuje na mniejszą pozycję w zapisie i ustawia
na tę pozycję zmienną pozycja. Następnie zwiększa ten wskaźnik (być może oba) i zapisuje cyfry jakie są w zapisach bbr-naf liczb A, B.
Jeśli pom!=0: zwiększa pozycję o 1 i ustawia cyfry na cyfry występujące na tej pozycji w zapisach bbr-naf liczb A,B. być może aktualizuje wskaźniki wa,wb.*/
void czytaj_cyfry(int *cyfraa, int *cyfrab, int *pozycja, int *A, int *B, int an, int bn, int *wa, int *wb, int pom)
{
    /*Zmienne pomocnicze:*/
    int poza=-1, pozb=-1;

    *cyfraa=0; *cyfrab=0;
    if(pom)
    {
        *pozycja=(*pozycja)+1;
        if(*wa<an)
        {
            pobierz_cyfre(A, an, *wa, cyfraa, &poza);
            if(poza!=*pozycja) *cyfraa=0;
            else (*wa)++;
        }
        if(*wb<bn)
        {
            pobierz_cyfre(B, bn, *wb, cyfrab, &pozb);
            if(pozb!=*pozycja) *cyfrab=0;
            else (*wb)++;
        }
    }
    else//pom==0
    {
        pobierz_cyfre(A, an, *wa, cyfraa, &poza);
        pobierz_cyfre(B, bn, *wb, cyfrab, &pozb);
        if(poza>=0 && pozb>=0) *pozycja=min(poza, pozb);
        else *pozycja=max(poza, pozb);
        if(poza!=*pozycja) *cyfraa=0;
        else (*wa)++;
        if(pozb!=*pozycja) *cyfrab=0;
        else (*wb)++;
    }
    return;
}

/*Zapisuje w dynamicznie utworzonej *cn-elementowej tablicy *c sumę liczb reprezentowanych przez
an-elementową tablicę a i bn-elementową tablicę b.*/
void nadd(int *a, int an, int *b, int bn, int **c, int *cn)
{
    /*Zmienne pomocnicze:*/
    int *wynik=NULL, n=0, wa=0, wb=0, pozycja=0,//Pozycja jest zawsze <= pozycje zapisane w a[wa], b[wb];
        cyfraa, cyfrab,
        pom=0, rez, rez_nast, mod=0;

    while(wa<an || wb<bn)
    {
        czytaj_cyfry(&cyfraa, &cyfrab, &pozycja, a, b, an, bn, &wa, &wb, pom);
        rez=pom+cyfraa+cyfrab;
        if(rez%2==0) mod=0;
        else
        {
            /*Trzeba coś zapisać w napiernafie: musimy spojrzeć na resztę mod 4.*/
            n++;
            wynik=realloc(wynik, (size_t)n*sizeof(int));
            czytaj_nastepne_cyfry(&cyfraa, &cyfrab, pozycja+1, a, b, an, bn, wa, wb);
            rez_nast=rez+2*(cyfraa+cyfrab);
            if((rez_nast-1)%4==0)//Trzeba zapisać 1.
            {
                mod=1;
                wynik[n-1]=pozycja;
            }
            else//Zapisujemy -1.
            {
                mod=(-1);
                wynik[n-1]=(-1)*pozycja-1;
            }
        }
        pom=(rez-mod)/2;
    }
    if(pom!=0)
    {
        n++;
        wynik=realloc(wynik, (size_t)n*sizeof(int));
        if(pom==1) wynik[n-1]=pozycja+1;
        else wynik[n-1]=(-1)*pozycja-2;
    }
    *c=wynik;
    *cn=n;
    return;
}

/*Dostaje n elementową tablice *A z zapisem napiernaf liczby i zapisuje na niej zapis napiernaf liczby przeciwnej.*/
void nneg(int *A, int n)
{
    for(int i=0; i<n; i++) A[i]=(-1)*A[i]-1;
    return;
}

/*zapisuje w dynamicznie utworzonej *cn-elementowej tablicy *c
różnicę liczb reprezentowanych przez an-elementową tablicę a i bn-elementową tablicę b*/
void nsub(int *a, int an, int *b, int bn, int **c, int *cn)
{
    /*Kod neguje liczbę b, korzysta z funkcji nadd, a potem przywraca pierwotny stan tablicy b negując ją drugi raz.*/

    /*Zmienne pomocnicze:*/
    int *wynik=NULL, wn=0;

    nneg(b, bn);
    nadd(a, an, b, bn, &wynik, &wn);
    nneg(b, bn);
    *c=wynik;
    *cn=wn;
    return;
}

/*Tablica T zawiera zapis napiernaf, a zmienna x zawiera liczbę całkowitą (jednoelementowy zapis napiernaf potęgi dwójki).
Funkcja zapisuje w dynamicznie utworzonej tablicy **T_przesunięte zapis liczby T pomnożonej przez liczbę x (w napiernafie)
n==rozmiar tablicy T.*/
void przesun(int *T, int **T_przesuniete, int x, int n)
{
    /*Zmienne pomocnicze:*/
    int *wynik=NULL, ujemna=0;

    if(n>0)
    {
        wynik=malloc((size_t)n*sizeof(int));
        if(x<0)
        {
            x=(-1)-x;
            nneg(T, n);
            ujemna=1;
        }
        for(int i=0; i<n; i++)
        {
            if(T[i]>=0) wynik[i]=x+T[i];
            else wynik[i]=T[i]-x;
        }
        if(ujemna) nneg(T, n);//Przywrócenie pierwotnego stanu tablicy T.
    }
    *T_przesuniete=wynik;
    return;
}

/*Zapisuje w dynamicznie utworzonej *cn-elementowej tablicy *c
iloczyn liczb reprezentowanych przez an-elementową tablicę a i bn-elementową tablicę b.*/
void nmul(int *a, int an, int *b, int bn, int **c, int *cn)
{
    /*Zmienne pomocnicze:*/
    int *placeholder=NULL, phn=0, *wynik=NULL, wn=0, *b_przesuniete=NULL;

    if(an>0 && bn>0)
    {
        for(int i=0; i<an; i++)
        {
            przesun(b, &b_przesuniete, a[i], bn);//Tablica b_przesunięte jest przesuniętą tablicą b.
            nadd(b_przesuniete, bn, wynik, wn, &placeholder, &phn);
            /*Aktualizacja tablicy wynik: wartości placeholdera kopiujemy do tablicy wynik.*/
            przepisz(&wynik, &wn, placeholder, phn);
            free(b_przesuniete);
        }
    }
    *c=wynik;
    *cn=wn;
    return;
}

/*Funkcja do dzielenia na dwa na potrzeby szybkiego potęgowania:
dostaje tablicę T o rozmiarze n zawierającą reprezentację napiernaf pewnej liczby całkowitej x (dodatniej).
Tablicę T ustawia w zależności od przypadku:
x==2m: tablica T zawiera reprezentację napiernaf liczby m;
x==4k+1: tablica T zawiera reprezentację napiernaf liczby 2k;
x==4k+3: tablica T zawiera reprezentację napiernaf liczby 2k+1;
ponadto ustawia zmienną *n na nowy rozmiar tablicy T.*/
void na_dwa(int **T, int *n)
{
    /*Zmienne pomocnicze:*/
    int *tab=(*T),//Wskaźnik pomocniczy.
        *wynik=NULL, wn=0, *placeholder=NULL, phn=0, minus_jeden[1]={-1};

    if(tab!=NULL)
    {
        /*Patrzymy na pierwszy element zapisu, żeby ustalić parzystość:*/
        if(tab[0]!=0 && tab[0]!=(-1)) //Liczba jest parzysta.
        {
            wn=*n;
            wynik=malloc((size_t)wn*sizeof(int));
            for(int i=0; i<wn; i++)
            {
                if(tab[i]>=0) wynik[i]=tab[i]-1;
                else wynik[i]=tab[i]+1;
            }
        }
        else
        {
            /*Liczba nieparzysta: musimy sprawdzić resztę mod 4 i podzielić zgodnie ze specyfikacją funkcji:*/
            if(tab[0]==0)
            {
                /*Liczba jest postaci 4k+1; Chcemy zwrócić zapis liczby 2k, zatem zapominamy o pierwszym elemencie i pozostałe "dzielimy na dwa".*/
                wn=((*n)-1);
                wynik=malloc((size_t)wn*sizeof(int));
                for(int i=0; i<wn; i++)
                {
                    if(tab[i+1]>=0) wynik[i]=tab[i+1]-1;
                    else wynik[i]=tab[i+1]+1;
                }
            }
            else
            {
                /*tab[0]==(-1), zatem liczba jest postaci 4k+3; Chcemy zwrócić 2k+1. Zapominamy o pierwszym elemencie i mamy zapis liczby 4(k+1).
                Dzielimy na 2 i mamy zapis 2k+2. wystarczy odjąć 1 (np. funkcją nadd):*/
                wn=((*n)-1);
                wynik=malloc((size_t)wn*sizeof(int));
                for(int i=0; i<wn; i++)//Dzielimy na dwa zapominając o tab[0]:
                {
                    if(tab[i+1]>=0) wynik[i]=tab[i+1]-1;
                    else wynik[i]=tab[i+1]+1;
                }
                nadd(minus_jeden, 1, wynik, wn, &placeholder, &phn);
                /*Wartości placeholdera stają się wartościami wyniku:*/
                przepisz(&wynik, &wn, placeholder, phn);
            }
        }
        /*Rozwiązanie zadania jest już na osobno zaalokowanej pamięci na wskaźniku wynik. Zwalniamy niepotrzebną tablicę *T==tab:*/
        free(tab);
    }
    *T=wynik;
    *n=wn;
    return;
}

/*Zapisuje w dynamicznie utworzonej *cn-elementowej tablicy *c wynik potęgowania,
którego podstawa jest reprezentowana przez an-elementową tablicę a,
a nieujemny wykładnik jest reprezentowany przez bn-elementową tablicę b.*/
void nexp(int *a, int an, int *b, int bn, int **c, int *cn)
{
    /*Zmienne pomocnicze:*/
    int *wynik=NULL, wn=0, *placeholder=NULL, phn=0,
        *mnoznik=NULL, mn=0, *pom=NULL, pn=0;

    if(an>0)
    {
        wynik=malloc(sizeof(int));
        wn=1;
        wynik[0]=0;//Wynik musi na początku zawierać reprezentację liczby 1.
        mnoznik=malloc((size_t)an*sizeof(int));
        mn=an;
        for(int i=0; i<an; i++) mnoznik[i]=a[i];//Pomocnicza tablica do modyfikowania żeby nie stracić zawartości podstawy potęgowania.
        pom=malloc((size_t)bn*sizeof(int));
        pn=bn;
        for(int i=0; i<bn; i++) pom[i]=b[i];//Pomocnicza tablica do modyfikowania, żeby nie stracić zawartości wykladnika.
        while(pn>0)//Dopoki wykladnik>0.
        {
            if(pom[0]==0 || pom[0]==(-1))
            {
                nmul(wynik, wn, mnoznik, mn, &placeholder, &phn);
                przepisz(&wynik, &wn, placeholder, phn);
            }
            nmul(mnoznik, mn, mnoznik, mn, &placeholder, &phn);
            przepisz(&mnoznik, &mn, placeholder, phn);
            na_dwa(&pom, &pn);
        }
        free(pom);
        free(mnoznik);
    }
    *c=wynik;
    *cn=wn;
    return;
}

/*Zwraca znak(1-dodatnia, 0-zero, -1-ujemna) liczby zapisanej napiernafem w tablicy A.*/
int znak(int *A, int an)
{
    /*Zmienne pomocnicze:*/
    int wynik=0;

    if(an>0)
    {
        if(A[an-1]>=0) wynik=1;
        else wynik=(-1);
    }
    return wynik;
}

/*zwraca 1, jeśli liczba zapisana w tablicy a jest większa od liczby zapisanej w tablicy b, 0 w przeciwnym przypadku.*/
int wieksza(int *a, int an, int *b, int bn)
{
    /*Zmienne pomocnicze:*/
    int wynik=0, *pom=NULL, pn=0, w;

    nsub(a, an, b, bn, &pom, &pn);//pom=a-b w napiernafie.
    w=znak(pom, pn);
    if(w==1) wynik=1;
    free(pom);
    return wynik;
}

/*Dzieli liczbę, której reprezentacją jest an-elementowa tablica a, przez niezerową liczbę, której reprezentacją jest bn-elementowa tablica b.
Iloraz zapisuje w dynamicznie utworzonej *qn-elementowej tablicy *q.
Resztę z dzielenia zapisuje w dynamicznie utworzonej *rn-elementowej tablicy *r.*/
void ndivmod(int *a, int an, int *b, int bn, int **q, int *qn, int **r, int *rn)
{
    /*Zmienne pomocnicze:*/
    int *potega=NULL, potn=0,/*zmienna pomocnicza potega ma początkową wartość 1;*/
        *jeden=malloc(sizeof(int)),/*zmienna pomocnicza – reprezentacja napiernaf liczby 1;*/
        *dwa=malloc(sizeof(int)),/*zmienna pomocnicza – reprezentacja napiernaf liczby 2;*/
        *iloraz=NULL, iln=0, *reszta=NULL, ren=0, *placeholder=NULL, phn=0,
        znak_A=znak(a, an), znak_B=znak(b, bn),
        *wiel=NULL, wieln=0, *wiel_nast=NULL, wiel_nastn=0,
        *nowy_iloraz=NULL, nowy_iln=0, *nowe_a=NULL, nowe_an=0;

    jeden[0]=0; dwa[0]=1;
    if(znak_A!=0)
    {
        if(znak_A==(-1))
        {
            if(znak_B==1)//A<0, B>0
            {
                nneg(a, an);
                ndivmod(a, an, b, bn, &iloraz, &iln, &reszta, &ren);
                nneg(iloraz, iln);
                if(ren>0)
                {
                    nsub(iloraz, iln, jeden, 1, &iloraz, &iln);//iloraz-=1 w napiernafie.
                    nsub(b, bn, reszta, ren, &placeholder, &phn);//reszta=B-reszta w napiernafie.
                    przepisz(&reszta, &ren, placeholder, phn);
                }
                /*Odnegowanie liczby A:*/
                nneg(a, an);
            }
            else//A, B ujemne.
            {
                nneg(a, an);
                nneg(b, bn);
                ndivmod(a, an, b, bn, &iloraz, &iln, &reszta, &ren);
                if(ren>0)
                {
                    nadd(iloraz, iln, jeden, 1, &placeholder, &phn);//iloraz++ w napiernafie.
                    przepisz(&iloraz, &iln, placeholder, phn);
                    nsub(b, bn, reszta, ren, &placeholder, &phn);//reszta=B-reszta w napiernafie.
                    przepisz(&reszta, &ren, placeholder, phn);
                }
                nneg(a, an);
                nneg(b, bn);
            }
        }
        else//A>0
        {
            if(znak_B==(-1))//A>0, B<0:
            {
                nneg(b, bn);
                ndivmod(a, an, b, bn, &iloraz, &iln, &reszta, &ren);
                nneg(iloraz, iln);
                nneg(b, bn);
            }
            else//A>0, B>0; Główna część algorytmu:
            {
                if(wieksza(b, bn, a, an))//Jeśli a<b, to iloraz=0, reszta=a;
                {
                    reszta=malloc((size_t)an*sizeof(int));
                    ren=an;
                    for(int i=0; i<ren; i++) reszta[i]=a[i];
                }
                else//a>=b
                {
                    /*Zmienna pomocnicza, na początku ma wartość b, potem jest zwiększana dwukrotnie w każdej iteracji.*/
                    wiel=malloc((size_t)bn*sizeof(int)); wieln=bn;
                    for(int i=0; i<wieln; i++) wiel[i]=b[i];

                    nmul(dwa, 1, wiel, wieln, &wiel_nast, &wiel_nastn);//wiel_nast to zmienna pomocnicza równa wiel*2;
                    while(!(wieksza(wiel_nast, wiel_nastn, a, an)))//Dopóki a>=wiel*2, czyli !(wiel_nast>a);
                    {
                        nmul(dwa, 1, wiel, wieln, &placeholder, &phn);//wiel*=2;
                        przepisz(&wiel, &wieln, placeholder, phn);
                        nmul(dwa, 1, wiel, wieln, &placeholder, &phn);//wiel_nast*=2;
                        przepisz(&wiel_nast, &wiel_nastn, placeholder, phn);
                        nadd(jeden, 1, potega, potn, &placeholder, &phn);//potega++;
                        przepisz(&potega, &potn, placeholder, phn);
                    }
                    nsub(a, an, wiel, wieln, &nowe_a, &nowe_an);
                    ndivmod(nowe_a, nowe_an, b, bn, &nowy_iloraz, &nowy_iln, &reszta, &ren);
                    nexp(dwa, 1, potega, potn, &placeholder, &phn);
                    przepisz(&potega, &potn, placeholder, phn);
                    nadd(potega, potn, nowy_iloraz, nowy_iln, &iloraz, &iln);
                    free(wiel);
                    free(wiel_nast);
                    if(nowy_iln)free(nowy_iloraz);
                    if(nowe_an)free(nowe_a);
                    if(potn)free(potega);
                }
            }
        }
    }
    *q=iloraz;
    *qn=iln;
    *r=reszta;
    *rn=ren;
    free(dwa);
    free(jeden);
    return;
}