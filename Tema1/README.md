1.1 Descriere
În această temă trebuie să simulați o stație de încărcare pentru un vehicul electric, folosind mai multe LED-uri și butoane. În cadrul acestui task trebuie să țineți cont de stările butonului și să folosiți debouncing, dar și să coordonați toate componentele ca într-un scenariu din viața reală.

1.2 Componente
4x LED-uri (pentru a simula procentul de încărcare)
1x LED RGB (pentru starea de liber sau ocupat)
2x Butoane (pentru start încărcare și stop încărcare)
8x Rezistoare (6x 220/330ohm, 2x 1K)
Breadboard
Linii de legătură

1.3 Detalii tehnice
Led-ul RGB reprezintă disponibilitatea stației. Dacă stația este liberă led-ul va fi verde, iar dacă stația este ocupată se va face roșu.

Led-urile simple reprezintă gradul de încărcare al bateriei, pe care îl vom simula printr-un loader progresiv (L1 = 25%, L2 = 50%, L3 = 75%, L4 = 100%). Loader-ul se încărca prin aprinderea succesivă a led-urilor, la un interval fix de 3s. LED-ul care semnifică procentul curent de încărcare va avea starea de clipire, LED-urile din urma lui fiind aprinse continuu, iar celelalte stinse.

Apăsarea scurtă a butonului de start va porni încărcarea. Apăsarea acestui buton în timpul încărcării nu va face nimic.

Apăsarea lungă a butonului de stop va opri încărcarea forțat și va reseta stația la starea liberă. Apăsarea acestui buton cat timp stația este liberă nu va face nimic.

1.3 Detalii tehnice
Led-ul RGB reprezintă disponibilitatea stației. Dacă stația este liberă led-ul va fi verde, iar dacă stația este ocupată se va face roșu.

Led-urile simple reprezintă gradul de încărcare al bateriei, pe care îl vom simula printr-un loader progresiv (L1 = 25%, L2 = 50%, L3 = 75%, L4 = 100%). Loader-ul se încărca prin aprinderea succesivă a led-urilor, la un interval fix de 3s. LED-ul care semnifică procentul curent de încărcare va avea starea de clipire, LED-urile din urma lui fiind aprinse continuu, iar celelalte stinse.

Apăsarea scurtă a butonului de start va porni încărcarea. Apăsarea acestui buton în timpul încărcării nu va face nimic.

Apăsarea lungă a butonului de stop va opri încărcarea forțat și va reseta stația la starea liberă. Apăsarea acestui buton cat timp stația este liberă nu va face nimic.
![image](https://github.com/user-attachments/assets/f26a7a0d-467e-4cbf-86d9-8d3b0e6527f8)

Video cu funcționalitatea temei
https://github.com/user-attachments/assets/79f4e663-38ce-4a34-967e-5cc08cb090e5

![da1](https://github.com/user-attachments/assets/c40f6683-9c41-4e95-a87a-7b0ec1ca60ab)


![da2](https://github.com/user-attachments/assets/946b430b-7e97-4513-a66a-09bbf44b0440)

![da3](https://github.com/user-attachments/assets/67548c1f-4495-414b-b26b-259e10ecb3c7)

![da4](https://github.com/user-attachments/assets/2b468c45-f1f1-4c71-b951-8c871a23b111)
