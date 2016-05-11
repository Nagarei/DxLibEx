#include "dxlibex/basic_types/tchar.hpp"
#include "dxlibex/utility/inferior_string_ref.hpp"
namespace dxle{
namespace win32api{
    enum class accsess : DWORD {
        zero = 0,//0
        read = generic_ = GENERIC_READ,
        write = GENERIC_WRITE,
        
        delete_ = DELETE,
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
        generic_read = GENERIC_READ,
        generic_write = GENERIC_WRITE,
        generic_ = GENERIC_EXECUTE,
        generic_ = GENERIC_ALL
    };
    
    class file_handle : public impl::Unique_HandledObject_Bace <file_handle, false, HANDLE>
    {
        file_handle(tinferior_string_ref filename) : Unique_HandledObject_Bace(::CreateFile(filename.c_str())){}
    };
}
//using namespace win32api;//dxle名前空間には読み込まない
}