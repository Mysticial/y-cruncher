/* RawFile_Windows.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/20/2011
 * Last Modified    : 03/24/2018
 * 
 *      There are two key components to achieving zero-overhead disk access on
 *  Windows: FILE_FLAG_NO_BUFFERING and SetFileValidData().
 * 
 *  Note that this has only really been tested for NTFS. It is possible that
 *  this implementation may not work as well on other file systems.
 * 
 * 
 *      FILE_FLAG_NO_BUFFERING is needed because the disk caching behavior on
 *  Windows is so bad that it literally commits suicide.
 * 
 *  For random access on very large files, it has been observed that the OS
 *  tries so hard to cache the file that it pages itself out of memory and
 *  terminally hangs the system in a pagefile "thrash of death". This is the
 *  case for all buffering modes other than FILE_FLAG_NO_BUFFERING.
 *
 *  In other words, FILE_FLAG_NO_BUFFERING is mandatory. Without it, Swap Mode
 *  will not work on Windows. At least not for very large computations that
 *  require many times more disk than can fit in memory.
 * 
 * 
 *      SetFileValidData() is needed to allocate large files without zeroing
 *  overhead. When a file is created and written to non-sequentially, all
 *  unwritten regions before the write are zeroed. This is done for security
 *  reasons to avoid exposing deleted data on the disk.
 * 
 *  However, this zeroing is very expensive - especially if the files are many
 *  terabytes large. SetFileValidData() allows you to skip this zeroing.
 *  But since it exposes raw data on the disk, it is a potential security risk.
 *  So the function requires admin privileges to call.
 * 
 *  SetFileValidData() is a (very large) performance optimization. But it
 *  nevertheless is not mandatory. Computations will still work without it, but
 *  at drastically reduced performance.
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "Settings.h"
#include <algorithm>
#include <Windows.h>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/SystemLibs/Environment/Environment.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "RawFile.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Errors
struct FailPrinter_SetFileValidData{
    FailPrinter_SetFileValidData(const std::string& path){
        Console::Warning(
            "SetFileValidData() has failed. Expect performance degradation.\n"
            "Try re-running as administrator.\n"
        );
        Console::println_labelc("Path: ", path, 'Y');
        Console::Warning("Further messages for this warning will be suppressed.\n");
    }
};
void warn_SetFileValidData_fail(const std::string& path){
    static FailPrinter_SetFileValidData printer(path);
}
void handle_error(DWORD errorcode, const char* function, std::string path, std::string msg){
    msg += "\n\n";
    switch (errorcode){
        case ERROR_ACCESS_DENIED:
            msg += "Access Denied";
            break;
        case ERROR_FILE_NOT_FOUND:
            msg += "Path does not exist.";
            break;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            msg += "Not Enough Memory";
            break;
        case ERROR_DISK_FULL:
        case ERROR_HANDLE_DISK_FULL:
            msg += "Disk is Full";
            break;
        case ERROR_WRITE_FAULT:
            msg += "Write Fault";
            break;
        case ERROR_READ_FAULT:
            msg += "Read Fault";
            break;
        case ERROR_CRC:
            msg += "Cyclic Redundancy Check - Possible Hardware Failure";
            break;
        case ERROR_NOT_READY:
            msg += "The device is not ready. Is the connection stable?";
            break;
        case ERROR_DEVICE_NOT_CONNECTED:
            msg += "The device has been disconnected. Is the connection stable?";
            break;
        case ERROR_WORKING_SET_QUOTA:
            msg += "Insufficient quota. System may be low on memory.";
            break;
        case ERROR_INVALID_PARAMETER:
            msg += "Invalid parameter. This can happen if the sector alignment is too small.";
            break;
        default:
            msg += "Unknown Error, See:\nhttp://msdn.microsoft.com/en-us/library/ms681381(VS.85).aspx";
    }
    throw FileException(errorcode, function, std::move(path), std::move(msg));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
RawFile::~RawFile(){
    close(m_persistent);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
void RawFile::open(Mode mode){
    switch (mode){
    case CREATE:
        m_filehandle = CreateFileW(
            m_wpath.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_ALWAYS,
            m_raw_io ? FILE_FLAG_NO_BUFFERING : FILE_FLAG_WRITE_THROUGH,
            nullptr
        );
        if (m_filehandle == INVALID_HANDLE_VALUE){
            DWORD errorcode = GetLastError();
            handle_error(errorcode, "RawFile()", m_path, "Unable to create file.");
        }

        try{
            set_size(0);
        }catch (...){
            close(false);
            throw;
        }
        break;

    case OPEN_READONLY:
        m_filehandle = CreateFileW(
            m_wpath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            m_raw_io ? FILE_FLAG_NO_BUFFERING : 0,
            nullptr
        );
        if (m_filehandle == INVALID_HANDLE_VALUE){
            DWORD errorcode = GetLastError();
            handle_error(errorcode, "RawFile()", m_path, "Unable to open file.");
        }
        break;

    case OPEN_READWRITE:
        m_filehandle = CreateFileW(
            m_wpath.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            m_raw_io ? FILE_FLAG_NO_BUFFERING : FILE_FLAG_WRITE_THROUGH,
            nullptr
        );
        if (m_filehandle == INVALID_HANDLE_VALUE){
            DWORD errorcode = GetLastError();
            handle_error(errorcode, "RawFile()", m_path, "Unable to open file.");
        }
        break;

    }
}
RawFile::RawFile(
    ukL_t alignment_k,
    std::string path, Mode mode,
    bool persistent, bool raw_io
)
    : AlignedAccessFile(alignment_k, std::move(path))
    , m_wpath(StringTools::utf8_to_wstr(m_path))
    , m_persistent(persistent)
    , m_raw_io(raw_io)
{
    open(mode);
}
RawFile::RawFile(
    ukL_t alignment_k,
    ufL_t bytes, std::string path,
    bool persistent, bool raw_io
)
    : AlignedAccessFile(alignment_k, std::move(path))
    , m_wpath(StringTools::utf8_to_wstr(m_path))
    , m_persistent(persistent)
    , m_raw_io(raw_io)
{
    m_filehandle = CreateFileW(
        m_wpath.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_ALWAYS,
        raw_io ? FILE_FLAG_NO_BUFFERING : FILE_FLAG_WRITE_THROUGH,
        nullptr
    );
    if (m_filehandle == INVALID_HANDLE_VALUE){
        DWORD errorcode = GetLastError();
        handle_error(errorcode, "RawFile()", m_path, "Unable to create file.");
    }

    try{
        set_size(bytes);

        //  Set valid data
        if (SetFileValidData(m_filehandle, (LONGLONG)bytes)){
            return;
        }
        warn_SetFileValidData_fail(m_path);

    }catch (...){
        close(false);
        throw;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Misc.
RawFile::operator bool() const{
    return m_filehandle != INVALID_HANDLE_VALUE;
}
void RawFile::flush(){
    //  Flushing is not necessary since FILE_FLAG_NO_BUFFERING implies no caching.
    //  https://msdn.microsoft.com/en-us/library/windows/desktop/aa364439%28v=vs.85%29.aspx
    if (!m_raw_io){
        FlushFileBuffers(m_filehandle);
    }
}
void RawFile::close(bool keep_file){
    if (m_filehandle == INVALID_HANDLE_VALUE){
        return;
    }

    //  Flush
    flush();

    //  Close
    if (!CloseHandle(m_filehandle)){
        Console::Warning("RawFile::close(): Unable to close file.");
    }
    m_filehandle = INVALID_HANDLE_VALUE;

    //  Delete
    if (!keep_file && _wremove(m_wpath.c_str())){
        Console::Warning("RawFile::close(): Unable to delete file.");
    }

    m_path.clear();
    m_wpath.clear();
}
void RawFile::close_and_set_size(ufL_t bytes){
    if (m_filehandle == INVALID_HANDLE_VALUE){
        return;
    }

    //  Flush
    flush();

    //  Close
    if (!CloseHandle(m_filehandle)){
        Console::Warning("RawFile::close_and_set_size(): Unable to close file.");
    }
    m_filehandle = INVALID_HANDLE_VALUE;

    std::string path = std::move(m_path);
    std::wstring wpath = std::move(m_wpath);
    m_path.clear();
    m_wpath.clear();

    //  At this point forward, the object is closed. No matter what happens next,
    //  the object will remain in the closed state.

    //  Re-open
    HANDLE handle = CreateFileW(
        wpath.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_WRITE_THROUGH,
        nullptr
    );
    if (handle == INVALID_HANDLE_VALUE){
        DWORD error = GetLastError();
        handle_error(error, "RawFile::close_and_set_size()", path, "Unable to reopen file.");
    }

    try{
        LARGE_INTEGER t;
        t.QuadPart = (LONGLONG)bytes;
        if (!SetFilePointerEx(handle, t, nullptr, FILE_BEGIN)){
            DWORD error = GetLastError();
            handle_error(error, "RawFile::close_and_set_size()", path, "SetFilePointerEx() failed.");
        }
        if (!SetEndOfFile(handle)){
            DWORD error = GetLastError();
            handle_error(error, "RawFile::close_and_set_size()", path, "SetEndOfFile() failed.");
        }

        //  Close
        if (!CloseHandle(handle)){
            Console::Warning("RawFile::close_and_set_size(): Unable to close file.");
        }
    }catch (...){
        //  Close
        if (!CloseHandle(handle)){
            Console::Warning("RawFile::close_and_set_size(): Unable to close file.");
        }
        throw;
    }
}
void RawFile::rename(std::string path, bool readonly){
    if (m_filehandle == INVALID_HANDLE_VALUE){
        throw FileException("RawFile::rename()", std::move(path), "File isn't open.");
    }

    std::string old_path = std::move(m_path);
    std::wstring old_wpath = std::move(m_wpath);
    std::wstring new_wpath = StringTools::utf8_to_wstr(path);
    close(true);

    //  Rename it
    if (_wrename(old_wpath.c_str(), new_wpath.c_str())){
        //  Check error
        errno_t err;
        _get_errno(&err);

        //  Failed because of something other than file already exists.
        if (err != EEXIST){
            throw FileException(
                err, "RawFile::rename()",
                std::move(old_path),
                "Unable to rename file."
            );
        }

        //  File already exists. Remove the existing one.
        if (_wremove(new_wpath.c_str())){
            throw FileException(
                err, "RawFile::rename()",
                std::move(old_path),
                "Unable to rename file because the existing one can't be deleted."
            );
        }

        //  Try one last time.
        if (_wrename(old_wpath.c_str(), new_wpath.c_str())){
            //  Failed again
            _get_errno(&err);
            throw FileException(
                err, "RawFile::rename()",
                std::move(old_path),
                "Unable to rename file."
            );
        }
    }

    m_path = std::move(path);
    m_wpath = std::move(new_wpath);
    open(readonly ? OPEN_READONLY : OPEN_READWRITE);
}
void RawFile::set_size(ufL_t bytes){
    if (m_filehandle == INVALID_HANDLE_VALUE){
        throw InvalidParametersException("RawFile::set_size()", "File isn't open.");
    }
    LARGE_INTEGER t;
    t.QuadPart = (LONGLONG)bytes;
    if (!SetFilePointerEx(m_filehandle, t, nullptr, FILE_BEGIN)){
        DWORD error = GetLastError();
        handle_error(error, "RawFile::set_size()", m_path, "SetFilePointerEx() failed.");
    }
    if (!SetEndOfFile(m_filehandle)){
        DWORD error = GetLastError();
        handle_error(error, "RawFile::set_size()", m_path, "SetEndOfFile() failed.");
    }
}
upL_t RawFile::pick_buffer_size(upL_t bytes) const{
    //  Speculative work-around for quota errors (1453) that may occur for large
    //  accesses when the system is low on memory.
    if (bytes <= CHECK_MEM_THRESHOLD){
        return bytes;
    }
    upL_t buffer = Environment::GetFreePhysicalMemory();
    buffer = Alignment::align_int_down_k(m_alignment_k, buffer / 2);
    buffer = std::max(buffer, CHECK_MEM_THRESHOLD);
    buffer = std::min(buffer, bytes);
    buffer = std::min(buffer, MAX_IO_BYTES);
    return buffer;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Read/Write
upL_t RawFile::load(void* data, ufL_t offset, upL_t bytes, bool throw_on_partial){
    if (m_filehandle == INVALID_HANDLE_VALUE){
        throw InvalidParametersException("RawFile::load()", "File isn't open.");
    }
    check_alignment(data, offset, bytes);
    if (bytes == 0){
        return 0;
    }

    //  Set File Pointer
    LARGE_INTEGER t;
    t.QuadPart = (LONGLONG)offset;
    if (!SetFilePointerEx(m_filehandle, t, nullptr, FILE_BEGIN)){
        DWORD errorcode = GetLastError();
        handle_error(errorcode, "RawFile::load()", m_path, "SetFilePointerEx() failed.");
    }

    upL_t block_size = pick_buffer_size(bytes);

    //  Read
    upL_t processed = 0;
    while (bytes > 0){
        upL_t current = std::min(bytes, block_size);

        DWORD IO_bytes;
        bool ret = !ReadFile(m_filehandle, data, (DWORD)current, &IO_bytes, nullptr);
        if (ret){
            DWORD errorcode = GetLastError();
            handle_error(errorcode, "RawFile::load()", m_path, "ReadFile() failed.");
        }

        processed += IO_bytes;
        if (IO_bytes != current){
            if (throw_on_partial){
                DWORD errorcode = GetLastError();
                handle_error(
                    errorcode,
                    "RawFile::load()", m_path,
                    "Incomplete Read: " + std::to_string(IO_bytes) + " / " + std::to_string(current)
                );
            }
            break;
        }

        data = (char*)data + current;
        bytes -= current;
    }

    return processed;
}
upL_t RawFile::store(const void* data, ufL_t offset, upL_t bytes, bool throw_on_partial){
    if (m_filehandle == INVALID_HANDLE_VALUE){
        throw InvalidParametersException("RawFile::store()", "File isn't open.");
    }
    check_alignment(data, offset, bytes);
    if (bytes == 0){
        return 0;
    }

    //  Set File Pointer
    LARGE_INTEGER t;
    t.QuadPart = (LONGLONG)offset;
    if (!SetFilePointerEx(m_filehandle, t, nullptr, FILE_BEGIN)){
        DWORD errorcode = GetLastError();
        handle_error(errorcode, "RawFile::store()", m_path, "SetFilePointerEx() failed.");
    }

    upL_t block_size = pick_buffer_size(bytes);

    //  Write
    upL_t processed = 0;
    while (bytes > 0){
        upL_t current = std::min(bytes, block_size);

        DWORD IO_bytes;
        bool ret = !WriteFile(m_filehandle, data, (DWORD)current, &IO_bytes, nullptr);
        if (ret){
            DWORD errorcode = GetLastError();
            handle_error(errorcode, "RawFile::store()", m_path, "WriteFile() failed.");
        }

        processed += IO_bytes;
        if (IO_bytes != current){
            if (throw_on_partial){
                DWORD errorcode = GetLastError();
                handle_error(
                    errorcode,
                    "RawFile::store()", m_path,
                    "Incomplete Write: " + std::to_string(IO_bytes) + " / " + std::to_string(current)
                );
            }
            break;
        }

        data = (char*)data + current;
        bytes -= current;
    }

    return processed;
}
void RawFile::load(void* data, ufL_t offset, upL_t bytes, void* P, upL_t PL){
    load(data, offset, bytes, true);
}
void RawFile::store(const void* data, ufL_t offset, upL_t bytes, void* P, upL_t PL){
    store(data, offset, bytes, true);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
