/*
 ********************************************************************************
 ** Copyright (C) 2018 Donald J. Bartley <djbcoffee@gmail.com>
 **
 ** This source file may be used and distributed without restriction provided
 ** that this copyright statement is not removed from the file and that any
 ** derivative work contains the original copyright notice and the associated
 ** disclaimer.
 **
 ** This source file is free software; you can redistribute it and/or modify it
 ** under the terms of the GNU General Public License as published by the Free
 ** Software Foundation; either version 2 of the License, or (at your option) any
 ** later version.
 **
 ** This source file is distributed in the hope that it will be useful, but
 ** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 ** FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 ** more details.
 **
 ** You should have received a copy of the GNU General Public License along with
 ** this source file.  If not, see <http://www.gnu.org/licenses/> or write to the
 ** Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 ** 02110-1301, USA.
 ********************************************************************************
 ** File: 4004-as/src/lexer.h
 **
 ** Description:
 ** Header file that goes with lexer.c
 ********************************************************************************
 ** Version 1.0.0
 ********************************************************************************
 ** DJB 10/13/18 Created.
 ********************************************************************************
 */

#ifndef ___LEXER_H___
#define ___LEXER_H___

//------------------------------------------------------------------------------
//Defines
#define MAX_PROGRAM_MEMORY              (4096)

#define MAX_SYMBOL_SIZE                 (255)
#define MAX_DIR_MNEMONIC_SIZE           (4)
#define MAX_OPERANDS                    (2)

#define BYTE_DIRECTIVE_SUCCESS          (1)
#define END_DIRECTIVE_SUCCESS           (2)
#define EQU_DIRECTIVE_SUCCESS           (3)
#define ORG_DIRECTIVE_SUCCESS           (4)

#define BYTE_DIRECTIVE_TEXT             "BYTE"
#define END_DIRECTIVE_TEXT              "END"
#define EQU_DIRECTIVE_TEXT              "EQU"
#define ORG_DIRECTIVE_TEXT              "ORG"

#define ADD_INSTRUCTION_TEXT            "ADD"
#define ADM_INSTRUCTION_TEXT            "ADM"
#define BBL_INSTRUCTION_TEXT            "BBL"
#define CLB_INSTRUCTION_TEXT            "CLB"
#define CLC_INSTRUCTION_TEXT            "CLC"
#define CMA_INSTRUCTION_TEXT            "CMA"
#define CMC_INSTRUCTION_TEXT            "CMC"
#define DAA_INSTRUCTION_TEXT            "DAA"
#define DAC_INSTRUCTION_TEXT            "DAC"
#define DCL_INSTRUCTION_TEXT            "DCL"
#define FIM_INSTRUCTION_TEXT            "FIM"
#define FIN_INSTRUCTION_TEXT            "FIN"
#define IAC_INSTRUCTION_TEXT            "IAC"
#define INC_INSTRUCTION_TEXT            "INC"
#define ISZ_INSTRUCTION_TEXT            "ISZ"
#define JCN_INSTRUCTION_TEXT            "JCN"
#define JIN_INSTRUCTION_TEXT            "JIN"
#define JMS_INSTRUCTION_TEXT            "JMS"
#define JUN_INSTRUCTION_TEXT            "JUN"
#define KBP_INSTRUCTION_TEXT            "KBP"
#define LD_INSTRUCTION_TEXT             "LD"
#define LDM_INSTRUCTION_TEXT            "LDM"
#define NOP_INSTRUCTION_TEXT            "NOP"
#define RAL_INSTRUCTION_TEXT            "RAL"
#define RAR_INSTRUCTION_TEXT            "RAR"
#define RD0_INSTRUCTION_TEXT            "RD0"
#define RD1_INSTRUCTION_TEXT            "RD1"
#define RD2_INSTRUCTION_TEXT            "RD2"
#define RD3_INSTRUCTION_TEXT            "RD3"
#define RDM_INSTRUCTION_TEXT            "RDM"
#define RDR_INSTRUCTION_TEXT            "RDR"
#define SBM_INSTRUCTION_TEXT            "SBM"
#define SRC_INSTRUCTION_TEXT            "SRC"
#define STC_INSTRUCTION_TEXT            "STC"
#define SUB_INSTRUCTION_TEXT            "SUB"
#define TCC_INSTRUCTION_TEXT            "TCC"
#define TCS_INSTRUCTION_TEXT            "TCS"
#define WMP_INSTRUCTION_TEXT            "WMP"
#define WPM_INSTRUCTION_TEXT            "WPM"
#define WR0_INSTRUCTION_TEXT            "WR0"
#define WR1_INSTRUCTION_TEXT            "WR1"
#define WR2_INSTRUCTION_TEXT            "WR2"
#define WR3_INSTRUCTION_TEXT            "WR3"
#define WRM_INSTRUCTION_TEXT            "WRM"
#define WRR_INSTRUCTION_TEXT            "WRR"
#define XCH_INSTRUCTION_TEXT            "XCH"

#define NUMBER_OF_ADD_TYPES             (1)
#define NUMBER_OF_ADM_TYPES             (1)
#define NUMBER_OF_BBL_TYPES             (1)
#define NUMBER_OF_CLB_TYPES             (1)
#define NUMBER_OF_CLC_TYPES             (1)
#define NUMBER_OF_CMA_TYPES             (1)
#define NUMBER_OF_CMC_TYPES             (1)
#define NUMBER_OF_DAA_TYPES             (1)
#define NUMBER_OF_DAC_TYPES             (1)
#define NUMBER_OF_DCL_TYPES             (1)
#define NUMBER_OF_FIM_TYPES             (1)
#define NUMBER_OF_FIN_TYPES             (1)
#define NUMBER_OF_IAC_TYPES             (1)
#define NUMBER_OF_INC_TYPES             (1)
#define NUMBER_OF_ISZ_TYPES             (1)
#define NUMBER_OF_JCN_TYPES             (1)
#define NUMBER_OF_JIN_TYPES             (1)
#define NUMBER_OF_JMS_TYPES             (1)
#define NUMBER_OF_JUN_TYPES             (1)
#define NUMBER_OF_KBP_TYPES             (1)
#define NUMBER_OF_LD_TYPES              (1)
#define NUMBER_OF_LDM_TYPES             (1)
#define NUMBER_OF_NOP_TYPES             (1)
#define NUMBER_OF_RAL_TYPES             (1)
#define NUMBER_OF_RAR_TYPES             (1)
#define NUMBER_OF_RD0_TYPES             (1)
#define NUMBER_OF_RD1_TYPES             (1)
#define NUMBER_OF_RD2_TYPES             (1)
#define NUMBER_OF_RD3_TYPES             (1)
#define NUMBER_OF_RDM_TYPES             (1)
#define NUMBER_OF_RDR_TYPES             (1)
#define NUMBER_OF_SBM_TYPES             (1)
#define NUMBER_OF_SRC_TYPES             (1)
#define NUMBER_OF_STC_TYPES             (1)
#define NUMBER_OF_SUB_TYPES             (1)
#define NUMBER_OF_TCC_TYPES             (1)
#define NUMBER_OF_TCS_TYPES             (1)
#define NUMBER_OF_WMP_TYPES             (1)
#define NUMBER_OF_WPM_TYPES             (1)
#define NUMBER_OF_WR0_TYPES             (1)
#define NUMBER_OF_WR1_TYPES             (1)
#define NUMBER_OF_WR2_TYPES             (1)
#define NUMBER_OF_WR3_TYPES             (1)
#define NUMBER_OF_WRM_TYPES             (1)
#define NUMBER_OF_WRR_TYPES             (1)
#define NUMBER_OF_XCH_TYPES             (1)

//------------------------------------------------------------------------------
//Enumerations
enum Passes
{
    PassOne = 1,
    PassTwo
};

enum DirectiveIndexes
{
    ByteDirective = 0,  //Must start at zero to match first index of array.
    EndDirective,
    EquDirective,
    OrgDirective
};

enum InstructionIndexes
{
    AddInstruction = 0, //Must start at zero to match first index of array.
    AdmInstruction,
    BblInstruction,
    ClbInstruction,
    ClcInstruction,
    CmaInstruction,
    CmcInstruction,
    DaaInstruction,
    DacInstruction,
    DclInstruction,
    FimInstruction,
    FinInstruction,
    IacInstruction,
    IncInstruction,
    IszInstruction,
    JcnInstruction,
    JinInstruction,
    JmsInstruction,
    JunInstruction,
    KbpInstruction,
    LdInstruction,
    LdmInstruction,
    NopInstruction,
    RalInstruction,
    RarInstruction,
    Rd0Instruction,
    Rd1Instruction,
    Rd2Instruction,
    Rd3Instruction,
    RdmInstruction,
    RdrInstruction,
    SbmInstruction,
    SrcInstruction,
    StcInstruction,
    SubInstruction,
    TccInstruction,
    TcsInstruction,
    WmpInstruction,
    WpmInstruction,
    Wr0Instruction,
    Wr1Instruction,
    Wr2Instruction,
    Wr3Instruction,
    WrmInstruction,
    WrrInstruction,
    XchInstruction
};

enum InstructionTypes
{
    Implied,
    Immediate4Bit,
    Register,
    RegisterAnd8BitAddress,
    RegisterPair,
    RegisterPairAnd8BitImmediate,
    ConditionAnd8BitAddress,
    Address12Bit
};

enum GetExpressionStates
{
    GetExpressionCheckForExpression = 0,
    GetExpressionCheckForComma
};

//------------------------------------------------------------------------------
//Structures
typedef struct tagDirectiveInfo
{
    uint32_t m_iIndex;
    char* m_cpDirective;
} DirectiveInfo;

typedef struct tagExpressionInfo
{
    char* m_cpStart;
    int m_iValue;
} ExpressionInfo;

typedef struct tagOpcodeInfo
{
    uint8_t m_iOpCode;
    uint8_t m_iType;
    uint8_t m_iNumberOfOperands;
    uint8_t m_iLength;
} OpcodeInfo;

typedef struct tagInstructionInfo
{
    uint32_t m_iIndex;
    char* m_cpMnemonic;
    uint8_t m_iNumberOfTypes;
    const OpcodeInfo* m_pOpcodeInfo;
} InstructionInfo;

typedef struct tagLexerInfo
{
    char* m_cpStartOfLabel;
    char* m_cpEndOfLabel;
    char* m_cpStartOfStatement;
    char* m_cpEndOfStatement;
    char* m_cpStartOfComment;
    char* m_cpEndOfComment;
    char* m_cpStatementSymbolStart;
    char* m_cpStatementSymbolEnd;
    char* m_cpStatementMnemonicStart;
    char* m_cpStatementMnemonicEnd;
    char* m_cpStatementExpresionStart;
    uint8_t m_iPass;
    uint8_t m_iaProgramMemory[MAX_PROGRAM_MEMORY];
    uint32_t m_iLargestSymbolLength;
    uint32_t m_iLargestOperandLength;
    uint32_t m_iSourceLineLength;
    uint32_t m_iSourceLineNumber;
    uint32_t m_iLocationCounter;
} LexerInfo;

//------------------------------------------------------------------------------
//Prototypes
int do_assembly(void);
void free_lexer_memory(void);
uint32_t get_location_counter_value(void);
BSTreeNode* get_symbol_table_root(void);

#endif /*___LEXER_H___*/
