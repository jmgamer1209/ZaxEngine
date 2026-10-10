module;
#include <string>
#include <initializer_list>

export module Debug;

 export namespace Debug
{
	 void Log(const std::string& log);
	 void Log(std::initializer_list<std::string> list);

	 void Log(const std::wstring& log);
	 //void Log(std::initializer_list<std::wstring> list);

	 void Log(const int log);
	 void Log(const double log);
};

