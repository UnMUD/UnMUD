// MUD Programming
// Ron Penton
// (C)2003
// BasicLibFiles.h - This file contains file functions


#ifndef BASICLIBFILES_H
#define BASICLIBFILES_H

#include "BasicLibTypes.h"
#include <string>
#include <set>

namespace BasicLib
{

    typedef std::set<std::string> filelist;
    filelist GetFileList( const std::string p_directory );


} // end namespace BasicLib


#endif

