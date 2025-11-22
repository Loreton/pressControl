# pressControl
ESP32 - Gestione pompa ed allarme in caso di lunga apertura
per evitare il problema del suono emesso dai buzzer in casa è molto basso, proviamo ad utilizzare l'output dei buzze in  OPEN-COLLECTOR


ESP32 - devo controllare una pompa provvista di un press-control.
    Ho un relè per comandare il press-control.

    un pin per monitorare status del press-control
    un pin per monitorare status della pompa

    un led per mostrare status del press-control
    un led per mostrare status della pompa

    un buzzer passivo per segnalare con suoni
    un buzzer attivo per inviare beep

    un pulsante per accendere e spegnere il press-control

    Una volta che viene rilevata l'accensione della pompa emettere un suono di 4 frequenze crescenti sul buzzer passivo e decrescenti nel caso la pompa si spegne.

    Se la pompa continua a rimanere accesa allora emette un beep dopo 60 secondi, un beep dopo altri 55 secondi, un beep dopo altri 50 secondi e così via per 6 step.

    Dopo l'ultimo step:
    1. spegnere il press-control
    2. se il press-control o la pompa dovessero essere ancora accesi emettere un beep ogni 5 sec.

    Comunque dall'accensione del press-control impostare un timer per spegnerlo dopo 30 minuti

    Lo stato del press-control, della pompa  e del tempo rimasto del timer devono essere inviati a telegram



    pressControState:
        pressed:
            se il relay non è acceso allora vuol dire che è stato attivato il relè magnetotermico.
            Siccome, al momento, non ha l'inching, attiviamo anche il relay interno con uno puilsetime si 30 minuti

        released:


    Esp32 ho un relè che comanda una pompa ed un pin che controlla il suo stato.  Una volta accesa emettere in beep dopo  60 secondi,
    un beep dopo altri 55 secondi, un beep dopo altri 50 secondi e così via per 6 step. Dopo l'ultimo step emettere un beep ogni 5 sec.
