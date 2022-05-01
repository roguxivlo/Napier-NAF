# Napier-NAF
Moduł do operacji arytmetycznych na dużych liczbach w zapisie Napier-NAF. Implementacja powstała jako projekt na Wstępie do Programowania
na wydziale MIM UW w semestrze 2021Z.

## Napier-NAF
Liczby na których pracuje moduł, są zapisane w postaci [BRR-NAF](https://en.wikipedia.org/wiki/Non-adjacent_form), przy pomocy
addytywnego systemu zapisu liczb opartego na systemie [Location arithmetic](https://en.wikipedia.org/wiki/Location_arithmetic).
Reprezentacją cyfry o wartości `2 ** n` jest nieujemna liczba całkowita `n`. Reprezentacją cyfry o wartości `-(2 ** n)` jest
ujemna liczba całkowita `-n - 1`.

## Zawartość modułu
Moduł zawiera funkcje implementujące następujące operacje arytmetyczne:
- Konwersja z typu `int` do zapisu Napier-NAF, konwersja z zapisu Napier-NAF do typu `int`
- Dodawanie, odejmowanie
- Mnożenie
- Potęgowanie
- Dzielenie z resztą
