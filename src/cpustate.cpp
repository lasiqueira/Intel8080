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
	segment_registers_.fill(0);
	memory_ = std::make_unique<std::array<uint8_t, 0xF4240>>();
	memory_->fill(0);
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

void CpuState::PrintSegmentRegisters() const
{
	for (size_t i = 0; i < segment_registers_.size(); i++)
	{
		std::cout << dis_segment_registers_[i] << ": " << segment_registers_[i] << " ";
	}
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

void CpuState::DisassembleInstruction(const std::string_view instruction, const uint8_t cycles)
{
	dis_total_cycles_ += cycles;
	std::cout << instruction << " - " << "Cycles: +" << std::to_string(cycles) << " = " << dis_total_cycles_ << std::endl;
}

uint32_t CpuState::GetCsIp() const
{	
	//0 == CS
	uint32_t cs = static_cast<uint32_t>(segment_registers_[0]) << 4;
	return cs + ip_;
}

std::array<uint8_t, 0xF4240>& CpuState::GetMemory()
{
	return *memory_;
}

uint32_t CpuState::GetMemoryAddress(const uint8_t reg) const
{
	//TODO understand how to use the segment registers to get the memory address
	switch (reg)
	{
	case 0x00: return static_cast<uint32_t>(registers_[3] + registers_[6]); //BX + SI
	case 0x01: return static_cast<uint32_t>(registers_[3] + registers_[7]); //BX + DI
	case 0x02: return static_cast<uint32_t>(registers_[5] + registers_[6]); //BP + SI
	case 0x03: return static_cast<uint32_t>(registers_[5] + registers_[7]); //BP + DI
	case 0x04: return static_cast<uint32_t>(registers_[6]); //SI
	case 0x05: return static_cast<uint32_t>(registers_[7]); //DI
	case 0x06: return static_cast<uint32_t>(registers_[5]); //BP
	case 0x07: return static_cast<uint32_t>(registers_[3]); //BX
	default: return 0;
	}
}

uint8_t CpuState::DecodeInstruction()
{
	if (GetCsIp() >= memory_->size())
	{
		return 1;
	}
	uint8_t opcode = memory_->at(GetCsIp());
	switch (opcode)
	{
	case 0x88: MovRegMemToFromReg(0, 0); break;
	case 0x89: MovRegMemToFromReg(0, 1); break;
	case 0x8A: MovRegMemToFromReg(1, 0); break;
	case 0x8B: MovRegMemToFromReg(1, 1); break;
	case 0xC6: MovImmToRegMem(0); break;
	case 0xC7: MovImmToRegMem(1); break;
	case 0xB0: MovImmToReg(0, 0x00); break;
	case 0xB1: MovImmToReg(0, 0x01); break;
	case 0xB2: MovImmToReg(0, 0x02); break;
	case 0xB3: MovImmToReg(0, 0x03); break;
	case 0xB4: MovImmToReg(0, 0x04); break;
	case 0xB5: MovImmToReg(0, 0x05); break;
	case 0xB6: MovImmToReg(0, 0x06); break;
	case 0xB7: MovImmToReg(0, 0x07); break;
	case 0xB8: MovImmToReg(1, 0x00); break;
	case 0xB9: MovImmToReg(1, 0x01); break;
	case 0xBA: MovImmToReg(1, 0x02); break;
	case 0xBB: MovImmToReg(1, 0x03); break;
	case 0xBC: MovImmToReg(1, 0x04); break;
	case 0xBD: MovImmToReg(1, 0x05); break;
	case 0xBE: MovImmToReg(1, 0x06); break;
	case 0xBF: MovImmToReg(1, 0x07); break;
	case 0xA0: MovMemToAcc(0); break;
	case 0xA1: MovMemToAcc(1); break;
	case 0xA2: MovAccToMem(0); break;
	case 0xA3: MovAccToMem(1); break;
	case 0x00: AddRegMemWithRegToEither(0, 0); break;
	case 0x01: AddRegMemWithRegToEither(0, 1); break;
	case 0x02: AddRegMemWithRegToEither(1, 0); break;
	case 0x03: AddRegMemWithRegToEither(1, 1); break;
	case 0x80: AddAdcSubSbcCmpImmToRegMem(0, 0); break;
	case 0x81: AddAdcSubSbcCmpImmToRegMem(0, 1); break;
	case 0x82: AddAdcSubSbcCmpImmToRegMem(1, 0); break;
	case 0x83: AddAdcSubSbcCmpImmToRegMem(1, 1); break;
	case 0x04: AddImmToAcc(0); break;
	case 0x05: AddImmToAcc(1); break;
	case 0x28: SubRegMemWithRegToEither(0, 0); break;
	case 0x29: SubRegMemWithRegToEither(0, 1); break;
	case 0x2A: SubRegMemWithRegToEither(1, 0); break;
	case 0x2B: SubRegMemWithRegToEither(1, 1); break;
	case 0x2C: SubImmToAcc(0); break;
	case 0x2D: SubImmToAcc(1); break;
	case 0x38: CmpRegMemAndReg(0, 0); break;
	case 0x39: CmpRegMemAndReg(0, 1); break;
	case 0x3A: CmpRegMemAndReg(1, 0); break;
	case 0x3B: CmpRegMemAndReg(1, 1); break;
	case 0x3C: CmpImmWithAcc(0); break;
	case 0x3D: CmpImmWithAcc(1); break;
	case 0x74: JeJz(); break;
	case 0x75: JneJnz(); break;
	case 0x7C: JlJnge(); break;
	case 0x7E: JleJng(); break;
	case 0x72: JbJnae(); break;
	case 0x76: JbeJna(); break;
	case 0x7A: JpJpe(); break;
	case 0x7B: JnpJpo(); break;
	case 0x70: Jo(); break;
	case 0x78: Js(); break;
	case 0x7D: JnlJge(); break;
	case 0x7F: JnleJg(); break;
	case 0x73: JnbJae(); break;
	case 0x77: JnbeJa(); break;
	case 0x71: Jno(); break;
	case 0x79: Jns(); break;
	case 0xE2: Loop(); break;
	case 0xE1: LoopzLoope(); break;
	case 0xE0: LoopnzLoopne(); break;
	case 0xE3: Jcxz(); break;
	//TODO implement all opcodes
	//default: std::cout << "Opcode not implemented:b0=0x" << std::hex << +opcode << std::endl; return 1;
	default: return 1;
	}
	ip_++;
	return 0;
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

//TODO deduplicate identical functions (ADD, MOV, SUB, CMP, etc) by adding OP as a parameter (wait until simulation starts perhaps?)

//MOV Register/memory to/from register
void CpuState::MovRegMemToFromReg(const uint8_t d, const uint8_t w)
{
	uint8_t modrm = memory_->at(GetCsIp() + 1);
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
	case 0x00: MovRmtfgMem(d, w, src, dst); break;
	case 0x01: MovRmtfgMemDisp8(d, w, src, dst); break;
	case 0x02: MovRmtfgMemDisp16(d, w, src, dst); break;
	case 0x03: MovRmtfgReg(src, dst, w); break;
	default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::MovRmtfgMem(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address_name;
	uint32_t memory_address;
	uint8_t cycles = 0;
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		cycles +=6;
		uint16_t imm = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
		memory_address_name = std::to_string(imm);
		memory_address = imm;
		ip_ += 2;
		
	}
	else
	{
		memory_address_name = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
		auto memory_address_cycles = d == 0 ? dis_memory_cycles_[dst][0] : dis_memory_cycles_[src][0];
		cycles += memory_address_cycles;
		memory_address = GetMemoryAddress(d == 0 ? dst : src);
	}
	std::string reg_name = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	if (d == 0)
	{
		cycles += 9;
		if (w == 1) 
		{
			memory_->at(memory_address) = static_cast<uint8_t>(registers_[src] & 0XFF);
			memory_->at(memory_address+1) = static_cast<uint8_t>((registers_[src] >> 8) & 0xFF);
		}
		else
		{
			if (src < 4)
			{
				memory_->at(memory_address) = static_cast<uint8_t>(registers_[src] & 0XFF);
			}
			else
			{
				memory_->at(memory_address) = static_cast<uint8_t>((registers_[src % 4] >> 8) & 0xFF);
			}
		}
		DisassembleInstruction("MOV [" + memory_address_name + "], " + reg_name, cycles);
	}
	else
	{
		cycles += 8;
		if (w == 1)
		{
			registers_[dst] = memory_->at(memory_address) | memory_->at(memory_address + 1) << 8;
		}
		else
		{
			uint8_t val = memory_->at(memory_address);
			if (dst < 4)
			{
				registers_[dst] &= 0xFF00;
				registers_[dst] |= val;
			}
			else
			{
				registers_[dst % 4] &= 0x00FF;
				registers_[dst % 4] |= (val << 8);
			}
		}
		DisassembleInstruction("MOV " + reg_name + ", [" + memory_address_name + "]", cycles);
	}
	ip_++;
}

void CpuState::MovRmtfgMemDisp8(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{

	std::string memory_address_name = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	uint32_t memory_address = GetMemoryAddress(d == 0 ? dst : src);
	
	std::string reg_name = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	
	uint8_t disp = memory_->at(GetCsIp() + 2);
	uint8_t cycles = d == 0 ? dis_memory_cycles_[dst][disp>0] : dis_memory_cycles_[src][disp>0];

	
	if (d == 0)
	{	
		cycles += 9;
		if (w == 1)
		{
			memory_->at(memory_address + disp) = static_cast<uint8_t>(registers_[src] & 0XFF);
			memory_->at(memory_address + disp + 1) = static_cast<uint8_t>((registers_[src] >> 8) & 0xFF);
		}
		else
		{
			if (src < 4)
			{
				memory_->at(memory_address + disp) = static_cast<uint8_t>(registers_[src] & 0XFF);
			}
			else
			{
				memory_->at(memory_address + disp) = static_cast<uint8_t>((registers_[src % 4] >> 8) & 0xFF);
			}
		}
		DisassembleInstruction("MOV [" + memory_address_name + "+" + std::to_string(disp) + "], " + reg_name, cycles);
	}
	else
	{
		cycles += 8;
		if (w == 1)
		{
			registers_[dst] = memory_->at(memory_address + disp) | memory_->at(memory_address + disp  + 1) << 8;
		}
		else
		{
			uint8_t val = memory_->at(memory_address + disp);
			if (dst < 4)
			{
				registers_[dst] &= 0xFF00;
				registers_[dst] |= val;
			}
			else
			{
				registers_[dst % 4] &= 0x00FF;
				registers_[dst % 4] |= (val << 8);
			}
		}

		DisassembleInstruction("MOV " + reg_name + ", [" + memory_address_name + "+" + std::to_string(disp) + "]", cycles);
	}
	ip_ += 2;
}

void CpuState::MovRmtfgMemDisp16(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{

	std::string memory_address_name = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	
	uint32_t memory_address = GetMemoryAddress(d == 0 ? dst : src);
	std::string reg_name = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
	uint8_t cycles = d == 0 ? dis_memory_cycles_[dst][disp>0] : dis_memory_cycles_[src][disp>0];

	if (d == 0)
	{
		cycles += 9;
		if (w == 1)
		{
			memory_->at(memory_address + disp) = static_cast<uint8_t>(registers_[src] & 0XFF);
			memory_->at(memory_address + disp + 1) = static_cast<uint8_t>((registers_[src] >> 8) & 0xFF);
		}
		else
		{
			if (src < 4)
			{
				memory_->at(memory_address + disp) = static_cast<uint8_t>(registers_[src] & 0XFF);
			}
			else
			{
				memory_->at(memory_address + disp) = static_cast<uint8_t>((registers_[src % 4] >> 8) & 0xFF);
			}
		}
		DisassembleInstruction("MOV [" + memory_address_name + "+" + std::to_string(disp) + "], " + reg_name, cycles);
	}
	else
	{
		cycles += 8;
		if (w == 1)
		{
			registers_[dst] = memory_->at(memory_address + disp) | memory_->at(memory_address + disp + 1) << 8;
		}
		else
		{
			uint8_t val = memory_->at(memory_address + disp);
			if (dst < 4)
			{
				registers_[dst] &= 0xFF00;
				registers_[dst] |= val;
			}
			else
			{
				registers_[dst % 4] &= 0x00FF;
				registers_[dst % 4] |= (val << 8);
			}
		}
		DisassembleInstruction("MOV " + reg_name + ", [" + memory_address_name + "+" + std::to_string(disp) + "]", cycles);
	}
	ip_ += 3;
}

void CpuState::MovRmtfgReg(const uint8_t src, const uint8_t dst, const uint8_t w)
{
	std::string reg_source = dis_registers_[src][w];
	std::string reg_dest = dis_registers_[dst][w];
	uint8_t cycles = 2;
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
	DisassembleInstruction("MOV " + reg_dest + ", " + reg_source, cycles);
	ip_++;
}

void CpuState::MovImmToRegMem(const uint8_t w)
{
	uint8_t modrm = memory_->at(GetCsIp() + 1);
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t dst = modrm & 0x07;
	switch (mod)
	{
	case 0x00: MovItrmMem(w, dst); break;
	case 0x01: MovItrmMemDisp8(w, dst); break;
	case 0x02: MovItrmMemDisp16(w, dst); break;
	case 0x03: MovItrmReg(dst, w); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::MovItrmMem(const uint8_t w, const uint8_t dst)
{
	
	std::string memory_address_name;
	uint32_t memory_address;
	uint8_t cycles = 10;
	if (dst == 0x06)
	{
		uint16_t imm = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
		memory_address_name = std::to_string(imm);
		memory_address = imm;
		ip_ += 2;
		cycles += 6;
	}
	else
	{
		memory_address_name = dis_memory_addresses_[dst];
		auto memory_address_cycles = dis_memory_cycles_[dst][0];
		cycles += memory_address_cycles;
		memory_address = GetMemoryAddress(dst);
	}
	uint16_t imm = memory_->at(GetCsIp() + 2);
	if (w == 1)
	{
		imm |= memory_->at(GetCsIp() + 3) << 8;
		memory_->at(memory_address) = static_cast<uint8_t>(imm & 0xFF);
		memory_->at(memory_address + 1) = static_cast<uint8_t>((imm >> 8) & 0xFF);
	}
	else
	{
		memory_->at(memory_address) = static_cast<uint8_t>(imm & 0xFF);
	}
	
	DisassembleInstruction("MOV [" + memory_address_name + "], " + std::to_string(imm), cycles);
	ip_ += 2 + w;
}

void CpuState::MovItrmMemDisp8(const uint8_t w, const uint8_t dst)
{
	uint8_t cycles = 10;
	std::string memory_address_name = dis_memory_addresses_[dst];
	uint32_t memory_address = GetMemoryAddress(dst);
	uint8_t disp = memory_->at(GetCsIp() + 2);
	uint16_t imm = memory_->at(GetCsIp() + 3);
	
	auto memory_address_cycles = dis_memory_cycles_[dst][disp>0];
	cycles += memory_address_cycles;
	
	if (w == 1)
	{
		imm |= memory_->at(GetCsIp() + 4) << 8;
		memory_->at(memory_address + disp) = static_cast<uint8_t>(imm & 0xFF);
		memory_->at(memory_address + disp + 1) = static_cast<uint8_t>((imm >> 8) & 0xFF);
	}
	else
	{
		memory_->at(memory_address + disp) = static_cast<uint8_t>(imm & 0xFF);
	}
	DisassembleInstruction("MOV [" + memory_address_name + "+" + std::to_string(disp) + "], " + std::to_string(imm), cycles);
	ip_ += 3 + w;
}

void CpuState::MovItrmMemDisp16(const uint8_t w, const uint8_t dst)
{
	uint8_t cycles = 10;
	std::string memory_address_name = dis_memory_addresses_[dst];
	uint32_t memory_address = GetMemoryAddress(dst);
	uint16_t disp = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
	uint16_t imm = memory_->at(GetCsIp() + 4);

	auto memory_address_cycles = dis_memory_cycles_[dst][disp>0];
	cycles += memory_address_cycles;
	if (w == 1)
	{
		imm |= memory_->at(GetCsIp() + 5) << 8;
		memory_->at(memory_address + disp) = static_cast<uint8_t>(imm & 0xFF);
		memory_->at(memory_address + disp + 1) = static_cast<uint8_t>((imm >> 8) & 0xFF);
	}
	else
	{
		memory_->at(memory_address + disp) = static_cast<uint8_t>(imm & 0xFF);
	}
	DisassembleInstruction("MOV [" + memory_address_name + "+" + std::to_string(disp) + "], " + std::to_string(imm), cycles);
	ip_ += 4 + w;
}

void CpuState::MovItrmReg( const uint8_t w, const uint8_t dst)
{
	std::string reg_name = dis_registers_[dst][w];
	uint16_t imm = memory_->at(GetCsIp() + 2);
	uint8_t cycles = 4;
	if (w == 1)
	{
		imm |= memory_->at(GetCsIp() + 3) << 8;
		registers_[dst] = imm;
	}
	else
	{
		if (dst < 4)
		{
			registers_[dst] &= 0xFF00;
			registers_[dst] |= imm;
		}
		else
		{
			registers_[dst % 4] &= 0x00FF;
			registers_[dst % 4] |= (imm << 8);
		}
	}
	DisassembleInstruction("MOV " + reg_name + ", " + std::to_string(imm), cycles);
	ip_+= 2 + w;
}

void CpuState::MovImmToReg(const uint8_t w, const uint8_t reg)
{
	uint16_t imm = memory_->at(GetCsIp() + 1);
	uint8_t cycles = 4;
	if (w == 1)
	{
		imm |= memory_->at(GetCsIp() + 2) << 8;
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
	DisassembleInstruction("MOV " + dis_registers_[reg][w] + ", " + std::to_string(imm), cycles);
	
	ip_ += 1 + w;
}

void CpuState::MovMemToAcc(const uint8_t w)
{
	uint16_t imm = memory_->at(GetCsIp() + 1);
	uint8_t cycles = 10;
	if (w == 1)
	{
		imm |= memory_->at(GetCsIp() + 2) << 8;
		registers_[0] = imm;
	}
	else
	{
		// is it always the full register?
		registers_[0] = imm;
	}
	DisassembleInstruction("MOV " + dis_registers_[0][w] + ", [" + std::to_string(imm) + "]", cycles);
	ip_ += 1 + w;
}

void CpuState::MovAccToMem(const uint8_t w)
{
	uint16_t imm = memory_->at(GetCsIp() + 1);
	uint8_t cycles = 10;
	if (w == 1)
	{
		imm |= memory_->at(GetCsIp() + 2) << 8;
		memory_->at(imm) = static_cast<uint8_t>(registers_[0] & 0xFF);
		memory_->at(imm + 1) = static_cast<uint8_t>((registers_[0] >> 8) & 0xFF);
	}
	else
	{
		// is it always the full register?
		memory_->at(imm) = static_cast<uint8_t>(registers_[0] & 0xFF);
	}
	DisassembleInstruction("MOV [" + std::to_string(imm) + "], " + dis_registers_[0][w], cycles);
	ip_ += 1 + w;
}

void CpuState::AddRegMemWithRegToEither(const uint8_t d, const uint8_t w)
{
	uint8_t modrm = memory_->at(GetCsIp() + 1);
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
	case 0x00: AddRmwrtwMem(d, w, src, dst); break;
	case 0x01: AddRmwrtwMemDisp8(d, w, src, dst); break;
	case 0x02: AddRmwrtwMemDisp16(d, w, src, dst); break;
	case 0x03: AddRmwrtwReg(src, dst, w); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::AddRmwrtwMem(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address;
	//immediate memory address to register
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
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
		DisassembleInstruction("ADD [" + memory_address + "], " + reg, 0);
	}
	else
	{
		DisassembleInstruction("ADD " + reg + ", [" + memory_address + "]", 0);
	}
	ip_++;
}

void CpuState::AddRmwrtwMemDisp8(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = memory_->at(GetCsIp() + 2);
	if (d == 0)
	{
		DisassembleInstruction("ADD [" + memory_address + "+" + std::to_string(disp) + "], " + reg, 0);
		
	}
	else
	{
		DisassembleInstruction("ADD " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]", 0);
	}
	ip_ += 2;
}

void CpuState::AddRmwrtwMemDisp16(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
	if (d == 0)
	{
		DisassembleInstruction("ADD [" + memory_address + "+" + std::to_string(disp) + "], " + reg, 0);
	}
	else
	{
		DisassembleInstruction("ADD " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]", 0);
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
	DisassembleInstruction("ADD " + reg_dest + ", " + reg_source, 0);
	ip_++;
}

void CpuState::AddAdcSubSbcCmpImmToRegMem(const uint8_t s, const uint8_t w)
{
	uint8_t opcode = memory_->at(GetCsIp());
	uint8_t modrm = memory_->at(GetCsIp() + 1);
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t arith = (modrm & 0x38) >> 3;
	uint8_t dst = modrm & 0x07;
	switch (mod)
	{
	case 0x00: AddAdcSubSbcCmpItrmMem(s, w, dst, arith); break;
	case 0x01: AddAdcSubSbcCmpItrmMemDisp8(s, w, dst, arith); break;
	case 0x02: AddAdcSubSbcCmpItrmMemDisp16(s, w, dst, arith); break;
	case 0x03: AddAdcSubSbcCmpItrmReg(s, w, dst, arith); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::AddAdcSubSbcCmpItrmMem(const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith)
{

	std::string memory_address;

	if (dst == 0x06)
	{
		uint16_t imm_adr = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
		memory_address = std::to_string(imm_adr);
		ip_ += 2;
	}
	else
	{
		memory_address = dis_memory_addresses_[dst];
	}
	uint16_t imm = memory_->at(GetCsIp() + 2);

	if (w == 1 && s == 0)
	{
		imm |= memory_->at(GetCsIp() + 3) << 8;
		ip_ += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "], " + std::to_string(static_cast<int16_t>(imm)), 0);
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "], " + std::to_string(imm), 0);
	}
	ip_ += 2;
}

void CpuState::AddAdcSubSbcCmpItrmMemDisp8(const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint8_t disp = memory_->at(GetCsIp() + 2);
	uint16_t imm = memory_->at(GetCsIp() + 3);
	if (w == 1 && s == 0)
	{
		imm |= memory_->at(GetCsIp() + 4) << 8;
		ip_ += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(static_cast<int16_t>(imm)), 0);
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm), 0);
	}
	ip_ += 3;
}

void CpuState::AddAdcSubSbcCmpItrmMemDisp16(const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith)
{
	std::string memory_address = dis_memory_addresses_[dst];
	std::string reg = dis_registers_[dst][w];
	uint16_t disp = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
	uint16_t imm = memory_->at(GetCsIp() + 4);
	if (w == 1 && s == 0)
	{
		imm |= memory_->at(GetCsIp() + 5) << 8;
		ip_ += 1;
	}
	if (s == 1)
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(static_cast<int16_t>(imm)), 0);
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm), 0);
	}
	ip_ += 4;
}

void CpuState::AddAdcSubSbcCmpItrmReg(const uint8_t s, const uint8_t w, const uint8_t dst, const uint8_t arith)
{
	std::string reg_dest = dis_registers_[dst][w];
	uint16_t imm = memory_->at(GetCsIp() + 2);
	if (w == 1 && s == 0)
	{
		imm |= memory_->at(GetCsIp() + 3) << 8;
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
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " " + reg_dest + ", " + std::to_string(static_cast<int16_t>(imm)), 0);
	}
	else
	{
		DisassembleInstruction(dis_arithmetic_imm_reg_operations_[arith] + " " + reg_dest + ", " + std::to_string(imm), 0);
	}
	ip_ += 2;
}

void CpuState::AddImmToAcc(const uint8_t w)
{
	uint16_t imm = memory_->at(GetCsIp() + 1);
	if (w == 1)
	{
		imm |= memory_->at(GetCsIp() + 2) << 8;
	}

	//TODO check if low high acc needs to be implemented. Even the disassemble might need to be fixed as I'd be considering only the low;
	registers_[0] += imm;
	SetFlags(registers_[0]);
	
	DisassembleInstruction("ADD " + dis_registers_[0][w] + ", " + std::to_string(imm), 0);
	ip_ += 1 + w;
}

void CpuState::SubRegMemWithRegToEither(const uint8_t d, const uint8_t w)
{
	uint8_t modrm = memory_->at(GetCsIp() + 1);
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
	case 0x00: SubRmwrtwMem(d, w, src, dst); break;
	case 0x01: SubRmwrtwMemDisp8(d, w, src, dst); break;
	case 0x02: SubRmwrtwMemDisp16(d, w, src, dst); break;
	case 0x03: SubRmwrtwReg(src, dst, w); break;
	default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::SubRmwrtwMem(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address;
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
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
		DisassembleInstruction("SUB [" + memory_address + "], " + reg, 0);
	}
	else
	{
		DisassembleInstruction("SUB " + reg + ", [" + memory_address + "]", 0);
	}
	ip_++;
}

void CpuState::SubRmwrtwMemDisp8(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = memory_->at(GetCsIp() + 2);
	if (d == 0)
	{
		DisassembleInstruction("SUB [" + memory_address + "+" + std::to_string(disp) + "], " + reg, 0);
	}
	else
	{
		DisassembleInstruction("SUB " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]", 0);
	}
	ip_ += 2;
}

void CpuState::SubRmwrtwMemDisp16(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
	if (d == 0)
	{
		DisassembleInstruction("SUB [" + memory_address + "+" + std::to_string(disp) + "], " + reg, 0);
	}
	else
	{
		DisassembleInstruction("SUB " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]", 0);
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
	DisassembleInstruction("SUB " + reg_dest + ", " + reg_source, 0);
	ip_++;
}

void CpuState::SubImmToAcc(const uint8_t w)
{
	uint16_t imm = memory_->at(GetCsIp() + 1);
	if (w == 1)
	{
		imm |= memory_->at(GetCsIp() + 2) << 8;
	}
	
	//TODO check if low high acc needs to be implemented. Even the disassemble might need to be fixed as I'd be considering only the low;
	registers_[0] -= imm;
	SetFlags(registers_[0]);

	DisassembleInstruction("SUB " + dis_registers_[0][w] + ", " + std::to_string(imm), 0);
	ip_ += 1 + w;
}

void CpuState::CmpRegMemAndReg(const uint8_t d, const uint8_t w)
{
	uint8_t modrm = memory_->at(GetCsIp() + 1);
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
	case 0x00: CmpRmarMem(d, w, src, dst); break;
	case 0x01: CmpRmarMemDisp8(d, w, src, dst); break;
	case 0x02: CmpRmarMemDisp16(d, w, src, dst); break;
	case 0x03: CmpRmarReg(src, dst, w); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void CpuState::CmpRmarMem(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address;
	if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06))
	{
		uint16_t imm = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
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
		DisassembleInstruction("CMP [" + memory_address + "], " + reg, 0);
	}
	else
	{
		DisassembleInstruction("CMP " + reg + ", [" + memory_address + "]", 0);
	}
	ip_++;
}

void CpuState::CmpRmarMemDisp8(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint8_t disp = memory_->at(GetCsIp() + 2);
	if (d == 0)
	{
		DisassembleInstruction("CMP [" + memory_address + "+" + std::to_string(disp) + "], " + reg, 0);
	}
	else
	{
		DisassembleInstruction("CMP " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]", 0);
	}
	ip_ += 2;
}

void CpuState::CmpRmarMemDisp16(const uint8_t d, const uint8_t w, const uint8_t src, const uint8_t dst)
{
	std::string memory_address = d == 0 ? dis_memory_addresses_[dst] : dis_memory_addresses_[src];
	std::string reg = d == 0 ? dis_registers_[src][w] : dis_registers_[dst][w];
	uint16_t disp = memory_->at(GetCsIp() + 2) | memory_->at(GetCsIp() + 3) << 8;
	if (d == 0)
	{
		DisassembleInstruction("CMP [" + memory_address + "+" + std::to_string(disp) + "], " + reg, 0);
	}
	else
	{
		DisassembleInstruction("CMP " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]", 0);
	}
	ip_ += 3;
}

void CpuState::CmpRmarReg(uint8_t src, uint8_t dst, uint8_t w)
{
	std::string reg_source = dis_registers_[src][w];
	std::string reg_dest = dis_registers_[dst][w];
	//TODO check how low/high byte registers are done for arithmetic operations
	SetFlags(registers_[dst] - registers_[src]);
	DisassembleInstruction("CMP " + reg_dest + ", " + reg_source, 0);
	ip_++;
}

void CpuState::CmpImmWithAcc(uint8_t w)
{
	uint16_t imm = memory_->at(GetCsIp() + 1);
	if (w == 1)
	{ 
		imm |= memory_->at(GetCsIp() + 2) << 8;
	}
	//TODO check if low high acc needs to be implemented. Even the disassemble might need to be fixed as I'd be considering only the low;
	SetFlags(registers_[0] - imm);
	DisassembleInstruction("CMP " + dis_registers_[0][w] + ", " + std::to_string(imm), 0);
	ip_ += 1 + w;
}

void CpuState::JneJnz()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JNE " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.zero_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JeJz()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JE " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.zero_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JlJnge()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JL " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.sign_ != flags_.overflow_)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JleJng()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JLE " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.zero_ == 1 || flags_.sign_ != flags_.overflow_)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JbJnae()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JB " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.carry_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JbeJna()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JBE " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.zero_ == 1 || flags_.carry_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JpJpe()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JP " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.parity_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnpJpo()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JNP " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.parity_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Jo()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JO " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.overflow_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Js()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JS " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.sign_ == 1)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnlJge()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JNL " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.sign_ == flags_.overflow_)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnleJg()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JNLE " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.zero_ == 0 && flags_.sign_ == flags_.overflow_)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnbJae()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JNB " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.carry_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::JnbeJa()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JNBE " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.zero_ == 0 && flags_.carry_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Jno()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JNO " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.overflow_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Jns()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JNS " + std::to_string(static_cast<int8_t>(imm)), 0);
	if (flags_.sign_ == 0)
	{
		ip_ += static_cast<int8_t>(imm);
	}

	ip_++;
}

void CpuState::Loop()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("LOOP " + std::to_string(static_cast<int8_t>(imm)), 0);
	ip_++;
}

void CpuState::LoopzLoope()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("LOOPZ " + std::to_string(static_cast<int8_t>(imm)), 0);
	ip_++;
}

void CpuState::LoopnzLoopne()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("LOOPNZ " + std::to_string(static_cast<int8_t>(imm)), 0);
	ip_++;
}

void CpuState::Jcxz()
{
	uint8_t imm = memory_->at(GetCsIp() + 1);
	DisassembleInstruction("JCXZ " + std::to_string(static_cast<int8_t>(imm)), 0);
	ip_++;
}

