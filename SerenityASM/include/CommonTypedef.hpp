//
//  CommonType.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef CommonType_h
#define CommonType_h

#include <string>
#include <fstream>
using namespace std;

typedef enum {
    No_errors,
    Wrong_formation,
    Illegal_characters_in_label,
    Label_duplication,
    The_amount_of_operand_is_wrong,
    No_such_register,
    No_such_instruction_or_pseudo_instruction,
    No_such_label,
    No_such_coregister,
    Redefined_label,
    Can_not_assemble,
    Wrong_immediate_number_or_offset,
    Illegal_origin_address,
    reduplicated_origin
} ErrorInfo;  // 错误信息

typedef unsigned int MachineCode;
typedef string AssemblyCode;
typedef unsigned int Address;
typedef string RegType;
typedef string Label;
typedef string OpName;
typedef string Register;

typedef enum{RegIns,ImmIns,SpcIns,JumpIns,LoadSaveIns, PseudoIns, FormatIns} InsType;

const ErrorInfo correct = No_errors;
const MachineCode errorIns = 0xfffffff;

#endif /* CommonType_h */
