#include "dxlibex/basic_types/tchar.hpp"
#include "dxlibex/utility/inferior_string_ref.hpp"
namespace dxle{
namespace win32api{
	enum class creation_disposition: DWORD {
		create_new = CREATE_NEW,
		create_overwrite = CREATE_ALWAYS,
		open_exitsint = OPEN_EXISTING,
		make_open = OPEN_ALWAYS,
		truncate_existing = TRUNCATE_EXISTING
	};
	enum class file_attributes : DWORD{
		archive = FILE_ATTRIBUTE_ARCHIVE,
		encrypted = FILE_ATTRIBUTE_ENCRYPTED,
		hidden = FILE_ATTRIBUTE_HIDDEN,
		normal = FILE_ATTRIBUTE_NORMAL,
		not_content_indexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
		offline = FILE_ATTRIBUTE_OFFLINE,
		read_only = FILE_ATTRIBUTE_READONLY,
		system = FILE_ATTRIBUTE_SYSTEM,
		temp = FILE_ATTRIBUTE_TEMPORARY
	};
	enum class file_flag : DWORD{
		write_through = FILE_FLAG_WRITE_THROUGH,
		overlapped = FILE_FLAG_OVERLAPPED,
		no_buffering = FILE_FLAG_NO_BUFFERING,
		random_access = FILE_FLAG_RANDOM_ACCESS,
		sequential_scan = FILE_FLAG_SEQUENTIAL_SCAN,
		delete_on_close = FILE_FLAG_DELETE_ON_CLOSE,
		backup_semantics = FILE_FLAG_BACKUP_SEMANTICS,
		posix_semantics = FILE_FLAG_POSIX_SEMANTICS,
		open_reparse_point = FILE_FLAG_OPEN_REPARSE_POINT,
		open_no_recall = FILE_FLAG_OPEN_NO_RECALL
	};
    class file_handle : public impl::Unique_HandledObject_Bace <file_handle, false, HANDLE>
    {
        file_handle(tinferior_string_ref filename)
    };
}
//using namespace win32api;//dxle名前空間には読み込まない
}