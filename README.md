# Prova finale di Algoritmi e Strutture Dati 2022-2023

## Descrizione del Progetto

Questo progetto implementa un programma in C per la pianificazione del percorso ottimale su un'autostrada che presenta una serie di stazioni di servizio, ciascuna dotata di un parco veicoli elettrici a noleggio. L'obiettivo è identificare il percorso con il minor numero di tappe tra due stazioni di servizio, rispettando i vincoli dati dall'autonomia dei veicoli e dalle distanze delle stazioni.

### Autore

Il progetto è stato da me sviluppato come prova finale di Algoritmi e Strutture Dati per l'anno accademico 2022-2023.

### Funzionalità Implementate

Il programma gestisce le seguenti operazioni:

1. **Aggiunta di Stazioni**: 
   - Comando: `aggiungi-stazione`
   - Aggiunge una nuova stazione alla distanza specificata dall'inizio dell'autostrada, con un parco veicoli di diverse autonomie.

2. **Demolizione di Stazioni**: 
   - Comando: `demolisci-stazione`
   - Rimuove una stazione esistente alla distanza specificata.

3. **Aggiunta di Veicoli**: 
   - Comando: `aggiungi-auto`
   - Aggiunge un veicolo con una determinata autonomia a una stazione esistente.

4. **Rottamazione di Veicoli**: 
   - Comando: `rottama-auto`
   - Rimuove un veicolo con una determinata autonomia da una stazione esistente.

5. **Pianificazione del Percorso**:
   - Comando: `pianifica-percorso`
   - Pianifica e restituisce il percorso ottimale tra due stazioni, minimizzando il numero di tappe. Se esistono più percorsi con lo stesso numero di tappe, viene scelto il percorso con la distanza minima dalla partenza.

### Utilizzo

Per utilizzare il programma, compila il file `progetto_30L.c` usando un compilatore C (ad esempio, GCC) e lancia il programma.

Le opzioni di compilazione utilizzate in fase di valutazione sono le seguenti:
   ```bash
	gcc -Wall -Werror -std=gnu11 -02 -lm test.c -o progetto_30L.c
   ```
   
Per eseguire il programma:

   ```bash
   ./autostrada
   ```

Inserisci i comandi secondo il formato specificato nella descrizione del progetto.
Nella cartella `test` sono presenti i casi di test forniti dai docenti.

1. **Aggiungere una stazione**:
   ```
   aggiungi-stazione [distanza] [numero-auto] [autonomia-auto-1] ... [autonomia-auto-n]
   ```

2. **Demolire una stazione**:
   ```
   demolisci-stazione [distanza]
   ```

3. **Aggiungere un'auto a una stazione**:
   ```
   aggiungi-auto [distanza-stazione] [autonomia-auto-da-aggiungere]
   ```

4. **Rottamare un'auto da una stazione**:
   ```
   rottama-auto [distanza-stazione] [autonomia-auto-da-rottamare]
   ```

5. **Pianificare un percorso**:
   ```
   pianifica-percorso [distanza-stazione-partenza] [distanza-stazione-arrivo]
   ```
