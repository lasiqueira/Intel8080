#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
void disassemble_8086_opcode(std::vector<uint8_t> &buffer, uint32_t &offset)
{
	uint8_t opcode = buffer[offset];
	switch (opcode)
	{
		
		default:
			std::cout << "Opcode not implemented: b0=0x" << std::hex << +opcode << " b1=0x" << +buffer[offset+1] << std::endl;
		offset++;
		break;
	}

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
	//TODO extract to function read to file and return an Array of bytes
	std::string filename = argv[1];
	uint32_t offset = 0;
	std::vector<uint8_t> buffer = read_file(filename);

	while (offset < buffer.size())
	{
		disassemble_8086_opcode(buffer, offset);
		offset++;
	}
	return 0;
}