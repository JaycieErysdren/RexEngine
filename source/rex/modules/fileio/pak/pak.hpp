// ========================================================
//
// FILE:			/source/rex/modules/fileio/pak/pak.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		PAK namespace: header
//
// LAST EDITED:		December 21st, 2022
//
// ========================================================

#ifndef __PAK_H__
#define __PAK_H__

//
//
// PAK namespace: Header
//
//

namespace PAK
{

//
//
// Enums
//
//

typedef enum
{
	READ = 1,
	WRITE,
} handle_type;

//
//
// Classes
//
//

class FileInfo
{
	public:

		//
		// Variables
		//

		string filename;
		rex_int32 file_len;
		rex_int32 file_offset;
};

class Handle
{
	public:

		//
		// Variables
		//

		handle_type type;

		vector<FileInfo> files;

		//
		// Functions
		//

		bool Open(string filename, handle_type handle_type);
		void Close();

		bool GetFile(string filename, void **buffer, rex_int *buffer_len);

		bool AddFile(string filename, void *buffer, rex_int buffer_len);

	private:

		//
		// Variables
		//

		FILE *file_handle;
};

} // namespace PAK

#endif // __PAK_H__
