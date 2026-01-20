        global idtKeyboardHandler
        global idtCascadeHandler
        global idtCOM2Handler
        global idtCOM1Handler
        global idtLPT2Handler
        global idtFloppyHandler
        global idtLPT1Handler
        global idtRTCHandler
        global idtMouseHandler
        global idtSB16Handler
        global idtTimerPre
        extern hcf
        extern idtTimer
idtKeyboardHandler:
idtCascadeHandler:
idtCOM2Handler:
idtCOM1Handler:
idtLPT2Handler:
idtFloppyHandler:
idtLPT1Handler:
idtRTCHandler:
idtMouseHandler:
idtSB16Handler:
        cli
        call hcf
        sti
        iretq

idtTimerPre:
        cli
        call idtTimer
        sti
        iretq
