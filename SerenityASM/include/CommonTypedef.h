#ifndef __COMMON_TYPEDEF__
#define __COMMON_TYPEDEF__

#include <string>
#include <vector>

using namespace std;

typedef enum {
    No_errors,
    Wrong_formation,
    Illegal_characters_in_label,
    Label_duplication,
    The_amount_of_operand_is_wrong,
    No_such_register,
    No_such_instruction_or_pseudo_instruction,
    Redefined_label,
    Can_not_assemble
} ErrorInfo;

typedef unsigned int Instruction;
typedef unsigned int Address;
typedef string AssembleIns;
typedef string Register;
typedef string RegType;
typedef string Label;
const ErrorInfo correct = No_errors;
const Instruction errorIns = 0xffffffff;

#endif