# Tapatan

Programmcode für den Arduino Mega des Tapatan-Spiels

## Inhaltsverzeichnis
- [Über das Projekt](#über-das-projekt)
- [Ziele](#ziele)
- [Verwendung](#verwendung)
- [Mitwirkende](#mitwirkende)
- [Kontakt](#kontakt)

## Über das Projekt
Im Rahmen des Projekts im 5. Semester des trinationalen Studiengangs Mechatronik bestand unsere Aufgabe darin, ein bestehendes Projekt unserer Vorgänger zu analysieren und zu verbessern. Dabei haben wir die Spiele Tapatan und Tic-Tac-Toe mechatronisch umgesetzt.

## Ziele
Unser Ziel war es, den bestehenden Programmcode und die Mechanik grundlegend zu überarbeiten und zu optimieren. Die wichtigsten Änderungen und Erweiterungen umfassen:

### Software-Ziele:
- Implementierung des Spiels Tic-Tac-Toe als zusätzliche Spielvariante.
- Neuentwicklung des Algorithmus zur Zugberechnung des Computers für das Spiel Tapatan, um sowohl die Rechenzeit zu verkürzen als auch ein intelligenteres Verhalten zu erreichen.
- Integration eines Player-vs.-Player-Modus in beiden Spielvarianten.
- Einführung von drei Schwierigkeitsstufen für beide Spiele.
- Optimierung der Motorsteuerung zur Minimierung von Vibrationen.

### Mechanische Ziele:
- Überarbeitung des Gantry-Systems, um eine vibrationsfreie Bewegung zu gewährleisten und mechanische Überbestimmungen zu vermeiden.
- Neugestaltung des Gehäuses für ein ansprechenderes Design.
- Einbau von Stauraum zur Aufbewahrung der Spielfiguren und des Netzteilkabels.
- Verbesserung der Zugänglichkeit von Elektronik und Mikrocontroller-Anschlüssen zur Erleichterung von Wartung und Reparaturen.

### Elektronik-Ziele:
- Entwicklung einer Platine (PCB), um die Anzahl der Kabel zu reduzieren und die Übersichtlichkeit zu erhöhen.
- Integration der Verkabelung in die Mechanik, um eine saubere und geordnete Kabelführung zu gewährleisten.

## Verwendung / Anleitung

### Spielmodis
Folgende Spielmodis können ausgewählt werden:
![alt text](https://github.com/jfawer/Tapatan/blob/readme/Bilder/Spielmodis.png)

### Tic-Tac-Toe
Die Spieler setzen abwechselnd ihre Figuren auf das Spielfeld (Es wird jeweils angezeigt wer am Zug ist). <br>
Das Spiel endet, wenn ein Spieler drei Figuren in einer horizontalen, vertikalen oder diagonalen Reihe plaziert hat. Ebenso endet das Spiel wenn alle Felder belegt sind (Unentschieden). <br>
Hinweis: Das Spiel zeigt automatisch den Gewinner oder ein Unentschieden an

### Tapatan
Die Spieler setzen abwechselnd ihre Figuren auf das Spielfeld, bis beide Spieler jeweils drei Figuren platziert haben. (Es wird angezeigt, welcher Spieler am Zug ist.) <br>
Sobald alle Figuren gesetzt sind, können diese bewegt werden. Die Figuren dürfen horizontal, vertikal oder diagonal verschoben werden. Hinweis: In dieser Version können die Figuren in alle diagonalen Richtungen bewegt werden, was die Komplexität des Spiels erhöht (siehe Bild). <br>
Das Spiel endet, wenn ein Spieler drei Figuren in einer horizontalen, vertikalen oder diagonalen Reihe plaziert hat. <br>
Hinweis: Das Spiel zeigt automatisch den Gewinner oder ein Unentschieden an
![alt text](https://github.com/jfawer/Tapatan/blob/readme/Bilder/Verfahrwege.png)

## Mitwirkende

1. Eliane Hess  
2. Aymeric Nouvier  
3. Simeon Yasmin  
4. Liam Mahieu  
5. Jan Fawer  

## Kontakt

Für Fragen oder Anregungen zum Projekt können Sie sich gerne an die oben genannten Mitwirkenden wenden.

Trinationaler Studiengang Mechatronik (trinat.humboldt@outlook.com)

Projekt-Link: https://github.com/jfawer/Tapatan.git
