# Garagentor

Loxone führt die Torlogik aus. Home Assistant sendet Befehle über KNX und zeigt den von Loxone gemeldeten Zustand an. Die Zuordnung erfolgt im jeweiligen Projekt.

## Anschlüsse

| Programm | Signal | Zuordnung |
| --- | --- | --- |
| I1–I3 | Endlage AUF, Endlage ZU, Motorspannung | Drei unabhängige Rückmeldungen zuordnen |
| I4–I8 | AUF, ZU, STOP, Referenzfahrt, Teilposition anfahren | Fünf Taster oder KNX-Befehle zuordnen |
| I9 | Fahrfreigabe | Bis zur geprüften Inbetriebnahme ausgeschaltet lassen |
| I10 | Wunschposition 1–99 Prozent | Gemeinsame einstellbare Zahl. Startwert 50. |
| I11–I12 | Gespeicherte Fahrzeiten AUF und ZU | Remanente Analogspeicher |
| I13 | Zusammengefasste Zeiteinstellungen | Externe Rechenbausteine nach Tabelle |
| O1–O2 | Anforderungen AUF und ZU | Zwei KNX-Ausgänge. Der Aktor erzeugt den Kontaktimpuls. |
| O3–O6 | Zustand, Position, Referenzschritt, Fehler | Offline prüfen und in der Oberfläche beschriften |
| O7–O10 | Fahrzeiten AUF/ZU, letzte externe Fahrtrichtung, Teilziel erlaubt | Fahrzeitspeicher und Anzeige zuordnen |

## Zeiten einstellen

Die sichtbaren Einstellungen sind Sekundenwerte. In Loxone Config die Werte in separaten Bausteinen beschriften. Die Faktoren bleiben beim Einrichten fest.

| Einstellung | Standard | Faktor | Schritt / Bereich |
| --- | --- | --- | --- |
| Stillstand | 1 s | 64 | 0,5 s / 0,5–15,5 s |
| Motorstart-Timeout | 5 s | 2048 | 0,5 s / 0,5–15,5 s |
| Stopp-Timeout | 5 s | 65536 | 0,5 s / 0,5–15,5 s |
| Zuschlag zur gemessenen Fahrzeit | 10 s | 524288 | 2 s / 0–30 s |

Die Summe der vier Produkte plus 1 geht an I13. Die Kontaktzeit wird am KNX-Aktor eingestellt. 0,5 s ist ein Startwert für den Vor-Ort-Test.

## Prüfen

Die Referenzfahrt startet an einer bestätigten Endlage. Jede vollständige Fahrt speichert die gemessene Zeit der Richtung. Teilziele brauchen zwei gültige Zeiten. Aus einer unbekannten Position wird zuerst die obere Endlage angefahren. STOP verwirft eine geplante Weiterfahrt.

Ein manueller HA-Notbetrieb setzt voraus, dass HA beide Endlagen und das Motorsignal unabhängig von Loxone empfängt. Torbefehle bleiben bis zu dieser Zuordnung gesperrt.
