//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_SYNTAX_H
#define VCC23_SYNTAX_H

namespace vcc23 {
  enum class Instruction : unsigned short
  {
    Unknown,
    Nop,
    
    AssignDecLitRef,
    AssignHexLitRef,
    AssignRefRef,
    AddDecLitRef,
    AddHexLitRef,
    AddRefRef,
    SubDecLitRef,
    SubHexLitRef,
    SubRefRef,
    MulDecLitRef,
    MulHexLitRef,
    MulRefRef,
    DivDecLitRef,
    DivHexLitRef,
    DivRefRef,
    NegRef,
    XorDecLitRef,
    XorHexLitRef,
    XorRefRef,
    AndDecLitRef,
    AndHexLitRef,
    AndRefRef,
    OrDecLitRef,
    OrHexLitRef,
    OrRefRef,
    LeftShiftDecLitRef,
    LeftShiftHexLitRef,
    RightShiftDecLitRef,
    RightShiftHexLitRef,
    CmpDecLitRef,
    CmpHexLitRef,
    CmpRefRef,
    CmpRef,
    AbsJumpDecLit,
    AbsJumpHexLit,
    AbsJumpRef,
    RelJumpBackDecLit,
    RelJumpBackHexLit,
    RelJumpBackRef,
    RelJumpForwardDecLit,
    RelJumpForwardHexLit,
    RelJumpForwardRef,
    SelectRamDefault,
    SelectRamDecLit,
    SelectRamHexLit,
    SelectRomDefault,
    SelectRomDecLit,
    SelectRomHexLit,
    ReadInputDeviceRef,
    ReadInputDeviceDecLitRef,
    ReadInputDeviceHexLitRef,
    ReadInputDeviceRefRef,
    WriteOutputDeviceDecLit,
    WriteOutputDeviceHexLit,
    WriteOutputDeviceRef,
    PrintDecLitRef,
    PrintHexLitRef,
    PrintRefRef,
    EndWithDecLitExitCode,
    EndWithHexLitExitCode,
    EndWithSuccess
  };
}

#endif //VCC23_SYNTAX_H
