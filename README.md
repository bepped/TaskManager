#Task Manager

Semplice task manager che esegue contemporaneamente un certo numero di task.
Il numero di task viene impostato nel file taskmanager.ini presente nella directory dell'eseguibile.
Nel file deve essere presente la seguente riga: number=<numero di task>.
Nel caso in cui non fosse presente il file o la riga di cui sopra, viene generato un solo task.

Ogni task viene configurato tramite un task editor, attraverso il quale si devono:

1) selezionare il tipo di task da eseguire; vi sono 2 tipi di task: 
	a) stampare una stringa a video 
	b) verificare la presenza di un file

2) inserire la stringa da stampare o il file da verificare

3) selezionare la frequenza (mutuamente esclusiva) che puo essere:
	a) secondi
	b) ore
	c) giorni

4) impostare l'intervello di tempo che è espresso in 
	a) secondi
	b) ore 
	d) elenco di giorni

5) selezionare giorno e ora di avvio del task


E' possibile eseguire in determinati giorni i task con frequenza secondi o ore.
Se nessun giorno è selezionato i task di secondi o ore vengono eseguiti indefinitamente
a partire dalla data-ora impostata.

Tramite il task editor è posibile: salvare il task, ignorare le modifiche nei parametri del task, eliminare il task.