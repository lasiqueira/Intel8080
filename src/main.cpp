#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>
#include <string>
#include <string_view>
#include "cpustate.hpp"

//declarations
////helpers

uint32_t ReadFile(const std::string_view filename, std::array<uint8_t, 0xF4240> &buffer);
void DumpMemoryToFile(const std::string_view file_name, std::array<uint8_t, 0xF4240>& buffer);


//implementations

uint32_t ReadFile(const std::string_view file_name, std::array<uint8_t, 0xF4240> &buffer)
{
	std::ifstream file(file_name.data(), std::ios::in | std::ios::binary);
	file.unsetf(std::ios::skipws);
	std::streampos fileSize;
	file.seekg(0, std::ios::end);
	auto file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	file.read(reinterpret_cast<char*>(buffer.data()), file_size);

	file.close();

	return static_cast<uint32_t>(file_size);
}

void DumpMemoryToFile(const std::string_view file_name, std::array<uint8_t, 0xF4240>& buffer)
{
	std::ofstream file(file_name.data(), std::ios::out | std::ios::binary);
	if (!file)
	{
		std::cerr << "Failed to open file for writing: " << file_name << std::endl;
		return;
	}

	file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	file.close();
}

int main(int argc, char* argv[])
{
	std::string file_name = argv[1];
	
	//init
	auto cpu_state = CpuState(); 
	auto file_size = ReadFile(file_name, cpu_state.GetMemory());

	uint8_t stop = 0;
	while (stop != 1)
	{
		stop = cpu_state.DecodeInstruction();
	}

	cpu_state.PrintRegisters();
	cpu_state.PrintSegmentRegisters();
	cpu_state.PrintFlags();
	
	// Dump memory to file
	//DumpMemoryToFile("memory.data", cpu_state.GetMemory());

	return 0;
}