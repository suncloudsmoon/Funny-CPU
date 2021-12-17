#include <fstream>
#include <string>

#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cassert>

#include "storageutil.hpp"
#include "storage.hpp"

namespace storeutil {
	void create_empty_storage_device(const char* dest_path, UData hex_size, UData storage_size) {
		const std::string fscanf_arg = "%0" + std::to_string(hex_size) + "x";
		std::FILE* output = std::fopen(dest_path, "w+");
		if (!output) {
			throw FileNotFoundException("Unable to open file called \"" + std::string(dest_path) + "\"!");
		}
		// Printing storage size to index 0
		if (std::fprintf(output, fscanf_arg.c_str(), storage_size) < 0) {
			std::fclose(output);
			throw FileWriteException(std::strerror(errno));
		}
		if (std::fputc(' ', output) == EOF) {
			std::fclose(output);
			throw FileWriteException(std::strerror(errno));
		}

		for (unsigned int i = 0; i < storage_size; i++) {
			if (std::fprintf(output, fscanf_arg.c_str(), 0) < 0) {
				std::fclose(output);
				throw FileWriteException(std::strerror(errno));
			}
			if (std::fputc(' ', output) == EOF) {
				std::fclose(output);
				throw FileWriteException(std::strerror(errno));
			}
		}
		// Clean up memory
		std::fclose(output);
	}

	void flash_storage_to_storage(const char* src_path, const char* dest_path) {
		Storage input(src_path);
		Storage output(dest_path);

		// Fetching input size
		input.send(StorageCommand::info);
		Data input_size = input.get();
		if (input_size <= 0) {
			throw FileReadException("invalid size: " + std::to_string(input_size) + " for file " + src_path + "!");
		}

		// Fetching output size
		output.send(StorageCommand::info);
		Data output_size = output.get();
		if (output_size <= 0) {
			throw FileReadException("invalid size: " + std::to_string(output_size) + " for file " + dest_path + "!");
		}

		UData input_index = 0;
		for (UData output_index = 0; (output_index < output_size) && (input_index < input_size); output_index++) {
			input.send(StorageCommand::read, input_index);
			Data feed = input.get();

			output.send(StorageCommand::write, output_index, feed);
			output.get(); // Write command

			input_index++;
		}
	}
}

