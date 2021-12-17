#include <iostream>

#include "assembler.hpp"
#include "storageutil.hpp"

#include "cpu.hpp"
#include "bus.hpp"
#include "memory.hpp"
#include "storage.hpp"

static void assembly_to_storage(const char* input_asm, const char* output_asm, const char* storage_file, UData storage_size);
static void start_cpu();

int main() {
	assembly_to_storage("bios.fa", "bios.fhex", "storage.fst", 512);
	start_cpu();
}

static void assembly_to_storage(const char *input_asm, const char *output_asm, const char *storage_file, UData storage_size) {
	Assembler translator;
	translator.assemble(input_asm, output_asm);

	storeutil::create_empty_storage_device(storage_file, HEX_SIZE, storage_size);
	storeutil::flash_storage_to_storage(output_asm, storage_file);
}

static void start_cpu() {
	Memory mem(512);
	Storage bios("bios.fhex");
	
	Bus bus;
	bus.connect(MEMORY_PORT, mem);
	bus.connect(BIOS_PORT, bios);

	CPU cpu(bus);
	cpu.start();
}

