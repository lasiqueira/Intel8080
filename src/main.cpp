#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>

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
std::unordered_map<uint8_t, std::array<std::string, 2>> g_register_map;

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

std::array<std::string, 8> g_memory_addresses;

void disassemble_8086_opcode(const std::vector<uint8_t> &buffer, uint32_t &offset)
{
	uint8_t opcode = buffer[offset];

	//MOV Register/memory to/from register
	if ((opcode >> 2) == 0x22)
	{
		uint8_t d = (opcode & 0x02) >> 1;
		uint8_t w = opcode & 0x01;
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
		//Register to Register
		if(mod == 0x03)
		{
			std::string reg_source = g_register_map[src][w];
			std::string reg_dest = g_register_map[dst][w];
			std::cout << "MOV " << reg_dest << ", " << reg_source << std::endl;
			offset++;

		}
		else if (mod == 0x00)
		{
			std::string memory_address = d==0? g_memory_addresses[dst] : g_memory_addresses[src];
			std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
			if(d == 0)
			{
				std::cout << "MOV [" << memory_address << "], " << reg << std::endl;
			} else
			{
				std::cout << "MOV " << reg << ", [" << memory_address << "]" << std::endl;
			}
			offset++;
		}
		else if (mod == 0x01)
		{	
			std::string memory_address = d==0? g_memory_addresses[dst] : g_memory_addresses[src];
			std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
			uint8_t disp = buffer[offset + 2];
			if(d == 0)
			{
				std::cout << "MOV [" << memory_address << "+" << +disp << "], " << reg << std::endl;
			} else
			{
				std::cout << "MOV " << reg << ", [" << memory_address << "+" << +disp << "]" << std::endl;
			}
			offset+=2;
		}
		else if (mod == 0x02)
		{
			std::string memory_address = d==0? g_memory_addresses[dst] : g_memory_addresses[src];
			std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
			uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
			if(d == 0)
			{
				std::cout << "MOV [" << memory_address << "+" << disp << "], " << reg << std::endl;
			}
			else
			{
				std::cout << "MOV " << reg << ", [" << memory_address << "+" << disp << "]" << std::endl;
			}
			offset += 3;
		}
		else
		{
		std::cout << "Opcode not implemented:b0=0x" << std::hex << +opcode << " b1=0x" << std::hex << +modrm << std::endl;
		exit(1);		
		}
	}
	//MOV immediate to register
	else if((opcode >> 4) == 0xB)
	{
		uint8_t w = (opcode & 0x08) >> 3;
		uint8_t reg = opcode & 0x07;
		uint16_t imm = buffer[offset + 1];
		if(w == 1)
		{
			imm |= buffer[offset + 2] << 8;
		}
		std::string reg_dest = g_register_map[reg][w];
		std::cout << "MOV " << reg_dest << ", " << imm << std::endl;
		offset+=1+w;
		
	}
	//MOV Immediate to register/memory
	else if(opcode >>1 == 0x63)
	{
		uint8_t w = opcode & 0x01;
		uint8_t modrm = buffer[offset + 1];
		uint8_t mod = (modrm & 0xC0) >> 6;
		uint8_t dst = modrm & 0x07;

		if(mod == 0x00)
		{
			std::string memory_address = g_memory_addresses[dst];
			uint16_t imm = buffer[offset + 2];
			if(w == 1)
			{
				imm |= buffer[offset + 3] << 8;
			}
			std::cout << "MOV [" << memory_address << "], " << imm << std::endl;
			offset+=2+w;

		}
		else if(mod == 0x01)
		{
			std::string memory_address = g_memory_addresses[dst];
			uint8_t disp = buffer[offset + 2];
			uint16_t imm = buffer[offset + 3];
			if(w == 1)
			{
				imm |= buffer[offset + 4] << 8;
			}
			
			std::cout << "MOV [" << memory_address << "+" << disp << "], " << imm << std::endl;
			offset+=3+w;
		}
		else if(mod == 0x02)
		{
			std::string memory_address = g_memory_addresses[dst];

			uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
			uint16_t imm = buffer[offset + 4];
			if(w == 1)
			{
				imm |= buffer[offset + 5] << 8;
			}

			std::cout << "MOV [" << memory_address << "+" << disp << "], " << imm << std::endl;
			offset+=4+w;
		}
		if(mod == 0x03)
		{
			std::string reg_dest = g_register_map[dst][w];
			uint16_t imm = buffer[offset + 2];
			if(w == 1)
			{
				imm |= buffer[offset + 3] << 8;
			}	
			std::cout << "MOV " << reg_dest << ", " << imm << std::endl;
			offset+=2+w;

		}

	}
	else
	{
		std::cout << "Opcode not implemented: b0=0x" << std::hex << +opcode << std::endl;
		exit(1);
	}
	offset++;
	
}

void read_file(const std::string_view filename, std::vector<uint8_t> &buffer)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);
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
	while (offset < buffer.size())
	{
		disassemble_8086_opcode(buffer, offset);
	}
	return 0;
}