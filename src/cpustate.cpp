#include "cpustate.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

CpuState::CpuState()
{
	registers_.fill(0);
	ip_ = 0;
	flags_ = { 0 };
}

CpuState::~CpuState()
{
}

void CpuState::PrintRegisters() const 
{
	for (size_t i = 0; i < registers_.size(); i++)
	{
		std::cout << dis_registers_[i][1] << ": " << registers_[i] << " ";
	}
	std::cout << "IP: " << ip_;
	std::cout << std::endl;
}

void CpuState::PrintFlags() const
{
	std::cout << "CF: " << flags_.carry_ << " "
		<< "PF: " << flags_.parity_ << " "
		<< "AF: " << flags_.aux_carry_ << " "
		<< "ZF: " << flags_.zero_ << " "
		<< "SF: " << flags_.sign_ << " "
		<< "OF: " << flags_.overflow_ << " "
		<< "IF: " << flags_.interrupt_enable_ << " "
		<< "DF: " << flags_.direction_ << " "
		<< "TF: " << flags_.trap_ << std::endl;
}

void CpuState::DisassembleInstruction(const std::string_view instruction)
{
	std::cout << instruction << std::endl;
}

uint16_t CpuState::GetIp() const
{
	return ip_;
}

void CpuState::DecodeInstruction(const std::vector<uint8_t>& buffer)
{
	uint8_t opcode = buffer[ip_];
	switch (opcode)
	{
	case 0x88: MovRegMemToFromReg(buffer, 0, 0); break;
	case 0x89: MovRegMemToFromReg(buffer, 0, 1); break;
	case 0x8A: MovRegMemToFromReg(buffer, 1, 0); break;
	case 0x8B: MovRegMemToFromReg(buffer, 1, 1); break;
	case 0xC6: MovImmToRegMem(buffer, 0); break;
	case 0xC7: MovImmToRegMem(buffer, 1); break;
	case 0xB0: MovImmToReg(buffer, 0, 0x00); break;
	case 0xB1: MovImmToReg(buffer, 0, 0x01); break;
	case 0xB2: MovImmToReg(buffer, 0, 0x02); break;
	case 0xB3: MovImmToReg(buffer, 0, 0x03); break;
	case 0xB4: MovImmToReg(buffer, 0, 0x04); break;
	case 0xB5: MovImmToReg(buffer, 0, 0x05); break;
	case 0xB6: MovImmToReg(buffer, 0, 0x06); break;
	case 0xB7: MovImmToReg(buffer, 0, 0x07); break;
	case 0xB8: MovImmToReg(buffer, 1, 0x00); break;
	case 0xB9: MovImmToReg(buffer, 1, 0x01); break;
	case 0xBA: MovImmToReg(buffer, 1, 0x02); break;
	case 0xBB: MovImmToReg(buffer, 1, 0x03); break;
	case 0xBC: MovImmToReg(buffer, 1, 0x04); break;
	case 0xBD: MovImmToReg(buffer, 1, 0x05); break;
	case 0xBE: MovImmToReg(buffer, 1, 0x06); break;
	case 0xBF: MovImmToReg(buffer, 1, 0x07); break;
	case 0xA0: MovMemToAcc(buffer, 0); break;
	case 0xA1: MovMemToAcc(buffer, 1); break;
	case 0xA2: MovAccToMem(buffer, 0); break;
	case 0xA3: MovAccToMem(buffer, 1); break;
	case 0x00: AddRegMemWithRegToEither(buffer, 0, 0); break;
	case 0x01: AddRegMemWithRegToEither(buffer, 0, 1); break;
	case 0x02: AddRegMemWithRegToEither(buffer, 1, 0); break;
	case 0x03: AddRegMemWithRegToEither(buffer, 1, 1); break;
	case 0x80: AddAdcSubSbcCmpImmToRegMem(buffer, 0, 0); break;
	case 0x81: AddAdcSubSbcCmpImmToRegMem(buffer, 0, 1); break;
	case 0x82: AddAdcSubSbcCmpImmToRegMem(buffer, 1, 0); break;
	case 0x83: AddAdcSubSbcCmpImmToRegMem(buffer, 1, 1); break;
	case 0x04: AddImmToAcc(buffer, 0); break;
	case 0x05: AddImmToAcc(buffer, 1); break;
	case 0x28: SubRegMemWithRegToEither(buffer, 0, 0); break;
	case 0x29: SubRegMemWithRegToEither(buffer, 0, 1); break;
	case 0x2A: SubRegMemWithRegToEither(buffer, 1, 0); break;
	case 0x2B: SubRegMemWithRegToEither(buffer, 1, 1); break;
	case 0x2C: SubImmToAcc(buffer, 0); break;
	case 0x2D: SubImmToAcc(buffer, 1); break;
	case 0x38: CmpRegMemAndReg(buffer, 0, 0); break;
	case 0x39: CmpRegMemAndReg(buffer, 0, 1); break;
	case 0x3A: CmpRegMemAndReg(buffer, 1, 0); break;
	case 0x3B: CmpRegMemAndReg(buffer, 1, 1); break;
	case 0x3C: CmpImmWithAcc(buffer, 0); break;
	case 0x3D: CmpImmWithAcc(buffer, 1); break;
	case 0x74: JeJz(buffer); break;
	case 0x75: JneJnz(buffer); break;
	case 0x7C: JlJnge(buffer); break;
	case 0x7E: JleJng(buffer); break;
	case 0x72: JbJnae(buffer); break;
	case 0x76: JbeJna(buffer); break;
	case 0x7A: JpJpe(buffer); break;
	case 0x7B: JnpJpo(buffer); break;
	case 0x70: Jo(buffer); break;
	case 0x78: Js(buffer); break;
	case 0x7D: JnlJge(buffer); break;
	case 0x7F: JnleJg(buffer); break;
	case 0x73: JnbJae(buffer); break;
	case 0x77: JnbeJa(buffer); break;
	case 0x71: Jno(buffer); break;
	case 0x79: Jns(buffer); break;
	case 0xE2: Loop(buffer); break;
	case 0xE1: LoopzLoope(buffer); break;
	case 0xE0: LoopnzLoopne(buffer); break;
	case 0xE3: Jcxz(buffer); break;

	default: std::cout << "Opcode not implemented:b0=0x" << std::hex << +opcode << std::endl; exit(1);
	}
	ip_++;
}

//ops

void CpuState::SetFlags(const uint16_t val)
{
	flags_.zero_ = (val == 0);
	flags_.sign_ = (val & 0x8000) != 0;
	flags_.parity_ = CheckParity(val);

	//TODO implement other flags
}

bool CpuState::CheckParity(const uint16_t val) const
{
	uint8_t low_bits = val & 0xFF;
	uint8_t count = 0;
	for (size_t i = 0; i < 8; i++)
	{
		if (low_bits & (1 << i))
		{
			count++;
		}
	}
	return count % 2 == 0;
}
//MOV Register/memory to/from register
void CpuState::MovRegMemToFromReg(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w)
{
	uint8_t modrm = buffer[ip_ + 1];
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
	case 0x00: MovRmtfgMem(buffer, d, w, src, dst); break;
	case 0x01: MovRmtfgMemDisp8(buffer, d, w, src, dst); break;
	case 0x02: MovRmtfgMemDisp16(buffer, d, w, src, dst); break;
	case 0x03: MovRmtfgReg(src, dst, w); break;
	default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::MovRmtfgMem(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address;
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
		memory_address = std::to_string(imm);
		ip_ += 2;
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
	ip_++;
}

void CpuState::MovRmtfgMemDisp8(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = buffer[ip_ + 2];
	if (d == 0)
	{
		DisassembleInstruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("MOV " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	ip_ += 2;
}

void CpuState::MovRmtfgMemDisp16(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
	if (d == 0)
	{
		DisassembleInstruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("MOV " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	ip_ += 3;
}

void CpuState::MovRmtfgReg(const uint8_t src, const uint8_t dst, const uint8_t w)
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
		if (src < 4)
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
	ip_++;
}

void CpuState::MovImmToRegMem(const std::vector<uint8_t>& buffer, const uint8_t w)
{
	uint8_t modrm = buffer[ip_ + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t dst = modrm & 0x07;
	switch (mod)
	{
	case 0x00: MovItrmMem(buffer, w, dst); break;
	case 0x01: MovItrmMemDisp8(buffer, w, dst); break;
	case 0x02: MovItrmMemDisp16(buffer, w, dst); break;
	case 0x03: MovItrmReg(buffer, dst, w); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::MovItrmMem(const std::vector<uint8_t>& buffer, const uint8_t w, const uint8_t dst)
{
	uint16_t imm = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
	std::string memory_address = std::to_string(imm);
	std::string reg = dis_registers_[dst][w];
	DisassembleInstruction("MOV [" + memory_address + "], " + reg);
	ip_ += 2;
}

void CpuState::MovItrmMemDisp8(const std::vector<uint8_t>& buffer, const uint8_t w, const uint8_t dst)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint8_t disp = buffer[ip_ + 2];
	DisassembleInstruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	ip_ += 2;
}

void CpuState::MovItrmMemDisp16(const std::vector<uint8_t>& buffer, const uint8_t w, const uint8_t dst)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint16_t disp = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
	DisassembleInstruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	ip_ += 3;
}

void CpuState::MovItrmReg(const std::vector<uint8_t>& buffer, const uint8_t dst, const uint8_t w)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	DisassembleInstruction("MOV [" + memory_address + "], " + reg);
	ip_++;
}

void CpuState::MovImmToReg(const std::vector<uint8_t>& buffer, const uint8_t w, const uint8_t reg)
{
	uint16_t imm = buffer[ip_ + 1];
	if (w == 1)
	{
		imm |= buffer[ip_ + 2] << 8;
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
	
	ip_ += 1 + w;
}

void CpuState::MovMemToAcc(const std::vector<uint8_t>& buffer, const uint8_t w)
{
	uint16_t imm = buffer[ip_ + 1];
	if (w == 1)
	{
		imm |= buffer[ip_ + 2] << 8;
	}
	DisassembleInstruction("MOV " + dis_registers_[0][w] + ", [" + std::to_string(imm) + "]");
	ip_ += 1 + w;
}

void CpuState::MovAccToMem(const std::vector<uint8_t>& buffer, const uint8_t w)
{
	uint16_t imm = buffer[ip_ + 1];
	if (w == 1)
	{
		imm |= buffer[ip_ + 2] << 8;
	}
	DisassembleInstruction("MOV [" + std::to_string(imm) + "], " + dis_registers_[0][w]);
	ip_ += 1 + w;
}

void CpuState::AddRegMemWithRegToEither(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w)
{
	uint8_t modrm = buffer[ip_ + 1];
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
	case 0x00: AddRmwrtwMem(buffer, d, w, src, dst); break;
	case 0x01: AddRmwrtwMemDisp8(buffer, d, w, src, dst); break;
	case 0x02: AddRmwrtwMemDisp16(buffer, d, w, src, dst); break;
	case 0x03: AddRmwrtwReg(src, dst, w); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::AddRmwrtwMem(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address;
	//immediate memory address to register
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
		memory_address = std::to_string(imm);
		ip_ += 2;
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
	ip_++;
}

void CpuState::AddRmwrtwMemDisp8(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = buffer[ip_ + 2];
	if (d == 0)
	{
		DisassembleInstruction("ADD [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
		
	}
	else
	{
		DisassembleInstruction("ADD " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	ip_ += 2;
}

void CpuState::AddRmwrtwMemDisp16(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
	if (d == 0)
	{
		DisassembleInstruction("ADD [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("ADD " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	ip_ += 3;
}

void CpuState::AddRmwrtwReg(uint8_t src, uint8_t dst, uint8_t w)
{
	std::string reg_source = dis_registers_[src][w];
	std::string reg_dest = dis_registers_[dst][w];
	//TODO check how low/high byte registers are done for arithmetic operations
	registers_[dst] += registers_[src];
	SetFlags(registers_[dst]);
	DisassembleInstruction("ADD " + reg_dest + ", " + reg_source);
	ip_++;
}

void CpuState::AddAdcSubSbcCmpImmToRegMem(const std::vector<uint8_t>& buffer, const uint8_t s, const uint8_t w)
{
	uint8_t opcode = buffer[ip_];
	uint8_t modrm = buffer[ip_ + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t arith = (modrm & 0x38) >> 3;
	uint8_t dst = modrm & 0x07;
	switch (mod)
	{
	case 0x00: AddAdcSubSbcCmpItrmMem(buffer, s, w, dst, arith); break;
	case 0x01: AddAdcSubSbcCmpItrmMemDisp8(buffer, s, w, dst, arith); break;
	case 0x02: AddAdcSubSbcCmpItrmMemDisp16(buffer, s, w, dst, arith); break;
	case 0x03: AddAdcSubSbcCmpItrmReg(buffer, s, w, dst, arith); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::AddAdcSubSbcCmpItrmMem(const std::vector<uint8_t>& buffer, const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith)
{

	std::string memory_address;

	if (dst == 0x06)
	{
		uint16_t imm_adr = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
		memory_address = std::to_string(imm_adr);
		ip_ += 2;
	}
	else
	{
		memory_address = dis_memory_addresses_[dst];
	}
	uint16_t imm = buffer[ip_ + 2];

	if (w == 1 && s == 0)
	{
		imm |= buffer[ip_ + 3] << 8;
		ip_ += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "], " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "], " + std::to_string(imm));
	}
	ip_ += 2;
}

void CpuState::AddAdcSubSbcCmpItrmMemDisp8(const std::vector<uint8_t>& buffer, const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint8_t disp = buffer[ip_ + 2];
	uint16_t imm = buffer[ip_ + 3];
	if (w == 1 && s == 0)
	{
		imm |= buffer[ip_ + 4] << 8;
		ip_ += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm));
	}
	ip_ += 3;
}

void CpuState::AddAdcSubSbcCmpItrmMemDisp16(const std::vector<uint8_t>& buffer, const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint16_t disp = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
	uint16_t imm = buffer[ip_ + 4];
	if (w == 1 && s == 0)
	{
		imm |= buffer[ip_ + 5] << 8;
		ip_ += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm));
	}
	ip_ += 4;
}

void CpuState::AddAdcSubSbcCmpItrmReg(const std::vector<uint8_t>& buffer, const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith)
{
	std::string reg_dest = dis_registers_[dst][w];
	uint16_t imm = buffer[ip_ + 2];
	if (w == 1 && s == 0)
	{
		imm |= buffer[ip_ + 3] << 8;
		ip_ += 1;
	}

	//TODO implement others
	//TODO check how low/high byte registers are done for arithmetic operations
	switch (arith)
	{
	case 0x00: registers_[dst] += imm; SetFlags(registers_[dst]); break;
		case 0x05: registers_[dst] -= imm; SetFlags(registers_[dst]); break;
		case 0x07: SetFlags(registers_[dst] - imm); break;
		default: break;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " " + reg_dest + ", " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " " + reg_dest + ", " + std::to_string(imm));
	}
	ip_ += 2;
}

void CpuState::AddImmToAcc(const std::vector<uint8_t>& buffer, const uint8_t w)
{
	uint16_t imm = buffer[ip_ + 1];
	if (w == 1)
	{
		imm |= buffer[ip_ + 2] << 8;
	}

	//TODO check if low high acc needs to be implemented. Even the disassemble might need to be fixed as I'd be considering only the low;
	registers_[0] += imm;
	SetFlags(registers_[0]);
	
	DisassembleInstruction("ADD " + dis_registers_[0][w] + ", " + std::to_string(imm));
	ip_ += 1 + w;
}

void CpuState::SubRegMemWithRegToEither(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w)
{
	uint8_t modrm = buffer[ip_ + 1];
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
	case 0x00: SubRmwrtwMem(buffer, d, w, src, dst); break;
	case 0x01: SubRmwrtwMemDisp8(buffer, d, w, src, dst); break;
	case 0x02: SubRmwrtwMemDisp16(buffer, d, w, src, dst); break;
	case 0x03: SubRmwrtwReg(src, dst, w); break;
	default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::SubRmwrtwMem(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address;
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
		memory_address = std::to_string(imm);
		ip_ += 2;
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
	ip_++;
}

void CpuState::SubRmwrtwMemDisp8(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = buffer[ip_ + 2];
	if (d == 0)
	{
		DisassembleInstruction("SUB [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("SUB " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	ip_ += 2;
}

void CpuState::SubRmwrtwMemDisp16(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
	if (d == 0)
	{
		DisassembleInstruction("SUB [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("SUB " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	ip_ += 3;
}

void CpuState::SubRmwrtwReg(const uint8_t src, const uint8_t dst, const uint8_t w)
{
	std::string reg_source = dis_registers_[src][w];
	std::string reg_dest = dis_registers_[dst][w];
	//TODO check how low/high byte registers are done for arithmetic operations
	registers_[dst] -= registers_[src];
	SetFlags(registers_[dst]);
	DisassembleInstruction("SUB " + reg_dest + ", " + reg_source);
	ip_++;
}

void CpuState::SubImmToAcc(const std::vector<uint8_t>& buffer, const uint8_t w)
{
	uint16_t imm = buffer[ip_ + 1];
	if (w == 1)
	{
		imm |= buffer[ip_ + 2] << 8;
	}
	
	//TODO check if low high acc needs to be implemented. Even the disassemble might need to be fixed as I'd be considering only the low;
	registers_[0] -= imm;
	SetFlags(registers_[0]);

	DisassembleInstruction("SUB " + dis_registers_[0][w] + ", " + std::to_string(imm));
	ip_ += 1 + w;
}

void CpuState::CmpRegMemAndReg(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w)
{
	uint8_t modrm = buffer[ip_ + 1];
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
	case 0x00: CmpRmarMem(buffer, d, w, src, dst); break;
	case 0x01: CmpRmarMemDisp8(buffer, d, w, src, dst); break;
	case 0x02: CmpRmarMemDisp16(buffer, d, w, src, dst); break;
	case 0x03: CmpRmarReg(src, dst, w); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::CmpRmarMem(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address;
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
		memory_address = std::to_string(imm);
		ip_ += 2;
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
	ip_++;
}

void CpuState::CmpRmarMemDisp8(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = buffer[ip_ + 2];
	if (d == 0)
	{
		DisassembleInstruction("CMP [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("CMP " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	ip_ += 2;
}

void CpuState::CmpRmarMemDisp16(const std::vector<uint8_t>& buffer, const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = buffer[ip_ + 2] | buffer[ip_ + 3] << 8;
	if (d == 0)
	{
		DisassembleInstruction("CMP [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		DisassembleInstruction("CMP " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	ip_ += 3;
}

void CpuState::CmpRmarReg(uint8_t src, uint8_t dst, uint8_t w)
{
	std::string reg_source = dis_registers_[src][w];
	std::string reg_dest = dis_registers_[dst][w];
	//TODO check how low/high byte registers are done for arithmetic operations
	SetFlags(registers_[dst] - registers_[src]);
	DisassembleInstruction("CMP " + reg_dest + ", " + reg_source);
	ip_++;
}

void CpuState::CmpImmWithAcc(const std::vector<uint8_t>& buffer, uint8_t w)
{
	uint16_t imm = buffer[ip_ + 1];
	if (w == 1)
	{
		imm |= buffer[ip_ + 2] << 8;
	}
	//TODO check if low high acc needs to be implemented. Even the disassemble might need to be fixed as I'd be considering only the low;
	SetFlags(registers_[0] - imm);
	DisassembleInstruction("CMP " + dis_registers_[0][w] + ", " + std::to_string(imm));
	ip_ += 1 + w;
}

void CpuState::JneJnz(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JNE " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.zero_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JeJz(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JE " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.zero_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JlJnge(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JL " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.sign_ != flags_.overflow_)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JleJng(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JLE " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.zero_ == 1 || flags_.sign_ != flags_.overflow_)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JbJnae(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JB " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.carry_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JbeJna(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JBE " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.zero_ == 1 || flags_.carry_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JpJpe(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JP " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.parity_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnpJpo(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JNP " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.parity_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Jo(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JO " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.overflow_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Js(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JS " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.sign_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnlJge(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JNL " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.sign_ == flags_.overflow_)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnleJg(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JNLE " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.zero_ == 0 && flags_.sign_ == flags_.overflow_)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnbJae(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JNB " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.carry_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnbeJa(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JNBE " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.zero_ == 0 && flags_.carry_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Jno(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JNO " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.overflow_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Jns(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JNS " + std::to_string(static_cast<int8_t>(imm)));
	if (flags_.sign_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Loop(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("LOOP " + std::to_string(static_cast<int8_t>(imm)));
	ip_++;
}

void CpuState::LoopzLoope(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("LOOPZ " + std::to_string(static_cast<int8_t>(imm)));
	ip_++;
}

void CpuState::LoopnzLoopne(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("LOOPNZ " + std::to_string(static_cast<int8_t>(imm)));
	ip_++;
}

void CpuState::Jcxz(const std::vector<uint8_t>& buffer)
{
	uint8_t imm = buffer[ip_ + 1];
	DisassembleInstruction("JCXZ " + std::to_string(static_cast<int8_t>(imm)));
	ip_++;
}

