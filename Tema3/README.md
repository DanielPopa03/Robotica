Coechipier: Calin Cristian

Descriere task: Implementarea unui joc de apasat cat mai rapid butonul corespunzator.

S-au folosit doua placi arduino care comunica prin protocolul SPI, una fiind master, iar una slave. Placa master este responsabila de controlarea LCD-ului si a game flow-ului in general, precum si a timer-ului reprezentat printr-un servomotor. Placa slave este responsabila de inputuri si de visual feedback (leduri).

Scopul jocul este sa apesi mai repede butonul corespunzator culorii care apare pe led-ul rgb destinat tie decat adversarul tau. Cine apasa primul va primi 1 punct.
Placa master controleaza inceperea / sfarsitul / game state-ul jocului, transmitand in fiecare runda ce culoare trebuie sa apese player-ul. Masterul asteapta ca runda sa se termine(in cea ce priveste timpul alocat) apoi trimite din nou culorile pentru playeri.
Atunci cand trimite culorile colecteaza si rezultatul rundei anterioare, in cazul primei trimiteri va primi de la slave "-" ce simbolizeaza "niciun punct pt nici un player", iar in ultima runda va trimite si un caracter "l" pt a anunta slave-ul de sfarsitul transmisiuni.
Placa slave primeste prin SPI culoarile si le reda led-ului rgb corespunzator player-ului curent si asteapta input. Punctajul se transmite master-ului sub forma unui singur caracter 1 sau 2(ce corespund cu numarul playerului) iar master aloca punctul jucatorului atunci cand primeste mesajul.

La final placa master afiseaza  cine a castigat, iar in caz de remiza "Draw".

Componente:
2 placi arduino
2 player x 3 LED (r, g, b) = 6 LEDuri colorate. 2 player x 1 LEDRGB programabil = 2x RGBLED
2 player x 3 butoane corespunzatoare culorilor r,g,b
1 x buton de start (un joystick, nu mai aveam butoane)
1 x servomotor
1 x LCD
1 x Potentiometru 10K
1 x Buzzer
Rezistente de 220Ohm, 33Ohm si 1K, pentru butoane Leduri si Background-ul LCD-ului.
Fire de legatura
![WhatsApp Image 2024-11-20 at 22 22 50_7c313de0](https://github.com/user-attachments/assets/a8fd45b7-e23c-4c48-a4ff-1487973797ae)
![WhatsApp Image 2024-11-20 at 22 22 50_2aaf0357](https://github.com/user-attachments/assets/0422780c-7d53-40f5-a332-1a573759f91e)
![WhatsApp Image 2024-11-20 at 22 22 51_ee9f20bb](https://github.com/user-attachments/assets/fba395b4-c1b0-4af6-add3-e1290edc84aa)
![WhatsApp Image 2024-11-20 at 22 53 19_ccd11597](https://github.com/user-attachments/assets/614bdf12-92a5-48ea-8710-5d1b10c68e7f)


https://github.com/user-attachments/assets/c1fc5365-c65c-4a0c-916b-1bfd6e01e261



