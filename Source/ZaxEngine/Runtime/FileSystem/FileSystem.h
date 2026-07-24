#pragma once
#include "boost/filesystem/path.hpp"

namespace ZaxEngine::Runtime
{
	class FileSystem
	{
	public:
		
		FileSystem();
		
		void Initialize(boost::filesystem::path projectPath);
	};

}