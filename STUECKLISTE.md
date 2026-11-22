# Stückliste für die Garagentorsteuerung

Das Beispiel verwendet einen Normstahl Magic 1000-3. Die beiden Endlagen und die Motoransteuerung werden getrennt erfasst. Die Stückzahlen gelten für ein Tor.

## Bei beiden Varianten

| Menge | Teil | Aufgabe |
| ---: | --- | --- |
| 1 | Torantrieb mit getrennten AUF- und ZU-Eingängen | Im Beispiel Normstahl Magic 1000-3. Bei C6 = 002 wurde der Stopp per Richtungsimpuls am Beispielantrieb geprüft. |
| 1 | Endlagen-Zusatzkarte oder zwei potentialfreie Endlagenkontakte | Je ein Kontakt für Endlage AUF und ZU. Im Beispiel ZKMagic. |
| 1 | Optokoppler Lite-On LTV-814 | Erfasst die Motorspannung bei beiden Polaritäten. |
| 1 | Widerstand 8,2 kΩ, 0,25 W | Vorwiderstand für den Optokoppler gemäß Beispielschaltplan. |
| 1 | 24-V-DC-Netzteil für die Rückmeldungen | Vorhandene geeignete Kleinspannungsversorgung kann mitverwendet werden. |
| nach Bedarf | Adern, Klemmen und isolierte Unterbringung | Im Beispiel sieben Adern bei gemeinsamem Rückleiter für AUF und ZU. Die gemeinsame Bezugsklemme am jeweiligen Antrieb prüfen. |

## Variante 1 · Loxone und KNX

| Menge | Teil | Aufgabe |
| ---: | --- | --- |
| 1 | Loxone Miniserver mit KNX-Zugang und drei freien 24-V-Digitaleingängen | Endlage AUF, Endlage ZU und Motoransteuerung. Im Beispiel Miniserver Gen. 1. |
| 1 | KNX-Schaltaktor mit zwei freien geeigneten Relaiskanälen | Potentialfreie Kontaktimpulse für AUF und ZU. Im Beispiel SwitchME12. |
| bei Bedarf | KNX-Busspannungsversorgung | Erforderlich, wenn noch kein versorgter KNX-Bus vorhanden ist. |

**Loxone ohne KNX:** Hat der Miniserver zwei freie, geeignete potentialfreie Relaisausgänge, können diese AUF und ZU direkt als kurze Kontaktimpulse ansteuern. Die drei Digitaleingänge bleiben für die Rückmeldungen vorgesehen. Der KNX-Schaltaktor und der KNX-Bus entfallen für diese Variante.

## Variante 2 · KNX ohne Loxone

| Menge | Teil | Aufgabe |
| ---: | --- | --- |
| 1 | KNX-Binäreingang mit mindestens drei Kanälen für 24 V DC | Beispiel MDT BE-04024.02. Drei Kanäle nutzen, einen als Reserve lassen. Den Optokoppler am konkreten Eingangsmodul im Langsamlauf in beiden Richtungen prüfen. |
| 1 | KNX-Schaltaktor mit zwei freien geeigneten Relaiskanälen | Zwei potentialfreie Schließer für AUF und ZU mit automatischer Abschaltung nach dem Impuls. |
| bei Bedarf | KNX-Busspannungsversorgung | Erforderlich, wenn noch kein versorgter KNX-Bus vorhanden ist. |
| bei HA-Bedienung | Home Assistant mit KNX-Schnittstelle | Verarbeitet die drei KNX-Rückmeldungen und sendet die Befehle. Der HA-Blueprint in diesem Repository deckt den manuellen Notbetrieb ab. |

**Die Marke des KNX-Schaltaktors ist frei wählbar.** Geeignet ist jeder Schaltaktor, der zwei potentialfreie Schließerkontakte für den Steuereingang des Torantriebs bereitstellt und kurze Impulse mit automatischer Abschaltung ausgeben kann. Die erforderliche Kontaktzeit am Antrieb prüfen.

- [Beispielschaltplan Loxone und KNX](beispiele/loxone-knx/garagentor-schaltplan-v8.svg)
- [MDT-Binäreingang BE](https://www.mdt.de/produkte/produktdetail/sensorik/binaereingaenge/binaereingang-be.html)
