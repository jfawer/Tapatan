# Tapatan

## Inhaltsverzeichnis
- [Über das Projekt](#über-das-projekt)
- [Ziele](#ziele)
- [Verwendung / Anleitung](#verwendung-anleitung)
- [Technische Umsetzung](#technische-umsetzung)
- [Mitwirkende](#mitwirkende)
- [Kontakt](#kontakt)

<a name="über-das-projekt"/>

## Über das Projekt
Im Rahmen eines Projekts im 5. Semester des trinationalen Studiengangs Mechatronik bestand unsere Aufgabe darin, ein bestehendes Projekt unserer Vorgänger zu analysieren und zu verbessern. Dabei haben wir die Spiele Tapatan und Tic-Tac-Toe mechatronisch umgesetzt.

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

<a name="verwendung-anleitung"/>

## Verwendung / Anleitung

### Spielvorbereitung
1. Stelle die Spielfiguren auf ihre Plätze.
2. Ziehe die Schublade mit den Spielfiguren heraus um an das rote Teil zu gelangen (siehe Abbildung 1 / Abbildung 2).
3. Spanne den Riemen in dem du am roten Teil drehst (siehe Abbildung 1/Abbildung 2). <br>
   Hinweis: Entspanne nach dem Spielen den Riemen wieder, um die Lebensdauer des Mechanismus zu verlängern und Schäden durch anhaltende Spannung zu vermeiden.
5. Stecke das Netzteilkabel ein.
6. Schalte den Schalter auf der Rückseite des Spieles ein.

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
Die Spieler setzen abwechselnd ihre Figuren auf das Spielfeld (es wird jeweils angezeigt, wer am Zug ist). <br>
Das Spiel endet, wenn ein Spieler drei Figuren in einer horizontalen, vertikalen oder diagonalen Reihe platziert hat. Ebenso endet das Spiel, wenn alle Felder belegt sind (Unentschieden). <br>
Hinweis: Das Spiel zeigt automatisch den Gewinner oder ein Unentschieden an. <br>

### Tapatan
Die Spieler setzen abwechselnd ihre Figuren auf das Spielfeld, bis beide Spieler jeweils drei Figuren platziert haben (es wird angezeigt, welcher Spieler am Zug ist). <br>
Sobald alle Figuren gesetzt sind, können diese bewegt werden. Die Figuren dürfen horizontal, vertikal oder diagonal verschoben werden. Hinweis: In dieser Version können die Figuren in alle diagonalen Richtungen bewegt werden, was die Komplexität des Spiels erhöht (siehe Abbildung 4). <br>
Das Spiel endet, wenn ein Spieler drei Figuren in einer horizontalen, vertikalen oder diagonalen Reihe platziert hat. <br>
Hinweis: Das Spiel zeigt automatisch den Gewinner oder ein Unentschieden an.<br>
<figure>
   <img src="https://github.com/jfawer/Tapatan/blob/readme/Bilder/Verfahrwege.png" width="150" />
   <figurecaption><p><i>Abbildung 4: Spielfeld Tapatan</i></p></figurecaption>
</figure>

<a name="technische-umsetzung"/>

## Technische Umsetzung

### Funktionsweise

#### Erkennen der Spielfiguren
Um auszuwerten, welche Spielfiguren auf welchem Feld stehen, wurde in jede Spielfigur ein Magnet integriert. Die beiden unterschiedlichen Farben (Rot und Blau) besitzen jeweils entgegengesetzt ausgerichtete Magnete. Dadurch erzeugen die verschiedenen Spielsteine unterschiedliche Magnetfelder an ihrer Unterseite. Mithilfe bipolarer Hallsensoren können diese Magnetfelder erkannt werden. Die Hallsensoren sind direkt unter der Spielplatte angebracht, sodass für jedes Feld ermittelt werden kann, ob es leer ist oder ein blauer bzw. roter Spielstein darauf platziert wurde. <br>

Um zu verhindern, dass sich die Spielsteine gegenseitig anziehen oder abstoßen, wurde zusätzlich ein Stahlring um den Magneten in den Figuren integriert. Dieser beeinflusst das Magnetfeld so, dass es in radialer Richtung abgeschirmt wird und hauptsächlich in axialer Richtung wirkt.<br>

#### Verschieben der Spielfiguren
Die Spielfiguren werden mithilfe eines Elektromagneten bewegt, der sich unterhalb der Spielplatte befindet und durch das Gantry-System positioniert wird. Die Magnete in den Spielfiguren werden vom Elektromagneten angezogen, sodass die Figuren präzise und kontrolliert über das Spielfeld verschoben werden können.

<figure>
   <img src="https://github.com/jfawer/Tapatan/blob/readme/Bilder/Funktionsbeschreibung.png" width="800" />
   <figurecaption><p><i>Abbildung 7: Aufbau</i></p></figurecaption>
</figure>

#### Gantry
Das Gantry-System basiert auf dem Prinzip eines H-Gantry (siehe Abbildung 6). Bewegen sich die beiden Motoren in die gleiche Richtung, verschiebt sich der Wagen, auf dem sich der Elektromagnet befindet, entlang der Y-Achse. Drehen sich die Motoren hingegen in entgegengesetzte Richtungen, bewegt sich der Wagen entlang der X-Achse. Dieses System ermöglicht eine präzise und stabile Positionierung der Spielfiguren.
<figure>
   <img src="https://www.galil.com/sites/default/files/pictures/H-Bot_diagram_600x339.jpg" width="600" />
   <figurecaption><p><i>Abbildung 6: H-Gantry, Quelle: https://www.galil.com/news/whats-new-galil/white-paper-introducing-galils-new-h-bot-firmware </i></p></figurecaption>
</figure>

<figure>
   <img src="https://github.com/jfawer/Tapatan/blob/readme/Bilder/Gantry.jpg" width="600" />
   <figurecaption><p><i>Abbildung 7: H-Gantry Tapatan</i></p></figurecaption>
</figure>

### Elektronik
#### Beschreibung des Schaltplans
ERGÄNZEN!!!!!!!!!!!!!

#### Farbcode Verkabelung
| Farbe | Signal |
| ----- | ------ |
| Rot | 5V |
| Schwarz | Ground |
| Grün |  |

ERGÄNZEN!!!!!

### Programmcode
Beschreibung der Einzelnen h-Files, Funktion von main und motorcontrol

### Stückliste
Gesamtbaugruppe:
| Teileart  | Teilename       | Herstellung | Material | Anzahl |
| :-------  | :---------      | :---------- | -------: | -----: |
| Baugruppe | HGantry         |             |          | 1      |
| Baugruppe | ElectronicPlate |             |          | 1      |
| Baugruppe | Steinehalterung |             |          | 1      |
| Baugruppe | Gehäuse         |             |          | 1      |

HGantry:
| Teileart  | Teilename        | Herstellung | Material | Anzahl |
| :-------  | :---------       | :---------- | -------: | -----: |
| Mechanik  | Schiene_lang     | Einkaufen   |          | 1      |
| Mechanik  | Wagen_Gross      | Einkaufen   |          | 1      |
| Mechanik  | Schiene          | Einkaufen   |          | 1      |
| Mechanik  | Alu-Profil_20x20 | Einkaufen   | l=600    | 1      |
| Baugruppe | Umlenkrolle      |             |          | 2      |
| Baugruppe | Umlenturm        |             |          | 2      |
| Mechanik  | Standoff_M3x13   | Einkaufen   |          | 2      |
| Baugruppe | StepperAssm      |             |          | 1      |
| Baugruppe | EndStop_500      |             |          | 1      |
| Mechanik  | Tail             | 3D-Druck    | PLA      | 1      |
| Mechanik  | Verbin_Step      | 3D-Druck    | PLA      | 1      |
| Mechanik  | RailAllign       | 3D-Druck    | PLA      | 1      |
| Baugruppe | Spanneinheit     |             |          | 1      |
| Mechanik  | Ecke_Profil      |             |          | 1      |
| Baugruppe | EndStop_400      |             |          | 1      |
| Baugruppe | MagnetAssm       |             |          | 1      |
| Baugruppe | Chain            |             |          | 1      |
| Baugruppe | Chain2           |             |          | 1      |

HGantry-(StepperAssm)-Umlenkrolle :
| Teileart  | Teilename             | Herstellung | Material | Anzahl |
| :-------  | :---------            | :---------- | -------: | -----: |
| Mechanik  | Kugellager_Da9xDi3xh5 | Einkaufen   |          | 1      |
| Mechanik  | Rolle                 | 3D-Druck    |          | 1      |
| Mechanik  | Unterlegscheibe       | 3D-Druck    |          | 1      |
| Mechanik  | Zylschr. M3x12        | Einkaufen   |          | 1      |

HGantry-Umlenkturm:
| Teileart  | Teilename             | Herstellung | Material | Anzahl |
| :-------  | :---------            | :---------- | -------: | -----: |
| Mechanik  | Kugellager_Da9xDi3xh5 | Einkaufen   |          | 2      |
| Mechanik  | Rolle                 | 3D-Druck    |          | 2      |
| Mechanik  | Unterlegscheibe       | 3D-Druck    |          | 1      |
| Mechanik  | Zylschr. M3x14        | Einkaufen   |          | 1      |
| Mechanik  | Distanzzylinder       | 3D-Druck    |          | 1      |

HGantry-StepperAssm:
| Teileart   | Teilename         | Herstellung | Material | Anzahl |
| :-------   | :---------        | :---------- | -------: | -----: |
| Mechanik   | StH_Left          | 3D-Druck    | PLA      | 1      |
| Mechanik   | StH_Right         | 3D-Druck    | PLA      | 1      |
| Elektronik | NEMA17_Motor      | Einkaufen   |          | 2      |
| Baugruppe  | Umlenkrolle       |             |          | 4      |
| Mechanik   | Riemenzahnrad_GT2 | Einkaufen   |          | 2      |

HGantry-EndStop_500:
| Teileart   | Teilename   | Herstellung | Material | Anzahl |
| :-------   | :---------  | :---------- | -------: | -----: |
| Elektronik | microswitch | Einkaufen   |          | 1      |
| Mechanik   | EndH_500    | 3D-Druck    | PLA      | 1      |

HGantry-Spanneinheit:
| Teileart   | Teilename       | Herstellung | Material | Anzahl |
| :-------   | :---------      | :---------- | -------: | -----: |
| Mechanik   | Spannhalterung  | 3D-Druck    | PLA      | 1      |
| Baugruppe  | Winkelrolle     |             |          | 2      |
| Mechanik   | Tensioner       | 3D-Druck    | PLA      | 1      |
| Mechanik   | 6kt-Schr. M4x40 | Einkaufen   |          | 1      |
| Mechanik   | 6kt-Mutter. M4  | Einkaufen   |          | 1      |
| Mechanik   | Knob            | 3D-Druck    | PLA      | 1      |

HGantry-Spanneinheit-Winkelrolle:
| Teileart  | Teilename             | Herstellung | Material | Anzahl |
| :-------  | :---------            | :---------- | -------: | -----: |
| Mechanik  | Kugellager_Da9xDi3xh5 | Einkaufen   |          | 1      |
| Mechanik  | Rolle                 | 3D-Druck    |          | 1      |
| Mechanik  | Unterlegscheibe       | 3D-Druck    |          | 1      |
| Mechanik  | Zylschr. M3x12        | Einkaufen   |          | 1      |

HGantry-EndStop_400:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Mechanik   | Alu-Profil_20x20 | Einkaufen   | l=200    | 1      |
| Elektronik | microswitch      | Einkaufen   |          | 1      |
| Mechanik   | StopHalt_400     | 3D-Druck    | PLA      | 1      |
| Mechanik   | Stuetze          | 3D-Druck    | PLA      | 1      |
| Mechanik   | Stuetz_Con       | 3D-Druck    | PLA      | 1      |

HGantry-MagnetAssm:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Mechanik   | MagnetHalter     | 3D-Druck    | PLA      | 1      |
| Mechanik   | 6kt-Schr. M3x10  |             |          | 2      |
| Elektronik | Elektromagnet    | Einkaufen   |          | 1      |

HGantry-Chain:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Mechanik   | ChainStart       | 3D-Druck    | PLA      | 1      |
| Mechanik   | Link             | 3D-Druck    | PLA      | 21     |
| Mechanik   | ChainEnd         | 3D-Druck    | PLA      | 1      |

HGantry-Chain2:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Mechanik   | Link Start       | 3D-Druck    | PLA      | 1      |
| Mechanik   | Link             | 3D-Druck    | PLA      | 35     |
| Mechanik   | Link End         | 3D-Druck    | PLA      | 1      |

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
