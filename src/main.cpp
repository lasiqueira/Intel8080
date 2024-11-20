#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>
#include <string>
#include <string_view>

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

void get_arithmetic_imm_reg_map(std::unordered_map<uint8_t, std::string> &arithmetic_imm_reg_map)
{
	arithmetic_imm_reg_map[0x00] = "ADD";
	arithmetic_imm_reg_map[0x02] = "ADC";
	arithmetic_imm_reg_map[0x05] = "SUB";
	arithmetic_imm_reg_map[0x03] = "SBB";
	arithmetic_imm_reg_map[0x07] = "CMP";

}
std::unordered_map<uint8_t, std::string> arithmetic_imm_reg_map;

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
			std::string memory_address;
			//immediate memory address to register
			if ((d == 0 && dst == 0x06) || (d == 1 && src == 0x06)) {
				uint16_t imm = buffer[offset + 2] | buffer[offset + 3] << 8;
				memory_address = std::to_string(imm);
				offset += 2;
			}
			else {
				memory_address = d == 0 ? g_memory_addresses[dst] : g_memory_addresses[src];
			}
			
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
	//MOV memory to accumulator
	else if ((opcode>>1) == 0x50)
	{
		uint8_t w = opcode & 0x01;
		uint16_t imm = buffer[offset + 1];
		if (w == 1)
		{
			imm |= buffer[offset + 2] << 8;
		}
		std::string reg = g_register_map[0x00][w];
		std::cout << "MOV " << reg << ", [" << imm << "]" << std::endl;
		offset += 1 + w;

	}
	//MOV accumulator to memory
	else if ((opcode >> 1) == 0x51)
	{
		uint8_t w = opcode & 0x01;
		uint16_t imm = buffer[offset + 1];
		if (w == 1)
		{
			imm |= buffer[offset + 2] << 8;
		}
		std::string reg = g_register_map[0x00][w];
		std::cout << "MOV [" << imm << "], " << reg << std::endl;
		offset += 1 + w;
	}
	//ADD reg/memory with register to either
	else if(opcode >> 2 == 0x00)
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
			std::cout << "ADD " << reg_dest << ", " << reg_source << std::endl;
			offset++;

		}
		else if (mod == 0x00)
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
			if(d == 0)
			{
				std::cout << "ADD [" << memory_address << "], " << reg << std::endl;
			} else
			{
				std::cout << "ADD " << reg << ", [" << memory_address << "]" << std::endl;
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
				std::cout << "ADD [" << memory_address << "+" << +disp << "], " << reg << std::endl;
			} else
			{
				std::cout << "ADD " << reg << ", [" << memory_address << "+" << +disp << "]" << std::endl;
			}
			offset+=2;
		}
		else if(mod == 0x02)
		{
			std::string memory_address = d==0? g_memory_addresses[dst] : g_memory_addresses[src];
			std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
			uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
			if(d == 0)
			{
				std::cout << "ADD [" << memory_address << "+" << +disp << "], " << reg << std::endl;
			} else
			{
				std::cout << "ADD " << reg << ", [" << memory_address << "+" << +disp << "]" << std::endl;
			}
			offset+=3;
		}
	}
	//ADD/ADC/SUB/SBC/CMP immediate to register/memory
	else if(opcode >> 2 == 0x20)
	{
		uint8_t w = opcode & 0x01;
		uint8_t s = (opcode & 0x02) >> 1;
		uint8_t modrm = buffer[offset + 1];
		uint8_t mod = (modrm & 0xC0) >> 6;
		uint8_t arith = (modrm & 0x38) >> 3;
		uint8_t dst = modrm & 0x07;
		if(mod == 0x00)
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

			if(w == 1 && s == 0)
			{
				imm |= buffer[offset + 3] << 8;
				offset+=1;
			}
			if(s == 1)
			{
				std::cout << arithmetic_imm_reg_map[arith] << " [" << memory_address << "], " << static_cast<int16_t>(imm) << std::endl;
			}
			else 
			{
				std::cout << arithmetic_imm_reg_map[arith] <<  " [" << memory_address << "], " << imm << std::endl;
			}
			offset+=2;

		}
		else if(mod == 0x01)
		{
			std::string memory_address = g_memory_addresses[dst];
			uint8_t disp = buffer[offset + 2];
			uint16_t imm = buffer[offset + 3];
			if(w == 1 && s == 0)
			{
				imm |= buffer[offset + 4] << 8;
				offset+=1;
			}
			
			if(s == 1)
			{
				std::cout << arithmetic_imm_reg_map[arith] <<  " [" << memory_address << "+" << disp << "], " << static_cast<int16_t>(imm) << std::endl;
			}
			else
			{
				std::cout << arithmetic_imm_reg_map[arith] <<  " [" << memory_address << "+" << disp << "], " << imm << std::endl;
			}
			offset+=3;
		}
		else if(mod == 0x02)
		{
			std::string memory_address = g_memory_addresses[dst];

			uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
			uint16_t imm = buffer[offset + 4];
			if(w == 1 && s == 0)
			{
				imm |= buffer[offset + 5] << 8;
				offset+=1;
			}
			if(s == 1)
			{
				std::cout << arithmetic_imm_reg_map[arith] <<  " [" << memory_address << "+" << disp << "], " << static_cast<int16_t>(imm) << std::endl;
			}
			else
			{
				std::cout << arithmetic_imm_reg_map[arith] <<  " [" << memory_address << "+" << disp << "], " << imm << std::endl;
			}
			offset+=4;
		}
		else if(mod == 0x03)
		{
			std::string reg_dest = g_register_map[dst][w];
			uint16_t imm = buffer[offset + 2];

			if(w == 1 && s == 0)
			{
				imm |= buffer[offset + 3] << 8;
				offset+=1;
			}
			if(s == 1)
			{
				std::cout << arithmetic_imm_reg_map[arith] << " " << reg_dest << ", " << static_cast<int16_t>(imm) << std::endl;
			}
			else
			{
				std::cout << arithmetic_imm_reg_map[arith] << " "  << reg_dest << ", " << imm << std::endl;
			}
			offset+=2;

		}
	}
	//ADD immediate to accumulator
	else if (opcode >> 1 == 0x02)
	{
		uint8_t w = opcode & 0x01;
		uint16_t imm = buffer[offset + 1];
		if (w == 1)
		{
			imm |= buffer[offset + 2] << 8;
		}
		std::string reg = g_register_map[0x00][w];
		std::cout << "ADD " << reg << ", " << imm << std::endl;
		offset += 1 + w;
	}

	// SUB reg/memory with register to either
	else if(opcode >> 2 == 0x0A)
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
			std::cout << "SUB " << reg_dest << ", " << reg_source << std::endl;
			offset++;

		}
		else if (mod == 0x00)
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
			if(d == 0)
			{
				std::cout << "SUB [" << memory_address << "], " << reg << std::endl;
			} else
			{
				std::cout << "SUB " << reg << ", [" << memory_address << "]" << std::endl;
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
				std::cout << "SUB [" << memory_address << "+" << +disp << "], " << reg << std::endl;
			} else
			{
				std::cout << "SUB " << reg << ", [" << memory_address << "+" << +disp << "]" << std::endl;
			}
			offset+=2;
		}
		else if(mod == 0x02)
		{
			std::string memory_address = d==0? g_memory_addresses[dst] : g_memory_addresses[src];
			std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
			uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
			if(d == 0)
			{
				std::cout << "SUB [" << memory_address << "+" << +disp << "], " << reg << std::endl;
			} else
			{
				std::cout << "SUB " << reg << ", [" << memory_address << "+" << +disp << "]" << std::endl;
			}
			offset+=3;
		}
	}
	//SUB immediate to accumulator
	else if(opcode >> 1 == 0x16)
	{
		uint8_t w = opcode & 0x01;
		uint16_t imm = buffer[offset + 1];
		if (w == 1)
		{
			imm |= buffer[offset + 2] << 8;
		}
		std::string reg = g_register_map[0x00][w];
		std::cout << "SUB " << reg << ", " << imm << std::endl;
		offset+=1+w;
	}
	// CMP reg/memory and register
	else if(opcode >> 2 == 0x0E)
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
		//Register and Register
		if(mod == 0x03)
		{
			std::string reg_source = g_register_map[src][w];
			std::string reg_dest = g_register_map[dst][w];
			std::cout << "CMP " << reg_dest << ", " << reg_source << std::endl;
			offset++;

		}
		else if (mod == 0x00)
		{
			std::string memory_address;
			//immediate memory address with register
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
			if(d == 0)
			{
				std::cout << "CMP [" << memory_address << "], " << reg << std::endl;
			} else
			{
				std::cout << "CMP " << reg << ", [" << memory_address << "]" << std::endl;
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
				std::cout << "CMP [" << memory_address << "+" << +disp << "], " << reg << std::endl;
			} else
			{
				std::cout << "CMP " << reg << ", [" << memory_address << "+" << +disp << "]" << std::endl;
			}
			offset+=2;
		}
		else if(mod == 0x02)
		{
			std::string memory_address = d==0? g_memory_addresses[dst] : g_memory_addresses[src];
			std::string reg = d == 0 ? g_register_map[src][w] : g_register_map[dst][w];
			uint16_t disp = buffer[offset + 2] | buffer[offset + 3] << 8;
			if(d == 0)
			{
				std::cout << "CMP [" << memory_address << "+" << +disp << "], " << reg << std::endl;
			} else
			{
				std::cout << "CMP " << reg << ", [" << memory_address << "+" << +disp << "]" << std::endl;
			}
			offset+=3;
		}
	}
	// CMP accumulator with immediate
	else if (opcode >> 1 == 0x1E)
	{
		uint8_t w = opcode & 0x01;
		uint16_t imm = buffer[offset + 1];
		if (w == 1)
		{
			imm |= buffer[offset + 2] << 8;
		}
		std::string reg = g_register_map[0x00][w];
		std::cout << "CMP " << reg << ", " << imm << std::endl;
		offset += 1 + w;
	}
	//JNZ
	else if(opcode == 0x75)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JNZ " << +imm << std::endl;
		offset++;
	}
	//JE
	else if(opcode == 0x74)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JE " << +imm << std::endl;
		offset++;
	}
	//JL
	else if(opcode == 0x7C)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JL " << +imm << std::endl;
		offset++;
	}
	//JLE
	else if(opcode == 0x7E)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JLE " << +imm << std::endl;
		offset++;
	}
	//JB
	else if(opcode == 0x72)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JB " << +imm << std::endl;
		offset++;
	}
	//JBE
	else if(opcode == 0x76)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JBE " << +imm << std::endl;
		offset++;
	}
	//JP
	else if(opcode == 0x7A)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JP " << +imm << std::endl;
		offset++;
	}
	//JO
	else if(opcode == 0x70)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JO " << +imm << std::endl;
		offset++;
	}
	//JS
	else if(opcode == 0x78)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JS " << +imm << std::endl;
		offset++;
	}
	//JNE
	else if(opcode == 0x75)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JNE " << +imm << std::endl;
		offset++;
	}
	//JNL
	else if(opcode == 0x7D)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JNL " << +imm << std::endl;
		offset++;
	}
	//JG
	else if(opcode == 0x7F)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JG " << +imm << std::endl;
		offset++;
	}
	//JNB
	else if(opcode == 0x73)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JNB " << +imm << std::endl;
		offset++;
	}
	//JA
	else if(opcode == 0x77)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JA " << +imm << std::endl;
		offset++;
	}
	//JNP
	else if(opcode == 0x7B)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JNP " << +imm << std::endl;
		offset++;
	}
	//JNO
	else if(opcode == 0x71)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JNO " << +imm << std::endl;
		offset++;
	}
	//JNS
	else if(opcode == 0x79)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JNS " << +imm << std::endl;
		offset++;
	}
	//LOOP
	else if(opcode == 0xE2)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "LOOP " << +imm << std::endl;
		offset++;
	}
	//LOOPZ
	else if(opcode == 0xE1)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "LOOPZ " << +imm << std::endl;
		offset++;
	}
	//LOOPNZ
	else if(opcode == 0xE0)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "LOOPNZ " << +imm << std::endl;
		offset++;
	}
	//JCXZ
	else if(opcode == 0xE3)
	{
		int8_t imm = buffer[offset + 1];
		std::cout << "JCXZ " << +imm << std::endl;
		offset++;
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
		disassemble_8086_opcode(buffer, offset);
	}
	return 0;
}