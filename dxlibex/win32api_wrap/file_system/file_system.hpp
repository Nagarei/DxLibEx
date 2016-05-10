#include "dxlibex/basic_types/tchar.hpp"
#include "dxlibex/utility/inferior_string_ref.hpp"
namespace dxle{
namespace win32api{
    class file_handle : public impl::Unique_HandledObject_Bace <file_handle, false, HANDLE>
    {
        file_handle(tinferior_string_ref filename)
    };
}
//using namespace win32api;//dxle名前空間には読み込まない
}