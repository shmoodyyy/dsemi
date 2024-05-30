#ifndef DSEMI_FONTS_OPENTYPE_TYPES_H
#define DSEMI_FONTS_OPENTYPE_TYPES_H
#include <vector>
#include <cstdint>

namespace dsemi {
	namespace font_opentypes_tables {

		//=====================
		// common tables
		//=====================

		struct coverage_format1 {
			uint16_t              coverage_format = 1;
			uint16_t              glyph_count;
			std::vector<uint16_t> glyph_ids;
		};

		struct coverage_format2 {
			struct range_record {
				uint16_t start_glyph_id;
				uint16_t end_glyp_id;
				uint16_t start_coverage_index;
			};

			uint16_t                  coverage_format = 2;
			uint16_t                  range_count;
			std::vector<range_record> range_records;
		};

		//=====================
		// gdef
		//=====================

		struct gdef {
			struct header {
				uint16_t major_version                = 0u;
				uint16_t minor_version                = 0u;
				uint16_t glyph_class_def_offset       = 0u;
				uint16_t attach_list_offset           = 0u;
				uint16_t lig_caret_list_offset        = 0u;
				uint16_t mark_attach_class_def_offset = 0u;
				uint16_t mark_glyph_sets_def_offset   = 0u; // GDEF Header Version 1.2 and 1.3
				uint16_t item_var_store_offset        = 0u; // GDEF Header Version 1.3
			} header;


			enum glyph_class {
				GLYPH_ZERO,
				GLYPH_BASE,
				GLYPH_LIGATURE,
				GLYPH_MARK,
				GLYPH_COMPONENT
			};

			// [range_record.start_coverage_index] defines the glyph class
			coverage_format2 glyph_class_table;

			struct attachment_point_list_table {
				struct attach_point {
					uint16_t point_count;
					std::vector<uint16_t> point_indices;
				};

				// offset from attachment_list_table
				uint16_t coverage_offset;
				uint16_t glyph_count;
				// offsets from attachment_list_table
				std::vector<uint16_t> attach_point_offsets;
				std::vector<attach_point> attach_points;
			} attachment_point_list_table;

			struct ligature_caret_list_table {
				uint16_t              coverage_offset;
				uint16_t              glyph_count;
				std::vector<uint16_t> glyph_offsets;
			} ligature_caret_list_table;

			struct ligature_glyph_table {
				uint16_t              caret_count;
				std::vector<uint16_t> caret_value_offsets;
			} ligature_glyph_table;

			enum caret_value_table_format {
				CARET_VALUE_UNDEF,
				CARET_VALUE_FORMAT1 = 1,
				CARET_VALUE_FORMAT2 = 2,
				CARET_VALUE_FORMAT3 = 3
			};
			// this is so aids, write this shit out in paper / SAI sketch please
			struct caret_value_format1 {
				
			};
		};

		//=====================
		// --
		//=====================

	}
}

#endif
