# Tapatan

## Inhaltsverzeichnis
- [Über das Projekt](#über-das-projekt)
- [Ziele](#ziele)
- [Verwendung / Anleitung](#verwendung)
- [Technische Umsetzung](#technische-umsetzung)
- [Mitwirkende](#mitwirkende)
- [Kontakt](#kontakt)

<a name="über-das-projekt"/>

## Über das Projekt
Im Rahmen des Projekts im 5. Semester des trinationalen Studiengangs Mechatronik bestand unsere Aufgabe darin, ein bestehendes Projekt unserer Vorgänger zu analysieren und zu verbessern. Dabei haben wir die Spiele Tapatan und Tic-Tac-Toe mechatronisch umgesetzt.

<a name="ziele"/>

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

<a name="verwendung"/>

## Verwendung / Anleitung

### Spielvorbereitung
1. Stelle die Spielfiguren auf ihre Plätze
2. Ziehe die Schublade mit den Spielfiguren heraus um an das Rote Teil zu kommen (siehe Abbildung 1 / Abbildung 2)
3. Spanne den Riemen in dem du am Roten Teil drehst (siehe Abbildung 1/Abbildung 2) <br>
   Hinweis: Entspanne nach dem Spielen den Riemen wieder
5. Stecke das Netzteilkabel ein
6. Schalte den Schalter auf der Rückseite des Spielfeldes auf ein

<figure>
   <img src="https://github.com/jfawer/Tapatan/blob/readme/Bilder/Riemen-Spannen.png" width="600" />
   <figcaption><p><i>Abbildung 1: Riemenspannen Mechanismus</i></p></figcaption>
</figure> <br>

<figure>
   <img src="https://github.com/jfawer/Tapatan/blob/readme/Bilder/Riemen-Spannen2.png" width="600" />
   <figcaption><p><i>Abbildung 2: Riemenspannen Mechanismus</i></p></figcaption>
</figure>

### Spielmodis
Folgende Spielmodis können ausgewählt werden:
<figure>
   <img src="https://github.com/jfawer/Tapatan/blob/readme/Bilder/Spielmodis.png" width="800" />
   <figurecaption><p><i>Abbildung 3: Spielmodis</i></p></figurecaption>
</figure>


### Tic-Tac-Toe
Die Spieler setzen abwechselnd ihre Figuren auf das Spielfeld (Es wird jeweils angezeigt wer am Zug ist). <br>
Das Spiel endet, wenn ein Spieler drei Figuren in einer horizontalen, vertikalen oder diagonalen Reihe plaziert hat. Ebenso endet das Spiel wenn alle Felder belegt sind (Unentschieden). <br>
Hinweis: Das Spiel zeigt automatisch den Gewinner oder ein Unentschieden an

### Tapatan
Die Spieler setzen abwechselnd ihre Figuren auf das Spielfeld, bis beide Spieler jeweils drei Figuren platziert haben. (Es wird angezeigt, welcher Spieler am Zug ist.) <br>
Sobald alle Figuren gesetzt sind, können diese bewegt werden. Die Figuren dürfen horizontal, vertikal oder diagonal verschoben werden. Hinweis: In dieser Version können die Figuren in alle diagonalen Richtungen bewegt werden, was die Komplexität des Spiels erhöht (siehe Bild 4). <br>
Das Spiel endet, wenn ein Spieler drei Figuren in einer horizontalen, vertikalen oder diagonalen Reihe plaziert hat. <br>
Hinweis: Das Spiel zeigt automatisch den Gewinner oder ein Unentschieden an
<figure>
   <img src="https://github.com/jfawer/Tapatan/blob/readme/Bilder/Verfahrwege.png" width="150" />
   <figurecaption><p><i>Abbildung 4: Spielfeld Tapatan</i></p></figurecaption>
</figure>

<a name="technische-umsetzung"/>

## Technische Umsetzung
Im flogenden Abschnitt werden die Mechanik, Elektronik, und der Programmcode beschrieben

### Mechanik


### Elektronik

### Informatik

## Mitwirkende

1. Eliane Hess  
2. Aymeric Nouvier  
3. Simeon Yasmin  
4. Liam Mahieu  
5. Jan Fawer  

<a name="kontakt"/>

## Kontakt

Für Fragen oder Anregungen zum Projekt können Sie sich gerne an die oben genannten Mitwirkenden wenden.

Trinationaler Studiengang Mechatronik (trinat.humboldt@outlook.com)

Projekt-Link: https://github.com/jfawer/Tapatan.git
