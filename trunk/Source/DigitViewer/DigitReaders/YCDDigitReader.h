/* YCDDigitReader.h - .ycd Reader Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 01/09/2018
 * 
 * 
 * 
 *  This file implements the digit reader for .ycd version 1.1.0.
 * 
 *  This reader is backwards compatible and is able to read v1.0.0 .ycd files.
 *  The old digit reader will not be able to read v1.1.0 files.
 * 
 * 
 * 
 *  Compressed File version 1.1.0:
 *      The primary motivation of v1.1.0 is for a compressed digit file to be
 *  agnostic to the computation that produced it. One of the biggest weaknesses
 *  of v1.0.0 is that you can't "append" new digits to an existing set of
 *  compressed .ycd files.
 * 
 *  In order to get around this problem, v1.1.0 has two major changes:
 * 
 *  First, v1.1.0 removes all of the information that is specific to the
 *  computation that produces the .ycd file.
 * 
 *      -   NameLong:
 *      -   NameShort:
 *      -   Program:
 *      -   Version:
 *      -   TotalBlocks:
 * 
 *  Second, v1.1.0 adds support for extended files where each .ycd file does
 *  not know how many digits are in the set of .ycd files.
 * 
 * 
 * 
 *  Extended Files:
 *      Normally, a .ycd file has the "total_digits" parameter that specifies
 *  how many digits are stored in the complete set of .ycd files. This number
 *  tells the digit viewer not to attempt to read any digits beyond this limit.
 * 
 *      When "total_digits" is zero, the .ycd file becomes and "extended" file.
 *  This means that there is no information on how many digits are present. The
 *  digit viewer will assume that there are infinitely many digits and will not
 *  block the user from accessing digits that are not present.
 * 
 *      If you a set of digits are stored into the "extended" format, you can
 *  later on directly add more .ycd files without modifying the existing ones.
 *  The v1.1.0 digit viewer will be able to combine all the existing and new
 *  .ycd files as one without having to reprocess everything (as was the case
 *  in v1.0.0).
 * 
 *  Example:
 *      Compute 5 billion digits of Pi. Save as following format:
 *          Pi - Dec - 0.ycd  (extended format)
 *          Pi - Dec - 1.ycd  (extended format)
 *          Pi - Dec - 2.ycd  (extended format)
 *          Pi - Dec - 3.ycd  (extended format)
 *          Pi - Dec - 4.ycd  (extended format)
 * 
 *      These files are then distributed. The digit viewer is able to handle
 *  these together.
 *
 *      Now a later computation produces 5 more billion digits:
 *          Pi - Dec - 5.ycd  (extended format)
 *          Pi - Dec - 6.ycd  (extended format)
 *          Pi - Dec - 7.ycd  (extended format)
 *          Pi - Dec - 8.ycd  (extended format)
 *          Pi - Dec - 9.ycd  (extended format)
 * 
 *      In v1.0.0, you can't just be place these new files (5 - 9) into the same
 *  folder as 0 - 5. The digit viewer won't be able to treat them as one
 *  The v1.1.0 digit viewer can do it if all the files are of extended format.
 * 
 * 
 * 
 *  Partial Files:
 *      Extended format .ycd files only work if the file is complete. That last
 *  (possibly incomplete) file of a set cannot be in extended format. So often
 *  times, the very last .ycd file will be the only one that's not in extended
 *  format.
 *  
 *  Example: 51 million digits of Pi. 10 million digits per file:
 *          Pi - Dec - 0.ycd  (extended format, total_digits = 0)
 *          Pi - Dec - 1.ycd  (extended format, total_digits = 0)
 *          Pi - Dec - 2.ycd  (extended format, total_digits = 0)
 *          Pi - Dec - 3.ycd  (extended format, total_digits = 0)
 *          Pi - Dec - 4.ycd  (extended format, total_digits = 0)
 *          Pi - Dec - 5.ycd  (normal format  , total_digits = 51000000)
 *      
 * 
 * 
 *  Partial List of Rules:
 *      1.  Extended File: total_digits = 0
 *      2.  Extended File: Must have "block_size" digits. (less than that will result in read error)
 *      3.  Normal File: total_digits > 0
 *      4.  All files must be the same format version.
 *      5.  All files must be the same base.
 *      6.  All files must have the same "block_size" parameter.
 *      7.  The leading zeros in the file ID # are optional. (with some restrictions)
 *          As the ID increases, the # of digits must remain the same or increase by at most 1.
 * 
 *      Example: (allowed)          Example: (allowed)
 *          Pi - Dec - 00.ycd           Pi - Dec - 0.ycd
 *          Pi - Dec - 01.ycd           Pi - Dec - 1.ycd
 *          Pi - Dec - 02.ycd           Pi - Dec - 2.ycd
 *          Pi - Dec - 10.ycd           Pi - Dec - 10.ycd
 *          Pi - Dec - 11.ycd           Pi - Dec - 11.ycd
 * 
 *      Example: (not allowed)      Example: (not allowed)
 *          Pi - Dec - 000.ycd          Pi - Dec - 0.ycd
 *          Pi - Dec - 01.ycd           Pi - Dec - 001.ycd
 *          Pi - Dec - 002.ycd          Pi - Dec - 002.ycd
 *          Pi - Dec - 10.ycd           Pi - Dec - 010.ycd
 *          Pi - Dec - 11.ycd           Pi - Dec - 011.ycd
 * 
 * 
 * 
 *  Version 1.1.0 Digit Viewer:
 *      To handle extended files, a handle object "YCDReader" has two states:
 *          1.  Extended Mode
 *          2.  Normal Mode
 * 
 *      When opening an extended file, the total_digits is unknown and the
 *  handle object is in "Extended Mode". The digit viewer assumes there are
 *  infinite digits. When the user requests a digit, the digit viewer will
 *  calculate the file ID # that holds that digit and will return an error if
 *  that file doesn't exist.
 * 
 *      As soon as the digit viewer opens a .ycd file that has a non-zero
 *  "total_digit" parameter (a non-extended), that parameter is recorded and
 *  the handle changes to "Normal Mode". Normal mode is the same as in v1.0.0.
 *  attempting to read digits beyond the limit will return an error.
 *  Encountering a second non-extended file with a conflicting "total_digit"
 *  parameter will return an error.
 * 
 *      To summarize, the v1.1.0 digit viewer "try" to enter "Normal Mode"
 *  whenever possible and as soon as possible.
 * 
 * 
 * 
 *  Multiple Path Support:
 *      The v1.1.0 digit viewer provides one additional feature. The .ycd files
 *  no longer need to be in the same path. They can be spread out accross
 *  multiple paths. This avoids the need to build RAID arrays just to work with
 *  large datasets that don't fit onto a single hard drive. (which was a huge
 *  problem when handling 10 trillion digits of Pi with only 2TB drives...)
 *      This is implemented by allowing the digit viewer to search all the paths
 *  for each file.
 * 
 * 
 * 
 *  Update 7/15/2013: The following information is out of date.
 * 
 *  Optimizing File Locating: (Implemented in "ycr_dio_getfilepath()")
 *      The multiple path support and the leading zero tolerance leads to a
 *  performance problem. When searching for a file, the digit viewer must search
 *  all paths and all possible amount of leading zeros in the ID #.
 * 
 *      This is dealt with using two optimizations. The first is path prediction.
 *  In the majority of cases, adjacent groups of .ycd files will be on the same
 *  path. Therefore, the digit viewer will search the most recently use path first.
 *  Only if it fails will it iterate through all the paths.
 *      The second optimization is limiting the possible cases of leading zeros
 *  the .ycd files are allowed to have. The digit viewer will check for cases
 *  where the # of digits (after inserting leading zeros) is no more than 1
 *  longer than the maximum # of digits seen so far.
 *      When a handle is created, the # of digits is stored in "max_id_length".
 *  When searching for file, the digit viewer will keep padding zeros until the
 *  digit length is (max_id_length + 1) before it gives up. Each time a file is
 *  successfully found, "max_id_length" is updated accordingly if the new file
 *  has a longer ID digit-length.
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_YCDDigitReader_H
#define ydv_DigitViewer_YCDDigitReader_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include "DigitViewer/InjectableBuffer.h"
#include "DigitReader.h"
#include "YCDFileReader.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class YCDReader : public DigitReader{
public:
    //  Open a new reader. You must pass it the path of any one of the .ycd files.
    //  From there it will take care of the rest.
    //  For ASCII char output, use:   raw = false
    //  For raw integer output, use:  raw = true
    YCDReader(
        std::string path,   //  UTF-8
        bool raw = false,
        InjectableBuffer<u64_t> buffer = InjectableBuffer<u64_t>(YC_DIGITREADER_DEFAULT_BUFFER / sizeof(u64_t))
    );

    virtual void set_native_buffer(InjectableBuffer<u64_t> buffer) override;
    virtual void clear_native_buffer() override;

    virtual const char* get_extension() const{
        return "ycd";
    }

    //  Refer to the abstract class for documentation of these virtual functions.
    virtual void        print           () const;
    virtual int         get_radix       () const;
    virtual uiL_t       get_digits      () const;
    virtual void        set_raw         (bool raw);
    virtual bool        check_range     (uiL_t start, uiL_t end);
    virtual std::string get_first_digits(upL_t L);
    virtual void        read            (uiL_t pos, char* str, upL_t digits);

    //  Add a new search path for which this reader will search in an attempt
    //  to find .ycd files associated with this digit stream.
    void                add_search_path     (std::string path);

    const std::string&  get_name            () const;
    ufL_t               get_digits_per_file () const;
    void                print_paths         () const;
    upL_t               get_num_paths       () const;

private:
    std::string m_name;                 //  File name
    upL_t m_max_id_length;              //  Max length of id
    std::vector<std::string> m_paths;   //  Search Paths

    int m_radix;                        //  Radix of the digits. (10 or 16)
    void (*fp_convert)(char*, const u64_t*, upL_t); //  Function pointer for digit conversion.
    upL_t m_digits_per_word;

    std::string m_first_digits;         //  First digits

    //  Total # of digits in all the .ycd files.
    //  Total # of digits = 0 indicates extended/unbounded file
    uiL_t m_total_digits;

    ufL_t m_digits_per_file;            //  # of digits per compressed file
    ufL_t m_blocks_per_file;            //  # of 64-bit blocks per file

    YCDFileReader current_file;

    //  Binary Buffer
    InjectableBuffer<u64_t> m_buffer;

    virtual void reload();  //  Override
    void load_new_file(std::string path, uiL_t id);
    void set_current_file(uiL_t id);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
