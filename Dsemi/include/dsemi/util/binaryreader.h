#ifndef DSEMI_UTILITY_BINARY_READER_H
#define DSEMI_UTILITY_BINARY_READER_H

#include <iostream>

namespace dsemi {

	enum binary_mode {
		BINARY_LITTLE_ENDIAN = 0x01,
		BINARY_BIG_ENDIAN = 0x02
	};

	class binary_reader {
		friend class font_opentype;
	public:
		binary_reader(const std::string& file_path, binary_mode mode);
		~binary_reader();

		bool is_open();
		void close();

		char read_char();
		uint16_t read_uint16();
		uint32_t read_uint32();
		uint64_t read_uint64();

		std::streampos get_offset();
		void set_offset(std::streampos offset);

	private:
		bool _verify_last_read(uint64_t offset_success);

	private:
		const std::string _file_path;
		std::ifstream     _file;
		uint64_t          _offset;
		uint64_t          _length;
		char*             _buffer;

		binary_mode _mode;
		bool        _flip_endians;
	};

}

#endif