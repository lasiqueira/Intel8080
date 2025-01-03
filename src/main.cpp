#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>
#include <string>
#include <string_view>
#include "cpustate.hpp"

//TODO deduplicate identical functions (ADD, MOV, SUB, CMP, etc) by adding OP as a parameter (wait until simulation starts perhaps?)

//declarations
////helpers

void read_file(const std::string_view filename, std::vector<uint8_t> &buffer);



//implementations

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
	//TODO move offset to CpuState?
	//TODO move buffer to CpuState?

	std::string filename = argv[1];
	uint32_t offset = 0;
	//init
	std::vector<uint8_t> buffer;
	read_file(filename, buffer);	
	auto cpu_state = CpuState();

	while (offset < buffer.size())
	{
		cpu_state.DecodeInstruction(buffer, offset);
	}
	cpu_state.PrintRegisters();
	return 0;
}