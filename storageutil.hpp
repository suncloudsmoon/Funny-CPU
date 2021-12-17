#ifndef STORAGEUTIL_HPP
#define STORAGEUTIL_HPP

#include <stdexcept>

#include "storage.hpp"

namespace storeutil {
	/*
	 * Creates a new file using the "w+" option in fopen() api
	 */
	void create_empty_storage_device(const char* dest_path, UData hex_size, UData storage_size);
	/*
	 * Flashes one storage device's data to another
	 */
	void flash_storage_to_storage(const char* src_path, const char* dest_path);

	class FileNotFoundException : public std::runtime_error {
	public:
		FileNotFoundException(const std::string& error_message) : std::runtime_error("FileNotFoundException: " + error_message) {}
	};

	class FileReadException : public std::runtime_error {
	public:
		FileReadException(const std::string& error_message) : std::runtime_error("FileReadException: " + error_message) {}
	};

	class FileWriteException : public std::runtime_error {
	public:
		FileWriteException(const std::string &error_message) : std::runtime_error("FileWriteException: " + error_message) {}
	};
}


#endif /* STORAGEUTIL_HPP */