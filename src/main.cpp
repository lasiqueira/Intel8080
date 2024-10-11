#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>

std::unordered_map<uint8_t, std::array<std::string, 2>> get_register_map()
{
	std::unordered_map<uint8_t, std::array<std::string, 2>> register_map;
	register_map[0x00] = {"AL", "AX"};
	register_map[0x01] = {"CL", "CX"};
	register_map[0x02] = {"DL", "DX"};
	register_map[0x03] = {"BL", "BX"};
	register_map[0x04] = {"AH", "SP"};
	register_map[0x05] = {"CH", "BP"};
	register_map[0x06] = {"DH", "SI"};
	register_map[0x07] = {"BH", "DI"};
	return register_map;

}
auto g_register_map = get_register_map();

void disassemble_8086_opcode(std::vector<uint8_t> &buffer, uint32_t &offset)
{
	uint8_t opcode = buffer[offset];
	
	//MOV Register/memory to/from register
	if (opcode >> 2 == 0x22)
	{
		uint8_t d = (opcode >> 1) & 0x01;
		uint8_t w = opcode & 0x01;
		uint8_t modrm = buffer[offset + 1];
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

		std::cout << "MOV " << g_register_map[dst][w] << ", " << g_register_map[src][w] << std::endl;
		offset++;
	}
	
	else
	{
		std::cout << "Opcode not implemented: b0=0x" << std::hex << +opcode << std::endl;
		exit(1);
	}
	offset++;
	
}

std::vector<uint8_t> read_file(const std::string& filename)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	file.unsetf(std::ios::skipws);
	std::streampos fileSize;
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<uint8_t> buffer;
	buffer.reserve(fileSize);

	buffer.insert(buffer.begin(),
		std::istream_iterator<uint8_t>(file),
		std::istream_iterator<uint8_t>());

	file.close();
	return buffer;
}

int main(int argc, char* argv[])
{
	std::string filename = argv[1];
	uint32_t offset = 0;
	auto buffer = read_file(filename);

	while (offset < buffer.size())
	{
		disassemble_8086_opcode(buffer, offset);
	}
	return 0;
}