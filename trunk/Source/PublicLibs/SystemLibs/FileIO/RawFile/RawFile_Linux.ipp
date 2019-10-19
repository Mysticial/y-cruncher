/* RawFile_Linux.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/31/2011
 * Last Modified    : 03/24/2018
 * 
 *      Please read the comments for "RawFile_Windows.ipp" before you
 *  continue reading here.
 * 
 *  Windows is "barely" able to achieve zero-overhead disk access thanks to two
 *  key WinAPI features. Unfortunately, Linux seems to fall slightly short.
 * 
 * 
 *  FILE_FLAG_NO_BUFFERING -> O_DIRECT
 * 
 *      On Windows, FILE_FLAG_NO_BUFFERING is guaranteed in almost all contexts.
 *  And when it succeeds, there really is no buffering. All I/O is effectively
 *  DMA with zero CPU and memory overhead.
 * 
 *  The Linux/POSIX equivalent is O_DIRECT. However, it does not provide the
 *  same guarantee as FILE_FLAG_NO_BUFFERING does in Windows.
 * 
 *  O_DIRECT on Linux is often not supported and is often ignored. So it's a
 *  hint rather than a guarantee. So even when O_DIRECT succeeds, internal
 *  buffering is often still observed along with CPU consumption. This hurts
 *  performance when disk I/O is overlapped with CPU-intensive computation.
 * 
 *  When O_DIRECT fails, the only fallback is to retry without O_DIRECT.
 *  Fortunately, the disk cache on Linux is better behaved than on Windows.
 *  So while it works without O_DIRECT, the performance still isn't good.
 * 
 * 
 *  SetFileValidData() -> fallocate() / ftruncate()
 * 
 *      On Windows, "SetFileValidData()" is very well behaved. When it succeeds,
 *  it truly allocates a file without zeroing. It even exposes all the raw data
 *  in its place - which is why it needs admin privileges.
 * 
 *  Unfortunately, there appears to be no equivalent in Linux. The closest thing
 *  is "fallocate()". However, it does not skip the zeroing. Instead, it seems
 *  to rely either on extents or sparse files to avoid the actual zeroing.
 * 
 *  Furthermore, "fallocate()" isn't reliably supported. "ftruncate()" is an
 *  alternative that usually results in a sparse file.
 * 
 *  Sparse files is an acceptable implementation. But it is not optimal since it
 *  may fragment the file. Normally, fragmentation isn't an issue since the OS
 *  buffers it and tries to combine things together. But this isn't possible
 *  because we are using O_DIRECT and the files will be many times larger than
 *  ram anyway.
 * 
 *  The current approach for file allocation is to try "fallocate()" first, then
 *  fall back to "ftruncate()".
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/Exceptions/SystemException.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "RawFile.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Errors
struct FailPrinter_O_DIRECT{
    FailPrinter_O_DIRECT(const std::string& path){
        Console::Warning(
            "O_DIRECT has failed. Expect performance degradation.\n"
            "If the file system has journaling enabled, try disabling it.\n"
        );
        Console::println_labelc("Path: ", path, 'Y');
        Console::Warning("Further messages for this warning will be suppressed.\n");
    }
};
struct FailPrinter_fallocate{
    FailPrinter_fallocate(const std::string& path){
        Console::Warning(
            "fallocate() is not supported on this volume. Expect performance degradation.\n"
        );
        Console::println_labelc("Path: ", path, 'Y');
        Console::Warning("Further messages for this warning will be suppressed.\n");
    }
};
struct FailPrinter_ftruncate{
    FailPrinter_ftruncate(const std::string& path){
        Console::Warning(
            "ftruncate() has failed. Expect performance degradation.\n"
        );
        Console::println_labelc("Path: ", path, 'Y');
        Console::Warning("Further messages for this warning will be suppressed.\n");
    }
};
void warn_O_DIRECT_fail(const std::string& path){
    static FailPrinter_O_DIRECT printer(path);
}
void warn_fallocate_fail(int errorcode, const std::string& path){
    switch (errorcode){
    case EOPNOTSUPP:{
        static FailPrinter_fallocate printer(path);
        return;
    }
    case ENOSPC:
        Console::Warning("fallocate() has failed. Disk is full.\n");
        break;
    default:
        Console::Warning(
            "fallocate() has failed. (errno = " + std::to_string(errorcode) +  ")  Expect performance degradation.\n"
        );
    }
    Console::println_labelc("Path: ", path, 'Y');
}
void warn_ftruncate_fail(const std::string& path){
    static FailPrinter_ftruncate printer(path);
}
void handle_error(int errorcode, const char* function, std::string path, std::string msg){
    msg += "\n\n";
    switch (errorcode){
        case EACCES:
            msg += "Access Denied";
            break;
        case ENOENT:
            msg += "Path does not exist.";
            break;
        case ENOMEM:
            msg += "Not Enough Memory";
            break;
        case ENOSPC:
            msg += "Disk is Full";
            break;
        case EIO:
            msg += "Hardware Error";
            break;
        case EMFILE:
            msg += "Too many open files. Try increasing the ulimit for file handles.";
            break;
        case EINVAL :
            msg += "Invalid parameter. This can happen if the sector alignment is too small.";
            break;
        default:
            msg += "Unknown POSIX or C/C++ Error";
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
        do{
            m_filehandle = ::open(
                m_path.c_str(),
                O_CREAT | O_RDWR | O_LARGEFILE | (m_raw_io ? O_DIRECT : 0),
                S_IRUSR | S_IWUSR
            );
            if (m_filehandle != -1){
                break;
            }

            if (errno == EINVAL){
                warn_O_DIRECT_fail(m_path);
                m_filehandle = ::open(
                    m_path.c_str(),
                    O_CREAT | O_RDWR | O_LARGEFILE,
                    S_IRUSR | S_IWUSR
                );
            }
            if (m_filehandle != -1){
                break;
            }

            int errorcode = errno;
            handle_error(errorcode, "RawFile()", m_path, "Unable to create file.");
        }while (false);

        try{
            set_size(0);
        }catch (...){
            close(false);
            throw;
        }
        break;

    case OPEN_READONLY:
        do{
            m_filehandle = ::open(
                m_path.c_str(),
                O_RDONLY | O_LARGEFILE | (m_raw_io ? O_DIRECT : 0)
            );
            if (m_filehandle != -1){
                break;
            }

            if (errno == EINVAL){
                warn_O_DIRECT_fail(m_path);
                m_filehandle = ::open(
                    m_path.c_str(),
                    O_RDONLY | O_LARGEFILE
                );
            }
            if (m_filehandle != -1){
                break;
            }

            int errorcode = errno;
            handle_error(errorcode, "RawFile()", m_path, "Unable to open file.");
        }while (false);
        break;

    case OPEN_READWRITE:
        do{
            m_filehandle = ::open(
                m_path.c_str(),
                O_RDWR | O_LARGEFILE | (m_raw_io ? O_DIRECT : 0)
            );
            if (m_filehandle != -1){
                break;
            }

            if (errno == EINVAL){
                warn_O_DIRECT_fail(m_path);
                m_filehandle = ::open(
                    m_path.c_str(),
                    O_RDWR | O_LARGEFILE
                );
            }
            if (m_filehandle != -1){
                break;
            }

            int errorcode = errno;
            handle_error(errorcode, "RawFile()", m_path, "Unable to open file.");
        }while (false);
        break;

    }
}
RawFile::RawFile(
    ukL_t alignment_k,
    std::string path, Mode mode,
    bool persistent, bool raw_io
)
    : AlignedAccessFile(alignment_k, std::move(path))
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
    , m_persistent(persistent)
    , m_raw_io(raw_io)
{
    do{
        m_filehandle = ::open(
            m_path.c_str(),
            O_CREAT | O_RDWR | O_LARGEFILE | (raw_io ? O_DIRECT : 0),
            S_IRUSR | S_IWUSR
        );
        if (m_filehandle != -1){
            break;
        }

        if (errno == EINVAL){
            warn_O_DIRECT_fail(m_path);
            m_filehandle = ::open(
                m_path.c_str(),
                O_CREAT | O_RDWR | O_LARGEFILE,
                S_IRUSR | S_IWUSR
            );
        }
        if (m_filehandle != -1){
            break;
        }

        int errorcode = errno;
        handle_error(errorcode, "RawFile()", m_path, "Unable to create file.");
    }while (false);

    try{
        set_size(bytes);

        //  Try fallocate().
        if (!fallocate(m_filehandle, 0, 0, bytes)){
            return;
        }
        warn_fallocate_fail(errno, m_path);

        //  If we can't allocate up front, the next best thing is a sparse file.
        if (!ftruncate(m_filehandle, bytes)){
            return;
        }
        warn_ftruncate_fail(m_path);

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
    return m_filehandle != -1;
}
void RawFile::flush(){
    if (fsync(m_filehandle) == -1){
        Console::Warning("RawFile::close(): Unable to flush file.");
    }
}
void RawFile::close(bool keep_file){
    if (m_filehandle == -1){
        return;
    }

    //  Flush
    flush();

    //  Close
    if (::close(m_filehandle) == -1){
        Console::Warning("RawFile::close(): Unable to close file.");
    }
    m_filehandle = -1;

    //  Delete
    if (!keep_file && remove(m_path.c_str()) != 0){
        Console::Warning("RawFile::close(): Unable to delete file.");
    }

    m_path.clear();
}
void RawFile::close_and_set_size(ufL_t bytes){
    if (m_filehandle == -1){
        return;
    }

    //  Flush
    flush();

    //  Close
    if (::close(m_filehandle) == -1){
        Console::Warning("RawFile::close_and_set_size(): Unable to close file.");
    }
    m_filehandle = -1;

    std::string path = std::move(m_path);
    m_path.clear();

    //  At this point forward, the object is closed. No matter what happens next,
    //  the object will remain in the closed state.

    int handle = ::open(
        path.c_str(),
        O_RDWR | O_LARGEFILE
    );
    if (handle == -1){
        int errorcode = errno;
        handle_error(errorcode, "RawFile::close_and_set_size()", path, "Unable to open file.");
    }

    try{
        if (ftruncate(handle, bytes) != 0){
            int err = errno;
            if (::close(handle) == -1){
                Console::Warning("RawFile::close_and_set_size(): Unable to close file.");
            }
            throw SystemException("RawFile::close_and_set_size()", "ftruncate() failed.", err);
        }

        if (::close(handle) == -1){
            Console::Warning("RawFile::close_and_set_size(): Unable to close file.");
        }
    }catch (...){
        if (::close(handle) == -1){
            Console::Warning("RawFile::close_and_set_size(): Unable to close file.");
        }
        throw;
    }
}
void RawFile::rename(std::string path, bool readonly){
    if (m_filehandle == -1){
        throw FileException("RawFile::rename()", std::move(path), "File isn't open.");
    }

    std::string old_path = std::move(m_path);
    close(true);

    //  Rename it
    if (::rename(old_path.c_str(), path.c_str())){
        //  Failed because of something other than file already exists.
        if (errno != EEXIST){
            int errorcode = errno;
            handle_error(errorcode, "RawFile::rename()", old_path, "Unable to rename file.");
        }

        //  File already exists. Remove the existing one.
        if (remove(path.c_str())){
            int errorcode = errno;
            handle_error(errorcode, "RawFile::rename()", old_path, "Unable to rename file because the existing one can't be deleted.");
        }

        //  Try one last time.
        if (::rename(old_path.c_str(), path.c_str())){
            int errorcode = errno;
            handle_error(errorcode, "RawFile::rename()", old_path, "Unable to rename file.");
        }
    }

    m_path = std::move(path);
    open(readonly ? OPEN_READONLY : OPEN_READWRITE);
}
void RawFile::set_size(ufL_t bytes){
    if (m_filehandle == -1){
        throw InvalidParametersException("RawFile::set_size()", "File isn't open.");
    }
    if (ftruncate(m_filehandle, bytes) != 0){
        throw SystemException("RawFile::set_size()", "ftruncate() failed.", errno);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Read/Write
upL_t RawFile::load(void* data, ufL_t offset, upL_t bytes, bool throw_on_partial){
    if (m_filehandle == -1){
        throw InvalidParametersException("RawFile::load()", "File isn't open.");
    }
    check_alignment(data, offset, bytes);
    if (bytes == 0){
        return 0;
    }

    //  Set File Pointer
    if (lseek64(m_filehandle, offset, SEEK_SET) == -1){
        int errorcode = errno;
        handle_error(errorcode, "RawFile::load()", m_path, "lseek64() failed.");
    }

    //  Read
    upL_t processed = 0;
    while (bytes > 0){
        upL_t current = std::min(bytes, MAX_IO_BYTES);

        spL_t IO_bytes = read(m_filehandle, data, current);
        if (IO_bytes == -1){
            int errorcode = errno;
            handle_error(errorcode, "RawFile::load()", m_path, "read() failed.");
        }

        processed += IO_bytes;
        if ((upL_t)IO_bytes != current){
            if (throw_on_partial){
                int errorcode = errno;
                handle_error(
                    errorcode, "RawFile::load()", m_path,
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
    if (m_filehandle == -1){
        throw InvalidParametersException("RawFile::store()", "File isn't open.");
    }
    check_alignment(data, offset, bytes);
    if (bytes == 0){
        return 0;
    }

    //  Set File Pointer
    if (lseek64(m_filehandle, offset, SEEK_SET) == -1){
        int errorcode = errno;
        handle_error(errorcode, "RawFile::store()", m_path, "lseek64() failed.");
    }

    //  Write
    upL_t processed = 0;
    while (bytes > 0){
        upL_t current = std::min(bytes, MAX_IO_BYTES);

        spL_t IO_bytes = write(m_filehandle, data, current);
        if (IO_bytes == -1){
            int errorcode = errno;
            handle_error(errorcode, "RawFile::store()", m_path, "write() failed.");
        }

        processed += IO_bytes;
        if ((upL_t)IO_bytes != current){
            if (throw_on_partial){
                int errorcode = errno;
                handle_error(
                    errorcode, "RawFile::store()", m_path,
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
