Samtalespillet
Samtalespillet er et samarbeidsspill, ment for bruk på date, basert på Arduino. Det krever også RFID-scanner og 30-dioders led-strip. Spillet er et resultat av en bruksorientert designprosess i kurset IN1060 ved Universitet i Oslo.
Spillets gang
Spillet krever to brukere, som bytter på å være forteller og gjetter. Fortelleren trekker et spørsmålskort og uten å fortelle gjetteren hvilket spørsmål hun trakk, besvarer hun spørsmålet for gjetteren. Gjetteren får lese opp fire ulike alternativer til spørsmål fortelleren kan ha svart på. Han skal nå gjette hvilket av alternativene han tror er riktig. Gjetteren han riktig, får paret poeng.

Teknisk forklaring
Spillet starter med en tilstand hvor det lyser svakt i alle led-diodene. Dette signaliserer at spillet er klart til start. Bruker 1 scanner et spørsmålskort, og får feedback gjennom et lilla lys. Alle diodene slukkes, én etter én. Spillet venter nå på at bruker 2 skal registrere sitt svaralternativskort. Det skapes spenning ved at 3 lysdioder ”faller” ned fra toppen av led-stripen, til bunnen. Er svaret galt, blinker det rødt to ganger. Er derimot svaret riktig, blinker det grønt lys to ganger, og 3 dioder som indikerer skår, lyser opp. Ved hvert riktige svar, lyser nye 3 dioder opp. Har man svart riktig på 10 spørsmål, blinker alle led-stripens dioder. Spillet resettes etter noen sekunder, og er klart til å spilles på ny.
Bygget med
MFRC522 RFID Library - Brukt for å skrive til og registrere kort.
FastLED Library - Brukt for å lage lysmønstre.
