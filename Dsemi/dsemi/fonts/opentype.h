#ifndef DSEMI_FONTS_OPENTYPE_H
#define DSEMI_FONTS_OPENTYPE_H
#include "dsemi/fonts/opentype_tables.h"
#include "dsemi/util/binaryreader.h"
#include <unordered_map>
#include <string>

namespace dsemi {

	enum file_read_error {
		FILE_READ_ERROR_NONE,
		FILE_READ_ERROR_WRONG_FORMAT,
		FILE_READ_ERROR_INVALID_FILE,
		FILE_READ_ERROR_COULD_NOT_OPEN
	};

	class font_opentype {
	public:
		//=====================
		// header structs
		//=====================
		struct tag {
			tag()
				: characters("")
			{}
			char characters[5];
		};

		struct table_directory {
			table_directory()
				: sfnt_version(0), num_tables(0), search_range(0), entry_selector(0), range_shift(0)
			{}
			uint32_t sfnt_version;
			uint16_t num_tables;
			uint16_t search_range;
			uint16_t entry_selector;
			uint16_t range_shift;
		};

		struct table_record {
			table_record()
				: table_tag(), checksum(0), offset(0), length(0)
			{}
			tag      table_tag;
			uint32_t checksum;
			uint32_t offset;
			uint32_t length;
		};

	public:
		font_opentype();
		
		bool read_file(const std::string& file_path);
		inline file_read_error get_error_type() const noexcept {
			return _file_read_error;
		}

		/*const table_directory& get_table_directory() const noexcept;
		const table_record& get_table_record(const tag& table_tag);*/

	private:
		table_directory _table_directory;
		std::unordered_map<std::string, table_record> _table_records;
		file_read_error _file_read_error;

		//========================
		// read_file subfunctions
		//========================
	private:
		bool _read_table_directory(binary_reader& file);
		bool _verify_table_checksum(binary_reader& current_file, const table_record& table_record);
	};

}

#endif
