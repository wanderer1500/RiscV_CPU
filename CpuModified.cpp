
#include <bits/stdc++.h>
using namespace std;
#define all(p) p.begin(), p.end()
/**************************/
// const int N=1024;
int N;
int pc = 0;
/*******************/
vector<string> assembly;
class Register
{
public:
    string regName;
    int regNo;
    string regNo_bin;
    Register(string name)
    {
        this->regName = name;
        regNo = stoi(regName.substr(1, regName.length()));
        regNo_in_bin();
    }

private:
    void regNo_in_bin()
    {
        bitset<5> num(regNo);
        regNo_bin = num.to_string();
    }
};
/*****************************************************/
class Immediate
{
public:
    string hexNo;
    string hex_to_bin;
    string imm7bits;
    string imm5bits;
    string jump;
    string upImbits;

    Immediate(string hex)
    {
        this->hexNo = hex;
        hexTobin(hex);
        hexTobin_5bit();
        hexTobin_7bit();
        upperIm();
    }

private:
    void hexTobin(string hex)
    {
        string ans = "";
        /*map for hex to binary conversion*/
        map<char, string> mp = {
            {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}};
        for (int i = 2; i < hex.size(); i++)
        {
            ans += mp[hex[i]];
        }
        bitset<12> bits(ans);     // 12 bits
        bitset<20> bits_lui(ans); // upper immediate and j type
        this->jump = bits_lui.to_string();
        this->hex_to_bin = bits.to_string();
        this->upImbits = bits_lui.to_string();
    }
    void hexTobin_7bit()
    {
        this->imm7bits = hex_to_bin.substr(0, 7);
    }
    void hexTobin_5bit()
    {
        this->imm5bits = hex_to_bin.substr(7, 5);
    }
    void upperIm()
    {
        while (upImbits.size() != 32)
        {
            upImbits = upImbits + '0';
        }
    }
};
/*S type*/
class Stype_instruction
{
public:
    string imm;
    string rs2;
    string rs1;
    string func3;
    string opcode;
    Stype_instruction(string opcode, string rs2, string rs1, string func3, string imm)
    {
        this->imm = imm;
        this->rs1 = rs1;
        this->rs2 = rs2;
        this->opcode = opcode;
        this->func3 = func3;
    }
    string Stype_to_binary()
    {
        Register Rs1(rs1);
        Register Rs2(rs2);
        Immediate Imm(imm);
        string ans = Imm.imm7bits + Rs2.regNo_bin + Rs1.regNo_bin + func3 + Imm.imm5bits + opcode;
        return ans;
    }
};
/*R type*/
class Btype_instruction
{
public:
    string opcode;
    string rs1;
    string rs2;
    string imm;
    string func3;

    Btype_instruction(string opcode, string rs1, string rs2, string imm, string func3)
    {
        this->opcode = opcode;
        this->rs1 = rs1;
        this->rs2 = rs2;
        this->imm = imm;
        this->func3 = func3;
    }
    string Btype_to_binary()
    {
        Register Rs1(rs1);
        Register Rs2(rs2);
        Immediate Im(imm);
        string ans = Im.imm7bits + Rs2.regNo_bin + Rs1.regNo_bin + func3 + Im.imm5bits + opcode;
        return ans;
    }
};
/* L type*/
class Ltype_instruction
{
public:
    string opcode;
    string rs1;
    string rd;
    string imm;
    string func3;
    Ltype_instruction(string opcode, string rs1, string rd, string imm, string func3)
    {
        this->opcode = opcode;
        this->rs1 = rs1;
        this->rd = rd;
        this->imm = imm;
        this->func3 = func3;
    }

    string Ltype_to_binary()
    {
        Register Rs1(rs1);
        Register Rd(rd);
        Immediate Im(imm);
        string ans = Im.hex_to_bin + Rs1.regNo_bin + func3 + Rd.regNo_bin + opcode;
        return ans;
    }
};
/*I type */
class Itype_instruction
{
public:
    string opcode;
    string rs1;
    string imm;
    string rd;
    string func3;
    string func7;
    Itype_instruction(string opcode, string rs1, string imm, string rd, string func3, string func7)
    {
        this->opcode = opcode;
        this->rs1 = rs1;
        this->imm = imm;
        this->rd = rd;
        this->func3 = func3;
        this->func7 = func7;
    }
    string Itype_to_binary()
    {
        Register Rs1(rs1);
        Register Rd(rd);
        Immediate Im(imm);
        string ans = Im.hex_to_bin + Rs1.regNo_bin + func3 + Rd.regNo_bin + opcode;
        return ans;
    }
    string IRtype_to_binary()
    {
        Register Rs1(rs1);
        Register Rd(rd);
        Immediate Im(imm);
        string ans = func7 + Im.imm5bits + Rs1.regNo_bin + func3 + Rd.regNo_bin + opcode;
        return ans;
    }
};
/*R type*/
class Rtype_instruction
{
public:
    string opcode;
    string rs1;
    string rs2;
    string rd;
    string func3;
    string func7;
    Rtype_instruction(string opcode, string rs1, string rs2, string rd, string func3, string func7)
    {
        this->opcode = opcode;
        this->rs1 = rs1;
        this->rs2 = rs2;
        this->rd = rd;
        this->func3 = func3;
        this->func7 = func7;
    }
    string Rtype_to_binary()
    {
        Register Rd(rd);
        Register Rs1(rs1);
        Register Rs2(rs2);
        string ans = "";
        ans = func7 + Rs2.regNo_bin + Rs1.regNo_bin + func3 + Rd.regNo_bin + opcode;
        return ans;
    }
};
class Jtype_instruction
{
public:
    string rd;
    string rs1;
    string opcode;
    string func3;
    string imm;
    Jtype_instruction(string opcode, string rs1, string rd, string imm, string func3)
    {

        this->rs1 = rs1;
        this->rd = rd;
        this->opcode = opcode;
        this->imm = imm;
        this->func3 = func3;
    }
    string Jtype_to_binary()
    {
        Register Rd(rd);
        Immediate Im(imm);
        string ans = Im.jump + Rd.regNo_bin + opcode;
        return ans;
    }
    string JL_type_to_binary()
    {
        Register Rd(rd);
        Register Rs1(rs1);
        Immediate Im(imm);
        string ans = Im.hex_to_bin + Rs1.regNo_bin + func3 + Rd.regNo_bin + opcode;
        return ans;
    }
};

class Instruction_Convertor
{
public:
    map<string, vector<string>> instruction_map = {
        {"ADD", {"R", "000", "0000000"}},
        {"SUB", {"R", "000", "0100000"}},
        {"AND", {"R", "111", "0000000"}},
        {"OR", {"R", "110", "0000000"}},
        {"XOR", {"R", "100", "0000000"}},
        {"SLL", {"R", "001", "0000000"}},
        {"SRL", {"R", "101", "0100000"}},
        {"SLT", {"R", "010", "0000000"}},
        {"SLTU", {"R", "011", "0000000"}},
        {"ADDI", {"I", "000", ""}},
        {"SLLI", {"IL", "001", "0000000"}},
        {"SLTI", {"IL", "010", "0000000"}},
        {"SLLTU", {"IL", "011", "0000000"}},
        {"XORI", {"I", "100", ""}},
        {"SRLI", {"IR", "101", "0100000"}},
        {"SRAI", {"IR", "101", "0100000"}},
        {"ORI", {"I", "110", ""}},
        {"ANDI", {"I", "111", ""}},
        {"LB", {"L", "000", ""}},
        {"LH", {"L", "001", ""}},
        {"LW", {"L", "010", ""}},
        {"LBU", {"L", "100", ""}},
        {"LHU", {"L", "101", ""}},
        {"BEQ", {"B", "000", ""}},
        {"BNE", {"B", "001", ""}},
        {"BLT", {"B", "100", ""}},
        {"BGE", {"B", "101", ""}},
        {"SB", {"S", "000", ""}},
        {"SH", {"S", "001", ""}},
        {"SW", {"S", "010", ""}},
        {"SD", {"S", "011", ""}},
        {"JAL", {"J", "", ""}},
        {"JALR", {"JL", "000", ""}}

    };
    map<string, string> opCode_from_Type = {
        {"R", "0110011"},
        {"I", "0010011"},
        {"IL", "0010011"},
        {"IR", "0010011"},
        {"L", "0000011"},
        {"B", "1100011"},
        {"S", "0100011"},
        {"J", "1101111"},
        {"JL", "1100111"}

    };
    string conversion(string s)
    {
        stringstream ss(s);
        string opCode;
        ss >> opCode;
        if (instruction_map.find(opCode) == instruction_map.end())
        {
            return "Invalid";
        }
        string type = instruction_map[opCode][0];
        string func3 = instruction_map[opCode][1];
        string func7 = instruction_map[opCode][2];
        if (type == "S")
        {
            opCode = opCode_from_Type[type];
            string rs2, temp, imm, rs1;
            imm = "";
            rs1 = "";
            ss >> rs2 >> temp;
            rs2.pop_back();
            int ind = -1;
            for (int i = 0; i < temp.size(); i++)
            {
                if (temp[i] == '[')
                {
                    ind = i;
                    break;
                }
                imm += temp[i];
            }
            rs1 = temp.substr(ind + 1, temp.length());
            rs1.pop_back();
            Stype_instruction S1(opCode, rs2, rs1, func3, imm);
            string ans = S1.Stype_to_binary();
            return ans;
        }
        else if (type == "B")
        {
            opCode = opCode_from_Type[type];
            string rs2, imm, rs1;
            ss >> rs1 >> rs2 >> imm;
            rs1.pop_back();
            rs2.pop_back();
            Btype_instruction B1(opCode, rs1, rs2, imm, func3);
            string ans = B1.Btype_to_binary();
            return ans;
        }
        else if (type == "L")
        {
            opCode = opCode_from_Type[type];
            string rd, temp;
            ss >> rd >> temp;
            string rs1, imm;
            imm = "";
            int ind = -1;
            for (int i = 0; i < temp.size(); i++)
            {
                if (temp[i] == '[')
                {
                    ind = i;
                    break;
                }
                imm += temp[i];
            }
            rs1 = temp.substr(ind + 1, temp.length());
            rs1.pop_back();
            Ltype_instruction L1(opCode, rs1, rd, imm, func3);
            string ans = L1.Ltype_to_binary();
            return ans;
        }
        else if (type == "IR")
        {
            string rd, rs1, imm;
            ss >> rd >> rs1 >> imm;
            rd.pop_back();
            rs1.pop_back();
            opCode = opCode_from_Type[type];
            Itype_instruction I1(opCode, rs1, imm, rd, func3, func7);

            string ans = I1.IRtype_to_binary();
            return ans;
        }
        else if (type == "I")
        {
            string rd, rs1, imm;
            ss >> rd >> rs1 >> imm;
            rd.pop_back();
            rs1.pop_back();
            opCode = opCode_from_Type[type];
            Itype_instruction I1(opCode, rs1, imm, rd, func3, func7);

            string ans = I1.Itype_to_binary();
            return ans;
        }
        else if (type == "R")
        {
            string rs1, rs2, rd;
            ss >> rd >> rs1 >> rs2;
            rd.pop_back();
            rs1.pop_back();
            opCode = opCode_from_Type[type];
            Rtype_instruction R1(opCode, rs1, rs2, rd, func3, func7);

            string ans = R1.Rtype_to_binary();
            return ans;
        }
        else if (type == "J")
        {
            string rd, imm;
            ss >> rd >> imm;
            rd.pop_back();
            opCode = opCode_from_Type[type];
            Jtype_instruction J_Type(opCode, "", rd, imm, func3);
            return J_Type.Jtype_to_binary();
        }
        else if (type == "JL")
        {
            string rd, rs1, imm;
            ss >> rd >> rs1 >> imm;
            rd.pop_back();
            rs1.pop_back();
            opCode = opCode_from_Type[type];
            Jtype_instruction J_Type(opCode, rd, func3, rs1, imm);
            return J_Type.Jtype_to_binary();
        }
        return "Invalid";
    }
};

/*********************/

class StringParser
{
public:
    string str = "";
    StringParser(string syntax)
    {
        this->str = syntax;
    }
    vector<string> convertor()
    {
        string to_machine_code = "";
        vector<string> vec;

        for (auto s : str)
        {
            if (s == ';')
            {
                Instruction_Convertor C1;
                //   cout<<C1.conversion(to_machine_code)<<"\n";
                vec.push_back(C1.conversion(to_machine_code));
                assembly.push_back(to_machine_code);
                to_machine_code = "";
            }
            else
            {
                to_machine_code += s;
            }
        }
        return vec;
    }
};
// CPU
/****************************************/
vector<string> IM;
// vector<string> RF(N);
vector<int> GPR(32);

vector<int> DM(1024);

/******************stall logic ************************/

int GPR_in_use[33] = {0};
bool stage_activation[6] = {false};

bool stall_1 = false;
bool stall_2 = false;

bool data_bubble = false;
bool control_bubble = false;
/******************************************************/
int to_int(string binaryStr)
{
    int decimalValue = 0;
    int length = binaryStr.length();

    // Handle empty string or invalid length
    if (length == 0)
    {
        return 0;
    }

    // Get the sign bit
    bool isNegative = (binaryStr[0] == '1');

    if (!isNegative)
    {
        // Positive number: simply convert from binary to decimal
        for (int i = 0; i < length; i++)
        {
            if (binaryStr[i] == '1')
            {
                decimalValue = (decimalValue << 1) | 1;
            }
            else if (binaryStr[i] == '0')
            {
                decimalValue <<= 1;
            }
            else
            {
                // Handle invalid characters
                return 0;
            }
        }
    }
    else
    {
        // Negative number: use two's complement
        // First, flip all bits
        for (int i = 0; i < length; i++)
        {
            if (binaryStr[i] == '1')
            {
                decimalValue = (decimalValue << 1) | 0;
            }
            else if (binaryStr[i] == '0')
            {
                decimalValue = (decimalValue << 1) | 1;
            }
            else
            {
                // Handle invalid characters
                return 0;
            }
        }
        // Add 1 to complete two's complement and make it negative
        decimalValue = -(decimalValue + 1);
    }

    return decimalValue;
}

string to_str(int num)
{
    // If number is 0, return "0"
    if (num == 0)
    {
        return "0";
    }

    string result;
    bool isNegative = (num < 0);

    // If negative, convert to positive and handle separately
    if (isNegative)
    {
        // Handle INT_MIN specially as its positive value can't be represented
        if (num == INT_MIN)
        {
            return "10000000000000000000000000000000";
        }
        num = -num;
    }

    // Convert absolute value to binary
    while (num > 0)
    {
        result = char('0' + (num & 1)) + result;
        num >>= 1;
    }

    // If number is negative, convert to two's complement
    if (isNegative)
    {
        // Pad with zeros to make it 32 bits
        while (result.length() < 32)
        {
            result = '0' + result;
        }

        // Flip all bits
        for (int i = 0; i < 32; i++)
        {
            result[i] = (result[i] == '0') ? '1' : '0';
        }

        // Add 1 to complete two's complement
        int carry = 1;
        for (int i = 31; i >= 0; i--)
        {
            if (result[i] == '1' && carry == 1)
            {
                result[i] = '0';
            }
            else if (result[i] == '0' && carry == 1)
            {
                result[i] = '1';
                carry = 0;
            }
        }
    }

    // Ensure positive numbers are padded to 32 bits with leading zeros
    if (!isNegative)
    {
        while (result.length() < 32)
        {
            result = '0' + result;
        }
    }

    return result;
}

int signedExtend(string imm)
{
    string extended_imm = imm;
    if (imm[0] == '0')
    {
        while (extended_imm.size() < 32)
        {
            extended_imm = "0" + extended_imm;
        }
    }
    else
    {
        while (extended_imm.size() < 32)
        {
            extended_imm = "1" + extended_imm;
        }
    }
    return to_int(extended_imm);
}

// Control word structure remains the same
struct controlWord
{
    int RegRead, RegWrite, AluSrc, AluOp, Branch, Jump, MemRead, MemWrite, Mem2Reg;
};

// Control unit mapping remains the same
map<string, controlWord> controlUnit;

// Control unit initialization function restructured
void initControlUnit()
{
    // Define instruction types
    const struct
    {
        string opcode;
        string type;
        controlWord signals;
    } instructionSet[] = {
        // R-type arithmetic instructions
        {"0110011", "R-type", {1, 1, 0, 10, 0, 0, 0, 0, 0}},

        // I-type immediate arithmetic
        {"0010011", "I-type", {1, 1, 1, 11, 0, 0, 0, 0, 0}},

        // I-type load instructions
        {"0000011", "I-type Load", {1, 1, 1, 0, 0, 0, 1, 0, 1}},

        // S-type store instructions
        {"0100011", "S-type", {1, 0, 1, 0, 0, 0, 0, 1, -1}},

        // B-type branch instructions
        {"1100011", "B-type", {1, 0, 0, 1, 1, 0, 0, 0, -1}},

        // U-type upper immediate
        {"0110111", "U-type", {0, 1, 1, 0, 0, 0, 0, 0, 0}},

        // J-type jump instructions
        {"1101111", "J-type", {0, 1, 0, 0, 0, 1, 0, 0, 0}}};

    // Initialize control unit map
    for (const auto &inst : instructionSet)
    {
        controlUnit[inst.opcode] = inst.signals;
    }
}

// Alternative implementation method using initialization function
void initControlUnit_alt()
{
    // Helper function to create control word
    auto makeControl = [](int rr, int rw, int as, int ao, int b, int j, int mr, int mw, int m2r)
    {
        return controlWord{rr, rw, as, ao, b, j, mr, mw, m2r};
    };

    // Initialize each instruction type
    controlUnit = {
        {"0110011", makeControl(1, 1, 0, 10, 0, 0, 0, 0, 0)}, // R-type
        {"0010011", makeControl(1, 1, 1, 11, 0, 0, 0, 0, 0)}, // I-type
        {"0000011", makeControl(1, 1, 1, 0, 0, 0, 1, 0, 1)},  // I-type Load
        {"0100011", makeControl(1, 0, 1, 0, 0, 0, 0, 1, -1)}, // S-type
        {"1100011", makeControl(1, 0, 0, 1, 1, 0, 0, 0, -1)}, // B-type
        {"0110111", makeControl(0, 1, 1, 0, 0, 0, 0, 0, 0)},  // U-type
        {"1101111", makeControl(0, 1, 0, 0, 0, 1, 0, 0, 0)}   // J-type
    };
}
/*************************************************************/
void initDM()
{

    DM[0] = 10;
}

class IFID
{
public:
    int DPC;
    int NPC;
    string IR;
};

/*****************************************************************/
class IDEX
{
public:
    class CW
    {
    public:
        int RegRead, RegWrite, AluSrc, AluOp, Branch, Jump, MemRead, MemWrite, Mem2Reg;
        void controller(string str)
        {
            controlWord c = controlUnit[str];
            this->RegRead = c.RegRead;
            this->RegWrite = c.RegWrite;
            this->AluSrc = c.AluSrc;
            this->AluOp = c.AluOp;
            this->Branch = c.Branch;
            this->Jump = c.Jump;
            this->MemRead = c.MemRead;
            this->MemWrite = c.MemWrite;
            this->Mem2Reg = c.Mem2Reg;
        }
    };

public:
    string imm1, imm2, func, IR;
    string rs1, rs2, rdl;
    int JPC, DPC;
    CW cw;
};
/****************************************************************/
class EXMO
{
public:
    class CW
    {
    public:
        int RegRead, RegWrite, AluSrc, AluOp, Branch, Jump, MemRead, MemWrite, Mem2Reg;
        void Transfer_CW(IDEX &idex)
        {

            this->RegRead = idex.cw.RegRead;
            this->RegWrite = idex.cw.RegWrite;
            this->AluSrc = idex.cw.AluSrc;
            this->AluOp = idex.cw.AluOp;
            this->Branch = idex.cw.Branch;
            this->Jump = idex.cw.Jump;
            this->MemRead = idex.cw.MemRead;
            this->MemWrite = idex.cw.MemWrite;
            this->Mem2Reg = idex.cw.Mem2Reg;
        }
    };

public:
    int AluOut;
    string rdl, rs2;
    CW cw;
};

/***************************************************************************/
class MOWB
{
public:
    class CW
    {
    public:
        int RegRead, RegWrite, AluSrc, AluOp, Branch, Jump, MemRead, MemWrite, Mem2Reg;
        void Transfer_CW(EXMO &exmo)
        {

            this->RegRead = exmo.cw.RegRead;
            this->RegWrite = exmo.cw.RegWrite;
            this->AluSrc = exmo.cw.AluSrc;
            this->AluOp = exmo.cw.AluOp;
            this->Branch = exmo.cw.Branch;
            this->Jump = exmo.cw.Jump;
            this->MemRead = exmo.cw.MemRead;
            this->MemWrite = exmo.cw.MemWrite;
            this->Mem2Reg = exmo.cw.Mem2Reg;
        }
    };

public:
    CW cw;
    int AluOut, LDOUT;
    string rdl;
};

/******************Hazard Detection *******************************************/
void Hazard_Identitify(string CW_stage_3, string RD_stage_4, string RS1_stage_3, string RS2_stage_3)
{
    // Helper function to convert binary string to integer
    auto bin_to_int = [](const string &bin_str)
    {
        return stoi(bin_str, nullptr, 2);
    };

    // Helper lambda for checking if RD_stage_4 matches any of RS1 or RS2
    auto is_rd_equal_to_rs = [&](const string &RD_stage, const string &RS1, const string &RS2)
    {
        return RD_stage == RS1 || RD_stage == RS2;
    };

    // Data hazard types
    if ((CW_stage_3 == "0110011" || CW_stage_3 == "1100011") && is_rd_equal_to_rs(RD_stage_4, RS1_stage_3, RS2_stage_3))
    {
        data_bubble = true;
        int reg_idx = bin_to_int(RD_stage_4);
        if (reg_idx >= 1 && reg_idx <= 33)
        {
            GPR_in_use[reg_idx] = 1; // Mark the register as in use (1-based index)
        }
        cout << "Data bubble inserted" << endl;
        cout << "Inserted at R/B type" << endl;
    }

    // Load/Store hazard types
    if ((CW_stage_3 == "0010011" || CW_stage_3 == "0000011") && RD_stage_4 == RS1_stage_3)
    {
        data_bubble = true;
        int reg_idx = bin_to_int(RD_stage_4);
        if (reg_idx >= 1 && reg_idx <= 33)
        {
            GPR_in_use[reg_idx] = 1; // Mark the register as in use (1-based index)
        }
        cout << "Data bubble inserted" << endl;
        cout << "Inserted at load/store" << endl;
    }

    // Control hazard types
    if (CW_stage_3 == "1100011" || CW_stage_3 == "1101111")
    {
        control_bubble = true;
        cout << "Control bubble inserted" << endl;
    }
}
/************************************************** */

void IF(IFID &ifid)
{
    if (data_bubble || control_bubble)
    {
        cout << "stopped due to bubble insertion" << endl;
        return;
    } // Added
    /*********Modified***************/
    if (pc < N * 4)
    {
        int idx = pc / 4;
        ifid.IR = IM[idx];
        ifid.DPC = pc;
        ifid.NPC = pc + 4;
        pc = pc + 4;

        stage_activation[2] = true;
    }
    else
    {

        stage_activation[1] = false;
    }
}
/***********************************************************************/
void ID(IDEX &idex, IFID &ifid, EXMO &exmo)
{   // exmo also shared in ID
    /*********Added**********/
    if (!stage_activation[1])
    {
        stage_activation[2] = false;
        return;
    }
    if (stall_1)
    {
        stall_1 = false;
        return;
    }
    if (data_bubble || control_bubble)
    {
        cout << "stage2 stopped due to bubble insertion" << endl;
        return;
    }
    /********************/

    string ir = ifid.IR; // Keep ir as it is, without reversing

    // Extract substrings directly as in the working code
    string str[7] = {""};
    str[1] = ir.substr(0, 20);
    str[2] = ir.substr(0, 12);
    str[3] = ir.substr(0, 7) + ir.substr(20, 5);
    str[4] = ir.substr(25, 7);
    str[5] = ir.substr(17, 3);
    str[6] = ir.substr(20, 5);

    // Assign values to idex fields, similar to the working code
    idex.JPC = ifid.DPC + 4 * signedExtend(str[1]);
    idex.DPC = ifid.DPC;
    idex.imm1 = str[2];
    idex.imm2 = str[3];
    idex.cw.controller(str[4]);
    idex.func = str[5];
    idex.rdl = str[6];
    idex.IR = ifid.IR;
    /************removed */
    // if(idex.cw.RegRead){
    //     idex.rs1=to_str(GPR[to_int(ir.substr(12,5))]);
    // }
    // if(idex.cw.AluSrc && (ir.substr(25,7)=="0010011" || ir.substr(25,7)=="0000011")){
    //     if(idex.cw.RegRead){
    //         idex.rs2=idex.imm1;
    //     }

    // }else{
    //     if(idex.cw.RegRead){
    //         idex.rs2=to_str(GPR[to_int(ir.substr(7,5))]);
    //     }
    // }
    /******************Added */
    stage_activation[3] = true;

    Hazard_Identitify(idex.IR.substr(25, 7), exmo.rdl, ir.substr(12, 5), ir.substr(7, 5));
}

string AluControlUnit(int AluOp, string func, string func7)
{
    string AluSelect;
    if (AluOp == 00)
    {
        AluSelect = "0010";
    }
    else if (AluOp == 01)
    {
        if (func == "000")
        {
            AluSelect = "0110";
        }
        else if (func == "001")
        {
            AluSelect = "0110";
        }
        else
        {
            AluSelect = "1111";
        }
    }

    else if (AluOp == 10)
    {
        if (func == "000")
        {

            if (func7 == "0000000")
            {
                AluSelect = "0010";
            }
            else
            {
                AluSelect = "0110";
            }
        }
        else if (func == "001")
        {
            AluSelect = "0011";
        }
        else if (func == "111")
        {
            AluSelect = "0000";
        }
        else if (func == "110")
        {
            AluSelect = "0001";
        }
        else
        {
            AluSelect = "1111";
        }
    }

    else if (AluOp == 11)
    {
        if (func == "000")
        {
            AluSelect = "0010";
        }
        else if (func == "010")
        {
            AluSelect = "0111";
        }
        else if (func == "100")
        {
            AluSelect = "0011";
        }
        else
        {
            AluSelect = "1111";
        }
    }
    else
    {
        AluSelect = "1111";
    }
    return AluSelect;
}

// Function to perform various ALU operations based on the AluSelect signal
int Alu(string AluSelect, string Rs1, string Rs2)
{
    // Convert string inputs to integer values
    int rs1 = to_int(Rs1);
    int rs2 = to_int(Rs2);
    int result = 0;

    // Perform operation based on the AluSelect control signal
    if (AluSelect == "0000")
    {
        // Bitwise AND
        result = rs1 & rs2;
    }
    else if (AluSelect == "0001")
    {
        // Bitwise OR
        result = rs1 | rs2;
    }
    else if (AluSelect == "0010")
    {
        // Addition
        result = rs1 + rs2;
    }
    else if (AluSelect == "0110")
    {
        // Subtraction
        result = rs1 - rs2;
    }
    else if (AluSelect == "0011")
    {
        // Logical left shift (only use lower 5 bits of rs2 for shift amount)
        result = rs1 << (rs2 & 0x1F);
    }
    else if (AluSelect == "0100")
    {
        // Logical right shift (only use lower 5 bits of rs2 for shift amount)
        result = rs1 >> (rs2 & 0x1F);
    }
    else if (AluSelect == "0111")
    {
        // Less-than comparison (sets result to 1 if rs1 < rs2, otherwise 0)
        result = (rs1 < rs2) ? 1 : 0;
    }
    else if (AluSelect == "1111")
    {
        // Invalid AluSelect code
        cout << "Error: Invalid AluSelect code." << endl;
        return -1; // Return -1 to indicate an error
    }
    else
    {
        // Handle unexpected AluSelect codes
        cout << "Error: Unknown AluSelect code." << endl;
        return -1;
    }

    return result;
}

void IE(EXMO &exmo, IDEX &idex, IFID &ifid)
{
    /***************** Stage Activation Check *****************/
    if (!stage_activation[2])
    {
        stage_activation[3] = false; // Disable next stage if current stage isn't active
        return;
    }

    if (data_bubble)
    {
        cout << "Stage 3 stopped due to bubbleInserted" << endl;
        return;
    }

    if (stall_1)
    {
        cout << "Stopped for one cycle due to stall_1" << endl;
        return;
    }

    // Fetch the instruction from the IDEX register
    string ir = idex.IR;

    /***************** Register Read Handling *****************/
    if (idex.cw.RegRead)
    {
        // If RegRead is enabled, get the value of rs1 (source register 1)
        string rs1_reg = ir.substr(12, 5); // Extract rs1 register index (5 bits)
        cout << "rs1_reg: " << rs1_reg << " => " << "GPR[" << to_int(rs1_reg) << "]: " << GPR[to_int(rs1_reg)] << endl;
        idex.rs1 = to_str(GPR[to_int(rs1_reg)]);
    }

    /***************** ALU Source Handling *****************/
    if (idex.cw.AluSrc && (ir.substr(25, 7) == "0010011" || ir.substr(25, 7) == "0000011"))
    {
        // If AluSrc is true and instruction is of type immediate, use imm1 as rs2
        if (idex.cw.RegRead)
        {
            cout << "Using imm1 as rs2: " << idex.imm1 << endl;
            idex.rs2 = idex.imm1;
        }
    }
    else
    {
        // Otherwise, use rs2 register
        if (idex.cw.RegRead)
        {
            string rs2_reg = ir.substr(7, 5); // Extract rs2 register index (5 bits)
            cout << "rs2_reg: " << rs2_reg << " => " << "GPR[" << to_int(rs2_reg) << "]: " << GPR[to_int(rs2_reg)] << endl;
            idex.rs2 = to_str(GPR[to_int(rs2_reg)]);
        }
    }

    /***************** ALU Control Unit *****************/
    string AluSelect = AluControlUnit(idex.cw.AluOp, idex.func, idex.IR.substr(0, 7));
    cout << "ALUSelect: " << AluSelect << endl;

    /***************** ALU Operation *****************/
    string opcode = idex.IR.substr(25, 7); // Extract opcode (7 bits)
    if (opcode == "0100011" || opcode == "1100011")
    {
        // For store or branch instructions, use imm2 for ALU operation
        exmo.AluOut = Alu(AluSelect, idex.rs1, idex.imm2);
    }
    else
    {
        // Otherwise, perform ALU operation with rs1 and rs2
        exmo.AluOut = Alu(AluSelect, idex.rs1, idex.rs2);
    }

    /***************** Branch and Jump Handling *****************/
    int AluZeroFlag = (idex.rs1 == idex.rs2); // Set flag if rs1 == rs2
    exmo.cw.Transfer_CW(idex);                // Transfer control word to EXMO

    if (idex.cw.Branch)
    {
        // If branch is true, check if ALU result is zero for branch condition
        if (AluZeroFlag)
        {
            cout << "Branch taken: " << "to_int(idex.imm2) * 4 + idex.DPC = "
                 << to_int(idex.imm2) * 4 + idex.DPC << endl;
            pc = to_int(idex.imm2) * 4 + idex.DPC; // Update PC for branch target
        }
        if (control_bubble)
        {
            stall_1 = true; // Insert bubble
        }
        control_bubble = false; // Reset bubble control flag
    }

    if (idex.cw.Jump)
    {
        // If jump is true, set PC to jump target
        pc = idex.JPC;
        if (control_bubble)
        {
            stall_1 = true; // Insert bubble
        }
        control_bubble = false; // Reset bubble control flag
    }

    /***************** Transfer rs2 to EXMO *****************/
    exmo.rdl = idex.rdl;
    exmo.rs2 = idex.rs2;

    /***************** Stage Activation *****************/
    stage_activation[4] = true; // Activate next stage (stage 4)
}
void MA(MOWB &mowb, EXMO &exmo, IDEX &idex)
{
    // Stage activation check
    if (!stage_activation[3])
    {
        stage_activation[4] = false;
        return;
    }

    // Debug information for ALU result
    cout << "ALU Output (exmo.AluOut): " << exmo.AluOut << endl;

    // Memory write operation
    if (exmo.cw.MemWrite)
    {
        DM[exmo.AluOut] = to_int(idex.rs2);
        cout << "Memory Write: DM[" << exmo.AluOut << "] = "
             << DM[exmo.AluOut] << endl;
    }

    // Memory read operation
    if (exmo.cw.MemRead)
    {
        mowb.LDOUT = DM[exmo.AluOut];
        cout << "Memory Read: DM[" << exmo.AluOut << "] = "
             << DM[exmo.AluOut] << endl;
    }

    // Transfer pipeline information to next stage
    mowb.AluOut = exmo.AluOut;
    mowb.cw.Transfer_CW(exmo);
    mowb.rdl = exmo.rdl;

    // Activate next pipeline stage
    stage_activation[5] = true;
}

void RW(MOWB &mowb)
{
    // Early return if stage is not active
    if (!stage_activation[4])
    {
        stage_activation[5] = false;
        return;
    }

    // Track register unlocking requirement
    bool shouldUnlock = false;

    // Process register write operation
    if (mowb.cw.RegWrite)
    {
        // Convert destination register string to integer
        int destReg;
        try
        {
            destReg = stoi(mowb.rdl, nullptr, 2);
        }
        catch (const std::exception &e)
        {
            // Handle invalid register address
            return;
        }

        // Validate register index
        if (destReg < 0 || destReg >= GPR.size())
        {
            return;
        }

        // Write data to register based on Mem2Reg signal
        if (mowb.cw.Mem2Reg)
        {
            // Load from memory
            GPR[to_int(mowb.rdl)] = mowb.LDOUT;
        }
        else
        {
            // ALU result
            GPR[to_int(mowb.rdl)] = mowb.AluOut;
        }

        // Check if register needs to be unlocked
        if (GPR_in_use[destReg] == 1)
        {
            shouldUnlock = true;
        }
    }

    // Handle register unlocking
    if (shouldUnlock)
    {
        try
        {
            int regToUnlock = stoi(mowb.rdl, nullptr, 2);
            // Clear pipeline control flags
            data_bubble = false;
            stall_1 = true;
            // Release register lock
            GPR_in_use[regToUnlock] = 0;
        }
        catch (const std::exception &e)
        {
            // Handle register conversion error
            return;
        }
    }
}

int main()
{
    // string ans=regTobin("R10");
    // cout<<ans<<"\n";
    string s1 = "ADDI x1, x0, 0x8;ADDI x2, x0, 0x7;";
    StringParser p(s1);
    vector<string> m = p.convertor();
    for (auto str : assembly)
    {
        for (auto c : str)
        {
            cout << c;
        }
        cout << "\n";
    }
    for (auto str : m)
    {
        for (auto c : str)
        {
            cout << c;
        }
        cout << "\n";
    }
    m = {
        "00000000000000000010000010000011",
        "00000000000000001000011001100011",
        "00000000000100011000000110010011",
        "00000000001100001000010101100011",
        "00000000000100000000000100010011",
        "00000000000100100000001000010011",
        "00000000000100010000001101100011",
        "00000000010100100000000110110011",
        "00000000000000100000001010110011",
        "00000000000000011000001000110011",
        "00000000000100010000000100010011",
        "11111111111111111011001101101111",
        "00000000001100000010000010100011"};
    // m = {
    //     "00000000101100110010001010000011",
    //     "00000000000100001000000010010011",
    //     "00000000000100101000001010010011",
    //     "00000000010100001000001001100011",
    //     "00000000000100010000000100110011",
    //     "00000000000100001000000010010011",
    //     "11111111111111111101000111101111",
    //     "00000000001011111010000000100011"};
    DM[11] = 11;
    stage_activation[1] = true;
    IFID ifid;
    IDEX idex;
    EXMO exmo;
    MOWB mowb;
    IM = m;
    pc = 0;
    initControlUnit();
    initDM();
    int n = m.size();
    N = n;
    // cout<<"HI"<<endl;
    while (pc < n * 4 || any_of(begin(stage_activation), end(stage_activation), [](bool active)
                                { return active; }))
    {
        if (stage_activation[5])
        {
            RW(mowb);
            cout << "stage5" << endl;
            cout << endl;
            cout << "DM[exmo.ALUOUT] " << DM[exmo.AluOut] << endl;
            cout << "to_int(mowb.rdl) " << to_int(mowb.rdl) << endl;
            cout << "GPR[to_int(mowb.rdl)] " << GPR[to_int(mowb.rdl)] << endl;
            cout << "PC " << pc << endl;
            cout << "GPR[0] " << GPR[0] << endl;
            cout << "GPR[1] " << GPR[1] << endl;
            cout << "GPR[2] " << GPR[2] << endl;
            cout << "GPR[3] " << GPR[3] << endl;
            cout << "GPR[4] " << GPR[4] << endl;
            cout << "GPR[5] " << GPR[5] << endl;
            cout << "DM[0] " << DM[0] << endl;
            cout << "DM[1] " << DM[1] << endl;
        }
        if (stage_activation[4])
        {
            MA(mowb, exmo, idex);
            cout << "stage4" << endl;
            cout << endl;
            cout << "mowb.ALUOUT " << mowb.AluOut << endl;
            cout << "mowb.LDOUT " << mowb.LDOUT << endl;
            cout << "mowb.rdl " << mowb.rdl << endl;
        }
        if (stage_activation[3])
        {
            IE(exmo, idex, ifid);
            cout << "stage3" << endl;
            cout << endl;
            cout << "ALUOUT " << exmo.AluOut << endl;
        }
        if (stage_activation[2])
        {
            cout << "stage2" << endl;
            ID(idex, ifid, exmo);

            cout << "imm1 " << idex.imm1 << endl;
            cout << "imm2 " << idex.imm2 << endl;
            cout << "func " << idex.func << endl;
            cout << "rdl " << idex.rdl << endl;
            cout << "rs1 " << idex.rs1 << endl;
            cout << "rs2 " << idex.rs2 << endl;
            cout << "JPC " << idex.JPC << endl;
            cout << "DPC " << idex.DPC << endl;
            cout << endl;
            cout << "RegRead " << idex.cw.RegRead << endl;
            cout << "RegWrite " << idex.cw.RegWrite << endl;
            cout << "ALUSrc " << idex.cw.AluSrc << endl;
            cout << "ALUOp " << idex.cw.AluOp << endl;
            cout << "Branch " << idex.cw.Branch << endl;
            cout << "Jump " << idex.cw.Jump << endl;
            cout << "MemRead " << idex.cw.MemRead << endl;
            cout << "MemWrite " << idex.cw.MemWrite << endl;
            cout << "Mem2Reg " << idex.cw.Mem2Reg << endl;
        }
        if (stage_activation[1])
        {
            cout << "stage1" << endl;
            cout << endl
                 << "Instruction: " << pc / 4 << endl;
            IF(ifid);
            cout << "IR " << ifid.IR << endl;
            cout << "DPC " << ifid.DPC << endl;
            cout << "NPC " << ifid.NPC << endl;

            cout << endl;
        }
    }

    for (int i = 0; i < 32; i++)
    {
        cout << "GPR" << "[" << i + 1 << "]" << "=" << GPR[i] << "\n";
    }

    return 0;
    assembly.clear();
}
