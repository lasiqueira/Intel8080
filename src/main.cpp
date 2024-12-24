#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>
#include <string>
#include <string_view>

//TODO deduplicate identical functions (ADD, MOV, SUB, CMP, etc) by adding OP as a parameter (wait until simulation starts perhaps?)

//declarations
////helpers
void get_register_map(std::unordered_map<uint8_t, std::array<std::string, 2>> &register_map);
void get_memory_addresses(std::array<std::string, 8> &memory_addresses);
void get_arithmetic_imm_reg_map(std::unordered_map<uint8_t, std::string> &arithmetic_imm_reg_map);
void decode_8086_instruction(const std::vector<uint8_t> &buffer, uint32_t &offset);
void read_file(const std::string_view filename, std::vector<uint8_t> &buffer);
void disassemble_instruction(const std::string& instruction);


//ops
////MOV Register/memory to/from register
void mov_reg_mem_to_from_reg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w);
////MOV immediate to register/memory
void mov_imm_to_reg_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);
////MOV immediate to register
void mov_imm_to_reg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t reg);
////MOV memory to accumulator
void mov_mem_to_acc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);
////MOV accumulator to memory
void mov_acc_to_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);

//// mov_reg_mem_to_from_reg subfunctions
void mov_rmtfg_reg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset);
void mov_rmtfg_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void mov_rmtfg_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void mov_rmtfg_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
//// mov_imm_to_reg_mem subfunctions
void mov_itrm_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst);
void mov_itrm_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst);
void mov_itrm_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst);
void mov_itrm_reg(uint8_t dst, uint8_t w, const std::vector<uint8_t>& buffer, uint32_t& offset);

////ADD reg/memory with register to either
void add_reg_mem_with_reg_to_either(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w);

//// add_reg_mem_with_reg_to_either subfunctions
void add_rmwrtw_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void add_rmwrtw_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void add_rmwrtw_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void add_rmwrtw_reg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset);

//// ADD/ADC/SUB/SBC/CMP immediate to register/memory
void add_adc_sub_sbc_cmp_imm_to_reg_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w);

//// add_adc_sub_sbc_cmp_imm_to_reg_mem subfunctions
void add_adc_sub_sbc_cmp_itrm_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith);
void add_adc_sub_sbc_cmp_itrm_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith);
void add_adc_sub_sbc_cmp_itrm_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith);
void add_adc_sub_sbc_cmp_itrm_reg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith);

//// ADD immediate to accumulator
void add_imm_to_acc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);

//// SUB reg/memory with register to either
void sub_reg_mem_with_reg_to_either(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w);

//// sub_reg_mem_with_reg_to_either subfunctions
void sub_rmwrtw_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void sub_rmwrtw_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void sub_rmwrtw_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void sub_rmwrtw_reg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset);

//// SUB immediate to accumulator
void sub_imm_to_acc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);

//// CMP reg / memory and register
void cmp_reg_mem_and_reg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w);

//// cmp_reg_mem_and_reg subfunctions
void cmp_rmar_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void cmp_rmar_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void cmp_rmar_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst);
void cmp_rmar_reg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset);

//// CMP immediate to register
void cmp_imm_with_acc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w);

//// JNE/JNZ
void jne_jnz(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JE/JZ
void je_jz(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JL/JNGE
void jl_jnge(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JLE/JNG
void jle_jng(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JB/JNAE
void jb_jnae(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JBE/JNA
void jbe_jna(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JP/JPE
void jp_jpe(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JO
void jo(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JS
void js(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JNL/JGE
void jnl_jge(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JNLE/JG
void jnle_jg(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JNB/JAE
void jnb_jae(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JNBE/JA
void jnbe_ja(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JNP/JPO
void jnp_jpo(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JNO
void jno(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JNS
void jns(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// LOOP
void loop(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// LOOPZ/LOOPE
void loopz_loope(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// LOOPNZ/LOOPNE
void loopnz_loopne(const std::vector<uint8_t>& buffer, uint32_t& offset);

//// JCXZ
void jcxz(const std::vector<uint8_t>& buffer, uint32_t& offset);


//globals
std::unordered_map<uint8_t, std::array<std::string, 2>> g_register_map;
std::array<std::string, 8> g_memory_addresses;
std::unordered_map<uint8_t, std::string> arithmetic_imm_reg_map;


//implementations

void disassemble_instruction(const std::string& instruction) 
{
	std::cout << instruction << std::endl;
}

// LOOP
void loop(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "LOOP " << +imm << std::endl;
	offset++;
}

// LOOPZ/LOOPE
void loopz_loope(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "LOOPZ/LOOPE " << +imm << std::endl;
	offset++;
}

// LOOPNZ/LOOPNE
void loopnz_loopne(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "LOOPNZ/LOOPNE " << +imm << std::endl;
	offset++;
}

// JCXZ
void jcxz(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JCXZ " << +imm << std::endl;
	offset++;
}

// JNE/JNZ
void jne_jnz(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JNE/JNZ " << +imm << std::endl;
	offset++;
}

// JE/JZ
void je_jz(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JE/JZ " << +imm << std::endl;
	offset++;
}

// JL/JNGE
void jl_jnge(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JL/JNGE " << +imm << std::endl;
	offset++;
}

// JLE/JNG

void jle_jng(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JLE/JNG " << +imm << std::endl;
	offset++;
}

// JB/JNAE
void jb_jnae(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JB/JNAE " << +imm << std::endl;
	offset++;
}

// JBE/JNA
void jbe_jna(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JBE/JNA " << +imm << std::endl;
	offset++;
}

// JP/JPE
void jp_jpe(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JP/JPE " << +imm << std::endl;
	offset++;
}

// JO
void jo(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JO " << +imm << std::endl;
	offset++;
}

// JS
void js(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JS " << +imm << std::endl;
	offset++;
}

// JNL/JGE
void jnl_jge(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JNL/JGE " << +imm << std::endl;
	offset++;
}

// JNLE/JG
void jnle_jg(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JNLE/JG " << +imm << std::endl;
	offset++;
}

// JNB/JAE
void jnb_jae(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JNB/JAE " << +imm << std::endl;
	offset++;
}

// JNBE/JA
void jnbe_ja(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JNBE/JA " << +imm << std::endl;
	offset++;
}

// JNP/JPO
void jnp_jpo(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JNP/JPO " << +imm << std::endl;
	offset++;
}

// JNO
void jno(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JNO " << +imm << std::endl;
	offset++;
}

// JNS
void jns(const std::vector<uint8_t>& buffer, uint32_t& offset)
{
	int8_t imm = buffer[offset + 1];
	std::cout << "JNS " << +imm << std::endl;
	offset++;
}

// CMP immediate to register
void cmp_imm_with_acc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	std::string reg_dest = g_register_map[0x00][w];
	disassemble_instruction("CMP " + reg_dest + ", " + std::to_string(imm));
	offset += 1 + w;
}

// CMP reg / memory and register
void cmp_reg_mem_and_reg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w)
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
	case 0x00: cmp_rmar_mem(buffer, offset, d, w, src, dst); break;
	case 0x01: cmp_rmar_mem_disp_8(buffer, offset, d, w, src, dst); break;
	case 0x02: cmp_rmar_mem_disp_16(buffer, offset, d, w, src, dst); break;
	case 0x03: cmp_rmar_reg(src, dst, w, offset); break;
	default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void cmp_rmar_reg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset)
{
	std::string reg_source = g_register_map[src][w];
	std::string reg_dest = g_register_map[dst][w];
	disassemble_instruction("CMP " + reg_dest + ", " + reg_source);
	offset++;
}

void cmp_rmar_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	if (d == 0)
	{
		disassemble_instruction("CMP [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		disassemble_instruction("CMP " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 3;
}

void cmp_rmar_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	uint8_t disp = buffer[offset + 2];
	if (d == 0)
	{
		disassemble_instruction("CMP [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		disassemble_instruction("CMP " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 2;
}

void cmp_rmar_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
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
		memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	}
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	if (d == 0)
	{
		disassemble_instruction("CMP [" + memory_address + "], " + reg);
	}
	else
	{
		disassemble_instruction("CMP " + reg + ", [" + memory_address + "]");
	}
	offset++;
}

//SUB immediate to accumulator
void sub_imm_to_acc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	std::string reg = g_register_map[0x00][w];
	disassemble_instruction("SUB " + reg + ", " + std::to_string(imm));
	offset += 1 + w;
}

//SUB reg/memory with register to either
void sub_reg_mem_with_reg_to_either(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w)
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
		case 0x00: sub_rmwrtw_mem(buffer, offset, d, w, src, dst); break;
		case 0x01: sub_rmwrtw_mem_disp_8(buffer, offset, d, w, src, dst); break;
		case 0x02: sub_rmwrtw_mem_disp_16(buffer, offset, d, w, src, dst); break;
		case 0x03: sub_rmwrtw_reg(src, dst, w, offset); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void sub_rmwrtw_reg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset)
{
	std::string reg_source = g_register_map[src][w];
	std::string reg_dest = g_register_map[dst][w];
	disassemble_instruction("SUB " + reg_dest + ", " + reg_source);
	offset++;
}

void sub_rmwrtw_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	if (d == 0)
	{
		disassemble_instruction("SUB [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		disassemble_instruction("SUB " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 3;
}

void sub_rmwrtw_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	uint8_t disp = buffer[offset + 2];
	if (d == 0)
	{
		disassemble_instruction("SUB [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		disassemble_instruction("SUB " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 2;
}

void sub_rmwrtw_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
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
		memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	}
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	if (d == 0)
	{
		disassemble_instruction("SUB [" + memory_address + "], " + reg);
	}
	else
	{
		disassemble_instruction("SUB " + reg + ", [" + memory_address + "]");
	}
	offset++;
}

//ADD immediate to accumulator
void add_imm_to_acc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	std::string reg = g_register_map[0x00][w];
	disassemble_instruction("ADD " + reg + ", " + std::to_string(imm));
	offset += 1 + w;
}

void add_adc_sub_sbc_cmp_itrm_reg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith)
{
	std::string reg_dest = g_register_map[dst][w];
	uint16_t imm = buffer[offset + 2];

	if (w == 1 && s == 0)
	{
		imm |= buffer[offset + 3] << 8;
		offset += 1;
	}
	if (s == 1)
	{
		disassemble_instruction(arithmetic_imm_reg_map[arith] + " " + reg_dest + ", " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		disassemble_instruction(arithmetic_imm_reg_map[arith] + " " + reg_dest + ", " + std::to_string(imm));
	}
	offset += 2;
}

void add_adc_sub_sbc_cmp_itrm_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith)
{
	std::string memory_address = g_memory_addresses[dst];
	std::string reg = g_register_map[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	uint16_t imm = buffer[offset + 4];
	if (w == 1 && s == 0)
	{
		imm |= buffer[offset + 5] << 8;
		offset += 1;
	}
	if (s == 1)
	{
		disassemble_instruction(arithmetic_imm_reg_map[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		disassemble_instruction(arithmetic_imm_reg_map[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm));
	}
	offset += 4;
}

void add_adc_sub_sbc_cmp_itrm_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith)
{
	std::string memory_address = g_memory_addresses[dst];
	std::string reg = g_register_map[dst][w];
	uint8_t disp = buffer[offset + 2];
	uint16_t imm = buffer[offset + 3];
	if (w == 1 && s == 0)
	{
		imm |= buffer[offset + 4] << 8;
		offset += 1;
	}
	if (s == 1)
	{
		disassemble_instruction(arithmetic_imm_reg_map[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
		disassemble_instruction(arithmetic_imm_reg_map[arith] + " [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm));
	}
	offset += 3;
}

void add_adc_sub_sbc_cmp_itrm_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w, uint8_t dst, uint8_t arith)
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
		memory_address = g_memory_addresses[dst];
	}
	uint16_t imm = buffer[offset + 2];

	if (w == 1 && s == 0)
	{
		imm |= buffer[offset + 3] << 8;
		offset += 1;
	}
	if (s == 1)
    {
        disassemble_instruction(arithmetic_imm_reg_map[arith] + " [" + memory_address + "], " + std::to_string(static_cast<int16_t>(imm)));
	}
	else
	{
        disassemble_instruction(arithmetic_imm_reg_map[arith] + " [" + memory_address + "], " + std::to_string(imm));
	}
	offset += 2;
}

//ADD/ADC/SUB/SBC/CMP immediate to register/memory
void add_adc_sub_sbc_cmp_imm_to_reg_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t s, uint8_t w)
{
	uint8_t modrm = buffer[offset + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t arith = (modrm & 0x38) >> 3;
	uint8_t dst = modrm & 0x07;
	switch (mod)
	{
		case 0x00: add_adc_sub_sbc_cmp_itrm_mem(buffer, offset, s, w, dst, arith); break;
		case 0x01: add_adc_sub_sbc_cmp_itrm_mem_disp_8(buffer, offset, s, w, dst, arith); break;
		case 0x02: add_adc_sub_sbc_cmp_itrm_mem_disp_16(buffer, offset, s, w, dst, arith); break;
		case 0x03: add_adc_sub_sbc_cmp_itrm_reg(buffer, offset, s, w, dst, arith); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

void add_rmwrtw_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst) 
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
		memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	}

	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	if (d == 0)
	{
        disassemble_instruction("ADD [" + memory_address + "], " + reg);
	}
	else
	{
        disassemble_instruction("ADD " + reg + ", [" + memory_address + "]");
	}
	offset++;
}

void add_rmwrtw_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst) 
{
	std::string memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	uint8_t disp = buffer[offset + 2];
	if (d == 0)
	{
        disassemble_instruction("ADD [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
        disassemble_instruction("ADD " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 2;
}

void add_rmwrtw_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	if (d == 0)
	{
		disassemble_instruction("ADD [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		disassemble_instruction("ADD " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 3;
}

void add_rmwrtw_reg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset)
{
	std::string reg_source = g_register_map[src][w];
	std::string reg_dest = g_register_map[dst][w];
	disassemble_instruction("ADD " + reg_dest + ", " + reg_source);
	offset++;
}

//ADD reg/memory with register to either
void add_reg_mem_with_reg_to_either(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w)
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
		case 0x00: add_rmwrtw_mem(buffer, offset, d, w, src, dst); break;
		case 0x01: add_rmwrtw_mem_disp_8(buffer, offset, d, w, src, dst); break;
		case 0x02: add_rmwrtw_mem_disp_16(buffer, offset, d, w, src, dst); break;
		case 0x03: add_rmwrtw_reg(src, dst, w, offset); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}

//MOV accumulator to memory
void mov_acc_to_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	std::string reg = g_register_map[0x00][w];
	disassemble_instruction("MOV [" + std::to_string(imm) + "], " + reg);
	offset += 1 + w;
}

//MOV memory to accumulator
void mov_mem_to_acc(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	std::string reg = g_register_map[0x00][w];
	disassemble_instruction("MOV " + reg + ", [" + std::to_string(imm) + "]");
	offset += 1 + w;
}

//MOV immediate to register
void mov_imm_to_reg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t reg)
{
	uint16_t imm = buffer[offset + 1];
	if (w == 1)
	{
		imm |= buffer[offset + 2] << 8;
	}
	std::string reg_dest = g_register_map[reg][w];
	disassemble_instruction("MOV " + reg_dest + ", " + std::to_string(imm));
	offset += 1 + w;
}

void mov_itrm_reg(uint8_t dst, uint8_t w, const std::vector<uint8_t>& buffer, uint32_t& offset) 
{
	std::string reg_dest = g_register_map[dst][w];
	uint16_t imm = buffer[offset + 2];
	if (w == 1) {
		imm |= buffer[offset + 3] << 8;
	}
	disassemble_instruction("MOV " + reg_dest + ", " + std::to_string(imm));
	offset += 2 + w;
}

void mov_itrm_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst) 
{
	std::string memory_address = g_memory_addresses[dst];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	uint16_t imm = buffer[offset + 4];
	if (w == 1) {
		imm |= buffer[offset + 5] << 8;
	}
	disassemble_instruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm));
	offset += 4 + w;
}

void mov_itrm_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst) 
{
	std::string memory_address = g_memory_addresses[dst];
	uint8_t disp = buffer[offset + 2];
	uint16_t imm = buffer[offset + 3];
	if (w == 1) {
		imm |= buffer[offset + 4] << 8;
	}
	disassemble_instruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + std::to_string(imm));
	offset += 3 + w;
}

void mov_itrm_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w, uint8_t dst) 
{
	std::string memory_address = g_memory_addresses[dst];
	uint16_t imm = buffer[offset + 2];
	if (w == 1) {
		imm |= buffer[offset + 3] << 8;
	}
	disassemble_instruction("MOV [" + memory_address + "], " + std::to_string(imm));
	offset += 2 + w;
}

//MOV immediate to register/memory
void mov_imm_to_reg_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t w)
{
	uint8_t modrm = buffer[offset + 1];
	uint8_t mod = (modrm & 0xC0) >> 6;
	uint8_t dst = modrm & 0x07;

	switch (mod)
	{
		case 0x00: mov_itrm_mem(buffer, offset, w, dst); break;
		case 0x01: mov_itrm_mem_disp_8(buffer, offset, w, dst); break;
		case 0x02: mov_itrm_mem_disp_16(buffer, offset, w, dst); break;
		case 0x03: mov_itrm_mem(buffer, offset, w, dst); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
}
void mov_rmtfg_mem_disp_16(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
	if (d == 0)
	{
		disassemble_instruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);
	}
	else
	{
		disassemble_instruction("MOV " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");
	}
	offset += 3;
}
void mov_rmtfg_mem_disp_8(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst)
{
	std::string memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	uint8_t disp = buffer[offset + 2];
	if (d == 0) 
	{
		disassemble_instruction("MOV [" + memory_address + "+" + std::to_string(disp) + "], " + reg);

	}
	else 
	{
		disassemble_instruction("MOV " + reg + ", [" + memory_address + "+" + std::to_string(disp) + "]");

	}
	offset += 2;
}
void mov_rmtfg_mem(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w, uint8_t src, uint8_t dst) 
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
		memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
	}
	std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
	if (d == 0)
	{
		disassemble_instruction("MOV [" + memory_address + "], " + reg);
	}
	else
	{
		disassemble_instruction("MOV " + reg + ", [" + memory_address + "]");
	}
	offset++;
}
void mov_rmtfg_reg(uint8_t src, uint8_t dst, uint8_t w, uint32_t& offset) {
	std::string reg_source = g_register_map[src][w];
	std::string reg_dest = g_register_map[dst][w];
	disassemble_instruction("MOV " + reg_dest + ", " + reg_source);
	offset++;
}

//MOV Register/memory to/from register
void mov_reg_mem_to_from_reg(const std::vector<uint8_t>& buffer, uint32_t& offset, uint8_t d, uint8_t w)
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
		case 0x00: mov_rmtfg_mem(buffer, offset, d, w, src, dst); break;
		case 0x01: mov_rmtfg_mem_disp_8(buffer, offset, d, w, src, dst); break;
		case 0x02: mov_rmtfg_mem_disp_16(buffer, offset, d, w, src, dst); break;
		case 0x03: mov_rmtfg_reg(src, dst, w, offset); break;
		default: std::cout << "Invalid MOD value: " << mod << std::endl; exit(1);
	}
	

}
void get_register_map(std::unordered_map<uint8_t, std::array<std::string, 2>> &register_map)
{
	register_map[0x00] = {"AL", "AX"};
	register_map[0x01] = {"CL", "CX"};
	register_map[0x02] = {"DL", "DX"};
	register_map[0x03] = {"BL", "BX"};
	register_map[0x04] = {"AH", "SP"};
	register_map[0x05] = {"CH", "BP"};
	register_map[0x06] = {"DH", "SI"};
	register_map[0x07] = {"BH", "DI"};

}

void get_memory_addresses(std::array<std::string, 8> &memory_addresses)
{
	memory_addresses[0x00] = "BX+SI";
	memory_addresses[0x01] = "BX+DI";
	memory_addresses[0x02] = "BP+SI";
	memory_addresses[0x03] = "BP+DI";
	memory_addresses[0x04] = "SI";
	memory_addresses[0x05] = "DI";
	memory_addresses[0x06] = "BP";
	memory_addresses[0x07] = "BX";
}


void get_arithmetic_imm_reg_map(std::unordered_map<uint8_t, std::string> &arithmetic_imm_reg_map)
{
	arithmetic_imm_reg_map[0x00] = "ADD";
	arithmetic_imm_reg_map[0x02] = "ADC";
	arithmetic_imm_reg_map[0x05] = "SUB";
	arithmetic_imm_reg_map[0x03] = "SBB";
	arithmetic_imm_reg_map[0x07] = "CMP";

}

void decode_8086_instruction(const std::vector<uint8_t> &buffer, uint32_t &offset)
{
	uint8_t opcode = buffer[offset];
	switch (opcode)
	{
		case 0x88: mov_reg_mem_to_from_reg(buffer, offset, 0, 0); break;
		case 0x89: mov_reg_mem_to_from_reg(buffer, offset, 0, 1); break;
		case 0x8A: mov_reg_mem_to_from_reg(buffer, offset, 1, 0); break;
		case 0x8B: mov_reg_mem_to_from_reg(buffer, offset, 1, 1); break;
		case 0xC6: mov_imm_to_reg_mem(buffer, offset, 0); break;
		case 0xC7: mov_imm_to_reg_mem(buffer, offset, 1); break;
		case 0xB0: mov_imm_to_reg(buffer, offset, 0, 0x00); break;
		case 0xB1: mov_imm_to_reg(buffer, offset, 0, 0x01); break;
		case 0xB2: mov_imm_to_reg(buffer, offset, 0, 0x02); break;
		case 0xB3: mov_imm_to_reg(buffer, offset, 0, 0x03); break;
		case 0xB4: mov_imm_to_reg(buffer, offset, 0, 0x04); break;
		case 0xB5: mov_imm_to_reg(buffer, offset, 0, 0x05); break;
		case 0xB6: mov_imm_to_reg(buffer, offset, 0, 0x06); break;
		case 0xB7: mov_imm_to_reg(buffer, offset, 0, 0x07); break;
		case 0xB8: mov_imm_to_reg(buffer, offset, 1, 0x00); break;
		case 0xB9: mov_imm_to_reg(buffer, offset, 1, 0x01); break;
		case 0xBA: mov_imm_to_reg(buffer, offset, 1, 0x02); break;
		case 0xBB: mov_imm_to_reg(buffer, offset, 1, 0x03); break;
		case 0xBC: mov_imm_to_reg(buffer, offset, 1, 0x04); break;
		case 0xBD: mov_imm_to_reg(buffer, offset, 1, 0x05); break;
		case 0xBE: mov_imm_to_reg(buffer, offset, 1, 0x06); break;
		case 0xBF: mov_imm_to_reg(buffer, offset, 1, 0x07); break;
		case 0xA0: mov_mem_to_acc(buffer, offset, 0); break;
		case 0xA1: mov_mem_to_acc(buffer, offset, 1); break;
		case 0xA2: mov_acc_to_mem(buffer, offset, 0); break;
		case 0xA3: mov_acc_to_mem(buffer, offset, 1); break;
		case 0x00: add_reg_mem_with_reg_to_either(buffer, offset, 0, 0); break;
		case 0x01: add_reg_mem_with_reg_to_either(buffer, offset, 0, 1); break;
		case 0x02: add_reg_mem_with_reg_to_either(buffer, offset, 1, 0); break;
		case 0x03: add_reg_mem_with_reg_to_either(buffer, offset, 1, 1); break;
		case 0x80: add_adc_sub_sbc_cmp_imm_to_reg_mem(buffer, offset, 0, 0); break;
		case 0x81: add_adc_sub_sbc_cmp_imm_to_reg_mem(buffer, offset, 0, 1); break;
		case 0x82: add_adc_sub_sbc_cmp_imm_to_reg_mem(buffer, offset, 1, 0); break;
		case 0x83: add_adc_sub_sbc_cmp_imm_to_reg_mem(buffer, offset, 1, 1); break;
		case 0x04: add_imm_to_acc(buffer, offset, 0); break;
		case 0x05: add_imm_to_acc(buffer, offset, 1); break;
		case 0x28: sub_reg_mem_with_reg_to_either(buffer, offset, 0, 0); break;
		case 0x29: sub_reg_mem_with_reg_to_either(buffer, offset, 0, 1); break;
		case 0x2A: sub_reg_mem_with_reg_to_either(buffer, offset, 1, 0); break;
		case 0x2B: sub_reg_mem_with_reg_to_either(buffer, offset, 1, 1); break;
		case 0x2C: sub_imm_to_acc(buffer, offset, 0); break;
		case 0x2D: sub_imm_to_acc(buffer, offset, 1); break;
		case 0x38: cmp_reg_mem_and_reg(buffer, offset, 0, 0); break;
		case 0x39: cmp_reg_mem_and_reg(buffer, offset, 0, 1); break;
		case 0x3A: cmp_reg_mem_and_reg(buffer, offset, 1, 0); break;
		case 0x3B: cmp_reg_mem_and_reg(buffer, offset, 1, 1); break;
		case 0x3C: cmp_imm_with_acc(buffer, offset, 0); break;
		case 0x3D: cmp_imm_with_acc(buffer, offset, 1); break;
		case 0x74: je_jz(buffer, offset); break;
		case 0x75: jne_jnz(buffer, offset); break;
		case 0x7C: jl_jnge(buffer, offset); break;
		case 0x7E: jle_jng(buffer, offset); break;
		case 0x72: jb_jnae(buffer, offset); break;
		case 0x76: jbe_jna(buffer, offset); break;
		case 0x7A: jp_jpe(buffer, offset); break;
		case 0x7B: jnp_jpo(buffer, offset); break;
		case 0x70: jo(buffer, offset); break;
		case 0x78: js(buffer, offset); break;
		case 0x7D: jnl_jge(buffer, offset); break;
		case 0x7F: jnle_jg(buffer, offset); break;
		case 0x73: jnb_jae(buffer, offset); break;
		case 0x77: jnbe_ja(buffer, offset); break;
		case 0x71: jno(buffer, offset); break;
		case 0x79: jns(buffer, offset); break;
		case 0xE2: loop(buffer, offset); break;
		case 0xE1: loopz_loope(buffer, offset); break;
		case 0xE0: loopnz_loopne(buffer, offset); break;
		case 0xE3: jcxz(buffer, offset); break;

		default: std::cout << "Opcode not implemented:b0=0x" << std::hex << +opcode << std::endl; exit(1);
	}
	offset++;
	
}

void read_file(const std::string_view filename, std::vector<uint8_t> &buffer)
{
	std::ifstream file(filename.data(), std::ios::in | std::ios::binary);
	file.unsetf(std::ios::skipws);
	std::streampos fileSize;
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	buffer.reserve(fileSize);

	buffer.insert(buffer.begin(),
		std::istream_iterator<uint8_t>(file),
		std::istream_iterator<uint8_t>());

	file.close();
}

int main(int argc, char* argv[])
{
	std::string filename = argv[1];
	uint32_t offset = 0;
	//init
	std::vector<uint8_t> buffer;
	read_file(filename, buffer);
	get_register_map(g_register_map);
	get_memory_addresses(g_memory_addresses);
	get_arithmetic_imm_reg_map(arithmetic_imm_reg_map);
	while (offset < buffer.size())
	{
		decode_8086_instruction(buffer, offset);
	}
	return 0;
}