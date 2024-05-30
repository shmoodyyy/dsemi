#include "dsemi/util/binaryreader.h"
#include "dsemi/util/assert.h"

namespace dsemi {

	binary_reader::binary_reader(const std::string& file_path, binary_mode mode)
		: _file(file_path, std::ios::in | std::ios::binary), _mode(mode), _file_path(file_path)
	{
		ASSERT(((mode & (BINARY_BIG_ENDIAN | BINARY_LITTLE_ENDIAN)) < 0x03), "Can't set binary mode to both big and little endian");

		_buffer = new char[8];

		// determine if the program is running on little endian architecture
		int n = 1;
		char first_byte = *(char*)&n;

		_flip_endians =
			(first_byte == 1 && (_mode & BINARY_BIG_ENDIAN)) ||
			(first_byte == 0 && (_mode & BINARY_LITTLE_ENDIAN));
	}

	binary_reader::~binary_reader()
	{
		delete[] _buffer;
		close();
	}

	bool binary_reader::is_open()
	{
		return _file.is_open();
	}

	void binary_reader::close()
	{
		if (_file.is_open())
		{
			_file.close();
		}
	}

	char binary_reader::read_char()
	{
		char out;
		do {
			_file.read(_buffer, 1u);
			out = *_buffer;
		} while (!_verify_last_read(1u));

		return out;
	}

	uint16_t binary_reader::read_uint16()
	{
		uint16_t out;
		do {
			_file.read(_buffer, 2u);
			out = *(uint16_t*)_buffer;
		} while (!_verify_last_read(2u));

		if (_flip_endians)
		{
			out =
				(out << 8) |
				(out >> 8);
		}
		return out;
	}

	uint32_t binary_reader::read_uint32()
	{
		uint32_t out;
		do {
			_file.seekg(_offset, _file.beg);
			_file.read(_buffer, 4u);
			out = *(uint32_t*)_buffer;
		} while (!_verify_last_read(4u));

		if (_flip_endians)
		{
			out =
				( out << 24)               |
				((out << 8 ) & 0x00FF0000) |
				((out >> 8 ) & 0x0000FF00) |
				( out >> 24);
		}
		return out;
	}

	uint64_t binary_reader::read_uint64()
	{
		uint64_t out;
		do {
			_file.read(_buffer, 8u);
			out = *(uint64_t*)_buffer;
		} while (!_verify_last_read(8u));

		if (_flip_endians)
		{
			out =
				( out << 56)                       |
				((out << 40) & 0x00FF000000000000) |
				((out << 28) & 0x0000FF0000000000) |
				((out << 8 ) & 0x000000FF00000000) |
				((out >> 8 ) & 0x00000000FF000000) |
				((out >> 28) & 0x0000000000FF0000) |
				((out >> 40) & 0x000000000000FF00) |
				( out >> 56);
		}
		return out;
	}

	std::streampos binary_reader::get_offset()
	{
		return _offset;
	}

	void binary_reader::set_offset(std::streampos offset)
	{
		_offset = offset;
		_file.seekg(offset, _file.beg);
	}

	bool binary_reader::_verify_last_read(uint64_t offset_success)
	{
		if(_file.fail())
		{
			// TODO: error handling for when the bits are bad D:
			if (!_file.bad())
			{
				_file.close();
				_file.open(_file_path, std::ios::in | std::ios::binary);
			}

			_file.clear();
			_file.seekg(_offset, _file.beg);
			return false;
		}
		else
		{
			// std::ifstream seems to have some random ass bullshit with their offset / tellg() being misaligned by 1 byte,
			// but the values still seem to fit? -- nvm it's just some runtime bit errors or something????
			_offset += offset_success;

			if (_offset != _file.tellg())
				_file.seekg(_offset, _file.beg);
			return true;
		}

	}
}
