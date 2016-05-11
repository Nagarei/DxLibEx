/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_WIN32API_WRAP_FILE_SYSTEM_FILESYSTEM_HPP_
#define DXLE_INC_WIN32API_WRAP_FILE_SYSTEM_FILESYSTEM_HPP_
#include "dxlibex/basic_types/tchar.hpp"
#include "dxlibex/utility/inferior_string_ref.hpp"
#include "dxlibex/win32api_wrap/exception.hpp"
#include <cstdint>
namespace dxle{
namespace win32api{
    enum class accsess : DWORD {//dwDesiredAccess
        zero = 0,//0
        read_control = READ_CONTROL,
        Write_dac = WRITE_DAC,
        write_owner = WRITE_OWNER,
        synchronize = SYNCHRONIZE,
        standerd_rights_required = STANDARD_RIGHTS_REQUIRED,
        standerd_rights_read = STANDARD_RIGHTS_READ,
        standerd_rights_write = STANDARD_RIGHTS_WRITE,
        standerd_rights_execute = STANDARD_RIGHTS_EXECUTE,
        standerd_rights_all = STANDARD_RIGHTS_ALL,
        specific_rights_all = SPECIFIC_RIGHTS_ALL,
        system_security = ACCESS_SYSTEM_SECURITY,
        maximum_allowed = MAXIMUM_ALLOWED,
        read = GENERIC_READ,
        write = GENERIC_WRITE,
        execute = GENERIC_EXECUTE,
        all = GENERIC_ALL,
        delete_ = DELETE
    };
	enum class share_mode : DWORD {//dwShareMode
		read = FILE_SHARE_READ,
		write = FILE_SHARE_WRITE,
		delete_ = FILE_SHARE_DELETE
	};
	enum class creation_disposition: DWORD {//dwCreationDisposition
		create_new = CREATE_NEW,
		create_overwrite = CREATE_ALWAYS,
		open_exitsting = OPEN_EXISTING,
		make_open = OPEN_ALWAYS,
		truncate_existing = TRUNCATE_EXISTING
	};
	enum class file_attribute : DWORD { normal = FILE_ATTRIBUTE_NORMAL };//dwFlagsAndAttributes
	enum class file_attributes : DWORD {//dwFlagsAndAttributes
		archive = FILE_ATTRIBUTE_ARCHIVE,
		encrypted = FILE_ATTRIBUTE_ENCRYPTED,
		hidden = FILE_ATTRIBUTE_HIDDEN,
		not_content_indexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
		offline = FILE_ATTRIBUTE_OFFLINE,
		read_only = FILE_ATTRIBUTE_READONLY,
		system = FILE_ATTRIBUTE_SYSTEM,
		temp = FILE_ATTRIBUTE_TEMPORARY
	};
	enum class file_flag : DWORD {//dwFlagsAndAttributes
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
	namespace detail {
		class flags_and_attributes {
		private:
			DWORD dwFlagsAndAttributes;
		public:
			flags_and_attributes() = default;
			flags_and_attributes(const flags_and_attributes&) = default;
			flags_and_attributes(flags_and_attributes&&) = default;
			flags_and_attributes& operator=(const flags_and_attributes&) = default;
			flags_and_attributes& operator=(flags_and_attributes&&) = default;
			flags_and_attributes(file_attribute attribute) : dwFlagsAndAttributes(static_cast<DWORD>(attribute)) {}
			flags_and_attributes(file_attributes attribute, file_flag flag) : dwFlagsAndAttributes(static_cast<DWORD>(attribute) | static_cast<DWORD>(flag)) {}
			explicit operator DWORD() { return dwFlagsAndAttributes; }
		};
	}
	detail::flags_and_attributes operator|(file_attributes attribute, file_flag flag) { return{ attribute, flag }; }
	detail::flags_and_attributes operator|(file_flag flag, file_attributes attribute) { return{ attribute, flag }; }
	enum class file_type : DWORD {
		unknown = FILE_TYPE_UNKNOWN,
		disk = FILE_TYPE_DISK,
		charactor = FILE_TYPE_CHAR,
		pipe = FILE_TYPE_PIPE,
		remote = FILE_TYPE_REMOTE
	};
	class file_handle : public impl::Unique_HandledObject_Bace <file_handle, false, HANDLE>
    {
	public:
		void open(
			tinferior_string_ref filename, accsess desired_access, share_mode share_mode, ::SECURITY_ATTRIBUTES* security_attributes,
			creation_disposition creation_disposition, detail::flags_and_attributes flags_and_attributes, HANDLE hTemplateFile = nullptr
		) DXLE_NOEXCEPT_OR_NOTHROW {
			this->set_handle(::CreateFile(
				filename.c_str(), static_cast<DWORD>(desired_access), static_cast<DWORD>(share_mode), security_attributes,
				static_cast<DWORD>(creation_disposition), static_cast<DWORD>(flags_and_attributes), hTemplateFile
			));
		}
		void open(
			tinferior_string_ref filename, accsess desired_access, share_mode share_mode, 
			creation_disposition creation_disposition, detail::flags_and_attributes flags_and_attributes, HANDLE hTemplateFile = nullptr
		) DXLE_NOEXCEPT_OR_NOTHROW {
			this->open(filename, desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, hTemplateFile);
		}
		file_handle() : Unique_HandledObject_Bace() {}
        file_handle(
			tinferior_string_ref filename, accsess desired_access, share_mode share_mode, ::SECURITY_ATTRIBUTES* security_attributes, 
			creation_disposition creation_disposition, detail::flags_and_attributes flags_and_attributes, HANDLE hTemplateFile = nullptr
		) DXLE_NOEXCEPT_OR_NOTHROW
			: Unique_HandledObject_Bace(::CreateFile(
				filename.c_str(), static_cast<DWORD>(desired_access), static_cast<DWORD>(share_mode), security_attributes, 
				static_cast<DWORD>(creation_disposition), static_cast<DWORD>(flags_and_attributes), hTemplateFile
			)){}
		file_handle(
			tinferior_string_ref filename, accsess desired_access, share_mode share_mode, 
			creation_disposition creation_disposition, detail::flags_and_attributes flags_and_attributes, HANDLE hTemplateFile = nullptr
		) DXLE_NOEXCEPT_OR_NOTHROW
			: file_handle(filename, desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, hTemplateFile) {}
		void delete_this() { ::CloseHandle(GetHandle()); }//for impl::Unique_HandledObject_Bace

		bool is_open() const { return this->is_vaid(); }
		std::uint64_t get_file_size() const {
			DXLE_RUNTIME_ERROR_THROW_WITH_MESSAGE_IF(!this->is_open(), "ファイルが開かれていません");
			LARGE_INTEGER re;
			::SetLastError(NO_ERROR);		//エラー情報をクリアする
			DXLE_WIN32_RUNTIME_ERROR_THROW_WITH_MESSAGE_IF(!::GetFileSizeEx(this->GetHandle(), &re), "");
			return re.QuadPart;
		}
		file_type get_file_type() const {
			DXLE_RUNTIME_ERROR_THROW_WITH_MESSAGE_IF(!this->is_open(), "ファイルが開かれていません");
			return static_cast<file_type>(::GetFileType(this->GetHandle()));
		}
		BY_HANDLE_FILE_INFORMATION get_info() const {
			DXLE_RUNTIME_ERROR_THROW_WITH_MESSAGE_IF(!this->is_open(), "ファイルが開かれていません");
			BY_HANDLE_FILE_INFORMATION re;
			DXLE_WIN32_RUNTIME_ERROR_THROW_WITH_MESSAGE_IF(!::GetFileInformationByHandle(this->GetHandle(), &re), "");
			return re;
		}
		HANDLE get_raw_handle() const { return this->GetHandle(); }
	};
}
//using namespace win32api;//dxle名前空間には読み込まない
}
#endif //DXLE_INC_WIN32API_WRAP_FILE_SYSTEM_FILESYSTEM_HPP_
