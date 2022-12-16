Gra polega na demonstracji komunikacji międzyprocesowej
oraz wielowątkowości.
Obsługuje ona do 4 graczy korzystając z kolejek FIFO. 
Działa tylko na systemie linux (tworzona na Ubuntu 20.04.2).

1.W celu zbudowania programów należy otworzyć terminal
	w folderze i wywołać polecenie "make"

2.Uruchomienie serwera odbywa sie za pomoca polecenia "./server"

3.Uruchomienie gracza odbywa sie w nowej instancji konsoli 
za pomoca polecenia "./player p(numer gracza)" na przykład: "./player p1"

4.Reszta funkcji odbywa się nastepująco:
strzałki - poruszanie
B/b - pojawienie bestii
c/t/T - pojawienie monety
Q/q - odłączenie gracza od serwera


5.Zeby zakończyć proces serwera należy zamknąć okno gry

6.Modyfikacja czasu trwania rundy odbywa sie za pomoca zmiennej ROUND_TIME
	w pliku source.h

Good luck!

Example of successful session:
![Example](https://github.com/WilhelmOsada/Multithread_game/blob/main/Multithread_Game.png?raw=true)
