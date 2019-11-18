# HTTP nástěnka

Projekt obsahuje dvě aplikace, klienta a server.
Server obsahuje seznam nástěnek, přičemž každá nástěnka může obsahovat několik příspěvků.
Pomocí klienta je možné zasílat http požadavky skrze API na server, který je následně zpracuje. 
Pomocí toho API je možné vkládat, mazat či upravovat dané nástněnky či příspěvky.
Dotazy na server lze samozřejmě zasílat pomocí webového prohlížeče či jiné aplikace, např. Postman,
nicméně zde je nutné dodržet správný formát requestu.

V případě, že na server přijde více requestů v jeden okamžik, se server rozdělí na dva procesy, přičemž child proces obstará rutinu ohledně zpracování a odeslání dat klientovi.
Rodičovský proces je tedy opět připraven na přijímání dalších requestů. Rutina zpracování dat
v child procesu představuje načtení aktuálního stavu nástěnky ze sdílené paměti, její následnou
deserializaci do vektoru, zpracování a odeslání dat, a následnou serializace nástěnek do sdílené paměti. 
 Vše je samožejmě opatřeno mutexy.
 

# Použití
## Přeložení
```
make 
```
## Spuštění klienta
```
./isaclient [-h] -H <host> -p <port> <command>
```
Volitlený parametr "-h" zobrazí informace o spuštění

&#60;Host&#62; : zvolený host

&#60;port&#62; : Port, na který se bude klient připojovat, v rozsahu 1-65535, prvních 1024 portů vyhrazneo systémem

&#60;Command&#62; : Dosazený příkaz dle API

* boards -  Vrátí seznam nástěnek
* board add &#60;name&#62; - Vytvoří novou prázdnou nástěnku &#60;name&#62;
* board delete &#60;name&#62; - Odstraní nástněku &#60;name&#62;
* board list &#60;name&#62; - Vrátí obsah nástěnky &#60;name&#62;
* item add &#60;name&#62; &#60;content&#62; - Vloží příspěvek &#60;content&#62; do nástěnky &#60;name&#62;
* item delete &#60;name&#62; &#60;id&#62; - Smaže příspěvěk z nástěnky &#60;name&#62; s číslem &#60;id&#62;
* item update &#60;name&#62; &#60;id&#62; &#60;content&#62; - Změní obsah příspěvku nástěnky &#60;name&#62; s číslem &#60;id&#62;
na obsah daný položkou &#60;content&#62;

## Spuštění serveru
```
./isaserver [-h] -p <port> 
```
Volitelný parametr "-h" zobrazí informace o spuštění

&#60;port&#62; : Port, na kterém bude server naslouchat, v rozsahu 1-65535, prvních 1024 portů vyhrazeno systémem

## Příklad spuštění
Spustíme server, který bude naslouchat na portu 12345
```
./isaserver -p <12345> 
```
Nyní spustíme klienta, který odešle request na daný port serveru (v našem případě 12345) a vytvoří novou nástěnku "TestBoard".
Následně do této nástěnky vloží příspěvěk "firstCommit".
Nakonec si zobrazíme stav nástěnky "TestBoard".
```
./isaclient -H localhost -p 12345 board add TestBoard
./isaclient -H localhost -p 12345 board add TestBoard firstCommit
./isaclient -H localhost -p 12345 board list TestBoard
```
Výstup by měl vypadat takto:
```
[TestBoard]

1. firstCommit
```

# Seznam souborů
<pre>
.
├── client
│   ├── ClientArgumentParser.cpp
│   ├── ClientArgumentParser.h
│   ├── Client.cpp
│   └── Client.h
├── CMakeLists.txt
├── doc
├── error
│   └── Error.h
├── helpers
│   ├── SignalHandler.cpp
│   ├── SignalHandler.h 
│   ├── VectorMapper.cpp
│   └── VectorMapper.h
├── isaclient.cpp
├── isaserver.cpp
├── README.md
└──  server
    ├── DataProcesser.cpp
    ├── DataProcesser.h
    ├── RequestParser.cpp
    ├── RequestParser.h
    ├── ServerArgumentParser.cpp
    ├── ServerArgumentParser.h
    ├── Server.cpp
    └── Server.h
</pre>

# Omezení / doporučení
Jelikož server běží v nekonečné smyčce, tak pro jeho správné ukočení 
je nutno použít signál SIGINT (klávesová zkratka CTRL+C).