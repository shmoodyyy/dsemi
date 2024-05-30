#include "Dsemi/fonts/opentype.h"

namespace dsemi {

	font_opentype::font_opentype()
	:   _table_directory(), 
		_table_records()
	{
	}

	bool font_opentype::_verify_table_checksum(binary_reader& current_file, const table_record& table_record)
	{
		uint32_t return_offset = current_file.get_offset();
		
		current_file.set_offset(table_record.offset);
		uint32_t sum = 0L;
		uint32_t read_steps = table_record.length / sizeof(uint32_t) + (table_record.length % sizeof(uint32_t) > 0); // account for padding
		for (uint32_t i = 0; i < read_steps; ++i)
		{
			uint32_t current_byte = current_file.read_uint32();
			sum += current_byte;
		}
		current_file.set_offset(return_offset);

		return sum == table_record.checksum;
	}

	bool font_opentype::read_file(const std::string& file_path)
	{
		binary_reader file(file_path, BINARY_BIG_ENDIAN);

		if (file.is_open())
		{
			if (!_read_table_directory(file))
			{
				_file_read_error = FILE_READ_ERROR_INVALID_FILE;
				return false;
			}


		}
		else
		{
			_file_read_error = FILE_READ_ERROR_COULD_NOT_OPEN;
			return false;
		}
	}

	bool font_opentype::_read_table_directory(binary_reader& file)
	{
		//==================
		// table directory
		//==================
		_table_directory.sfnt_version = file.read_uint32();
		_table_directory.num_tables = file.read_uint16();
		_table_directory.search_range = file.read_uint16();
		_table_directory.entry_selector = file.read_uint16();
		_table_directory.range_shift = file.read_uint16();

		//================
		// table records
		//================
		for (int i = 0; i < _table_directory.num_tables; ++i)
		{
			table_record current_table;
			for (int j = 0; j < 4; ++j)
			{
				current_table.table_tag.characters[j] = file.read_char();
			}
			std::string table_tag_str = current_table.table_tag.characters;

			if (file.get_offset() % 2 != 0)
				__debugbreak();
			current_table.checksum = file.read_uint32();
			current_table.offset = file.read_uint32();
			current_table.length = file.read_uint32();

			if (!_verify_table_checksum(file, current_table))
			{
				// special case for the head table, where the checksum is invalid due to checksumAdjustment field
				if (table_tag_str != "head")
					return false;
			}
			_table_records.emplace(
				current_table.table_tag.characters,
				current_table
			);
		}
	}

}
