#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <unordered_map>
#include <memory>

struct Flags
{
	uint16_t carry_ : 1;
	uint16_t parity_ : 1;
	uint16_t aux_carry_ : 1;
	uint16_t zero_ : 1;
	uint16_t sign_ : 1;
	uint16_t overflow_ : 1;
	uint16_t interrupt_enable_ : 1;
	uint16_t direction_ : 1;
	uint16_t trap_ : 1;
	uint16_t pad_ : 7;
};

class CpuState
{
private:
	
	//TODO when working with w == 0, use mod to get the correct register, then use reg to determine if high or low
	
	//registers AX, CX, DX, BX, SP, BP, SI, DI
	std::array<uint16_t, 8> registers_;

	uint16_t ip_;

	Flags flags_;
	
	//segment registers CS, DS, SS, ES
	std::array<uint16_t, 4> segment_registers_;

	std::unique_ptr<std::array<uint8_t, 0xF4240>> memory_; //1MB of memory
	//disassembler helper arrays

	std::array<std::array<std::string, 2>, 8> dis_registers_ =
	{
		{
			{"AL", "AX"},
			{"CL", "CX"},
			{"DL", "DX"},
			{"BL", "BX"},
			{"AH", "SP"},
			{"CH", "BP"},
			{"DH", "SI"},
			{"BH", "DI"}
		}
	};


	std::array<std::string, 8> dis_memory_addresses_ =
	{
		"BX+SI",
		"BX+DI",
		"BP+SI",
		"BP+DI",
		"SI",
		"DI",
		"BP",
		"BX"
	};

	std::unordered_map<uint8_t, std::string> dis_arithmetic_imm_reg_operations_ =
	{
		{0x00, "ADD"},
		{0x02, "ADC"},
		{0x05, "SUB"},
		{0x03, "SBB"},
		{0x07, "CMP"}
	};
	
	void SetFlags(const uint16_t val);
	bool CheckParity(const uint16_t val) const;

	//ops
	////MOV Register/memory to/from register
	void MovRegMemToFromReg(const uint8_t d, const uint8_t w);
	////MOV immediate to register/memory
	void MovImmToRegMem(const uint8_t w);
	////MOV immediate to register
	void MovImmToReg(const uint8_t w, const uint8_t reg);
	////MOV memory to accumulator
	void MovMemToAcc(const uint8_t w);
	////MOV accumulator to memory
	void MovAccToMem(const uint8_t w);

	//// MovRegMemToFromReg subfunctions
	void MovRmtfgMem(const uint8_t d, const uint8_t w, const uint8_t src,  const uint8_t dst);
	void MovRmtfgMemDisp8(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void MovRmtfgMemDisp16(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void MovRmtfgReg(const uint8_t src, const uint8_t dst, const uint8_t w);
	//// MovImmToRegMem subfunctions
	void MovItrmMem(const uint8_t w, const uint8_t dst);
	void MovItrmMemDisp8(const uint8_t w, const uint8_t dst);
	void MovItrmMemDisp16(const uint8_t w, const uint8_t dst);
	void MovItrmReg(uint8_t dst, uint8_t w);

	////ADD reg/memory with register to either
	void AddRegMemWithRegToEither(const uint8_t d, const uint8_t w);

	//// AddRegMemWithRegToEither subfunctions
	void AddRmwrtwMem(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void AddRmwrtwMemDisp8(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void AddRmwrtwMemDisp16(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void AddRmwrtwReg(const uint8_t src, const uint8_t dst, const uint8_t w);

	//// ADD/ADC/SUB/SBC/CMP immediate to register/memory
	void AddAdcSubSbcCmpImmToRegMem(const uint8_t s, const uint8_t w);

	//// AddAdcSubSbcCmpImmToRegMem subfunctions
	void AddAdcSubSbcCmpItrmMem(const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith);
	void AddAdcSubSbcCmpItrmMemDisp8(const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith);
	void AddAdcSubSbcCmpItrmMemDisp16(const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith);
	void AddAdcSubSbcCmpItrmReg(const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith);

	//// ADD immediate to accumulator
	void AddImmToAcc(const uint8_t w);

	//// SUB reg/memory with register to either
	void SubRegMemWithRegToEither(const uint8_t d, const uint8_t w);

	//// SubRegMemWithRegToEither subfunctions
	void SubRmwrtwMem(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void SubRmwrtwMemDisp8(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void SubRmwrtwMemDisp16(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void SubRmwrtwReg(const uint8_t src, const uint8_t dst, const uint8_t w);

	//// SUB immediate to accumulator
	void SubImmToAcc(const uint8_t w);

	//// CMP reg / memory and register
	void CmpRegMemAndReg(const uint8_t d, const uint8_t w);

	//// CmpRegMemAndReg subfunctions
	void CmpRmarMem(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void CmpRmarMemDisp8(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void CmpRmarMemDisp16(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst);
	void CmpRmarReg(const uint8_t src, const uint8_t dst, const uint8_t w);

	//// CMP immediate to register
	void CmpImmWithAcc(const uint8_t w);

	//// JNE/JNZ
	void JneJnz();

	//// JE/JZ
	void JeJz();

	//// JL/JNGE
	void JlJnge();

	//// JLE/JNG
	void JleJng();

	//// JB/JNAE
	void JbJnae();

	//// JBE/JNA
	void JbeJna();

	//// JP/JPE
	void JpJpe();

	//// JO
	void Jo();

	//// JS
	void Js();

	//// JNL/JGE
	void JnlJge();

	//// JNLE/JG
	void JnleJg();

	//// JNB/JAE
	void JnbJae();

	//// JNBE/JA
	void JnbeJa();

	//// JNP/JPO
	void JnpJpo();

	//// JNO
	void Jno();

	//// JNS
	void Jns();

	//// LOOP
	void Loop();

	//// LOOPZ/LOOPE
	void LoopzLoope();

	//// LOOPNZ/LOOPNE
	void LoopnzLoopne();

	//// JCXZ
	void Jcxz();
public:
	CpuState();
	~CpuState();
	void PrintRegisters() const;
	void PrintFlags() const;
	void DisassembleInstruction(const std::string_view instruction);
	uint8_t DecodeInstruction();

    std::array<uint8_t, 0xF4240>& GetMemory();
	uint16_t GetIp() const;
};
