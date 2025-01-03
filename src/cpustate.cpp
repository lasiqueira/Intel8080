#include "cpustate.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

CpuState::CpuState()
{
	registers_.fill(0);
}

CpuState::~CpuState()
{
}

void CpuState::PrintRegisters()
{
	for (size_t i = 0; i < registers_.size(); i++)
	{
		std::cout << dis_registers_[i][1] << ": " << registers_[i] << std::endl;
	}
}

void CpuState::DisassembleInstruction(const std::string_view instruction)
{
	std::cout << instruction << std::endl;
}

void CpuState::DecodeInstruction(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t opcode = buffer[offset];
	switch (opcode)
	{
	case 0x88: MovRegMemToFromReg(buffer, offset, 0, 0); break;
	case 0x89: MovRegMemToFromReg(buffer, offset, 0, 1); break;
	case 0x8A: MovRegMemToFromReg(buffer, offset, 1, 0); break;
	case 0x8B: MovRegMemToFromReg(buffer, offset, 1, 1); break;
	case 0xC6: MovImmToRegMem(buffer, offset, 0); break;
	case 0xC7: MovImmToRegMem(buffer, offset, 1); break;
	case 0xB0: MovImmToReg(buffer, offset, 0, 0x00); break;
	case 0xB1: MovImmToReg(buffer, offset, 0, 0x01); break;
	case 0xB2: MovImmToReg(buffer, offset, 0, 0x02); break;
	case 0xB3: MovImmToReg(buffer, offset, 0, 0x03); break;
	case 0xB4: MovImmToReg(buffer, offset, 0, 0x04); break;
	case 0xB5: MovImmToReg(buffer, offset, 0, 0x05); break;
	case 0xB6: MovImmToReg(buffer, offset, 0, 0x06); break;
	case 0xB7: MovImmToReg(buffer, offset, 0, 0x07); break;
	case 0xB8: MovImmToReg(buffer, offset, 1, 0x00); break;
	case 0xB9: MovImmToReg(buffer, offset, 1, 0x01); break;
	case 0xBA: MovImmToReg(buffer, offset, 1, 0x02); break;
	case 0xBB: MovImmToReg(buffer, offset, 1, 0x03); break;
	case 0xBC: MovImmToReg(buffer, offset, 1, 0x04); break;
	case 0xBD: MovImmToReg(buffer, offset, 1, 0x05); break;
	case 0xBE: MovImmToReg(buffer, offset, 1, 0x06); break;
	case 0xBF: MovImmToReg(buffer, offset, 1, 0x07); break;
	case 0xA0: MovMemToAcc(buffer, offset, 0); break;
	case 0xA1: MovMemToAcc(buffer, offset, 1); break;
	case 0xA2: MovAccToMem(buffer, offset, 0); break;
	case 0xA3: MovAccToMem(buffer, offset, 1); break;
	case 0x00: AddRegMemWithRegToEither(buffer, offset, 0, 0); break;
	case 0x01: AddRegMemWithRegToEither(buffer, offset, 0, 1); break;
	case 0x02: AddRegMemWithRegToEither(buffer, offset, 1, 0); break;
	case 0x03: AddRegMemWithRegToEither(buffer, offset, 1, 1); break;
	case 0x80: AddAdcSubSbcCmpImmToRegMem(buffer, offset, 0, 0); break;
	case 0x81: AddAdcSubSbcCmpImmToRegMem(buffer, offset, 0, 1); break;
	case 0x82: AddAdcSubSbcCmpImmToRegMem(buffer, offset, 1, 0); break;
	case 0x83: AddAdcSubSbcCmpImmToRegMem(buffer, offset, 1, 1); break;
	case 0x04: AddImmToAcc(buffer, offset, 0); break;
	case 0x05: AddImmToAcc(buffer, offset, 1); break;
	case 0x28: SubRegMemWithRegToEither(buffer, offset, 0, 0); break;
	case 0x29: SubRegMemWithRegToEither(buffer, offset, 0, 1); break;
	case 0x2A: SubRegMemWithRegToEither(buffer, offset, 1, 0); break;
	case 0x2B: SubRegMemWithRegToEither(buffer, offset, 1, 1); break;
	case 0x2C: SubImmToAcc(buffer, offset, 0); break;
	case 0x2D: SubImmToAcc(buffer, offset, 1); break;
	case 0x38: CmpRegMemAndReg(buffer, offset, 0, 0); break;
	case 0x39: CmpRegMemAndReg(buffer, offset, 0, 1); break;
	case 0x3A: CmpRegMemAndReg(buffer, offset, 1, 0); break;
	case 0x3B: CmpRegMemAndReg(buffer, offset, 1, 1); break;
	case 0x3C: CmpImmWithAcc(buffer, offset, 0); break;
	case 0x3D: CmpImmWithAcc(buffer, offset, 1); break;
	case 0x74: JeJz(buffer, offset); break;
	case 0x75: JneJnz(buffer, offset); break;
	case 0x7C: JlJnge(buffer, offset); break;
	case 0x7E: JleJng(buffer, offset); break;
	case 0x72: JbJnae(buffer, offset); break;
	case 0x76: JbeJna(buffer, offset); break;
	case 0x7A: JpJpe(buffer, offset); break;
	case 0x7B: JnpJpo(buffer, offset); break;
	case 0x70: Jo(buffer, offset); break;
	case 0x78: Js(buffer, offset); break;
	case 0x7D: JnlJge(buffer, offset); break;
	case 0x7F: JnleJg(buffer, offset); break;
	case 0x73: JnbJae(buffer, offset); break;
	case 0x77: JnbeJa(buffer, offset); break;
	case 0x71: Jno(buffer, offset); break;
	case 0x79: Jns(buffer, offset); break;
	case 0xE2: Loop(buffer, offset); break;
	case 0xE1: LoopzLoope(buffer, offset); break;
	case 0xE0: LoopnzLoopne(buffer, offset); break;
	case 0xE3: Jcxz(buffer, offset); break;

	default: std::cout << "Opcode not implemented:b0=0x" << std::hex << +opcode << std::endl; exit(1);
	}
	offset++;
}

//ops

//MOV Register/memory to/from register
void CpuState::MovRegMemToFromReg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w)
{
	uint8_t modrm = buffer[offset + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t src;
	uint8_t dst;

	if (d == 0)
	{
		src = (modrm >> 3) & 0x07;
		dst = modrm & 0x07;
	}
	else
	{
		src = modrm & 0x07;
		dst = (modrm >> 3) & 0x07;
	}
	switch (mod)
	{
	case 0x00: MovRmtfgMem(buffer, offset, d, w, src, dst); break;
	case 0x01: MovRmtfgMemDisp8(buffer, offset, d, w, src, dst); break;
	case 0x02: MovRmtfgMemDisp16(buffer, offset, d, w, src, dst); break;
	case 0x03: MovRmtfgReg(src, dst, w, offset); break;
	default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::MovRmtfgMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address;
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = buffer[offset + 2] | buffer[offset + 3] << 8;
		memory_address = std::to_string(imm);
		offset += 2;
	}
	else
	{
		memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	}
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	if (d == 0)
	{
		DisassembleInstruction("MOV [" + memory_address + "], " + reg);
	}
	else
	{
		DisassembleInstruction("MOV " + reg + ", [" + memory_address + "]");
	}
	offset++;
}

void CpuState::MovRmtfgMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = buffer[offset + 2];
	if (d == 0)
	{
		DisassembleInstruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("MOV " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 2;
}

void CpuState::MovRmtfgMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	if (d == 0)
	{
		DisassembleInstruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("MOV " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 3;
}

void CpuState::MovRmtfgReg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset)
{
	std::string reg_source = dis_registers_[src][w];
	std::string reg_dest = dis_registers_[dst][w];
	if (w == 1)
	{
		registers_[dst] = registers_[src];
	}
	else
	{
		uint8_t val = 0;
		//low byte
		if(src < 4)
		{
			val = registers_[src] & 0x00FF;
		}
		//high byte
		else
		{
			val = registers_[src % 4] & 0xFF00;
		}
		//low byte
		if (dst < 4)
		{
			registers_[dst] &= 0xFF00;
			registers_[dst] |= val;
		}
		//high byte
		else
		{
			registers_[dst % 4] &= 0x00FF;
			registers_[dst % 4] |= (val << 8);
		}
		
	}
	DisassembleInstruction("MOV " + reg_dest + ", " + reg_source);
	offset++;
}

void CpuState::MovImmToRegMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint8_t modrm = buffer[offset + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t dst = modrm & 0x07;
	switch (mod)
	{
		case 0x00: MovItrmMem(buffer, offset, w, dst); break;
		case 0x01: MovItrmMemDisp8(buffer, offset, w, dst); break;
		case 0x02: MovItrmMemDisp16(buffer, offset, w, dst); break;
		case 0x03: MovItrmReg(dst, w, buffer, offset); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::MovItrmMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst)
{
	uint16_t imm = buffer[offset + 2] | buffer[offset + 3] << 8;
	std::string memory_address = std::to_string(imm);
	std::string reg = dis_registers_[dst][w];
	DisassembleInstruction("MOV [" + memory_address + "], " + reg);
	offset += 2;
}

void CpuState::MovItrmMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint8_t disp = buffer[offset + 2];
	DisassembleInstruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	offset += 2;
}

void CpuState::MovItrmMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	DisassembleInstruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	offset += 3;
}

void CpuState::MovItrmReg(uint8_t dst, uint8_t w, const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	DisassembleInstruction("MOV [" + memory_address + "], " + reg);
	offset++;
}

void CpuState::MovImmToReg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t reg)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
		registers_[reg] = imm;
	}
	else 
	{	
		//low byte
		if (reg < 4)
		{
			registers_[reg] &= 0xFF00;
			registers_[reg] |= imm;
		}
		//high byte
		else
		{
			registers_[reg % 4] &= 0x00FF;
			registers_[reg % 4] |= (imm << 8);
		}
	}
	DisassembleInstruction("MOV " + dis_registers_[reg][w] + ", " + std::to_string(imm));
	
	offset += 1 + w;
}

void CpuState::MovMemToAcc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	DisassembleInstruction("MOV " + dis_registers_[0][w] + ", [" + std::to_string(imm) + "]");
	offset += 1 + w;
}

void CpuState::MovAccToMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	DisassembleInstruction("MOV [" + std::to_string(imm) + "], " + dis_registers_[0][w]);
	offset += 1 + w;
}

void CpuState::AddRegMemWithRegToEither(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w)
{
	uint8_t modrm = buffer[offset + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t src;
	uint8_t dst;
	if (d == 0)
	{
		src = (modrm >> 3) & 0x07;
		dst = modrm & 0x07;
	}
	else
	{
		src = modrm & 0x07;
		dst = (modrm >> 3) & 0x07;
	}
	switch (mod)
	{
		case 0x00: AddRmwrtwMem(buffer, offset, d, w, src, dst); break;
		case 0x01: AddRmwrtwMemDisp8(buffer, offset, d, w, src, dst); break;
		case 0x02: AddRmwrtwMemDisp16(buffer, offset, d, w, src, dst); break;
		case 0x03: AddRmwrtwReg(src, dst, w, offset); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::AddRmwrtwMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address;
	//immediate memory address to register
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = buffer[offset + 2] | buffer[offset + 3] << 8;
		memory_address = std::to_string(imm);
		offset += 2;
	}
	else
	{
		memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	}

	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	if (d == 0)
	{
		DisassembleInstruction("ADD [" + memory_address + "], " + reg);
	}
	else
	{
		DisassembleInstruction("ADD " + reg + ", [" + memory_address + "]");
	}
	offset++;
}

void CpuState::AddRmwrtwMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = buffer[offset + 2];
	if (d == 0)
	{
		DisassembleInstruction("ADD [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
		
	}
	else
	{
		DisassembleInstruction("ADD " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 2;
}

void CpuState::AddRmwrtwMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	if (d == 0)
	{
		DisassembleInstruction("ADD [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("ADD " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 3;
}

void CpuState::AddRmwrtwReg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset)
{
	std::string reg_source = dis_registers_[src][w];
	std::string reg_dest = dis_registers_[dst][w];
	DisassembleInstruction("ADD " + reg_dest + ", " + reg_source);
	offset++;
}

void CpuState::AddAdcSubSbcCmpImmToRegMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w)
{
	uint8_t opcode = buffer[offset];
	uint8_t modrm = buffer[offset + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t arith = (modrm & 0x38) >> 3;
	uint8_t dst = modrm & 0x07;
	switch (mod)
	{
		case 0x00: AddAdcSubSbcCmpItrmMem(buffer, offset, s, w, dst, arith); break;
		case 0x01: AddAdcSubSbcCmpItrmMemDisp8(buffer, offset, s, w, dst, arith); break;
		case 0x02: AddAdcSubSbcCmpItrmMemDisp16(buffer, offset, s, w, dst, arith); break;
		case 0x03: AddAdcSubSbcCmpItrmReg(buffer, offset, s, w, dst, arith); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::AddAdcSubSbcCmpItrmMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith)
{

	std::string memory_address;

	if (dst == 0x06)
	{
		uint16_t imm_adr = buffer[offset + 2] | buffer[offset + 3] << 8;
		memory_address = std::to_string(imm_adr);
		offset += 2;
	}
	else
	{
		memory_address = dis_memory_addresses_[dst];
	}
	uint16_t imm = buffer[offset + 2];

	if (w == 1 && s == 0)
	{
		imm |= buffer[offset + 3] << 8;
		offset += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "], " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "], " + std::to_string(imm));
	}
	offset += 2;
}

void CpuState::AddAdcSubSbcCmpItrmMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint8_t disp = buffer[offset + 2];
	uint16_t imm = buffer[offset + 3];
	if (w == 1 && s == 0)
	{
		imm |= buffer[offset + 4] << 8;
		offset += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm));
	}
	offset += 3;
}

void CpuState::AddAdcSubSbcCmpItrmMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	uint16_t imm = buffer[offset + 4];
	if (w == 1 && s == 0)
	{
		imm |= buffer[offset + 5] << 8;
		offset += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm));
	}
	offset += 4;
}

void CpuState::AddAdcSubSbcCmpItrmReg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith)
{
	std::string reg_dest = dis_registers_[dst][w];
	uint16_t imm = buffer[offset + 2];
	if (w == 1 && s == 0)
	{
		imm |= buffer[offset + 3] << 8;
		offset += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " " + reg_dest + ", " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " " + reg_dest + ", " + std::to_string(imm));
	}
	offset += 2;
}

void CpuState::AddImmToAcc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	DisassembleInstruction("ADD " + dis_registers_[0][w] + ", " + std::to_string(imm));
	offset += 1 + w;
}

void CpuState::SubRegMemWithRegToEither(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w)
{
	uint8_t modrm = buffer[offset + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t src;
	uint8_t dst;
	if (d == 0)
	{
		src = (modrm >> 3) & 0x07;
		dst = modrm & 0x07;
	}
	else
	{
		src = modrm & 0x07;
		dst = (modrm >> 3) & 0x07;
	}
	switch (mod)
	{
	case 0x00: SubRmwrtwMem(buffer, offset, d, w, src, dst); break;
	case 0x01: SubRmwrtwMemDisp8(buffer, offset, d, w, src, dst); break;
	case 0x02: SubRmwrtwMemDisp16(buffer, offset, d, w, src, dst); break;
	case 0x03: SubRmwrtwReg(src, dst, w, offset); break;
	default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::SubRmwrtwMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address;
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = buffer[offset + 2] | buffer[offset + 3] << 8;
		memory_address = std::to_string(imm);
		offset += 2;
	}
	else
	{
		memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	}
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	if (d == 0)
	{
		DisassembleInstruction("SUB [" + memory_address + "], " + reg);
	}
	else
	{
		DisassembleInstruction("SUB " + reg + ", [" + memory_address + "]");
	}
	offset++;
}

void CpuState::SubRmwrtwMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = buffer[offset + 2];
	if (d == 0)
	{
		DisassembleInstruction("SUB [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("SUB " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 2;
}

void CpuState::SubRmwrtwMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	if (d == 0)
	{
		DisassembleInstruction("SUB [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("SUB " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 3;
}

void CpuState::SubRmwrtwReg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset)
{
	std::string reg_source = dis_registers_[src][w];
	std::string reg_dest = dis_registers_[dst][w];
	DisassembleInstruction("SUB " + reg_dest + ", " + reg_source);
	offset++;
}

void CpuState::SubImmToAcc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	DisassembleInstruction("SUB " + dis_registers_[0][w] + ", " + std::to_string(imm));
	offset += 1 + w;
}

void CpuState::CmpRegMemAndReg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w)
{
	uint8_t modrm = buffer[offset + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t src;
	uint8_t dst;
	if (d == 0)
	{
		src = (modrm >> 3) & 0x07;
		dst = modrm & 0x07;
	}
	else
	{
		src = modrm & 0x07;
		dst = (modrm >> 3) & 0x07;
	}
	switch (mod)
	{
		case 0x00: CmpRmarMem(buffer, offset, d, w, src, dst); break;
		case 0x01: CmpRmarMemDisp8(buffer, offset, d, w, src, dst); break;
		case 0x02: CmpRmarMemDisp16(buffer, offset, d, w, src, dst); break;
		case 0x03: CmpRmarReg(src, dst, w, offset); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::CmpRmarMem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address;
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = buffer[offset + 2] | buffer[offset + 3] << 8;
		memory_address = std::to_string(imm);
		offset += 2;
	}
	else
	{
		memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	}
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	if (d == 0)
	{
		DisassembleInstruction("CMP [" + memory_address + "], " + reg);
	}
	else
	{
		DisassembleInstruction("CMP " + reg + ", [" + memory_address + "]");
	}
	offset++;
}

void CpuState::CmpRmarMemDisp8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = buffer[offset + 2];
	if (d == 0)
	{
		DisassembleInstruction("CMP [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("CMP " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 2;
}

void CpuState::CmpRmarMemDisp16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	if (d == 0)
	{
		DisassembleInstruction("CMP [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("CMP " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 3;
}

void CpuState::CmpRmarReg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset)
{
	std::string reg_source = dis_registers_[src][w];
	std::string reg_dest = dis_registers_[dst][w];
	DisassembleInstruction("CMP " + reg_dest + ", " + reg_source);
	offset++;
}

void CpuState::CmpImmWithAcc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	DisassembleInstruction("CMP " + dis_registers_[0][w] + ", " + std::to_string(imm));
	offset += 1 + w;
}

void CpuState::JneJnz(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JNE " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JeJz(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JE " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JlJnge(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JL " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JleJng(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JLE " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JbJnae(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JB " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JbeJna(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JBE " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JpJpe(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JP " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JnpJpo(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JNP " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::Jo(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JO " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::Js(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JS " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JnlJge(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JNL " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JnleJg(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JNLE " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JnbJae(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JNB " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::JnbeJa(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JNBE " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::Jno(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JNO " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::Jns(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JNS " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::Loop(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("LOOP " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::LoopzLoope(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("LOOPZ " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::LoopnzLoopne(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("LOOPNZ " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

void CpuState::Jcxz(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	uint8_t imm = buffer[offset + 1];
	DisassembleInstruction("JCXZ " + std::to_string(static_cast<int8_t>(imm)));
	offset++;
}

