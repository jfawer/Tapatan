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



Tapatan:
| Teileart  | Teilename                           | Herstellung | Material | Anzahl |
| :-------  | :---------                          | :---------- | -------: | -----: |
| Baugruppe | [HGantry](#hgantry)                 |             |          | 1      |
| Baugruppe | [ElectronicPlate](#electronicplate) |             |          | 1      |
| Baugruppe | [Steineschublade](#steineschublade) |             |          | 1      |
| Baugruppe | [Gehäuse](#gehäuse)                 |             |          | 1      |

<a name="hgantry"/>

[Tapatan](#tapatanstk)-HGantry:
| Teileart  | Teilename                         | Herstellung | Material | Anzahl |
| :-------  | :---------                        | :---------- | -------: | -----: |
| Mechanik  | Schiene_lang                      | Einkaufen   |          | 1      |
| Mechanik  | Wagen_Gross                       | Einkaufen   |          | 1      |
| Mechanik  | Schiene                           | Einkaufen   |          | 1      |
| Mechanik  | Alu-Profil_20x20                  | Einkaufen   | l=600    | 1      |
| Baugruppe | [Umlenkrolle](#umlenkrolle)       |             |          | 2      |
| Baugruppe | [Umlenkturm](#umlenkturm)         |             |          | 2      |
| Mechanik  | Standoff_M3x13                    | Einkaufen   |          | 2      |
| Baugruppe | [StepperAssm](#stepperassm)       |             |          | 1      |
| Baugruppe | [EndStop_500](#endstop500)        |             |          | 1      |
| Mechanik  | Tail                              | 3D-Druck    | PLA      | 1      |
| Mechanik  | Verbin_Step                       | 3D-Druck    | PLA      | 1      |
| Mechanik  | RailAllign                        | 3D-Druck    | PLA      | 1      |
| Baugruppe | [Spanneinheit](#spanneinheit)     |             |          | 1      |
| Mechanik  | Ecke_Profil                       |             |          | 1      |
| Baugruppe | [EndStop_400](#endstop400)        |             |          | 1      |
| Baugruppe | [MagnetAssm](#magnetassm)         |             |          | 1      |
| Baugruppe | [Chain](#chain)                   |             |          | 1      |
| Baugruppe | [Chain2](#chain2)                 |             |          | 1      |
| Mechanik  | Zyl.-Schr. M3x10                  | Einkaufen   |          | 4      |
| Mechanik  | Zyl.-Schr. M3x8                   | Einkaufen   |          | 2      |
| Mechanik  | Nutenstein M3                     | Einkaufen   |          | 4      |


<a name="umlenkrolle"/>

[HGantry](#hgantry)-([StepperAssm](#stepperassm))-Umlenkrolle :
| Teileart  | Teilename             | Herstellung | Material | Anzahl |
| :-------  | :---------            | :---------- | -------: | -----: |
| Mechanik  | Kugellager_Da9xDi3xh5 | Einkaufen   |          | 1      |
| Mechanik  | Rolle                 | 3D-Druck    | d=14     | 1      |
| Mechanik  | Unterlegscheibe       | 3D-Druck    |          | 1      |
| Mechanik  | Zylschr. M3x12        | Einkaufen   |          | 1      |

<a name="umlenkturm"/>

[HGantry](#hgantry)-Umlenkturm:
| Teileart  | Teilename             | Herstellung | Material | Anzahl |
| :-------  | :---------            | :---------- | -------: | -----: |
| Mechanik  | Kugellager_Da9xDi3xh5 | Einkaufen   |          | 2      |
| Mechanik  | Rolle                 | 3D-Druck    | d=14     | 2      |
| Mechanik  | Unterlegscheibe       | 3D-Druck    |          | 1      |
| Mechanik  | Zylschr. M3x14        | Einkaufen   |          | 1      |
| Mechanik  | Distanzzylinder       | 3D-Druck    |          | 1      |

<a name="stepperassm"/>

[HGantry](#hgantry)-StepperAssm:
| Teileart   | Teilename                   | Herstellung | Material | Anzahl |
| :-------   | :---------                  | :---------- | -------: | -----: |
| Mechanik   | StH_Left                    | 3D-Druck    | PLA      | 1      |
| Mechanik   | StH_Right                   | 3D-Druck    | PLA      | 1      |
| Elektronik | NEMA17_Motor                | Einkaufen   |          | 2      |
| Baugruppe  | [Umlenkrolle](#umlenkrolle) |             |          | 4      |
| Mechanik   | Riemenzahnrad_GT2           | Einkaufen   |          | 2      |
| Mechanik   | Zyl.-Schr. M5x8             | Einkaufen   |          | 4      |
| Mechanik   | Nutenstein M5               | Einkaufen   |          | 4      |
| Mechanik   | Zyl.-Schr. M3x8             | Einkaufen   |          | 8      |

<a name="endstop500"/>

[HGantry](#hgantry)-EndStop_500:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Elektronik | microswitch      | Einkaufen   |          | 1      |
| Mechanik   | EndH_500         | 3D-Druck    | PLA      | 1      |
| Mechanik   | Zyl.-Schr. M5x8  | Einkaufen   |          | 2      |
| Mechanik   | Nutenstein M5    | Einkaufen   |          | 2      |
| Mechanik   | Inserts M3       | Einkaufen   |          | 2      |
| Mechanik   | Zyl.-Schr. M3x10 | Einkaufen   |          | 2      |

<a name="spanneinheit"/>

[HGantry](#hgantry)-Spanneinheit:
| Teileart   | Teilename                   | Herstellung | Material | Anzahl |
| :-------   | :---------                  | :---------- | -------: | -----: |
| Mechanik   | Spannhalterung              | 3D-Druck    | PLA      | 1      |
| Baugruppe  | [Winkelrolle](#winkelrolle) |             |          | 2      |
| Mechanik   | Tensioner                   | 3D-Druck    | PLA      | 1      |
| Mechanik   | 6kt-Schr. M4x40             | Einkaufen   |          | 1      |
| Mechanik   | 6kt-Mutter. M4              | Einkaufen   |          | 1      |
| Mechanik   | Knob                        | 3D-Druck    | PLA      | 1      |
| Mechanik   | Zyl.-Schr. M5x8             | Einkaufen   |          | 2      |
| Mechanik   | Nutenstein M5               | Einkaufen   |          | 2      |

<a name="winkelrolle"/>

[HGantry](#hgantry)-[Spanneinheit](#spanneinheit)-Winkelrolle:
| Teileart  | Teilename             | Herstellung | Material | Anzahl |
| :-------  | :---------            | :---------- | -------: | -----: |
| Mechanik  | Kugellager_Da9xDi3xh5 | Einkaufen   |          | 1      |
| Mechanik  | Rolle                 | 3D-Druck    | d=17.2   | 1      |
| Mechanik  | Unterlegscheibe       | 3D-Druck    |          | 1      |
| Mechanik  | Zylschr. M3x12        | Einkaufen   |          | 1      |

<a name="endstop400"/>

[HGantry](#hgantry)-EndStop_400:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Mechanik   | Alu-Profil_20x20 | Einkaufen   | l=200    | 1      |
| Elektronik | microswitch      | Einkaufen   |          | 1      |
| Mechanik   | StopHalt_400     | 3D-Druck    | PLA      | 1      |
| Mechanik   | Stuetze          | 3D-Druck    | PLA      | 1      |
| Mechanik   | Stuetz_Con       | 3D-Druck    | PLA      | 1      |
| Mechanik   | Zyl.-Schr. M5x8  | Einkaufen   |          | 2      |
| Mechanik   | Nutenstein M5    | Einkaufen   |          | 2      |
| Mechanik   | Inserts M3       | Einkaufen   |          | 2      |
| Mechanik   | Zyl.-Schr. M3x10 | Einkaufen   |          | 2      |

<a name="magnetassm"/>

[HGantry](#hgantry)-MagnetAssm:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Mechanik   | MagnetHalter     | 3D-Druck    | PLA      | 1      |
| Mechanik   | 6kt-Schr. M3x10  |             |          | 2      |
| Elektronik | Elektromagnet    | Einkaufen   |          | 1      |

<a name="chain"/>

[HGantry](#hgantry)-Chain:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Mechanik   | ChainStart       | 3D-Druck    | PLA      | 1      |
| Mechanik   | Link             | 3D-Druck    | PLA      | 21     |
| Mechanik   | ChainEnd         | 3D-Druck    | PLA      | 1      |

<a name="chain2"/>

[HGantry](#hgantry)-Chain2:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Mechanik   | Link Start       | 3D-Druck    | PLA      | 1      |
| Mechanik   | Link             | 3D-Druck    | PLA      | 35     |
| Mechanik   | Link End         | 3D-Druck    | PLA      | 1      |

<a name="electronicplate"/>

[Tapatan](#tapatanstk)-ElectronicPlate
| Teileart   | Teilename                        | Herstellung    | Material | Anzahl |
| :-------   | :---------                       | :----------    | -------: | -----: |
| Mechanik   | Electronic_Floor                 | Laserschneiden | Holz     | 1      |
| Mechanik   | Backplate1                       | Laserschneiden | Holz     | 1      |
| Mechanik   | Winkel                           | 3D-Druck       | PLA      | 2      |
| Mechanik   | Schubladenstopper                | 3D-Druck       | PLA      | 1      |
| Elektronik | Arduino Mega                     | Einkaufen      |          | 1      |
| Mechanik   | StandoffM3x5                     | Einkaufen      |          | 6      |
| Elektronik | TB6600 - Motor Driver            | Einkaufen      |          | 2      |
| Mechanik   | Linsenkopfschr. M3x10            | Einkaufen      |          | 3      |
| Mechanik   | Linsenkopfschr. M3x16            | Einkaufen      |          | 2      |
| Mechanik   | Senkschr. M3x16                  | Einkaufen      |          | 2      |
| Mechanik   | Mutter M3                        | Einkaufen      |          | 4      |
| Elektronik | Schalter                         | Einkaufen      |          | 1      |
| Elektronik | [Arduino_Shield](#arduinoshield) | Einkaufen      |          | 1      |
| Mechanik   | Inserts M3                       | Einkaufen      |          | 2      |
| Elektronik | DC-DC-Converter 12V/9V           | Einkaufen      |          | 1      |
| Elektronik | DC-DC-Converter 12V/5V           | Einkaufen      |          | 1      |

<a name="arduinoshield"/>

[ElectronicPlate](#electronicplate)-Arduino_Shield
| Teileart   | Teilename              | Herstellung    | Material | Anzahl |
| :-------   | :---------             | :----------    | -------: | -----: |
| Elektronik | Leiterplatte           | Einkaufen      |          | 1      |
| Elektronik | Widerstand 680Ohm      | Einkaufen      |          | 7      |
| Elektronik | LED_Red                | Einkaufen      |          | 4      |
| Elektronik | Diode 1N4007           | Einkaufen      |          | 3      |
| Elektronik | Widerstand 1kOhm       | Einkaufen      |          | 3      |
| Elektronik | NPN Transistor BC547   | Einkaufen      |          | 3      |
| Elektronik | Relay_SPDT             | Einkaufen      |          | 3      |
| Elektronik | Widerstand 10kOhm      | Einkaufen      |          | 3      |
| Elektronik | LED_Green              | Einkaufen      |          | 1      |
| Elektronik | LED_Blue               | Einkaufen      |          | 1      |
| Elektronik | Mosfet IRLZ24          | Einkaufen      |          | 3      |
| Elektronik | LED_Yellow             | Einkaufen      |          | 4      |


<a name="steineschublade"/>

[Tapatan](#tapatanstk)-Steineschublade
| Teileart | Teilename                 | Herstellung    | Material | Anzahl |
| :------- | :---------                | :----------    | -------: | -----: |
| Mechanik | Plate_Steinehalterung     | Laserschneiden | Holz     | 1      |
| Mechanik | Schubladenstopper         | 3D-Druck       | PLA      | 1      |
| Mechanik | Backplate_Steinehalterung | Laserschneiden | Holz     | 1      |
| Mechanik | Sideplate_L_Steinehalter  | Laserschneiden | Holz     | 1      |
| Mechanik | Sideplate_R_Steinehalter  | Laserschneiden | Holz     | 1      |
| Mechanik | Winkel                    | 3D-Druck       | PLA      | 4      |
| Mechanik | Front_Steinehalterung     | Laserschneiden | Holz     | 1      |
| Mechanik | Schliessmech.1            | 3D-Druck       | PLA      | 1      |
| Mechanik | Schliessmech.2            | 3D-Druck       | PLA      | 1      |
| Mechanik | Schliessmech.3            | 3D-Druck       | PLA      | 1      |
| Mechanik | Schliessmech.4            | 3D-Druck       | PLA      | 1      |
| Mechanik | Senkschr. M3x12           | Einkaufen      |          | 4      |
| Mechanik | Linsenkopfschr. M3x16     | Einkaufen      |          | 4      |
| Mechanik | Linsenkopfschr. M3x10     | Einkaufen      |          | 8      |
| Mechanik | Mutter M3                 | Einkaufen      |          | 8      |
| Mechanik | Mutter M5                 | Einkaufen      |          | 1      |
| Mechanik | Sechskantschr. M5x25      | Einkaufen      |          | 1      |
| Mechanik | Drehgriff                 | 3D-Druck       | PLA      | 1      |
| Mechanik | Schubladenteiler          | 3D-Druck       | PLA      | 1      |
| Mechanik | Inserts M3                | Einkaufen      |          | 6      |

<a name="gehäuse"/>

[Tapatan](#tapatanstk)-Gehäuse
| Teileart   | Teilename                              | Herstellung    | Material       | Anzahl |
| :-------   | :---------                             | :----------    | -------:       | -----: |
| Baugruppe  | [EckAssm](#eckassm)                    |                |                | 2      |
| Baugruppe  | [EckAssm BackRight](#eckassmbackright) |                |                | 1      |
| Baugruppe  | [EckAssm BackLeft](#eckassmbackleft)   |                |                | 1      |
| Baugruppe  | [KantenAbdeckung_L](#kantenabdeckungl) |                |                | 2      |
| Baugruppe  | [KantenAbdeckung_K](#kantenabdeckungk) |                |                | 2      |
| Mechanik   | SteinHalter                            | 3D-Druck       | PLA            | 2      |
| Mechanik   | Backpanel                              |                | PVC-Hartschaum | 1      |
| Mechanik   | Frontpanel                             |                | Acrylglas      | 1      |
| Mechanik   | Spielfeld                              |                | Acrylglas      | 1      |
| Baugruppe  | [BotPart_Tap](#botparttap)             |                |                | 1      |
| Baugruppe  | [PCB-Deckel](#pcbdeckel)               |                |                | 1      |
| Baugruppe  | [LCDAssm](#lcdassm)                    |                |                | 1      |
| Mechanik   | SidePanel                              |                | Acrylglas      | 4      |
| Mechanik   | Abdeckplatte                           |                | PVC-Hartschaum | 1      |
| Mechanik   | Drehknopf                              | 3D-Druck       | PLA            | 1      |
| Mechanik   | Sensorplatte                           | 3D-Druck       | PLA            | 1      |
| Baugruppe  | [ChainBig](#chainbig)                  | 3D-Druck       | PLA            | 1      |
| Elektronik | Hallsensor  DRV5055A2QLPG              | Einkaufen      |                | 1      |
| Elektronik | Rotary_Switch                          | Einkaufen      |                | 1      |
| Elektronik | LED_Streifen                           | Einkaufen      |                | 1.2m   |
| Mechanik   | Chip                                   | 3D-Druck       | PLA            | 10     |

<a name="eckassm"/>

[Gehäuse](#gehäuse)-EckAssm
| Teileart   | Teilename                              | Herstellung    | Material       | Anzahl |
| :-------   | :---------                             | :----------    | -------:       | -----: |
| Mechanik   | Säule                                  | 3D-Druck       | PLA            | 1      |
| Baugruppe  | [DreieckAssm](#dreieckassm)            |                |                | 1      |
| Mechanik   | Verbinder 1                            | 3D-Druck       | PLA            | 1      |
| Mechanik   | Verbinder 2                            | 3D-Druck       | PLA            | 2      |
| Mechanik   | Gummifuss                              | Einkaufen      |                | 1      |
| Mechanik   | Verbinder 3                            | 3D-Druck       | PLA            | 2      |

<a name="eckassmbackright"/>

[Gehäuse](#gehäuse)-EckAssm BackRight
| Teileart   | Teilename                              | Herstellung    | Material       | Anzahl |
| :-------   | :---------                             | :----------    | -------:       | -----: |
| Mechanik   | Säule                                  | 3D-Druck       | PLA            | 1      |
| Baugruppe  | [DreieckAssm](#dreieckassm)            |                |                | 1      |
| Mechanik   | Verbinder 1                            | 3D-Druck       | PLA            | 1      |
| Mechanik   | Verbinder 2                            | 3D-Druck       | PLA            | 2      |
| Mechanik   | Gummifuss                              | Einkaufen      |                | 1      |
| Mechanik   | Verbinder 3                            | 3D-Druck       | PLA            | 2      |

<a name="eckassmbackleft"/>

[Gehäuse](#gehäuse)-EckAssm BackLeft
| Teileart   | Teilename                              | Herstellung    | Material       | Anzahl |
| :-------   | :---------                             | :----------    | -------:       | -----: |
| Mechanik   | Säule                                  | 3D-Druck       | PLA            | 1      |
| Baugruppe  | [DreieckAssm](#dreieckassm)            |                |                | 1      |
| Mechanik   | Verbinder 1                            | 3D-Druck       | PLA            | 1      |
| Mechanik   | Verbinder 2                            | 3D-Druck       | PLA            | 2      |
| Mechanik   | Gummifuss                              | Einkaufen      |                | 1      |
| Mechanik   | Verbinder 3                            | 3D-Druck       | PLA            | 2      |

<a name="dreieckassm"/>

[Gehäuse](#gehäuse)-[EckAssm](#eckassm)/[EckAssm BackRight](#eckassmbackright)/[EckAssm BackLeft](#eckassmbackleft)-DreieckAssm
| Teileart   | Teilename                              | Herstellung    | Material       | Anzahl |
| :-------   | :---------                             | :----------    | -------:       | -----: |
| Mechanik   | Inside                                 | 3D-Druck       | PLA            | 1      |
| Mechanik   | Outside                                | 3D-Druck       | PLA            | 1      |

<a name="kantenabdeckungl"/>

[Gehäuse](#gehäuse)-KantenAbdeckung-L
| Teileart   | Teilename        | Herstellung    | Material | Anzahl |
| :-------   | :---------       | :----------    | -------: | -----: |
| Mechanik   | KantenHalt       | 3D-Druck       | PLA      | 2      |
| Mechanik   | ConnectorKanVerb | 3D-Druck       | PLA      | 1      |

<a name="kantenabdeckungk"/>

[Gehäuse](#gehäuse)-KantenAbdeckung-K
| Teileart   | Teilename        | Herstellung    | Material | Anzahl |
| :-------   | :---------       | :----------    | -------: | -----: |
| Mechanik   | KantenHalt       | 3D-Druck       | PLA      | 2      |
| Mechanik   | KantenAluVerb    | 3D-Druck       | PLA      | 1      |

<a name="botparttap"/>

[Gehäuse](#gehäuse)-BotPart-Tap
| Teileart   | Teilename                 | Herstellung    | Material | Anzahl |
| :-------   | :---------                | :----------    | -------: | -----: |
| Mechanik   | KantenHalt                | 3D-Druck       | PLA      | 2      |
| Mechanik   | ConnectorKanVerb          | 3D-Druck       | PLA      | 2      |
| Mechanik   | AluProfil_20x20           | Einkaufen      | l=500    | 3      |
| Mechanik   | AluProfil_20x20           | Einkaufen      | l=600    | 2      |
| Mechanik   | AluProfil_20x20           | Einkaufen      | l=194    | 2      |
| Mechanik   | CornerConnector (Default) | 3D-Druck       | PLA      | 4      |
| Mechanik   | CornerConnector (Left)    | 3D-Druck       | PLA      | 2      |
| Mechanik   | CornerConnector (Right)   | 3D-Druck       | PLA      | 2      |
| Mechanik   | Bodenplatte (Right)       | Laserschneiden | Holz     | 1      |
| Mechanik   | Bodenplatte (Default)     | Laserschneiden | Holz     | 1      |
| Mechanik   | T-Halterung               | 3D-Druck       | PLA      | 2      |
| Mechanik   | Mittelstütze              | 3D-Druck       | PLA      | 2      |
| Mechanik   | SchubladenleisteLinks     | 3D-Druck       | PLA      | 2      |
| Mechanik   | SchubladenleisteRechts    | 3D-Druck       | PLA      | 2      |

<a name="pcbdeckel"/>

[Gehäuse](#gehäuse)-PCB-Deckel
| Teileart   | Teilename          | Herstellung    | Material | Anzahl |
| :-------   | :---------         | :----------    | -------: | -----: |
| Elektronik | Tapatan_Deckel_PCB |                |          | 1      |
| Mechanik   | PCB-Haleer-Deckel  | 3D-Druck       | PLA      | 1      |

<a name="lcdassm"/>

[Gehäuse](#gehäuse)-LCDAssm
| Teileart   | Teilename | Herstellung    | Material  | Anzahl |
| :-------   | :-------- | :----------    | -------:  | -----: |
| Elektronik | LCD 20x40 |                | Einkaufen | 1      |
| Mechanik   | Chassis   | 3D-Druck       | PLA       | 1      |
| Mechanik   | Cover     | 3D-Druck       | PLA       | 1      |

<a name="chainbig"/>

[HGantry](#hgantry)-ChainBig:
| Teileart   | Teilename        | Herstellung | Material | Anzahl |
| :-------   | :---------       | :---------- | -------: | -----: |
| Mechanik   | ChainBigStart    | 3D-Druck    | PLA      | 1      |
| Mechanik   | ChainBigLink     | 3D-Druck    | PLA      | 19     |
| Mechanik   | ChainBigEnd      | 3D-Druck    | PLA      | 1      |

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
