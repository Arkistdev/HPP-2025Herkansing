[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/TNDugn9X)
# Circuit Satisfiability

Het Circuit Satisfiability Problem voor een gegeven binair circuit bestaat uit het vinden van een geschikte input waarbij het circuit een waarde '1' als output geeft. Bijvoorbeeld, beschouw het volgende 32-bit (32 input lijnen) circuit diagram:

![32bitCircuit](https://user-images.githubusercontent.com/4514056/220600573-db22f4f6-2a72-4c3a-96f8-fb7e2fac9d91.png)

Aangezien dit circuit gebruik maakt van AND-gates, OR-gates en inverters, kunnen we het hele circuit in C++ modelleren door gebruik van de logische operatoren AND (&&), OR (||) en NOT (!). Een voorbeeld sequentieel programma is te zien in [circuitSatisfiability.cc](circuitSatisfiability.cc); dit programma modelleert een dergelijk circuit en lost het probleem op door te loop-en over alle mogelijke invoerpatronen, en schrijft enkel die invoerpatronen naar het scherm die een '1' als uitvoer tot gevolg hebben.

## Deel 1

Download het [circuitSatisfiability.cc](circuitSatisfiability.cc) programma, en pas het aan zodat we kunnen meten hoe lang het programma nodig heeft om de berekening uit te voeren:

```cpp
// ...
double startTime = 0.0, totalTime = 0.0;
startTime = MPI_Wtime();

for (i = 0; i < UNIT_MAX; i++) {
    count += checkCircuit(id, i);
}

totalTime = MPI_Wtime() - startTime;
cout << "Process " << id << " finished in " << totalTime << " seconds." << endl;
// ...
```

Compileer het programma vervolgens met:

`mpic++ circuitSatisfiability.cc -Wall -ansi -pedantic -std=c++17 -o circuitSatisfiability`

En draai het met

`mpirun -np 1 ./circuitSatisfiability`

**Let op: dit programma gaat 232 mogelijkheden proberen, en heeft daar even de tijd voor nodig (± 5 minuten)! Zorg dat je wat anders te doen hebt terwijl het programma draait ;-)**

Creëer een spreadsheet met de kolommen Number of Processes en Time. Noteer hierin een '1' in de eerste rij en daarnaast de tijd die het kostte om te berekenen. Dit geldt als onze baseline waartegen we zullen vergelijken. Noteer in een extra kolom hoeveel oplossingen deze sequentiele versie van het programma heeft gevonden.

## Deel 2

Gebruik parallelLoopChunks.cc (of parallelLoopSlices.cc) als voorbeeld, en gebruik het parallel loop patroon om circuitSatisfiability.cc te versnellen.

Zoek op hoe `MPI_Reduce` werkt om het reduction patroon te kunnen gebruiken om de gedistribueerde count-values samen te brengen in een globale count, en laat proces 0 deze global count naar het scherm schrijven. (Vergelijk je global count met die van de sequentiële versie, om te zien of je geen fouten hebt gemaakt).

Zodra je programma correct compileert, draai je het met verschillende hoeveelheden processen (2, 4 ,8, 16, 32, 64, 128 en 256; zover je PC dat toelaat). Voeg de tijd die elk van deze duurt toe aan je spreadsheet.

Om te voorkomen dat mpirun een error geeft als je meer processen probeert te starten dan beschikbare cores op je PC, draai je mpirun met de `-oversubscribe` flag. Deze vertelt mpirun dat hij meer processen mag maken dan beschikbare cores. (Probeer tijdens het draaien van meerdere processen zo min mogelijk met je PC te doen, om de test resultaten niet te beinvloeden).

Maak een line-chart van de gevonden waardes (met op de x-as de hoeveelheid processen, en op de y-as de benodigde tijd).

## Analyse

- Voeg je spreadsheet toe aan je verslag, met de data en de line-chart. Denk erom dat je je grafiek voorziet van een titel, en de assen duidelijk benoemt (inclusief meeteenheden). 
- Maak een korte analyse (max 1 A4) van de resultaten in je grafiek. Probeer deze zo kwantitatief mogelijk te houden, refererend aan de data die je hebt verzameld.

## Inleveren HPP Opdrachten
Voor de opdrachten van High Performance Programming lever je een verslag in, in PDF formaat.

Begin het verslag met:

- De titel van de opdracht;
- Je naam en studentnummer;
- Een link naar je GitHub Classroom repository met je werk.

- Lees de hele opdracht goed door, stel alvast vragen als iets niet duidelijk is
- Voor ieder deel / vraag:
  -  Vermeld het nummer van het deel of de vraag;
  -  Maak de opdracht en/of beantwoord de vraag;
  -  Kies code snippets en/of screenshots om je oplossing te laten zien;
  -  Beschrijf je oplossing beknopt, waarbij je vooral duidelijk maakt hoe je het hebt aangepakt.

Bewaar / exporteer je verslag als PDF en lever die in.
