// MUD Programming
// Ron Penton
// (C)2003
// BasicLibFiles.h - This file contains file functions


#include "BasicLibFiles.h"

#ifdef WIN32
#   include "Windows.h"
#else
#   include <sys/types.h>
#   include <dirent.h>
#endif

namespace BasicLib
{

    filelist GetFileList( const std::string p_directory )
    {
        filelist s;
#   ifdef WIN32
        std::string mask = p_directory + "*";
        WIN32_FIND_DATA d;
        HANDLE h = FindFirstFile( mask.c_str(), &d );

        if( h != INVALID_HANDLE_VALUE )
        {
            bool next = true;
            while( next )
            {
                s.insert( p_directory + d.cFileName );
                next = FindNextFile( h, &d );
            }
        }
#   else
        DIR* d = opendir( p_directory.c_str() );
        dirent* ent = readdir( d );

        while( ent != 0 )
        {
            s.insert( p_directory + ent->d_name );
            ent = readdir( d );
        }
#   endif
        return s;
    }

} // end namespace BasicLib
