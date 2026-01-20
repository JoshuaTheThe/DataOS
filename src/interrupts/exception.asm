        global idtPageFaultHandler
        global idtInvalidOpcodeHandler
        global idtDivideByZeroHandler
        global idtGeneralProtectionFaultHandler
        global idtDefaultHandler
        global idtDebugHandler
        global idtNMIHandler
        global idtBreakpointHandler
        global idtOverflowHandler
        global idtBoundRangeHandler
        global idtDeviceNotAvailableHandler
        global idtDoubleFaultHandler
        global idtCoprocessorSegmentOverrunHandler
        global idtInvalidTSSHandler
        global idtSegmentNotPresentHandler
        global idtStackSegmentFaultHandler
        global idtFloatingPointHandler
        global idtAlignmentCheckHandler
        global idtMachineCheckHandler
        global idtSIMDFloatingPointHandler
        global idtVirtualizationHandler
        global idtControlProtectionHandler
        extern hcf

idtDivideByZeroHandler:
        cli
        call hcf
        sti
        iretq

idtDebugHandler:
        cli
        call hcf
        sti
        iretq

idtNMIHandler:
        cli
        call hcf
        sti
        iretq

idtBreakpointHandler:
        cli
        call hcf
        sti
        iretq

idtOverflowHandler:
        cli
        call hcf
        sti
        iretq

idtBoundRangeHandler:
        cli
        call hcf
        sti
        iretq

idtInvalidOpcodeHandler:
        cli
        call hcf
        sti
        iretq

idtDeviceNotAvailableHandler:
        cli
        call hcf
        sti
        iretq

idtDoubleFaultHandler:
        cli
        call hcf
        sti
        iretq

idtCoprocessorSegmentOverrunHandler:
        cli
        call hcf
        sti
        iretq

idtInvalidTSSHandler:
        cli
        call hcf
        sti
        iretq

idtSegmentNotPresentHandler:
        cli
        call hcf
        sti
        iretq

idtStackSegmentFaultHandler:
        cli
        call hcf
        sti
        iretq

idtGeneralProtectionFaultHandler:
        cli
        call hcf
        sti
        iretq

idtPageFaultHandler:
        cli
        call hcf
        sti
        iretq

idtFloatingPointHandler:
        cli
        call hcf
        sti
        iretq

idtAlignmentCheckHandler:
        cli
        call hcf
        sti
        iretq

idtMachineCheckHandler:
        cli
        call hcf
        sti
        iretq

idtSIMDFloatingPointHandler:
        cli
        call hcf
        sti
        iretq

idtVirtualizationHandler:
        cli
        call hcf
        sti
        iretq

idtControlProtectionHandler:
        cli
        call hcf
        sti
        iretq
