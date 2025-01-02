#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <vector>
#include <unordered_map>

class CpuState
{
private:
	
	//TODO when working with w == 0, use mod to get the correct register, then use reg to determine if high or low
	
	//registers AX, CX, DX, BX, SP, BP, SI, DI
	std::array<uint16_t, 8> registers_;
	
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
	

	//ops
	////MOV Register/memory to/from register
	void MovRegMemToFromReg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w);
	////MOV immediate to register/memory
	void MovImmToRegMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);
	////MOV immediate to register
	void MovImmToReg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t reg);
	////MOV memory to accumulator
	void MovMemToAcc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);
	////MOV accumulator to memory
	void MovAccToMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);

	//// MovRegMemToFromReg subfunctions
	void MovRmtfgReg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset);
	void MovRmtfgMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void MovRmtfgMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void MovRmtfgMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	//// MovImmToRegMem subfunctions
	void MovItrmMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst);
	void MovItrmMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst);
	void MovItrmMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst);
	void MovItrmReg(uint8_t dst, uint8_t w, const std::vector<uint8_t>& buffer, uint32_t& offset);

	////ADD reg/memory with register to either
	void AddRegMemWithRegToEither(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w);

	//// AddRegMemWithRegToEither subfunctions
	void AddRmwrtwMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void AddRmwrtwMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void AddRmwrtwMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void AddRmwrtwReg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset);

	//// ADD/ADC/SUB/SBC/CMP immediate to register/memory
	void AddAdcSubSbcCmpImmToRegMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w);

	//// AddAdcSubSbcCmpImmToRegMem subfunctions
	void AddAdcSubSbcCmpItrmMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith);
	void AddAdcSubSbcCmpItrmMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith);
	void AddAdcSubSbcCmpItrmMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith);
	void AddAdcSubSbcCmpItrmReg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith);

	//// ADD immediate to accumulator
	void AddImmToAcc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);

	//// SUB reg/memory with register to either
	void SubRegMemWithRegToEither(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w);

	//// SubRegMemWithRegToEither subfunctions
	void SubRmwrtwMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void SubRmwrtwMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void SubRmwrtwMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void SubRmwrtwReg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset);

	//// SUB immediate to accumulator
	void SubImmToAcc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);

	//// CMP reg / memory and register
	void CmpRegMemAndReg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w);

	//// CmpRegMemAndReg subfunctions
	void CmpRmarMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void CmpRmarMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void CmpRmarMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
	void CmpRmarReg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset);

	//// CMP immediate to register
	void CmpImmWithAcc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);

	//// JNE/JNZ
	void JneJnz(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JE/JZ
	void JeJz(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JL/JNGE
	void JlJnge(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JLE/JNG
	void JleJng(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JB/JNAE
	void JbJnae(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JBE/JNA
	void JbeJna(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JP/JPE
	void JpJpe(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JO
	void Jo(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JS
	void Js(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JNL/JGE
	void JnlJge(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JNLE/JG
	void JnleJg(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JNB/JAE
	void JnbJae(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JNBE/JA
	void JnbeJa(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JNP/JPO
	void JnpJpo(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JNO
	void Jno(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JNS
	void Jns(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// LOOP
	void Loop(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// LOOPZ/LOOPE
	void LoopzLoope(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// LOOPNZ/LOOPNE
	void LoopnzLoopne(const std::vector<uint8_t>& buffer, uint32_t& offset);

	//// JCXZ
	void Jcxz(const std::vector<uint8_t>& buffer, uint32_t& offset);
public:
	CpuState();
	~CpuState();
	void PrintRegisters();
	void DisassembleInstruction(const std::string_view instruction);
	void DecodeInstruction(const std::vector<uint8_t>& buffer, uint32_t& offset);
};
